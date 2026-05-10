#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#include "helper.h"
#include "HospitalException.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

//for updating 
void FileHandler::loadPatients(Storage<Patient>& storage) {
    ifstream file("patients.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("patients.txt");
    }

    char line[300];
    while (file.getline(line, 300)) {
        if (line[0] == '\0') {
            continue;
        }

        int pos = 0;
        char idStr[10], name[50], ageStr[5], genderStr[3],contact[12], password[50], balanceStr[20];

        parseField(line, pos, idStr);
        parseField(line, pos, name);
        parseField(line, pos, ageStr);
        parseField(line, pos, genderStr);
        parseField(line, pos, contact);
        parseField(line, pos, password);
        parseField(line, pos, balanceStr);

        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++) {
            id = id * 10 + (idStr[i] - '0');
        }

        int age = 0;
        for (int i = 0; ageStr[i] != '\0'; i++) {
            age = age * 10 + (ageStr[i] - '0');
        }

        double balance = 0.0;
        int dotPos = -1;
        for (int i = 0; balanceStr[i] != '\0'; i++) {
            if (balanceStr[i] == '.') { dotPos = i; break; }
        }

        if (dotPos == -1) {
            for (int i = 0; balanceStr[i] != '\0'; i++) {
                balance = balance * 10 + (balanceStr[i] - '0');
            }
        }
        else {
            for (int i = 0; i < dotPos; i++) {
                balance = balance * 10 + (balanceStr[i] - '0');
            }

            double frac = 0.1;
            for (int i = dotPos + 1; balanceStr[i] != '\0'; i++) {
                balance += (balanceStr[i] - '0') * frac;
                frac *= 0.1;
            }
        }

        Patient p(id, name, age, genderStr[0], contact, password, balance);
        storage.add(p);
    }

    file.close();
}

void FileHandler::loadDoctors(Storage<Doctor>& storage) {
    ifstream file("doctors.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("doctors.txt");
    }

    char line[300];
    while (file.getline(line, 300)) {
        if (line[0] == '\0') {
            continue;
        }

        int pos = 0;
        char idStr[10], name[50], spec[51], contact[12], password[50], feeStr[20];

        parseField(line, pos, idStr);
        parseField(line, pos, name);
        parseField(line, pos, spec);
        parseField(line, pos, contact);
        parseField(line, pos, password);
        parseField(line, pos, feeStr);

        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++) {
            id = id * 10 + (idStr[i] - '0');
        }

        double fee = 0.0;
        int dotPos = -1;
        for (int i = 0; feeStr[i] != '\0'; i++)
            if (feeStr[i] == '.') { dotPos = i; break; }

        if (dotPos == -1) {
            for (int i = 0; feeStr[i] != '\0'; i++) {
                fee = fee * 10 + (feeStr[i] - '0');
            }
        }
        else {
            for (int i = 0; i < dotPos; i++) {
                fee = fee * 10 + (feeStr[i] - '0');
            }
            double frac = 0.1;
            for (int i = dotPos + 1; feeStr[i] != '\0'; i++) {
                fee += (feeStr[i] - '0') * frac;
                frac *= 0.1;
            }
        }

        Doctor d(id, name, spec, contact, password, fee);
        storage.add(d);
    }
    file.close();
}

void FileHandler::loadAdmin(Admin& admin) {
    ifstream file("admin.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("admin.txt");
    }

    char line[200];
    file.getline(line, 200);
    file.close();

    if (line[0] == '\0') {
        return;
    }

    int pos = 0;
    char idStr[10], name[50], password[50];
    parseField(line, pos, idStr);
    parseField(line, pos, name);
    parseField(line, pos, password);

    int id = 0;
    for (int i = 0; idStr[i] != '\0'; i++) {
        id = id * 10 + (idStr[i] - '0');
    }

    admin = Admin(id, name, password);
}

