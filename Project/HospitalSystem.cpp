#define _CRT_SECURE_NO_WARNINGS
#include "HospitalSystem.h"
#include "FileHandler.h"
#include "HospitalException.h"
#include <iostream>
using namespace std;

HospitalSystem::HospitalSystem() {}

void HospitalSystem::loadAllData() {
    patients.clear();
    doctors.clear();
    appointments.clear();
    bills.clear();
    prescriptions.clear();

    try { FileHandler::loadPatients(patients); }
    catch (FileNotFoundException& e) { cout << "Failed: patients.txt" << endl; }

    try { FileHandler::loadDoctors(doctors); }
    catch (FileNotFoundException& e) { cout << "Failed: doctors.txt" << endl; }

    try { FileHandler::loadAdmin(admin); }
    catch (FileNotFoundException& e) { cout << "Failed: admin.txt" << endl; }

    try { FileHandler::loadAppointments(appointments); }
    catch (FileNotFoundException& e) { cout << "Failed: appointments.txt" << endl; }

    try { FileHandler::loadBills(bills); }
    catch (FileNotFoundException& e) { cout << "Failed: bills.txt" << endl; }

    try { FileHandler::loadPrescriptions(prescriptions); }
    catch (FileNotFoundException& e) { cout << "Failed: prescriptions.txt" << endl; }
}
