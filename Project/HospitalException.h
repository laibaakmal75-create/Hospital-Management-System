#pragma once
//base class
class HospitalException {

	char message[200];

public:
	//Parameterized const
	HospitalException(const char* m);
	//message of error 
	virtual const char* what() const;
};

class InsufficientFundsException : public HospitalException {
public:
	InsufficientFundsException();
};

class SlotUnavailableException : public HospitalException {
public:
	SlotUnavailableException();
};

class FileNotFoundException : public HospitalException {
public:
	FileNotFoundException(const char* file);
};

class InvalidInputException : public HospitalException {
public:
	InvalidInputException();
};