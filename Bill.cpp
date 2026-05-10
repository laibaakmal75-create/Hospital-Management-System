#include "Bill.h"
#include "helper.h"
#include<iostream>
using namespace std;

Bill::Bill() {
	billId = 0;
	patientId = 0;
	appointmentId = 0;
	amount = 0.0;
	status[0] = '\0';
	date[0] = '\0';
}

Bill::Bill(int billId, int patientId, int appointmentId, double amount, const char* status, const char* date) {
	this->billId = billId;
	this->patientId = patientId;
	this->appointmentId = appointmentId;
	this->amount = amount;
	copyString(this->status, status);
	copyString(this->date, date);
}
int Bill::getId()const {
	return billId;
}

int Bill::getBillId() const {
	return billId;
}

int Bill::getPatientId() const {
	return patientId;
}

int Bill::getAppointmentId() const {
	return appointmentId;
}

double Bill::getAmount() const {
	return amount;
}

const char* Bill::getStatus() const {
	return status;
}

const char* Bill::getDate() const {
	return date;
}

void Bill::setBillId(int id) {
	this->billId = id;
}

void Bill::setPatientId(int id) {
	this->patientId = id;
}

void Bill::setAppointmentId(int id) {
	this->appointmentId = id;
}

void Bill::setAmount(double amount) {
	this->amount = amount;
}

void Bill::setStatus(const char* status) {
	copyString(this->status, status);
}

void Bill::setDate(const char* date) {
	copyString(this->date, date);
}

std::ostream& operator<<(std::ostream& out, const Bill& b) {
	out << "Bill ID: " << b.billId << endl;
	out << "Patient ID: " << b.patientId << endl;
	out << "Appointment ID: " << b.appointmentId << endl;
	out << "Amount: " << b.amount <<" PKR"<< endl;
	out << "Status: " << b.status << endl;
	out << "Date: " << b.date << endl;
	return out;
}
