#define _CRT_SECURE_NO_WARNINGS
#include "App.h"
#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"
#include "helper.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>
using namespace std;

App::App()
	: window(sf::VideoMode(1100, 700), "MediCore Hospital", sf::Style::Close | sf::Style::Titlebar),
	currentScreen(Screen::LOGIN),
	currentRole(UserRole::NONE),
	selectedRole(UserRole::PATIENT),
	loggedPatient(nullptr),
	loggedDoctor(nullptr),
	loginAttempts(0),
	accountLocked(false),
	scrollOffset(0.f)
{
	window.setFramerateLimit(60);
	init();
}

void App::init() {
	if (!fontRegular.loadFromFile("Comfortaa-Regular.ttf"))
		fontRegular.loadFromFile("arial.ttf");
	if (!fontBold.loadFromFile("Comfortaa-Bold.ttf"))
		fontBold.loadFromFile("arial.ttf");

	hospital.loadAllData();

	toast.box.setSize({ 340.f, 48.f });
	toast.box.setPosition(380.f, 20.f);
	toast.msg.setFont(fontRegular);
	toast.msg.setCharacterSize(13);
	toast.msg.setFillColor(Colors::WHITE);
	toast.msg.setPosition(396.f, 34.f);

	backBtn = makeButton(240.f, 20.f, 90.f, 34.f, "< Back", fontRegular, Colors::MINT, sf::Color(160, 205, 185), Colors::TEXT_DARK, 13);
	backBtn.onClick = [this]() {
		switchScreen(Screen::LOGIN);
		};

	buildLoginScreen();
}

void App::run() {
	while (window.isOpen()) {
		handleEvents();
		render();
	}
}

void App::handleEvents() {
	sf::Event e;
	while (window.pollEvent(e)) {
		if (e.type == sf::Event::Closed)
			window.close();

		if (e.type == sf::Event::MouseWheelScrolled)
			scrollOffset -= e.mouseWheelScroll.delta * 20.f;

		if (currentScreen == Screen::LOGIN) {
			loginIdInput.handleEvent(e, window);
			loginPassInput.handleEvent(e, window);
			loginBtn.handleEvent(e, window);
			rolePatientBtn.handleEvent(e, window);
			roleDoctorBtn.handleEvent(e, window);
			roleAdminBtn.handleEvent(e, window);
		}
		else {
			backBtn.handleEvent(e, window);
			input1.handleEvent(e, window);
			input2.handleEvent(e, window);
			input3.handleEvent(e, window);
			input4.handleEvent(e, window);
			input5.handleEvent(e, window);
			input6.handleEvent(e, window);
			actionBtn.handleEvent(e, window);

			if (currentRole == UserRole::PATIENT)
				for (auto& b : patientSidebarBtns) b.handleEvent(e, window);
			else if (currentRole == UserRole::DOCTOR)
				for (auto& b : doctorSidebarBtns) b.handleEvent(e, window);
			else if (currentRole == UserRole::ADMIN)
				for (auto& b : adminSidebarBtns) b.handleEvent(e, window);
		}
	}
}

void App::buildLoginScreen() {
	rolePatientBtn = makeButton(540.f, 230.f, 110.f, 36.f, "Patient", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 13);
	rolePatientBtn.onClick = [this]() {
		selectedRole = UserRole::PATIENT;
		toast.show("Login as Patient", false);
		};

	roleDoctorBtn = makeButton(660.f, 230.f, 110.f, 36.f, "Doctor", fontRegular, Colors::MINT, sf::Color(160, 205, 185), Colors::TEXT_DARK, 13);
	roleDoctorBtn.onClick = [this]() {
		selectedRole = UserRole::DOCTOR;
		toast.show("Login as Doctor", false);
		};

	roleAdminBtn = makeButton(780.f, 230.f, 110.f, 36.f, "Admin", fontRegular, sf::Color(180, 170, 220), sf::Color(155, 145, 200), Colors::WHITE, 13);
	roleAdminBtn.onClick = [this]() {
		selectedRole = UserRole::ADMIN;
		toast.show("Login as Admin", false);
		};

	loginIdInput = makeInput(180.f, 380.f, 350.f, 44.f, "Enter your ID", fontRegular);
	loginPassInput = makeInput(180.f, 436.f, 350.f, 44.f, "Enter password", fontRegular, true);

	loginBtn = makeButton(180.f, 500.f, 350.f, 44.f, "Login", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 15);
	loginBtn.onClick = [this]() {
		if (selectedRole == UserRole::PATIENT) doPatientLogin();
		else if (selectedRole == UserRole::DOCTOR) doDoctorLogin();
		else if (selectedRole == UserRole::ADMIN) doAdminLogin();
		};
}

void App::buildPatientSidebar() {
	patientSidebarBtns.clear();
	float y = 160.f;
	float gap = 52.f;

	struct Item { std::string label; Screen scr; };
	std::vector<Item> items = {
		{"Book Appointment",   Screen::PATIENT_BOOK},
		{"Cancel Appointment", Screen::PATIENT_CANCEL},
		{"My Appointments",    Screen::PATIENT_APPOINTMENTS},
		{"Medical Records",    Screen::PATIENT_RECORDS},
		{"My Bills",           Screen::PATIENT_BILLS},
		{"Pay Bill",           Screen::PATIENT_PAY},
		{"Top Up Balance",     Screen::PATIENT_TOPUP},
		{"Logout",             Screen::LOGIN}
	};

	for (auto& item : items) {
		Button b = makeButton(8.f, y, 194.f, 40.f, item.label, fontRegular, sf::Color(0, 0, 0, 0), sf::Color(220, 155, 155, 80), Colors::TEXT_DARK, 12);
		Screen s = item.scr;
		b.onClick = [this, s]() {
			if (s == Screen::LOGIN) logout();
			else switchScreen(s);
			};
		patientSidebarBtns.push_back(b);
		y += gap;
	}
}

void App::buildDoctorSidebar() {
	doctorSidebarBtns.clear();
	float y = 160.f;
	float gap = 52.f;

	struct Item { std::string label; Screen scr; };
	std::vector<Item> items = {
		{"Today's Appointments", Screen::DOCTOR_TODAY},
		{"Mark Complete",        Screen::DOCTOR_COMPLETE},
		{"Mark No-Show",         Screen::DOCTOR_NOSHOW},
		{"Write Prescription",   Screen::DOCTOR_PRESCRIPTION},
		{"Patient History",      Screen::DOCTOR_HISTORY},
		{"Logout",               Screen::LOGIN}
	};

	for (auto& item : items) {
		Button b = makeButton(8.f, y, 194.f, 40.f, item.label, fontRegular, sf::Color(0, 0, 0, 0), sf::Color(192, 225, 210, 100), Colors::TEXT_DARK, 12);
		Screen s = item.scr;
		b.onClick = [this, s]() {
			if (s == Screen::LOGIN) logout();
			else switchScreen(s);
			};
		doctorSidebarBtns.push_back(b);
		y += gap;
	}
}

