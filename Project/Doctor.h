#pragma once

#include"Person.h"
#include"HospitalException.h"
#include <iostream>
using namespace std;

class Doctor :public Person {
	char specialization[51];
	char contact[12];
	double fee;

public:
	//constructors
	Doctor();
	Doctor(int id, const char* name, const char* specialization, const char* contact, const char* password, double fee);

	//getters
	const char* getSpecialization() const;
	const char* getContact() const;
	double getfee() const;

	//setters
	void setSpecialization(const char* specialization);
	void setContact(const char* contact);
	void setFee(double fee);

	//overload
	bool operator==(const Doctor& other) const;
	friend std::ostream& operator<<(std::ostream& out, const Doctor& d);
	
	//virtual funcs
	void displayMenu();
	void display() const;
};