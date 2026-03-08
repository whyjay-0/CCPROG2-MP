// Referral management
void createReferral (){ // GP only
	// select patient by entering name,,, would search by strcmp name and strcmp role
	// select specialist by entering name,,, would search by strcmp name and strcmp role
	// save referral
}

void showReferrals(User *currentUser, ){
	if (currentUser.role == ) // role specific if GP can see their list of referrals made
							  // otherwise Specialist can see those assigned to them
							  // patients will see who they are referred to
	
}

void findUserByName(){ //this function would search for the user that matches the search you made by name
	// would find the specific user that matches the role searching for,,, 
	// will be used for createReferral so its easier to find IDs for the referral struct
}

// we need search and sort functions also, but the sort should only be visual, it will not change any of their data
// So it could be sort by userID, by name, or by role
// we could apply it sa specialist? maybe they could see all users? or better if may admin acc
// we could also apply to list of patients within patient_management.c
// and referral list,,, this is where referral date might be good