void App::buildAdminSidebar() {
	adminSidebarBtns.clear();
	float y = 160.f;
	float gap = 46.f;

	struct Item { std::string label; Screen scr; };
	std::vector<Item> items = {
		{"Add Doctor",       Screen::ADMIN_ADD_DOCTOR},
		{"Remove Doctor",    Screen::ADMIN_REMOVE_DOCTOR},
		{"Add Patient",      Screen::ADMIN_ADD_PATIENT},
		{"Remove Patient",   Screen::ADMIN_REMOVE_PATIENT},
		{"All Patients",     Screen::ADMIN_ALL_PATIENTS},
		{"All Doctors",      Screen::ADMIN_ALL_DOCTORS},
		{"All Appointments", Screen::ADMIN_ALL_APPOINTMENTS},
		{"Unpaid Bills",     Screen::ADMIN_UNPAID_BILLS},
		{"Discharge Patient",Screen::ADMIN_DISCHARGE},
		{"Security Log",     Screen::ADMIN_SECURITY_LOG},
		{"Daily Report",     Screen::ADMIN_REPORT},
		{"Logout",           Screen::LOGIN}
	};

	for (auto& item : items) {
		Button b = makeButton(8.f, y, 194.f, 36.f, item.label, fontRegular, sf::Color(0, 0, 0, 0), sf::Color(220, 155, 155, 80), Colors::TEXT_DARK, 11);
		Screen s = item.scr;
		b.onClick = [this, s]() {
			if (s == Screen::LOGIN) logout();
			else switchScreen(s);
			};
		adminSidebarBtns.push_back(b);
		y += gap;
	}
}

void App::drawBackground() {
	window.clear(Colors::BG);
}

void App::drawCard(float x, float y, float w, float h) {
	sf::RectangleShape card({ w, h });
	card.setPosition(x, y);
	card.setFillColor(Colors::CARD_BG);
	card.setOutlineThickness(1.f);
	card.setOutlineColor(Colors::BORDER);
	window.draw(card);
}

void App::drawSidebarPanel() {
	sf::RectangleShape sidebar({ 210.f, 700.f });
	sidebar.setPosition(0.f, 0.f);
	sidebar.setFillColor(Colors::SIDEBAR);
	window.draw(sidebar);

	sf::RectangleShape logoBar({ 210.f, 100.f });
	logoBar.setPosition(0.f, 0.f);
	logoBar.setFillColor(Colors::ACCENT);
	window.draw(logoBar);

	sf::RectangleShape crossV({ 4.f, 20.f });
	crossV.setPosition(26.f, 30.f);
	crossV.setFillColor(Colors::WHITE);
	window.draw(crossV);

	sf::RectangleShape crossH({ 20.f, 4.f });
	crossH.setPosition(18.f, 38.f);
	crossH.setFillColor(Colors::WHITE);
	window.draw(crossH);

	auto title = makeText("MediCore", fontBold, 18, Colors::WHITE, 50.f, 28.f);
	window.draw(title);
	auto sub = makeText("Hospital Management", fontRegular, 11, sf::Color(255, 255, 255, 180), 56.f, 65.f);
	window.draw(sub);

	sf::RectangleShape div({ 210.f, 1.f });
	div.setPosition(0.f, 100.f);
	div.setFillColor(Colors::BORDER);
	window.draw(div);
}

sf::Text App::makeLabel(const std::string& s, int size, sf::Color c, float x, float y) {
	return makeText(s, fontRegular, size, c, x, y);
}

void App::clearInputs() {
	input1.clear(); input2.clear(); input3.clear();
	input4.clear(); input5.clear(); input6.clear();
	scrollOffset = 0.f;
	tableData.clear();
	tableHeaders.clear();
}

void App::logout() {
	loggedPatient = nullptr;
	loggedDoctor = nullptr;
	currentRole = UserRole::NONE;
	loginAttempts = 0;
	accountLocked = false;
	loginIdInput.clear();
	loginPassInput.clear();
	clearInputs();
	currentScreen = Screen::LOGIN;
}

void App::render() {
	drawBackground();

	if (currentScreen == Screen::LOGIN) {
		renderLogin();
	}
	else {
		drawSidebarPanel();

		if (currentRole == UserRole::PATIENT && loggedPatient) {
			drawCard(8.f, 615.f, 194.f, 76.f);
			auto nm = makeLabel(loggedPatient->getName(), 12, Colors::TEXT_DARK, 16.f, 622.f);
			window.draw(nm);
			std::string bal = "PKR " + std::to_string((int)loggedPatient->getBalance());
			auto bl = makeLabel(bal, 11, Colors::SUCCESS, 16.f, 642.f);
			window.draw(bl);
		}
		else if (currentRole == UserRole::DOCTOR && loggedDoctor) {
			drawCard(8.f, 615.f, 194.f, 76.f);
			auto nm = makeLabel(loggedDoctor->getName(), 12, Colors::TEXT_DARK, 16.f, 622.f);
			window.draw(nm);
			auto sp = makeLabel(loggedDoctor->getSpecialization(), 11, Colors::TEXT_MID, 16.f, 642.f);
			window.draw(sp);
		}

		if (currentRole == UserRole::PATIENT) {
			auto lbl = makeLabel("PATIENT", 10, Colors::TEXT_MID, 16.f, 135.f);
			window.draw(lbl);
			for (auto& b : patientSidebarBtns) b.draw(window);
		}
		else if (currentRole == UserRole::DOCTOR) {
			auto lbl = makeLabel("DOCTOR", 10, Colors::TEXT_MID, 16.f, 135.f);
			window.draw(lbl);
			for (auto& b : doctorSidebarBtns) b.draw(window);
		}
		else if (currentRole == UserRole::ADMIN) {
			auto lbl = makeLabel("ADMIN", 10, Colors::TEXT_MID, 16.f, 135.f);
			window.draw(lbl);
			for (auto& b : adminSidebarBtns) b.draw(window);
		}

		window.draw(backBtn.shape);
		window.draw(backBtn.label);

		switch (currentScreen) {
		case Screen::PATIENT_MENU:         renderPatientMenu(); break;
		case Screen::DOCTOR_MENU:          renderDoctorMenu();  break;
		case Screen::ADMIN_MENU:           renderAdminMenu();   break;
		case Screen::PATIENT_BOOK:
		case Screen::PATIENT_CANCEL:
		case Screen::PATIENT_PAY:
		case Screen::PATIENT_TOPUP:
		case Screen::DOCTOR_COMPLETE:
		case Screen::DOCTOR_NOSHOW:
		case Screen::DOCTOR_PRESCRIPTION:
		case Screen::DOCTOR_HISTORY:
		case Screen::ADMIN_ADD_DOCTOR:
		case Screen::ADMIN_ADD_PATIENT:
		case Screen::ADMIN_REMOVE_DOCTOR:
		case Screen::ADMIN_REMOVE_PATIENT:
		case Screen::ADMIN_DISCHARGE:      renderForm();  break;
		case Screen::PATIENT_APPOINTMENTS:
		case Screen::PATIENT_RECORDS:
		case Screen::PATIENT_BILLS:
		case Screen::DOCTOR_TODAY:
		case Screen::ADMIN_ALL_PATIENTS:
		case Screen::ADMIN_ALL_DOCTORS:
		case Screen::ADMIN_ALL_APPOINTMENTS:
		case Screen::ADMIN_UNPAID_BILLS:
		case Screen::ADMIN_SECURITY_LOG:
		case Screen::ADMIN_REPORT:         renderTable(); break;
		default: break;
		}
	}

	toast.draw(window);
	window.display();
}

