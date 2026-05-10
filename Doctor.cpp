#include"Doctor.h"
#include"helper.h"
#include<iostream>
using namespace std;

Doctor::Doctor() {
	specialization[0] = '\0';
	contact[0] = '\0';
	fee = 0.0;
}

Doctor::Doctor(int id, const char* name, const char* specialization, const char* contact, const char* password, double fee)
	:Person(id, name, password) {

	copyString(this->specialization, specialization);
	copyString(this->contact, contact);
	this->fee = fee;
}

const char* Doctor::getSpecialization() const {
	return specialization;
}

const char* Doctor::getContact() const {
	return contact;
}

double Doctor:: getfee() const {
	return fee;
}

void Doctor::setSpecialization(const char* specialization) {
	copyString(this->specialization, specialization);
}

void Doctor::setContact(const char* contact) {
	copyString(this->contact, contact);
}

void Doctor::setFee(double fee) {
	this->fee = fee;
}

bool Doctor:: operator==(const Doctor& other) const {
	return this->id == other.id;
}

ostream& operator<<(ostream& out, const Doctor& d) {
	d.display();
	return out;
}

void Doctor::display() const {
	cout << "ID: " << id << endl;
	cout << "Name: " << name << endl;
	cout << "Specialization: " << specialization <<endl;
	cout << "Contact: " << contact << endl;
	cout << "Fee: " << fee << endl;
}

void Doctor::displayMenu() {
	cout << endl<<"Welcome, Dr. " << name << endl;
	cout << "Specialization: " << specialization << endl;
	cout << "==============================================="<<endl;
	cout << "1. View Today's Appointments"<<endl;
	cout << "2. Mark Appointment Complete"<<endl;
	cout << "3. Mark Appointment No-Show"<<endl;
	cout << "4. Write Prescription"<<endl;
	cout << "5. View Patient Medical History"<<endl;
	cout << "6. Logout"<<endl;
}