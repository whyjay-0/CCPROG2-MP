#include "Letran_Navarrosa_Machine-Project.h"

// Utility
// clears everything inputted, for validation checking when invalid
void clearInput(){
	while (scanf("%*s") == 1); // while scanf is detecting string, disregard/clear '*'
}

/*
This function accepts an input where type is passed and min and max if there are limits
TYPES:
1 - int
2 - float
3 - char
4 - string 100
5 - string 16
6 - string 15
@param void *input - this parameter accepts the memory address of any type and stores it
@param type - this param determines which type of validation will be done
@param min - minimum accepted number
@param max - max accepted number, if out of min/max = invalid
@param limit1 - char limitation to 1 or 2 character only.
@param limit2 - ^^^^
@param limit3/4 - smaller case letters for limit1 and 2
*/
int getValidInput(void *input, int type, int min, int max, char limit1, char limit2, char limit3, char limit4){
	int valid = 0;
	int scan = 0; // since scanf has a return for amt of scanned values, it can be used for valid checking of input
	
	while(valid==0){
		inputText();
		printBorder();
		printf("%22sInput: ","");
		
		switch(type){
			case 1: // int
				scan = scanf("%d", (int*)input); // (int*)var, transforms the type of var into a pointer to an int
				if (scan != 1){
					printCentered("Invalid input. Enter an integer");
					clearInput();
				}
				else if (*(int*)input < min || *(int*)input > max){ // *(int*)var , value inside the pointer to int
					printf("%50sOut of range (%d-%d). Try again", "", min, max);
				}
				else 
					valid=1;
				break;
			case 2: // float
				scan = scanf("%f", (float*)input);
				if (scan != 1){
					printCentered("Invalid input. Enter a float");
					clearInput();
				}
				else if (*(float*)input < min || *(float*)input > max){
					printf("%50sOut of range (%d-%d). Try again", "", min, max);
				}
				else
					valid=1;
				break;
			case 3: // char
				scan = scanf(" %c", (char*)input);
				if (scan != 1){
					((char*)input)[0] = '\0';
					printf("Invalid input. Enter a character: ");
					clearInput();
				}
				else if (*(char*)input != limit1 && *(char*)input != limit2 && *(char*)input != limit3 && *(char*)input != limit4)
					printf("Invalid input. Enter %c or %c", limit1, limit2);
				else
					valid=1;
				break;
			case 4: // string 100
				scan = scanf(" %100[^\n]", (char*)input);
				if (scan != 1){
					((char*)input)[0] = '\0';
					printf("Invalid input");
					clearInput();
				}
				else if (strlen((char*)input)==0){
					printf("Empty input. Try again");
				}
				else
					valid=1;
				break;
			case 5: // string 16
				scan = scanf(" %16[^\n]", (char*)input);
				if (scan != 1){
					((char*)input)[0] = '\0';
					printf("Invalid input");
					clearInput();
				}
				else if (strlen((char*)input)==0){
					printf("Empty input. Try again");
				}
				else
					valid=1;
				break;
			case 6: // string 15
				scan = scanf(" %15[^\n]", (char*)input);
				if (scan != 1){
					((char*)input)[0] = '\0';
					printf("Invalid input");
					clearInput();
				}
				else if (strlen((char*)input)==0){
					printf("Empty input. Try again");
				}
				else
					valid=1;
				break;
		}
	}
	return 1;
}


