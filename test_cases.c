#include "Letran_Navarrosa_Machine-Project.h"

// PRINT HELPERS 
void testInt(char *f, int n, int e, int a) {
    printf("[%s - Test %d] Expected: %d | Actual: %d -> %s\n",
           f,n,e,a,(e==a)?"PASS":"FAIL");
}

void testDouble(char *f, int n, double e, double a) {
    printf("[%s - Test %d] Expected: %.2lf | Actual: %.2lf -> %s\n",
           f,n,e,a,(fabs(e-a)<0.01)?"PASS":"FAIL");
}

void testStr(char *f, int n, char *e, char *a) {
    printf("[%s - Test %d] Expected: %s | Actual: %s -> %s\n",
           f,n,e,a,(strcmp(e,a)==0)?"PASS":"FAIL");
}

// MOCK INPUT HELPER
void setInput(const char *data) {
    FILE *fp = fopen("mock.txt","w");
    fputs(data,fp);
    fclose(fp);
    freopen("mock.txt","r",stdin);
}

// USER FUNCTIONS

void test_getValidInput() {
    int x; char c;

    setInput("25\n");
    getValidInput(&x,1,0,100,0,0,0,0);
    testInt("getValidInput",1,25,x);

    setInput("150\n50\n");
    getValidInput(&x,1,0,100,0,0,0,0);
    testInt("getValidInput",3,50,x);
    
    setInput("M\n");
    getValidInput(&c,3,0,0,'M','F','m','f');
    testInt("getValidInput",2,'M',c);
}

void test_registerUser() {

}

void test_loginUser() {

}

void test_hashPassword() {
    unsigned long h1,h2;
    hashPassword("abc",&h1);
    hashPassword("abc",&h2);
    printf("Hash Password: 1 - %lu, 2 - %lu\n", h1, h2);
    testInt("hashPassword",1,h1==h2,1);

    hashPassword("abc",&h1);
    hashPassword("xyz",&h2);
    printf("Hash Password: 1 - %lu, 2 - %lu\n", h1, h2);
    testInt("hashPassword",2,h1!=h2,1);
}

void test_userFile() {
    //saveAllUSersToFile
    //loadAllUsersFromFile
}

void test_forgotPassword() {
    User u[3]={{1,"A",1,"GP","A",1,"Adobo"},{2,"B",1,"GP","B",2,"Purple"},{3,"C",1,"GP","C",3,"Dog"}};
    
    //test 1 - valid usn -> questType=1
    setInput("Adobo\nabc");
    testInt("forgotPassword",1,1,forgotPassword(u, 3, "A"));

    //test 2 - valid usn -> questType=2
    setInput("Adobo\nPurple\nabc");
    testInt("forgotPassword",2,1,forgotPassword(u, 3, "B"));
    //test 3 - invalid usn
    testInt("forgotPassword",3,0,forgotPassword(u, 3, "D"));
}

void test_getUserID() {
    //test 1 - normal case
    User u[2]={{1},{2}};
    Patient p[1]={{1,5}};
    printf("%d", getUserID(u,p));
    //testInt("getUserID",1,6,getUserID(u,p));

    //test 2 - empty
    //User user[2];
    //Patient patient[1];
    //testInt("getUserID",2,0,getUserID(user,patient));
}

void test_dashboards() {
    //gpDashboard
    //specialistDashboard
    //patientDashboard
}

void test_findUser() {
    //findUserByID
    //findUserByName
    User u[2]={{1,"A",1,"GP"},{2,"B",1,"Patient"}};
    testInt("findUserByID",1,1,findUserByID(u,2,2));
    testInt("findUserByID",1,1,findUserByID(u,2,3));
    testInt("findUserByName",2,-1,findUserByName(u,2,"A"));
}

// PATIENT FUNCTIONS

void test_addPatient() {
    User u={1,"John",1,"Patient"};
    Patient arr[1];
    User users[1];

    setInput("25\nM\n+63 912-345-6789\n70\n1.75\n120/80\n100\n");

    Patient p = addPatient(&u,arr,0,users,0);

    testStr("addPatient",1,"John",p.name);
    testInt("addPatient",2,25,p.age);
}

void test_initPatient() {
    Patient p; initPatient(&p);
    testInt("initPatient",1,-1,p.userID);
}

void test_diagnosePatient() {
    Patient p;
    strcpy(p.name,"Test");
    p.age=50; p.gender='M'; strcpy(p.bp,"120/80");
    p.bloodSugar=100;

    setInput(
        "200\n50\n90\nY\nN\nN\nN\n1.0\nN\nY\nN\nN\n"
    );

    diagnosePatient(&p);

    printf("[diagnosePatient - Test 1] cardioRisk: %.2lf → %s\n",
        p.cardioRisk,
        p.cardioRisk>=0 ? "PASS":"FAIL");
}

