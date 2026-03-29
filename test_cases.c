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

    //test 1 - valid integer input
    setInput("25\n");
    getValidInput(&x,1,0,100,0,0,0,0);
    testInt("getValidInput",1,25,x);

    //test 2 - above max, invalid, then valid
    setInput("150\n50\n");
    getValidInput(&x,1,0,100,0,0,0,0);
    testInt("getValidInput",2,50,x);
    
    //test 3 - valid char
    setInput("M\n");
    getValidInput(&c,3,0,0,'M','F','m','f');
    testInt("getValidInput",3,'M',c);
}

void test_registerUser() {
    User u[4]={{1,"Jess"},{2,"Sab"}};
    Patient p[2];
    int c=2;

    //test 1 - valid registration: new patient
    setInput("A\nAsh\nabc\n1\n1\nAdobo\n");
    testInt("registerUser",1,2,registerUser(u,&c,p,0));
    printf("Count: %d (Expected: 3)", c);

    //test 2 - duplicate usn, gp
    setInput("Jess\nok\nB\nBat\nxyz\n2\n3\nCat\n");
    testInt("registerUser",1,3,registerUser(u,&c,p,0));
    printf("Count: %d (Expected: 4)", c);
}

void test_loginUser() {
    User u[1]={{1,"A",193485963}};

    //test 1 - correct login
    setInput("A\nabc\n");
    User *user = loginUser(u,1);
    testStr("loginUser",1,"A",user->username);

    //test 2 - wrong password
    setInput("A\nxyz\nabc\n");
    User *user1 = loginUser(u,1);
    testStr("loginUser",2,"A",user1->username);
}

void test_hashPassword() {
    unsigned long h1,h2;

    //test 1 - same input
    hashPassword("abc",&h1);
    hashPassword("abc",&h2);
    printf("Hash Password: 1 - %lu, 2 - %lu\n", h1, h2);
    testInt("hashPassword",1,h1==h2,1);

    //test 2 - different input
    hashPassword("abc",&h1);
    hashPassword("xyz",&h2);
    printf("Hash Password: 1 - %lu, 2 - %lu\n", h1, h2);
    testInt("hashPassword",2,h1!=h2,1);
}

void test_editUserDetails() {
    User u[1]={{1,"A",1,"GP","Jess",1,"Adobo","h"}};

    //test 1 - change username
    setInput("1\nSab\n");
    editUserDetails(u,1,u);
    testStr("editUserDeatils",1,"Sab",u[0].username);

    //test 2 - edit security question
    setInput("3\n2\nPurple\n");
    editUserDetails(u,1,u);
    testStr("editUserDeatils",2,"Purple",u[0].answer);
}

void test_userFile() {
    //saveAllUSersToFile
    //loadAllUsersFromFile
    User u[3]={{1,"A",1,"GP","Ash",1,"Ad","A"},{2,"B",2,"Specialist","Bat",2,"Blue","B"},{3,"C",3,"Patient","Cat",3,"C","C"}};

    //test 1 - valid
    testInt("saveAllUsersToFile",1,1,saveAllUsersToFile(u,3,"u.txt"));
    testInt("loadUsersFromFile",1,3,loadUsersFromFile(u,"u.txt"));

    //test 2 - invalid
    testInt("saveAllUsersToFile",2,0,saveAllUsersToFile(u,3,""));
    testInt("loadUsersFromFile",2,0,loadUsersFromFile(u,""));
}

void test_forgotPassword() {
    User u[3]={{1,"A",1,"GP","A",1,"Adobo"},{2,"B",1,"GP","B",2,"Purple"},{3,"C",1,"GP","C",3,"Dog"}};
    
    //test 1 - valid usn -> questType=1
    setInput("Adobo\nabc\nok\n");
    testInt("forgotPassword",1,1,forgotPassword(u, 3, "A"));

    //test 2 - valid usn -> questType=2
    setInput("Adobo\nPurple\nabc\nok\n");
    testInt("forgotPassword",2,1,forgotPassword(u, 3, "B"));
    //test 3 - invalid usn
    testInt("forgotPassword",3,0,forgotPassword(u, 3, "D"));
}

