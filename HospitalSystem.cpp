#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#include "HospitalSystem.h"
#include "HospitalException.h"
#include "helper.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

HospitalSystem::HospitalSystem() {}

void HospitalSystem::loadAllData() { 
  
    try { FileHandler::loadPatients(patients); }
    catch (FileNotFoundException& e) { cout <<"Failed: patients.txt" << endl; }

    try { FileHandler::loadDoctors(doctors); }
    catch (FileNotFoundException& e) { cout <<"Failed: doctors.txt" << endl; }

    try { FileHandler::loadAdmin(admin); }
    catch (FileNotFoundException& e) { cout <<"Failed: admin.txt" << endl; }

    try { FileHandler::loadAppointments(appointments); }
    catch (FileNotFoundException& e) { cout <<"Failed: appointments.txt" << endl; }

    try { FileHandler::loadBills(bills); }
    catch (FileNotFoundException& e) { cout <<"Failed: bills.txt" << endl; }

    try { FileHandler::loadPrescriptions(prescriptions); }
    catch (FileNotFoundException& e) { cout <<"Failed: prescriptions.txt" << endl; }
}

void HospitalSystem::getToday(char* dateStr) {
    time_t now = time(0);
    strftime(dateStr, 11, "%d-%m-%Y", localtime(&now));
}

// Case insensitive comparison implemented manually using tolower
void HospitalSystem::caseInsensitiveCompare(const char* str1, const char* str2, bool& result) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        char c1 = str1[i];
        char c2 = str2[i];
        if (c1 >= 'A' && c1 <= 'Z') c1 = c1 - 'A' + 'a';
        if (c2 >= 'A' && c2 <= 'Z') c2 = c2 - 'A' + 'a';
        if (c1 != c2) { result = false; return; }
        i++;
    }
    result = (str1[i] == '\0' && str2[i] == '\0');
}

