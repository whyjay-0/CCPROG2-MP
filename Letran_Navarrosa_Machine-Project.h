#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <math.h>

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
	char name[101];
	int age;
	char gender;
	char contact[17];
	float bmi;
	char bmiCat[12];
	char bp[16]; // sys/dia mmHg
	float bloodSugar;
	float totalChol;
	float hdlChol;
	int eGFR;
	char htMed;
	char statins;
	char smoking;
	char diabetes;
	float creatinine;
	char cvdFamily;
	char diet;
	char exercise;
	char alcohol;
	double cardioRisk; 
} Patient;

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
void diagnosePatient (Patient *patient);
// Calculate BMI
void calculateBMI (Patient *patient, const float weight, const float height);
// Calculate Risk
calculateCardioRisk(Patient *patient, const float totalChol, const float hdlChol, const int eGFR,  const char htMed, const char statins, const char smoking, const char diabetes);

// Save patient to file
int savePatientToFile (const Patient *patient, const char *filename);
// Load patient to file
int loadPatientsFromFile (Patient *patients, const char *filename);
// Edit patient ID, name, age, contact
void editPatient (Patient *patient);
// Edit patient health metric
void editPatientHealth (Patient *patient);
// Delete patient
void deletePatient (Patient *patient);

#endif