void test_getUserID() {
    //test 1 - normal case
    User u[100]={{1},{2}};
    Patient p[100]={{1,5}};
    testInt("getUserID",1,6,getUserID(u,p));

    //test 2 - empty
    User user[100];
    Patient patient[100];
    testInt("getUserID",2,1,getUserID(user,patient));
}

void test_findUser() {
    //findUserByID
    //findUserByName
    User u[2]={{1,"A",1,"GP","Ash"},{2,"B",1,"Patient","Bat"}};
    testInt("findUserByID",1,1,findUserByID(u,2,2));
    testInt("findUserByID",2,-1,findUserByID(u,2,3));
    printf("\n");
    testInt("findUserByName",1,0,findUserByName(u,2,"Ash"));
    testInt("findUserByName",2,-1,findUserByName(u,2,"Cat"));
}

// PATIENT FUNCTIONS

void test_addPatient() {
    User u[2]={{1,"John",1,"Patient", "John"},{2,"Jess",1,"GP", "Jess"}};
    Patient arr[100];
    User users[100];

    //test 1 - patient self-add
    setInput("k\n25\nM\n+63 912-345-6789\n70\n1.75\n120/80\n100\n");

    Patient p = addPatient(&u[0],arr,0,users,0);

    testStr("addPatient",1,"John",p.name);
    testInt("addPatient",1,25,p.age);

    //test 2 - GP adds patient
    setInput("John\n25\nM\n+63 912-345-6789\n70\n1.75\n120/80\n100\n");

    p = addPatient(&u[1],arr,0,users,0);

    testStr("addPatient",2,"John",p.name);
    testInt("addPatient",2,25,p.age);
}

void test_initPatient() {
    //test 1 - empty struct
    Patient p; initPatient(&p);
    testInt("initPatient",1,-1,p.userID);
    testDouble("initPatient",1,-1.0,p.cardioRisk);
    printf("\n");

    //test 2 - struct with garbage values
    Patient pt={1,1,"A",24,'M',"w",1.0,"q","as",4.0,.cardioRisk=9.0}; 
    initPatient(&pt);
    testInt("initPatient",2,-1,pt.userID);
    testDouble("initPatient",2,-1.0,pt.cardioRisk);
}

void test_diagnosePatient() {
    Patient p;
    //test 1 - valid data
    strcpy(p.name,"Test");
    p.age=50; p.gender='M'; strcpy(p.bp,"120/80");
    p.bloodSugar=100;

    setInput(
        "200\n50\n90\nY\nN\nN\nN\nN\n1.0\nN\nY\nN\nN\nh\n"
    );

    diagnosePatient(&p);
    printf("[diagnosePatient - Test 1] cardioRisk: %.2lf -> %s\n",
        p.cardioRisk,
        p.cardioRisk>=0 ? "PASS":"FAIL");
    
    //test 2 - invalid age
    p.age=20;
    
    setInput(
        "200\n50\n90\nY\nN\nN\nN\nN\n1.0\nN\nY\nN\nN\nh\n"
    );

    diagnosePatient(&p);
    printf("[diagnosePatient - Test 2] cardioRisk: %.2lf -> %s\n",
        p.cardioRisk,
        p.cardioRisk==-1 ? "PASS":"FAIL");
}

void test_getPatientID() {
    //test 1 - normal list
    Patient p1[3]={{1},{2},{3}};
    testInt("getPatientID",1,4,getPatientID(p1,3));

    //test 2 - non-sequential list
    Patient p2[3]={{1},{5},{4}};
    testInt("getPatientID",2,6,getPatientID(p2,3));
}

void test_calculateBMI() {
    Patient p;
    //test 1 - normal
    calculateBMI(&p,70,1.75);
    testDouble("calculateBMI",1,22.86,p.bmi);
    testStr("calculateBMI",1,"Healthy",p.bmiCat);

    //test 2 - underweight
    calculateBMI(&p,45,1.7);
    testDouble("calculateBMI",2,15.57,p.bmi);
    testStr("calculateBMI",2,"Underweight",p.bmiCat);

    //test 3 - obese
    calculateBMI(&p,100,1.8);
    testDouble("calculateBMI",3,30.86,p.bmi);
    testStr("calculateBMI",3,"Obese",p.bmiCat);
}