// Bubble sort appointments by date ascending or descending
void HospitalSystem::sortAppointmentsByDate(Appointment* arr, int n, bool ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compare dates as strings (DD-MM-YYYY)
            // Convert to YYYY-MM-DD for easy comparison
            const char* d1 = arr[j].getDate();
            const char* d2 = arr[j + 1].getDate();
            // Build comparable strings: YYYYMMDD
            char c1[9], c2[9];
            c1[0] = d1[6]; c1[1] = d1[7]; c1[2] = d1[8]; c1[3] = d1[9];
            c1[4] = d1[3]; c1[5] = d1[4];
            c1[6] = d1[0]; c1[7] = d1[1]; c1[8] = '\0';

            c2[0] = d2[6]; c2[1] = d2[7]; c2[2] = d2[8]; c2[3] = d2[9];
            c2[4] = d2[3]; c2[5] = d2[4];
            c2[6] = d2[0]; c2[7] = d2[1]; c2[8] = '\0';

            bool shouldSwap = false;
            for (int k = 0; k < 8; k++) {
                if (c1[k] > c2[k]) { shouldSwap = ascending; break; }
                if (c1[k] < c2[k]) { shouldSwap = !ascending; break; }
            }
            if (shouldSwap) {
                Appointment temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void HospitalSystem::sortPrescriptionsByDate(Prescription* arr, int n, bool ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            const char* d1 = arr[j].getDate();
            const char* d2 = arr[j + 1].getDate();
            char c1[9], c2[9];
            c1[0] = d1[6]; c1[1] = d1[7]; c1[2] = d1[8]; c1[3] = d1[9];
            c1[4] = d1[3]; c1[5] = d1[4];
            c1[6] = d1[0]; c1[7] = d1[1]; c1[8] = '\0';

            c2[0] = d2[6]; c2[1] = d2[7]; c2[2] = d2[8]; c2[3] = d2[9];
            c2[4] = d2[3]; c2[5] = d2[4];
            c2[6] = d2[0]; c2[7] = d2[1]; c2[8] = '\0';

            bool shouldSwap = false;
            for (int k = 0; k < 8; k++) {
                if (c1[k] > c2[k]) { shouldSwap = ascending; break; }
                if (c1[k] < c2[k]) { shouldSwap = !ascending; break; }
            }
            if (shouldSwap) {
                Prescription temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void HospitalSystem::run() {
    loadAllData();
    showMainMenu();
}

void HospitalSystem::showMainMenu() {
    int choice;
    do {
        cout << "\nWelcome to MediCore Hospital Management System" << endl;
        cout << "===============================================" << endl;
        cout << "Login as:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Admin" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (!Validator::validateMenuChoice(choice, 1, 4)) {
            cout << "Invalid choice." << endl;
            continue;
        }

        if (choice == 1) {
            Patient* p = loginPatient();
            if (p) patientMenu(p);
        }
        else if (choice == 2) {
            Doctor* d = loginDoctor();
            if (d) doctorMenu(d);
        }
        else if (choice == 3) {
            if (loginAdmin()) adminMenu();
        }

    } while (choice != 4);

    cout << "Goodbye!" << endl;
}

Patient* HospitalSystem::loginPatient() {
    int attempts = 0;
    char idStr[10], password[50];

    while (attempts < 3) {
        cout << "Enter Patient ID: ";
        cin >> idStr;
        cout << "Enter Password: ";
        cin >> password;

        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
            id = id * 10 + (idStr[i] - '0');

        Patient* p = patients.findById(id);
        if (p && compareString(p->getPassword(), password)) {
            return p;
        }

        attempts++;
        cout << "Invalid credentials. Attempts left: " << (3 - attempts) << endl;
        FileHandler::appendSecurityLog("Patient", idStr, "FAILED");

        if (attempts == 3) {
            cout << "Account locked. Contact admin." << endl;
            FileHandler::appendSecurityLog("Patient", idStr, "LOCKED");
            return nullptr;
        }
    }
    return nullptr;
}

Doctor* HospitalSystem::loginDoctor() {
    int attempts = 0;
    char idStr[10], password[50];

    while (attempts < 3) {
        cout << "Enter Doctor ID: ";
        cin >> idStr;
        cout << "Enter Password: ";
        cin >> password;

        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
            id = id * 10 + (idStr[i] - '0');

        Doctor* d = doctors.findById(id);
        if (d && compareString(d->getPassword(), password)) {
            return d;
        }

        attempts++;
        cout << "Invalid credentials. Attempts left: " << (3 - attempts) << endl;
        FileHandler::appendSecurityLog("Doctor", idStr, "FAILED");

        if (attempts == 3) {
            cout << "Account locked. Contact admin." << endl;
            FileHandler::appendSecurityLog("Doctor", idStr, "LOCKED");
            return nullptr;
        }
    }
    return nullptr;
}

bool HospitalSystem::loginAdmin() {
    int attempts = 0;
    char idStr[10], password[50];

    while (attempts < 3) {
        cout << "Enter Admin ID: ";
        cin >> idStr;
        cout << "Enter Password: ";
        cin >> password;

        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
            id = id * 10 + (idStr[i] - '0');

        if (admin.getId() == id && compareString(admin.getPassword(), password)) {
            return true;
        }

        attempts++;
        cout << "Invalid credentials. Attempts left: " << (3 - attempts) << endl;
        FileHandler::appendSecurityLog("Admin", idStr, "FAILED");

        if (attempts == 3) {
            cout << "Account locked. Contact admin." << endl;
            FileHandler::appendSecurityLog("Admin", idStr, "LOCKED");
            return false;
        }
    }
    return false;
}

void HospitalSystem::patientMenu(Patient* p) {
    int choice;
    do {
        p->displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (!Validator::validateMenuChoice(choice, 1, 8)) {
            cout << "Invalid choice." << endl;
            continue;
        }

        switch (choice) {
        case 1: bookAppointment(p); break;
        case 2: cancelAppointment(p); break;
        case 3: viewMyAppointments(p); break;
        case 4: viewMyMedicalRecords(p); break;
        case 5: viewMyBills(p); break;
        case 6: payBill(p); break;
        case 7: topUpBalance(p); break;
        case 8: cout << "Logged out." << endl; break;
        }
    } while (choice != 8);
}

void HospitalSystem::bookAppointment(Patient* p) {
    char spec[51];
    cout << "Enter specialization to search: ";
    cin.ignore();
    cin.getline(spec, 51);

    // Display doctors with matching specialization (case insensitive)
    bool found = false;
    for (int i = 0; i < doctors.size(); i++) {
        Doctor& d = doctors.getAll()[i];
        bool match = false;
        caseInsensitiveCompare(d.getSpecialization(), spec, match);
        if (match) {
            cout << "ID: " << d.getId() << " | Name: " << d.getName()
                << " | Fee: PKR " << d.getfee() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No doctors available for that specialization." << endl;
        return;
    }

    // Get doctor ID
    int docId;
    cout << "Enter Doctor ID: ";
    cin >> docId;
    Doctor* doc = doctors.findById(docId);
    if (!doc) { cout << "Doctor not found." << endl; return; }

    // Get date with validation
    char date[11];
    int dateAttempts = 0;
    while (dateAttempts < 3) {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;
        if (Validator::validateDate(date)) break;
        cout << "Invalid date. Use format DD-MM-YYYY." << endl;
        dateAttempts++;
        if (dateAttempts == 3) return;
    }

    // Show available slots
    const char* allSlots[] = { "09:00","10:00","11:00","12:00",
                               "13:00","14:00","15:00","16:00" };
    cout << "Available slots:" << endl;
    for (int s = 0; s < 8; s++) {
        bool taken = false;
        for (int i = 0; i < appointments.size(); i++) {
            Appointment& a = appointments.getAll()[i];
            if (a.getDoctorId() == docId &&
                compareString(a.getDate(), date) &&
                compareString(a.getTimeSlot(), allSlots[s]) &&
                !compareString(a.getStatus(), "cancelled")) {
                taken = true;
                break;
            }
        }
        if (!taken) cout << allSlots[s] << endl;
    }

    // Get time slot
    char slot[6];
    bool slotBooked = false;
    do {
        slotBooked = false;
        cout << "Enter time slot: ";
        cin >> slot;

        if (!Validator::validateTimeSlot(slot)) {
            cout << "Invalid time slot." << endl;
            continue;
        }

        try {
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getAll()[i];
                if (a.getDoctorId() == docId &&
                    compareString(a.getDate(), date) &&
                    compareString(a.getTimeSlot(), slot) &&
                    !compareString(a.getStatus(), "cancelled")) {
                    throw SlotUnavailableException();
                }
            }
        }
        catch (SlotUnavailableException& e) {
            cout << e.what() << endl;
            slotBooked = true;
        }
    } while (slotBooked);

    // Check balance
    try {
        if (p->getBalance() < doc->getfee())
            throw InsufficientFundsException();
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
        return;
    }

    // Generate appointment ID
    int newApptId = 1;
    for (int i = 0; i < appointments.size(); i++)
        if (appointments.getAll()[i].getId() >= newApptId)
            newApptId = appointments.getAll()[i].getId() + 1;

    // Create and save appointment
    Appointment newAppt(newApptId, p->getId(), docId, date, slot, "pending");
    appointments.add(newAppt);
    FileHandler::appendAppointment(newAppt);

    // Deduct balance
    *p -= doc->getfee();
    FileHandler::updatePatient(*p);

    // Generate bill ID
    int newBillId = 1;
    for (int i = 0; i < bills.size(); i++)
        if (bills.getAll()[i].getId() >= newBillId)
            newBillId = bills.getAll()[i].getId() + 1;

    // Create and save bill
    Bill newBill(newBillId, p->getId(), newApptId, doc->getfee(), "unpaid", date);
    bills.add(newBill);
    FileHandler::appendBill(newBill);

    cout << "Appointment booked successfully. Appointment ID: " << newApptId << endl;
}

void HospitalSystem::cancelAppointment(Patient* p) {
    // Show pending appointments
    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == p->getId() && compareString(a.getStatus(), "pending")) {
            Doctor* doc = doctors.findById(a.getDoctorId());
            cout << a.getId() << " | "
                << (doc ? doc->getName() : "Unknown") << " | "
                << a.getDate() << " | " << a.getTimeSlot() << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "You have no pending appointments." << endl;
        return;
    }

    int apptId;
    cout << "Enter Appointment ID to cancel: ";
    cin >> apptId;

    Appointment* appt = appointments.findById(apptId);
    if (!appt || appt->getPatientId() != p->getId() ||
        !compareString(appt->getStatus(), "pending")) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // Get doctor fee for refund
    Doctor* doc = doctors.findById(appt->getDoctorId());
    double fee = doc ? doc->getfee() : 0.0;

    // Update appointment status
    appt->setStatus("cancelled");
    FileHandler::updateAppointment(*appt);

    // Refund balance
    *p += fee;
    FileHandler::updatePatient(*p);

    // Update bill status
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getAppointmentId() == apptId) {
            b.setStatus("cancelled");
            FileHandler::updateBill(b);
            break;
        }
    }

    cout << "Appointment cancelled. PKR " << fee << " refunded to your balance." << endl;
}

void HospitalSystem::viewMyAppointments(Patient* p) {
    // Collect patient's appointments
    Appointment temp[100];
    int count = 0;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == p->getId())
            temp[count++] = a;
    }

    if (count == 0) { cout << "No appointments found." << endl; return; }

    sortAppointmentsByDate(temp, count, true);

    for (int i = 0; i < count; i++) {
        Doctor* doc = doctors.findById(temp[i].getDoctorId());
        cout << temp[i].getId() << " | "
            << (doc ? doc->getName() : "Unknown") << " | "
            << (doc ? doc->getSpecialization() : "Unknown") << " | "
            << temp[i].getDate() << " | "
            << temp[i].getTimeSlot() << " | "
            << temp[i].getStatus() << endl;
    }
}

