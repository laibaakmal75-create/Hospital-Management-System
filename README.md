# MediCore — Hospital Management System

A C++ Hospital Management System built using Object Oriented Programming principles. MediCore supports three user roles — **Patient**, **Doctor**, and **Admin** and manages appointments, prescriptions, billing, and medical records. The UI is built with **SFML 2.6.2**.

---

## Features

- Patient: Book/cancel appointments, view medical records, pay bills, top up balance
- Doctor: View today's appointments, mark complete/no-show, write prescriptions
- Admin: Manage doctors and patients, view reports, discharge patients, monitor security log
- All data persists across sessions via `.txt` files
- Graphical interface built with SFML using a custom color palette and Comfortaa font

---

## Requirements

- Visual Studio 2019 or later (Windows, x64)
- [SFML 2.6.2](https://www.sfml-dev.org/download.php) — 64-bit version
- Comfortaa font (Regular + Bold) from [Google Fonts](https://fonts.google.com/specimen/Comfortaa)

---

## How to Compile

**1. Link SFML in Visual Studio**

Right click project → Properties → set Configuration to `All Configurations`, Platform to `x64`

- **C/C++ → General → Additional Include Directories:** `C:\SFML\include`
- **Linker → General → Additional Library Directories:** `C:\SFML\lib`
- **Linker → Input → Additional Dependencies:** add these three:
```
sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
```

**2. Copy DLL files**

Copy all `.dll` files from `C:\SFML\bin` into the project source folder.

**3. Copy font files**

Place `Comfortaa-Regular.ttf` and `Comfortaa-Bold.ttf` in the project source folder.

**4. Build**

Open `oop project.sln` in Visual Studio and press `Ctrl+Shift+B`.

---

## How to Run

**1. Set up data files**

Create the following `.txt` files in the project source folder (same folder as `.cpp` files):

| File | Initial Content |
|------|----------------|
| `admin.txt` | `1,Admin,admin123` |
| `patients.txt` | `1,Ahmed Ali,28,M,03001234567,pass123,5000.00` |
| `doctors.txt` | `1,Dr. Sara Khan,Cardiology,03111234567,doc456,1500.00` |
| `appointments.txt` | *(empty)* |
| `bills.txt` | *(empty)* |
| `prescriptions.txt` | *(empty)* |
| `security_log.txt` | *(empty)* |
| `discharged.txt` | *(empty)* |

**2. Run**

Press `Ctrl+F5` in Visual Studio. The SFML window will open.

**3. Login credentials (sample data)**

| Role | ID | Password |
|------|----|----------|
| Admin | 1 | admin123 |
| Patient | 1 | pass123 |
| Doctor | 1 | doc456 |

---

## Project Structure

```
├── main.cpp
├── App.h / App.cpp          — SFML UI and all screens
├── UI.h / UI.cpp            — Reusable UI components
├── HospitalSystem.h / .cpp  — Core system logic
├── FileHandler.h / .cpp     — File I/O for all entities
├── Validator.h / .cpp       — Input validation
├── Storage.h                — Generic template storage class
├── Person.h / .cpp          — Abstract base class
├── Patient.h / .cpp
├── Doctor.h / .cpp
├── Admin.h / .cpp
├── Appointment.h / .cpp
├── Bill.h / .cpp
├── Prescription.h / .cpp
├── HospitalException.h / .cpp
└── helper.h                 — String utility functions
```

---

## Notes

- `std::string`, `std::vector`, `strcmp()` are only used in SFML files, prohibited in backend
- All sorting is implemented manually
- All string operations use custom `copyString()` and `compareString()` from `helper.h`