// User management
/* Registers a user; returns flag if valid or not

@param users[] - contains the list of users
@param userCount - amount of users in the array users[]
@return int - index of newUser
*/
int registerUser (User *users, int *userCount, Patient *patients, int patientCount){
	int i,similar=0;
	User newUser;
	char input[101];
	char cInput;
	int choice, complete=0, validUser=0, index, pIndex;
	
	if (*userCount >= MAX_USERS){
		printCentered("Max User Limit reached");
		waitForInput();
		index=-1;
	}
	else {
		while (complete!=1){
			do{
				validUser=0;
				printCentered("Please enter desired username [100 Characters Max].");
				getValidInput(input,4,0,0,0,0,0,0);
				
				similar=0;
				for(i=0;i<*userCount;i++){
					if (strcmp(users[i].username,input)==0){
						similar+=1;
					}
				}
				if (similar){
					printCentered("Username is already in use");
					waitForInput();
				}
				else {
					strcpy(newUser.username, input);
					validUser=1;
				}
			} while (validUser==0);
			
			printCentered("Please enter your full name [100 Characters Max].");
			getValidInput(input,4,0,0,0,0,0,0);
			strcpy(newUser.name, input);
			
			printCentered("Please enter password.");
			getValidInput(input,4,0,0,0,0,0,0);
			hashPassword(input, &newUser.passwordHash);
			
			clearScreen();
			printf("%55sIndicate your role:\n%59s[1] Patient\n%53s[2] General Practitioner\n%58s[3] Specialist","","","","");
			getValidInput(&choice,1,1,3,0,0,0,0);
				
			switch (choice){
				case 1: 
					strcpy(newUser.role, "Patient");
					// find if full name is in patient record/array
					pIndex = findPatientByName(patients,patientCount,newUser.name);
					if (pIndex!=-1){
						clearScreen();
						printf("%39sA patient record already exists with a similar name.\n%34sDo you want to link this account to your patient record? [Y/N]","","");
						getValidInput(&cInput,3,0,0,'Y','N','y','n');
						switch(cInput){
							case 'Y':
							case 'y':
								newUser.userID = patients[pIndex].userID;
								break;
							case 'N':
							case 'n':
								newUser.userID = getUserID(users,patients);
								break;
							default:
								printCentered("Invalid input.");
								waitForInput();
						}
					}
					else {
						newUser.userID = getUserID(users,patients);
					}
					strcpy(newUser.hospital, "N/A");
					complete=1; 
					break;
				case 2: 
					strcpy(newUser.role, "GP"); 
					newUser.userID = getUserID(users,patients); // set the userID of new user to highest userID found + 1
					strcpy(newUser.hospital, "N/A");
					
					complete=1; 
					break;
				case 3: 
					strcpy(newUser.role, "Specialist");
					newUser.userID = getUserID(users,patients);
					
					printCentered("Enter the name of the hospital you are affiliated with");
					getValidInput(newUser.hospital,4,0,0,0,0,0,0);
					
					complete=1; 
					break;
				default: 
					printCentered("Invalid input.");
					waitForInput();
			}
			clearScreen();
			printf("%51sSelect a security question:\n%49s[1] What is your favorite food?\n%49s[2] What is your favorite color?\n%46s[3] What is your favorite animal/pet?\n","","","","");
			getValidInput(&newUser.questType,1,1,3,0,0,0,0);
			printCentered("Input answer to security question.");
			getValidInput(newUser.answer,4,0,0,0,0,0,0);
		}
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
	int i, validUser=0,validPass=0, found=-1, matching=-1, passChange=0;
	unsigned long inputHash=0;
	char username[101],password[101];
	do{
		do{
			clearScreen();
			printCentered("Enter your username.");
			if (found==0){
				printf("\n%55sUsername not found.","");
			}
			
			getValidInput(username,4,0,0,0,0,0,0);
			found=0;
			
			printf("userCount = %d\n", userCount);
			
			for (i=0;i<userCount;i++){
				if (strcmp(users[i].username,username)==0){
					found=1;
					i=userCount; // end loop
				}
			}
			
			if (found){
				validUser=1;
			}
		} while (validUser==0);
		do{
			if (passChange==1)
				printCentered("Confirm your password");
			else
				printCentered("Enter your password. If you have forgotten, input 0");
			if (matching==0 && passChange==0){
				printf("\n%57sWrong password","");
			}
			getValidInput(password,4,0,0,0,0,0,0);
			hashPassword(password,&inputHash);
			
			matching=0;
			for (i=0;i<userCount;i++){
				if (strcmp(users[i].username,username)==0 && users[i].passwordHash == inputHash){
					matching=1;
					i=userCount;
				}
			}
			
			if (password[0]=='0' && strlen(password)==1){
				if (forgotPassword(users, userCount, username)==0){
					printCentered("Invalid username");
					waitForInput();
				}
				else {
					passChange=1;
					validPass=0; // reenter pass
				}
			}
			if (matching==1) {
				validPass=1;
				printCentered("Logged in successfully!");
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

void editUserDetails (User *currentUser, int userCount, User *users){
	int i, padding, choice=-1, similar=0, complete=0;
	char sInput[101], sConfirm[101], sUser[101];
	unsigned long confirm=0, newPass=0;
	
	padding = WIDTH - strlen(currentUser->username) - strlen(currentUser->role) - 23;
    if (padding < 0)
    	padding=0;
    printf("%*sLogged in as: %s | Role: %s\n", padding, "", currentUser->username, currentUser->role);
    
    for (i=0;i<25;i++){
       	printf("\n");
	}
        
    printf("%27s","");
	for (i=0;i<WIDTH-100;i++){
		printf("%c",205);
	}
	printf("  Edit Details  ");
	for (i=0;i<WIDTH-100;i++){
		printf("%c",205);
	}
	
	printf("\n");
    printf("%52s[1] Change Username\n","");
    printf("%52s[2] Change Password\n","");
    printf("%52s[3] Edit security question\n","");
    printf("%52s[4] Edit full name\n","");
	if (strcmp(currentUser->role, "Specialist")==0){
		printf("%52s[5] Edit hospital information","");
	} 
    printf("%52s[0] Exit","");
	getValidInput(&choice,1,0,5,0,0,0,0);
	
	switch (choice){
		case 1:
			do{
				clearScreen();
				complete=0;
				printCentered("Enter new username [100 Characters Max]");
				getValidInput(sUser,4,0,0,0,0,0,0);
				
				similar=0;
				for (i=0;i<userCount;i++){
					if (strcmp(users[i].username,sUser)==0){
						similar += 1;
					}
				}
				if (similar){
					printCentered("Username is already used");
					waitForInput();
				}
				else {
					strcpy(currentUser->username,sUser);
					complete=1;
				}
					
			} while (complete==0);
			saveAllUsersToFile(users, userCount, "users.txt");
			break;
		case 2:
			do {
				printCentered("Enter new password");
				getValidInput(sInput,4,0,0,0,0,0,0);
				printCentered("Enter old password to confirm. Enter 0 to go back to dashboard");
				getValidInput(sConfirm,4,0,0,0,0,0,0);
				hashPassword(sConfirm,&confirm);
				if (strcmp(sConfirm,"0")==0){
					complete=1;
				}
				else if (confirm == currentUser->passwordHash){
					hashPassword(sInput,&newPass);
					currentUser->passwordHash = newPass;
					complete=1;
				}
				else {
					printCentered("Incorrect password. Try again");
					waitForInput();
				}
			} while (complete==0);
			saveAllUsersToFile(users, userCount, "users.txt");
			break;
		case 3:
			clearScreen();
			printf("%49sSelect a new security question:\n%49s  [1] What is your favorite food?\n%49s  [2] What is your favorite color?\n%49s  [3] What is your favorite animal/pet?\n","","","","");
			getValidInput(&currentUser->questType,1,1,3,0,0,0,0);
			printCentered("Input answer to security question.");
			getValidInput(currentUser->answer,4,0,0,0,0,0,0);
			saveAllUsersToFile(users, userCount, "users.txt");
			break;
		case 4:
			printCentered("Enter your new full name");
			getValidInput(currentUser->name,4,0,0,0,0,0,0);
			padding = (WIDTH - 35 - strlen(currentUser->name)) / 2;
			if (padding < 0)
    			padding=0;
			clearScreen();
			printf("%*sAre you sure you want the new name %s", padding, "", currentUser->name);
			waitForInput();
			saveAllUsersToFile(users, userCount, "users.txt");
			break;
		case 5:
			if (strcmp(currentUser->role, "Specialist")==0){
				printCentered("Enter new name of the hospital you are affiliated with");
				getValidInput(currentUser->hospital,4,0,0,0,0,0,0);
				padding = (WIDTH - 41 - strlen(currentUser->hospital)) / 2;
				if (padding < 0)
    				padding=0;
				clearScreen();
				printf("%*sAre you sure %s is your affiliated hospital", padding, "", currentUser->hospital);
				waitForInput();
			}
			else {
				printCentered("Invalid role.");
				waitForInput();
			}
			saveAllUsersToFile(users, userCount, "users.txt");
			break;
		case 0:
			printCentered("Exiting...");
			saveAllUsersToFile(users, userCount, "users.txt");
			break;
		default:
			printCentered("Invalid input");
			waitForInput();
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
			fprintf(fp, "%d, %s, %lu, %s, %s, %d, %s, %s\n", users[i].userID, users[i].username, users[i].passwordHash, users[i].role, users[i].name, users[i].questType, users[i].answer, users[i].hospital);
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
			
			result = fscanf(fp, "%d, %100[^,], %lu, %100[^,], %100[^,], %d, %100[^,], %100[^\n]", &temp.userID, temp.username, &temp.passwordHash, temp.role, temp.name, &temp.questType, temp.answer, temp.hospital);
			// since fscanf outputs the amount of input items, we can use it to check if fscanf was successful
			if (result==8 && count < MAX_USERS){
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
	int flag=0,i,valid=0,count=0;
	char newPass[101];
	char input[101];
	for (i=0;i<userCount;i++){
		if (strcmp(users[i].username,username)==0){
			do{

				if (users[i].questType == 1){
					printCentered("What is your favorite food?");
					if (count==1)
						printf("\n%58sWrong Answer","");
					getValidInput(input,4,0,0,0,0,0,0);
					
					if (strcmp(input,users[i].answer)==0){
						valid=1;
					}
					else {
						valid=0;
					}
				}
				else if (users[i].questType == 2){
					printCentered("What is your favorite color?");
					if (count==1)
						printf("\n%58sWrong Answer","");
					getValidInput(input,4,0,0,0,0,0,0);
					
					if (strcmp(input,users[i].answer)==0){
						valid=1;
					}
					else {
						valid=0;
					}
				}
				else if (users[i].questType == 3){
					printCentered("What is your favorite animal/pet?");
					if (count==1)
						printf("\n%58sWrong Answer","");
					getValidInput(input,4,0,0,0,0,0,0);
					
					if (strcmp(input,users[i].answer)==0){
						valid=1;
					}
					else {
						valid=0;
					}
				}
				count=1;
			} while (valid==0);
			if (valid){
				printCentered("Please enter new password.");
				getValidInput(newPass,4,0,0,0,0,0,0);
				hashPassword(newPass, &users[i].passwordHash); // Need validity check for passwords
				printCentered("Successfully Changed Password!");
				saveAllUsersToFile(users, userCount, "users.txt"); // save new password
				waitForInput();
				flag = 1;
			}
		}
	}
	return flag;
}

// Finds max userID and sets new userID to be used
int getUserID (User *users, Patient *patients){
	int i,id,max=-1; // -1 since -1 < 0,, avoiding recurring 0 for userID, but will cause userID to start from 1
	int isEmpty=1;
	// check if there are no users
	
	for (i=0;i<MAX_USERS;i++){
		if (users[i].userID > max){
			max = users[i].userID;
			isEmpty=0;
		}
	}
	
	for (i=0;i<MAX_USERS;i++){
		if (patients[i].userID > max){
			max = patients[i].userID;
			isEmpty=0;
		}
	}
	
	if (isEmpty == 1){
		id = 0;
	}
	else{
		id = max + 1;
	}
	return id;
}

// User Dashboards
// GP Dashboard
void gpDashboard (User *currentUser, Patient *patients, int *patientCount, User *users, int userCount, Referral *referrals, int *referralCount, float data[][2]){
	int i, choice = -1, pchoice = -1, rchoice = -1, order = -1;
	int padding;
	
    do{
        clearScreen();
        
        padding = WIDTH - strlen(currentUser->username) - strlen(currentUser->role) - 23;
        if (padding < 0)
        	padding=0;
        printf("%*sLogged in as: %s | Role: %s\n", padding, "", currentUser->username, currentUser->role);
        
        for (i=0;i<25;i++){
        	printf("\n");
		}
        
        printf("%27s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  GP DASHBOARD  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
        printf("%50s[1] Add Patient\n","");
        printf("%50s[2] Show Patients\n","");
        printf("%50s[3] Show Referrals\n",""); 
        printf("%50s[4] Compute Averages (BMI and Cardio Risk)\n","");
        printf("%50s[5] Edit User Details\n","");
        printf("%50s[0] Logout","");
		getValidInput(&choice,1,0,5,0,0,0,0);
		
        switch(choice){
            case 1:
                if (*patientCount < MAX_USERS){
                    patients[*patientCount] = addPatient(currentUser,patients,*patientCount,users,userCount);
                    (*patientCount)++;
                }
                else{
                	printCentered("Reached max patient count");
                	waitForInput();
				}
                saveAllPatientsToFile(patients,*patientCount,"patients.txt");
                break;
			
            case 2:
                do{
            		clearScreen();
					printPatients(currentUser,users,patients,referrals,*patientCount,userCount,*referralCount,"*");
            		
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Patient CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%52s[1] Select Patient by Patient ID\n","");
            		printf("%52s[2] Select Patient by Name\n","");
            		printf("%52s[3] Sort Patient by Patient ID\n","");
            		printf("%52s[4] Sort Patient by Name\n","");
            		printf("%52s[0] Exit","");
					getValidInput(&pchoice,1,0,4,0,0,0,0);
            		
            		switch(pchoice){
            			case 1:
            				clearScreen();
							selectPatientID(patients,patientCount,referrals,users,currentUser,userCount,referralCount);
							break;
            			case 2:
            				clearScreen();
							selectPatientName(patients,patientCount,referrals,users,currentUser,userCount,referralCount);
							break;
            			case 3:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending","","","");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortPatientsByID(patients, *patientCount, order);
            				break;
            			case 4:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending","","","");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortPatientsByName(patients, *patientCount, order);
            				break;
            			case 0:
            				printCentered("Exiting...");
            				break;
            			default:
            				printCentered("Invalid input.");
					}
				} while(pchoice!=0);
				saveAllPatientsToFile(patients,*patientCount,"patients.txt");
                break;
            case 3:
                do{
                	clearScreen();
					printReferrals(currentUser,users,patients,referrals,*referralCount,userCount,"*");
					
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Referral CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%52s[1] Select Referrals by Referral ID\n","");
            		printf("%52s[2] Sort Referrals by Referral ID\n","");
            		printf("%52s[3] Sort Referrals by Status\n","");
            		printf("%52s[0] Exit","");
            		
            		getValidInput(&rchoice,1,0,3,0,0,0,0);
            		
            		switch(rchoice){
            			case 1:
            				clearScreen();
							selectReferralID(currentUser,referrals,referralCount,users,userCount,patients,*patientCount);
							break;
            			case 2:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending","","","");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortReferralsByID(referrals,*referralCount,order);
            				break;
            			case 3:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending","","","");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortReferralsByStatus(referrals,*referralCount,order);
            				break;
            			case 0:
            				printCentered("Exiting...");
            				break;
            			default:
            				printCentered("Invalid input.");
					}
				} while(rchoice!=0);
                saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
                break;
            case 4:
            	clearScreen();
				for (i=0;i<*patientCount;i++){
            		data[i][0] = patients[i].bmi;
            		data[i][1] = patients[i].cardioRisk;
				}
                computeAverages(data,*patientCount);
                waitForInput();
                break;
            case 5:
            	clearScreen();
            	editUserDetails(currentUser, userCount, users);
            	break;
            case 0:
    			printCentered("Logging out...");
    			break;
            default:
                printCentered("Invalid input.");
        }
    } while(choice!=0);
}

void specialistDashboard(User *currentUser, User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, float data[][2]){
	int i, choice=-1, pchoice=-1, rchoice=-1, uchoice=-1, order=-1;
	int padding;
	
    do{
        clearScreen();
        
        padding = WIDTH - strlen(currentUser->username) - strlen(currentUser->role) - 23;
        if (padding < 0)
        	padding=0;
        printf("%*sLogged in as: %s | Role: %s\n", padding, "", currentUser->username, currentUser->role);
        
        for (i=0;i<25;i++){
        	printf("\n");
		}
        
        printf("%24s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  SPECIALIST DASHBOARD  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
		
        printf("%50s[1] Show Referrals\n","");
        printf("%50s[2] Show Patients\n","");
        printf("%50s[3] Compute Averages (BMI + Cardio Risk)\n","");
        printf("%50s[4] Edit User Details\n","");
        printf("%50s[5] Search for Users\n","");
        printf("%50s[0] Logout","");
        getValidInput(&choice,1,0,5,0,0,0,0);
        
        switch(choice){
            case 1:
                do{
                	clearScreen();
					// showReferrals(currentUser,users,referrals,referralCount);
                	printReferrals(currentUser,users,patients,referrals,referralCount,userCount,"*");
                	
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Referral CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%52s[1] Select Referrals by ID\n","");
            		printf("%52s[2] Sort Referrals by ID\n","");
            		printf("%52s[3] Sort Referrals by Status\n","");
            		printf("%52s[0] Exit","");
            		getValidInput(&rchoice,1,0,3,0,0,0,0);
            		
            		switch(rchoice){
            			case 1:
            				clearScreen();
							selectReferralID(currentUser,referrals,&referralCount,users,userCount,patients,patientCount);
							break;
            			case 2:
            				clearScreen();
							printf("%59sOrder:\n%59s  [1] Ascending\n%59s  [2] Descending","","","");
            				getValidInput(&order,1,1,2,0,0,0,0);
            				sortReferralsByID(referrals,referralCount,order);
            				break;
            			case 3:
            				clearScreen();
							printf("%59sOrder:\n%59s  [1] Ascending\n%59s  [2] Descending","","","");
            				getValidInput(&order,1,1,2,0,0,0,0);
            				sortReferralsByStatus(referrals,referralCount,order);
            				break;
            			case 0:
							printCentered("Exiting...\n");
            				break;
            			default:
            				printCentered("Invalid input.");
					}
				} while(rchoice!=0);
				saveAllReferralsToFile(referrals,referralCount,"referrals.txt");
                break;
            case 2:
            	do{
            		clearScreen();
					printPatients(currentUser,users,patients,referrals,patientCount,userCount,referralCount,"*");
					
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Patient CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%52s[1] Select Patient by Patient ID\n","");
            		printf("%52s[2] Select Patient by Name\n","");
            		printf("%52s[3] Sort Patient by Patient ID\n","");
            		printf("%52s[4] Sort Patient by Name\n","");
            		printf("%52s[0] Exit","");
            		
            		getValidInput(&pchoice,1,0,4,0,0,0,0);
            		
            		switch(pchoice){
            			case 1:
            				clearScreen();
							selectPatientID(patients,&patientCount,referrals,users,currentUser,userCount,&referralCount);
							break;
            			case 2:
            				clearScreen();
							selectPatientName(patients,&patientCount,referrals,users,currentUser,userCount,&referralCount);
            				break;
            			case 3:
            				clearScreen();
							printf("%59sOrder:\n%59s  [1] Ascending\n%59s  [2] Descending","","","");
            				getValidInput(&order,1,1,2,0,0,0,0);
            				sortPatientsByID(patients, patientCount,order);
            				break;
            			case 4:
            				clearScreen();
							printf("%59sOrder:\n%59s  [1] Ascending\n%59s  [2] Descending","","","");
            				getValidInput(&order,1,1,2,0,0,0,0);
            				sortPatientsByName(patients, patientCount, order);
            				break;
            			case 0:
            				printCentered("Exiting...");
            				break;
            			default:
            				printCentered("Invalid input.");
					}
				} while(pchoice!=0);
				saveAllPatientsToFile(patients,patientCount,"patients.txt");
            	break;
            case 3:
            	clearScreen();
				for (i=0;i<patientCount;i++){
            		data[i][0] = patients[i].bmi;
            		data[i][1] = patients[i].cardioRisk;
				}
            	computeAverages(data,patientCount);
            	waitForInput();
            	break;
            case 4:
            	clearScreen();
            	editUserDetails(currentUser, userCount, users);
            	break;
            case 5:
            	do{
            		clearScreen();
            		printUsers(currentUser,users,patients,referrals,userCount,referralCount,"*");
            		
            		printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Users CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
					printf("%52s[1] Search For Patients\n","");
            		printf("%52s[2] Search For Specialists\n","");
            		printf("%52s[3] Search For GPs\n","");
            		printf("%52s[0] Exit","");
					
					getValidInput(&uchoice,1,0,3,0,0,0,0);
					
					switch(uchoice){
						case 1:
							printUsers(currentUser,users,patients,referrals,userCount,referralCount,"Patient");
							break;
						case 2:
							printUsers(currentUser,users,patients,referrals,userCount,referralCount,"Specialist");
							break;
						case 3:
							printUsers(currentUser,users,patients,referrals,userCount,referralCount,"GP");
							break;
						case 0:
							printCentered("Exiting...");
							break;
						default:
            				printCentered("Invalid input.");
					}
				} while (uchoice!=0);
            	break;
            case 0:
    			printCentered("Logging out...");
    			break;
            default:
                printCentered("Invalid input.");
        }
    } while(choice!=0);
}

void patientDashboard(User *currentUser, User *users, int userCount, Patient *patients, int *patientCount, Referral *referrals, int referralCount){
    int i, found=0, choice=-1;
    int padding;

    do{
    	clearScreen();
    	
    	padding = WIDTH - strlen(currentUser->username) - strlen(currentUser->role) - 23;
        if (padding < 0)
        	padding=0;
        printf("%*sLogged in as: %s | Role: %s\n", padding, "", currentUser->username, currentUser->role);
        
        for (i=0;i<26;i++){
        	printf("\n");
		}
    	
    	printf("%25s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  PATIENT DASHBOARD  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
    	
    	printf("%53s[1] Add Self as Patient\n","");
    	printf("%53s[2] View Diagnosis Report\n","");
    	printf("%53s[3] View My Referral\n","");
    	printf("%53s[4] Edit User Details\n","");
    	printf("%53s[0] Logout","");
    	
    	getValidInput(&choice,1,0,4,0,0,0,0);
    	switch(choice){
    		case 1:
    			clearScreen();
    			if (*patientCount < MAX_USERS){
                    patients[*patientCount] = addPatient(currentUser,patients,*patientCount,users,userCount);
                    (*patientCount)++;
                }
                else{
                	printCentered("Reached max patient count");
                	waitForInput();
				}
				saveAllPatientsToFile(patients,*patientCount,"patients.txt");
    			break;
    		case 2:
    			clearScreen();
    			printf("%25s","");
				for (i=0;i<WIDTH-100;i++){
					printf("%c",205);
				}
				printf("  Diagnosis report  ");
				for (i=0;i<WIDTH-100;i++){
					printf("%c",205);
				}
				printf("\n");
    			for (i=0;i<*patientCount;i++){
					if (strcmp(patients[i].name,currentUser->name)==0){
						showDiagnosisReport(&patients[i]);
			            found=1;
			        }
			    }
			    if (!found){
    			    printCentered("No record found.");
    			}
    			waitForInput();
    			break;
    		case 3:
    			clearScreen();
    			viewReferralStatus(users,userCount,referrals,referralCount,patients,*patientCount,currentUser);
    			waitForInput();
				break;
			case 4:
            	clearScreen();
            	editUserDetails(currentUser, userCount, users);
            	break;
    		case 0:
    			printCentered("Logging out...");
    			break;
    		default:
    			printCentered("Invalid input.");
		}
	} while(choice!=0);
}

int findUserByID (User *users, int userCount, int input){
	int i, index=-1;
	for (i=0;i<userCount;i++){
		if (users[i].userID == input){
			index = i;
			i = userCount; // end loop
		}
	}
	
	return index;
}

int findUserByName (User *users, int userCount, char *input){
	int i, index=-1;
	for (i=0;i<userCount;i++){
		if (strcmp(users[i].name,input)==0){
			index = i;
			i = userCount; // end loop
		}
	}
	return index;
}

void printMainMenu (int height, int width){
	int i;
	printTitle(height,width);
	
	printf("%55s[1] Register account\n","");
	printf("%61s[2] Login\n","");
	printf("%57s[3] Exit program\n","");
	printf("%15s","");
	for (i=0;i<width-30;i++){
		printf("%c",205);
	}
	for(i=0;i<8;i++){
		printf("\n");
	}
}

void printTitle(int height, int width){
	char *title[] = {
        " ____         _    _               _     ____         __                          _ ",
        "|  _ \\  __ _ | |_ (_)  ___  _ __  | |_  |  _ \\  ___  / _|  ___  _ __  _ __  __ _ | |",
        "| |_) |/ _` || __|| | / _ \\| '_ \\ | __| | |_) |/ _ \\| |_  / _ \\| '__|| '__|/ _` || |",
        "|  __/| (_| || |_ | ||  __/| | | || |_  |  _ <|  __/|  _||  __/| |   | |  | (_| || |",
        "|_|    \\__,_| \\__||_| \\___||_| |_| \\__| |_| \\_\\\\___||_|   \\___||_|   |_|   \\__,_||_|",
        "            __  __                                                            _                ",
        "            |  \\/  |  __ _  _ __    __ _   __ _   ___  _ __ ___    ___  _ __  | |_               ",
        "            | |\\/| | / _` || '_ \\  / _` | / _` | / _ \\| '_ ` _ \\  / _ \\| '_ \\ | __|              ",
        "            | |  | || (_| || | | || (_| || (_| ||  __/| | | | | ||  __/| | | || |_               ",
        "            |_|  |_| \\__,_||_| |_| \\__,_| \\__, | \\___||_| |_| |_| \\___||_| |_| \\__|              ",
        "                             ____          |___/_                                                  ",
        "                                          / ___|  _   _  ___ | |_  ___  _ __ ___                                               ",
        "                                          \\___ \\ | | | |/ __|| __|/ _ \\| '_ ` _ \\                                              ",
        "                                           ___) || |_| |\\__ \\| |_|  __/| | | | | |                                             ",
        "                                          |____/  \\__, ||___/ \\__|\\___||_| |_| |_|                                             ",
        "                                                   |___/                                                                        "
    };
	int lines = 16, padding, i;
	
	for (i=0;i<width;i++){
		printf("%c",205);
	}
	printf("\n\n");
	padding = (width - 14) / 2;
	if (padding < 0)
		padding = 0;
	printf("%*sWelcome To The\n",padding,"");
	for (i=0;i<lines;i++){
		padding = (width - strlen(title[i]) + 4) / 2;
		printf("%*s%s\n",padding,"",title[i]);
	}
	printf("%15s","");
	for (i=0;i<width-30;i++){
		printf("%c",205);
	}
	printf("\n");
}

void clearScreen(){
	int i;
	for (i=0;i<HEIGHT;i++){
		printf("\n");
	}
}

void inputText(){
	int i;
	for (i=0;i<4;i++){
		printf("\n");
	}
}

void centerText(){
	int i;
	for (i=0;i<13;i++){
		printf("\n");
	}
}

void printBorder(){
	int i=0;
	printf("%18s","");
	for (i=0;i<WIDTH-36;i++){
		printf("%c",205);
	}
	printf("\n");
}

void printDivider(){
	int i=0;
	for (i=0;i<WIDTH;i++){
		printf("%c",205);
	}
}

void printCentered(const char* text){
	int len = strlen(text);
	int padding = (WIDTH - len) / 2;
	if (padding < 0){ // avoid negative, ie. %-10s
		padding = 0;
	}
	clearScreen();
	printf("%*s%s\n", padding, "", text);
}

void waitForInput(){
	char input[200];
	printf("\n%47sEnter any key to confirm/go back...\n","");
	getValidInput(input,4,0,0,0,0,0,0);
}

// For printing records/tables
void printHeader (int type, int widths[]){
	int i,j,k;
	int totalWidth = 0;
	
	if (type==1){
		for (k=0;k<5;k++){
			totalWidth += widths[k];
			totalWidth += 2;
		}
		totalWidth += 5 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH;
		}
		if (totalWidth > WIDTH){
			printf("\033[8;%d;%dt", HEIGHT, totalWidth); // resize terminal
		}
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// header
		printf("%c", 201);
		for (i=0;i<5;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<4)
				printf("%c",203);
		}
		printf("%c\n",187);
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// titles
		char *headers[5] = {"User ID", "Username", "Name", "Role", "Hospital"};
		for (i=0;i<5;i++){
			printf("%c %-*s ",186,widths[i],headers[i]);
		}
		printf("%c\n",186);
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// separator
		printf("%c",204);
		for (i=0;i<5;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<4)
				printf("%c",206);
		}
		printf("%c\n",185);
	}
	else if (type==2){
		for (k=0;k<10;k++){
			totalWidth += widths[k];
			totalWidth += 2;
		}
		totalWidth += 10 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH;
		}
		if (totalWidth > WIDTH){
			printf("\033[8;%d;%dt", HEIGHT, totalWidth); // resize terminal
		}
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// header
		printf("%c", 201);
		for (i=0;i<10;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<9)
				printf("%c",203);
		}
		printf("%c\n",187);
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// titles
		char *headers[10] = {"Patient ID","Name","Age","Gender","BMI","CVD","Diabetes","CardioRisk","Diagnosed","Referred"};
		for (i=0;i<10;i++){
			printf("%c %-*s ",186,widths[i],headers[i]);
		}
		printf("%c\n",186);
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// separator
		printf("%c",204);
		for (i=0;i<10;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<9)
				printf("%c",206);
		}
		printf("%c\n",185);
	}
	else if (type==3){
		for (k=0;k<5;k++){
			totalWidth += widths[k];
			totalWidth += 2;
		}
		totalWidth += 5 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH;
		}
		if (totalWidth > WIDTH){
			printf("\033[8;%d;%dt", HEIGHT, totalWidth); // resize terminal
		}
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// header
		printf("%c", 201);
		for (i=0;i<5;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<4)
				printf("%c",203);
		}
		printf("%c\n",187);
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// titles
		char *headers[5] = {"Referral ID", "GP Name", "Patient Name", "Specialist Name", "Status"};
		for (i=0;i<5;i++){
			printf("%c %-*s ",186,widths[i],headers[i]);
		}
		printf("%c\n",186);
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// separator
		printf("%c",204);
		for (i=0;i<5;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<4)
				printf("%c",206);
		}
		printf("%c\n",185);
	}
}

void printRows (int type, User *currentUser, User *users, Patient *patients, Referral *referrals, int count, int userCount, int widths[], int referralCount, char *filter){
	int i,index,j,gpindex;
	char referred;
	int totalWidth = 0;
	
	if (type==1){
		for (j=0;j<5;j++){
			totalWidth += widths[j];
			totalWidth += 2;
		}
		totalWidth+= 5 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH+1;
		}
		
		for (i=0;i<count;i++){
			if (strcmp(filter,users[i].role)==0 || strcmp(filter,"*")==0){
				printf("%*s",(WIDTH-totalWidth)/2,"");
				printf("%c %-*d %c %-*s %c %-*s %c %-*s %c %-*s %c\n",
					186, widths[0], users[i].userID,
					186, widths[1], users[i].username,
					186, widths[2], users[i].name,
					186, widths[3], users[i].role,
					186, widths[4], users[i].hospital, 186);
			}
		}
	}
	else if (type==2){
		for (j=0;j<10;j++){
			totalWidth+=widths[j];
			totalWidth+=2;
		}
		totalWidth+= 10 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH+1;
		}
		
		for (i=0;i<count;i++){
			if (patients[i].isReferred == 0){
				referred = 'N';
			}
			else if (patients[i].isReferred == 1){
				referred = 'Y';
			}
			
			
			printf("%*s",(WIDTH-totalWidth)/2,"");
			
			if (patients[i].cardioRisk!=-1){
				printf("%c %-*d %c %-*s %c %-*d %c %-*c %c %-*s %c %-*c %c %-*c %c %-*.2lf %c %-*c %c %-*c %c\n",
						186, widths[0], patients[i].patientID,
						186, widths[1], patients[i].name,
						186, widths[2], patients[i].age,
						186, widths[3], patients[i].gender,
						186, widths[4], patients[i].bmiCat,
						186, widths[5], patients[i].currentCVD,
						186, widths[6], patients[i].diabetes,
						186, widths[7], patients[i].cardioRisk,
						186, widths[8], patients[i].isDiagnosed,
						186, widths[9], referred, 186);
			}
			else {
				printf("%c %-*d %c %-*s %c %-*d %c %-*c %c %-*s %c %-*c %c %-*c %c %-*s %c %-*c %c %-*c %c\n",
						186, widths[0], patients[i].patientID,
						186, widths[1], patients[i].name,
						186, widths[2], patients[i].age,
						186, widths[3], patients[i].gender,
						186, widths[4], patients[i].bmiCat,
						186, widths[5], patients[i].currentCVD,
						186, widths[6], patients[i].diabetes,
						186, widths[7], "N/A",
						186, widths[8], patients[i].isDiagnosed,
						186, widths[9], referred, 186);
			}
			
		}
	}
	else if (type==3){
		for (j=0;j<5;j++){
			totalWidth+=widths[j];
			totalWidth+=2;
		}
		totalWidth+= 5 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH+1;
		}
		
		for (i=0;i<count;i++){
			if (strcmp(currentUser->role,"GP")==0){
				if (referrals[i].gpID == currentUser->userID){
					printf("%*s",(WIDTH-totalWidth)/2,"");
					index = findUserByID(users,userCount,referrals[i].specialistID);
					gpindex = findUserByID(users,userCount,referrals[i].gpID);
					printf("%c %-*d %c %-*d %c %-*s %c %-*s %c %-*s %c\n",
							186, widths[0], referrals[i].referralID,
							186, widths[1], users[gpindex].name,
							186, widths[2], referrals[i].patientName,
							186, widths[3], users[index].name,
							186, widths[4], referrals[i].status, 186);
				}
			}
			else if (strcmp(currentUser->role,"Specialist")==0){
				if (referrals[i].specialistID == currentUser->userID){
					printf("%*s",(WIDTH-totalWidth)/2,"");
					index = findUserByID(users,userCount,referrals[i].specialistID);
					gpindex = findUserByID(users,userCount,referrals[i].gpID);
					printf("%c %-*d %c %-*d %c %-*s %c %-*s %c %-*s %c\n",
							186, widths[0], referrals[i].referralID,
							186, widths[1], users[gpindex].name,
							186, widths[2], referrals[i].patientName,
							186, widths[3], users[index].name,
							186, widths[4], referrals[i].status, 186);
				}
			}
		}
	}
}