void FileHandler::loadAppointments(Storage<Appointment>& storage) {
    ifstream file("appointments.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("appointments.txt");
    }

    char line[200];
    while (file.getline(line, 200)) {
        if (line[0] == '\0') {
            continue;
        }

        int pos = 0;
        char apIdStr[10], patIdStr[10], docIdStr[10], date[12], timeSlot[6], status[20];

        parseField(line, pos, apIdStr);
        parseField(line, pos, patIdStr);
        parseField(line, pos, docIdStr);
        parseField(line, pos, date);
        parseField(line, pos, timeSlot);
        parseField(line, pos, status);

        int apId = 0, patId = 0, docId = 0;
        for (int i = 0; apIdStr[i] != '\0'; i++) {
            apId = apId * 10 + (apIdStr[i] - '0');
        }
        for (int i = 0; patIdStr[i] != '\0'; i++) {
            patId = patId * 10 + (patIdStr[i] - '0');
        }
        for (int i = 0; docIdStr[i] != '\0'; i++) {
            docId = docId * 10 + (docIdStr[i] - '0');
        }   

        Appointment a(apId, patId, docId, date, timeSlot, status);
        storage.add(a);
    }  

    file.close();
}

void FileHandler::loadBills(Storage<Bill>& storage) {
    ifstream file("bills.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("bills.txt");
    }

    char line[200];
    while (file.getline(line, 200)) {
        if (line[0] == '\0') {
            continue;
        }

        int pos = 0;
        char billIdStr[10], patIdStr[10], apIdStr[10], amountStr[20], status[20], date[12];

        parseField(line, pos, billIdStr);
        parseField(line, pos, patIdStr);
        parseField(line, pos, apIdStr);
        parseField(line, pos, amountStr);
        parseField(line, pos, status);
        parseField(line, pos, date);

        int billId = 0, patId = 0, apId = 0;
        for (int i = 0; billIdStr[i] != '\0'; i++) {
            billId = billId * 10 + (billIdStr[i] - '0');
        }
        for (int i = 0; patIdStr[i] != '\0'; i++) {
            patId = patId * 10 + (patIdStr[i] - '0');
        }
        for (int i = 0; apIdStr[i] != '\0'; i++) {
            apId = apId * 10 + (apIdStr[i] - '0');
        }

        double amount = 0.0;
        int dotPos = -1;
        for (int i = 0; amountStr[i] != '\0'; i++)
            if (amountStr[i] == '.') { dotPos = i; break; }
        if (dotPos == -1) {
            for (int i = 0; amountStr[i] != '\0'; i++)
                amount = amount * 10 + (amountStr[i] - '0');
        }
        else {
            for (int i = 0; i < dotPos; i++)
                amount = amount * 10 + (amountStr[i] - '0');
            double frac = 0.1;
            for (int i = dotPos + 1; amountStr[i] != '\0'; i++) {
                amount += (amountStr[i] - '0') * frac;
                frac *= 0.1;
            }
        }

        Bill b(billId, patId, apId, amount, status, date);
        storage.add(b);
    }
    file.close();
}

void FileHandler::loadPrescriptions(Storage<Prescription>& storage) {
    ifstream file("prescriptions.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("prescriptions.txt");
    }

    char line[900];
    while (file.getline(line, 900)) {
        if (line[0] == '\0') {
            continue;
        }
        int pos = 0;
        char presIdStr[10], apIdStr[10], patIdStr[10], docIdStr[10], date[12], medicines[500], notes[300];

        parseField(line, pos, presIdStr);
        parseField(line, pos, apIdStr);
        parseField(line, pos, patIdStr);
        parseField(line, pos, docIdStr);
        parseField(line, pos, date);
        parseField(line, pos, medicines);
        parseField(line, pos, notes);

        int presId = 0, apId = 0, patId = 0, docId = 0;
        for (int i = 0; presIdStr[i] != '\0'; i++) {
            presId = presId * 10 + (presIdStr[i] - '0');
        }
        for (int i = 0; apIdStr[i] != '\0'; i++) {
            apId = apId * 10 + (apIdStr[i] - '0');
        }
        for (int i = 0; patIdStr[i] != '\0'; i++) {
            patId = patId * 10 + (patIdStr[i] - '0');
        }
        for (int i = 0; docIdStr[i] != '\0'; i++) {
            docId = docId * 10 + (docIdStr[i] - '0');
        }

        Prescription p(presId, apId, patId, docId, date, medicines, notes);
        storage.add(p);
    }

    file.close();
}

//for appending
void FileHandler::appendPatient(const Patient& p) {
    ofstream file("patients.txt", ios::app);

    if (!file.is_open()) {
        throw FileNotFoundException("patients.txt");
    }

    file << p.getId() << "," << p.getName() << "," << p.getAge() << ","
        << p.getGender() << "," << p.getContact() << ","
        << p.getPassword() << "," << p.getBalance() << endl;

    file.close();
}

