#include "Letran_Navarrosa_Machine-Project.h"

// Patient logging
// Input patient details
/*
For patient to set their details.
returns Patient* - for saving to the TXT file
*/
Patient addPatient (User *currentUser, Patient *patients, int patientCount, User *users, int userCount){
	Patient newPatient;
	int index;
	float weight,height;
	char strInput[101];
	char choice;
	
	initPatient(&newPatient);
	
	if (strcmp(currentUser->role, "Patient")==0){
		// auto input of name, since patient can only add themself
		strcpy(newPatient.name,currentUser->name);
		newPatient.userID = currentUser->userID;
		newPatient.patientID = getPatientID(patients, patientCount);
		printf("Patient name: %s\n", newPatient.name);
	}
	else{ // user is not patient
		printf("Enter patient's full name:\n");
		getValidInput(strInput,4,0,0,0,0,0,0);
		
		index = findUserByName(users,userCount,newPatient.name);
		
		if (index!=-1){ // found user
			// prompt
			printf("A user with a similar name is a patient. Would you like to link their accounts? (Y/N): ");
			
			getValidInput(&choice,3,0,0,'Y','N','y','n');
			switch (choice){
				case 'Y':
				case 'y':
					newPatient.patientID = getPatientID(patients, patientCount);
					newPatient.userID = users[index].userID;
					break;
				case 'N':
				case 'n':
					newPatient.patientID = getPatientID(patients, patientCount);
					newPatient.userID = getUserID(users, patients);
					break;
				default:
					printf("Invalid choice.\n");
			}
		}
		else { // no old user
			newPatient.patientID = getPatientID(patients, patientCount); // set the userID of new user to old usercount + 1
			newPatient.userID = getUserID(users, patients); // getting a new userID even if patient does not have account yet
		}
	}
	
	// input age
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Enter your age: ");
	else
		printf("Enter patient's age: ");
	getValidInput(&newPatient.age,1,0,200,0,0,0,0);
	
	// input gender
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Enter your gender at birth (M/F): ");
	else
		printf("Enter patient's gender at birth (M/F): ");
	getValidInput(&newPatient.gender,3,0,0,'M','F','m','f');
	
	updateGender(&newPatient);
		
	// input contact details
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Enter your phone number (+63 xxx-xxx-xxxx): ");
	else
		printf("Enter patient's phone number (+63 xxx-xxx-xxxx): ");
	getValidInput(newPatient.contact,5,0,0,0,0,0,0);
		
	// input weight and height then calc bmi into patient struct
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Input your weight (in kg): ");
	else
		printf("Input patient's weight (in kg): ");
	getValidInput(&weight,2,0,500,0,0,0,0);
	
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Input your height (in m): ");
	else
		printf("Input patient's height (in m): ");
	getValidInput(&height,2,0,500,0,0,0,0);
	
	calculateBMI(&newPatient,weight,height);
		
	// input bp details, if none input 0 = will be for diagnosis
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Enter your blood pressure, in [Systolic/Diastolic] mmHg (i.e 120/80 mmHg). If unknown, input 0.\n");
	else
		printf("Enter patient's blood pressure, in [Systolic/Diastolic] mmHg (i.e 120/80 mmHg). If unknown, input 0.\n");
	getValidInput(newPatient.bp,6,0,0,0,0,0,0);
	
	// input blood sugar, if unknown input 0 = will be for diagnosis
	if (strcmp(currentUser->role, "Patient")==0)
		printf("Enter your blood sugar, in mg/dL. If unknown, input 0.\n");
	else
		printf("Enter patient's blood sugar, in mg/dL. If unknown, input 0.\n");
	getValidInput(&newPatient.bloodSugar,2,0,500,0,0,0,0);
	if (newPatient.bloodSugar==0)
		printf("A General Practitioner will diagnose this later on.\n");
	
	return newPatient;
}