void HospitalSystem::viewMyMedicalRecords(Patient* p) {
    Prescription temp[100];
    int count = 0;
    for (int i = 0; i < prescriptions.size(); i++) {
        Prescription& pr = prescriptions.getAll()[i];
        if (pr.getPatientId() == p->getId())
            temp[count++] = pr;
    }

    if (count == 0) { cout << "No medical records found." << endl; return; }

    sortPrescriptionsByDate(temp, count, false); // descending

    for (int i = 0; i < count; i++) {
        Doctor* doc = doctors.findById(temp[i].getDoctorId());
        cout << temp[i].getDate() << " | "
            << (doc ? doc->getName() : "Unknown") << " | "
            << temp[i].getMedicines() << " | "
            << temp[i].getNotes() << endl;
    }
}

void HospitalSystem::viewMyBills(Patient* p) {
    bool found = false;
    double totalUnpaid = 0.0;

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == p->getId()) {
            cout << b.getId() << " | " << b.getAppointmentId()
                << " | PKR " << b.getAmount() << " | "
                << b.getStatus() << " | " << b.getDate() << endl;
            if (compareString(b.getStatus(), "unpaid"))
                totalUnpaid += b.getAmount();
            found = true;
        }
    }

    if (!found) { cout << "No bills found." << endl; return; }
    cout << "Total outstanding: PKR " << totalUnpaid << endl;
}

