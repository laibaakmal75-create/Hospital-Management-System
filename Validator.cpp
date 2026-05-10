#include"Validator.h"

// id must be greater than 0
bool Validator::validateId(int id) {
	return id > 0;
}

// date must be in format "YYYY-MM-DD"
bool Validator::validateDate(const char* date) {
	
	//must be 10 chars long
	int l = 0;
	while (date[l] != '\0') {
		l++;
	}
	if (l < 10) {
		return false;
	}

	//check for dashes
	if (date[2] != '-' || date[5] != '-') {
		return false;
	}

	//checking all rem are digits
	for (int i = 0; i < 10; i++) {
		if (i == 2 || i == 5) {
			continue;
		}
		if (date[i] < '0' || date[i] > '9') {
			return false;
		}
	}

	//check for day motnh and year
	int day = (date[0] - '0') * 10 + (date[1] - '0');
	int month = (date[3] - '0') * 10 + (date[4] - '0');
	int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

	if (day < 1 || day > 31) {
		return false;
	}
	if (month < 1 || month > 12) {
		return false;
	}
	if (year < 2026){
		return false;
	}

	return true;
}

//must be one of fixed time slot
bool Validator::validateTimeSlot(const char* slot) {
	const char* validSlots[] = {"09:00", "10:00", "11:00", "12:00","13:00", "14:00", "15:00", "16:00"};

	for (int i = 0; i < 8; i++) {
		int j = 0;
		bool match = true;
		while (validSlots[i][j] != '\0' || slot[j] != '\0') {
			if (validSlots[i][j] != slot[j]) {
				match = false;
				break;
			}
			j++;
		}
		if (match) {
			return true;
		}
	}
	return false;
}

//contact should be 11 numeric digits
bool Validator::validateContact(const char* contact) {

	int len = 0;
	while (contact[len] != '\0' && contact[len] != '\n' && contact[len] != '\r'){
		len++;
    }
	if (len != 11) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (contact[i] < '0' || contact[i] > '9') {
			return false;
		}	
	}
	return true;
}

//pass atleast 6 chars
bool Validator::validatePassword(const char* password) {

	int len = 0;
	while (password[len] != '\0') {
		len++;
	}
	return (len >= 6);
}

//should be greater than 0
bool Validator::validatePositiveFloat(double value) {
	return (value > 0.0);
}

//choice btw min and max inclusive
bool Validator::validateMenuChoice(int choice, int min, int max) {
	return (choice >= min && choice <= max);
}

// age pos and realistic
bool Validator::validateAge(int age) {
	return (age > 0 && age <= 120);
}

// gender male or female 
bool Validator::validateGender(char gender) {
	return (gender == 'M' || gender == 'F');
}