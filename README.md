## Overview

PRMS (Patient Referral Management System) addresses the lack of referral system and the fragmented medical records scattered in local hospitals by providing a centralized, local, and organized record system created for local groups of hospitals.

Patients, General Practitioners (GPs), and Specialists can securely access, manage, and share patient information through a unified referral management platform that improves coordination, communication, and healthcare services among local hospitals..  

---
## Features

### Account System
- User registration and secure login for Patients, General Practitioners (GPs), and Specialists.
- Password recovery and account authentication for secure access to medical records.

### Dashboards
- Patients can view their diagnoses, referrals, and personal health records.
- GPs and Specialists can manage, search, sort, and summarize patient information and referrals.

### CVD Calculator
- Utilizing the PREVENT Equations from the American Heart Association (AHA), the program can compute Cardiovascular Disease (CVD) Risk and other health metrics.
- Estimates CVD risk based on patient health data.

### CRUD System
- GPs/Specialists can create, read, update, and delete patient medical records.
- Enables organized storage and retrieval of patient referrals and health information.

### Test Program
- Automatically loads test inputs to validate system functionalities.
- Simulates different user interactions and scenarios for system testing.

---

## Installation

**1. Clone the repository:**

```bash
git clone https://github.com/whyjay-0/CCPROG2-MP.git
cd CCPROG2-MP
```

### Compiling and Running the Application

```bash
cd src
gcc -Wall -std=c99 main.c user_management.c patient_logging.c referral_management.c -o ../RMS
```
Run RMS.exe from Terminal/Command Prompt at directory ``./CCPROG2-MP``

---
## Running the test program
Edit the main function inside test_cases.c to pick which functions will be tested.
```bash
cd src
gcc -Wall -std=c99 test_cases.c user_management.c patient_logging.c referral_management.c -o ../test
```
Run test.exe from Terminal/Command Prompt at directory ``./CCPROG2-MP``

---  
  
## Project Structure

```
.
├── RMS.exe                      # Main executable/program
├── README.md                    # Details about the program
├── src                          # Dir where main source code is found
│   ├── Letran_Navarro...h          # Contains structures and function prototypes
│   ├── main.c                      # Main loop of the program
│   ├── patient_logging.c           # Functions related to patients
│   ├── referral_management.c       # Functions related to referrals
│   ├── user_management.c           # Functions related to users/design
│   └── test_cases.c                # Test functions for all functions
│
└── data                         # Dir where user/patient details are stored
    ├── p.txt                       # Test output for patients are stored here
    ├── patients.txt                # Actual data for patients
    ├── r.txt                       # Test output for referrals are stored here
    ├── referrals.txt               # Actual data for referrals
    └── users.txt                   # Actual data for users (username, password, etc.)
```