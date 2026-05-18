Project: University Database CLI

Purpose
-------
This document explains the purpose and structure of the project, describes every file in the repository and details how modules communicate at runtime. It is intended for a developer who wants to understand, maintain, or extend the code.

Quick summary
-------------
- A small console (CLI) application to manage a simple university database: students, teachers, and grades.
- Authentication supports three roles: `ADMIN`, `TEACHER`, `STUDENT`.
- Persistence is file-based (plain text files in `data/`).
- Entry point: `main.cpp` which delegates to `CLIManager` for the interactive UI.

Build & run
-----------
From the project root (Windows PowerShell example):

```powershell
g++ -std=c++17 -g main.cpp src/*.cpp -o program.exe
.\program.exe
```

Overview of repository layout
-----------------------------
- `main.cpp` — program entry point.
- `README.md` — project README and run instructions.
- `documentation` — this file (project documentation).
- `LICENSE` — project license.
- `program.exe` — compiled binary (not source-controlled normally).
- `data/` — persistent data files (accounts, students, teachers, grades, subjects).
  - `accounts.txt`, `students.txt`, `teachers.txt`, `grades.txt`, `subjects.txt`.
- `src/` — C++ source files and headers:
  - `Account.h`, `Account.cpp`
  - `AuthManager.h`, `AuthManager.cpp`
  - `CLIManager.h`, `CLIManager.cpp`
  - `DataManager.h`, `DataManager.cpp`
  - `Repository.h` (template header-only)
  - `Person.h`, `Person.cpp`
  - `Student.h`, `Student.cpp`
  - `Teacher.h`, `Teacher.cpp`
  - `Grade.h`
  - `Utils.h`

Data file formats
-----------------
- `data/accounts.txt` — user accounts. Each line: `type;login;passwordHash`
  - `type` is `0` (admin), `1` (teacher), `2` (student).
  - Example: `0;admin;c991af`
  - Managed by `AuthManager`.

- `data/students.txt` — student records. Each line produced by `Student::toString()` and parsed by `Student::fromString()`.
  - Format: `login;name;surname;day;month;year;pesel;grade1,grade2,...`
  - Each grade uses `Grade::toString()`: `subject:value:teacherLogin` and grade entries are comma-separated.
  - Example: `s123;Alice;Smith;1;1;2000;01234567890;Math:4.5:teacher1,CS:5:teacher1`

- `data/teachers.txt` — teacher records. Each line produced by `Teacher::toString()` and parsed by `Teacher::fromString()`.
  - Format: `login;name;surname;day;month;year;pesel;subj1,subj2,...`
  - Example: `t1;John;Doe;5;5;1980;01234567890;Math,CS`

- `data/grades.txt` and `data/subjects.txt` are present but not currently used by the code (reserved for future features).

File-by-file description (source)
---------------------------------
Each file's role, key functions, and interactions are listed below.

`main.cpp`
- Constructs a `CLIManager` and runs its `.CLI()` loop while `CLIManager::isRunning()` returns `true`.
- Very small; delegates the program control flow to `CLIManager`.

`src/CLIManager.h` / `src/CLIManager.cpp`
- Central interactive UI and application state.
- Holds in-memory repositories:
  - `Repository<Student> students;`
  - `Repository<Teacher> teachers;`
- Owns `AuthManager auth` (instantiated), which loads users on construction.
- Constructor attempts to load `data/students.txt` and `data/teachers.txt` via `Repository::loadFromFile()`.
- Main menu (state `START`) shows:
  - `1) Sign in` — calls `auth.signIn()` and dispatches to role-specific menus.
  - `2) Sign up` — calls `auth.signUp()`.
  - `3) Exit (save & quit)` — saves `students` and `teachers` to disk and calls `AuthManager::saveUsers()` before setting `_running = false`.
  - Guidance and tips are printed at startup (greeting).
- Submenus: `adminMenu()`, `teacherMenu()`, `studentMenu()`.
  - Each submenu defines options, reads user input via `getOption()` and acts accordingly.
  - Important: each submenu now includes `0 - Exit program` which sets `_running = false` and returns — this terminates the application immediately.
- Admin actions include listing records, creating person records, saving data, sorting students, showing polymorphic views, and listing unique subjects.
- Teacher actions include viewing personal info and assigning grades to students. When assigning a grade, `Teacher::assignGrade()` constructs a `Grade` and calls `Student::addGrade()` to append it.
- Student actions include viewing personal info and grades.

