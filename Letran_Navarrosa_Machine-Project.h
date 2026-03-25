#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 100
// STRUCTS

// User struct for login/authentication
typedef struct {
    int userID;
    char username[101];
    unsigned long passwordHash; // When placed into TXT it will be a long + integer
    char role[30];   // "GP", "Specialist", "Patient"
    char name[101]; // Full name
} User;
typedef struct {
	int patientID;
	// int userID; // if no userID should default to -1
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
} Patient;
typedef struct {
	int ReferralID; // Referral ID
	int GPID; // GP user ID
	char PatientName[101]; // Patient Name
	int SpecialistID; // Specialist user ID
	char status[31]; // "Pending" "Accepted" "Completed" "Rejected"
	// maybe date?? pero we'll need to remake most to include date too for it to make sense T~T
	// reason for referral maybe wag na mas simple na referral system kahit medyo di nagmamake sense
} Referral;

// === Function Prototypes ===
int registerUser (User *users, int *userCount);
User* loginUser (User *users, int userCount);
void hashPassword (const char *password, unsigned long *outputHash);
int saveAllUsersToFile (User *users, int userCount, const char *filename);
int loadUsersFromFile (User *users, const char *filename);
int forgotPassword (User *users, int userCount, const char *username);
int getUserID (User *users, int userCount);
void gpDashboard (User *currentUser, Patient *patients, int *patientCount, User *users, int userCount, Referral *referrals, int *referralCount, double data[][2]);
void specialistDashboard(User *currentUser, User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, double data[][2]);
void patientDashboard(User *currentUser, User *users, Patient *patients, int *patientCount, Referral *referrals, int referralCount);

Patient addPatient (User *currentUser, Patient *patients, int patientCount);
void initPatient (Patient *patient);
void diagnosePatient (Patient *patients, int patientCount);
void showDiagnosisReport (Patient *currentPatient);
int getPatientID (Patient *patients, int patientCount);
void calculateBMI (Patient *patient, const float weight, const float height);
double mmol_conv (double mgdl);
void calculateCardioRisk (Patient *patient);
int saveAllPatientsToFile (Patient *patients, int patientCount, const char *filename);
int loadPatientsFromFile (Patient *patients, const char *filename);
void editPatient (Patient *patient, int count);
void deletePatient (Patient *patient, int *count);
void showPatients (Patient *patient, int count);
void computeAverages(double data[][2], int patientCount);
void sortPatientsByID (Patient *patients, int patientCount, int order);
void sortPatientsByName (Patient *patients, int patientCount, int order);

void createReferral (Referral *referrals, User *users, Patient *patients, User currentUser, int patientCount, int userCount, int *referralCount);
void showReferrals (User *currentUser, User *users, Referral *referrals, int referralCount);
void editReferral (Referral *referrals, int referralCount);
void deleteReferral (Referral *referrals, int *count);
int saveAllReferralsToFile (Referral *referrals, int referralCount, const char *filename);
int loadReferralsFromFile (Referral *referrals, const char *filename);
