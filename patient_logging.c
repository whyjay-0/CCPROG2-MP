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
		valid=0;
		
		// input gender
		printf("Enter your gender at birth (M/F): ");
		do{
			scanf(" %c",&newPatient.gender);
			if (newPatient.gender!='M' && newPatient.gender!='F'){
				printf("Invalid gender. Enter valid gender: \n");
			}
			else {
				valid=1;
			}
		} while (valid==0);
		valid=0;
		
		// input contact details
		printf("Enter phone number (+63 xxx-xxx-xxxx): ");
		do{
			scanf(" %17[^\n]",strInput);
			if (strlen(strInput)!=16){
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
		valid=0;
		
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
*/
// Diagnose a patient, will give suggestions and risk level based on the details that 
// will be input here and from the addPatient function
void diagnosePatient (Patient *patient){
	printf("----- Diagnosing Patient: %s -----\n", patient->name);
	
	// Ask user for needed info
	// Cholesterol values
	printf("Enter Total Cholesterol (mg/dL): ");
	scanf("%f", &patient->totalChol);
	printf("Enter HDL Cholesterol (mg/dL): ");
	scanf("%f", &patient->hdlChol);
	
	// eGFR value, estimated Glomerular Filtration Rate
	printf("Enter eGFR (mL/min/1.73m^2): ");
	scanf("%d", &patient->eGFR);

	// BP Treatment and Values
	if (strcmp(patient->bp,"")==0){
		printf("Enter blood pressure (SYS/DIA mmHg): ");
		scanf(" %16[^\n]",patient->bp);
	}
	
	printf("Is patient using anti-hypertensive medication? (Y/N): ");
	scanf(" %c", &patient->htMed);
	
	// CVD
	printf("Does patient have any known cardiovascular disease? (Y/N): ");
	scanf(" %c", &patient->currentCVD);
	
	// Blood sugar
	if (patient->bloodSugar==0){
		printf("Enter blood sugar (mg/dL): ");
		scanf("%f", &patient->bloodSugar);
	}
	
	// LDL treatment
	printf("Is patient using statins? (Y/N): ");
	scanf(" %c", &patient->statins);

	// Smoking
	printf("Is patient a regular smoker? (Y/N): ");
	scanf(" %c", &patient->smoking);

	// Diabetes
	printf("Does patient have diabetes? (Y/N): ");
	scanf(" %c", &patient->diabetes);
	
	// Creatinine
	printf("Enter Serum Creatinine of patient (mg/dL): ");
	scanf("%f", &patient->creatinine);
	
	// Family History CVD
	printf("Does patient have family history of CVD? (Y/N): ");
	scanf(" %c", &patient->cvdFamily);
	
	// Diet/Exercise
	printf("Does patient exercise regularly? (Y/N): ");
	scanf(" %c", &patient->exercise);
	printf("Does patient have a high-fat/sugar diet? (Y/N): ");
	scanf(" %c", &patient->diet);
	
	// Alcohol
	printf("Does patient consume alcohol regularly? (Y/N): ");
	scanf(" %c", &patient->alcohol);

	// Calculate Cardio Risk,, Can only be done on adults ages 30-79 without past CVD (This is assumed to be true).
	if (patient->age>=30 && patient->age<=79 && patient->currentCVD=='N')
		calculateCardioRisk(patient);
	else
		patient->cardioRisk = -1.0; // This should mean invalid since outside of age range

	printf("---------- DIAGNOSIS REPORT ----------\n\n");
	// Details
	printf("Patient: %s\n", patient->name);
	printf("Age: %d\nGender: %c\n", patient->age, patient->gender);
	printf("BMI: %f\n\n", patient->bmi);
	// Will be shown if calculateCardioRisk was done otherwise other details will be shown.
	// Temporary interpretations Changes might be made once AHA provides proper source code.
	if (patient->age>=30 && patient->age<=79 && patient->currentCVD=='N'){
		// Cardio Risk
		printf("10-Year Cardiovasular Risk: %.2lf%%\n", patient->cardioRisk * 100);
		// Risk Level Classification
		if(patient->cardioRisk < 0.05)
			printf("   Risk Level: Low Risk\n\n");
		else if(patient->cardioRisk >= 0.05 && patient->cardioRisk <= 0.074)
			printf("   Risk Level: Borderline Risk\n\n");
		else if(patient->cardioRisk >= 0.075 && patient->cardioRisk <= 0.199)
			printf("   Risk Level: Intermediate Risk\n\n");
		else
			printf("   Risk Level: Very High Risk\n\n");
	}
	*/
	// Other data and suggestions
	printf("Data and Suggestions:\n");
	if (patient->bmi>30)
		printf("   - Suggestion: Consider weight loss through diet and exercise.\n");
	if (patient->creatinine>1.2)
		printf("   - Kidney Function is decreased. Consider further renal evaluation.\n");
	if (patient->cvdFamily=='Y')
		printf("   - Increased risk of cardiovascular disease due to family history.\n");
	if (patient->diet=='Y')
		printf("   - Modify diet to lower fat and sugar intake\n");
	if (patient->exercise=='Y')
		printf("   - Continue regular physical activity\n");
	if (patient->alcohol=='Y')
		printf("   - Limit alcohol intake to reduce cardiovascular risk.\n");
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
void calculateCardioRisk(Patient *patient){
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
	if (patient->diabetes=='Y'){
		nDiabetes = 1;
	}
	if (patient->smoking=='Y'){
		nSmoking = 1;
	}
	if (patient->htMed=='Y'){
		nHTMed = 1;
	}
	if (patient->statins=='Y'){
		nStatin = 1;
	}
	int flag=1;
	// Input validation
	if (patient->currentCVD=='Y')
		flag=0;
	if (!(patient->gender=='M'||patient->gender=='F'))
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
	
	if (patient->gender=='F' && flag==1){
		logor_10yr_CVD = -3.307728 +
        				 0.7939329*(patient->age - 55)/10 +
        				 0.0305239*(mmol_conversion(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.1606857*(mmol_conversion(patient->hdlChol) - 1.3)/(0.3) -
        				 0.2394003*(min(SBP, 110) - 110)/20 +
        				 0.360078*(max(SBP, 110) - 130)/20 +
        				 0.8667604*(nDiabetes) +
        				 0.5360739*(nSmoking) +
        				 0.6045917*(min(patient->eGFR, 60) - 60)/(-15) +
        				 0.0433769*(max(patient->eGFR, 60) - 90)/(-15) +
        				 0.3151672*(nHTMed) -
        				 0.1477655*(nStatin) -
        				 0.0663612*(nHTMed)*(max(SBP, 110) - 130)/20 +
        				 0.1197879*(nStatin)*(mmol_conversion(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.0819715*(patient->age - 55)/10*(mmol_conversion(patient->totalChol - patient->hdlChol) - 3.5) +
        				 0.0306769*(patient->age - 55)/10*(mmol_conversion(patient->hdlChol) - 1.3)/(0.3) -
        				 0.0946348*(patient->age - 55)/10*(max(SBP, 110) - 130)/20 -
        				 0.27057*(patient->age - 55)/10*(nDiabetes) -
        				 0.078715*(patient->age - 55)/10*(nSmoking) -
        				 0.1637806*(patient->age - 55)/10*(min(patient->eGFR, 60) - 60)/(-15);
	} 
	else if (patient->gender=='M' && flag==1){
		logor_10yr_CVD = -3.031168 +
        				 0.7688528*(patient->age - 55)/10 +
        				 0.0736174*(mmol_conversion(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.0954431*(mmol_conversion(patient->hdlChol) - 1.3)/(0.3) -
        				 0.4347345*(min(SBP, 110) - 110)/20 +
        				 0.3362658*(max(SBP, 110) - 130)/20 +
        				 0.7692857*(nDiabetes) +
        				 0.4386871*(nSmoking) +
        				 0.5378979*(min(patient->eGFR, 60) - 60)/(-15) +
        				 0.0164827*(max(patient->eGFR, 60) - 90)/(-15) +
        				 0.288879*(nHTMed) -
        				 0.1337349*(nStatin) -
        				 0.0475924*(nHTMed)*(max(SBP, 110) - 130)/20 +
        				 0.150273*(nStatin)*(mmol_conversion(patient->totalChol - patient->hdlChol) - 3.5) -
        				 0.0517874*(patient->age - 55)/10*(mmol_conversion(patient->totalChol - patient->hdlChol) - 3.5) +
        				 0.0191169*(patient->age - 55)/10*(mmol_conversion(patient->hdlChol) - 1.3)/(0.3) -
        				 0.1049477*(patient->age - 55)/10*(max(SBP, 110) - 130)/20 -
        				 0.2251948*(patient->age - 55)/10*(nDiabetes) -
        				 0.0895067*(patient->age - 55)/10*(nSmoking) -
        				 0.1543702*(patient->age - 55)/10*(min(patient->eGFR, 60) - 60)/(-15)
	}
	// computing summation of x = (beta * transformed var) within Risk = (e^x) / (1 + e^x)
	cardioRisk = (exp(logor_10yr_CVD)) / (1 + exp(logor_10yr_CVD)); // outputted in decimal form, needs to * 100 to get % chance
	if (flag==0)
		cardioRisk=-1;
	patient->cardioRisk=cardioRisk;
}

// Save patient to file
int savePatientToFile (const Patient *patient, const char *filename){
	FILE *fp;
	int flag = 0;
	if ((fp=fopen(filename, "a"))==NULL){
		fprintf(stderr, "Error: %s does not exist.\n", filename);
	}
	else {
		fprintf(fp, "%s,%d,%c,%s,%.2f,%s,%s,%.2f,%c,%.2f,%.2f,%d,%c,%c,%c,%c,%.2f,%c,%c,%c,%c,%.2lf\n",
				patient->name,
                patient->age,
                patient->gender,
                patient->contact,
                patient->bmi,
                patient->bmiCat,
                patient->bp,
                patient->bloodSugar,
                patient->currentCVD,
                patient->totalChol,
                patient->hdlChol,
                patient->eGFR,
                patient->htMed,
                patient->statins,
                patient->smoking,
                patient->diabetes,
                patient->creatinine,
                patient->cvdFamily,
                patient->diet,
                patient->exercise,
                patient->alcohol,
                patient->cardioRisk);
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
			Patient temp;
			int result = fscanf(fp,
								"%100[^,],%d,%c,%16[^,],%f,%11[^,],%15[^,],%f,%c,%f,%f,%d,%c,%c,%c,%c,%f,%c,%c,%c,%c,%lf\n",
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
    							&temp.cardioRisk);
			// since fscanf outputs the amount of input items, we can use it to check if scanf was successful
			if (result==22){
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
void editPatient (Patient *patient, int count){
	char searchName[101];
	int found = 0;
	char strInput[101];
	int valid = 0;

	printf("Enter patient name to edit: ");
	scanf(" %100[^\n]", searchName);

	for(int i=0; i<count; i++) {
		if(strcmp(patient[i].name,searchName) == 0) {
			// input name
			printf("Enter new name:\n");
			do{
				scanf(" %100[^\n]",strInput);
				if (strlen(strInput)>100){
					printf("Invalid name. Enter name again:\n");
				}
				else{
					strcpy(patient[i].name,strInput);
					valid=1;
				}
			} while(valid==0);
			valid=0;
		
			// input age
			printf("Enter new age: ");
			do{
				scanf("%d",&patient[i].age);
				if (patient[i].age<=0){
					printf("Invalid age. Enter valid age: \n");
				}
				else {
					valid=1;
				}
			} while (valid==0);
			valid=0;
		
			// input contact details
			printf("Enter new phone number (+63 xxx-xxx-xxxx): ");
			do{
				scanf(" %17[^\n]",strInput);
				if (strlen(strInput)!=16){
					printf("Invalid details. Enter details again:\n");
				}
				else {
					strcpy(patient[i].contact,strInput);
					valid=1;
				}
			} while(valid==0);
			found=1;
			i=count;
		}	
	}	
	if(!found)
		printf("Patient not found.\n"); //maglloop ba dapat to like babalik sa mageenter ng name to edit,,,
}

// Delete patient
void deletePatient (Patient *patient, int *count){
	char searchName[101];
	int found=0;

	printf("Enter patient name to delete: ");
	scanf(" %100[^\n]", searchName);

	for(int i=0; i < *count; i++) {
		if(strcmp(patient[i].name,searchName) == 0) {
			// Shift patients to the left
			for(int j=i; j < *count-1; j++) 
				patient[j] = patient[j+1];

			(*count)--; //reduce total patient count
			found=1;

			printf("Patient deleted successfully.\n");
			i=*count;
		}	
	}	
	if(!found)
		printf("Patient not found.\n");
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
        	printf("| %03d | %-25s | %-3d | %3c%3s | %16s | %11s | %-12s | %.2f |",
           		i+1, 
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
