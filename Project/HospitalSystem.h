#pragma once

#pragma once
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"

class HospitalSystem {
    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;
    Admin admin;

public:
    HospitalSystem();

    // Startup
    void loadAllData();
    void run();

    // Login
    void showMainMenu();
    Patient* loginPatient();
    Doctor* loginDoctor();
    bool loginAdmin();

    // Patient actions
    void patientMenu(Patient* p);
    void bookAppointment(Patient* p);
    void cancelAppointment(Patient* p);
    void viewMyAppointments(Patient* p);
    void viewMyMedicalRecords(Patient* p);
    void viewMyBills(Patient* p);
    void payBill(Patient* p);
    void topUpBalance(Patient* p);

    // Doctor actions
    void doctorMenu(Doctor* d);
    void viewTodaysAppointments(Doctor* d);
    void markAppointmentComplete(Doctor* d);
    void markAppointmentNoShow(Doctor* d);
    void writePrescription(Doctor* d);
    void viewPatientMedicalHistory(Doctor* d);

    // Admin actions
    void adminMenu();
    void addDoctor();
    void removeDoctor();
    void addPatient();
    void removePatient();
    void viewAllPatients();
    void viewAllDoctors();
    void viewAllAppointments();
    void viewUnpaidBills();
    void dischargePatient();
    void viewSecurityLog();
    void generateDailyReport();

    // Helpers
    void getToday(char* dateStr);
    void sortAppointmentsByDate(Appointment* arr, int n, bool ascending);
    void sortPrescriptionsByDate(Prescription* arr, int n, bool ascending);
    void caseInsensitiveCompare(const char* str1, const char* str2, bool& result);

    Storage<Patient>& getPatients() { return patients; }
    Storage<Doctor>& getDoctors() { return doctors; }
    Storage<Appointment>& getAppointments() { return appointments; }
    Storage<Bill>& getBills() { return bills; }
    Storage<Prescription>& getPrescriptions() { return prescriptions; }
    Admin& getAdmin() { return admin; }
};