void test_showDiagnosisReport() {

}

void test_getPatientID() {
    Patient p[2]={{.patientID=1},{.patientID=3}};
    testInt("getPatientID",1,4,getPatientID(p,2));
}

void test_calculateBMI() {
    Patient p;
    calculateBMI(&p,70,1.75);
    testDouble("calculateBMI",1,22.86,p.bmi);
    testStr("calculateBMI",2,"Healthy",p.bmiCat);
}

void test_mmol() {
    testDouble("mmol_conv",1,2.58,mmol_conv(100));
}

void test_calculateCardioRisk() {

}

void test_patientFile() {
    //saveAllPatientsToFile
    //loadAllPatientsFromFile
    Patient p[1]={{.patientID=1,.userID=1}};
    saveAllPatientsToFile(p,1,"p.txt");

    Patient l[1];
    int c=loadPatientsFromFile(l,"p.txt");

    testInt("patientFile",1,1,c);
}

void test_editPatient() {

}

void test_deletePatient() {
    Patient p[2]={{1},{2}};
    int c=2;
    deletePatient(p,&c,0);
    testInt("deletePatient",1,1,c);
}

void test_computeAverages() {

}

void test_sortPatients() {
    //sortPatientsbyID
    //sortPatientsByName
    Patient p[2]={{2,0,"Z"},{1,0,"A"}};
    sortPatientsByID(p,2,1);
    testInt("sortPatientsByID",1,1,p[0].patientID);

    sortPatientsByName(p,2,1);
    testStr("sortPatientsByName",1,"A",p[0].name);
}

void test_findPatient() {
    //findPatientByID
    //findPatientByUserID
    //findPatientByName
    Patient p[2]={{1,0,"A"},{2,0,"B"}};
    testInt("findPatientByID",1,1,findPatientByID(p,2,2));
    testInt("findPatientByName",2,0,findPatientByName(p,2,"A"));
}

void test_selectPatient() {
    //selectPatientID
    //selectPatientName
}

void test_updateGender() {
    Patient p={.gender='m'};
    updateGender(&p);
    testInt("updateGender",1,'M',p.gender);

    p.gender='f';
    updateGender(&p);
    testInt("updateGender",2,'F',p.gender);
}

// REFERRALS

void test_createReferral() {

}

void test_editReferral() {
    Referral r;
    setInput("1\n");
    editReferral(&r);
    testStr("editReferral",1,"Accepted",r.status);
}

void test_deleteReferral(){
    Referral r[2]={{1},{2}};
    int c=2;
    deleteReferral(r,&c,0);
    testInt("deleteReferral",1,1,c);
}

void test_referralFile(){
    //saveAllReferralsToFile
    //loadAllReferralsFromFile
    Referral r[1]={{.referralID=1}};
    saveAllReferralsToFile(r,1,"r.txt");

    Referral l[1];
    int c=loadReferralsFromFile(l,"r.txt");

    testInt("referralFile",1,1,c);
}

void test_getReferralID(){
    Referral r[2]={{1},{5}};
    testInt("getReferralID",1,6,getReferralID(r,2));
}

void test_sortReferrals(){
    //sortReferralByID
    //sortReferralByStatus
    Referral r[2]={{2,0,0,"",0,"B"},{1,0,0,"",0,"A"}};

    sortReferralsByID(r,2,1);
    testInt("sortReferralsByID",1,1,r[0].referralID);

    sortReferralsByStatus(r,2,1);
    testStr("sortReferralsByStatus",2,"A",r[0].status);
}

void test_findReferralByID() {

}

void test_selectReferralID() {

}

// MAIN 
int main(){

    printf("=== TEST RUNNER ===\n\n");

    // USER 
    //test_getValidInput();
    //test_registerUser();
    //test_loginUser;
    //test_hashPassword();
    //test_editUserDetails();
    //test_userFile();
    //test_forgotPassword();
    test_getUserID();
    //test_dashboards();
    //test_findUser();

    // PATIENT 
    //test_addPatient();
    //test_initPatient();
    //test_diagnosePatient();
    //test_showDiagnosisReport();
    //test_getPatientID();
    //test_calculateBMI();
    //test_mmol();
    //test_calculateCardioRisk();
    //test_patientFile();
    //test_editPatient();
    //test_deletePatient();
    //test_computeAverages();
    //test_sortPatients();
    //test_findPatient();
    //test_selectPatient();
    //test_updateGender();

    // REFERRALS 
    //test_createReferral();
    //test_editReferral();
    //test_deleteReferral();
    //test_referralFile();
    //test_getReferralID();
    //test_sortReferrals();
    //test_findReferralByID();
    //test_selectReferralID();

    printf("\n=== END OF TESTS ===\n");

    return 0;
}