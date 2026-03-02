#include "Letran_Navarrosa_Machine-Project.h"

int main () {
	User users[MAX_USERS];
	Patient patients[MAX_USERS];
	int userCount = loadUserFromFile(users, "users.txt");
	int exit=0, i=0;
	
	do{
		for (i=0;i<userCount;i++){
			// Link patient userID and user userID
		}
		i=0;
		if (){ // choice is to register
			if (registerUser(user,&userCount)){
				if (saveUserToFile(user[userCount],"users.txt")){
					printf("User saved successfully");
				}
			}
		}
		else if (){ // choice is to login
			loginUser(users,&userCount);
			}
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
