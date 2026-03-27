#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 100
#define HEIGHT 38
#define WIDTH 130
// STRUCTS

// User struct for login/authentication
typedef struct {
    int userID;
    char username[101];
    unsigned long passwordHash; // When placed into TXT it will be a long + integer
    char role[30];   // "GP", "Specialist", "Patient"
    char name[101]; // Full name
    int questType;
    char answer[101];
} User;
typedef struct {
	int patientID;
	int userID; // if no userID should default add a new user ID
	char name[101];
	int age; // if age 30-79 cvdRisk is calculated
	char gender; // M or F, used for cvdRisk
	char contact[17];
	float bmi;
	char bmiCat[12];
	char bp[16]; // sys/dia mmHg, systolic blood pressure is parsed within calccvdRisk range: 90-200 mmHg
	float bloodSugar;
	char currentCVD; // Shows if patient currently has CVD, cvdRisk can only be calculated when patient has no known CVD
	float totalChol; // 130-320 mg/dL range, must conv to mmol to use for cvdRisk
	float hdlChol; // High density lipoprotein cholesterol 20-100 mg/dL, must conv to mmol to use for cvdRisk
	int eGFR; // Estimated glomerular filtration rate >0 mL/min/1.73m^2
	char htMed; // Y or N, when used for cvdRisk it is conv to 1 or 0
	char statins; // Y or N, when used for cvdRisk it is conv to 1 or 0
	char smoking; // Y or N, when used for cvdRisk it is conv to 1 or 0
	char diabetes; // Y or N, when used for cvdRisk it is conv to 1 or 0
	float creatinine; // For other information in diagnosis report
	char cvdFamily; // For other information in diagnosis report
	char diet; // For other information in diagnosis report
	char exercise; // For other information in diagnosis report
	char alcohol; // For other information in diagnosis report
	double cardioRisk;  // For cvdRisk
	char isDiagnosed; // Will show if patient has been diagnosed or not
	int isReferred; // patients should only be referred once
} Patient;
typedef struct {
	int referralID; // Referral ID
	int gpID; // GP user ID
	int patientID; // Patient's patientID
	char patientName[101]; // Patient Name
	int specialistID; // Specialist user ID
	char status[31]; // "Pending" "Accepted" "Completed" "Rejected"
	// maybe date?? pero we'll need to remake most to include date too for it to make sense T~T
	// reason for referral maybe wag na mas simple na referral system kahit medyo di nagmamake sense
} Referral;

// === Function Prototypes ===
void clearInput();
int getValidInput(void *input, int type, int min, int max, char limit1, char limit2, char limit3, char limit4);
int registerUser (User *users, int *userCount, Patient *patients, int patientCount);
User* loginUser (User *users, int userCount);
void hashPassword (const char *password, unsigned long *outputHash);
int saveAllUsersToFile (User *users, int userCount, const char *filename);
int loadUsersFromFile (User *users, const char *filename);
int forgotPassword (User *users, int userCount, const char *username);
int getUserID (User *users, Patient *patients);
void gpDashboard (User *currentUser, Patient *patients, int *patientCount, User *users, int userCount, Referral *referrals, int *referralCount, double data[][2]);
void specialistDashboard(User *currentUser, User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, double data[][2]);
void patientDashboard(User *currentUser, User *users, int userCount, Patient *patients, int *patientCount, Referral *referrals, int referralCount);
int findUserByID (User *users, int userCount, int input);
int findUserByName (User *users, int userCount, char *input);
void printUsers (User *users, int userCount, char *filter);
void printMainMenu (int height, int width);
void printTitle(int height, int width);
void clearScreen();
void inputText();
void centerText();
void printBorder();
void printDivider();
void printCentered(char* text);
void waitForInput();

Patient addPatient (User *currentUser, Patient *patients, int patientCount, User *users, int userCount);
void initPatient (Patient *patient);
void diagnosePatient (Patient *patient);
void showPatientDetails (Patient *currentPatient);
void showDiagnosisReport (Patient *currentPatient);
int getPatientID (Patient *patients, int patientCount);
void calculateBMI (Patient *patient, const float weight, const float height);
double mmol_conv (double mgdl);
void calculateCardioRisk (Patient *patient);
int saveAllPatientsToFile (Patient *patients, int patientCount, const char *filename);
int loadPatientsFromFile (Patient *patients, const char *filename);
void editPatient (Patient *patient);
void deletePatient (Patient *patients, int *patientCount, int index);
void showPatients (Patient *patient, int count);
void computeAverages(double data[][2], int patientCount);
void sortPatientsByID (Patient *patients, int patientCount, int order);
void sortPatientsByName (Patient *patients, int patientCount, int order);
int findPatientByID (Patient *patients, int patientCount, int input);
int findPatientByUserID (Patient *patients, int patientCount, int input);
int findPatientByName (Patient *patients, int patientCount, char *input);
void selectPatientID (Patient *patients, int *patientCount, Referral *referrals, User *users, User *currentUser, int userCount, int *referralCount);
void selectPatientName (Patient *patients, int *patientCount, Referral *referrals, User *users, User *currentUser, int userCount, int *referralCount);
void updateGender (Patient *patient);
void printDiagnosing(Patient *currentPatient);

void createReferral (Referral *referrals, User *users, Patient *currentPatient, User currentUser, int userCount, int *referralCount);
void showReferrals (User *currentUser, User *users, Referral *referrals, int referralCount);
void viewReferralStatus(User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, User *currentUser);
void editReferral (Referral *referral);
void deleteReferral (Referral *referrals, int *referralCount, int index);
int saveAllReferralsToFile (Referral *referrals, int referralCount, const char *filename);
int loadReferralsFromFile (Referral *referrals, const char *filename);
int getReferralID (Referral *referrals, int referralCount);
void sortReferralsByID (Referral *referrals, int referralCount, int order);
void sortReferralsByStatus (Referral *referrals, int referralCount, int order);
int findReferralByID (Referral *referrals, int referralCount, int input);
void selectReferralID (User *currentUser, Referral *referrals, int *referralCount, User *users, int userCount, Patient *patients, int patientCount);
