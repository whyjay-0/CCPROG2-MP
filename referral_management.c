#include "Letran_Navarrosa_Machine-Project.h"

// Referral management
void createReferral (Referral *referrals, User *users, Patient *patients, User currentUser, int patientCount, int userCount, int *referralCount){ 
	// GP access only, currentpatient is patient being referred, user is GP
	// select patient by entering name,,, would search by strcmp name and strcmp role
	// select specialist by entering name,,, would search by strcmp name and strcmp role
	// save referral
	int i,found=0;
	char strInput[101];
	Referral newReferral;
	Patient currentPatient;
	User specialist;
	newReferral.ReferralID = 0;
	newReferral.GPID = 0;
	newReferral.SpecialistID = 0;
	strcpy(newReferral.status, "");
	
	printf("Enter the full name of the patient to refer:\n");
	scanf(" %100[^\n]", strInput);
	for(i=0;i<patientCount;i++){
		if (strcmp(patients[i].name,strInput)==0){
			currentPatient=patients[i];
			found=1;
			strcpy(newReferral.PatientName,currentPatient.name);
			i=patientCount; // end loop
		}
	}
	if (found==0){
		printf("Patient not found.\n");
	}
	else if (currentPatient.isDiagnosed=='Y' && found==1){
		printf("Enter the full name of the specialist:\n");
		scanf(" %100[^\n]", strInput);
		found=0;
		for (i=0;i<userCount;i++){
			// search for user
			if (strcmp(users[i].name,strInput)==0 && strcmp(users[i].role,"Specialist")==0){
				specialist=users[i];
				found=1;
				i=userCount;
			}
			if (found==0)
				printf("Specialist not found.\n");
		}
		newReferral.SpecialistID = specialist.userID; // found specialist
		newReferral.GPID = currentUser.userID; // assumed current user is GP
		
		strcpy(newReferral.status,"Pending"); // req will be sent to specialist, where they will need to accept or complete or reject
		
		// setting new ID for referral
		newReferral.ReferralID = *referralCount + 1; // set the referralID of new referral to old referralCount + 1
		referrals[*referralCount] = newReferral; //set the array of struct at index *referrakCount to the newReferral made
		// index = *userCount; // can be used if we want to return ID of the new referral
		(*referralCount)++; // increase amount of referrals
		
		saveAllReferralsToFile(referrals, *referralCount, "referrals.txt");
	}
	else {
		printf("Patient is not yet diagnosed.\n");
	}
}

void showReferrals (User *currentUser, User *users, Referral *referrals, int referralCount){
    int i, j,found=0;
    User temp, temp2;
	
    printf("\n===== REFERRALS =====\n");
    for (i=0;i<referralCount;i++){
        if (strcmp(currentUser->role,"GP")==0){
            if (referrals[i].GPID == currentUser->userID){
                printf("%d | %s | Specialist ID: %d | %s\n",
                    referrals[i].ReferralID,
                    referrals[i].PatientName,
                    referrals[i].SpecialistID,
                    referrals[i].status);
                found=1;
            }
        }
        else if (strcmp(currentUser->role,"Specialist")==0){
            if (referrals[i].SpecialistID == currentUser->userID){
                printf("%d | %s | Status: %s\n",
                    referrals[i].ReferralID,
                    referrals[i].PatientName,
                    referrals[i].status);
                found=1;
            }
        }
        else if (strcmp(currentUser->role,"Patient")==0){
        	if (strcmp(referrals[i].PatientName,currentUser->name) == 0){
        		for (j=0;j<MAX_USERS;j++){ // search name by userID GP
        			if (referrals[i].GPID == users[j].userID){
        				temp = users[j];
					}
					if (referrals[i].SpecialistID == users[j].userID){
						temp2 = users[j];
					}
				}
        		printf("%d | GP: %s | Specialist: %s", referrals[i].ReferralID, temp.name, temp2.name);
			}
		}
    }

    if (!found){
        printf("No referrals found.\n");
    }
}

void editReferral (Referral *referrals, int referralCount){
    int id, i, found=0, choice;
	
    printf("Enter Referral ID to update: ");
    scanf("%d",&id);
	
    for (i=0;i<referralCount;i++){
        if (referrals[i].ReferralID == id){
            printf("1 - Accept\n2 - Complete\n3 - Reject\nChoice: ");
            scanf("%d",&choice);
			
            switch(choice){
                case 1:
                    strcpy(referrals[i].status,"Accepted");
                    break;
                case 2:
                    strcpy(referrals[i].status,"Completed");
                    break;
                case 3:
                    strcpy(referrals[i].status,"Rejected");
                    break;
                default:
                    printf("Invalid choice.\n");
            }
			
            found=1;
            i=referralCount; // end loop
        }
    }
	
    if (!found){
        printf("Referral not found.\n");
    }
}

void deleteReferral (Referral *referrals, int *count){
    int id, i, j, found=0;
	
    printf("Enter Referral ID to delete: ");
    scanf("%d",&id);
	
    for (i=0;i<*count;i++){
        if (referrals[i].ReferralID == id){
            for (j=i;j<*count-1;j++){
                referrals[j] = referrals[j+1];
            }
			
            (*count)--;
            found=1;
            printf("Referral deleted.\n");
			
            i=*count;
        }
    }
	
    if (!found){
        printf("Referral not found.\n");
    }
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
            	referrals[i].ReferralID,
            	referrals[i].GPID,
            	referrals[i].PatientName,
            	referrals[i].SpecialistID,
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
                &temp.ReferralID,
                &temp.GPID,
                temp.PatientName,
                &temp.SpecialistID,
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
void sortPatientsByName(Patient *patients, int count) { //uses selection sort
    Patient tempArr[100];
    int i, j, minIndex;
    Patient temp;

    // copy original array to tempArr,, temporary sorting?
    for (i = 0; i < count; i++) {
        tempArr[i] = patients[i];
    }

    // selection sort
    for (i = 0; i < count - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < count; j++) {
            if (strcmp(tempArr[j].name, tempArr[minIndex].name) < 0) {
                minIndex = j;
            }
        }

        // Swap tempArr[i] and tempArr[minIndex]
        if (minIndex != i) {
            temp = tempArr[i];
            tempArr[i] = tempArr[minIndex];
            tempArr[minIndex] = temp;
        }
    }

    // print sorted patients
    printf("---- SORTED PATIENTS BY NAME (A-Z) ----\n");
    for (i = 0; i < count; i++) {
        printf("%s | Age: %d | BMI: %.2f\n", tempArr[i].name, tempArr[i].age, tempArr[i].bmi);
    }
}

/*
2D arrays will be implemented through computation averages of BMI and CardioRisk for Specialists
*/