void HospitalSystem::payBill(Patient* p) {
    bool hasUnpaid = false;
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == p->getId() && compareString(b.getStatus(), "unpaid")) {
            cout << b.getId() << " | PKR " << b.getAmount() << " | " << b.getDate() << endl;
            hasUnpaid = true;
        }
    }

    if (!hasUnpaid) { cout << "No unpaid bills." << endl; return; }

    int billId;
    cout << "Enter Bill ID to pay: ";
    cin >> billId;

    Bill* bill = bills.findById(billId);
    if (!bill || bill->getPatientId() != p->getId() ||
        !compareString(bill->getStatus(), "unpaid")) {
        cout << "Invalid bill ID." << endl;
        return;
    }

    try {
        if (p->getBalance() < bill->getAmount())
            throw InsufficientFundsException();
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
        return;
    }

    *p -= bill->getAmount();
    bill->setStatus("paid");
    FileHandler::updateBill(*bill);
    FileHandler::updatePatient(*p);

    cout << "Bill paid successfully. Remaining balance: PKR " << p->getBalance() << endl;
}

void HospitalSystem::topUpBalance(Patient* p) {
    int attempts = 0;
    while (attempts < 3) {
        double amount;
        cout << "Enter amount to add (PKR): ";
        cin >> amount;

        try {
            if (!Validator::validatePositiveFloat(amount))
                throw InvalidInputException();

            *p += amount;
            FileHandler::updatePatient(*p);
            cout << "Balance updated. New balance: PKR " << p->getBalance() << endl;
            return;

        }
        catch (InvalidInputException& e) {
            cout << e.what() << endl;
            attempts++;
        }
    }
}

