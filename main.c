#include "Letran_Navarrosa_Machine-Project.h"

int main () {
	User users[MAX_USERS];
	User *currentUser;
	Patient patients[MAX_USERS];
	int userCount=1;
	// int userCount = loadUserFromFile(users, "users.txt");
	int exit=0, i=0, choice=0, gp=0, specialist=0, patient=0;
	currentUser = loginUser(users,userCount);
	
	for (i=0;i<userCount;i++){
			// Link patient userID and user userID
			patients[i].userID = users[i].userID;
		}
		
		
	do{
		printf("Input choice: ");
		scanf("%d",&choice);
		i=0;
		switch (choice){
			case 1: // Registration
				if (registerUser(users,&userCount)){
					if (saveUserToFile(&users[userCount],"users.txt")){
						printf("User saved successfully");
					}
				}
				break;
			case 2: // Login and dashboard
				currentUser = loginUser(users,userCount);
				if (strcmp(currentUser->role,"GP")==0){
					gp=1;
					do{
						// Dashboard function? for each user
					} while(gp!=0); // 0 is exit, 1 is main menu, 2 is add patient, 
									//3 is list patient -> then they can diagnose after selecting from the list
				}
				else if (strcmp(currentUser->role,"Specialist")==0){
					specialist=1;
					do{
						// Dashboard function? for each user
					} while(specialist!=0); // 0 is exit, 1 is main menu, 2 is patient list can search and sort in patient list
					                        // and after selecting, print patient details
				}
				else if (strcmp(currentUser->role,"Patient")==0){
					patient=1;
					do{
						// Dashboard function? for each user
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
	
*/
	
/* // hashPassword function test
	
*/

/* // loginUser function test

*/

/* // saveUserToFile function test

*/

/* // loadUsersFromFile function test

*/