void App::renderLogin() {
	sf::RectangleShape left({ 420.f, 700.f });
	left.setPosition(0.f, 0.f);
	left.setFillColor(Colors::ACCENT);
	window.draw(left);

	sf::CircleShape c1(120.f);
	c1.setFillColor(sf::Color(255, 255, 255, 30));
	c1.setPosition(-40.f, 400.f);
	window.draw(c1);

	sf::CircleShape c2(80.f);
	c2.setFillColor(sf::Color(255, 255, 255, 20));
	c2.setPosition(280.f, -20.f);
	window.draw(c2);

	sf::RectangleShape cv({ 8.f, 50.f });
	cv.setPosition(80.f, 295.f);
	cv.setFillColor(Colors::WHITE);
	window.draw(cv);

	sf::RectangleShape ch({ 50.f, 8.f });
	ch.setPosition(59.f, 316.f);
	ch.setFillColor(Colors::WHITE);
	window.draw(ch);

	auto title = makeText("MediCore", fontBold, 38, Colors::WHITE, 140.f, 290.f);
	window.draw(title);
	auto sub = makeText("Hospital Management System", fontRegular, 14, sf::Color(255, 255, 255, 200), 130.f, 340.f);
	window.draw(sub);

	drawCard(440.f, 140.f, 620.f, 400.f);

	auto loginAs = makeText("Login as:", fontBold, 13, Colors::TEXT_MID, 540.f, 200.f);
	window.draw(loginAs);

	rolePatientBtn.shape.setPosition(540.f, 220.f);
	rolePatientBtn.label.setPosition(540.f + (110.f - rolePatientBtn.label.getLocalBounds().width) / 2.f, 228.f);
	rolePatientBtn.draw(window);

	roleDoctorBtn.shape.setPosition(660.f, 220.f);
	roleDoctorBtn.label.setPosition(660.f + (110.f - roleDoctorBtn.label.getLocalBounds().width) / 2.f, 228.f);
	roleDoctorBtn.draw(window);

	roleAdminBtn.shape.setPosition(780.f, 220.f);
	roleAdminBtn.label.setPosition(780.f + (110.f - roleAdminBtn.label.getLocalBounds().width) / 2.f, 228.f);
	roleAdminBtn.draw(window);

	auto idLbl = makeText("ID:", fontBold, 12, Colors::TEXT_MID, 540.f, 280.f);
	window.draw(idLbl);
	loginIdInput.box.setPosition(540.f, 297.f);
	loginIdInput.placeholder.setPosition(552.f, 310.f);
	loginIdInput.displayText.setPosition(552.f, 310.f);
	loginIdInput.draw(window);

	auto pwLbl = makeText("Password:", fontBold, 12, Colors::TEXT_MID, 540.f, 360.f);
	window.draw(pwLbl);
	loginPassInput.box.setPosition(540.f, 377.f);
	loginPassInput.placeholder.setPosition(552.f, 390.f);
	loginPassInput.displayText.setPosition(552.f, 390.f);
	loginPassInput.draw(window);

	loginBtn.shape.setPosition(540.f, 440.f);
	loginBtn.label.setPosition(666.f, 452.f);
	loginBtn.draw(window);
}

void App::renderPatientMenu() {
	float x = 240.f;
	auto title = makeText("Patient Dashboard", fontBold, 20, Colors::TEXT_DARK, x, 70.f);
	window.draw(title);

	if (!loggedPatient) return;

	drawCard(x, 110.f, 260.f, 90.f);
	auto balLbl = makeLabel("Current Balance", 11, Colors::TEXT_MID, x + 16, 122.f);
	window.draw(balLbl);
	std::string balStr = "PKR " + std::to_string((int)loggedPatient->getBalance());
	auto balVal = makeText(balStr, fontBold, 24, Colors::ACCENT, x + 16, 142.f);
	window.draw(balVal);

	drawCard(x + 276.f, 110.f, 580.f, 90.f);
	auto nameLbl = makeLabel("Logged in as:", 11, Colors::TEXT_MID, x + 292, 122.f);
	window.draw(nameLbl);
	auto nameVal = makeText(loggedPatient->getName(), fontBold, 18, Colors::TEXT_DARK, x + 292, 142.f);
	window.draw(nameVal);

	auto hint = makeLabel("Use the sidebar to navigate.", 13, Colors::TEXT_MID, x, 230.f);
	window.draw(hint);
}

void App::renderDoctorMenu() {
	float x = 240.f;
	auto title = makeText("Doctor Dashboard", fontBold, 20, Colors::TEXT_DARK, x, 70.f);
	window.draw(title);

	if (!loggedDoctor) return;

	drawCard(x, 110.f, 400.f, 90.f);
	auto nm = makeText(loggedDoctor->getName(), fontBold, 18, Colors::TEXT_DARK, x + 16, 122.f);
	window.draw(nm);
	auto sp = makeLabel(loggedDoctor->getSpecialization(), 13, Colors::TEXT_MID, x + 16, 150.f);
	window.draw(sp);

	drawCard(x + 416.f, 110.f, 200.f, 90.f);
	auto feeLbl = makeLabel("Consultation Fee", 11, Colors::TEXT_MID, x + 432, 122.f);
	window.draw(feeLbl);
	std::string feeStr = "PKR " + std::to_string((int)loggedDoctor->getfee());
	auto feeVal = makeText(feeStr, fontBold, 20, Colors::MINT, x + 432, 142.f);
	window.draw(feeVal);

	auto hint = makeLabel("Use the sidebar to navigate.", 13, Colors::TEXT_MID, x, 230.f);
	window.draw(hint);
}

void App::renderAdminMenu() {
	float x = 240.f;
	auto title = makeText("Admin Panel", fontBold, 20, Colors::TEXT_DARK, x, 70.f);
	window.draw(title);

	drawCard(x, 110.f, 820.f, 80.f);
	auto lbl = makeLabel("Logged in as Administrator. Use the sidebar to manage the system.", 13, Colors::TEXT_MID, x + 16, 140.f);
	window.draw(lbl);

	auto hint = makeLabel("Use the sidebar to navigate.", 13, Colors::TEXT_MID, x, 220.f);
	window.draw(hint);
}

void App::renderTable() {
	float x = 240.f;

	std::string title = "Results";
	switch (currentScreen) {
	case Screen::PATIENT_APPOINTMENTS:   title = "My Appointments"; break;
	case Screen::PATIENT_RECORDS:        title = "Medical Records"; break;
	case Screen::PATIENT_BILLS:          title = "My Bills"; break;
	case Screen::DOCTOR_TODAY:           title = "Today's Appointments"; break;
	case Screen::ADMIN_ALL_PATIENTS:     title = "All Patients"; break;
	case Screen::ADMIN_ALL_DOCTORS:      title = "All Doctors"; break;
	case Screen::ADMIN_ALL_APPOINTMENTS: title = "All Appointments"; break;
	case Screen::ADMIN_UNPAID_BILLS:     title = "Unpaid Bills"; break;
	case Screen::ADMIN_SECURITY_LOG:     title = "Security Log"; break;
	case Screen::ADMIN_REPORT:           title = "Daily Report"; break;
	default: break;
	}

	auto titleTxt = makeText(title, fontBold, 20, Colors::TEXT_DARK, x, 70.f);
	window.draw(titleTxt);

	if (tableData.empty()) {
		auto empty = makeLabel("No records found.", 14, Colors::TEXT_MID, x, 120.f);
		window.draw(empty);
		return;
	}

	float rowY = 110.f + scrollOffset;
	float colX = x;
	float colW = tableHeaders.empty() ? 0.f : 840.f / (float)tableHeaders.size();

	sf::RectangleShape hdrBg({ 840.f, 32.f });
	hdrBg.setPosition(x, rowY);
	hdrBg.setFillColor(Colors::ACCENT);
	window.draw(hdrBg);

	for (auto& h : tableHeaders) {
		auto hTxt = makeText(h, fontBold, 11, Colors::WHITE, colX + 6, rowY + 9);
		window.draw(hTxt);
		colX += colW;
	}
	rowY += 32.f;

	bool alt = false;
	for (auto& row : tableData) {
		sf::RectangleShape rowBg({ 840.f, 30.f });
		rowBg.setPosition(x, rowY);
		rowBg.setFillColor(alt ? sf::Color(240, 238, 228) : Colors::CARD_BG);
		window.draw(rowBg);

		colX = x;
		for (auto& cell : row) {
			auto cTxt = makeText(cell, fontRegular, 11, Colors::TEXT_DARK, colX + 6, rowY + 8);
			window.draw(cTxt);
			colX += colW;
		}

		sf::RectangleShape border({ 840.f, 1.f });
		border.setPosition(x, rowY + 30.f);
		border.setFillColor(Colors::BORDER);
		window.draw(border);

		rowY += 30.f;
		alt = !alt;
	}
}

