#pragma once
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class FileHandler {
public:

	//for loading
    static void loadPatients(Storage<Patient>& storage);
    static void loadDoctors(Storage<Doctor>& storage);
    static void loadAdmin(Admin& admin);
    static void loadAppointments(Storage<Appointment>& storage);
    static void loadBills(Storage<Bill>& storage);
    static void loadPrescriptions(Storage<Prescription>& storage);

    //for appending 
    static void appendPatient(const Patient& p);
    static void appendDoctor(const Doctor& d);
    static void appendAppointment(const Appointment& a);
    static void appendBill(const Bill& b);
    static void appendPrescription(const Prescription& p);

    //for updating
    static void updatePatient(const Patient& p);
    static void updateDoctor(const Doctor& d);
    static void updateAppointment(const Appointment& a);
    static void updateBill(const Bill& b);

	//for deleting
    static void deletePatient(int patientId);
    static void deleteDoctor(int doctorId);

    //cascade delete all patient related records
    static void cascadeDeletePatient(int patientId);

    //security log
    static void appendSecurityLog(const char* role, const char* enteredId, const char* result);

    //discharge
    static void archivePatient(int patientId,
        Storage<Patient>& patients,
        Storage<Appointment>& appointments,
        Storage<Bill>& bills,
        Storage<Prescription>& prescriptions);
};