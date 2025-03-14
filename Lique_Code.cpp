#include <iostream>  
#include <iomanip>   
#include <limits>    
#include <string>   
#include <vector>    
#include <algorithm> 
#include <cctype>    
using namespace std;

class Employee {
protected:
    int id;
    string name;
    double salary;
public:
    Employee(int id, string name, double salary) : id(id), name(name), salary(salary) {}
    virtual double computeSalary() const = 0;
    virtual void display() const = 0;
    virtual double getSalary() const { return salary; }
    int getId() const { return id; }
    virtual ~Employee() {}
};

class FullTimeEmployee : public Employee {
public:
    FullTimeEmployee(int id, string name, double salary) : Employee(id, name, salary) {}
    double computeSalary() const override { return getSalary(); }
    void display() const override {
        cout << "\nEmployee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << fixed << setprecision(2) << getSalary() << "\n";
    }
};

class PartTimeEmployee : public Employee {
private:
    int hoursWorked;
public:
    PartTimeEmployee(int id, string name, double salary, int hoursWorked) : Employee(id, name, salary), hoursWorked(hoursWorked) {}
    double computeSalary() const override { return getSalary() * hoursWorked; }
    void display() const override {
        cout << "\nEmployee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Wage: $" << fixed << setprecision(2) << getSalary() << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << "Total Salary: $" << computeSalary() << "\n";
    }
};

class ContractualEmployee : public Employee {
private:
    int projects;
public:
    ContractualEmployee(int id, string name, double salary, int projects) : Employee(id, name, salary), projects(projects) {}
    double computeSalary() const override { return getSalary() * projects; }
    void display() const override {
        cout << "\nEmployee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << fixed << setprecision(2) << getSalary() << "\n";
        cout << "Projects Completed: " << projects << "\n";
        cout << "Total Salary: $" << computeSalary() << "\n";
    }
};

class PayrollSystem {
private:
    vector<Employee*> employees;
public:
    void addEmployee(Employee* emp) {
        employees.push_back(emp);
    }

    bool isDuplicateId(const string& idStr) {
        if (idStr.empty() || idStr.find_first_not_of("0123456789") != string::npos) {
            cout << "Oops, that's not a valid input. Please enter a valid numeric ID." << endl;
            return true;
        }
        int id = stoi(idStr);
        for (const auto& emp : employees) {
            if (emp->getId() == id) {
                cout << "Duplicate ID! Please change and enter another ID." << endl;
                return true;
            }
        }
        return false;
    }

    void displayPayroll() const {
        cout << "\n------------- Employee Payroll Report -------------n";
        for (const Employee* emp : employees) {
            emp->display();
        }
    }

    ~PayrollSystem() {
        for (Employee* emp : employees) {
            delete emp;
        }
    }
};

string getValidatedString(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty() && value.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos) {
            return value;
        }
        cout << "Oops, Invalid format! Please enter a VALID NAME (letters only)." << endl;
    }
}

int getValidatedInteger(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
            return stoi(input);
        }
        cout << "Oops, Invalid format! Could you please enter only a number from (1-5)? Let's try again." << endl;
    }
}

double getValidatedDouble(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        bool valid = true;
        int dotCount = 0;
        for (char ch : input) {
            if (!isdigit(ch)) {
                if (ch == '.' && dotCount == 0) {
                    dotCount++;
                } else {
                    valid = false;
                    break;
                }
            }
        }
        if (valid && !input.empty()) {
            return stod(input);
        }
        cout << "Oops, Invalid format! Please enter a VALID NUMBER." << endl;
    }
}

int main() {
    PayrollSystem payroll;
    bool running = true;
    while (running) {
        cout << "\nMenu:\n";
        cout << "1 - Full-time Employee\n";
        cout << "2 - Part-time Employee\n";
        cout << "3 - Contractual Employee\n";
        cout << "4 - Display Payroll Report\n";
        cout << "5 - Exit\n";
        int choice = getValidatedInteger("Enter your choice: ");

        if (choice >= 1 && choice <= 3) {
            string idStr;
            while (true) {
                cout << "Enter ID: ";
                getline(cin, idStr);
                if (!payroll.isDuplicateId(idStr)) break;
            }
            int id = stoi(idStr);
            
            string name = getValidatedString("Enter Name: ");

            if (choice == 1) {
                double salary = getValidatedDouble("Enter Salary: ");
                payroll.addEmployee(new FullTimeEmployee(id, name, salary));
            } else if (choice == 2) {
                double salary = getValidatedDouble("Enter Salary per Hour: ");
                int hours = getValidatedInteger("Enter Hours Worked: ");
                payroll.addEmployee(new PartTimeEmployee(id, name, salary, hours));
            } else if (choice == 3) {
                double salary = getValidatedDouble("Enter Salary per Project: ");
                int projects = getValidatedInteger("Enter Number of Projects: ");
                payroll.addEmployee(new ContractualEmployee(id, name, salary, projects));
            }
        } else if (choice == 4) {
            payroll.displayPayroll();
        } else if (choice == 5) {
            running = false;
        }
    }
    cout << "Exiting the program now....\n";
    return 0;
}