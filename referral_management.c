#include "Letran_Navarrosa_Machine-Project.h"

// Referral management
void createReferral (Referral *referrals, User *users, Patient *currentPatient, User currentUser, int userCount, int *referralCount){ 
	// GP access only, currentpatient is patient being referred, user is GP
	// select patient by entering name,,, would search by strcmp name and strcmp role
	// select specialist by entering name,,, would search by strcmp name and strcmp role
	// save referral
	int found=0,choice=-1, index, valid=0, input;
	char strInput[101];
	Referral newReferral;
	User specialist;
	newReferral.referralID = 0;
	newReferral.gpID = 0;
	newReferral.specialistID = 0;
	
	strcpy(newReferral.status, "");
	strcpy(newReferral.patientName,currentPatient->name);
	
	printf("Referring Patient: %s\n", newReferral.patientName);
	if (currentPatient->isDiagnosed=='Y'){
		printf("Enter details of specialist by:\n1. ID\n2. Name\nChoice: ");
		scanf(" %d", &choice);
		switch(choice){
			case 1:
				do{
					// show users func, lists all users but only specialists.
					printf("Enter user ID to select: ");
					valid = scanf(" %d", &input);
					if (valid != 1){
						// invalid input
						printf("Invalid input.\n");
						scanf("%*s"); // clear input
					}
				} while(valid==0);
			
				index = findUserByID(users,userCount,input); // index of user
				if (index!=-1){
					found=1;
					specialist = users[index];
				}
				break;
			case 2:
				// show users func, lists all users but only specialists.
				printf("Enter name of user to select: ");
				scanf(" %100[^\n]s", strInput);
				index = findUserByName(users,userCount,strInput);
				if (index!=-1){
					found=1;
					specialist = users[index];
				}
				break;
			default:
				printf("Invalid choice.\n");
		}
		if (found){
			newReferral.specialistID = specialist.userID; // found specialist
			newReferral.gpID = currentUser.userID; // assumed current user is GP
				
			strcpy(newReferral.status,"Pending"); // req will be sent to specialist, where they will need to accept or complete or reject
				
			// setting new ID for referral
			newReferral.referralID = *referralCount + 1; // set the referralID of new referral to old referralCount + 1
			referrals[*referralCount] = newReferral; //set the array of struct at index *referrakCount to the newReferral made
			// index = *userCount; // can be used if we want to return ID of the new referral
			(*referralCount)++; // increase amount of referrals
			
			saveAllReferralsToFile(referrals, *referralCount, "referrals.txt");
		}
	}
	else {
		printf("Patient is not yet diagnosed.\n");
	}
}

void showReferrals (User *currentUser, User *users, Referral *referrals, int referralCount){
    int i, j,found=0;
    User gp, specialist;
	
    printf("\n===== REFERRALS =====\n");
    for (i=0;i<referralCount;i++){
        if (strcmp(currentUser->role,"GP")==0){
            if (referrals[i].gpID == currentUser->userID){
                printf("%d | %s | Specialist ID: %d | %s\n",
                    referrals[i].referralID,
                    referrals[i].patientName,
                    referrals[i].specialistID,
                    referrals[i].status);
                found=1;
            }
        }
        else if (strcmp(currentUser->role,"Specialist")==0){
            if (referrals[i].specialistID == currentUser->userID){
                printf("%d | %s | Status: %s\n",
                    referrals[i].referralID,
                    referrals[i].patientName,
                    referrals[i].status);
                found=1;
            }
        }
        else if (strcmp(currentUser->role,"Patient")==0){
        	if (strcmp(referrals[i].patientName,currentUser->name) == 0){
        		for (j=0;j<MAX_USERS;j++){ // search name by userID GP
        			if (referrals[i].gpID == users[j].userID){
        				gp = users[j];
					}
					if (referrals[i].specialistID == users[j].userID){
						specialist = users[j];
					}
				}
        		printf("%d | GP: %s | Specialist: %s", referrals[i].referralID, gp.name, specialist.name);
			}
		}
    }

    if (found==0){
        printf("No referrals found.\n");
    }
}

