#include "Letran_Navarrosa_Machine-Project.h"

// Patient logging
// Input patient details
/*
For GP to add new patients.
returns Patient* - for saving to the TXT file?
*/
Patient* addPatient (){
	Patient *newPatient = NULL;
	int complete=0;
	float weight,height;
	char strInput[101];
	do{
		// input name
		
		
		// input age
		printf("Input age: ");
		do{
			scanf("%d",&newPatient.age);
			if (newPatient.age<=0){
				printf("Invalid age. Enter valid age: \n");
			}
		} while (newPatient.age<=0)
		
		// input contact details
		
		
		// input weight and height then place into struct bmi
		printf("Input weight (in kg): ");
		scanf("%f", &weight);
		printf("Input height (in meters): ");
		scanf("%f", &height);
		calculateBMI(newPatient,weight,height);
		
		// input bp details, if none input 0 = will be for diagnosis
		if (strInput[0]=='0'){
			
		}
		
		// input blood sugar, if unknown input 0 = will be for diagnosis
		if (strInput[0]=='0'){
			
		}
		
		// Ask for previous blood test results, 1 or 0
		// calculate for cardio risk with inputs from blood tests (cholesterol, HDL, LDL, triglycerides)
	} while (complete==0);
	
	return newPatient;
}

// Diagnose a patient
void diagnosePatient (Patient *patient){
	
}

// Calculate BMI
void calculateBMI (Patient *patient, const float weight, const float height){
	float bmi = weight / (height * height);
	patient->bmi = bmi;
}

// Calculate Risk
void calculateRisk (Patient *patient){
	
}

// Print Recommendations (Weight, based on risk level) (Print reco should be in diagnose patient directly)

// Save patient to file
int savePatientToFile (const Patient *patient, const char *filename){
	FILE *fp;
	int flag = 0;
	if ((fp=fopen(filename, "a"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		fprintf(fp, "", patient->;
		flag=1;
	}
	
	fclose(fp);
	return flag;
}

// Load patient to file
/*
int loadPatientsFromFile (Patient *patients, const char *filename){
	FILE *fp;
	int count = 0;
	
	if ((fp=fopen(filename, "r"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		while (count < MAX_USERS){
			// place the types inside the struct here
			
			int result = fscanf(fp, "%d, %s, %lu, %s", ); //what should be scanned and where the scan will be saved here
			// since fscanf outputs the amount of input items, we can use it to check if scanf was successful
			if (result==4){
				users[count].userID = ID; // patients[i].patientID = patientID etc
				if (strlen(username)>sizeof(users[count].username)){ // validity check for each detail
					printf("Invalid username");
				}
				else {
					strcpy(users[count].username, username); //strcpy
				}
				users[count].passwordHash = hash;
				if (strlen(role)>sizeof(users[count].role)){
					printf("Invalid role");
				}
				else {
					strcpy(users[count].role,role);
				}
				count++;
			}
			else {
				count = MAX_USERS;
			}
		}
		fclose(fp);
	}
	return count;
}*/

// Edit patient ID, name, age, contact
void editPatient (Patient *patient, ){
	
}

// Edit patient health metric
void editPatientHealth (Patient *patient, ){
	
}

// Delete patient
void deletePatient (Patient *patient, ){
	
}