void App::renderForm() {
	float x = 240.f;

	std::string title = "Action";
	switch (currentScreen) {
	case Screen::PATIENT_BOOK:         title = "Book Appointment"; break;
	case Screen::PATIENT_CANCEL:       title = "Cancel Appointment"; break;
	case Screen::PATIENT_PAY:          title = "Pay Bill"; break;
	case Screen::PATIENT_TOPUP:        title = "Top Up Balance"; break;
	case Screen::DOCTOR_COMPLETE:      title = "Mark Appointment Complete"; break;
	case Screen::DOCTOR_NOSHOW:        title = "Mark No-Show"; break;
	case Screen::DOCTOR_PRESCRIPTION:  title = "Write Prescription"; break;
	case Screen::DOCTOR_HISTORY:       title = "Patient Medical History"; break;
	case Screen::ADMIN_ADD_DOCTOR:     title = "Add Doctor"; break;
	case Screen::ADMIN_ADD_PATIENT:    title = "Add Patient"; break;
	case Screen::ADMIN_REMOVE_DOCTOR:  title = "Remove Doctor"; break;
	case Screen::ADMIN_REMOVE_PATIENT: title = "Remove Patient"; break;
	case Screen::ADMIN_DISCHARGE:      title = "Discharge Patient"; break;
	default: break;
	}

	auto titleTxt = makeText(title, fontBold, 20, Colors::TEXT_DARK, x, 70.f);
	window.draw(titleTxt);
	drawCard(x, 105.f, 560.f, 500.f);

	switch (currentScreen) {
	case Screen::PATIENT_BOOK:
		input1.draw(window);
		input2.draw(window);
		input3.draw(window);
		input4.draw(window);
		break;
	case Screen::PATIENT_CANCEL:
	case Screen::PATIENT_PAY:
	case Screen::ADMIN_REMOVE_DOCTOR:
	case Screen::ADMIN_REMOVE_PATIENT:
	case Screen::ADMIN_DISCHARGE:
	case Screen::DOCTOR_COMPLETE:
	case Screen::DOCTOR_NOSHOW:
	case Screen::DOCTOR_HISTORY:
		input1.draw(window);
		break;
	case Screen::PATIENT_TOPUP:
		input1.draw(window);
		break;
	case Screen::DOCTOR_PRESCRIPTION:
		input1.draw(window);
		input2.draw(window);
		input3.draw(window);
		break;
	case Screen::ADMIN_ADD_DOCTOR:
		input1.draw(window);
		input2.draw(window);
		input3.draw(window);
		input4.draw(window);
		input5.draw(window);
		break;
	case Screen::ADMIN_ADD_PATIENT:
		input1.draw(window);
		input2.draw(window);
		input3.draw(window);
		input4.draw(window);
		input5.draw(window);
		input6.draw(window);
		break;
	default: break;
	}

	actionBtn.draw(window);
}

void App::doPatientLogin() {
	if (accountLocked) {
		toast.show("Account locked. Contact admin.", true);
		return;
	}

	std::string idStr = loginIdInput.value;
	std::string passStr = loginPassInput.value;
	int id = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') id = id * 10 + (c - '0');

	Patient* p = hospital.getPatients().findById(id);
	if (p && compareString(p->getPassword(), passStr.c_str())) {
		loggedPatient = p;
		currentRole = UserRole::PATIENT;
		loginAttempts = 0;
		buildPatientSidebar();
		switchScreen(Screen::PATIENT_MENU);
		toast.show("Welcome, " + std::string(p->getName()) + "!", false);
	}
	else {
		loginAttempts++;
		FileHandler::appendSecurityLog("Patient", idStr.c_str(), "FAILED");
		if (loginAttempts >= 3) {
			accountLocked = true;
			FileHandler::appendSecurityLog("Patient", idStr.c_str(), "LOCKED");
			toast.show("Account locked. Contact admin.", true);
		}
		else {
			toast.show("Invalid credentials. Attempts left: " + std::to_string(3 - loginAttempts), true);
		}
	}
}

void App::doDoctorLogin() {
	if (accountLocked) {
		toast.show("Account locked. Contact admin.", true);
		return;
	}

	std::string idStr = loginIdInput.value;
	std::string passStr = loginPassInput.value;
	int id = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') id = id * 10 + (c - '0');

	Doctor* d = hospital.getDoctors().findById(id);
	if (d && compareString(d->getPassword(), passStr.c_str())) {
		loggedDoctor = d;
		currentRole = UserRole::DOCTOR;
		loginAttempts = 0;
		buildDoctorSidebar();
		switchScreen(Screen::DOCTOR_MENU);
		toast.show("Welcome, Dr. " + std::string(d->getName()) + "!", false);
	}
	else {
		loginAttempts++;
		FileHandler::appendSecurityLog("Doctor", idStr.c_str(), "FAILED");
		if (loginAttempts >= 3) {
			accountLocked = true;
			FileHandler::appendSecurityLog("Doctor", idStr.c_str(), "LOCKED");
			toast.show("Account locked. Contact admin.", true);
		}
		else {
			toast.show("Invalid credentials. Attempts left: " + std::to_string(3 - loginAttempts), true);
		}
	}
}

void App::doAdminLogin() {
	if (accountLocked) {
		toast.show("Account locked. Contact admin.", true);
		return;
	}

	std::string idStr = loginIdInput.value;
	std::string passStr = loginPassInput.value;
	int id = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') id = id * 10 + (c - '0');

	Admin& adm = hospital.getAdmin();
	if (adm.getId() == id && compareString(adm.getPassword(), passStr.c_str())) {
		currentRole = UserRole::ADMIN;
		loginAttempts = 0;
		buildAdminSidebar();
		switchScreen(Screen::ADMIN_MENU);
		toast.show("Welcome, Administrator!", false);
	}
	else {
		loginAttempts++;
		FileHandler::appendSecurityLog("Admin", idStr.c_str(), "FAILED");
		if (loginAttempts >= 3) {
			accountLocked = true;
			FileHandler::appendSecurityLog("Admin", idStr.c_str(), "LOCKED");
			toast.show("Account locked. Contact admin.", true);
		}
		else {
			toast.show("Invalid credentials. Attempts left: " + std::to_string(3 - loginAttempts), true);
		}
	}
}

