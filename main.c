#include "Letran_Navarrosa_Machine-Project.h"

int main () {
	User users[MAX_USERS];
	User *currentUser;
	Patient patients[MAX_USERS];
	Patient currentPatient;
	int userCount = loadUsersFromFile(users, "users.txt");
	int patientCount = loadPatientsFromFile(patients, "patients.txt");
	int exit=0, choice=0, gp=0, specialist=0, patient=0, crudEnd=0, found=0;
	int newUser;
	int i;
	char cInput;
	char nameInput[101];
	printf("User count: %d\n", userCount);
	do{
		printf("Input choice: "); // design nalang later
		scanf("%d",&choice);
		
		switch (choice){
			case 1: // Registration
				newUser = registerUser(users, &userCount);
				if (newUser != -1){
					if (saveUserToFile(&users[newUser],"users.txt")){
						printf("User saved successfully!\n");
					}
				}
				break;
			case 2: // Login and dashboard
				currentUser = loginUser(users,userCount);
				// save all users to save any changes to password if forgotten
				for(i=0;i<userCount;i++){
					saveUserToFile(&users[i],"users.txt");
				}
				if (strcmp(currentUser->role,"GP")==0){
					gp=1;//gp will determine where they are in the menu
					do{
						printf("Enter gp value: ");// Showing status etc
						scanf("%d",&gp);
						switch (gp){
							case 1: // main menu
								// ewan basta details ng user
								printf("Enter 0 to exit.\n");
								break;
							case 2: // add patient
								if (patientCount<MAX_USERS){
									patients[patientCount] = addPatient();
									savePatientToFile(patients[patientCount], "patients.txt");
									patientCount++;
								}
								else
									printf("Max amount of patients reached.\n");
								break;
							case 3: // list all patients edit, delete, diagnose, or refer
								do{
									showPatients(patients,patientCount);
									printf("Enter input, edit, delete, diagnose, or refer patient: (1,2 or 0 to exit)\n");
									scanf("%d",&crudEnd); // if 0 is entered it exits this mode
									switch (crudEnd){
										case 1:
											editPatient(patients,patientCount);
											break;
										case 2:
											deletePatient(patients,patientCount);
											break;
										case 3:
											currentPatient=diagnosePatient(patients,patientCount);
											break;
										case 4:
											createReferral
											break;
										default:
											printf("Invalid input. Try again: ");
									}
								} while (crudEnd!=0);
								break;
							default:
								printf("Invalid input. Try again: ");
								break;
						}
					} while(gp!=0); // 0 is exit, 1 is main menu, 2 is add patient, 
									//3 is list patient -> then they can diagnose after selecting from the list
				}
				else if (strcmp(currentUser->role,"Specialist")==0){
					specialist=1;//specialist will determine where they are in the menu
					do{
						// Dashboard function for each user
						// they will be able to see list of referrals and patients connected to referral
						// after selecting a patient, they will be able to see their diagnosis report
					} while(specialist!=0); // 0 is exit, 1 is main menu, 2 is patient list can search and sort in patient list
					                        // and after selecting, print patient details
				}
				else if (strcmp(currentUser->role,"Patient")==0){
					patient=1; //patient will determine where they are in the menu
					do{
						// Dashboard function for each user
						// Patients should be able to add themself as patient to edit their details.
						// Within their main menu (dashboard), they will see their details (username, name, age, userID, bmi, role)
						// Maybe we could add other stuff also like current time and date and add colors just for design
					} while(patient!=0); // 0 is exit, 1 is main menu 
										 // (will be able to see time date and status), 2 is request GP??
										 // 3 to edit their details
				}
				else{
					printf("Invalid role\n");
				}
				break;
			case 3: // Exit
				exit=1;
				break;
			default: 
				printf("Invalid choice.\n");
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
