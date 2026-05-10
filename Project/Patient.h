#pragma once
#include"Person.h";
#include"HospitalException.h";

class Patient : public Person {
    int age;
    char gender;
    char contact[12];
    double balance;

public:

    //contructos
    Patient();
    Patient(int id, const char* name, int age, char gender, const char* contact, const char* password, double balance);

    // getters
    int getAge() const;
    char getGender() const;
    const char* getContact() const;
    double getBalance() const;

    // setters
    void setAge(int age);
    void setGender(char gender);
    void setContact(const char* contact);
    void setBalance(double balance);

    //operator overloads
    Patient& operator+=(double amount);
    Patient& operator-=(double amount);
    bool operator==(const Patient& other) const;

    //vitual funcs
    void displayMenu();
    void display() const;

};