void printFooter (int type, int widths[]){
	int i,j,k;
	int totalWidth = 0;
	
	if (type==1){
		for (k=0;k<5;k++){
			totalWidth+=widths[k];
			totalWidth += 2;
		}
		totalWidth += 5 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH+1;
		}
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// footer
		printf("%c", 200);
		for (i=0;i<5;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<4)
				printf("%c",202);
		}
		printf("%c\n",188);
	}
	else if (type==2){
		for (k=0;k<10;k++){
			totalWidth+=widths[k];
			totalWidth += 2;
		}
		totalWidth += 10 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH+1;
		}
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// footer
		printf("%c", 200);
		for (i=0;i<10;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<9)
				printf("%c",202);
		}
		printf("%c\n",188);
	}
	else if (type==3){
		for (k=0;k<5;k++){
			totalWidth+=widths[k];
			totalWidth += 2;
		}
		totalWidth += 5 + 1;
		if ((WIDTH-totalWidth)/2 < 0){
			totalWidth=WIDTH+1;
		}
		
		printf("%*s",(WIDTH-totalWidth)/2,"");
		// footer
		printf("%c", 200);
		for (i=0;i<5;i++){
			for (j=0;j<widths[i]+2;j++){
				printf("%c",205);
			}
			if (i<4)
				printf("%c",202);
		}
		printf("%c\n",188);
	}
}

