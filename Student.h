#pragma once
#define NOMINMAX
#include <string>
#include <vector>
#include <iostream>

class Student {
private:
    std::string lastName;           // Прізвище студента
    std::string firstName;          // Ім'я студента
    std::string department;         // Кафедра
    std::string group;              // Група
    int currentSemester;            // Кількість пройдених семестрів
    int startYear;                  // Рік вступу
    std::vector<double> semesterScores;  // Середні бали за кожен семестр

public:
    // === Конструктори ===

    // Конструктор за замовчуванням — створює порожнього студента
    Student();

    // Конструктор з параметрами — ініціалізує всі поля
    Student(std::string ln, std::string fn, std::string dep, std::string gr,
        int cSem, int sYear, std::vector<double> scores);

    // Конструктор копіювання (за замовчуванням)
    Student(const Student& other) = default;

    // Оператор присвоєння копіюванням (за замовчуванням)
    Student& operator=(const Student& other) = default;

    // Конструктор переміщення — ефективно переносить дані
    Student(Student&& other) noexcept;

    // Оператор присвоєння переміщенням — ефективно замінює вміст
    Student& operator=(Student&& other) noexcept;

    // === Геттери ===

    // Повертає прізвище
    const std::string& getLastName() const { return lastName; }

    // Повертає ім'я
    const std::string& getFirstName() const { return firstName; }

    // Повертає кафедру
    const std::string& getDepartment() const { return department; }

    // Повертає групу
    const std::string& getGroup() const { return group; }

    // Повертає середній бал за останній семестр
    double getCurrentAverageScore() const;

    // Повертає всі бали по семестрах
    const std::vector<double>& getSemesterScores() const { return semesterScores; }

    // Повертає середній бал за весь період навчання
    double getOverallAverageScore() const;

    // Повертає поточний семестр
    int getCurrentSemester() const { return currentSemester; }

    // Повертає рік навчання (1, 2, 3...)
    int getStudyYear() const { return (currentSemester + 1) / 2; }

    // Повертає рік вступу
    int getStartYear() const { return startYear; }

    // === Сеттери ===

    // Встановлює прізвище
    void setLastName(const std::string& ln) { lastName = ln; }

    // Встановлює ім'я
    void setFirstName(const std::string& fn) { firstName = fn; }

    // Встановлює кафедру
    void setDepartment(const std::string& dep) { department = dep; }

    // Встановлює групу
    void setGroup(const std::string& gr) { group = gr; }

    // Встановлює або додає середній бал за поточний семестр
    void setAverageScore(double score);

    // Встановлює кількість семестрів
    void setSemester(int sem) { currentSemester = sem; }

    // Встановлює рік вступу
    void setYear(int year) { startYear = year; }

    // === Оператори вводу/виводу ===

    // Зчитує студента з потоку (з файлу students.txt)
    friend std::istream& operator>>(std::istream& is, Student& s);

    // Записує студента у потік (у файл input.txt)
    friend std::ostream& operator<<(std::ostream& os, const Student& s);

    // Порівнює студентів за середнім балом останнього семестру (для сортування)
    bool operator>(const Student& other) const;
};