/*
Authors: Navarrosa, Yerachmiel Jannes C. 	(yj17cayabyab@yahoo.com | https://github.com/whyjay-0)
		 Letran, Jessica C. 				(jessica_letra@dlsu.edu.ph | https://github.com/jessletran)
Program Description: This program is a referral management system for patients, general practitioners and specialists, 
					 where each user is able to log in and register with their details and personal information and 
					 access a referral, diagnosis, and record system.
Last updated: May 8, 2026
@version 1.0
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 100
#define HEIGHT 38
#define WIDTH 130
// STRUCTS

// User struct for login/authentication
typedef struct {
    int userID; // unique userID determined by the highest active user/patient
    char username[101]; // unique username for each userID
    unsigned long passwordHash; // When placed into TXT it will be a long + integer
    char role[30];   // "GP", "Specialist", "Patient"
    char name[101]; // Full name
    int questType; // Security question type
    char answer[101]; // Answer to above
    char hospital[101]; // N/A if not specialist, specialists will be prompted
} User;
typedef struct {
	int patientID; // unique patient ID per patient, determined by highest ID active 
	int userID; // if no userID should default add a new user ID, a patient is reserved a userID in the event they create an acc
	char name[101]; // name
	int age; // if age 30-79 cvdRisk is calculated
	char gender; // M or F, used for cvdRisk
	char contact[17]; // contact details of patient
	float bmi; // value of their bmi
	char bmiCat[12]; // category of bmi
	char bp[16]; // sys/dia mmHg, systolic blood pressure is parsed within calccvdRisk range: 90-200 mmHg
	float bloodSugar; // blood sugar
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
// =================  User Management  ============================
/*
Description: Clears any remaining input in the input buffer to prevent unwanted behavior in subsequent inputs.
*/
void clearInput();

/*
Description: Validates user input based on type and constraints such as range or character limits.
@param input - pointer to the variable where input will be stored
@param type - determines expected input type (e.g., int, float, char, string)
@param min - minimum acceptable value (for numeric types)
@param max - maximum acceptable value (for numeric types)
@param limit1-4 - optional character constraints (e.g., allowed values for char inputs)
returns int flag indicating whether input is valid
*/
int getValidInput(void *input, int type, int min, int max, char limit1, char limit2, char limit3, char limit4);

/*
Description: Registers a new user and optionally links them to an existing patient record.
@param users - array of user records
@param userCount - pointer to total number of users
@param patients - array of patient records
@param patientCount - total number of patients
returns int flag indicating success or failure
*/
int registerUser (User *users, int *userCount, Patient *patients, int patientCount);

/*
Description: Authenticates a user by checking entered credentials.
@param users - array of user records
@param userCount - total number of users
returns pointer to the logged-in User, or NULL if authentication fails
*/
User* loginUser (User *users, int userCount);

/*
Description: Converts a plaintext password into a hashed value for secure storage.
@param password - plaintext password
@param outputHash - pointer where hashed value will be stored
*/
void hashPassword (const char *password, unsigned long *outputHash);

/*
Description: Allows editing of current user's account details.
@param currentUser - user currently logged in
@param userCount - total number of users
@param users - array of user records
*/
void editUserDetails (User *currentUser, int userCount, User *users);

/*
Description: Saves all user records into a specified file.
@param users - array of user records
@param userCount - total number of users
@param filename - file to save data into
returns int flag indicating success or failure
*/
int saveAllUsersToFile (User *users, int userCount, const char *filename);

/*
Description: Loads user records from a file into memory.
@param users - array where user data will be stored
@param filename - file to load data from
returns int number of successfully loaded users
*/
int loadUsersFromFile (User *users, const char *filename);

/*
Description: Handles password recovery process for a user.
@param users - array of user records
@param userCount - total number of users
@param username - username requesting password reset
returns int flag indicating success or failure
*/
int forgotPassword (User *users, int userCount, const char *username);

/*
Description: Retrieves a user ID, optionally linking it with a patient record.
@param users - array of user records
@param patients - array of patient records
returns int user ID
*/
int getUserID (User *users, Patient *patients, int userCount, int patientCount);

//=================  Dashboard Printing  ============================
/*
Description: Displays and manages the General Practitioner's dashboard interface.
*/
void gpDashboard (User *currentUser, Patient *patients, int *patientCount, User *users, int userCount, Referral *referrals, int *referralCount, float data[][2]);

/*
Description: Displays and manages the Specialist's dashboard interface.
*/
void specialistDashboard(User *currentUser, User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, float data[][2]);

