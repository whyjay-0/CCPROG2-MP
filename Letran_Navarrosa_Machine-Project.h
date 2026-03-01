#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
// STRUCTS

// User struct for login/authentication
typedef struct {
    int userID;
    char username[101];
    unsigned long passwordHash;
    char role[30];   // "GP", "Specialist", "Patient"
} User;
typedef struct {
	int patientID;
	int userID;
	char name[101];
	int age;
	char contact[101];
	float bmi;
	char bp[21];
	float bloodSugar;
	int cardioRisk;
} Patient;

// LOGIN

// User management
/* Registers a user; returns flag if valid or not

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return int - returns 1 or 0
*/
int registerUser (User users[], int *userCount);
/* Login a user; returns pointer to User struct if successful, NULL otherwise

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return User* - address of the user containing their ID, role, etc
*/
User* loginUser (User users[], int userCount);
/* // Hash a password for storage; basic hash I found online "djb2", this is only temporary maybe depending how robust ung hash tignan

@param password - input password as str
@param outputHash - integers that contains the data for the password
@return void
*/
void hashPassword (const char *password, unsigned long *outputHash);
// Save user to TXT file, return 1 if success, 0 otherwise
int saveUserToFile (const User *user, const char *filename);
// Load user from TXT file, return count of users
int loadUsersFromFile (Users users[], cost char *filename);
// Forgot password / Password recovery
/* Allows user to reset passwords. Uses username for validity check before user can change password

@return 0 or 1 if success or not
*/
void forgotPassword ();

// PATIENT MANAGEMENT

// Set patient ID, name, age, contact

// Diagnose a patient

// Calculate BMI

// Calculate Risk

// Print Recommendations (Weight, based on risk level)

// Save patient to file

// Load patient to file

// Edit patient ID, name, age, contact

// Edit patient health metric

// Delete patient