void FileHandler::appendDoctor(const Doctor& d) {
    ofstream file("doctors.txt", ios::app);

    if (!file.is_open()) {
        throw FileNotFoundException("doctors.txt");
    }   

    file << d.getId() << "," << d.getName() << "," << d.getSpecialization()
        << "," << d.getContact() << "," << d.getPassword()
        << "," << d.getfee() << endl;

    file.close();
}

void FileHandler::appendAppointment(const Appointment& a) {
    ofstream file("appointments.txt", ios::app);

    if (!file.is_open()) {
        throw FileNotFoundException("appointments.txt");
    }

    file << a.getId() << "," << a.getPatientId() << "," << a.getDoctorId()
        << "," << a.getDate() << "," << a.getTimeSlot()
        << "," << a.getStatus() << endl;

    file.close();
}

void FileHandler::appendBill(const Bill& b) {
    ofstream file("bills.txt", ios::app);

    if (!file.is_open()) {
        throw FileNotFoundException("bills.txt");
    }   

    file << b.getId() << "," << b.getPatientId() << ","
        << b.getAppointmentId() << "," << b.getAmount()
        << "," << b.getStatus() << "," << b.getDate() <<endl;

    file.close();
}

void FileHandler::appendPrescription(const Prescription& p) {
    ofstream file("prescriptions.txt", ios::app);

    if (!file.is_open()) {
        throw FileNotFoundException("prescriptions.txt");
    }
    file << p.getId() << "," << p.getAppointmentId() << ","
        << p.getPatientId() << "," << p.getDoctorId()
        << "," << p.getDate() << "," << p.getMedicines()
        << "," << p.getNotes() << endl;

    file.close();
}


//for updating
void FileHandler::updatePatient(const Patient& p) {
    Storage<Patient> temp;
    loadPatients(temp);

    ofstream file("patients.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("patients.txt");
    }

    for (int i = 0; i < temp.size(); i++) {
        Patient& curr = temp.getAll()[i];
        if (curr.getId() == p.getId()) {
            curr = p;
        }

        file << curr.getId() << "," << curr.getName() << ","
            << curr.getAge() << "," << curr.getGender() << ","
            << curr.getContact() << "," << curr.getPassword()
            << "," << curr.getBalance() <<endl;
    }

    file.close();
}

void FileHandler::updateDoctor(const Doctor& d) {
    Storage<Doctor> temp;
    loadDoctors(temp);

    ofstream file("doctors.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("doctors.txt");
    }

    for (int i = 0; i < temp.size(); i++) {
        Doctor& curr = temp.getAll()[i];
        if (curr.getId() == d.getId()) {
            curr = d;
        }

        file << curr.getId() << "," << curr.getName() << ","
            << curr.getSpecialization() << "," << curr.getContact()
            << "," << curr.getPassword() << "," << curr.getfee() << endl;
    }

    file.close();
}

void FileHandler::updateAppointment(const Appointment& a) {
    Storage<Appointment> temp;
    loadAppointments(temp);

    ofstream file("appointments.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("appointments.txt");
    }

    for (int i = 0; i < temp.size(); i++) {
        Appointment& curr = temp.getAll()[i];
        if (curr.getId() == a.getId()) {
            curr = a;
        }

        file << curr.getId() << "," << curr.getPatientId() << ","
            << curr.getDoctorId() << "," << curr.getDate()
            << "," << curr.getTimeSlot() << "," << curr.getStatus() <<endl;
    }
    file.close();
}

void FileHandler::updateBill(const Bill& b) {
    Storage<Bill> temp;
    loadBills(temp);

    ofstream file("bills.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("bills.txt");
    }

    for (int i = 0; i < temp.size(); i++) {
        Bill& curr = temp.getAll()[i];
        if (curr.getId() == b.getId()) {
            curr = b;
        }

        file << curr.getId() << "," << curr.getPatientId() << ","
            << curr.getAppointmentId() << "," << curr.getAmount()
            << "," << curr.getStatus() << "," << curr.getDate() << endl;
    }

    file.close();
}