void initPatient (Patient *patient){
	patient->userID=-1;
	patient->currentCVD='N';
	patient->totalChol=0.0;
	patient->hdlChol=0.0;
	patient->eGFR=0;
	patient->htMed='N';
	patient->statins='N';
	patient->smoking='N';
	patient->diabetes='N';
	patient->creatinine=0.0;
	patient->cvdFamily='N';
	patient->diet='N';
	patient->exercise='N';
	patient->alcohol='N';
	patient->cardioRisk=-1.0;
	patient->isDiagnosed='N';
}
/*
For GP to diagnose existing patients.
*/
// Diagnose a patient, will give suggestions and risk level based on the details that 
// will be input here and from the addPatient function
void diagnosePatient (Patient *patient){
	Patient *currentPatient;
	currentPatient = patient;
	// int validInput;
	
	printf("----- Diagnosing Patient: %s -----\n", currentPatient->name);
	
	// Ask user for needed info
	// Cholesterol values
	printf("Enter Total Cholesterol (mg/dL): ");
	getValidInput(&currentPatient->totalChol,2,0,1000,0,0,0,0);
	printf("Enter HDL Cholesterol (mg/dL): ");
	getValidInput(&currentPatient->hdlChol,2,0,1000,0,0,0,0);
	
	// eGFR value, estimated Glomerular Filtration Rate
	printf("Enter eGFR (mL/min/1.73m^2): ");
	getValidInput(&currentPatient->eGFR,1,0,1000,0,0,0,0);
	
	// BP Treatment and Values
	if (strcmp(currentPatient->bp,"0")==0){
		printf("Enter blood pressure (SYS/DIA mmHg): ");
		getValidInput(currentPatient->bp,6,0,0,0,0,0,0);
	}
	
	printf("Is patient using anti-hypertensive medication? (Y/N): ");
	getValidInput(&currentPatient->htMed,3,0,0,'Y','N','y','n');
	
	// CVD
	printf("Does patient have any known cardiovascular disease? (Y/N): ");
	getValidInput(&currentPatient->currentCVD,3,0,0,'Y','N','y','n');
	
	// Blood sugar
	if (currentPatient->bloodSugar==0){
		printf("Enter blood sugar (mg/dL): ");
		getValidInput(&currentPatient->bloodSugar,2,0,1000,0,0,0,0);
	}
	
	// LDL treatment
	printf("Is patient using statins? (Y/N): ");
	getValidInput(&currentPatient->statins,3,0,0,'Y','N','y','n');
	
	// Smoking
	printf("Is patient a regular smoker? (Y/N): ");
	getValidInput(&currentPatient->smoking,3,0,0,'Y','N','y','n');
	
	// Diabetes
	printf("Does patient have diabetes? (Y/N): ");
	getValidInput(&currentPatient->diabetes,3,0,0,'Y','N','y','n');
	
	// Creatinine
	printf("Enter Serum Creatinine of patient (mg/dL): ");
	getValidInput(&currentPatient->creatinine,2,0,1000,0,0,0,0);
	
	// Family History CVD
	printf("Does patient have family history of CVD? (Y/N): ");
	getValidInput(&currentPatient->cvdFamily,3,0,0,'Y','N','y','n');
	
	// Diet/Exercise
	printf("Does patient exercise regularly? (Y/N): ");
	getValidInput(&currentPatient->exercise,3,0,0,'Y','N','y','n');
	printf("Does patient have a high-fat/sugar diet? (Y/N): ");
	getValidInput(&currentPatient->diet,3,0,0,'Y','N','y','n');
	
	// Alcohol
	printf("Does patient consume alcohol regularly? (Y/N): ");
	getValidInput(&currentPatient->alcohol,3,0,0,'Y','N','y','n');
	
	// Calculate Cardio Risk,, Can only be done on adults ages 30-79 without past CVD (This is assumed to be true).
	if (currentPatient->age>=30 && currentPatient->age<=79 && currentPatient->currentCVD=='N')
		calculateCardioRisk(currentPatient);
	else
		currentPatient->cardioRisk = -1.0; // This should mean invalid since outside of age range
	
	printf("---------- DIAGNOSIS REPORT ----------\n\n");
	// Details
	printf("Patient: %s\n", currentPatient->name);
	printf("Age: %d\nGender: %c\n", currentPatient->age, currentPatient->gender);
	printf("BMI: %f, %s\n", currentPatient->bmi, currentPatient->bmiCat);
	printf("Blood Pressure: %s\n", currentPatient->bp);
	printf("Blood Sugar: %d mg/dL\n", currentPatient->bloodSugar);
	// Will be shown if calculateCardioRisk was done otherwise other details will be shown.
	// Temporary interpretations Changes might be made once AHA provides proper source code.
	if (currentPatient->age>=30 && currentPatient->age<=79 && currentPatient->currentCVD=='N'){
		// Cardio Risk
		printf("10-Year Cardiovasular Risk: %.2lf%%\n", currentPatient->cardioRisk * 100);
		// Risk Level Classification
		if(currentPatient->cardioRisk < 0.05)
			printf("   Risk Level: Low Risk\n\n");
		else if(currentPatient->cardioRisk >= 0.05 && currentPatient->cardioRisk <= 0.074)
		printf("   Risk Level: Borderline Risk\n\n");
		else if(currentPatient->cardioRisk >= 0.075 && currentPatient->cardioRisk <= 0.199)
			printf("   Risk Level: Intermediate Risk\n\n");
		else
			printf("   Risk Level: Very High Risk\n\n");
	}
	// Other data and suggestions
	printf("Data and Suggestions:\n");
	if (currentPatient->bmi>30)
		printf("   - Suggestion: Consider weight loss through diet and exercise.\n");
	if (currentPatient->creatinine>1.2)
		printf("   - Kidney Function is decreased. Consider further renal evaluation.\n");
	if (currentPatient->cvdFamily=='Y')
		printf("   - Increased risk of cardiovascular disease due to family history.\n");
	if (currentPatient->diet=='Y')
		printf("   - Modify diet to lower fat and sugar intake\n");
	if (currentPatient->exercise=='Y')
		printf("   - Continue regular physical activity\n");
	if (currentPatient->alcohol=='Y')
		printf("   - Limit alcohol intake to reduce cardiovascular risk.\n");
	currentPatient->isDiagnosed='Y';
}

