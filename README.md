# University database

## Table of contents
* [Description](#description)
* [Current Features](#current-features)
* [Planned Features](#planned-features)
* [Required libraries and packages](#libraries-packages)
* [How to Run](#how-to-run)
* [Example Output](#example-output)
* [Authors](#authors)

## Description
This project features a modern univeristy database Console Line Application (CLI) app, which stores student personal information, attended courses as well as grades for each subject. The programme also includes the list of course coordinators, with their personal information, courses led by them and the grades assigned to each student, that attends one of those courses. The programme may be used by an administrator, course coordinator or by a student, however their access levels will vary.
## Current Features

- Role-based authentication (admin, teacher, student). A default `admin` account is created on first run (login: `admin`, password: `admin`).
- Account management: sign up and sign in flows.
- Admin features: list and create student/teacher records, save data, sort students by average, view a polymorphic list of people, and list unique subjects.
- Teacher features: view personal info and assign grades to students.
- Student features: view personal info and grades; compute average grade.
- File-based persistence: stores data in `data/accounts.txt`, `data/students.txt`, and `data/teachers.txt`.
- Simple numeric CLI menus with quick-exit support (`0` exits the program from submenus; main menu `3` saves data and quits).
- Generic `Repository<T>` for in-memory collections with load/save support.
- Minimal, dependency-free C++17 codebase (build with `g++ -std=c++17`).

## Planned Features
## Required libraries and packages
`<bits/stdc++.h>` library
## How to run
Run `program.exe` directly in the terminal, using:
```bash
.\program.exe
```

## Greeting & Short Guide
When you start the program you'll see a short greeting and a concise guide.
## Example Output

## Authors
- Tomasz Ciba, Mechatronic Engineering, AGH University
- Krzysztof Feret, Mechatronic Engineering, AGH University