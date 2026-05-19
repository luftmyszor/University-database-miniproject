 # University Database — Project Documentation

 ## Table of contents
 - [Overview](#overview)
 - [Quick start](#quick-start)
   - [Build](#build)
   - [Run](#run)
 - [Project layout](#project-layout)
 - [Data file formats](#data-file-formats)
 - [File-by-file reference](#file-by-file-reference)
   - [Entry point](#entry-point)
   - [CLI and orchestration](#cli-and-orchestration)
   - [Authentication and accounts](#authentication-and-accounts)
   - [Persistence and data helpers](#persistence-and-data-helpers)
   - [Domain models](#domain-models)
   - [Utilities](#utilities)
 - [Runtime flow & communication](#runtime-flow--communication)
   - [Startup sequence](#startup-sequence)
   - [Sign in / Sign up flows](#sign-in--sign-up-flows)
   - [Admin / Teacher / Student workflows](#admin--teacher--student-workflows)
   - [Exit & persistence behavior](#exit--persistence-behavior)
 - [Notes, limitations & suggestions](#notes-limitations--suggestions)
 - [Extending the project](#extending-the-project)
 - [Appendix: example interactions](#appendix-example-interactions)
 - [Contact / Contributing](#contact--contributing)

 ---

 ## Overview

 A compact command-line application to manage a small university-style dataset: students, teachers and grades. It uses file-based persistence and role-based authentication (admin, teacher, student). The project aims to be as object oriented as possible, while maintaining simplicity and clarity.

 Key goals:
 - Clear, numeric CLI menus for common tasks.
 - Simple file persistence in `data/` (human-readable text formats).
 - Minimal domain models (`Person`, `Student`, `Teacher`, `Grade`).
 - Central CLI orchestration in `CLIManager`.


 ## Quick start

 ### Build

 From the project root (Windows PowerShell):

 ```powershell
 g++ -std=c++17 -g main.cpp src/*.cpp -o program.exe
 ```

 ### Run

 ```powershell
 .\program.exe
 ```

 When running, the main menu provides options to sign in, sign up, or exit. Submenus support a quick exit (`0`) which immediately ends the program; the main menu `3` saves data and quits.


 ## Project layout

 Top-level:
 - `main.cpp` — program entry
 - `README.md` — project readme and usage
 - `documentation.md` — this file
 - `data/` — persistent data files
 - `src/` — implementation and headers

 `data/` contains:
 - `accounts.txt` — user accounts
 - `students.txt` — student records
 - `teachers.txt` — teacher records
 - `grades.txt` — grades records
 - `subjects.txt` — subjects records
 - `courses.txt` — courses records

 `src/` contains source and header files described in detail below.


 ## Data file formats

 - `data/accounts.txt`
   - Each line: `type;login;passwordHash`
   - `type`: `0` = admin, `1` = teacher, `2` = student
   - Example: `0;admin;c991af`

 - `data/students.txt`
   - Format (as produced by `Student::toString()`):
     `login;name;surname;day;month;year;pesel;grade1,grade2,...`
   - `gradeN` format (from `Grade::toString()`): `subject:value:teacherLogin`
   - Example: `s123;Alice;Smith;1;1;2000;01234567890;Math:4.5:teacher1,CS:5:teacher1`

 - `data/teachers.txt`
   - Format (as produced by `Teacher::toString()`):
     `login;name;surname;day;month;year;pesel;subj1,subj2,...`
   - Example: `t1;John;Doe;5;5;1980;01234567890;Math,CS`

 Notes:
 - Parsers are implemented in `Student::fromString()` and `Teacher::fromString()`.
 - Missing files are tolerated on startup; repositories are populated only when files exist.


 ## File-by-file reference

 This section lists each important file, its responsibilities, and key APIs.

 ### Entry point

 `main.cpp`
 - Constructs `CLIManager` and enters a loop calling `CLI.CLI()` while `CLI.isRunning()` returns `true`.
 - Minimal — orchestration delegated to `CLIManager`.

 ### CLI and orchestration

 `src/CLIManager.h` / `src/CLIManager.cpp`
 - Central CLI; holds in-memory repositories for `Student` and `Teacher` using `Repository<T>`.
 - Member fields:
   - `Repository<Student> students;`
   - `Repository<Teacher> teachers;`
   - `Repository<Course> courses;`
   - `AuthManager auth` (loads users on construction)
   - `_running` flag to control main loop
 - Main responsibilities:
   - Print main menu and handle options (Sign in, Sign up, Exit)
   - Dispatch to `adminMenu()`, `teacherMenu()`, `studentMenu()` depending on authenticated user type.
   - Load repositories from `data/*.txt` on construction (calls `Repository::loadFromFile()` with appropriate parsers).
   - Save repositories on exit via `Repository::saveToFile()` (admin Exit path) or when explicitly invoked.
 - Submenus:
   - `adminMenu()` — many admin actions (list/create/save/sort students, list teachers, polymorphic view)
   - `teacherMenu()` — view personal info, assign grades
   - `studentMenu()` — view info and grades
 - Input helper: `getOption()` reads integer options robustly (returns -1 on invalid input).


 ### Authentication and accounts

 `src/AuthManager.h` / `src/AuthManager.cpp`
 - Static in-memory map `_users` keyed by `login` storing `Account` objects.
 - Files:
   - `ACCOUNTS_FILE` = `data/accounts.txt`
 - APIs:
   - `loadUsers()` — read `ACCOUNTS_FILE` via `DataManager::readFromFile()` and populate `_users`.
   - `saveUsers()` — serialize `_users` back to `ACCOUNTS_FILE`.
   - `signIn()` — prompt for login and password, validate hash and return `Account`.
   - `signUp()` — prompt for new account details and add to `_users`.
 - Behavior:
   - On first run (no users), a default admin account `admin/admin` is created and saved.

 `src/Account.h` / `src/Account.cpp`
 - Fields: `_type`, `_login`, `_passwordHash`.
 - `Account::hash()` implements a simple rolling hash producing a hex string (NOT cryptographically secure).
 - Accessors: `getLogin()`, `getType()`, `getPasswordHash()`, `getTypeStr()`.


 ### Persistence and data helpers

 `src/DataManager.h` / `src/DataManager.cpp`
 - `readFromFile(filename)` -> `std::vector<std::string>` lines; returns empty vector on missing file.
 - `saveToFile(filename, lines)` -> writes lines to disk and throws on open failure.
 - Used by `AuthManager` and can be used by other managers.

 `src/Repository.h`
 - Template, header-only class `Repository<T>` providing:
   - `add(const T &)`
   - `find_if(pred)`
   - `remove_if(pred)`
   - `getAll()` returns `std::vector<T>&`
   - `sortBy(cmp)`
   - `loadFromFile(filename, parser)` — reads lines and calls `parser(line)` to produce `T` items (throws on open failure)
   - `saveToFile(filename, serializer)` — writes serialized items
 - `Student` and `Teacher` data are loaded/saved via this template.


 ### Domain models

 `src/Person.h` / `src/Person.cpp`
 - Base class storing: `name`, `surname`, `dayOfBirth`, `monthOfBirth`, `yearOfBirth`, `peselNumber`.
 - Helpers: `toString()` (semicolon-separated), `display()`, input helpers.

 `src/Student.h` / `src/Student.cpp`
 - Inherits `Person`, adds `login` and `grades` (vector<`Grade`)`
 - `average()` computes arithmetic mean of `Grade::value`.
 - Serialization: `toString()` and `static fromString()` (parses grades as comma-separated `subject:value:teacher` items).

 `src/Teacher.h` / `src/Teacher.cpp`
 - Inherits `Person`, adds `login` and `subjects` (vector<string>).
 - `assignGrade(Student &student, subject, value)` constructs a `Grade` and calls `student.addGrade()`.
 - Serialization: `toString()` and `static fromString()`.

 `src/Grade.h`
 - Simple POD-like `struct Grade { string subject; double value; string teacherLogin; }`.
 - `toString()` serializes as `subject:value:teacherLogin` and `fromString()` parses it.


 ### Utilities

 `src/Utils.h`
 - `printRange(begin, end, sep)` prints a range with a separator (used by `CLIManager` for listing subjects, teachers, etc.).


 ## Runtime flow & communication

 This section explains how modules interact at runtime.

 ### Startup sequence
 1. `main()` constructs `CLIManager`.
 2. `CLIManager` constructs its `AuthManager` member which calls `AuthManager::loadUsers()`.
    - `AuthManager::loadUsers()` uses `DataManager::readFromFile("data/accounts.txt")` and populates `_users`.
    - If no users are found, a default admin account is created and saved.
 3. `CLIManager` attempts to load `students` and `teachers` via `Repository::loadFromFile()` using `Student::fromString` and `Teacher::fromString` as parsers. Missing files are handled and ignored.
 4. `main()` enters the main loop calling `CLI.CLI()` while `CLI.isRunning()` is true.


 ### Sign in / Sign up flows
 - Sign up (`auth.signUp()`): prompts for `login`, `password`, and `type` (1-admin / 2-teacher / 3-student), creates an `Account`, stores it in `_users`, and calls `saveUsers()`.
 - Sign in (`auth.signIn()`): prompts for credentials, validates `Account::hash(password)` against the stored hash, and returns the `Account`.
 - After sign in, `CLIManager` dispatches to the appropriate submenu based on `Account::getType()`.


 ### Admin / Teacher / Student workflows
 - `adminMenu()` operates on `students` and `teachers` repositories in memory:
   - List students/teachers, create student/teacher records (adds to in-memory repo), save data (calls `Repository::saveToFile()`), sort students, show polymorphic person list, and list unique subjects.
 - `teacherMenu()` finds the logged-in `Teacher` in `teachers` and allows assigning grades to `Student` objects by login. `Teacher::assignGrade()` calls `Student::addGrade()`.
 - `studentMenu()` finds the logged-in `Student` and shows info and grades.

 Important: Grade addition modifies the in-memory `students` repository; to persist grades you must call `students.saveToFile()` (e.g., via admin Save data or main-menu Exit).


 ### Exit & persistence behavior
 - Main menu option `3` (Exit) performs:
   - `students.saveToFile("data/students.txt", serializer)`
   - `teachers.saveToFile("data/teachers.txt", serializer)`
   - `AuthManager::saveUsers()`
   - sets `_running = false`
 - Submenu quick-exit `0` sets `_running = false` and returns immediately (it does not automatically save repositories). If you want to ensure persistent saves from any exit path, call save routines before quitting or centralize saving.


 ## Notes, limitations & suggestions

 - Password hashing: `Account::hash()` is a simple custom rolling hash.
 - Error handling: file I/O errors are sometimes caught and ignored; consider logging or surfacing important failures.
 - Concurrency: no file-locking or transactional guarantees — avoid multiple processes writing the same files concurrently.
 - Unused files: `data/grades.txt` and `data/subjects.txt` exist but are unused — remove them or implement functionality that consumes them.
 - Persistence on submenu exits: submenu `0` exit may lead to data loss if changes weren't saved; consider saving on every mutating operation or on any program termination.


 ## Appendix: example interactions

 Main menu (sample):
 ```
 ========================================
   Welcome to University Database CLI v1.0
   Manage students, courses and grades
   Roles: admin, teacher, student
   Short guide: choose the option number then press Enter
   Tip: in any submenu you can press 0 to exit the program immediately
   Tip: default admin account exists: login 'admin' / password 'admin'
 ========================================
 1) Sign in
 2) Sign up
 3) Exit (save & quit)
 > 
 ```

 Admin submenu (sample):
 ```
 --- ADMIN MENU ---
 0 - Exit program
 1 - List students
 2 - List teachers
 3 - Create student record
 4 - Create teacher record
 5 - Save data
 6 - Sort students by average
 7 - Sign out
 8 - Show all people (polymorphic view)
 9 - List unique subjects
 10 - Create a course
 ```


 ## Contact / Contributing

 - To report bugs or propose features, open an issue or a pull request.
 - For changes that affect data formats, increment a data-format version and document migration steps.

 ---

 *Generated by the project maintainer assistant — `documentation.md` created at project root.*
