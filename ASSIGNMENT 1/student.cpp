#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// Student class with encapsulation
class Student {
private:
    int regNo;
    string name;
    double marks;
    char grade;


    void calculateGrade() {
        if (marks >= 90)
            grade = 'A';
        else if (marks >= 80)
            grade = 'B';
        else if (marks >= 70)
            grade = 'C';
        else if (marks >= 60)
            grade = 'D';
        else
            grade = 'F';
    }

public:
    // Default constructor
    Student() : regNo(0), name(""), marks(0.0), grade('F') {}

    // Parameterized constructor
    Student(int r, string n, double m) : regNo(r), name(n), marks(m) {
        calculateGrade();
    }

    // Input student details from user
    void input() {
        cout << "Registration Number: ";
        cin >> regNo;
        cout << "Name: ";
        cin.ignore();   //
        getline(cin, name);
        cout << "Marks (0-100): ";
        cin >> marks;
        // Validate marks
        while (marks < 0 || marks > 100) {
            cout << "Marks must be between 0 and 100. Re-enter: ";
            cin >> marks;
        }
        calculateGrade();   // update grade after marks are set
    }

    // Display student record
    void display() const {
        cout << setw(10) << regNo
             << setw(20) << name
             << setw(10) << marks
             << setw(8) << grade << endl;
    }

    // Getter for marks (used to find highest marks)
    double getMarks() const {
        return marks;
    }

    // Getter for regNo
    int getRegNo() const {
        return regNo;
    }

    // Getter for name
    string getName() const {
        return name;
    }

    // Getter for grade
    char getGrade() const {
        return grade;
    }
};

int main() {
    int n;
    cout << "Enter number of students: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number. Exiting." << endl;
        return 1;
    }

    vector<Student> students;   // dynamic array of Student objects

    // Input data for each student
    for (int i = 0; i < n; i++) {
        cout << "\n--- Student " << i + 1 << " ---\n";
        Student s;          // create a temporary Student object
        s.input();          // fill it via member function
        students.push_back(s);  // add to vector
    }

    // Find student with highest marks using a lambda
    auto highest = max_element(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.getMarks() < b.getMarks();
        });

    // Display all students
    cout << "\n\n--- All Student Records ---\n";
    cout << setw(10) << "RegNo"
         << setw(20) << "Name"
         << setw(10) << "Marks"
         << setw(8) << "Grade" << endl;
    cout << string(48, '-') << endl;

    for (const auto& s : students) {
        s.display();
    }

    // Display the student with highest marks
    cout << "\n--- Student with Highest Marks ---\n";
    cout << "RegNo: " << highest->getRegNo() << endl;
    cout << "Name: " << highest->getName() << endl;
    cout << "Marks: " << highest->getMarks() << endl;
    cout << "Grade: " << highest->getGrade() << endl;

    return 0;
}
