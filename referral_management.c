// Referral management
void createReferral (Referral *referrals, const User users, const Patient patients, const User currentUser, const int patientCount, const int userCount, int *referralCount){ 
	// GP access only, currentpatient is patient being referred, user is GP
	// select patient by entering name,,, would search by strcmp name and strcmp role
	// select specialist by entering name,,, would search by strcmp name and strcmp role
	// save referral
	int i,found=0;
	char strInput[101];
	Referral newReferral;
	Patient currentPatient;
	User specialist;
	
	printf("Enter the full name of the patient to refer to:\n");
	scanf(" %100[^\n]", strInput);
	for(i=0;i<patientCount;i++){
		if (strcmp(patients[i].name,strInput)==0){
			currentPatient=patients[i];
			found=1;
			strcpy(newReferral.PatientName,currentPatient.name);
		}
	}
	if (found==0){
		printf("Patient not found.\n");
	}
	else if (currentPatient.isDiagnosed=='Y' && found==1){
		printf("Enter the full name of the specialist:\n");
		scanf(" %100[^\n]", strInput);
		for (i=0;i<userCount;i++){
			// search for user
			if (strcmp(users[i].name,strInput)==0){
				specialist=users[i];
			}
		}
		newReferral.SpecialistID = specialist.userID; // found specialist
		newReferral.GPID = currentUser.userID; // assumed current user is GP
		
		strcpy(newReferral.status,"Pending"); // req will be sent to specialist, where they will need to accept or complete or reject
		
		// setting new ID for referral
		newReferral.ReferralID = *referralCount + 1; // set the referralID of new referral to old referralCount + 1
		users[*userCount] = newUser; //set the array of struct at index *userCount to the newUser made
		// index = *userCount; // can be used if we want to return ID of the new referral
		(*referralCount)++; // increase amount of referrals
		
		saveReferralToFile(newReferral,"referrals.txt");
	}
	else {
		printf("Patient is not yet diagnosed.\n");
	}
}

void showReferrals (User *currentUser, Referral *referrals, Patient *patients, User *users){
	if (strcmp(currentUser.role,"") == 0) // role specific if GP can see their list of referrals made
							  // otherwise Specialist can see those assigned to them
							  // patients will see who they are referred to
	else if (strcmp(currentUser.role,"") == 0)
}

void editReferral(Referral *referral, User *currentUser){
	
}

void deleteReferral(Referral *referral){
	
}

void selectReferral(int referralID){
	
}

User findUserByName (){ //this function would search for the user that matches the search you made by name
	// would find the specific user that matches the role searching for,,, 
	// will be used for createReferral so its easier to find IDs for the referral struct
} 

User findUserByID (int userID){ // returns User being searched for
	
} // can be used when printing records of referrals, binary search maybe
// we need search and sort functions also, but the sort should only be visual, it will not change any of their data
// So it could be sort by userID, by name, or by role
// we could apply it sa specialist? maybe they could see all users? or better if may admin acc
// we could also apply to list of patients within patient_management.c
// and referral list,,, this is where referral date might be good

int saveReferralToFile (const Referral referral, const char *filename){
	
}

int loadReferralsFromFile (const Referral referrals, const char *filename){
	
}