`src/AuthManager.h` / `src/AuthManager.cpp`
- Responsible for loading and saving user accounts and providing sign-in/sign-up operations.
- Static member: `std::unordered_map<std::string, Account> _users;` stores accounts in memory keyed by login.
- `loadUsers()` reads `data/accounts.txt` (via `DataManager::readFromFile`) and populates `_users`.
  - If no users are found, a default admin account `admin/admin` is created and saved.
- `saveUsers()` serializes `_users` to `data/accounts.txt` using `DataManager::saveToFile()`.
- `signUp()` prompts for `login`, `password`, and `type` (1-admin,2-teacher,3-student), normalizes the type and creates an `Account`.
- `signIn()` prompts for `login` and `password`, verifies `Account::hash(password)` matches stored hash and returns the `Account` on success.

`src/Account.h` / `src/Account.cpp`
- Lightweight struct-like class representing a user account.
- Fields: `_type` (enum: `ADMIN`, `TEACHER`, `STUDENT`), `_login`, `_passwordHash`.
- `Account::hash()` implements a simple polynomial rolling hash over lowercase letters and returns a hexadecimal string.
  - Note: This is NOT cryptographically secure — see "Security notes" below.
- Provides `getType()`, `getLogin()`, `getPasswordHash()`, and `getTypeStr()` helpers.

`src/DataManager.h` / `src/DataManager.cpp`
- Small helper to read/write plain text files as vector<string> lines.
- `readFromFile()` returns an empty vector if the file cannot be opened (missing file is treated as empty).
- `saveToFile()` writes given lines to a file and throws on failure to open.
- Used by `AuthManager` to persist accounts.

`src/Repository.h` (header-only template)
- Generic in-memory container for T items.
- API highlights:
  - `add(const T &)` — append item.
  - `find_if(pred)` — returns iterator to matching item.
  - `remove_if(pred)` — erase items.
  - `getAll()` — access underlying vector.
  - `sortBy(cmp)` — sort with comparator.
  - `loadFromFile(filename, parser)` — open `filename`, read lines, parse each line with `parser` (std::function<T(const std::string &)>), push parsed T into `items`.
    - Throws `runtime_error` if file cannot be opened. Callers often wrap this in try/catch.
  - `saveToFile(filename, serializer)` — write each serialized item string using `serializer` (std::function<std::string(const T &)>).
- `Student` and `Teacher` repositories are loaded/saved using this mechanism.

`src/Person.h` / `src/Person.cpp`
- Base class for `Student` and `Teacher`.
- Stores personal data: `name`, `surname`, `dayOfBirth`, `monthOfBirth`, `yearOfBirth`, `peselNumber`.
- Helper input methods and getters, `toString()` returns a semicolon-separated representation, `display()` prints readable info.

`src/Grade.h`
- Plain struct representing a grade with `subject`, `value`, and `teacherLogin`.
- `toString()` serializes as `subject:value:teacherLogin`.
- `fromString()` parses that format.

`src/Student.h` / `src/Student.cpp`
- Extends `Person` and adds:
  - `login` (string) used to link user account -> person record.
  - `grades` (vector<Grade>).
- `average()` computes average grade value (0.0 if no grades).
- `toString()` / `fromString()` pair define the file format for `students.txt`.
- `display()` prints the student info, grades and average.

`src/Teacher.h` / `src/Teacher.cpp`
- Extends `Person` and adds:
  - `login` (string) and `subjects` (vector<string>).
- `assignGrade(Student &student, const std::string &subject, double value)` constructs `Grade(subject, value, login)` and calls `student.addGrade()`.
- `toString()` / `fromString()` pair define the file format for `teachers.txt`.

`src/Utils.h`
- Small utility template function `printRange(begin, end, sep)` that prints range elements with a separator.
- Used by `CLIManager` for printing lists.

Runtime communication & data flow
--------------------------------
This section describes what happens at startup and during typical user actions.

Startup sequence
- `main()` constructs `CLIManager CLI` and enters a loop calling `CLI.CLI()` while `CLI.isRunning()`.
- `CLIManager` constructor:
  - Constructs the `AuthManager auth` member which calls `AuthManager::loadUsers()` to populate in-memory accounts (from `data/accounts.txt`) using `DataManager`.
  - Attempts to load `students` and `teachers` repositories from `data/students.txt` and `data/teachers.txt` using `Repository::loadFromFile()` with `Student::fromString` and `Teacher::fromString` as parsers. Missing files are ignored (constructor catches exceptions).

