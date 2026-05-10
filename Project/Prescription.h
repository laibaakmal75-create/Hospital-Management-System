#pragma once
#include<iostream>

class Prescription {
	int prescriptionId;
	int patientId;
	int doctorId;
	int appointmentId; 
	char date[12];
	char medicines[500];
	char notes[300];

public:

	//constructors
	Prescription();
	Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const char* date,const char* medicines, const char* notes);

	//getters
	int getId()const;
	int getPrescriptionId() const;
	int getPatientId() const;
	int getDoctorId() const;
	const char* getMedicines() const;
	int getAppointmentId() const;
	const char* getDate() const;
	const char* getNotes() const;

	//setters
	void setPrescriptionId(int id);
	void setPatientId(int id);
	void setDoctorId(int id);
	void setMedicines(const char* medicines);
	void setAppointmentId(int id);
	void setDate(const char* date);
	void setNotes(const char* notes);

	//friend func
	friend std::ostream& operator<<(std::ostream& out, const Prescription& p);
};