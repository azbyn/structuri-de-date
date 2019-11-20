#include "doubleList.h"
#include "utils.h"

#include <iostream>

struct Student {
    std::string firstName;
    std::string lastName;
    double grades[3];
    double average() const {
        return ::average(grades, grades+3);
    }
    friend std::ostream& operator <<(std::ostream& os, const Student& s) {
        return os << s.firstName << " " << s.lastName
                      << " - average: " << s.average();
    }

    friend std::istream& operator >>(std::istream& stream, Student& s) {
        return stream >> s.firstName >> s.lastName
                      >> s.grades[0] >> s.grades[1] >> s.grades[2];
    }
};

void printStudents(const DoubleList<Student>& students) {
    std::cout << "students:\n";
    for (auto& s : students) std::cout << s << "\n";
}
double getAverage(const DoubleList<Student>& students) {
    double res = 0;
    int count = 0;
    for (auto& s : students) {
        res += s.average();
        ++count;
    }
    return res/count;
}

int main() {
    auto students = DoubleList<Student>::read("Students");
    printStudents(students);
    std::cout << "Average: " << getAverage(students) << "\n";

    return 0;
}