void App::switchScreen(Screen s) {
	currentScreen = s;
	clearInputs();
	scrollOffset = 0.f;

	float fx = 256.f, fy = 130.f, fw = 520.f, fh = 44.f, gap = 60.f;

	switch (s) {
	case Screen::PATIENT_BOOK: {
		input1 = makeInput(fx, fy, fw, fh, "Specialization (e.g. Cardiology)", fontRegular);
		input2 = makeInput(fx, fy + gap, fw, fh, "Doctor ID", fontRegular);
		input3 = makeInput(fx, fy + gap * 2, fw, fh, "Date (DD-MM-YYYY)", fontRegular);
		input4 = makeInput(fx, fy + gap * 3, fw, fh, "Time Slot (e.g. 09:00)", fontRegular);
		actionBtn = makeButton(fx, fy + gap * 4, fw, fh, "Book Appointment", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doBookAppointment(); };
		break;
	}
	case Screen::PATIENT_CANCEL: {
		input1 = makeInput(fx, fy, fw, fh, "Appointment ID to cancel", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Cancel Appointment", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doCancelAppointment(); };
		doViewAppointments();
		break;
	}
	case Screen::PATIENT_APPOINTMENTS: doViewAppointments(); break;
	case Screen::PATIENT_RECORDS:      doViewRecords();      break;
	case Screen::PATIENT_BILLS:        doViewBills();        break;
	case Screen::PATIENT_PAY: {
		doViewBills();
		input1 = makeInput(fx, fy, fw, fh, "Bill ID to pay", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Pay Bill", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doPayBill(); };
		break;
	}
	case Screen::PATIENT_TOPUP: {
		input1 = makeInput(fx, fy, fw, fh, "Amount to add (PKR)", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Top Up", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doTopUp(); };
		break;
	}
	case Screen::DOCTOR_TODAY: doViewToday(); break;
	case Screen::DOCTOR_COMPLETE: {
		doViewToday();
		input1 = makeInput(fx, fy, fw, fh, "Appointment ID", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Mark Complete", fontRegular, Colors::MINT, sf::Color(160, 205, 185), Colors::TEXT_DARK, 14);
		actionBtn.onClick = [this]() { doMarkComplete(); };
		break;
	}
	case Screen::DOCTOR_NOSHOW: {
		doViewToday();
		input1 = makeInput(fx, fy, fw, fh, "Appointment ID", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Mark No-Show", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doMarkNoShow(); };
		break;
	}
	case Screen::DOCTOR_PRESCRIPTION: {
		input1 = makeInput(fx, fy, fw, fh, "Appointment ID", fontRegular);
		input2 = makeInput(fx, fy + gap, fw, fh * 2, "Medicines (e.g. Paracetamol 500mg;...)", fontRegular);
		input3 = makeInput(fx, fy + gap * 2 + 20, fw, fh * 2, "Notes", fontRegular);
		actionBtn = makeButton(fx, fy + gap * 3 + 20, fw, fh, "Save Prescription", fontRegular, Colors::MINT, sf::Color(160, 205, 185), Colors::TEXT_DARK, 14);
		actionBtn.onClick = [this]() { doWritePrescription(); };
		break;
	}
	case Screen::DOCTOR_HISTORY: {
		input1 = makeInput(fx, fy, fw, fh, "Patient ID", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "View History", fontRegular, Colors::MINT, sf::Color(160, 205, 185), Colors::TEXT_DARK, 14);
		actionBtn.onClick = [this]() { doViewHistory(); };
		break;
	}
	case Screen::ADMIN_ADD_DOCTOR: {
		input1 = makeInput(fx, fy, fw, fh, "Full Name", fontRegular);
		input2 = makeInput(fx, fy + gap, fw, fh, "Specialization", fontRegular);
		input3 = makeInput(fx, fy + gap * 2, fw, fh, "Contact (11 digits)", fontRegular);
		input4 = makeInput(fx, fy + gap * 3, fw, fh, "Password (min 6 chars)", fontRegular, true);
		input5 = makeInput(fx, fy + gap * 4, fw, fh, "Consultation Fee (PKR)", fontRegular);
		actionBtn = makeButton(fx, fy + gap * 5, fw, fh, "Add Doctor", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doAddDoctor(); };
		break;
	}
	case Screen::ADMIN_ADD_PATIENT: {
		input1 = makeInput(fx, fy, fw, fh, "Full Name", fontRegular);
		input2 = makeInput(fx, fy + gap, fw, fh, "Age", fontRegular);
		input3 = makeInput(fx, fy + gap * 2, fw, fh, "Gender (M/F)", fontRegular);
		input4 = makeInput(fx, fy + gap * 3, fw, fh, "Contact (11 digits)", fontRegular);
		input5 = makeInput(fx, fy + gap * 4, fw, fh, "Password (min 6 chars)", fontRegular, true);
		input6 = makeInput(fx, fy + gap * 5, fw, fh, "Initial Balance (PKR)", fontRegular);
		actionBtn = makeButton(fx, fy + gap * 6, fw, fh, "Add Patient", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doAddPatient(); };
		break;
	}
	case Screen::ADMIN_REMOVE_DOCTOR: {
		doViewAllDoctors();
		input1 = makeInput(fx, fy, fw, fh, "Doctor ID to remove", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Remove Doctor", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doRemoveDoctor(); };
		break;
	}
	case Screen::ADMIN_REMOVE_PATIENT: {
		doViewAllPatients();
		input1 = makeInput(fx, fy, fw, fh, "Patient ID to remove", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Remove Patient", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doRemovePatient(); };
		break;
	}
	case Screen::ADMIN_ALL_PATIENTS:     doViewAllPatients();     break;
	case Screen::ADMIN_ALL_DOCTORS:      doViewAllDoctors();      break;
	case Screen::ADMIN_ALL_APPOINTMENTS: doViewAllAppointments(); break;
	case Screen::ADMIN_UNPAID_BILLS:     doViewUnpaidBills();     break;
	case Screen::ADMIN_SECURITY_LOG:     doViewSecurityLog();     break;
	case Screen::ADMIN_REPORT:           doGenerateReport();      break;
	case Screen::ADMIN_DISCHARGE: {
		input1 = makeInput(fx, fy, fw, fh, "Patient ID to discharge", fontRegular);
		actionBtn = makeButton(fx, fy + gap, fw, fh, "Discharge Patient", fontRegular, Colors::ACCENT, sf::Color(200, 130, 130), Colors::WHITE, 14);
		actionBtn.onClick = [this]() { doDischarge(); };
		break;
	}
	default: break;
	}
}

void App::doBookAppointment() {
	if (!loggedPatient) return;

	std::string spec = input1.value;
	std::string docIdStr = input2.value;
	std::string date = input3.value;
	std::string slot = input4.value;

	if (spec.empty() || docIdStr.empty() || date.empty() || slot.empty()) {
		toast.show("Please fill all fields.", true);
		return;
	}
	if (!Validator::validateDate(date.c_str())) {
		toast.show("Invalid date. Use DD-MM-YYYY.", true);
		return;
	}
	if (!Validator::validateTimeSlot(slot.c_str())) {
		toast.show("Invalid time slot.", true);
		return;
	}

	int docId = 0;
	for (char c : docIdStr) if (c >= '0' && c <= '9') docId = docId * 10 + (c - '0');

	Doctor* doc = hospital.getDoctors().findById(docId);
	if (!doc) {
		toast.show("Doctor not found.", true);
		return;
	}
	if (!compareStringInsensitive(doc->getSpecialization(), spec.c_str())) {
		toast.show("Doctor specialization does not match.", true);
		return;
	}

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getDoctorId() != docId) continue;
		if (!compareString(a.getDate(), date.c_str())) continue;
		if (!compareString(a.getTimeSlot(), slot.c_str())) continue;

		// strip \r \n from status
		const char* st = a.getStatus();
		char clean[20] = {};
		int ci = 0;
		for (int j = 0; st[j] != '\0' && ci < 19; j++) {
			if (st[j] != '\r' && st[j] != '\n' && st[j] != ' ')
				clean[ci++] = st[j];
		}

		if (compareString(clean, "pending")) {
			toast.show("Slot already taken. Choose another.", true);
			return;
		}
	}

	int newApptId = 1;
	for (int i = 0; i < appts.size(); i++)
		if (appts.getAll()[i].getId() >= newApptId)
			newApptId = appts.getAll()[i].getId() + 1;

	Appointment newAppt(newApptId, loggedPatient->getId(), docId, date.c_str(), slot.c_str(), "pending");
	appts.add(newAppt);
	FileHandler::appendAppointment(newAppt);

	Storage<Bill>& bills = hospital.getBills();
	int newBillId = 1;
	for (int i = 0; i < bills.size(); i++)
		if (bills.getAll()[i].getId() >= newBillId)
			newBillId = bills.getAll()[i].getId() + 1;

	Bill newBill(newBillId, loggedPatient->getId(), newApptId, doc->getfee(), "unpaid", date.c_str());
	bills.add(newBill);
	FileHandler::appendBill(newBill);

	toast.show("Appointment booked! ID: " + std::to_string(newApptId), false);
	clearInputs();
}

void App::doCancelAppointment() {
	if (!loggedPatient) return;

	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Appointment ID.", true);
		return;
	}

	int apptId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') apptId = apptId * 10 + (c - '0');

	Appointment* appt = hospital.getAppointments().findById(apptId);
	if (!appt || appt->getPatientId() != loggedPatient->getId() || !compareString(appt->getStatus(), "pending")) {
		toast.show("Invalid appointment ID.", true);
		return;
	}

	appt->setStatus("cancelled");
	FileHandler::updateAppointment(*appt);

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (b.getAppointmentId() == apptId) {
			if (compareString(b.getStatus(), "paid")) {
				Doctor* doc = hospital.getDoctors().findById(appt->getDoctorId());
				if (doc) {
					*loggedPatient += doc->getfee();
					FileHandler::updatePatient(*loggedPatient);
					toast.show("Cancelled. PKR " + std::to_string((int)doc->getfee()) + " refunded.", false);
				}
			}
			else {
				toast.show("Appointment cancelled. No charge was made.", false);
			}
			b.setStatus("cancelled");
			FileHandler::updateBill(b);
			break;
		}
	}
}

void App::doViewAppointments() {
	if (!loggedPatient) return;

	tableHeaders = { "ID", "Doctor", "Specialization", "Date", "Time", "Status" };
	tableData.clear();

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getPatientId() != loggedPatient->getId()) continue;
		Doctor* doc = hospital.getDoctors().findById(a.getDoctorId());
		tableData.push_back({
			std::to_string(a.getId()),
			doc ? doc->getName() : "Unknown",
			doc ? doc->getSpecialization() : "Unknown",
			a.getDate(), a.getTimeSlot(), a.getStatus()
			});
	}
}

void App::doViewRecords() {
	if (!loggedPatient) return;

	tableHeaders = { "Date", "Doctor", "Medicines", "Notes" };
	tableData.clear();

	Storage<Prescription>& pres = hospital.getPrescriptions();
	for (int i = 0; i < pres.size(); i++) {
		Prescription& pr = pres.getAll()[i];
		if (pr.getPatientId() != loggedPatient->getId()) continue;
		Doctor* doc = hospital.getDoctors().findById(pr.getDoctorId());
		tableData.push_back({
			pr.getDate(),
			doc ? doc->getName() : "Unknown",
			pr.getMedicines(),
			pr.getNotes()
			});
	}
}

void App::doViewBills() {
	if (!loggedPatient) return;

	tableHeaders = { "Bill ID", "Appt ID", "Amount", "Status", "Date" };
	tableData.clear();

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (b.getPatientId() != loggedPatient->getId()) continue;
		tableData.push_back({
			std::to_string(b.getId()),
			std::to_string(b.getAppointmentId()),
			"PKR " + std::to_string((int)b.getAmount()),
			b.getStatus(),
			b.getDate()
			});
	}
}