void test_mmol() {
    //test 1 - convert
    testDouble("mmol_conv",1,2.58,mmol_conv(100));

    //test 2 - zero input
    testDouble("mmol_conv",1,0.00,mmol_conv(0));
}

void test_calculateCardioRisk() {
    Patient p[1]={{1,1,"A",50,'M',"+63",2.0,"h","110/80",2.0,'N',150,80,1,'N','N','Y','N',2.0,'N','N','N','N',0,'N',1}};

    //test 1 - valid
    calculateCardioRisk(p);
    testDouble("calculateCardioRisk",1,0.21,p[0].cardioRisk);

    //test 2 - invalid age
    p[0].age=20;
    calculateCardioRisk(p);
    testDouble("calculateCardioRisk",2,-1,p[0].cardioRisk);

    //test 3 - has CVD
    p[0].age=50;
    p[0].currentCVD='Y';
    calculateCardioRisk(p);
    testDouble("calculateCardioRisk",3,-1,p[0].cardioRisk);
}

void test_patientFile() {
    //saveAllPatientsToFile
    //loadAllPatientsFromFile
    Patient p[1]={{1,1,"A",1,'M',"+63",2.0,"h","a",2.0,'N',3.0,2.0,1,'N','N','N','N',2.0,'N','N','N','N',2.0,'N',1}};
    
    //test 1 - valid 
    testInt("saveAllPatientsToFile",1,1,saveAllPatientsToFile(p,1,"p.txt"));
    testInt("loadPatientsFromFile",1,1,loadPatientsFromFile(p,"p.txt"));

    //test 1 - valid 
    testInt("saveAllPatientsToFile",2,0,saveAllPatientsToFile(p,1,""));
    testInt("loadPatientsFromFile",2,0,loadPatientsFromFile(p,""));
}

void test_editPatient() {
    Patient p[1]={{1,1,"Jess",18,'F',"+63 123-456-7890"}};

    //test 1 - update all fields
    setInput("Jessica\n20\n+63 098-765-4321\n");
    editPatient(p);
    printf("Expected: Jessica 20 +63 098-765-4321\n");
    printf("Actual: %s %d %s\n", p[0].name,p[0].age,p[0].contact);

    //test 1 - update age only
    setInput("Jessica\n18\n+63 098-765-4321\n");
    editPatient(p);
    printf("Expected: Jessica 18 +63 098-765-4321\n");
    printf("Actual: %s %d %s\n", p[0].name,p[0].age,p[0].contact);
}

void test_deletePatient() {
    //test 1 - delete middle
    Patient p1[3]={{1},{2},{3}};
    int c=3;
    deletePatient(p1,&c,1);
    printf("[%d,%d]", p1[0].patientID, p1[1].patientID);
    testInt("deletePatient",1,2,c);

    //test 2 - delete first
    Patient p2[3]={{1},{2},{3}};
    c=3;
    deletePatient(p2,&c,0);
    printf("[%d,%d]", p2[0].patientID, p2[1].patientID);
    testInt("deletePatient",2,2,c);
}

void test_computeAverages() {
    float data[2][2]={{20, 0.21}, {35,0}};

    //test 1 - valid data
    computeAverages(data,2);
    printf("Expected: Avg BMI: 27.50 | Avg CRisk: 10.50");

    //test 2 - no data
    computeAverages(data,0);
    printf("Expected: No data available.\n");
}

