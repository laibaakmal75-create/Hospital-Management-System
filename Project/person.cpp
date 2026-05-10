#include "Person.h"
#include"helper.h";

Person::Person() {
	id = 0;
	name[0] = '\0';
	password[0] = '\0';
}

Person::Person(int id, const char* name, const char* password) {
	this->id = id;
	copyString(this->name, name);
	copyString(this->password, password);
}

int Person::getId()const {
	return id;
}

const char* Person::getName()const {
	return name;
}


const char* Person::getPassword()const {
	return password;
}

void Person::setId(int i) {
	this->id = i;
}

void Person::setName(const char* n) {
	copyString(this->name, n);
}

void Person::setPassword(const char* p) {
	copyString(this->password, p);
}

Person::~Person() {
}