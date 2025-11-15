#include "Student.h"
#include <numeric>
#include <sstream>
#include <algorithm>

Student::Student()
    : currentSemester(0), startYear(0)
{
}

Student::Student(std::string ln, std::string fn, std::string dep, std::string gr,
    int cSem, int sYear, std::vector<double> scores)
    : lastName(std::move(ln)),
    firstName(std::move(fn)),
    department(std::move(dep)),
    group(std::move(gr)),
    currentSemester(cSem),
    startYear(sYear),
    semesterScores(std::move(scores))
{
}

Student::Student(Student&& other) noexcept
    : lastName(std::move(other.lastName)),
    firstName(std::move(other.firstName)),
    department(std::move(other.department)),
    group(std::move(other.group)),
    currentSemester(other.currentSemester),
    startYear(other.startYear),
    semesterScores(std::move(other.semesterScores))
{
}

Student& Student::operator=(Student&& other) noexcept
{
    if (this != &other) {
        lastName = std::move(other.lastName);
        firstName = std::move(other.firstName);
        department = std::move(other.department);
        group = std::move(other.group);
        currentSemester = other.currentSemester;
        startYear = other.startYear;
        semesterScores = std::move(other.semesterScores);
    }
    return *this;
}

// Повертає середній бал останнього семестру (або 0.0, якщо даних немає)
double Student::getCurrentAverageScore() const
{
    return semesterScores.empty() ? 0.0 : semesterScores.back();
}

// Повертає середній бал за весь період навчання
double Student::getOverallAverageScore() const
{
    if (semesterScores.empty()) return 0.0;
    double sum = std::accumulate(semesterScores.begin(), semesterScores.end(), 0.0);
    return sum / semesterScores.size();
}

// Додає або замінює бал за поточний семестр
void Student::setAverageScore(double score)
{
    if (semesterScores.empty()) {
        semesterScores.push_back(score);
    }
    else {
        semesterScores.back() = score;
    }
}

// Зчитує студента з рядка формату: Прізвище;Ім'я;Кафедра;Група;Семестр;Рік;4.5,4.8,5.0
std::istream& operator>>(std::istream& is, Student& s)
{
    s.semesterScores.clear();
    if (std::getline(is, s.lastName, ';') &&
        std::getline(is, s.firstName, ';') &&
        std::getline(is, s.department, ';') &&
        std::getline(is, s.group, ';'))
    {
        is >> s.currentSemester;
        is.ignore(1, ';');
        is >> s.startYear;
        is.ignore(1, ';');

        std::string scoresStr;
        if (std::getline(is, scoresStr)) {
            std::stringstream ss(scoresStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                try {
                    size_t idx;
                    double score = std::stod(token, &idx);
                    if (idx == token.length()) {
                        s.semesterScores.push_back(score);
                    }
                }
                catch (...) {
                    // Ігноруємо некоректні значення
                }
            }
        }
    }
    else {
        is.setstate(std::ios::failbit);
    }
    return is;
}

// Записує студента у рядок для збереження у файл
std::ostream& operator<<(std::ostream& os, const Student& s)
{
    os << s.lastName << ";" << s.firstName << ";" << s.department << ";" << s.group << ";"
        << s.currentSemester << ";" << s.startYear << ";";
    for (size_t i = 0; i < s.semesterScores.size(); ++i) {
        os << s.semesterScores[i];
        if (i < s.semesterScores.size() - 1) {
            os << ",";
        }
    }
    os << "\n";
    return os;
}

// Порівняння за середнім балом останнього семестру (для сортування)
bool Student::operator>(const Student& other) const
{
    return this->getCurrentAverageScore() > other.getCurrentAverageScore();
}