// printing diagnosis report
void showDiagnosisReport (Patient *currentPatient){ // For specialist only,,, need selectPatient function
	char choice;
	printf("---------- PATIENT DETAILS ----------\n\n");
	// Details
	printf("Patient: %s\n", currentPatient->name);
	printf("Age: %d\nGender: %c\n", currentPatient->age, currentPatient->gender);
	printf("BMI: %f, %s\n", currentPatient->bmi, currentPatient->bmiCat);
	printf("Blood Pressure: %s\n", currentPatient->bp);
	printf("Blood Sugar: %f mg/dL\n", currentPatient->bloodSugar);
	// Will be shown if calculateCardioRisk was done otherwise other details will be shown.
	// Temporary interpretations Changes might be made once AHA provides proper source code.
	
	if (currentPatient->isDiagnosed=='Y'){
		printf("Show past diagnosis? (Y/N)");
		getValidInput(&choice,3,0,0,'Y','N','y','n');
		switch (choice){
			case 'Y':
			case 'y':
				if (currentPatient->age>=30 && currentPatient->age<=79 && currentPatient->currentCVD=='N'){
					// Cardio Risk
					printf("10-Year Cardiovasular Risk: %.2lf%%\n", currentPatient->cardioRisk * 100);
					// Risk Level Classification
					if(currentPatient->cardioRisk < 0.05)
						printf("   Risk Level: Low Risk\n\n");
					else if(currentPatient->cardioRisk >= 0.05 && currentPatient->cardioRisk <= 0.074)
						printf("   Risk Level: Borderline Risk\n\n");
					else if(currentPatient->cardioRisk >= 0.075 && currentPatient->cardioRisk <= 0.199)
						printf("   Risk Level: Intermediate Risk\n\n");
					else
						printf("   Risk Level: Very High Risk\n\n");
				}
				// Other data and suggestions
				printf("Data and Suggestions:\n");
				if (currentPatient->bmi>30)
					printf("   - Suggestion: Consider weight loss through diet and exercise.\n");
				if (currentPatient->creatinine>1.2)
					printf("   - Kidney Function is decreased. Consider further renal evaluation.\n");
				if (currentPatient->cvdFamily=='Y')
					printf("   - Increased risk of cardiovascular disease due to family history.\n");
				if (currentPatient->diet=='Y')
					printf("   - Modify diet to lower fat and sugar intake\n");
				if (currentPatient->exercise=='Y')
					printf("   - Continue regular physical activity\n");
				if (currentPatient->alcohol=='Y')
					printf("   - Limit alcohol intake to reduce cardiovascular risk.\n");
				break;
			case 'N':
			case 'n':
				break;
			default:
				printf("Invalid choice.\n");
		}
	}
	else {
		printf("\nCurrent patient is not diagnosed.\n");
	}
}

