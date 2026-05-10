#pragma once
//abstract base class

class Person {
protected:
	int id;
	char name[50];
	char password[50];

public:
	//constructors
	Person();
	Person(int id, const char* name, const char* password);

	//getters
	int getId() const;
	const char* getName() const;
	const char* getPassword() const;

	//setters
	void setId(int id);
	void setName(const char* name);
	void setPassword(const char* password);

	//this means that we cannot run it through Person directly but only through serived classes
	virtual void displayMenu() = 0; 
	virtual void display() const = 0; 

	//if class has even one virtual func its better to add virtual dest
	//so that derived class's dest runs first and cleans up mem aswell

	virtual ~Person();
};