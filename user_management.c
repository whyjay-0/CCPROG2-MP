#include "Letran_Navarrosa_Machine-Project.h"

// User management
/* Registers a user; returns flag if valid or not

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return int - index of newUser
*/
int registerUser (User *users, int *userCount){
	int i,similar=0;
	User newUser;
	char input[101];
	int choice, complete=0, validUser=0, validName=0, validChoice=0, index;
	
	if (*userCount >= MAX_USERS){
		printf("Max User Limit reached.");
		index=-1;
	}
	else {
		while (complete!=1){
			do{
				validUser=0;
				printf("\nPlease enter desired username (100 Characters): \n");
				scanf(" %100[^\n]", input);
				if (strlen(input)>100){
					printf("Invalid input\n");
				}
				else {
					similar=0;
					for(i=0;i<*userCount;i++){
						if (strcmp(users[i].username,input)==0){
							similar+=1;
						}
					}
					if (similar){
						printf("Invalid Username\n");
					}
					else {
						strcpy(newUser.username, input);
						validUser=1;
					}
				}
			} while (validUser==0);
			
			do{
				validName=0;
				printf("\nPlease enter your full name (100 Characters): \n");
				scanf(" %100[^\n]", input);
				if (strlen(input)>100){
					printf("Invalid input\n");
				}
				else {
					strcpy(newUser.name, input);
					validName=1;
				}
			} while (validName==0);
			
			printf("\nPlease enter password: \n");
			scanf(" %100[^\n]", input);
			hashPassword(input, &newUser.passwordHash);
			
			do{
				validChoice=0;
				printf("\nIndicate your role:\n1 - Patient\n2 - General Practitioner\n3 - Specialist\nChoice: ");
				scanf("%d", &choice);
					
				switch (choice){
					case 1: 
						strcpy(newUser.role, "Patient"); 
						validChoice=1; 
						complete=1; 
						break;
					case 2: 
						strcpy(newUser.role, "GP"); 
						validChoice=1; 
						complete=1; 
						break;
					case 3: 
						strcpy(newUser.role, "Specialist"); 
						validChoice=1; 
						complete=1; 
						break;
					default: 
						printf("Invalid input.\n"); 
						validChoice=0;
				}
			} while (validChoice==0);
		}
		newUser.userID = *userCount + 1; // set the userID of new user to old usercount + 1
		users[*userCount] = newUser; //set the array of struct at index *userCount to the newUser made
		index = *userCount; //the index of the user before it is incremented
		(*userCount)++; // increase amount of users
	}
	return index; // index of the user
}

/* Login a user; returns pointer to User struct if successful, NULL otherwise

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return User* - address of the user containing their ID, role, etc
*/
User* loginUser (User *users, int userCount){
	User *user=NULL;
	int i, validUser=0,validPass=0, found=0, matching=0, passChange=0;
	unsigned long inputHash=0;
	char username[101],password[101];
	do{
		do{
			printf("Enter your username: ");
			scanf(" %100[^\n]", username);
			found=0;
			for (i=0;i<userCount;i++){
				if (strcmp(users[i].username,username)==0){
					found=1;
					i=userCount; // end loop
				}
			}
			if (strlen(username)==0 || strlen(username)>100)
				printf("Invalid input.\n");
			else if (found==0){
				printf("Username not found.\n");
			}
			else {
				validUser=1;
			}
		} while (validUser==0);
		do{
			if (passChange==1)
				printf("Confirm your password.\n");
			else
				printf("Enter your password. If you have forgotten, input 0.\n");
			scanf(" %100[^\n]",password);
			
			if ((strlen(password)==0 || strlen(password)>100) && password[0]!='0')
				printf("Invalid input.\n");
			hashPassword(password,&inputHash);
			matching=0;
			for (i=0;i<userCount;i++){
				if (strcmp(users[i].username,username)==0 && users[i].passwordHash == inputHash){
					matching=1;
					i=userCount;
				}
			}
			
			if (password[0]=='0'){
				if (forgotPassword(users, userCount, username)==0){
					printf("Invalid username.\n");
				}
				else {
					passChange=1;
					validPass=0; // reenter pass
				}
			}
			else if (matching==0){
				printf("Invalid password.\n");
			}
			else {
				validPass=1;
				printf("Logged in successfully!\n");
			}
		} while (validPass==0);
	} while (validUser == 0 || validPass == 0);
	
	for (i=0;i<userCount;i++){
		if (strcmp(users[i].username,username)==0 && users[i].passwordHash == inputHash){
			//valid login, finding index of that user.
			user = &users[i];
			i=userCount; // end loop
		}
	}
	return user;
}
/* // Hash a password for storage; basic hash I found online "djb2", this is only temporary maybe depending how robust ung hash tignan

@param password - input password as str
@param outputHash - integers that contains the data for the password
@return void
*/
void hashPassword (const char *password, unsigned long *outputHash){
	int c;
	*outputHash = 5381;
	// djb2 algorithm, hash is in integer; needs to be converted to output
	while ((c = *password++)){
		*outputHash = ((*outputHash << 5) + *outputHash + c); //bitwise left shift by 5 (<< 5)
	}
}
// Save user to TXT file, return 1 if success, 0 otherwise
int saveAllUsersToFile (User *users, int userCount, const char *filename){
	FILE *fp;
	int i, flag = 0;
	if ((fp=fopen(filename, "w"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		for (i=0;i<userCount;i++){
			fprintf(fp, "%d, %s, %lu, %s, %s\n", users[i].userID, users[i].username, users[i].passwordHash, users[i].role, users[i].name);
		}
		flag=1;
		fclose(fp);
	}
	return flag;
}
// Load user from TXT file, return count of users
int loadUsersFromFile (User *users, const char *filename){
	FILE *fp;
	int count = 0, flag=1, result;
	
	if ((fp=fopen(filename, "r"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		while (flag){
			User temp;
			
			result = fscanf(fp, "%d, %100[^,], %lu, %100[^,], %100[^\n]", &temp.userID, temp.username, &temp.passwordHash, temp.role, temp.name);
			// since fscanf outputs the amount of input items, we can use it to check if scanf was successful
			if (result==5 && count < MAX_USERS){
				users[count]=temp;
				count++;
			}
			else {
				flag=0;
			}
		}
		fclose(fp);
	}
	return count;
}
// Forgot password / Password recovery
/* Allows user to reset passwords. Uses username for validity check before user can change password

@return 0 or 1 if success or not
*/
int forgotPassword (User *users, int userCount, const char *username){
	int flag=0,i;
	char newPass[101];
	for (i=0;i<userCount;i++){
		if (strcmp(users[i].username,username)==0){
			printf("Please enter password: ");
			scanf(" %100[^\n]", newPass);
			hashPassword(newPass, &users[i].passwordHash); // Need validity check for passwords
			printf("Successfully Changed Password!\n");
			saveAllUsersToFile(users, userCount, "users.txt"); // save new password
			flag = 1;
		}
	}
	return flag;
}