// for calculating column and categories' widths
// type determines what type to run
// count is amt of rows to be printed,,, referralCount, patientCount, userCount
void calculateWidths (int type, User *users, Patient *patients, Referral *referrals, int count, int userCount, int widths[]){
	int i;
	int index;
	
	if (type==1){ // Users
		widths[0] = strlen("user ID"); // since user ID will range from 1-100 no need to test
		widths[1] = strlen("username");
		widths[2] = strlen("name");
		widths[3] = strlen("role");
		widths[4] = strlen("hospital");
		
		for (i=0;i<userCount;i++){
			if (strlen(users[i].username) > widths[1])
				widths[1] = strlen(users[i].username);
			if (strlen(users[i].name) > widths[2])
				widths[2] = strlen(users[i].name);
			if (strlen(users[i].role) > widths[3])
				widths[3] = strlen(users[i].role);
			if (strlen(users[i].hospital) > widths[4])
				widths[4] = strlen(users[i].hospital);
		}
	}
	if (type==2){ // Patients
		char *headers[10] = {"Patient ID","Name","Age","Gender","BMI","CVD","Diabetes","CardioRisk","Diagnosed","Referred"};
		// only test for name and bmi, others are just 1-3 characters
		for (i=0;i<10;i++){
			widths[i] = strlen(headers[i]);
		}
		
		for (i=0;i<count;i++){
			if (strlen(patients[i].name) > widths[1])
				widths[1] = strlen(patients[i].name);
			if (strlen(patients[i].bmiCat) > widths[4])
				widths[4] = strlen(patients[i].bmiCat);
		}
	}
	if (type==3){ // Referrals
		widths[0] = strlen("Referral ID");
		widths[1] = strlen("GP Name");
		widths[2] = strlen("Patient Name");
		widths[3] = strlen("Specialist Name");
		widths[4] = strlen("Status");
		
		for (i=0;i<count;i++){
			index = findUserByID(users,userCount,referrals[i].gpID);
			if (strlen(users[index].name)>widths[1])
				widths[1] = strlen(users[index].name);
			if (strlen(referrals[i].patientName)>widths[2])
				widths[2] = strlen(referrals[i].patientName);
			index = findUserByID(users,userCount,referrals[i].specialistID);
			if (strlen(users[index].name)>widths[3])
				widths[3] = strlen(users[index].name);
			if (strlen(referrals[i].status)>widths[4])
				widths[4] = strlen(referrals[i].status);
		}
	}
}

