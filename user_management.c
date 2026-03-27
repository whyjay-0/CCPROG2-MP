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
					printCentered("Invalid input. Enter an integer.");
					clearInput();
				}
				else if (*(int*)input < min || *(int*)input > max){ // *(int*)var , value inside the pointer to int
					printf("%50sOut of range (%d-%d). Try again.", "", min, max);
				}
				else 
					valid=1;
				break;
			case 2: // float
				scan = scanf("%f", (float*)input);
				if (scan != 1){
					printCentered("Invalid input. Enter an float.");
					clearInput();
				}
				else if (*(float*)input < min || *(float*)input > max){
					printf("%50sOut of range (%d-%d). Try again.", "", min, max);
				}
				else
					valid=1;
				break;
			case 3: // char
				scan = scanf(" %c", (char*)input);
				if (scan != 1){
					printf("Invalid input. Enter a character: ");
					clearInput();
				}
				else if (*(char*)input != limit1 && *(char*)input != limit2 && *(char*)input != limit3 && *(char*)input != limit4)
					printf("Invalid input. Enter %c or %c:", limit1, limit2);
				else
					valid=1;
				break;
			case 4: // string 100
				scan = scanf(" %100[^\n]", (char*)input);
				if (scan != 1){
					printf("Invalid input.\n");
					clearInput();
				}
				else if (strlen((char*)input)==0){
					printf("Empty input. Try again: ");
				}
				else
					valid=1;
				break;
			case 5: // string 16
				scan = scanf(" %16[^\n]", (char*)input);
				if (scan != 1){
					printf("Invalid input.");
					clearInput();
				}
				else if (strlen((char*)input)==0){
					printf("Empty input. Try again: ");
				}
				else
					valid=1;
				break;
			case 6: // string 15
				scan = scanf(" %15[^\n]", (char*)input);
				if (scan != 1){
					printf("Invalid input.");
					clearInput();
				}
				else if (strlen((char*)input)==0){
					printf("Empty input. Try again: ");
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
	int choice, complete=0, validUser=0, validName=0, validChoice=0, index, pIndex;
	
	if (*userCount >= MAX_USERS){
		printCentered("Max User Limit reached.");
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
					printCentered("Invalid Username.");
				}
				else {
					strcpy(newUser.username, input);
					validUser=1;
				}
			} while (validUser==0);
			
			do{
				validName=0;
				printCentered("Please enter your full name [100 Characters Max].");
				getValidInput(input,4,0,0,0,0,0,0);
				strcpy(newUser.name, input);
				validName=1;
			} while (validName==0);
			
			printCentered("Please enter password.");
			getValidInput(input,4,0,0,0,0,0,0);
			hashPassword(input, &newUser.passwordHash);
			
			do{
				validChoice=0;
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
							}
						}
						else {
							newUser.userID = getUserID(users,patients);
						}
						validChoice=1; 
						complete=1; 
						break;
					case 2: 
						strcpy(newUser.role, "GP"); 
						newUser.userID = getUserID(users,patients); // set the userID of new user to highest userID found + 1
						validChoice=1; 
						complete=1; 
						break;
					case 3: 
						strcpy(newUser.role, "Specialist");
						newUser.userID = getUserID(users,patients);
						validChoice=1; 
						complete=1; 
						break;
					default: 
						printCentered("Invalid input.");
						validChoice=0;
				}
			} while (validChoice==0);
			clearScreen();
			printf("%51sSelect a security question:\n%49s  [1] What is your favorite food?\n%49s  [2] What is your favorite color?\n%46s  [3] What is your favorite animal/pet?\n","","","","");
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
				printCentered("Confirm your password.");
			else
				printCentered("Enter your password. If you have forgotten, input 0.");
			if (matching==0){
				printf("\n%57sWrong password.","");
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
					printCentered("Invalid username.");
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
// Save user to TXT file, return 1 if success, 0 otherwise
int saveAllUsersToFile (User *users, int userCount, const char *filename){
	FILE *fp;
	int i, flag = 0;
	if ((fp=fopen(filename, "w"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		for (i=0;i<userCount;i++){
			fprintf(fp, "%d, %s, %lu, %s, %s, %d, %s\n", users[i].userID, users[i].username, users[i].passwordHash, users[i].role, users[i].name, users[i].questType, users[i].answer);
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
			
			result = fscanf(fp, "%d, %100[^,], %lu, %100[^,], %100[^,], %d, %100[^\n]", &temp.userID, temp.username, &temp.passwordHash, temp.role, temp.name, &temp.questType, temp.answer);
			// since fscanf outputs the amount of input items, we can use it to check if fscanf was successful
			if (result==7 && count < MAX_USERS){
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
	int flag=0,i,valid=0;
	char newPass[101];
	char input[101];
	for (i=0;i<userCount;i++){
		if (strcmp(users[i].username,username)==0){
			
			if (users->questType == 1){
				printCentered("What is your favorite food?");
				getValidInput(input,4,0,0,0,0,0,0);
				
				if (strcmp(input,users->answer)==0){
					valid=1;
				}
			}
			else if (users->questType == 2){
				printCentered("What is your favorite color?");
				getValidInput(input,4,0,0,0,0,0,0);
				
				if (strcmp(input,users->answer)==0){
					valid=1;
				}
			}
			else if (users->questType == 3){
				printCentered("What is your favorite animal/pet?");
				getValidInput(input,4,0,0,0,0,0,0);
				
				if (strcmp(input,users->answer)==0){
					valid=1;
				}
			}
			
			if (valid){
				printCentered("Please enter new password.");
				getValidInput(newPass,4,0,0,0,0,0,0);
				hashPassword(newPass, &users[i].passwordHash); // Need validity check for passwords
				printCentered("Successfully Changed Password!");
				saveAllUsersToFile(users, userCount, "users.txt"); // save new password
				flag = 1;
			}
			else {
				printCentered("Invalid answer. Exiting...");
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
void gpDashboard (User *currentUser, Patient *patients, int *patientCount, User *users, int userCount, Referral *referrals, int *referralCount, double data[][2]){
	int i, choice = -1, pchoice = -1, rchoice = -1, order = -1;
	
    do{
        clearScreen();
        
        printf("%27s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  GP DASHBOARD  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
        printf("%57s[1] Add Patient\n","");
        printf("%56s[2] Show Patients\n","");
        printf("%56s[3] Show Referrals\n",""); 
        printf("%44s[4] Compute Averages (BMI and Cardio Risk)\n","");
        printf("%60s[0] Logout","");
		getValidInput(&choice,1,0,4,0,0,0,0);
		
        switch(choice){
            case 1:
                if (*patientCount < MAX_USERS){
                    patients[*patientCount] = addPatient(currentUser,patients,*patientCount,users,userCount);
                    (*patientCount)++;
                }
                else
                	printCentered("Reached max patient count.");
                saveAllPatientsToFile(patients,*patientCount,"patients.txt");
                break;
			
            case 2:
                do{
            		clearScreen();
					showPatients(patients,*patientCount);
            		
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Patient CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%49s[1] Select patient by patientID\n","");
            		printf("%52s[2] Select patient by name\n","");
            		printf("%52s[3] Sort patient by userID\n","");
            		printf("%53s[4] Sort patient by name\n","");
            		printf("%61s[0] Exit","");
					getValidInput(&pchoice,1,0,4,0,0,0,0);
            		
            		switch(pchoice){
            			case 1:
            				selectPatientID(patients,patientCount,referrals,users,currentUser,userCount,referralCount);
							break;
            			case 2:
            				selectPatientName(patients,patientCount,referrals,users,currentUser,userCount,referralCount);
							break;
            			case 3:
            				printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortPatientsByID(patients, *patientCount, order);
            				break;
            			case 4:
            				printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
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
					showReferrals(currentUser,users,referrals,*referralCount);
					
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Referral CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%52s[1] Select referrals by ID\n","");
            		printf("%53s[2] Sort referrals by ID\n","");
            		printf("%51s[3] Sort referrals by Status\n","");
            		printf("%61s[0] Exit","");
            		
            		getValidInput(&rchoice,1,0,3,0,0,0,0);
            		
            		switch(rchoice){
            			case 1:
            				selectReferralID(currentUser,referrals,referralCount,users,userCount,patients,*patientCount);
							break;
            			case 2:
            				printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortReferralsByID(referrals,*referralCount,order);
            				break;
            			case 3:
            				printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
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
            case 0:
    			printCentered("Logging out...");
    			break;
            default:
                printCentered("Invalid input.");
        }
    } while(choice!=0);
}

void specialistDashboard(User *currentUser, User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, double data[][2]){
	int i, choice=-1, pchoice=-1, rchoice=-1, order=-1;
    do{
        clearScreen();
        
        printf("%24s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  SPECIALIST DASHBOARD  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
		
        printf("%56s[1] Show Referrals\n","");
        printf("%54s[2] Show patient list.\n","");
        printf("%45s[3] Compute Averages (BMI + Cardio Risk)\n","");
        printf("%60s[0] Logout","");
        getValidInput(&choice,1,0,3,0,0,0,0);
        
        switch(choice){
            case 1:
                do{
                	clearScreen();
					showReferrals(currentUser,users,referrals,referralCount);
                	
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Referral CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%52s[1] Select referrals by ID\n","");
            		printf("%53s[2] Sort referrals by ID\n","");
            		printf("%51s[3] Sort referrals by Status\n","");
            		printf("%61s[0] Exit","");
            		getValidInput(&rchoice,1,0,3,0,0,0,0);
            		
            		switch(rchoice){
            			case 1:
            				clearScreen();
							selectReferralID(currentUser,referrals,&referralCount,users,userCount,patients,patientCount);
							break;
            			case 2:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortReferralsByID(referrals,referralCount,order);
            				break;
            			case 3:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
            				getValidInput(&order,1,0,1,0,0,0,0);
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
					showPatients(patients,patientCount);
					
					printf("%27s","");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("  Patient CRUD  ");
					for (i=0;i<WIDTH-100;i++){
						printf("%c",205);
					}
					printf("\n");
					
            		printf("%49s[1] Select patient by patientID\n","");
            		printf("%52s[2] Select patient by name\n","");
            		printf("%52s[3] Sort patient by userID\n","");
            		printf("%53s[4] Sort patient by name\n","");
            		printf("%61s[0] Exit","");
            		
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
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
            				getValidInput(&order,1,0,1,0,0,0,0);
            				sortPatientsByID(patients, patientCount,order);
            				break;
            			case 4:
            				clearScreen();
							printf("%61sOrder:\n%59s  1. Ascending\n%58s  0. Descending");
            				getValidInput(&order,1,0,1,0,0,0,0);
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

    do{
    	clearScreen();
    	
    	printf("%25s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  PATIENT DASHBOARD  ");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
    	
    	printf("%53s[1] Add self as patient\n","");
    	printf("%52s[2] View diagnosis report\n","");
    	printf("%55s[3] View my referral\n","");
    	printf("%60s[0] Logout","");
    	
    	getValidInput(&choice,1,0,3,0,0,0,0);
    	switch(choice){
    		case 1:
    			clearScreen();
    			if (*patientCount < MAX_USERS){
                    patients[*patientCount] = addPatient(currentUser,patients,*patientCount,users,userCount);
                    (*patientCount)++;
                }
                else{
                	printCentered("Max patient count reached.");
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
    			    waitForInput();
    			}
    			waitForInput();
    			break;
    		case 3:
    			clearScreen();
    			viewReferralStatus(users,userCount,referrals,referralCount,patients,*patientCount,currentUser);
    			waitForInput();
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

void printUsers (User *users, int userCount, char *filter){ // filter will be like "Specialist" "Patient" "*" or "GP"
	int i;
	clearScreen();
	printf("%26s","");
	for (i=0;i<WIDTH-100;i++){
		printf("%c",205);
	}
	printf("  List of Users  ");
	for (i=0;i<WIDTH-100;i++){
		printf("%c",205);
	}
	printf("\n");
	for (i=0;i<userCount;i++){
		if (strcmp(filter,users[i].role)==0){
			printf("%d | %s | %s | %s\n", users[i].userID,users[i].username,users[i].name,users[i].role);
		}
		else if (strcmp(filter,"*")==0){
			printf("%d | %s | %s | %s\n", users[i].userID,users[i].username,users[i].name,users[i].role);
		}
	}
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

void printCentered(char* text){
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
	printf("%47sEnter any key to confirm/go back...\n","");
	getValidInput(input,4,0,0,0,0,0,0);
}