void App::doPayBill() {
	if (!loggedPatient) return;

	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Bill ID.", true);
		return;
	}

	int billId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') billId = billId * 10 + (c - '0');

	Bill* bill = hospital.getBills().findById(billId);
	if (!bill || bill->getPatientId() != loggedPatient->getId() || !compareString(bill->getStatus(), "unpaid")) {
		toast.show("Invalid bill ID.", true);
		return;
	}
	if (loggedPatient->getBalance() < bill->getAmount()) {
		toast.show("Insufficient balance.", true);
		return;
	}

	*loggedPatient -= bill->getAmount();
	bill->setStatus("paid");
	FileHandler::updateBill(*bill);
	FileHandler::updatePatient(*loggedPatient);
	toast.show("Bill paid! Balance: PKR " + std::to_string((int)loggedPatient->getBalance()), false);
	doViewBills();
}

void App::doTopUp() {
	if (!loggedPatient) return;

	std::string amtStr = input1.value;
	if (amtStr.empty()) {
		toast.show("Enter amount.", true);
		return;
	}

	double amount = 0.0;
	for (char c : amtStr) if (c >= '0' && c <= '9') amount = amount * 10 + (c - '0');

	if (!Validator::validatePositiveFloat(amount)) {
		toast.show("Invalid amount. Must be greater than 0.", true);
		return;
	}

	*loggedPatient += amount;
	FileHandler::updatePatient(*loggedPatient);
	toast.show("Balance updated! New balance: PKR " + std::to_string((int)loggedPatient->getBalance()), false);
}

void App::doViewToday() {
	if (!loggedDoctor) return;

	tableHeaders = { "ID", "Patient", "Time Slot", "Status" };
	tableData.clear();

	char today[11];
	time_t now = time(0);
	strftime(today, 11, "%d-%m-%Y", localtime(&now));

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getDoctorId() != loggedDoctor->getId()) continue;
		if (!compareString(a.getDate(), today)) continue;
		Patient* pat = hospital.getPatients().findById(a.getPatientId());
		tableData.push_back({
			std::to_string(a.getId()),
			pat ? pat->getName() : "Unknown",
			a.getTimeSlot(), a.getStatus()
			});
	}
}

void App::doMarkComplete() {
	if (!loggedDoctor) return;

	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Appointment ID.", true);
		return;
	}

	int apptId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') apptId = apptId * 10 + (c - '0');

	char today[11];
	time_t now = time(0);
	strftime(today, 11, "%d-%m-%Y", localtime(&now));

	Appointment* appt = hospital.getAppointments().findById(apptId);
	if (!appt || appt->getDoctorId() != loggedDoctor->getId() ||
		!compareString(appt->getStatus(), "pending") ||
		!compareString(appt->getDate(), today)) {
		toast.show("Invalid appointment ID.", true);
		return;
	}

	appt->setStatus("completed");
	FileHandler::updateAppointment(*appt);
	hospital.loadAllData();
	// re-grab the pointer after reload
	loggedDoctor = hospital.getDoctors().findById(loggedDoctor->getId());
	toast.show("Appointment marked as completed.", false);
	doViewToday();
}