void test_sortPatients() {
    //sortPatientsbyID
    //sortPatientsByName

    //test 1 - ascending
    Patient p[4]={{2,0,"L"},{3,0,"X"},{1,0,"A"},{4,0,"T"}};
    sortPatientsByID(p,4,1);
    printf("sortPatientsById - Ascending\n");
    printf("%d %s\n", p[0].patientID, p[0].name);
    printf("%d %s\n", p[1].patientID, p[1].name);
    printf("%d %s\n", p[2].patientID, p[2].name);
    printf("%d %s\n\n", p[3].patientID, p[3].name);
    sortPatientsByName(p,4,1);
    printf("sortPatientsByName - Ascending\n");
    printf("%d %s\n", p[0].patientID, p[0].name);
    printf("%d %s\n", p[1].patientID, p[1].name);
    printf("%d %s\n", p[2].patientID, p[2].name);
    printf("%d %s\n\n", p[3].patientID, p[3].name);

    //test 2 - descending
    Patient p2[4]={{2,0,"L"},{3,0,"X"},{1,0,"A"},{4,0,"T"}};
    sortPatientsByID(p2,4,2);
    printf("sortPatientsById - Descending\n");
    printf("%d %s\n", p2[0].patientID, p2[0].name);
    printf("%d %s\n", p2[1].patientID, p2[1].name);
    printf("%d %s\n", p2[2].patientID, p2[2].name);
    printf("%d %s\n\n", p2[3].patientID, p2[3].name);
    sortPatientsByName(p2,4,2);
    printf("sortPatientsByName - Descending\n");
    printf("%d %s\n", p2[0].patientID, p2[0].name);
    printf("%d %s\n", p2[1].patientID, p2[1].name);
    printf("%d %s\n", p2[2].patientID, p2[2].name);
    printf("%d %s\n\n", p2[3].patientID, p2[3].name);
}

void test_findPatient() {
    //findPatientByID
    //findPatientByUserID
    //findPatientByName
    Patient p[4]={{1,2,"Jess"},{2,4,"Sab"},{3,6,"Kai"},{4,8,"Ri"}};

    //test 1 - Found
    testInt("findPatientByID",1,0,findPatientByID(p,4,1));
    testInt("findPatientByUserID",1,3,findPatientByUserID(p,4,8));
    testInt("findPatientByName",1,2,findPatientByName(p,4,"Kai"));

    //test 2 - Not Found
    testInt("findPatientByID",2,-1,findPatientByID(p,4,7));
    testInt("findPatientByUserID",2,-1,findPatientByUserID(p,4,5));
    testInt("findPatientByName",2,-1,findPatientByName(p,4,"Den"));
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
    Referral r[100] = {0};
    User u[3] = {{1,"A",1,"GP"},{2,"B",1,"Specialist"},{3,"C",1,"Patient"}};
    Patient p[1] = {{1,6,"John",}};
    int refCount = 0;

    //test 1 - valid referral
    p[0].isDiagnosed='Y';
    p[0].isReferred=0;
    setInput("1\n2\n");
    createReferral(r,u, &p[0], u[0], 3, &refCount, p);
    testInt("createReferral",1,1,refCount);
    testStr("createReferral",1,"Pending",r[0].status);

    // test 2 - not diagnosed
    p[0].isDiagnosed = 'N';
    p[0].isReferred = 0;
    setInput("ok\n");
    createReferral(r, u, &p[0], u[0], 3, &refCount, p);
    testInt("createReferral",2,1,refCount);
}

void test_editReferral() {
    Referral r;
    
    //test 1 - Accept
    setInput("1\n");
    editReferral(&r);
    testStr("editReferral",1,"Accepted",r.status);

    //test 2 - Reject
    setInput("3\n");
    editReferral(&r);
    testStr("editReferral",2,"Rejected",r.status);

    //test 3 - Invalid then valid
    setInput("4\n2\n");
    editReferral(&r);
    testStr("editReferral",3,"Completed",r.status);
}

void test_deleteReferral(){
    //test 1 - delete middle
    Referral r[3]={{1},{2},{3}};
    int c=3;
    deleteReferral(r,&c,1);
    printf("Actual: [%d,%d,%d]   |   Expected: [1,3,0]\n", r[0].referralID, r[1].referralID, r[2].referralID);
    testInt("deleteReferral",1,2,c);

    //test 2 - delete last
    Referral r2[3]={{1},{2},{3}};
    c=3;
    deleteReferral(r2,&c,2);
    printf("\nActual: [%d,%d,%d]   |   Expected: [1,2,0]\n", r2[0].referralID, r2[1].referralID, r2[2].referralID);
    testInt("deleteReferral",2,2,c);
}

