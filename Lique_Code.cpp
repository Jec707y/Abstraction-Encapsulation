#include <iostream> 
#include <vector>    
#include <iomanip>   
#include <limits>    
#include <algorithm> 
#include <string>    
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
    int getId() const { return id; }
    virtual ~Employee() {}
};

class FullTimeEmployee : public virtual Employee {
public:
    FullTimeEmployee(int id, string name, double salary) : Employee(id, name, salary) {}
    double computeSalary() const override { return salary; }
    void display() const override {
        cout << "\nEmployee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << fixed << setprecision(2) << salary << "\n";
    }
};

class PartTimeEmployee : public virtual Employee {
private:
    int hoursWorked;
public:
    PartTimeEmployee(int id, string name, double salary, int hoursWorked) : Employee(id, name, salary), hoursWorked(hoursWorked) {}
    double computeSalary() const override { return salary * hoursWorked; }
    void display() const override {
        cout << "\nEmployee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Wage: $" << fixed << setprecision(2) << salary << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << "Total Salary: $" << computeSalary() << "\n";
    }
};

class ContractualEmployee : public virtual Employee {
private:
    int projects;
public:
    ContractualEmployee(int id, string name, double salary, int projects) : Employee(id, name, salary), projects(projects) {}
    double computeSalary() const override { return salary * projects; }
    void display() const override {
        cout << "\nEmployee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << fixed << setprecision(2) << salary << "\n";
        cout << "Projects Completed: " << projects << "\n";
        cout << "Total Salary: $" << computeSalary() << "\n";
    }
};

class PayrollSystem {
private:
    vector<Employee*> employees;
    vector<int> employeeIds;
public:
    void addEmployee(Employee* emp) {
        employees.push_back(emp);
        employeeIds.push_back(emp->getId());
    }

    bool isDuplicateId(int id) {
        return std::find(employeeIds.begin(), employeeIds.end(), id) != employeeIds.end();
    }

    void displayPayroll() const {
        cout << "\n-------------- Employee Payroll Report --------------\n";
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

int getValidatedMenuChoice(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin >> input;

        if (all_of(input.begin(), input.end(), ::isdigit)) {
            int choice = stoi(input);
            if (choice >= 1 && choice <= 5) {
                return choice;
            }
        }
        
        cout << "Could you please enter only a number? Let's try again." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int getValidatedInteger(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail() || value <= 0) {
            cout << "Oops, that's not a valid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    
            return value;
        }
    }
}

double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input! Please enter a positive number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string getValidatedString(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin >> ws, value);
        if (!value.empty() && !all_of(value.begin(), value.end(), ::isdigit)) {
            return value;
        }
        cout << "Please enter a name, not numbers or other characters." << endl;
    }
}

int main() {
    PayrollSystem payroll;
    int choice;
    bool running = true;
    while (running) {
        cout << "\nMenu:\n";
        cout << "1 - Full-time Employee\n";
        cout << "2 - Part-time Employee\n";
        cout << "3 - Contractual Employee\n";
        cout << "4 - Display Payroll Report\n";
        cout << "5 - Exit\n";
        choice = getValidatedMenuChoice("Enter your choice: ");

        if (choice >= 1 && choice <= 3) {
            int id;
            while (true) {
                id = getValidatedInteger("Enter ID: ");
                if (!payroll.isDuplicateId(id)) {
                    break;
                } else {
                    cout << "Duplicate ID! Please change and enter another ID." << endl;
                }
            }
            
            string name = getValidatedString("Enter Name: ");

            if (choice == 1) {
                double salary = getValidatedDouble("Enter Salary: ");
                payroll.addEmployee(new FullTimeEmployee(id, name, salary));
            }
            else if (choice == 2) {
                double salary = getValidatedDouble("Enter Salary per Hour: ");
                int hours = getValidatedInteger("Enter Hours Worked: ");
                payroll.addEmployee(new PartTimeEmployee(id, name, salary, hours));
            }
            else if (choice == 3) {
                double salary = getValidatedDouble("Enter Salary per Project: ");
                int projects = getValidatedInteger("Enter Number of Projects: ");
                payroll.addEmployee(new ContractualEmployee(id, name, salary, projects));
            }
        }
        else if (choice == 4) {
            payroll.displayPayroll();
        }
        else if (choice == 5) {
            running = false;
        }
    }
    
    cout << "Exiting the program now.....\n";
    return 0;
}