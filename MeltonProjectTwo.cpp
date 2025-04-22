#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>

using namespace std;

// Course class holds course number, title, and prerequisites
class Course {
public:
    string courseNum;
    string courseName;
    vector<string> prereqList;

    Course() {}

    Course(string number, string name, vector<string> prereqs) {
        courseNum = number;
        courseName = name;
        prereqList = prereqs;
    }
};

// Global hash table to store all courses
unordered_map<string, Course> courseTable;

// Utility function to split a line by a given delimiter
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream stream(line);
    while (getline(stream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// Validates course data: checks that all prerequisites exist in the course list
bool validateFile(const vector<string>& courseLines) {
    unordered_map<string, bool> courseNumbers;

    for (const auto& line : courseLines) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() < 2) {
            cerr << "Error: Each line must contain at least a course number and a title." << endl;
            return false;
        }
        courseNumbers[tokens[0]] = true;
    }

    for (const auto& line : courseLines) {
        vector<string> tokens = split(line, ',');
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (courseNumbers.find(tokens[i]) == courseNumbers.end()) {
                cerr << "Error: Prerequisite " << tokens[i] << " not found in course list." << endl;
                return false;
            }
        }
    }

    return true;
}

// Loads the course data from the file and populates the hash table
bool loadCourses(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file." << endl;
        return false;
    }

    string line;
    vector<string> courseLines;
    while (getline(file, line)) {
        if (!line.empty()) {
            courseLines.push_back(line);
        }
    }
    file.close();

    if (!validateFile(courseLines)) {
        return false;
    }

    courseTable.clear();

    for (const auto& line : courseLines) {
        vector<string> tokens = split(line, ',');
        string number = tokens[0];
        string name = tokens[1];
        vector<string> prereqs(tokens.begin() + 2, tokens.end());

        courseTable[number] = Course(number, name, prereqs);
    }

    cout << "Courses loaded and stored successfully." << endl;
    return true;
}

// Prints information for a specific course, including title and prerequisites with titles
void printCourse(const string& courseNum) {
    auto it = courseTable.find(courseNum);
    if (it == courseTable.end()) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& course = it->second;
    cout << course.courseNum << ": " << course.courseName << endl;
    cout << "Prerequisites: ";
    if (course.prereqList.empty()) {
        cout << "None";
    }
    else {
        for (const string& prereq : course.prereqList) {
            auto preIt = courseTable.find(prereq);
            if (preIt != courseTable.end()) {
                cout << prereq << " (" << preIt->second.courseName << ") ";
            }
            else {
                cout << prereq << " ";
            }
        }
    }
    cout << endl;
}

// Prints all courses in alphanumeric order by course number
void printAllCourses() {
    vector<string> sortedKeys;
    for (const auto& entry : courseTable) {
        sortedKeys.push_back(entry.first);
    }
    sort(sortedKeys.begin(), sortedKeys.end());

    for (const string& courseNum : sortedKeys) {
        const Course& course = courseTable[courseNum];
        cout << course.courseNum << ": " << course.courseName << endl;
    }
}

// Main function provides menu and handles user interaction
int main() {
    string path, courseNum, input;

    while (true) {
        cout << "\nMenu:" << endl;
        cout << "1. Load and Validate File" << endl;
        cout << "2. Print Alphanumeric Course List" << endl;
        cout << "3. Print Course Information" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice: ";
        getline(cin, input);

        // Validate input is a number
        if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit)) {
            cout << "Invalid input. Please enter a number between 1 and 9." << endl;
            continue;
        }

        int choice = stoi(input);  // Convert string to integer

        switch (choice) {
        case 1:{
            cout << "Would you like to use the default course file? (Y/N): ";
            string tempInput;
            getline(cin, tempInput);
            char useDefault = (!tempInput.empty()) ? toupper(tempInput[0]) : 'N';

            if (useDefault == 'Y') {
                path = "CS 300 ABCU_Advising_Program_Input.csv"; // Default file name
            }
            else {
                cout << "Enter file path: ";
                getline(cin, path);
            }
            loadCourses(path);
            break;
        }
        case 2:
            printAllCourses();
            break;
        case 3:
            cout << "Enter course number: ";
            getline(cin, courseNum);
            printCourse(courseNum);
            break;
        case 9:
            cout << "Exiting program." << endl;
            return 0;
        default:
            cout << "Invalid option. Try again." << endl;
        }
    }
}