void test_referralFile(){
    //saveAllReferralsToFile
    //loadAllReferralsFromFile

    //test 1 - save data and valid file
    Referral r1[2]={{1,1,1,"John",1,"Completed"},{2,2,2,"Jess",2,"Rejected"}};
    testInt("saveAllReferralsToFile",1,1,saveAllReferralsToFile(r1,2,"r.txt"));
    testInt("loadAllReferralsFromFile",1,2,loadReferralsFromFile(r1,"r.txt"));

     //test 1 - invalid path and missing file
    Referral r2[2]={{1,1,1,"John",1,"Completed"},{2,2,2,"Jess",2,"Rejected"}};
    testInt("saveAllReferralsToFile",2,0,saveAllReferralsToFile(r2,2,""));
    testInt("loadAllReferralsFromFile",2,0,loadReferralsFromFile(r2,""));

}

void test_getReferralID(){
    //test 1 - Normal list
    Referral r1[3]={{1},{2},{3}};
    testInt("getReferralID",1,4,getReferralID(r1,3));
    
     //test 2 - non-sequential list
    Referral r2[3]={{1},{5},{3}};
    testInt("getReferralID",1,6,getReferralID(r2,3));
}

void test_sortReferrals(){
    //sortReferralByID
    //sortReferralByStatus

    //test 1 - ascending
    Referral r1[4]={{2,0,0,"",0,"Completed"},{1,0,0,"",0,"Accepted"},{4,0,0,"",0,"Pending"},{3,0,0,"",0,"Rejected"}};
    sortReferralsByID(r1,4,1);
    printf("sortReferralsById - Ascending\n");
    printf("%d %s\n", r1[0].referralID, r1[0].status);
    printf("%d %s\n", r1[1].referralID, r1[1].status);
    printf("%d %s\n", r1[2].referralID, r1[2].status);
    printf("%d %s\n\n", r1[3].referralID, r1[3].status);
    sortReferralsByStatus(r1,4,1);
    printf("sortReferralsByStatus - Ascending\n");
    printf("%d %s\n", r1[0].referralID, r1[0].status);
    printf("%d %s\n", r1[1].referralID, r1[1].status);
    printf("%d %s\n", r1[2].referralID, r1[2].status);
    printf("%d %s\n\n", r1[3].referralID, r1[3].status);

    //test 2 - descending
    Referral r2[4]={{2,0,0,"",0,"Completed"},{1,0,0,"",0,"Accepted"},{4,0,0,"",0,"Pending"},{3,0,0,"",0,"Rejected"}};
    sortReferralsByID(r2,4,2);
    printf("sortReferralsById - Descending\n");
    printf("%d %s\n", r2[0].referralID, r2[0].status);
    printf("%d %s\n", r2[1].referralID, r2[1].status);
    printf("%d %s\n", r2[2].referralID, r2[2].status);
    printf("%d %s\n\n", r2[3].referralID, r2[3].status);
    sortReferralsByStatus(r2,4,2);
    printf("sortReferralsByStatus - Descending\n");
    printf("%d %s\n", r2[0].referralID, r2[0].status);
    printf("%d %s\n", r2[1].referralID, r2[1].status);
    printf("%d %s\n", r2[2].referralID, r2[2].status);
    printf("%d %s\n\n", r2[3].referralID, r2[3].status);
}

void test_findReferralByID() {
    Referral r[4]={{1},{2},{3},{4}};

    //test 1 - found
    testInt("findReferralByID",1,1,findReferralByID(r,4,2));
    //Test 2 - not found
    testInt("findReferralByID",2,-1,findReferralByID(r,4,5));
}

// MAIN 
int main(){

    printf("=== TEST RUNNER ===\n\n");

    // USER 
    //test_getValidInput(); 
    //test_registerUser(); 
    //test_loginUser(); 
    //test_hashPassword(); 
    //test_editUserDetails(); 
    //test_userFile();
    //test_forgotPassword(); 
    //test_getUserID(); 
    //test_findUser(); 

    // PATIENT 
    //test_addPatient(); 
    //test_initPatient(); 
    //test_diagnosePatient(); 
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
    //test_updateGender(); 

    // REFERRALS 
    //test_createReferral();
    //test_editReferral(); 
    //test_deleteReferral(); 
    //test_referralFile(); 
    //test_getReferralID(); 
    //test_sortReferrals(); 
    //test_findReferralByID(); 

    printf("\n=== END OF TESTS ===\n");

    return 0;
}