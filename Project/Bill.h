#pragma once
#include<iostream>

class Bill {
	int billId;
	int patientId;
	int appointmentId;
	double amount;
	char status[20];
	char date[12];

public:

	//constructors
	Bill();
	Bill(int billId, int patientId, int appointmentId, double amount, const char* status, const char* date);

	//getters
	int getId() const;
	int getBillId() const;
	int getPatientId() const;
	int getAppointmentId() const;
	double getAmount() const;
	const char* getStatus() const;
	const char* getDate() const;

	//setters
	void setBillId(int id);
	void setPatientId(int id);
	void setAppointmentId(int id);
	void setAmount(double amount);
	void setStatus(const char* status);
	void setDate(const char* date);

	//friend func
	friend std::ostream& operator<<(std::ostream& out, const Bill& b);
};