void App::doMarkNoShow() {
	if (!loggedDoctor) return;

	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Appointment ID.", true);
		return;
	}

	int apptId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') apptId = apptId * 10 + (c - '0');

	char today[11];
	time_t now = time(0);
	strftime(today, 11, "%d-%m-%Y", localtime(&now));

	Appointment* appt = hospital.getAppointments().findById(apptId);
	if (!appt || appt->getDoctorId() != loggedDoctor->getId() ||
		!compareString(appt->getStatus(), "pending") ||
		!compareString(appt->getDate(), today)) {
		toast.show("Invalid appointment ID.", true);
		return;
	}

	appt->setStatus("no-show");
	FileHandler::updateAppointment(*appt);

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (b.getAppointmentId() == apptId) {
			b.setStatus("cancelled");
			FileHandler::updateBill(b);
			break;
		}
	}

	hospital.loadAllData();
	loggedDoctor = hospital.getDoctors().findById(loggedDoctor->getId());
	toast.show("Appointment marked as no-show.", false);
	doViewToday();
}

void App::doWritePrescription() {
	if (!loggedDoctor) return;

	std::string idStr = input1.value;
	std::string meds = input2.value;
	std::string notes = input3.value;

	if (idStr.empty() || meds.empty()) {
		toast.show("Fill Appointment ID and medicines.", true);
		return;
	}

	int apptId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') apptId = apptId * 10 + (c - '0');

	Appointment* appt = hospital.getAppointments().findById(apptId);
	if (!appt || appt->getDoctorId() != loggedDoctor->getId() || !compareString(appt->getStatus(), "completed")) {
		toast.show("Invalid or incomplete appointment.", true);
		return;
	}

	Storage<Prescription>& pres = hospital.getPrescriptions();
	for (int i = 0; i < pres.size(); i++) {
		if (pres.getAll()[i].getAppointmentId() == apptId) {
			toast.show("Prescription already written.", true);
			return;
		}
	}

	int newId = 1;
	for (int i = 0; i < pres.size(); i++)
		if (pres.getAll()[i].getId() >= newId)
			newId = pres.getAll()[i].getId() + 1;

	Prescription pr(newId, apptId, appt->getPatientId(), loggedDoctor->getId(), appt->getDate(), meds.c_str(), notes.c_str());
	pres.add(pr);
	FileHandler::appendPrescription(pr);
	hospital.loadAllData();
	loggedDoctor = hospital.getDoctors().findById(loggedDoctor->getId());
	toast.show("Prescription saved successfully.", false);
	clearInputs();
}

void App::doViewHistory() {
	if (!loggedDoctor) return;

	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Patient ID.", true);
		return;
	}

	int patId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') patId = patId * 10 + (c - '0');

	Patient* pat = hospital.getPatients().findById(patId);
	if (!pat) {
		toast.show("Patient not found.", true);
		return;
	}

	// reload data first to make sure we have fresh statuses from file
	int docId = loggedDoctor->getId();
	hospital.loadAllData();
	loggedDoctor = hospital.getDoctors().findById(docId);

	bool hasCompleted = false;
	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getPatientId() != patId || a.getDoctorId() != docId) continue;

		const char* st = a.getStatus();
		char clean[20] = {};
		int ci = 0;
		for (int j = 0; st[j] != '\0' && ci < 19; j++) {
			if (st[j] != '\r' && st[j] != '\n' && st[j] != ' ')
				clean[ci++] = st[j];
		}
		if (compareString(clean, "completed")) {
			hasCompleted = true;
			break;
		}
	}

	if (!hasCompleted) {
		toast.show("Access denied. No completed appointments with this patient.", true);
		return;
	}

	tableHeaders = { "Date", "Medicines", "Notes" };
	tableData.clear();

	Storage<Prescription>& pres = hospital.getPrescriptions();
	for (int i = 0; i < pres.size(); i++) {
		Prescription& pr = pres.getAll()[i];
		if (pr.getPatientId() == patId && pr.getDoctorId() == docId)
			tableData.push_back({ pr.getDate(), pr.getMedicines(), pr.getNotes() });
	}

	if (tableData.empty())
		toast.show("No prescriptions found for this patient.", true);
}

void App::doAddDoctor() {
	std::string name = input1.value, spec = input2.value;
	std::string cont = input3.value, pass = input4.value, feeStr = input5.value;

	if (name.empty() || spec.empty() || cont.empty() || pass.empty() || feeStr.empty()) {
		toast.show("Fill all fields.", true);
		return;
	}
	if (!Validator::validateContact(cont.c_str())) {
		toast.show("Contact must be exactly 11 digits.", true);
		return;
	}
	if (!Validator::validatePassword(pass.c_str())) {
		toast.show("Password must be at least 6 characters.", true);
		return;
	}

	double fee = 0.0;
	for (char c : feeStr) if (c >= '0' && c <= '9') fee = fee * 10 + (c - '0');
	if (!Validator::validatePositiveFloat(fee)) {
		toast.show("Invalid fee.", true);
		return;
	}

	Storage<Doctor>& docs = hospital.getDoctors();
	int newId = 1;
	for (int i = 0; i < docs.size(); i++)
		if (docs.getAll()[i].getId() >= newId)
			newId = docs.getAll()[i].getId() + 1;

	Doctor d(newId, name.c_str(), spec.c_str(), cont.c_str(), pass.c_str(), fee);
	docs.add(d);
	FileHandler::appendDoctor(d);
	toast.show("Doctor added! ID: " + std::to_string(newId), false);
	clearInputs();
}

void App::doRemoveDoctor() {
	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Doctor ID.", true);
		return;
	}

	int docId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') docId = docId * 10 + (c - '0');

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getDoctorId() == docId && compareString(a.getStatus(), "pending")) {
			toast.show("Cannot remove doctor with pending appointments.", true);
			return;
		}
	}

	hospital.getDoctors().removeById(docId);
	FileHandler::deleteDoctor(docId);
	toast.show("Doctor removed.", false);
	doViewAllDoctors();
}

void App::doAddPatient() {
	std::string name = input1.value, ageStr = input2.value;
	std::string gen = input3.value, cont = input4.value;
	std::string pass = input5.value, balStr = input6.value;

	if (name.empty() || ageStr.empty() || gen.empty() || cont.empty() || pass.empty() || balStr.empty()) {
		toast.show("Fill all fields.", true);
		return;
	}
	if (!Validator::validateContact(cont.c_str())) {
		toast.show("Contact must be exactly 11 digits.", true);
		return;
	}
	if (!Validator::validatePassword(pass.c_str())) {
		toast.show("Password must be at least 6 characters.", true);
		return;
	}
	if (!Validator::validateGender(gen[0])) {
		toast.show("Gender must be M or F.", true);
		return;
	}

	int age = 0;
	for (char c : ageStr) if (c >= '0' && c <= '9') age = age * 10 + (c - '0');
	if (!Validator::validateAge(age)) {
		toast.show("Invalid age.", true);
		return;
	}

	double bal = 0.0;
	for (char c : balStr) if (c >= '0' && c <= '9') bal = bal * 10 + (c - '0');

	Storage<Patient>& pats = hospital.getPatients();
	int newId = 1;
	for (int i = 0; i < pats.size(); i++)
		if (pats.getAll()[i].getId() >= newId)
			newId = pats.getAll()[i].getId() + 1;

	Patient p(newId, name.c_str(), age, gen[0], cont.c_str(), pass.c_str(), bal);
	pats.add(p);
	FileHandler::appendPatient(p);
	toast.show("Patient added! ID: " + std::to_string(newId), false);
	clearInputs();
}

