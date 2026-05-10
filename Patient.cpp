#include"Patient.h"
#include"helper.h"
#include<iostream>
using namespace std;

Patient::Patient() {
    age = 0;
    gender = '\0';
    contact[0] = '\0';
    balance = 0.0;
}

Patient::Patient(int id, const char* name, int age, char gender, const char* contact, const char* password, double balance)
:Person(id, name, password){
    this->age = age;
    this->gender = gender;
    copyString(this->contact, contact);
    this->balance = balance;
}

int Patient::getAge() const {
    return age;
}

char Patient::getGender() const {
    return gender;
}

const char* Patient::getContact() const {
    return contact;
}

double Patient::getBalance()const {
    return balance;
}

void Patient::setAge(int age) {
    this->age = age;
}

void Patient::setGender(char gender) {
    this->gender = gender;
}

void Patient::setContact(const char* contact){
    copyString(this->contact, contact); 
}

void Patient::setBalance(double balance) {
    this->balance = balance;
}

Patient& Patient::operator+=(double amount) {
    balance += amount;
    return *this;
}

Patient& Patient::operator-=(double amount) {
    balance -= amount;
    return *this;
}

bool Patient::operator==(const Patient& other) const {
    return this->id == other.id;
}

ostream& operator<<(ostream& out, const Patient& p) {
    p.display();
    return out;
}


void Patient::display() const {
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Gender: " << gender << endl;
    cout << "Contact: " << contact << endl;
    cout << "Balance: " << balance << endl;
}

void Patient::displayMenu() {
    cout << endl<< "Welcome, " << name << endl;
    cout << "Balance: PKR " << balance << endl;
    cout << "========================"<<endl;
    cout << "1. Book Appointment"<<endl;
    cout << "2. Cancel Appointment" << endl;
    cout << "3. View My Appointments"<<endl;
    cout << "4. View My Medical Records"<<endl;
    cout << "5. View My Bills"<<endl;
    cout << "6. Pay Bill"<<endl;
    cout << "7. Top Up Balance"<<endl;
    cout << "8. Logout" << endl;
}