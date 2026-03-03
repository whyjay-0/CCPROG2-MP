#include "Letran_Navarrosa_Machine-Project.h"

// Patient logging
// Input patient details
/*
For patient to set their details.
returns Patient* - for saving to the TXT file?
*/
Patient* addPatient (){
	Patient *newPatient = NULL;
	int complete=0,valid=0;
	float weight,height;
	char strInput[101];
	do{
		// input name
		printf("Enter your full name ([First name] [Middle initial] [Last name]):\n");
		do{
			scanf("%100[^\n]",strInput);
			if (strlen(strInput)>sizeof(newPatient.name)){
				printf("Invalid name. Enter name again:\n");
			}
			else{
				strcpy(newPatient.name,strInput);
				valid=1;
			}
		} while(valid==0);
		valid=0;
		
		// input age
		printf("Enter your age: ");
		do{
			scanf("%d",&newPatient.age);
			if (newPatient.age<=0){
				printf("Invalid age. Enter valid age: \n");
			}
		} while (newPatient.age<=0)
		
		// input contact details
		printf("Enter phone number (+63 xxx-xxx-xxxx): ");
		do{
			scanf("%100[^\n]",strInput);
			if (strlen(strInput)>sizeof(newPatient.contact)){
				printf("Invalid details. Enter details again:\n");
			}
			else {
				strcpy(newPatient.contact,strInput);
				valid=1;
			}
		} while(valid==0);
		valid=0;
		
		// input weight and height then place into struct bmi
		printf("Input weight (in kg): ");
		do{
			scanf("%f",&weight);
			if (weight<0){
				printf("Invalid measurement. Enter again.");
			}
			else {
				valid=1;
			}
		} while(valid==0);
		valid=0;
		
		printf("Input height (in m): ");
		do{
			scanf("%f",&height);
			if (height<0){
				printf("Invalid measurement. Enter again.");
			}
			else {
				valid=1;
			}
		} while(valid==0);
		
		calculateBMI(newPatient,weight,height);
		
		// input bp details, if none input 0 = will be for diagnosis
		printf("Enter Blood Pressure, in [Systolic/Diastolic] mmHg (i.e 120/80 mmHg). If unknown input 0.\n");
		do{
			scanf("%16[^\n]",strInput);
			if (strlen(strInput)>sizeof(newPatient.bp)){
				printf("Invalid input. Enter again:\n");
			}
			else if(strInput[0]=='0'){
				printf("A General Practitioner will diagnose this later on.\n");
			}
			else {
				strcpy(newPatient.contact,strInput);
				valid=1;
			}
		} while(valid==0);
		valid=0;
		
		// NEED VALIDITY CHECK AND RANGES FOR MEASUREMENTS
		// input blood sugar, if unknown input 0 = will be for diagnosis
		printf("Enter Blood Sugar, in mg/dL. If unknown input 0.\n");
		do{
			scanf("%f",&newPatient.bloodSugar);
			if (newPatient.bloodSugar<0){
				printf("Invalid measurement. Enter again.");
			}
			else if (newPatient.bloodSugar==0){
				printf("A General Practitioner will diagnose this later on.\n");
			}
			else {
				valid=1;
			}
		} while(valid==0);
		complete=1;
	} while (complete==0);
	
	return newPatient;
}
/*
For GP to diagnose existing patients.
edits cardiorisk ascdv risk

variables needed age, sex, race, BP, chol, and diabetes and smoking
*/
// Diagnose a patient, will give suggestions and risk level based on the details that 
// will be input here and from the addPatient function
void diagnosePatient (Patient *patient){
	// Ask for previous blood test results, 1 or 0
	// calculate for cardio risk with inputs from blood tests (cholesterol, HDL, LDL, triglycerides)
	// calculateCardioRisk
	// calculateASCVDRisk
	// give suggestions
}

// Calculate BMI
void calculateBMI (Patient *patient, const float weight, const float height){
	float bmi = weight / (height * height);
	patient->bmi = bmi;
}

// Calculate Risk using Framingham Risk Score to estimate probability of developing CVD within 10 years
void calculateCardioRisk (Patient *patient){
	
}

// Calculates risk of getting ASCVD-subset of diseases under the CVDs
void calculateASCVDRisk (Patient *patient){
	
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
		fprintf(fp, "%d, %s, %d, %s, %f, %s, %f, %lf , %lf", patient->userID, patient->name, patient->age, patient->contact, patient->bmi, patient->bp, patient->bloodSugar, patient->cardioRisk, patient->ascvdRisk);
		flag=1;
	}
	
	fclose(fp);
	return flag;
}

// Load patient to file
int loadPatientsFromFile (Patient *patients, const char *filename){
	FILE *fp;
	int count = 0;
	
	if ((fp=fopen(filename, "r"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		while (count < MAX_USERS){
			int userID;
			char name[101];
			int age;
			char contact[101];
			float bmi;
			char bp[21];
			float bloodSugar;
			double cardioRisk;
			double ascvdRisk;
			int result = fscanf(fp, "%d, %s, %d, %s, %f, %s, %f, %lf , %lf", &userID, name, &age, contact, &bmi, bp, &bloodSugar, &cardioRisk, &ascvdRisk);
			// since fscanf outputs the amount of input items, we can use it to check if scanf was successful
			if (result==9){
				patients[count].userID = userID; // patients[i].patientID = patientID etc
				if (strlen(name)>sizeof(patients[count].name)){ // validity check for each detail
					printf("Invalid name");
				}
				else {
					strcpy(patients[count].name, name); //strcpy
				}
				patients[count].age = age;
				if (strlen(contact)>sizeof(patients[count].contact)){
					printf("Invalid contact");
				}
				else {
					strcpy(patients[count].contact,contact);
				}
				patients[count].bmi = bmi;
				if (strlen(bp)>sizeof(patients[count].bp)){
					printf("Invalid bp");
				}
				else {
					strcpy(patients[count].bp,bp);
				}
				patients[count].bloodSugar = bloodSugar;
				patients[count].cardioRisk = cardioRisk;
				patients[count].ascvdRisk = ascvdRisk;
				count++;
			}
			else {
				count = MAX_USERS;
			}
		}
		fclose(fp);
	}
	return count;
}

// Edit patient ID, name, age, contact
void editPatient (Patient *patient, ){
	
}

// Edit patient health metric
void editPatientHealth (Patient *patient, ){
	
}

// Delete patient
void deletePatient (Patient *patient, ){
	
}