void HospitalSystem::doctorMenu(Doctor* d) {
    int choice;
    do {
        d->displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (!Validator::validateMenuChoice(choice, 1, 6)) {
            cout << "Invalid choice." << endl;
            continue;
        }

        switch (choice) {
        case 1: viewTodaysAppointments(d); break;
        case 2: markAppointmentComplete(d); break;
        case 3: markAppointmentNoShow(d); break;
        case 4: writePrescription(d); break;
        case 5: viewPatientMedicalHistory(d); break;
        case 6: cout << "Logged out." << endl; break;
        }
    } while (choice != 6);
}

void HospitalSystem::viewTodaysAppointments(Doctor* d) {
    char today[11];
    getToday(today);

    Appointment temp[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == d->getId() && compareString(a.getDate(), today))
            temp[count++] = a;
    }

    if (count == 0) { cout << "No appointments scheduled for today." << endl; return; }

    sortAppointmentsByDate(temp, count, true);

    for (int i = 0; i < count; i++) {
        Patient* pat = patients.findById(temp[i].getPatientId());
        cout << temp[i].getId() << " | "
            << (pat ? pat->getName() : "Unknown") << " | "
            << temp[i].getTimeSlot() << " | "
            << temp[i].getStatus() << endl;
    }
}

void HospitalSystem::markAppointmentComplete(Doctor* d) {
    char today[11];
    getToday(today);

    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == d->getId() &&
            compareString(a.getDate(), today) &&
            compareString(a.getStatus(), "pending")) {
            Patient* pat = patients.findById(a.getPatientId());
            cout << a.getId() << " | "
                << (pat ? pat->getName() : "Unknown") << " | "
                << a.getTimeSlot() << endl;
            hasPending = true;
        }
    }

    if (!hasPending) { cout << "No pending appointments today." << endl; return; }

    int apptId;
    cout << "Enter Appointment ID: ";
    cin >> apptId;

    Appointment* appt = appointments.findById(apptId);
    if (!appt || appt->getDoctorId() != d->getId() ||
        !compareString(appt->getStatus(), "pending") ||
        !compareString(appt->getDate(), today)) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    appt->setStatus("completed");
    FileHandler::updateAppointment(*appt);
    cout << "Appointment marked as completed." << endl;
}

void HospitalSystem::markAppointmentNoShow(Doctor* d) {
    char today[11];
    getToday(today);

    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == d->getId() &&
            compareString(a.getDate(), today) &&
            compareString(a.getStatus(), "pending")) {
            Patient* pat = patients.findById(a.getPatientId());
            cout << a.getId() << " | "
                << (pat ? pat->getName() : "Unknown") << " | "
                << a.getTimeSlot() << endl;
            hasPending = true;
        }
    }

    if (!hasPending) { cout << "No pending appointments today." << endl; return; }

    int apptId;
    cout << "Enter Appointment ID: ";
    cin >> apptId;

    Appointment* appt = appointments.findById(apptId);
    if (!appt || appt->getDoctorId() != d->getId() ||
        !compareString(appt->getStatus(), "pending") ||
        !compareString(appt->getDate(), today)) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    appt->setStatus("no-show");
    FileHandler::updateAppointment(*appt);

    // Cancel bill, no refund
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getAppointmentId() == apptId) {
            b.setStatus("cancelled");
            FileHandler::updateBill(b);
            break;
        }
    }

    cout << "Appointment marked as no-show." << endl;
}