void FileHandler::deletePatient(int patientId) {
    Storage<Patient> temp;
    loadPatients(temp);

    ofstream file("patients.txt");
    for (int i = 0; i < temp.size(); i++) {
        Patient& curr = temp.getAll()[i];
        if (curr.getId() != patientId) {
            file << curr.getId() << "," << curr.getName() << ","
                << curr.getAge() << "," << curr.getGender() << ","
                << curr.getContact() << "," << curr.getPassword()
                << "," << curr.getBalance() << endl;
        }
    } 

    file.close();
}

void FileHandler::deleteDoctor(int doctorId) {
    Storage<Doctor> temp;
    loadDoctors(temp);

    ofstream file("doctors.txt");
    for (int i = 0; i < temp.size(); i++) {
        Doctor& curr = temp.getAll()[i];
        if (curr.getId() != doctorId) {
            file << curr.getId() << "," << curr.getName() << ","
                << curr.getSpecialization() << "," << curr.getContact()
                << "," << curr.getPassword() << "," << curr.getfee() << endl;
        }
    }

    file.close();
}

void FileHandler::cascadeDeletePatient(int patientId) {

    //deleting from appointments.txt
    Storage<Appointment> appts;
    loadAppointments(appts);

    ofstream apptFile("appointments.txt");
    for (int i = 0; i < appts.size(); i++) {
        Appointment& a = appts.getAll()[i];
        if (a.getPatientId() != patientId) {
            apptFile << a.getId() << "," << a.getPatientId() << ","
                << a.getDoctorId() << "," << a.getDate()
                << "," << a.getTimeSlot() << "," << a.getStatus() << endl;
        }
    } 

    apptFile.close();

    //deleting from bills.txt
    Storage<Bill> bills;
    loadBills(bills);

    ofstream billFile("bills.txt");
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() != patientId) {
            billFile << b.getId() << "," << b.getPatientId() << ","
                << b.getAppointmentId() << "," << b.getAmount()
                << "," << b.getStatus() << "," << b.getDate() << endl;
        }
    }

    billFile.close();

    //deleting from prescriptions.txt
    Storage<Prescription> pres;
    loadPrescriptions(pres);
    ofstream presFile("prescriptions.txt");

    for (int i = 0; i < pres.size(); i++) {
        Prescription& p = pres.getAll()[i];
        if (p.getPatientId() != patientId) {
            presFile << p.getId() << "," << p.getAppointmentId() << ","
                << p.getPatientId() << "," << p.getDoctorId()
                << "," << p.getDate() << "," << p.getMedicines()
                << "," << p.getNotes() << endl;
        }
    }

    presFile.close();
}


void FileHandler::appendSecurityLog(const char* role, const char* enteredId, const char* result) {
    ofstream file("security_log.txt", ios::app);
    if (!file.is_open()) return;


    time_t now = time(0);
    char timestamp[25];
    strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", localtime(&now));

    file << timestamp << "," << role << "," << enteredId << "," << result << "\n";
    file.close();
}

void FileHandler::archivePatient(int patientId,
    Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions)
{
    ofstream archive("discharged.txt", ios::app);
    if (!archive.is_open()) throw FileNotFoundException("discharged.txt");

    //writing patient record
    Patient* p = patients.findById(patientId);

    if (p) {
        archive << p->getId() << "," << p->getName() << ","
            << p->getAge() << "," << p->getGender() << ","
            << p->getContact() << "," << p->getPassword()
            << "," << p->getBalance() << endl;
    }

    //writing appointments
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];

        if (a.getPatientId() == patientId) {
            archive << a.getId() << "," << a.getPatientId() << ","
                << a.getDoctorId() << "," << a.getDate()
                << "," << a.getTimeSlot() << "," << a.getStatus() << endl;
        }
    }

    //writing bills
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];

        if (b.getPatientId() == patientId) {
            archive << b.getId() << "," << b.getPatientId() << ","
                << b.getAppointmentId() << "," << b.getAmount()
                << "," << b.getStatus() << "," << b.getDate() <<endl;
        }
    }

    //writing prescriptions
    for (int i = 0; i < prescriptions.size(); i++) {
        Prescription& pr = prescriptions.getAll()[i];

        if (pr.getPatientId() == patientId) {
            archive << pr.getId() << "," << pr.getAppointmentId() << ","
                << pr.getPatientId() << "," << pr.getDoctorId()
                << "," << pr.getDate() << "," << pr.getMedicines()
                << "," << pr.getNotes() << endl;
        }
    }

    archive.close();
}