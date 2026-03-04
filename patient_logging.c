#include "Letran_Navarrosa_Machine-Project.h"

// Patient logging
// Input patient details
/*
For patient to set their details.
returns Patient* - for saving to the TXT file?
*/
Patient addPatient (){
	Patient newPatient;
	int complete=0,valid=0;
	float weight,height;
	char strInput[101];
	do{
		// input name
		printf("Enter your full name ([First name] [Middle initial] [Last name]):\n");
		do{
			scanf(" %100[^\n]",strInput);
			if (strlen(strInput)>100){
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
			else {
				valid=1;
			}
		} while (valid==0);
		
		// input gender
		printf("Enter your gender at birth (M/F): ");
		do{
			scanf("%c",&newPatient.gender);
			if (newPatient.gender!='M' || newPatient.gender!='F'){
				printf("Invalid gender. Enter valid gender: \n");
			}
			else {
				valid=1;
			}
		} while (valid==0);
		
		// input contact details
		printf("Enter phone number (+63 xxx-xxx-xxxx): ");
		do{
			scanf(" %17[^\n]",strInput);
			if (strlen(strInput)>16){
				printf("Invalid details. Enter details again:\n");
			}
			else {
				strcpy(newPatient.contact,strInput);
				valid=1;
			}
		} while(valid==0);
		valid=0;
		
		// input weight and height then calc bmi into patient struct
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
		
		calculateBMI(&newPatient,weight,height);
		
		// input bp details, if none input 0 = will be for diagnosis
		printf("Enter Blood Pressure, in [Systolic/Diastolic] mmHg (i.e 120/80 mmHg). If unknown input 0.\n");
		do{
			scanf(" %16[^\n]",strInput);
			if (strlen(strInput)>15){
				printf("Invalid input. Enter again:\n");
			}
			else if(strInput[0]=='0'){
				printf("A General Practitioner will diagnose this later on.\n");
				valid=1;
			}
			else {
				strcpy(newPatient.bp,strInput);
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
				valid=1;
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
	// Ask for gender, previous blood test results, 1 or 0
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
/*
Estimates 10-year risk of heart attack in patients 30-79 years with no history of CHD or diabetes.

@param totalChol - an int representing total cholesterol of patient
@param hdlChol - an int representing HDL cholesterol of patient
@param bpTreat - should be Y or N, representing whether patient has been treated for BP problems
@param smoking - should be Y or N, representing whether patient is regularly smoking
*/
void calculateCardioRisk (Patient *patient, const int totalChol, const int hdlChol, const char bpTreat, const char smoking, const char diabetes){
	double cardioRisk=0.0, L=0.0;
	int sysBP=0, i;
	double ageLog = log(patient->age);
	double totalCholLog = log(totalChol);
	double hdlCholLog = log(hdlChol);
	double sysBPLog;
	
	// beta coefficients gathered from https://www.mdcalc.com/calc/38/framingham-risk-score-hard-coronary-heart-disease#evidence
	// Men
	double betaAgeMen = 52.00961;
	double betaTotalCholMen = 20.014077;
	double betaHDLCholMen = -0.905964;
	double betaSysBPMen = 1.305784;
	double betaBPTMen = 0.241549;
	double betaSmokeMen = 12.096316;
	double betaAgeCholMen = -4.605038;
	double betaAgeSmokeMen = -2.84367;
	double betaAgeAgeMen = -2.93323;
	// Women
	double betaAgeWomen = 31.764001;
	double betaTotalCholWomen = 22.465206;
	double betaHDLCholWomen = -1.187731;
	double betaSysBPWomen = 2.552905;
	double betaBPTWomen = 0.420251;
	double betaSmokeWomen = 13.07543;
	double betaAgeCholWomen = -5.060998;
	double betaAgeSmokeWomen = -2.996945;
	
	// parse sysBP and log
	for (i=0;i<15 && patient->bp[i]!='/';i++){
		if (patient->bp[i]>='0' && patient->bp[i]<='9'){
			sysBP = sysBP * 10 + (patient->bp[i] - '0');
		}
	}
	sysBPLog = log(sysBP);
	
	// convert bpTreat and smoking
	int bpT=0;
	int smoke;
	if (bpTreat=='Y'){
		bpT=1;
	}
	if (smoking=='Y'){
		smoke=1;
	}
	
	// risk based on gender
	if (patient->gender == 'M'){
		if (patient->age > 70){
			L = betaAgeMen * ageLog + betaTotalCholMen * totalCholLog + betaHDLCholMen * hdlCholLog + betaSysBPMen * sysBPLog + 
				betaBPTMen * bpT + betaSmokeMen * smoke + betaAgeCholMen * ageLog * totalCholLog + betaAgeSmokeMen * log(70) * smoke +
				betaAgeAgeMen * ageLog * ageLog - 172.300168;
		}
		else {
			L = betaAgeMen * ageLog + betaTotalCholMen * totalCholLog + betaHDLCholMen * hdlCholLog + betaSysBPMen * sysBPLog + 
				betaBPTMen * bpT + betaSmokeMen * smoke + betaAgeCholMen * ageLog * totalCholLog + betaAgeSmokeMen * ageLog * smoke +
				betaAgeAgeMen * ageLog * ageLog - 172.300168;
		}
		cardioRisk = 1 - pow(0.9402,exp(L));
	}
	else if (patient->gender == 'F'){
		if (patient->age > 78){
			L = betaAgeWomen * ageLog + betaTotalCholWomen * totalCholLog + betaHDLCholWomen * hdlCholLog + betaSysBPWomen * sysBPLog + 
				betaBPTWomen * bpT + betaSmokeWomen * smoke + betaAgeCholWomen * ageLog * totalCholLog + betaAgeSmokeWomen * log(78) * smoke 
				- 146.5933061;
		}
		else {
			L = betaAgeWomen * ageLog + betaTotalCholWomen * totalCholLog + betaHDLCholWomen * hdlCholLog + betaSysBPWomen * sysBPLog + 
				betaBPTWomen * bpT + betaSmokeWomen * smoke + betaAgeCholWomen * ageLog * totalCholLog + betaAgeSmokeWomen * ageLog * smoke 
				- 146.5933061;
		}
		cardioRisk = 1 - pow(0.98767,exp(L));
	}
	
	patient->cardioRisk=cardioRisk;
}

// Calculates risk of getting ASCVD-subset of diseases under the CVDs,, uses ACC ASCVD Risk Estimation 
/*
void calculateASCVDRisk (Patient *patient){
	double ascvdRisk;
	
	
	
	
	
	
	
	
	
	
	patient->ascvdRisk=ascvdRisk;
} */

// Print Recommendations (Weight, based on risk level) (Print reco should be in diagnose patient directly)

// Save patient to file
int savePatientToFile (const Patient *patient, const char *filename){
	FILE *fp;
	int flag = 0;
	if ((fp=fopen(filename, "a"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		fprintf(fp, "%s, %d, %s, %f, %s, %f, %lf , %lf", patient->name, patient->age, patient->contact, patient->bmi, patient->bp, patient->bloodSugar, patient->cardioRisk, patient->ascvdRisk);
		flag=1;
	}
	
	fclose(fp);
	return flag;
}

// Load patient to file
int loadPatientsFromFile (Patient *patients, const char *filename){
	FILE *fp;
	int count = 0, flag=1;
	
	if ((fp=fopen(filename, "r"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		while (flag){
			char name[101];
			int age;
			char contact[101];
			float bmi;
			char bp[21];
			float bloodSugar;
			double cardioRisk;
			double ascvdRisk;
			int result = fscanf(fp, "%s, %d, %s, %f, %s, %f, %lf , %lf", name, &age, contact, &bmi, bp, &bloodSugar, &cardioRisk, &ascvdRisk);
			// since fscanf outputs the amount of input items, we can use it to check if scanf was successful
			if (result==8){
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
				flag=0;
			}
		}
		fclose(fp);
	}
	return count;
}
/*
// Edit patient ID, name, age, contact
void editPatient (Patient *patient, ){
	
}

// Edit patient health metric (similar to diagnose, might not be needed)
void editPatientHealth (Patient *patient, ){
	
}

// Delete patient
void deletePatient (Patient *patient, ){
	
}

// Print patient list
void showPatients (Patient *patient, ){
	
}
*/