User sign in / sign up
- `CLIManager::CLI()` prints the main menu and handles the option chosen by the user.
- Sign up -> `auth.signUp()` prompts and adds a new `Account` into `_users` and then `AuthManager::saveUsers()` writes `data/accounts.txt`.
- Sign in -> `auth.signIn()` validates credentials and returns an `Account`.
  - `CLIManager` inspects `Account::getType()` and calls `adminMenu()`, `teacherMenu()`, or `studentMenu()` accordingly.

Admin workflow (example)
- `adminMenu()` works with the `students` and `teachers` repositories in memory.
- Creating a student/teacher record will construct `Student`/`Teacher` objects and call `students.add()` / `teachers.add()`.
- Optionally the admin may call "Save data" which uses `Repository::saveToFile()` to write the corresponding `data/*.txt` file.

Teacher workflow (example)
- `teacherMenu()` locates the `Teacher` object inside `teachers` by matching login (`find_if`), obtains a reference `me` and then calls `me.assignGrade(studentRef, subject, value)`.
- `Teacher::assignGrade` appends a `Grade` to the given `Student` object in memory.
- To persist that grade, `students.saveToFile()` must be called (e.g., admin Save data option or on Exit).

Exit & persistence
- There are two exit mechanisms:
  1. In the main menu, `3) Exit (save & quit)` saves `students` and `teachers` and calls `AuthManager::saveUsers()` before stopping.
  2. In any submenu, `0 - Exit program` immediately sets `_running = false` and returns. (This will prevent returning to the main menu; the program main loop in `main.cpp` will end and the process exits.)
- Note: the submenu `0` path will not always save data automatically — it sets `_running = false` and returns. If you want to ensure data is saved on all exit paths, consider centralizing the save step in the destructor or in `main()` after the CLI loop.

Inter-module responsibilities
- `CLIManager` — UI and orchestration: uses `AuthManager` for authentication, `Repository<Student>` and `Repository<Teacher>` for in-memory data and persistence, and `DataManager` indirectly via `AuthManager`.
- `AuthManager` — user account lifecycle, persists accounts via `DataManager`.
- `Repository<T>` — generic data container and file serializer/deserializer for person types (students, teachers).
- `Student` / `Teacher` / `Person` / `Grade` — domain model classes.
- `Account` — authentication model and password hashing utility.

Notes, limitations and suggestions
---------------------------------
- Password hashing: `Account::hash()` uses a simple rolling hash and is NOT secure. For production use, replace with a secure hash (bcrypt/argon2) and use salts.
- Concurrency: data files are written without locking. If multiple processes may access the same files, add locking or switch to a proper DB.
- Error handling: file I/O errors are sometimes swallowed (caught and ignored). Consider surfacing critical failures or logging them.
- `Repository::loadFromFile()` throws on missing/unopenable files; `CLIManager` currently wraps those calls in try/catch to ignore missing files. This behavior is acceptable for the current simple CLI but should be documented.
- `data/grades.txt` and `data/subjects.txt` are present but unused; remove them or implement features that use them for better clarity.

Extending the project
---------------------
- To add a `Course` entity: create `Course.h/cpp`, a `Repository<Course>`, add CLI commands for course creation and assignment, and update `Student::toString()`/`fromString()` if you persist course enrolment in `students.txt`.
- To persist grades per-teacher file: implement a `GradesManager` that writes `data/grades.txt` in a chosen format and integrate read/write on startup/exit.
- To improve UX: add input validation helpers, menu redrawing, and better command parsing (accept commands by name as well as number).

Example: what happens when a teacher assigns a grade
--------------------------------------------------
1. Teacher signs in via `auth.signIn()` returning an `Account` with type TEACHER.
2. `CLIManager` calls `teacherMenu()`. `teacherMenu()` locates the `Teacher` record in `teachers` by matching `login`.
3. Teacher chooses "Assign grade": the code finds a `Student` in `students` by login using `find_if`.
4. `Teacher::assignGrade(student, subject, value)` is called. This constructs a `Grade` with `teacherLogin` = current teacher login and `student.addGrade(grade)` appends it to the `Student` in the repository.
5. The grade exists in memory. To persist that grade, `students.saveToFile("data/students.txt", serializer)` must be invoked (admin Save data or on exit via main menu option 3).

Contact & contributions
-----------------------
- Add issues or pull requests for bug fixes or enhancements.
- If you plan to add tests, create a `tests/` directory and use a test framework such as GoogleTest.

Appendix: file list (short)
---------------------------
- `main.cpp` — program entry
- `README.md` — readme
- `documentation` — this file
- `data/accounts.txt`, `data/students.txt`, `data/teachers.txt` — persisted data
- `src/*.h` / `src/*.cpp` — source code implementing CLI, models, persistence, and utilities

End of documentation
