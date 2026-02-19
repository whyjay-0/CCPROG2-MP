#include <stdio.h>
#include <string.h>

// STRUCTS

// User struct for login/authentication
typedef struct {
    int userID;
    char username[100];
    unsigned long passwordHash;
    char role[30];   // "GP", "Specialist", "Patient"
} User;

// LOGIN

// Register a new user (GP/Specialist/Patient)
int registerUser(int *userCount);
// Login a user; returns pointer to User struct if successful, NULL otherwise
User* loginUser(const char *username, const char *password);
// Hash a password for storage
void hashPassword(const char *password, unsigned long *outputHash);
// Check password against stored hash
int checkPassword(const char *inputPassword, const char *storedHash);