void editReferral (Referral *referral){
	int choice, valid;
    printf("[1] Accept\n[2] Complete\n[3] Reject\nChoice: ");
    valid = scanf("%d",&choice);
	
	if (valid){
		switch(choice){
        	case 1:
       	     strcpy(referral->status,"Accepted");
        	    break;
        	case 2:
        	    strcpy(referral->status,"Completed");
        	    break;
        	case 3:
        	    strcpy(referral->status,"Rejected");
        	    break;
        	default:
        	    printf("Invalid choice.\n");
    	}
	}
	else {
		printf("Invalid input.\n\n");
	}
}

void deleteReferral (Referral *referrals, int *referralCount, int index){
    int i;
	
    for (i=index;i<*referralCount-1;i++){
        referrals[i] = referrals[i+1];
        }
	(*referralCount)--;
	printf("\nSucessfully deleted referral #%d\n", referrals[i].referralID);
}

int saveAllReferralsToFile (Referral *referrals, int referralCount, const char *filename){
    FILE *fp;
    int flag = 0, i;

    if ((fp=fopen(filename, "w"))==NULL){
        fprintf(stderr, "Error opening file.\n");
    }
    else{
        for (i=0;i<referralCount;i++){
        	fprintf(fp, "%d,%d,%s,%d,%s\n",
            	referrals[i].referralID,
            	referrals[i].gpID,
            	referrals[i].patientName,
            	referrals[i].specialistID,
            	referrals[i].status);
		}
        flag = 1;
        fclose(fp);
    }
    return flag;
}

int loadReferralsFromFile (Referral *referrals, const char *filename){
    FILE *fp;
    int count = 0, flag=1, result;

    if ((fp=fopen(filename, "r"))==NULL){
        fprintf(stderr, "Error opening file.\n");
    }
    else{
        while(flag){
            Referral temp;

            result = fscanf(fp, "%d,%d,%100[^,],%d,%30[^\n]",
                &temp.referralID,
                &temp.gpID,
                temp.patientName,
                &temp.specialistID,
                temp.status);

            if (result==5){
                referrals[count] = temp;
                count++;
            }
            else{
                flag=0;
            }
        }
        fclose(fp);
    }

    return count;
}

void sortReferralsByID (Referral *referrals, int referralCount, int order){
	int i, j, index;
	Referral temp;
	
	for (i=0;i<referralCount-1;i++){
		index = i; // sorted portion at i
		// search/select
		for (j=i+1;j<referralCount;j++){
			if (order==1){ // ascending
				if(referrals[j].referralID < referrals[index].referralID)
					index = j;
			}
			else { // descending
				if(referrals[j].referralID > referrals[index].referralID)
					index = j;
			}
		}
		// swap
		if (index != i){
			temp = referrals[i];
			referrals[i] = referrals[index];
			referrals[index] = temp;
		}
	}
}

void sortReferralsByStatus (Referral *referrals, int referralCount, int order){
	int i, j, index;
	Referral temp;
	
	for (i=0;i<referralCount-1;i++){
		index = i; // sorted portion at i
		for (j=i+1;j<referralCount;j++){
			if (order==1){ // ascending
				if(strcmp(referrals[j].status, referrals[index].status) < 0)
					index = j;
			}
			else { // descending
				if(strcmp(referrals[j].status, referrals[index].status) > 0)
					index = j;
			}
		}
		// swap
		if (index != i){
			temp = referrals[i];
			referrals[i] = referrals[index];
			referrals[index] = temp;
		}
	}
}

int findReferralByID (Referral *referrals, int referralCount, int input){
	int i, index=-1;
	for (i=0;i<referralCount;i++){
		if (referrals[i].referralID == input){
			index = i;
			i = referralCount; // end loop
		}
	}
	
	return index;
}

