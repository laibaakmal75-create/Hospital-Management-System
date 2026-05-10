#include "Prescription.h"
#include "helper.h"
#include<iostream>
using namespace std;

Prescription::Prescription() {
	prescriptionId = 0;
	patientId = 0;
	doctorId = 0;
	medicines[0] = '\0';
	date[0] = '\0';
	notes[0] = '\0';
}

Prescription::Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const char* date, const char* medicines, const char* notes) {
	this->prescriptionId = prescriptionId;
	this->appointmentId = appointmentId;
	this->patientId = patientId;
	this->doctorId = doctorId;
	copyString(this->date, date);
	copyString(this->medicines, medicines);
	copyString(this->notes, notes);
}

int Prescription::getId()const {
	return prescriptionId;
}

int Prescription::getDoctorId()const {
	return prescriptionId;
}

int Prescription::getPrescriptionId() const {
	return prescriptionId;
}

int Prescription::getPatientId() const {
	return patientId;
}

const char* Prescription::getMedicines() const {
	return medicines;
}

int Prescription::getAppointmentId() const {
	return appointmentId;
}

const char* Prescription::getDate() const {
	return date;
}

const char* Prescription::getNotes() const {
	return notes;
}

void Prescription::setPrescriptionId(int id) {
	this->prescriptionId = id;
}

void Prescription::setPatientId(int id) {
	this->patientId = id;
}

void Prescription::setDoctorId(int id) {
	this->doctorId = id;
}

void Prescription::setMedicines(const char* medicines) {
	copyString(this->medicines, medicines);
}

void Prescription::setAppointmentId(int id) {
	this->appointmentId = id;
}

void Prescription::setDate(const char* date) {
	copyString(this->date, date);
}

void Prescription::setNotes(const char* notes) {
	copyString(this->notes, notes);
}

ostream& operator<<(ostream& out, const Prescription& p) {
	out << "Prescription ID: " << p.prescriptionId << endl;
	out << "Patient ID: " << p.patientId << endl;
	out << "Appointment ID: " << p.appointmentId << endl;
	out << "Doctor ID: " << p.doctorId << endl;
	out << "Date: " << p.date << endl;
	out << "Medicines: " << p.medicines << endl;
	out << "Notes: " << p.notes << endl;
	return out;
}  