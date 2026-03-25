#include "Letran_Navarrosa_Machine-Project.h"

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
						// find if full name is in patient record/array
						pIndex = findPatientByName(patients,patientCount,newUser.name);
						if (pIndex!=-1){
							printf("A patient record already exists with a similar name.\nDo you want to link this account to your patient record? (Y/N): ");
							scanf(" %c", &cInput);
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
									printf("Invalid input.\n");	
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
						printf("Invalid input.\n"); 
						validChoice=0;
				}
			} while (validChoice==0);
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

// Finds max userID and sets new userID to be used
int getUserID (User *users, Patient *patients){
	int i,max=0;
	for (i=0;i<MAX_USERS;i++){
		if (users[i].userID > max){
			max = users[i].userID;
		}
		if (patients[i].userID > max){
			max = patients[i].userID;
		}
	}
	return max + 1;
}

// User Dashboards
// GP Dashboard
void gpDashboard (User *currentUser, Patient *patients, int *patientCount, User *users, int userCount, Referral *referrals, int *referralCount, double data[][2]){
	int i, choice = -1, pchoice = -1, rchoice = -1, order = -1;
	
    do{
        printf("\n==== GP DASHBOARD ====\n");
        printf("1. Add Patient\n");
        printf("2. Show Patients\n");
        printf("3. Show Referrals\n"); 
        printf("4. Compute Averages (BMI + Cardio Risk)\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf(" %d",&choice);
		
        switch(choice){
            case 1:
                if (*patientCount < MAX_USERS){
                    patients[*patientCount] = addPatient(currentUser,patients,*patientCount);
                    (*patientCount)++;
                }
                else
                	printf("Reached max patient count.");
                saveAllPatientsToFile(patients,*patientCount,"patients.txt");
                break;
			
            case 2:
                do{
            		showPatients(patients,*patientCount);
            		printf("\n==== Patient CRUD ====\n");
            		printf("1. Select patient by patientID\n");
            		printf("2. Select patient by name\n");
            		printf("3. Sort patient by userID\n");
            		printf("4. Sort patient by name\n");
            		printf("0. Exit\n");
            		printf("Choice: ");
            		scanf(" %d",&pchoice);
            		
            		switch(pchoice){
            			case 1:
            				selectPatientID(patients,&patientCount,referrals,users,currentUser,userCount,&referralCount);
							break;
            			case 2:
            				selectPatientName(patients,&patientCount,referrals,users,currentUser,userCount,&referralCount);
							break;
            			case 3:
            				printf("Order:\n1. Ascending\n0. Descending\nChoice: ");
            				scanf(" %d",&order);
            				sortPatientsByID(patients, *patientCount, order);
            				break;
            			case 4:
            				printf("Order:\n1. Ascending\n0. Descending\nChoice: ");
            				scanf(" %d",&order);
            				sortPatientsByName(patients, *patientCount, order);
            				break;
            			default:
            				printf("Invalid input.\n");
					}
				} while(pchoice!=0);
                break;
            case 3:
                do{
                	showReferrals(currentUser,users,referrals,*referralCount);
                	printf("\n==== Referral CRUD ====\n");
            		printf("1. Select referrals by ID\n");
            		printf("2. Sort referrals by ID\n");
            		printf("3. Sort referrals by Status?\n");
            		printf("0. Exit\n");
            		printf("Choice: ");
				} while(rchoice!=0);
				
                
                // saveAllReferralsToFile(referrals,referralCount,"referrals.txt"); SAVE WHEN CREATING REFERRALS
                break;
            case 4:
            	for (i=0;i<*patientCount;i++){
            		data[i][0] = patients[i].bmi;
            		data[i][1] = patients[i].cardioRisk;
				}
                computeAverages(data,*patientCount);
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid input.\n");
        }
    } while(choice!=0);
}

void specialistDashboard(User *currentUser, User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, double data[][2]){
	int i, choice=-1, pchoice=-1, rchoice=-1, order=-1;
    do{
        printf("\n==== SPECIALIST DASHBOARD ====\n");
        printf("1. Show Referrals\n");
        printf("2. Update Referral Status\n");
        printf("3. Show patient list.\n");
        printf("4. Compute Averages (BMI + Cardio Risk)\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf(" %d",&choice);
        
        switch(choice){
            case 1:
                do{
                	showReferrals(currentUser,users,referrals,*referralCount);
                	printf("\n==== Referral CRUD ====\n");
            		printf("1. Select referrals by ID\n");
            		printf("2. Sort referrals by ID\n");
            		printf("3. Sort referrals by Status?\n");
            		printf("0. Exit\n");
            		printf("Choice: ");
				} while(rchoice!=0);
                break;
            case 2:
                editReferral(referrals, referralCount);
                break;
            case 3:
            	do{
            		showPatients(patients,patientCount);
            		printf("\n==== Patient CRUD ====\n");
            		printf("1. Select patient by patientID\n");
            		printf("2. Select patient by name\n");
            		printf("3. Sort patient by userID\n");
            		printf("4. Sort patient by name\n");
            		printf("Choice: ");
            		scanf(" %d",&pchoice);
            		
            		switch(pchoice){
            			case 1:
            				selectPatientID(patients,&patientCount,referrals,users,currentUser,userCount,&referralCount);
							break;
            			case 2:
            				selectPatientName(patients,&patientCount,referrals,users,currentUser,userCount,&referralCount);
            				break;
            			case 3:
            				printf("Order:\n1. Ascending\n0. Descending\nChoice: ");
            				scanf(" %d",&order);
            				sortPatientsByID(patients, patientCount,order);
            				break;
            			case 4:
            				printf("Order:\n1. Ascending\n0. Descending\nChoice: ");
            				scanf(" %d",&order);
            				sortPatientsByName(patients, patientCount, order);
            				break;
            			default:
            				printf("Invalid input.\n");
					}
				} while(pchoice!=0);
            	break;
            case 4:
            	for (i=0;i<patientCount;i++){
            		data[i][0] = patients[i].bmi;
            		data[i][1] = patients[i].cardioRisk;
				}
            	computeAverages(data,patientCount);
            	break;
            case 0:
            	printf("Logging out...\n");
                break;
            default:
                printf("Invalid input.\n");
        }
    } while(choice!=0);
}

void patientDashboard(User *currentUser, User *users, Patient *patients, int *patientCount, Referral *referrals, int referralCount){
    int i, found=0, choice=-1;

    do{
    	printf("\n==== PATIENT DASHBOARD ====\n");
    	printf("1. Add self as patient\n");
    	printf("2. View diagnosis reports\n");
    	printf("3. View referrals");
    	printf("0. Logout\n");
    	printf("Choice: ");
    	scanf(" %d",&choice);
    	
    	switch(choice){
    		case 1:
    			if (*patientCount < MAX_USERS){
                    patients[*patientCount] = addPatient(currentUser,patients,*patientCount);
                    (*patientCount)++;
                }
                else{
                	printf("Max patient count reached.");
				}
    			break;
    		case 2:
    			printf("\n===== Diagnosis report ====\n\n");
    			for (i=0;i<*patientCount;i++){
					if (strcmp(patients[i].name,currentUser->name)==0){
						showDiagnosisReport(&patients[i]);
			            found=1;
			        }
			    }
			    if (!found){
    			    printf("No record found.\n");
    			}
    			break;
    		case 3:
    			showReferrals(currentUser, users, referrals, referralCount);
    			break;
    		case 0:
    			printf("Logging out...\n");
    			break;
    		default:
    			printf("Invalid input.\n");
		}
	} while(choice!=0);
}

int findUserByID (User *users, int userCount, int input){
	int i, index=-1;
	for (i=0;i<userCount;i++){
		if (users[i].userID == input){
			index = i;
			i = patientCount; // end loop
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
	printf("USERS HERE");
}
