#pragma once
#include<iostream>

class Appointment {
	int appointmentId;
	int patientId;
	int doctorId;
	char date[12];
	char timeSlot[6];
	char status[20];

public:

	//constructors
	Appointment();
	Appointment(int appointmentId, int patientId, int doctorId, const char* date, const char* timeSlot, const char* status);

	//getters
	int getId()const;
	int getAppointmentId() const;
	int getPatientId() const;
	int getDoctorId() const;
	const char* getDate() const;
	const char* getTimeSlot() const;
	const char* getStatus() const;

	//setters
	void setAppointmentId(int id);
	void setPatientId(int id);
	void setDoctorId(int id);
	void setDate(const char* date);
	void setTimeSlot(const char* timeSlot);
	void setStatus(const char* status);

	//operator overloads 
	bool operator==(const Appointment& other) const;
	friend std::ostream& operator<<(std::ostream& out, const Appointment& a);

};