#include "HospitalException.h"
#include"helper.h";

HospitalException::HospitalException(const char* m) {
	copyString(message, m);
}

const char* HospitalException::what() const {
	return message;
}
InsufficientFundsException::InsufficientFundsException() : HospitalException("Error: Insufficient funds.") {}

FileNotFoundException::FileNotFoundException(const char* file) :HospitalException("Error: Cannot open this file.") {}

SlotUnavailableException::SlotUnavailableException(): HospitalException("Error: This time slot is already taken.") {}

InvalidInputException::InvalidInputException():HospitalException("Error: Invalid Input."){}
