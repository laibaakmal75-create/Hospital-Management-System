#include "Appointment.h"
#include "helper.h"
#include<iostream>
using namespace std;

Appointment::Appointment() {
	appointmentId = 0;
	patientId = 0;
	doctorId = 0;
	date[0] = '\0';
	timeSlot[0] = '\0';
	status[0] = '\0';
}

Appointment::Appointment(int appointmentId, int patientId, int doctorId, const char* date, const char* timeSlot, const char* status)
{
	this->appointmentId = appointmentId;
	this->patientId = patientId;
	this->doctorId = doctorId;
	copyString(this->date, date);
	copyString(this->timeSlot, timeSlot);
	copyString(this->status, status);
}
int Appointment::getId() const {
	return appointmentId;
}

int Appointment::getAppointmentId() const {
	return appointmentId;
}

int Appointment::getPatientId() const {
	return patientId;
}

int Appointment::getDoctorId() const {
	return doctorId;
}

const char* Appointment::getDate() const {
	return date;
}

const char* Appointment::getTimeSlot() const {
	return timeSlot;
}

const char* Appointment::getStatus() const {
	return status;
}

void Appointment::setAppointmentId(int id) {
	this->appointmentId = id;
}

void Appointment::setPatientId(int id) {
	this->patientId = id;
}

void Appointment::setDoctorId(int id) {
	this->doctorId = id;
}

void Appointment::setDate(const char* date) {
	copyString(this->date, date);
}

void Appointment::setTimeSlot(const char* timeSlot) {
	copyString(this->timeSlot, timeSlot);
}

void Appointment::setStatus(const char* status) {
	copyString(this->status, status);
}

bool Appointment::operator==(const Appointment& other) const {
	if (this->doctorId != other.doctorId) {
		return false;
	}
	if (!compareString(this->date, other.date)) {
		return false;
	}
	if (!compareString(this->timeSlot, other.timeSlot)) {
		return false;
	}
	if (compareString(this->status, "cancelled")) {
		return false;
	}
	if (compareString(other.status, "cancelled")) {
		return false;
	}
	return true;
}

ostream& operator<<(ostream& out, const Appointment& a) {
	out << "Appointment ID: " << a.appointmentId << endl;
	out << "Patient ID: " << a.patientId << endl;
	out << "Doctor ID: " << a.doctorId << endl;
	out << "Date: " << a.date << endl;
	out << "Time Slot: " << a.timeSlot << endl;
	out << "Status: " << a.status << endl;
	return out;
}