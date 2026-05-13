#pragma once
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class HospitalSystem {
private:

    //each one holds up to 100 objects of the type 
    //not for admin tho as admin is only 1 
    Storage<Patient>      
        patients;
    Storage<Doctor>  
        doctors;
    Storage<Appointment> 
        appointments;
    Storage<Bill>    
        bills;
    Storage<Prescription> 
        prescriptions;
    Admin 
        admin;

public:
    HospitalSystem();
    void loadAllData();

    //getters for sfml app to access storage 
    Storage<Patient>& getPatients() { return patients; }
    Storage<Doctor>& getDoctors() { return doctors; }
    Storage<Appointment>& getAppointments() { return appointments; }
    Storage<Bill>& getBills() { return bills; }
    Storage<Prescription>& getPrescriptions() { return prescriptions; }
    Admin& getAdmin() { return admin; }
};
