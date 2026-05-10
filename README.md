# MediCore

## Overview
MediCore is a fully functional Hospital Management System built with **C++** and a **SFML Graphical Interface**, serving as my Final Term Project for the OOP course. The system supports three roles — **Patient**, **Doctor**, and **Admin** — and manages appointments, bills, prescriptions, medical records, and security. All persistent data is stored and read from `.txt` files.

---

## Features

| Role | Capabilities |
|------|-------------|
| **Patient** | Book/Cancel Appointments, Pay Bills, View Medical Records, Top Up Balance |
| **Doctor** | View Today's Appointments, Mark Complete/No-Show, Write Prescriptions, View Patient History |
| **Admin** | Add/Remove Doctors & Patients, Discharge Patients, View All Data, Generate Daily Report, View Security Log |

> After **3 consecutive failed login attempts**, the account is locked and the event is logged to `security_log.txt`.

---

## Dependencies
- **C++** (Visual Studio 2019 or later, Windows x64)
- **SFML 2.6.2** — Download from [sfml-dev.org](https://www.sfml-dev.org/download.php)
- **Comfortaa Font** — Download from [Google Fonts](https://fonts.google.com/specimen/Comfortaa)

---

## How to Compile & Run

**Visual Studio (Windows) — Recommended**

1. Open the solution file `oop project.sln`
2. Set Configuration to `All Configurations` and Platform to `x64`
3. Link SFML via Project → Properties:
   - **C/C++ → Additional Include Directories** → add `SFML/include` path
   - **Linker → Additional Library Directories** → add `SFML/lib` path
   - **Linker → Input → Additional Dependencies** → add:
     ```
     sfml-graphics-d.lib
     sfml-window-d.lib
     sfml-system-d.lib
     ```
4. Copy all SFML `.dll` files into the project source folder
5. Place `Comfortaa-Regular.ttf` and `Comfortaa-Bold.ttf` in the same folder
6. Create the required `.txt` data files in the project source folder (see below)
7. Press `Ctrl+F5` to build and run

---

## Required Data Files

Place these in the same directory as your source files before running:

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

---

## Additional Notes
- `std::string` and all built-in string functions (`strcmp`, `strtok`, etc.) are **not used** in any backend logic due to project constraints they are only used in SFML/GUI code
- `std::vector` is likewise **only used** in the GUI implementation as permitted
- There are **no memory leaks** in the program
- All sorting is implemented manually no library sort functions used
