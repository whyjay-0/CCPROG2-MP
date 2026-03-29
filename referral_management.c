#include "Letran_Navarrosa_Machine-Project.h"

// Referral management
void createReferral (Referral *referrals, User *users, Patient *currentPatient, User currentUser, int userCount, int *referralCount, Patient *patients){ 
	// GP access only, currentpatient is patient being referred, user is GP
	// select patient by entering name,,, would search by strcmp name and strcmp role
	// select specialist by entering name,,, would search by strcmp name and strcmp role
	// save referral
	int found=0,choice=-1, index, input;
	char strInput[101];
	Referral newReferral;
	User specialist;
	newReferral.referralID = 0;
	newReferral.gpID = 0;
	newReferral.specialistID = 0;
	
	strcpy(newReferral.status, "");
	strcpy(newReferral.patientName,currentPatient->name);
	
	int padding = (WIDTH - 19 - strlen(newReferral.patientName)) / 2;
	printf("%*sReferring Patient: %s\n", padding, "", newReferral.patientName);
	if ((currentPatient->isDiagnosed=='Y' || currentPatient->isDiagnosed=='y') && currentPatient->isReferred==0 && *referralCount < MAX_USERS){
		clearScreen();
		printf("%49sEnter details of specialist by:\n%62s[1] ID\n%61s[2] Name","","","");
		getValidInput(&choice,1,1,2,0,0,0,0);
		switch(choice){
			case 1:
				// show users func, lists all users but only specialists.
				clearScreen();
				printUsers(&currentUser,users,patients,referrals,userCount,*referralCount,"Specialist");
				printf("\n\n");
				printf("%53sEnter user ID to select","");
				getValidInput(&input,1,0,500,0,0,0,0);
			
				index = findUserByID(users,userCount,input); // index of user
				if (index!=-1 && strcmp(users[index].role,"Specialist")==0){
					found=1;
					specialist = users[index];
				}
				break;
			case 2:
				// show users func, lists all users but only specialists.
				clearScreen();
				printUsers(&currentUser,users,patients,referrals,userCount,*referralCount,"Specialist");
				printf("\n\n");
				printf("%50sEnter name of user to select","");
				getValidInput(strInput,4,0,0,0,0,0,0);
				
				index = findUserByName(users,userCount,strInput);
				if (index!=-1 && strcmp(users[index].role,"Specialist")==0){
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
			newReferral.patientID = currentPatient->patientID;
				
			strcpy(newReferral.status,"Pending"); // req will be sent to specialist, where they will need to accept or complete or reject
				
			// setting new ID for referral
			newReferral.referralID = getReferralID(referrals,*referralCount); // set the referralID of new referral to old referralCount + 1
			referrals[*referralCount] = newReferral; //set the array of struct at index *referrakCount to the newReferral made
			// index = *userCount; // can be used if we want to return ID of the new referral
			(*referralCount)++; // increase amount of referrals
			
			currentPatient->isReferred = 1;
		}
		if (found==0) {
			printCentered("Specialist not found");
			waitForInput();
		}
	}
	else if (currentPatient->isDiagnosed=='N' || currentPatient->isDiagnosed=='n'){
		printCentered("Patient is not yet diagnosed");
		waitForInput();
	}
	else {
		printCentered("Max Referrals Reached");
		waitForInput();
	}
}

void viewReferralStatus(User *users, int userCount, Referral *referrals, int referralCount, Patient *patients, int patientCount, User *currentUser){
	int index=-1, i, j;
	int gpIndex, spIndex;
	// search index of the referral based on userID of patient
	
	// identify the user's role
	for (i=0;i<referralCount;i++){
		for (j=0;j<patientCount;j++){
			if ((strcmp(currentUser->role,"Patient")==0 && referrals[i].patientID == patients[j].patientID) || 
				(strcmp(currentUser->role,"GP")==0 && referrals[i].gpID == currentUser->userID) || 
				(strcmp(currentUser->role,"Specialist")==0 && referrals[i].specialistID == currentUser->userID)){
				index = i;
				i=referralCount;
			}
		}
	}
	
	if (index!=-1){
		// search gp index
		gpIndex = findUserByID(users,userCount,referrals[index].gpID);
		// search sp index
		spIndex = findUserByID(users,userCount,referrals[index].specialistID);
	}
	
	// print
	if (index!=-1){
		
		printf("%25s","");
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("  Referral %d Status  ", referrals[index].referralID);
		for (i=0;i<WIDTH-100;i++){
			printf("%c",205);
		}
		printf("\n");
		
		printf("%57sReferral ID: %d\n", "",referrals[index].referralID);
		int padding = (WIDTH - 14 - strlen(referrals[index].patientName)) / 2;
		printf("%*sPatient Name: %s\n\n", padding, "", referrals[index].patientName);
		padding = (WIDTH - 18 - strlen(users[gpIndex].name)) / 2;
		printf("%*sReferred By (GP): %s\n", padding, "", users[gpIndex].name);
		padding = (WIDTH - 21 - strlen(users[spIndex].name)) / 2;
		printf("%*sAssigned Specialist: %s\n\n", padding, "", users[spIndex].name);
		padding = (WIDTH - 42 - strlent(users[spIndex].hospital)) / 2;
		printf("%*sAssigned Specialist's Affiliated Hospital:\n",padding,"");
		printf("%*s     %s\n",padding,"",users[spIndex].hospital);
		padding = (WIDTH - 8 - strlen(referrals[index].status)) / 2;
		printf("%*sStatus: %s\n\n", padding, "", referrals[index].status);
	}
	else {
		printCentered("No Referral found.\n");
	}
}

void editReferral (Referral *referral){
	int choice;
    printf("%57sEdit Status to:\n%60s[1] Accept\n%59s[2] Complete\n%60s[3] Reject", "", "", "", "");
    getValidInput(&choice,1,1,3,0,0,0,0);
	
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
            printCentered("Invalid choice.\n");
            waitForInput();
    }
}

