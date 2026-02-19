#include "Letran_Navarrosa_Machine-Project.h"

// User management
// Register a new user (GP/Specialist/Patient)
int registerUser(int *userCount, User username){
	User newUser;
	char input[100];
	int choice, complete=0;
	
	(*userCount)++;
	newUser.userID = *userCount;
	
	while (complete!=1){
		printf("Please enter desired username (99 Characters): ");
		scanf("%s", input);
		strcpy(newUser.username, input);
		
		printf("Please enter password: ");
		scanf("%s", input);
		hashPassword(input, &newUser.passwordHash);
		
		printf("Indicate your role:\n1 - Patient\n2 - General Practitioner\n3 - Specialist\nChoice: ");
		scanf("%d", &choice);
			
		switch (choice){
			case 1: strcpy(newUser.role, "Patient"); complete=1; break;
			case 2: strcpy(newUser.role, "GP"); complete=1; break;
			case 3: strcpy(newUser.role, "Specialist"); complete=1; break;
			default: printf("Invalid input.");
		}
	}
	return newUser.userID;
}

// Login a user; returns pointer to User struct if successful, NULL otherwise
User* loginUser(const char *username, const char *password){
	
}
// Hash a password for storage; basic hash I found online "djb2", this is only temporary maybe depending how robust ung hash tignan
void hashPassword(const char *password, unsigned long *outputHash){
	int c;
	*outputHash = 5381;
	// djb2 algorithm, hash is in integer; needs to be converted to output
	while (c = *password++){
		*outputHash = ((*outputHash << 5) + *outputHash) + c; //bitwise left shift by 5 (<< 5)
	}
}
// Verify password against stored hash
int verifyPassword(const char *inputPassword, const char *storedHash){
	
}