void App::doRemovePatient() {
	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Patient ID.", true);
		return;
	}

	int patId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') patId = patId * 10 + (c - '0');

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getPatientId() == patId && compareString(a.getStatus(), "pending")) {
			toast.show("Cannot remove: patient has pending appointments.", true);
			return;
		}
	}

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (b.getPatientId() == patId && compareString(b.getStatus(), "unpaid")) {
			toast.show("Cannot remove: patient has unpaid bills.", true);
			return;
		}
	}

	hospital.getPatients().removeById(patId);
	FileHandler::deletePatient(patId);
	FileHandler::cascadeDeletePatient(patId);
	toast.show("Patient removed successfully.", false);
	doViewAllPatients();
}

void App::doViewAllPatients() {
	tableHeaders = { "ID", "Name", "Age", "Gender", "Contact", "Balance", "Unpaid Bills" };
	tableData.clear();

	Storage<Patient>& pats = hospital.getPatients();
	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < pats.size(); i++) {
		Patient& p = pats.getAll()[i];
		int unpaid = 0;
		for (int j = 0; j < bills.size(); j++) {
			if (bills.getAll()[j].getPatientId() == p.getId() && compareString(bills.getAll()[j].getStatus(), "unpaid"))
				unpaid++;
		}
		tableData.push_back({
			std::to_string(p.getId()), p.getName(),
			std::to_string(p.getAge()),
			std::string(1, p.getGender()),
			p.getContact(),
			"PKR " + std::to_string((int)p.getBalance()),
			std::to_string(unpaid)
			});
	}
}

void App::doViewAllDoctors() {
	tableHeaders = { "ID", "Name", "Specialization", "Contact", "Fee" };
	tableData.clear();

	Storage<Doctor>& docs = hospital.getDoctors();
	for (int i = 0; i < docs.size(); i++) {
		Doctor& d = docs.getAll()[i];
		tableData.push_back({
			std::to_string(d.getId()), d.getName(),
			d.getSpecialization(), d.getContact(),
			"PKR " + std::to_string((int)d.getfee())
			});
	}
}

void App::doViewAllAppointments() {
	tableHeaders = { "ID", "Patient", "Doctor", "Date", "Time", "Status" };
	tableData.clear();

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		Patient* pat = hospital.getPatients().findById(a.getPatientId());
		Doctor* doc = hospital.getDoctors().findById(a.getDoctorId());
		tableData.push_back({
			std::to_string(a.getId()),
			pat ? pat->getName() : "Unknown",
			doc ? doc->getName() : "Unknown",
			a.getDate(), a.getTimeSlot(), a.getStatus()
			});
	}
}

void App::doViewUnpaidBills() {
	tableHeaders = { "Bill ID", "Patient", "Amount", "Date" };
	tableData.clear();

	char today[11];
	time_t now = time(0);
	strftime(today, 11, "%d-%m-%Y", localtime(&now));

	int ty = (today[6] - '0') * 1000 + (today[7] - '0') * 100 + (today[8] - '0') * 10 + (today[9] - '0');
	int tm = (today[3] - '0') * 10 + (today[4] - '0');
	int td = (today[0] - '0') * 10 + (today[1] - '0');

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (!compareString(b.getStatus(), "unpaid")) continue;

		Patient* pat = hospital.getPatients().findById(b.getPatientId());
		const char* d = b.getDate();
		int by = (d[6] - '0') * 1000 + (d[7] - '0') * 100 + (d[8] - '0') * 10 + (d[9] - '0');
		int bm = (d[3] - '0') * 10 + (d[4] - '0');
		int bd = (d[0] - '0') * 10 + (d[1] - '0');
		int diff = (ty * 365 + tm * 30 + td) - (by * 365 + bm * 30 + bd);

		std::string dateStr = std::string(d) + (diff > 7 ? " [OVERDUE]" : "");
		tableData.push_back({
			std::to_string(b.getId()),
			pat ? pat->getName() : "Unknown",
			"PKR " + std::to_string((int)b.getAmount()),
			dateStr
			});
	}
}

void App::doDischarge() {
	std::string idStr = input1.value;
	if (idStr.empty()) {
		toast.show("Enter Patient ID.", true);
		return;
	}

	int patId = 0;
	for (char c : idStr) if (c >= '0' && c <= '9') patId = patId * 10 + (c - '0');

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (b.getPatientId() == patId && compareString(b.getStatus(), "unpaid")) {
			toast.show("Cannot discharge: unpaid bills exist.", true);
			return;
		}
	}

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (a.getPatientId() == patId && compareString(a.getStatus(), "pending")) {
			toast.show("Cannot discharge: pending appointments exist.", true);
			return;
		}
	}

	FileHandler::archivePatient(patId, hospital.getPatients(), hospital.getAppointments(), hospital.getBills(), hospital.getPrescriptions());
	hospital.getPatients().removeById(patId);
	FileHandler::deletePatient(patId);
	FileHandler::cascadeDeletePatient(patId);
	toast.show("Patient discharged and archived successfully.", false);
}

void App::doViewSecurityLog() {
	tableHeaders = { "Timestamp", "Role", "ID Entered", "Result" };
	tableData.clear();

	std::ifstream file("security_log.txt");
	if (!file.is_open()) {
		toast.show("No security log found.", true);
		return;
	}

	char line[300];
	while (file.getline(line, 300)) {
		if (line[0] == '\0') continue;
		std::vector<std::string> row;
		std::string cell;
		for (int i = 0; line[i] != '\0'; i++) {
			if (line[i] == ',') { row.push_back(cell); cell = ""; }
			else cell += line[i];
		}
		row.push_back(cell);
		while (row.size() < 4) row.push_back("");
		tableData.push_back(row);
	}
	file.close();
}

void App::doGenerateReport() {
	tableHeaders = { "Metric", "Value" };
	tableData.clear();

	char today[11];
	time_t now = time(0);
	strftime(today, 11, "%d-%m-%Y", localtime(&now));

	int pending = 0, completed = 0, noshow = 0, cancelled = 0;
	double revenue = 0.0;

	Storage<Appointment>& appts = hospital.getAppointments();
	for (int i = 0; i < appts.size(); i++) {
		Appointment& a = appts.getAll()[i];
		if (!compareString(a.getDate(), today)) continue;
		if (compareString(a.getStatus(), "pending"))   pending++;
		if (compareString(a.getStatus(), "completed")) completed++;
		if (compareString(a.getStatus(), "no-show"))   noshow++;
		if (compareString(a.getStatus(), "cancelled")) cancelled++;
	}

	Storage<Bill>& bills = hospital.getBills();
	for (int i = 0; i < bills.size(); i++) {
		Bill& b = bills.getAll()[i];
		if (compareString(b.getStatus(), "paid") && compareString(b.getDate(), today))
			revenue += b.getAmount();
	}

	tableData.push_back({ "Date", today });
	tableData.push_back({ "Total Appointments Today", std::to_string(pending + completed + noshow + cancelled) });
	tableData.push_back({ "Pending",   std::to_string(pending) });
	tableData.push_back({ "Completed", std::to_string(completed) });
	tableData.push_back({ "No-Show",   std::to_string(noshow) });
	tableData.push_back({ "Cancelled", std::to_string(cancelled) });
	tableData.push_back({ "Revenue Collected Today", "PKR " + std::to_string((int)revenue) });
}