void HospitalSystem::writePrescription(Doctor* d) {
    int apptId;
    cout << "Enter Appointment ID: ";
    cin >> apptId;

    Appointment* appt = appointments.findById(apptId);
    if (!appt || appt->getDoctorId() != d->getId() ||
        !compareString(appt->getStatus(), "completed")) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // Check if prescription already exists
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getAppointmentId() == apptId) {
            cout << "Prescription already written for this appointment." << endl;
            return;
        }
    }

    char medicines[500], notes[300];
    cout << "Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    cin.ignore();
    cin.getline(medicines, 500);

    cout << "Enter notes (max 300 chars): ";
    cin.getline(notes, 300);

    // Generate prescription ID
    int newPresId = 1;
    for (int i = 0; i < prescriptions.size(); i++)
        if (prescriptions.getAll()[i].getId() >= newPresId)
            newPresId = prescriptions.getAll()[i].getId() + 1;

    Prescription pr(newPresId, apptId, appt->getPatientId(),
        d->getId(), appt->getDate(), medicines, notes);
    prescriptions.add(pr);
    FileHandler::appendPrescription(pr);

    cout << "Prescription saved." << endl;
}

void HospitalSystem::viewPatientMedicalHistory(Doctor* d) {
    int patId;
    cout << "Enter Patient ID: ";
    cin >> patId;

    Patient* pat = patients.findById(patId);
    if (!pat) { cout << "Access denied. You can only view records of your own patients." << endl; return; }

    // Check if patient has at least one completed appointment with this doctor
    bool hasCompleted = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == patId && a.getDoctorId() == d->getId() &&
            compareString(a.getStatus(), "completed")) {
            hasCompleted = true;
            break;
        }
    }

    if (!hasCompleted) {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    Prescription temp[100];
    int count = 0;
    for (int i = 0; i < prescriptions.size(); i++) {
        Prescription& pr = prescriptions.getAll()[i];
        if (pr.getPatientId() == patId && pr.getDoctorId() == d->getId())
            temp[count++] = pr;
    }

    if (count == 0) { cout << "No medical records found." << endl; return; }

    sortPrescriptionsByDate(temp, count, false);

    for (int i = 0; i < count; i++) {
        cout << temp[i].getDate() << " | "
            << temp[i].getMedicines() << " | "
            << temp[i].getNotes() << endl;
    }
}

void HospitalSystem::adminMenu() {
    int choice;
    do {
        admin.displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (!Validator::validateMenuChoice(choice, 1, 12)) {
            cout << "Invalid choice." << endl;
            continue;
        }

        switch (choice) {
        case 1: addDoctor(); break;
        case 2: removeDoctor(); break;
        case 3: addPatient(); break;
        case 4: removePatient(); break;
        case 5: viewAllPatients(); break;
        case 6: viewAllDoctors(); break;
        case 7: viewAllAppointments(); break;
        case 8: viewUnpaidBills(); break;
        case 9: dischargePatient(); break;
        case 10: viewSecurityLog(); break;
        case 11: generateDailyReport(); break;
        case 12: cout << "Logged out." << endl; break;
        }
    } while (choice != 12);
}

void HospitalSystem::addDoctor() {
    char name[51], spec[51], contact[12], password[51];
    double fee;

    cout << "Enter name: "; cin.ignore(); cin.getline(name, 51);
    cout << "Enter specialization: "; cin.getline(spec, 51);

    do {
        cout << "Enter contact (11 digits): "; cin >> contact;
    } while (!Validator::validateContact(contact));

    do {
        cout << "Enter password (min 6 chars): "; cin >> password;
    } while (!Validator::validatePassword(password));

    do {
        cout << "Enter consultation fee: "; cin >> fee;
    } while (!Validator::validatePositiveFloat(fee));

    int newId = 1;
    for (int i = 0; i < doctors.size(); i++)
        if (doctors.getAll()[i].getId() >= newId)
            newId = doctors.getAll()[i].getId() + 1;

    Doctor d(newId, name, spec, contact, password, fee);
    doctors.add(d);
    FileHandler::appendDoctor(d);
    cout << "Doctor added successfully. ID: " << newId << endl;
}

void HospitalSystem::removeDoctor() {
    viewAllDoctors();
    int docId;
    cout << "Enter Doctor ID to remove: ";
    cin >> docId;

    // Check for pending appointments
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == docId && compareString(a.getStatus(), "pending")) {
            cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first." << endl;
            return;
        }
    }

    doctors.removeById(docId);
    FileHandler::deleteDoctor(docId);
    cout << "Doctor removed." << endl;
}