void deleteReferral (Referral *referrals, int *referralCount, int index){
    int i;
	
    for (i=index;i<*referralCount-1;i++){
        referrals[i] = referrals[i+1];
        }
	(*referralCount)--;
	
	referrals[*referralCount] = (Referral){0};
}

int saveAllReferralsToFile (Referral *referrals, int referralCount, const char *filename){
    FILE *fp;
    int flag = 0, i;

    if ((fp=fopen(filename, "w"))==NULL){
        fprintf(stderr, "Error opening file.\n");
    }
    else{
        for (i=0;i<referralCount;i++){
        	fprintf(fp, "%d, %d, %d, %s, %d, %s\n",
            	referrals[i].referralID,
            	referrals[i].gpID,
            	referrals[i].patientID,
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

            result = fscanf(fp, "%d, %d, %d, %100[^,], %d, %30[^\n]",
                &temp.referralID,
                &temp.gpID,
                &temp.patientID,
                temp.patientName,
                &temp.specialistID,
                temp.status);

            if (result==6 && count < MAX_USERS){
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

int getReferralID (Referral *referrals, int referralCount){
	int i,max=0;
	for (i=0;i<referralCount;i++){
		if (referrals[i].referralID > max){
			max = referrals[i].referralID;
		}
	}
	return max + 1;
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

void selectReferralID (User *currentUser, Referral *referrals, int *referralCount, User *users, int userCount, Patient *patients, int patientCount){
	int input, choice;
	int index, i, id;
	char cInput;
	
	printReferrals(currentUser,users,patients,referrals,*referralCount,userCount,"*");
	
	printf("\n\n%51sEnter Referral ID to select","");
	getValidInput(&input,1,0,100,0,0,0,0);
	
	index = findReferralByID(referrals,*referralCount,input);
	
	if (index == -1){
		printCentered("Referral not found.\n");
		waitForInput();
	}
	else if (strcmp(currentUser->role,"GP")==0 && index!=-1){
		do{
			clearScreen();
			viewReferralStatus(users,userCount,referrals,*referralCount,patients,patientCount, currentUser);
			
			printf("%28s","");
			for (i=0;i<WIDTH-100;i++){
				printf("%c",205);
			}
			printf("  Referral CRUD  ");
			for (i=0;i<WIDTH-100;i++){
				printf("%c",205);
			}
			printf("\n");
			
			printf("%55s[1] Delete Referral\n","");
			printf("%55s[0] Exit","");
    	    getValidInput(&choice,1,0,1,0,0,0,0);
    	    
    	    switch(choice){
    	    	case 1:
    	    		clearScreen();
    	    		
    	    		id = referrals[index].referralID;
    	    		
					printf("%38sAre you sure you want to delete referral #%02d? [Y/N]", "", referrals[index].referralID);
    	    		getValidInput(&cInput,3,0,0,'Y','N','y','n');
					switch (cInput){
    	    			case 'Y':
    	    			case 'y':
							deleteReferral(referrals,referralCount,index);
    	    				break;
    	    			case 'N':
    	    			case 'n':
    	    				break;
    	    			default:
    	    				printCentered("Invalid input.\n");
    	    				waitForInput();
					}
					saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
					
					printf("%47sSuccessfully deleted Referral #%02d","",id);
					waitForInput();
					
					choice = 0;
					break;
				case 0:
    	    		printf("Exiting...\n\n");
    	    		break;
    	    	default:
    	    		printCentered("Invalid input.");
    	    		waitForInput();
			}
		} while (choice!=0);
	}
	else if (strcmp(currentUser->role,"Specialist")==0 && index!=-1){
		do{
			clearScreen();
			viewReferralStatus(users,userCount,referrals,*referralCount,patients,patientCount,currentUser);
			
			printf("%28s","");
			for (i=0;i<WIDTH-100;i++){
				printf("%c",205);
			}
			printf("  Referral CRUD  ");
			for (i=0;i<WIDTH-100;i++){
				printf("%c",205);
			}
			printf("\n");
			
			printf("%55s[1] Edit status of referral\n","");
    	    printf("%55s[2] Delete referral\n","");
    	    printf("%55s[0] Exit","");
    	    getValidInput(&choice,1,0,2,0,0,0,0);
    	    
    	    switch(choice){
    	    	case 1:
   		     		clearScreen();
					editReferral(&referrals[index]);
   		     		saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
    	    		break;
    	    	case 2:
    	    		clearScreen();
    	    		
    	    		id = referrals[index].referralID;
    	    		
					printf("%38sAre you sure you want to delete referral #%02d? [Y/N]", "", referrals[index].referralID);
    	    		getValidInput(&cInput,3,0,0,'Y','N','y','n');
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
					saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
					
					printf("%47sSuccessfully Deleted Referral #%02d","",id);
					waitForInput();
					
					choice = 0;
    	    		break;
    	    	case 0:
    	    		printf("Exiting...\n\n");
    	    		break;
    	    	default:
    	    		printCentered("Invalid input.");
    	    		waitForInput();
			}
		} while(choice!=0);
	}
}
