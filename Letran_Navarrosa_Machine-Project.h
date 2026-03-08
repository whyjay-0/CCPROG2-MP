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
	int GPID; // GP user ID
	int PatientID; // Patient user ID
	int SpecialistID; // Specialist user ID
	char status[31]; // "Pending" "Accepted" "Completed" "Rejected"
	// maybe date?? pero we'll need to remake most to include date too for it to make sense T~T
	// reason for referral maybe wag na mas simple na referral system kahit medyo di nagmamake sense
} Referral;

// LOGIN

// User management
/* Registers a user; returns flag if valid or not

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return int - returns 1 or 0
*/
int registerUser (User *users, int *userCount);
/* Login a user; returns pointer to User struct if successful, NULL otherwise

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return User* - address of the user containing their ID, role, etc
*/
User* loginUser (User *users, int userCount);
/* // Hash a password for storage; basic hash I found online "djb2", this is only temporary maybe depending how robust ung hash tignan

@param password - input password as str
@param outputHash - integers that contains the data for the password
@return void
*/
void hashPassword (const char *password, unsigned long *outputHash);
// Save user to TXT file, return 1 if success, 0 otherwise
int saveUserToFile (const User *user, const char *filename);
// Load user from TXT file, return count of users
int loadUsersFromFile (User *users, const char *filename);
// Forgot password / Password recovery
/* Allows user to reset passwords. Uses username for validity check before user can change password

@return 0 or 1 if success or not
*/
int forgotPassword ();

// PATIENT LOGGING
// Set patient ID, name, age, contact in a new log
Patient addPatient ();
// Diagnose a patient
void diagnosePatient (Patient *patients,patientCount);
// Calculate BMI
void calculateBMI (Patient *patient, const float weight, const float height);
// Convert mg/dL to mmol
double mmol_conv (double mgdl);
// Calculate Risk
void calculateCardioRisk(Patient *patient);

// Save patient to file
int savePatientToFile (const Patient *patient, const char *filename);
// Load patient to file
int loadPatientsFromFile (Patient *patients, const char *filename);
// Edit patient ID, name, age, contact
void editPatient (Patient *patient, int count);
// Delete patient
void deletePatient (Patient *patient, int *count);
// Shows list of patients
void showPatients (Patient *patient, int count);
