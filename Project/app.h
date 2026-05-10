#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "UI.h"
#include "HospitalSystem.h"

enum class Screen {
    LOGIN,
    PATIENT_MENU,
    PATIENT_BOOK,
    PATIENT_CANCEL,
    PATIENT_APPOINTMENTS,
    PATIENT_RECORDS,
    PATIENT_BILLS,
    PATIENT_PAY,
    PATIENT_TOPUP,
    DOCTOR_MENU,
    DOCTOR_TODAY,
    DOCTOR_COMPLETE,
    DOCTOR_NOSHOW,
    DOCTOR_PRESCRIPTION,
    DOCTOR_HISTORY,
    ADMIN_MENU,
    ADMIN_ADD_DOCTOR,
    ADMIN_REMOVE_DOCTOR,
    ADMIN_ADD_PATIENT,
    ADMIN_REMOVE_PATIENT,
    ADMIN_ALL_PATIENTS,
    ADMIN_ALL_DOCTORS,
    ADMIN_ALL_APPOINTMENTS,
    ADMIN_UNPAID_BILLS,
    ADMIN_DISCHARGE,
    ADMIN_SECURITY_LOG,
    ADMIN_REPORT
};

enum class UserRole { NONE, PATIENT, DOCTOR, ADMIN };

class App {
    // Window
    sf::RenderWindow window;
    sf::Font fontRegular;
    sf::Font fontBold;

    // State
    Screen currentScreen;
    UserRole currentRole;
    HospitalSystem hospital;

    // Logged in user
    Patient* loggedPatient;
    Doctor* loggedDoctor;

    // Login screen
    TextInput loginIdInput;
    TextInput loginPassInput;
    Button    loginBtn;
    Button    rolePatientBtn;
    Button    roleDoctorBtn;
    Button    roleAdminBtn;
    UserRole  selectedRole;
    int       loginAttempts;
    bool      accountLocked;

    // Sidebar buttons (patient)
    std::vector<Button> patientSidebarBtns;
    std::vector<Button> doctorSidebarBtns;
    std::vector<Button> adminSidebarBtns;

    // Back button
    Button backBtn;

    // Toast
    Toast toast;

    // Scroll
    float scrollOffset;

    // Form inputs reused across screens
    TextInput input1, input2, input3, input4, input5, input6;
    Button    actionBtn;

    // Table data for display screens
    std::vector<std::vector<std::string>> tableData;
    std::vector<std::string> tableHeaders;

public:
    App();
    void run();

private:
    // Core
    void init();
    void handleEvents();
    void update();
    void render();

    // Screen builders
    void buildLoginScreen();
    void buildPatientSidebar();
    void buildDoctorSidebar();
    void buildAdminSidebar();

    // Screen renderers
    void renderLogin();
    void renderSidebar();
    void renderPatientMenu();
    void renderDoctorMenu();
    void renderAdminMenu();
    void renderTable();
    void renderForm();

    // Actions — Patient
    void doPatientLogin();
    void doBookAppointment();
    void doCancelAppointment();
    void doViewAppointments();
    void doViewRecords();
    void doViewBills();
    void doPayBill();
    void doTopUp();

    // Actions — Doctor
    void doDoctorLogin();
    void doViewToday();
    void doMarkComplete();
    void doMarkNoShow();
    void doWritePrescription();
    void doViewHistory();

    // Actions — Admin
    void doAdminLogin();
    void doAddDoctor();
    void doRemoveDoctor();
    void doAddPatient();
    void doRemovePatient();
    void doViewAllPatients();
    void doViewAllDoctors();
    void doViewAllAppointments();
    void doViewUnpaidBills();
    void doDischarge();
    void doViewSecurityLog();
    void doGenerateReport();

    // Helpers
    void switchScreen(Screen s);
    void logout();
    void drawBackground();
    void drawCard(float x, float y, float w, float h);
    void drawSidebarPanel();
    sf::Text makeLabel(const std::string& s, int size,
        sf::Color c, float x, float y);
    void clearInputs();
};