// get next ID
int getPatientID (Patient *patients, int patientCount){
	int i,max=0;
	for (i=0;i<patientCount;i++){
		if (patients[i].patientID > max){
			max = patients[i].patientID;
		}
	}
	return max + 1;
}

// Calculate BMI
void calculateBMI (Patient *patient, const float weight, const float height){
	float bmi = weight / (height * height);
	patient->bmi = bmi;

	if(bmi<18.5)
		strcpy(patient->bmiCat, "Underweight");
	else if(bmi<25.0)
		strcpy(patient->bmiCat, "  Healthy  ");
	else if(bmi<30.0)
		strcpy(patient->bmiCat, "Overweight ");
	else 
		strcpy(patient->bmiCat, "   Obese   ");
}

double mmol_conv (double mgdl){
	return mgdl / 38.67;
}

/* Calculate Risk using Predicting Risk of Cardiovascular Disease Events (PREVENT) Equations
   https://professional.heart.org/en/guidelines-and-statements/prevent-calculator
Source code calculation document for the PREVENT Equation is provided by the American Heart Association or AHA
and has been authorized for use after agreeing on their Terms and Conditions and License Agreement.

This function calculates the 10 year risk of total cardiovascular disease for adults in the ages 30-79. 
It utilizes the Cox proportional hazards model to calculate absolute risk based on these factors of the patient:
Age, HDL Cholesterol [Blood Test], Systolic Blood Pressure, estimated Glomerular Filtration Rate [Blood Test],
Diabetes, Lifestyle, and Medications.

@param - Patient *patient, the details of the patient whose CVD risk will be measured.
*/
void calculateCardioRisk (Patient *patient){
	double logor_10yr_CVD;
	// parse SBP
	int SBP=0,i;
	for (i=0;i<15 && patient->bp[i]!='/';i++){
		if (patient->bp[i]>='0' && patient->bp[i]<='9'){
			SBP = SBP * 10 + (patient->bp[i] - '0');
		}
	}
	// Booleans
	int nDiabetes = 0;
	int nSmoking = 0;
	int nHTMed = 0;
	int nStatin = 0;
	if (patient->diabetes=='Y' || patient->diabetes=='y'){
		nDiabetes = 1;
	}
	if (patient->smoking=='Y' || patient->smoking=='y'){
		nSmoking = 1;
	}
	if (patient->htMed=='Y' || patient->htMed=='y'){
		nHTMed = 1;
	}
	if (patient->statins=='Y' || patient->statins=='y'){
		nStatin = 1;
	}
	int flag=1;
	// Input validation
	if (patient->currentCVD=='Y' || patient->currentCVD=='y')
		flag=0;
	if (!(patient->gender=='M'||patient->gender=='F'||patient->gender=='m'||patient->gender=='f'))
		flag=0;
	if (patient->age < 30 || patient->age > 79)
		flag=0;
	if (patient->totalChol<130 || patient->totalChol>320)
		flag=0;
	if (patient->hdlChol < 20 || patient->hdlChol > 100)
		flag=0;
	if (SBP < 90 || SBP > 200)
		flag=0;
	if (!(nDiabetes==0 || nDiabetes==1))
		flag=0;
	if (!(nSmoking==0 || nSmoking==1))
		flag=0;
	if (patient->eGFR<=0)
		flag=0;
	if (!(nHTMed==0 || nHTMed==1))
		flag=0;
	if (!(nStatin==0 || nStatin==1))
		flag=0;
	
	if ((patient->gender=='F' || patient->gender=='f')&& flag==1){
		logor_10yr_CVD = -3.307728 +
        				 0.7939329*(patient->age - 55)/10 +
        				 0.0305239*(mmol_conv(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.1606857*(mmol_conv(patient->hdlChol) - 1.3)/(0.3) -
        				 0.2394003*(fmin(SBP, 110) - 110)/20 +
        				 0.360078*(fmax(SBP, 110) - 130)/20 +
        				 0.8667604*(nDiabetes) +
        				 0.5360739*(nSmoking) +
        				 0.6045917*(fmin(patient->eGFR, 60) - 60)/(-15) +
        				 0.0433769*(fmax(patient->eGFR, 60) - 90)/(-15) +
        				 0.3151672*(nHTMed) -
        				 0.1477655*(nStatin) -
        				 0.0663612*(nHTMed)*(fmax(SBP, 110) - 130)/20 +
        				 0.1197879*(nStatin)*(mmol_conv(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.0819715*(patient->age - 55)/10*(mmol_conv(patient->totalChol - patient->hdlChol) - 3.5) +
        				 0.0306769*(patient->age - 55)/10*(mmol_conv(patient->hdlChol) - 1.3)/(0.3) -
        				 0.0946348*(patient->age - 55)/10*(fmax(SBP, 110) - 130)/20 -
        				 0.27057*(patient->age - 55)/10*(nDiabetes) -
        				 0.078715*(patient->age - 55)/10*(nSmoking) -
        				 0.1637806*(patient->age - 55)/10*(fmin(patient->eGFR, 60) - 60)/(-15);
	} 
	else if ((patient->gender=='M' || patient->gender=='m') && flag==1){
		logor_10yr_CVD = -3.031168 +
        				 0.7688528*(patient->age - 55)/10 +
        				 0.0736174*(mmol_conv(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.0954431*(mmol_conv(patient->hdlChol) - 1.3)/(0.3) -
        				 0.4347345*(fmin(SBP, 110) - 110)/20 +
        				 0.3362658*(fmax(SBP, 110) - 130)/20 +
        				 0.7692857*(nDiabetes) +
        				 0.4386871*(nSmoking) +
        				 0.5378979*(fmin(patient->eGFR, 60) - 60)/(-15) +
        				 0.0164827*(fmax(patient->eGFR, 60) - 90)/(-15) +
        				 0.288879*(nHTMed) -
        				 0.1337349*(nStatin) -
        				 0.0475924*(nHTMed)*(fmax(SBP, 110) - 130)/20 +
        				 0.150273*(nStatin)*(mmol_conv(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.0517874*(patient->age - 55)/10*(mmol_conv(patient->totalChol - patient->hdlChol) - 3.5) +
        				 0.0191169*(patient->age - 55)/10*(mmol_conv(patient->hdlChol) - 1.3)/(0.3) -
        				 0.1049477*(patient->age - 55)/10*(fmax(SBP, 110) - 130)/20 -
        				 0.2251948*(patient->age - 55)/10*(nDiabetes) -
        				 0.0895067*(patient->age - 55)/10*(nSmoking) -
        				 0.1543702*(patient->age - 55)/10*(fmin(patient->eGFR, 60) - 60)/(-15);
	}
	// computing summation of x = (beta * transformed var) within Risk = (e^x) / (1 + e^x)
	double cardioRisk = (exp(logor_10yr_CVD)) / (1 + exp(logor_10yr_CVD)); // outputted in decimal form, needs to * 100 to get % chance
	if (flag==0)
		cardioRisk=-1;
	patient->cardioRisk=cardioRisk;
}

// Save patient to file
int saveAllPatientsToFile (Patient *patients, int patientCount, const char *filename){
	FILE *fp;
	int flag = 0, i;
	if ((fp=fopen(filename, "w"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		for (i=0;i<patientCount;i++){
			fprintf(fp, "%d, %d, %s, %d, %c, %s, %.2f, %s, %s, %.2f, %c, %.2f, %.2f, %d, %c, %c, %c, %c, %.2f, %c, %c, %c, %c, %.2lf, %c\n",
					patients[i].patientID,
					patients[i].userID,
					patients[i].name,
                	patients[i].age,
                	patients[i].gender,
                	patients[i].contact,
                	patients[i].bmi,
                	patients[i].bmiCat,
                	patients[i].bp,
                	patients[i].bloodSugar,
                	patients[i].currentCVD,
                	patients[i].totalChol,
                	patients[i].hdlChol,
                	patients[i].eGFR,
                	patients[i].htMed,
                	patients[i].statins,
                	patients[i].smoking,
               		patients[i].diabetes,
                	patients[i].creatinine,
                	patients[i].cvdFamily,
                	patients[i].diet,
                	patients[i].exercise,
                	patients[i].alcohol,
                	patients[i].cardioRisk,
					patients[i].isDiagnosed);
		}
		flag=1;
		fclose(fp);
	}
	
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
			Patient temp;
			int result = fscanf(fp,
								"%d, %d, %100[^,], %d, %c, %16[^,], %f, %11[^,], %15[^,], %f, %c, %f, %f, %d, %c, %c, %c, %c, %f, %c, %c, %c, %c, %lf, %c\n",
								&temp.patientID,
								&temp.userID,
								temp.name,
    							&temp.age,
    							&temp.gender,
    							temp.contact,
    							&temp.bmi,
    							temp.bmiCat,
    							temp.bp,
    							&temp.bloodSugar,
    							&temp.currentCVD,
    							&temp.totalChol,
    							&temp.hdlChol,
    							&temp.eGFR,
    							&temp.htMed,
    							&temp.statins,
    							&temp.smoking,
    							&temp.diabetes,
    							&temp.creatinine,
    							&temp.cvdFamily,
    							&temp.diet,
    							&temp.exercise,
    							&temp.alcohol,
    							&temp.cardioRisk,
								&temp.isDiagnosed);
			// since fscanf outputs the amount of input items, we can use it to check if fscanf was successful
			if (result==25 && count < MAX_USERS){
				patients[count] = temp;
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

// Edit name, age, contact
void editPatient (Patient *patient){
	// input name
	printf("Enter new name:\n");
	getValidInput(patient->name,4,0,0,0,0,0,0);
		
	// input age
	printf("Enter new age: ");
	getValidInput(&patient->age,1,0,200,0,0,0,0);

	// input contact details
	printf("Enter new phone number (+63 xxx-xxx-xxxx): ");
	getValidInput(patient->contact,5,0,0,0,0,0,0);
}

// Delete patient
void deletePatient (Patient *patients, int *patientCount, int index){
	int i;
	for (i = index; i < *patientCount - 1;i++){
		patients[i] = patients[i+1];
	}
	(*patientCount)--;
}

// Print patient list
void showPatients (Patient *patient, int count){
	if(count==0)
		printf("No patients found.\n");
	else {
		printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("------------\n");
    	printf("PATIENT LIST");
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("------------\n");
    	printf("| %3s | %15s%10s | %3s | %6s | %11s%5s | %7s%4s | %7s%5s | %5s | %11s | %10s |\n",
    	       "No.", "Name", "", "Age", "Gender", "Contact", "", "BMI", "", "BP", "", "Sugar", "Cardio Risk");
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("------------\n");
    
    	for(int i=0; i<count; i++) {
        	printf("| %03d | %03d | %-25s | %-3d | %3c%3s | %16s | %11s | %-12s | %.2f |",
           		i+1,
           		patient[i].patientID,
            	patient[i].name,
            	patient[i].age,
            	patient[i].gender, "",
            	patient[i].contact,
            	patient[i].bmiCat,
            	patient[i].bp,
            	patient[i].bloodSugar);
            if (patient[i].cardioRisk!=-1){
            	printf("   %.2lf%%    |\n", patient[i].cardioRisk * 100);
			}
			else {
				printf("   %-7s    |\n", "N/A");
			}
    	}
    
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("----------------------------------------");
    	printf("------------\n");
	}
}

void computeAverages(double data[][2], int patientCount){ // param *data - 2D array containing BMI and CRisk data
	double sumBMI=0, sumRisk=0, valid=0;
	int i;
	
	if (patientCount == 0){
		printf("No data available.\n");
	}
	else {
		for (i=0;i<patientCount;i++){
			sumBMI+=data[i][0];
			if (data[i][1] != -1){ // if patient is diagnosed then add to sum
				sumRisk += data[i][1];
				valid++;
			}
		}
	}
	printf("Average BMI: %.2lf\n", sumBMI / patientCount);
	printf("Average Cardiovascular Disease Risk: %.2lf\n", (sumRisk / valid)*100);
}

// selection sort of patientID of patients
void sortPatientsByID (Patient *patients, int patientCount, int order){
	int i, j, index;
	Patient temp;
	
	for (i=0;i<patientCount-1;i++){
		index = i; // sorted portion at i
		// search/select
		for (j=i+1;j<patientCount;j++){
			if (order==1){ // ascending
				if(patients[j].patientID < patients[index].patientID)
					index = j;
			}
			else { // descending
				if(patients[j].patientID > patients[index].patientID)
					index = j;
			}
		}
		// swap
		if (index != i){
			temp = patients[i];
			patients[i] = patients[index];
			patients[index] = temp;
		}
	}
}

// selection sort of nanme of patients
void sortPatientsByName (Patient *patients, int patientCount, int order){
	int i, j, index;
	Patient temp;
	
	for (i=0;i<patientCount-1;i++){
		index = i; // sorted portion at i
		for (j=i+1;j<patientCount;j++){
			if (order==1){ // ascending
				if(strcmp(patients[j].name, patients[index].name) < 0)
					index = j;
			}
			else { // descending
				if(strcmp(patients[j].name, patients[index].name) > 0)
					index = j;
			}
		}
		// swap
		if (index != i){
			temp = patients[i];
			patients[i] = patients[index];
			patients[index] = temp;
		}
	}
}

// search func,, returns index of patient inside patient struct array
// returns -1 if not found
int findPatientByID (Patient *patients, int patientCount, int input){
	int i, index=-1;
	for (i=0;i<patientCount;i++){
		if (patients[i].patientID == input){
			index = i;
			i = patientCount; // end loop
		}
	}
	
	return index;
}

int findPatientByName (Patient *patients, int patientCount, char *input){
	int i, index=-1;
	for (i=0;i<patientCount;i++){
		if (strcmp(patients[i].name,input)==0){
			index = i;
			i = patientCount; // end loop
		}
	}
	
	return index;
}

void selectPatientID (Patient *patients, int *patientCount, Referral *referrals, User *users, User *currentUser, int userCount, int *referralCount){
	int input, choice;
	int index;
	
	printf("Enter Patient ID to select: ");
	getValidInput(&input,1,0,100,0,0,0,0);
	
	index = findPatientByID(patients,*patientCount,input);
	
	if (index == -1){
		printf("Patient not found.\n");
	}
	else {
		do{
			showDiagnosisReport(&patients[index]);
			printf("\n==== Patient CRUD ====\n");
			printf("1. Edit Patient\n");
    	    printf("2. Delete Patient\n");
    	    printf("3. Diagnose Patient\n");
    	    printf("4. Refer Patient\n");
    	    printf("0. Exit\n");
    	    printf("Choice: ");
    	    getValidInput(&choice,1,1,4,0,0,0,0);
    	    
    	    switch(choice){
    	    	case 1:
   		     		editPatient(&patients[index]);
   		     		saveAllPatientsToFile(patients,*patientCount,"patients.txt");
    	    		break;
    	    	case 2:
    	    		deletePatient(patients,patientCount,index);
    	    		saveAllPatientsToFile(patients,*patientCount,"patients.txt");
    	    		break;
    	    	case 3:
    	    		diagnosePatient(&patients[index]);
    	    		saveAllPatientsToFile(patients,*patientCount,"patients.txt");
    	    		break;
    	    	case 4:
    	    		createReferral(referrals, users, &patients[index], *currentUser, userCount, referralCount);
    	    		saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
					break;
    	    	default:
    	    		printf("Invalid input.\n");
			}
		} while(choice!=0);
	}
}

void selectPatientName (Patient *patients, int *patientCount, Referral *referrals, User *users, User *currentUser, int userCount, int *referralCount){
	int choice;
	int index;
	char input[101];
	char cInput;
	printf("Enter name of patient to select: ");
	getValidInput(input,4,0,0,0,0,0,0);
	
	index = findPatientByName(patients,*patientCount,input);
	
	if (index == -1){
		printf("Patient not found.\n");
	}
	else {
		do{
			showDiagnosisReport(&patients[index]);
			printf("\n==== Patient CRUD ====\n");
			printf("1. Edit Patient\n");
    	    printf("2. Delete Patient\n");
    	    printf("3. Diagnose Patient\n");
    	    printf("4. Refer Patient\n");
    	    printf("0. Exit\n");
    	    printf("Choice: ");
    	    getValidInput(&choice,1,0,4,0,0,0,0);
    	    
    	    switch(choice){
    	    	case 1:
   		     		editPatient(&patients[index]);
    	    		saveAllPatientsToFile(patients,*patientCount,"patients.txt");
					break;
    	    	case 2:
    	    		printf("Are you sure you want to delete patient #%d? (Y/N): ", patients[index].patientID);
    	    		
    	    		getValidInput(&cInput,3,0,0,'Y','N','y','n');
					
					switch (cInput){
    	    			case 'Y':
    	    			case 'y':
							deletePatient(patients,patientCount,index);
    	    				break;
    	    			case 'N':
    	    			case 'n':
    	    				break;
    	    			default:
    	    				printf("Invalid input.\n");
					}
    	    		saveAllPatientsToFile(patients,*patientCount,"patients.txt");
					break;
    	    	case 3:
    	    		diagnosePatient(&patients[index]);
    	    		saveAllPatientsToFile(patients,*patientCount,"patients.txt");
					break;
    	    	case 4:
    	    		createReferral(referrals, users, &patients[index], *currentUser, userCount, referralCount);
    	    		saveAllReferralsToFile(referrals,*referralCount,"referrals.txt");
					break;
    	    	default:
    	    		printf("Invalid input.\n");
			}
		} while(choice!=0);
	}
}

void updateGender (Patient *patient){
	if (patient->gender == 'm'){
		patient->gender = 'M';
	}
	else if (patient->gender == 'f'){
		patient->gender = 'F';
	}
}