void HospitalSystem::addPatient() {
    char name[51], contact[12], password[51];
    char genderStr[3];
    int age;
    double balance;

    cout << "Enter name: "; cin.ignore(); cin.getline(name, 51);

    do {
        cout << "Enter age: "; cin >> age;
    } while (!Validator::validateAge(age));

    do {
        cout << "Enter gender (M/F): "; cin >> genderStr;
    } while (!Validator::validateGender(genderStr[0]));

    do {
        cout << "Enter contact (11 digits): "; cin >> contact;
    } while (!Validator::validateContact(contact));

    do {
        cout << "Enter password (min 6 chars): "; cin >> password;
    } while (!Validator::validatePassword(password));

    do {
        cout << "Enter initial balance: "; cin >> balance;
    } while (!Validator::validatePositiveFloat(balance));

    int newId = 1;
    for (int i = 0; i < patients.size(); i++)
        if (patients.getAll()[i].getId() >= newId)
            newId = patients.getAll()[i].getId() + 1;

    Patient p(newId, name, age, genderStr[0], contact, password, balance);
    patients.add(p);
    FileHandler::appendPatient(p);
    cout << "Patient added successfully. ID: " << newId << endl;
}

void HospitalSystem::removePatient() {
    viewAllPatients();
    int patId;
    cout << "Enter Patient ID to remove: ";
    cin >> patId;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == patId && compareString(a.getStatus(), "pending")) {
            cout << "Cannot remove patient with pending appointments." << endl;
            return;
        }
    }

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == patId && compareString(b.getStatus(), "unpaid")) {
            cout << "Cannot remove patient with unpaid bills." << endl;
            return;
        }
    }

    patients.removeById(patId);
    FileHandler::deletePatient(patId);
    FileHandler::cascadeDeletePatient(patId);
    cout << "Patient removed successfully." << endl;
}

void HospitalSystem::viewAllPatients() {
    for (int i = 0; i < patients.size(); i++) {
        Patient& p = patients.getAll()[i];
        int unpaidCount = 0;
        for (int j = 0; j < bills.size(); j++) {
            if (bills.getAll()[j].getPatientId() == p.getId() &&
                compareString(bills.getAll()[j].getStatus(), "unpaid"))
                unpaidCount++;
        }
        cout << p.getId() << " | " << p.getName() << " | " << p.getAge()
            << " | " << p.getGender() << " | " << p.getContact()
            << " | PKR " << p.getBalance() << " | Unpaid Bills: " << unpaidCount << endl;
    }
}

void HospitalSystem::viewAllDoctors() {
    for (int i = 0; i < doctors.size(); i++) {
        Doctor& d = doctors.getAll()[i];
        cout << d.getId() << " | " << d.getName() << " | "
            << d.getSpecialization() << " | " << d.getContact()
            << " | PKR " << d.getfee() << endl;
    }
}

void HospitalSystem::viewAllAppointments() {
    Appointment temp[100];
    int count = appointments.size();
    for (int i = 0; i < count; i++)
        temp[i] = appointments.getAll()[i];

    sortAppointmentsByDate(temp, count, false); // descending

    for (int i = 0; i < count; i++) {
        Patient* pat = patients.findById(temp[i].getPatientId());
        Doctor* doc = doctors.findById(temp[i].getDoctorId());
        cout << temp[i].getId() << " | "
            << (pat ? pat->getName() : "Unknown") << " | "
            << (doc ? doc->getName() : "Unknown") << " | "
            << temp[i].getDate() << " | "
            << temp[i].getTimeSlot() << " | "
            << temp[i].getStatus() << endl;
    }
}