void selectReferralID (User *currentUser, Referral *referrals, int *referralCount, User *users){
	int input, choice;
	int index, valid=0;
	char cInput;
	
	do{
		printf("Enter Referral ID to select: ");
		valid = scanf(" %d", &input);
		if (valid==0){
			scanf("%*[^\n]"); // clear inputs until newline
			printf("Invalid input.\n\n");
		}
	} while(valid==0);
	
	
	index = findReferralByID(referrals,*referralCount,input);
	
	if (index == -1){
		printf("Patient not found.\n");
	}
	else if (strcmp(currentUser->role,"GP")==0 && index!=-1){
		do{
			showReferrals(currentUser,users,referrals,*referralCount);
			printf("\n==== Referral CRUD ====\n");
			printf("1. Delete Referral\n");
			printf("0. Exit\n");
			printf("Choice: ");
    	    scanf(" %d", &choice);
    	    
    	    switch(choice){
    	    	case 1:
    	    		printf("Are you sure you want to delete referral #%d? (Y/N): ", referrals[index].referralID);
    	    		valid = 0;
    	    		while (valid==0){
    	    			valid = scanf(" %c", &cInput);
    	    			if (valid==0){
    	    				printf("Invalid input.\n");
						}
						else{
							switch (cInput){
    	    					case 'Y':
    	    					case 'y':
									deleteReferral(referrals,referralCount,index);
    	    						break;
    	    					case 'N':
    	    					case 'n':
    	    						break;
    	    					default:
    	    						printf("Invalid input.\n");
							}
						}
					}
					saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
					break;
				case 0:
    	    		printf("Exiting...\n\n");
    	    		break;
			}
		} while (choice!=0);
	}
	else if (strcmp(currentUser->role,"Specialist")==0 && index!=-1){
		do{
			showReferrals(currentUser,users,referrals,*referralCount);
			printf("\n==== Referral CRUD ====\n");
			printf("1. Edit status of referral\n");
    	    printf("2. Delete referral\n");
    	    printf("0. Exit\n");
    	    printf("Choice: ");
    	    scanf(" %d", &choice);
    	    
    	    switch(choice){
    	    	case 1:
   		     		editReferral(&referrals[index]);
   		     		saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
    	    		break;
    	    	case 2:
    	    		printf("Are you sure you want to delete referral #%d? (Y/N): ", referrals[index].referralID);
    	    		valid = 0;
    	    		while (valid==0){
    	    			valid = scanf(" %c", &cInput);
    	    			if (valid==0){
    	    				printf("Invalid input.\n");
						}
						else{
							switch (cInput){
    	    					case 'Y':
    	    					case 'y':
									deleteReferral(referrals,referralCount,index);
    	    						break;
    	    					case 'N':
    	    					case 'n':
    	    						break;
    	    					default:
    	    						printf("Invalid input.\n");
							}
						}
					}
					saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
    	    		break;
    	    	case 0:
    	    		printf("Exiting...\n\n");
    	    		break;
    	    	default:
    	    		printf("Invalid input.\n");
			}
		} while(choice!=0);
	}
}





// utility funcs, sorting and searching of diff types by diff means
/*
User findUserByName (){ //this function would search for the user that matches the search you made by name
	// would find the specific user that matches the role searching for,,, 
	// will be used for createReferral so its easier to find IDs for the referral struct
	printf("temp\n");
}

User findUserByID (int userID){ // returns User being searched for
	printf("temp\n");
} // can be used when printing records of referrals, binary search maybe
// we need search and sort functions also, but the sort should only be visual, it will not change any of their data
// So it could be sort by userID, by name, or by role
// we could apply it sa specialist? maybe they could see all users? or better if may admin acc
// we could also apply to list of patients within patient_management.c
// and referral list,,, this is where referral date might be good
*/

/*
2D arrays will be implemented through computation averages of BMI and CardioRisk for Specialists
*/
