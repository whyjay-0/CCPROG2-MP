#include "Letran_Navarrosa_Machine-Project.h"

int main () {
	User users[MAX_USERS] = {0};
	User *currentUser;
	Patient patients[MAX_USERS] = {0};
	Referral referrals[MAX_USERS] = {0};
	
	int userCount = loadUsersFromFile(users, "users.txt");
	int patientCount = loadPatientsFromFile(patients, "patients.txt");
	int referralCount = loadReferralsFromFile(referrals, "referrals.txt");
	
	int exit=0, choice=0, validInput;
	int newUser; // userID of new users
	double data[MAX_USERS][2]; // [][0] = BMI, [][1] = CRisk
	// 2D array here, for bmi and risk computation, averages.
	// printf("User count: %d\n", userCount);
	do{
		// design for main menu/login screen here
		printf("1. Register account\n");
		printf("2. Login\n");
		printf("3. Exit program\n");
		printf("Input choice: "); // design nalang later
		validInput = scanf(" %d",&choice); // to check if it received an int
		if (validInput==0){
			scanf("%*[^\n]"); // clear inputs until newline
			printf("Invalid input.\n\n");
		}
		else{
			switch (choice){
				case 1: // Registration
					newUser = registerUser(users, &userCount, patients, patientCount); // registerUser returns -1 if not valid registration
					if (newUser != -1){
						saveAllUsersToFile(users, userCount, "users.txt");
						printf("User saved successfully!\n");
					}
					break;
				case 2: // Login and dashboard
					currentUser = loginUser(users,userCount);
						
					if (currentUser == NULL){
					    printf("Login failed.\n");
					}
					else {
						if (strcmp(currentUser->role,"GP")==0){
							gpDashboard(currentUser,patients,&patientCount,users,userCount,referrals,&referralCount,data);
						}
						else if (strcmp(currentUser->role,"Specialist")==0){
							specialistDashboard(currentUser,users,userCount,referrals,referralCount,patients,patientCount,data);
							// Dashboard function for each user
							// they will be able to see list of referrals and patients connected to referral
							// after selecting a patient, they will be able to see their diagnosis report
							// 0 is exit, 1 is main menu, 2 is patient list can search and sort in patient list
							// and after selecting, print patient details
						}
						else if (strcmp(currentUser->role,"Patient")==0){
							patientDashboard(currentUser,users,userCount,patients,&patientCount,referrals,referralCount);
							// Dashboard function for each user
							// Patients should be able to add themself as patient to edit their details.
							// Within their main menu (dashboard), they will see their details (username, name, age, userID, bmi, role)
							// Maybe we could add other stuff also like current time and date and add colors just for design
							// 0 is exit, 1 is main menu 
							// (will be able to see time date and status), 2 is request GP??
							// 3 to edit their details
						}
						else{
							printf("Invalid role\n");
						}
					}
					break;
				case 3: // Exit
					printf("Exiting program...\n");
					exit=1;
					break;
				default: 
					printf("Invalid choice.\n");
			}
		}
	} while(exit==0);
	return 0;
}

/* // registerUser function test
	newUser = registerUser(users, &userCount);
	if (newUser != -1){
		if (saveUserToFile(&users[newUser],"users.txt")){
			printf("User saved successfully");
		}
	}
*/
	
/* // hashPassword function test
	
*/

/* // loginUser function test

*/

/* // saveUserToFile function test

*/

/* // loadUsersFromFile function test

*/