void HospitalSystem::viewUnpaidBills() {
    char today[11];
    getToday(today);

    // Convert today to integer for comparison
    int ty = (today[6] - '0') * 1000 + (today[7] - '0') * 100 + (today[8] - '0') * 10 + (today[9] - '0');
    int tm = (today[3] - '0') * 10 + (today[4] - '0');
    int td = (today[0] - '0') * 10 + (today[1] - '0');

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (!compareString(b.getStatus(), "unpaid")) continue;

        Patient* pat = patients.findById(b.getPatientId());
        const char* d = b.getDate();
        int by = (d[6] - '0') * 1000 + (d[7] - '0') * 100 + (d[8] - '0') * 10 + (d[9] - '0');
        int bm = (d[3] - '0') * 10 + (d[4] - '0');
        int bd = (d[0] - '0') * 10 + (d[1] - '0');

        // Calculate days difference roughly
        int totalToday = ty * 365 + tm * 30 + td;
        int totalBill = by * 365 + bm * 30 + bd;
        bool overdue = (totalToday - totalBill) > 7;

        cout << b.getId() << " | "
            << (pat ? pat->getName() : "Unknown") << " | PKR "
            << b.getAmount() << " | " << b.getDate();
        if (overdue) cout << " [OVERDUE]";
        cout << endl;
    }
}

void HospitalSystem::dischargePatient() {
    int patId;
    cout << "Enter Patient ID: ";
    cin >> patId;

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == patId && compareString(b.getStatus(), "unpaid")) {
            cout << "Cannot discharge patient with unpaid bills." << endl;
            return;
        }
    }

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == patId && compareString(a.getStatus(), "pending")) {
            cout << "Cannot discharge patient with pending appointments." << endl;
            return;
        }
    }

    FileHandler::archivePatient(patId, patients, appointments, bills, prescriptions);

    patients.removeById(patId);
    FileHandler::deletePatient(patId);
    FileHandler::cascadeDeletePatient(patId);

    cout << "Patient discharged and archived successfully." << endl;
}

void HospitalSystem::viewSecurityLog() {
    ifstream file("security_log.txt");
    if (!file.is_open()) { cout << "No security events logged." << endl; return; }

    char line[300];
    bool empty = true;
    while (file.getline(line, 300)) {
        if (line[0] != '\0') { cout << line << endl; empty = false; }
    }
    if (empty) cout << "No security events logged." << endl;
    file.close();
}

void HospitalSystem::generateDailyReport() {
    char today[11];
    getToday(today);

    int pending = 0, completed = 0, noshow = 0, cancelled = 0;
    double revenue = 0.0;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (!compareString(a.getDate(), today)) continue;
        if (compareString(a.getStatus(), "pending"))   pending++;
        if (compareString(a.getStatus(), "completed")) completed++;
        if (compareString(a.getStatus(), "no-show"))   noshow++;
        if (compareString(a.getStatus(), "cancelled")) cancelled++;
    }

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (compareString(b.getStatus(), "paid") && compareString(b.getDate(), today))
            revenue += b.getAmount();
    }

    cout << "\n--- Daily Report: " << today << " ---" << endl;
    cout << "Total appointments today: " << (pending + completed + noshow + cancelled)
        << " (Pending: " << pending << " Completed: " << completed
        << " No-show: " << noshow << " Cancelled: " << cancelled << ")" << endl;
    cout << "Revenue collected today: PKR " << revenue << endl;

    cout << "\nPatients with outstanding unpaid bills:" << endl;
    for (int i = 0; i < patients.size(); i++) {
        Patient& p = patients.getAll()[i];
        double owed = 0.0;
        for (int j = 0; j < bills.size(); j++) {
            Bill& b = bills.getAll()[j];
            if (b.getPatientId() == p.getId() && compareString(b.getStatus(), "unpaid"))
                owed += b.getAmount();
        }
        if (owed > 0)
            cout << p.getName() << " | PKR " << owed << endl;
    }

    cout << "\nDoctor-wise summary today:" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        Doctor& d = doctors.getAll()[i];
        int dc = 0, dp = 0, dn = 0;
        for (int j = 0; j < appointments.size(); j++) {
            Appointment& a = appointments.getAll()[j];
            if (a.getDoctorId() != d.getId() || !compareString(a.getDate(), today)) continue;
            if (compareString(a.getStatus(), "completed")) dc++;
            if (compareString(a.getStatus(), "pending"))   dp++;
            if (compareString(a.getStatus(), "no-show"))   dn++;
        }
        if (dc + dp + dn > 0)
            cout << d.getName() << " | Completed: " << dc
            << " | Pending: " << dp << " | No-show: " << dn << endl;
    }
}