void printUsers (User *currentUser, User *users, Patient *patients, Referral *referrals, int userCount, int referralCount, char *filter){
	int userWidths[5];
	int i;
	
	calculateWidths(1,users,patients,referrals,userCount,userCount,userWidths);
	
	if (userCount!=0){
		clearScreen();
		printf("%32s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  USERS  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
		
		printHeader(1,userWidths);
		printRows(1,currentUser,users,patients,referrals,userCount,userCount,userWidths,referralCount,filter);
		printFooter(1,userWidths);
		
		printf("\n");
	}
	else if (userCount==0){
		clearScreen();
		printf("%57sNo Records Found\n\n","");
	}
}

void printPatients (User *currentUser, User *users, Patient *patients, Referral *referrals, int patientCount, int userCount, int referralCount, char *filter){
	int patientWidths[10];
	int i;
	
	calculateWidths(2,users,patients,referrals,patientCount,userCount,patientWidths);
	
	if (patientCount!=0){
		clearScreen();
		printf("%27s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  PATIENT RECORDS  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
		
		printHeader(2,patientWidths);
		printRows(2,currentUser,users,patients,referrals,patientCount,userCount,patientWidths,referralCount,filter);
		printFooter(2,patientWidths);
		
		printf("\n");
	}
	else {
		clearScreen();
		printf("%57sNo Records Found\n\n","");
	}
}

void printReferrals (User *currentUser, User *users, Patient *patients, Referral *referrals, int referralCount, int userCount, char *filter){
	int referralWidths[5];
	int i;
	
	calculateWidths(3,users,patients,referrals,referralCount,userCount,referralWidths);
	
	if (referralCount!=0){
		clearScreen();
		printf("%30s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  REFERRALS  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
		
		printHeader(3,referralWidths);
		printRows(3,currentUser,users,patients,referrals,referralCount,userCount,referralWidths,referralCount,filter);
		printFooter(3,referralWidths);
		
		printf("\n");
	}
	else {
		clearScreen();
		printf("%57sNo Records Found\n\n","");
	}
}