/*
Description: Displays and manages the Patient's dashboard interface.
*/
void patientDashboard(User *currentUser, User *users, int userCount, Patient *patients, int *patientCount, Referral *referrals, int referralCount);

//=================  Helper Functions  ============================
/*
Description: Searches for a user by ID.
@param users - array of user records
@param userCount - total number of users
@param input - ID to search
returns index of user if found, otherwise -1
*/
int findUserByID (User *users, const int userCount, int input);

/*
Description: Searches for a user by name.
@param users - array of user records
@param userCount - total number of users
@param input - name to search
returns index of user if found, otherwise -1
*/
int findUserByName (User *users, const int userCount, char *input);

/*
Description: Prints the main menu UI.
@param height - console height
@param width - console width
*/
void printMainMenu (const int height, const int width);

/*
Description: Prints the program title.
@param height - console height
@param width - console width
*/
void printTitle(const int height, const int width);

/*
Description: Clears the console screen.
*/
void clearScreen();

/*
Description: Handles text input display formatting.
*/
void inputText();

/*
Description: Centers text output on the console.
*/
void centerText();

/*
Description: Prints a border for UI elements.
*/
void printBorder();

/*
Description: Prints a divider line.
*/
void printDivider();

/*
Description: Prints text centered horizontally.
@param text - string to display
*/
void printCentered(const char* text);

/*
Description: Pauses execution until user provides input.
*/
void waitForInput();

// =================  Printing Tables and Records  ============================
/*
Description: Prints table header based on type.
@param type - determines table format
@param widths - column widths
*/
void printHeader (int type, int widths[]);

/*
Description: Prints table rows based on data type and filters.
*/
void printRows (int type, User *currentUser, User *users, Patient *patients, Referral *referrals, int count, int userCount, int widths[], int referralCount, char *filter);

/*
Description: Prints table footer.
@param type - determines table format
@param widths - column widths
*/
void printFooter (int type, int widths[]);

/*
Description: Calculates appropriate column widths for table display.
*/
void calculateWidths (int type, User *users, Patient *patients, Referral *referrals, int count, int userCount, int widths[]);

/*
Description: Displays user records in table format.
*/
void printUsers (User *currentUser, User *users, Patient *patients, Referral *referrals, int userCount, int referralCount, char *filter);

/*
Description: Displays patient records in table format.
*/
void printPatients (User *currentUser, User *users, Patient *patients, Referral *referrals, int patientCount, int userCount, int referralCount, char *filter);

/*
Description: Displays referral records in table format.
*/
void printReferrals (User *currentUser, User *users, Patient *patients, Referral *referrals, int referralCount, int userCount, char *filter);


// =================  Patient  ============================
/*
Description: For adding new patient. If patient does not have an account, a new userID is generated. If patient has an account, they will be prompted if they want to link or not.
@param User currentUser - for determining whether user is patient or not
@param patients - record of patients
@param patientCount - amt within array of struct patients
@param users - record of users
@param userCount - amt within array of struct users
returns Patient* - for saving to the TXT file
*/
Patient addPatient (User *currentUser, Patient *patients, int patientCount, User *users, int userCount);
/*
Description: sets default values for the newPatient
@param patient - current patient to be initialized
*/
void initPatient (Patient *patient);
/*
Description: For GP to diagnose existing patients.
@param patient - patient that will be diagnosed, information is saved directly in patient struct
*/
void diagnosePatient (Patient *patient);
/*
Description: used for printing the specific details of the patient without the diagnosis
@param currentPatient - currentPatient being shown
*/
void showPatientDetails (Patient *currentPatient);
/*
Description: used to print diagnosis report including patient details
@param currentPatient - current patient being shown
*/
void showDiagnosisReport (Patient *currentPatient);
/*
Description: generates a new patient ID based on what was the highest ID value found, only within patients
@param patients - records of patients
@param patientCount - amt of patients in array of struct
returns int the value of the new ID
*/
int getPatientID (Patient *patients, int patientCount);
/*
Description: calculates the BMI based on the info gathered from patient details
@param - info of patient where bmi and bmiCat will be stored
@param weight - const since it will not change within the func
@param height - const since it wont change within func
*/
void calculateBMI (Patient *patient, const float weight, const float height);
/*
Description: converts mgdl to mmol for calculating Cardiovascular Disease Risk
@param mgdl - inputted mg/dL
returns double, the value of the converted mgdL
*/
double mmol_conv (double mgdl);
/*
Description: This function utilizes the American Heart Association's PREVENT Equations, use of the equations has been authorized and given access by the AHA PREVENT Team
@param patient - contains the details found for the computations, and is also used to store CardioRisk
** Constants and beta coefficients used within the func was provided also by the AHA after accessing their .r script files
*/
void calculateCardioRisk (Patient *patient);
/*
Description: Saves everything within patients struct of array to the file, filename
@param patients - array of struct of patients
@param patientCount - amt of patients
@param filename - the name of the file to be accessed
returns flag, whether it was successful or not
*/
int saveAllPatientsToFile (Patient *patients, int patientCount, const char *filename);
/*
Description: Loads everything within the file, filename, to patients struct of array 
@param patients - array of struct of patients
@param patientCount - amt of patients
@param filename - the name of the file to be accessed
returns int, how many successful fscanf there were
*/
int loadPatientsFromFile (Patient *patients, const char *filename);
/*
Description: Edits an existing patient record.
@param patient - patient to be modified
*/
void editPatient (Patient *patient);

/*
Description: Deletes a patient record from the array.
@param patients - array of patients
@param patientCount - pointer to total number of patients
@param index - index of patient to delete
*/
void deletePatient (Patient *patients, int *patientCount, int index);

/*
Description: Computes averages for stored dataset (e.g., BMI, risk metrics).
@param data - 2D array containing values
@param patientCount - total number of patients
*/
void computeAverages(float data[][2], int patientCount);

//=================  Helper Functions  ============================
/*
Description: Sorts patients by ID.
@param order - determines ascending or descending
*/
void sortPatientsByID (Patient *patients, int patientCount, int order);

/*
Description: Sorts patients by name.
@param order - determines ascending or descending
*/
void sortPatientsByName (Patient *patients, int patientCount, int order);

/*
Description: Finds patient by ID.
returns index if found, otherwise -1
*/
int findPatientByID (Patient *patients, int patientCount, int input);

/*
Description: Finds patient by associated user ID.
returns index if found, otherwise -1
*/
int findPatientByUserID (Patient *patients, int patientCount, int input);

/*
Description: Finds patient by name.
returns index if found, otherwise -1
*/
int findPatientByName (Patient *patients, int patientCount, char *input);

//==================  Selection CRUD  =======================
/*
Description: Prompts user to select a patient by ID and performs related actions.
*/
void selectPatientID (Patient *patients, int *patientCount, Referral *referrals, User *users, User *currentUser, int userCount, int *referralCount);

/*
Description: Prompts user to select a patient by name and performs related actions.
*/
void selectPatientName (Patient *patients, int *patientCount, Referral *referrals, User *users, User *currentUser, int userCount, int *referralCount);

//=================  Helper Functions  ============================
/*
Description: Updates the gender field of a patient.
*/
void updateGender (Patient *patient);

/*
Description: Displays diagnosing output for a patient.
*/
void printDiagnosing(Patient *currentPatient);

//=================  Referrals  ============================
/*
Description: Creates a referral record for a patient to a specialist.
*/
void createReferral (Referral *referrals, User *users, Patient *currentPatient, User currentUser, int userCount, int *referralCount, Patient *patients);

/*
Description: Displays referral status for users.
*/
void viewReferralStatus(User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, User *currentUser);

/*
Description: Edits status of an existing referral.
*/
void editReferral (Referral *referral);

/*
Description: Deletes a referral record.
*/
void deleteReferral (Referral *referrals, int *referralCount, int index);

/*
Description: Saves all referral records to a file.
returns flag indicating success or failure
*/
int saveAllReferralsToFile (Referral *referrals, int referralCount, const char *filename);

/*
Description: Loads referral records from a file.
returns number of successfully loaded records
*/
int loadReferralsFromFile (Referral *referrals, const char *filename);

//=================  Helper Functions  ============================
/*
Description: Generates a new referral ID.
*/
int getReferralID (Referral *referrals, int referralCount);

/*
Description: Sorts referrals by ID.
*/
void sortReferralsByID (Referral *referrals, int referralCount, int order);

/*
Description: Sorts referrals by status.
*/
void sortReferralsByStatus (Referral *referrals, int referralCount, int order);

/*
Description: Finds referral by ID.
returns index if found, otherwise -1
*/
int findReferralByID (Referral *referrals, int referralCount, int input);

//=================  Referral CRUD  ============================
/*
Description: Prompts user to select a referral by ID and perform actions.
*/
void selectReferralID (User *currentUser, Referral *referrals, int *referralCount, User *users, int userCount, Patient *patients, int patientCount);
