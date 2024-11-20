#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib> // For system("cls") or system("clear")
#include <sstream> // For dynamic center alignment
using namespace std;

// Color Codes
#define RESET "\033[0m"
#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define YELLOW "\033[1;93m"
#define CYAN "\033[1;96m"
#define WHITE "\033[1;97m"
#define BOLD "\033[1m"

// Function to clear the terminal screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to dynamically center text
string centerText(const string &text, int width = 80) {
    int spaces = (width - text.length()) / 2;
    stringstream ss;
    ss << string(max(0, spaces), ' ') << text;
    return ss.str();
}

// Machine Structure
struct Machine {
    int machineID;
    string name;
    string type;
    bool operationalStatus; // true = RUNNING, false = NOT RUNNING
    bool maintenanceStatus; // true = requires maintenance, false = does not require maintenance
};

// Equipment Monitoring System Class
class EquipmentMonitoringSystem {
private:
    vector<Machine> machines;
    const int maxMachines = 50;
    const string databaseFile = "machines_data.txt";

    void saveToFile() {
        ofstream outFile(databaseFile, ios::trunc);
        for (const auto &machine : machines) {
            outFile << machine.machineID << " "
                    << machine.name << " "
                    << machine.type << " "
                    << machine.operationalStatus << " "
                    << machine.maintenanceStatus << endl;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile(databaseFile);
        if (inFile.is_open()) {
            Machine machine;
            while (inFile >> machine.machineID >> machine.name >> machine.type
                          >> machine.operationalStatus >> machine.maintenanceStatus) {
                machines.push_back(machine);
            }
            inFile.close();
        }
    }

public:
    EquipmentMonitoringSystem() {
        loadFromFile();
    }

    ~EquipmentMonitoringSystem() {
        saveToFile();
    }

    void addMachine() {
        clearScreen();
        if (machines.size() >= maxMachines) {
            cout << RED << "Error: Maximum number of machines reached!" << RESET << endl;
            return;
        }

        Machine machine;
        cout << GREEN << "\n\n\t\tEnter Machine ID: " << RESET;
        cin >> machine.machineID;
        cin.ignore(); // To handle newline
        cout << GREEN << "\t\tEnter Machine Name: " << RESET;
        getline(cin, machine.name);
        cout << GREEN << "\t\tEnter Machine Type (e.g., Conveyor, Lathe, Drill): " << RESET;
        getline(cin, machine.type);
        cout << GREEN << "\t\tIs the machine operational (1 = RUNNING, 0 = NOT RUNNING)? " << RESET;
        cin >> machine.operationalStatus;
        cout << GREEN << "\t\tDoes the machine require maintenance (1 = YES, 0 = NO)? " << RESET;
        cin >> machine.maintenanceStatus;

        machines.push_back(machine);
        cout << CYAN << "\t\tMachine added successfully!" << RESET << endl;
    }

    void removeMachine() {
        clearScreen();
        int id;
        cout << YELLOW << "\n\n\t\tEnter Machine ID to remove: " << RESET;
        cin >> id;

        for (auto it = machines.begin(); it != machines.end(); ++it) {
            if (it->machineID == id) {
                machines.erase(it);
                cout << CYAN << "\t\tMachine removed successfully!" << RESET << endl;
                return;
            }
        }
        cout << RED << "\t\tError: Machine ID not found!" << RESET << endl;
    }

    void updateOperationalStatus() {
        clearScreen();
        int id;
        cout << YELLOW << "\n\n\t\tEnter Machine ID to update status: " << RESET;
        cin >> id;

        for (auto &machine : machines) {
            if (machine.machineID == id) {
                cout << GREEN << "\t\tEnter new operational status (1 = RUNNING, 0 = NOT RUNNING): " << RESET;
                cin >> machine.operationalStatus;
                cout << CYAN << "\t\tOperational status updated successfully!" << RESET << endl;
                return;
            }
        }
        cout << RED << "\t\tError: Machine ID not found!" << RESET << endl;
    }

    void searchMachine() {
        clearScreen();
        int id;
        cout << GREEN << "\n\n\t\tEnter Machine ID to search: " << RESET;
        cin >> id;

        for (const auto &machine : machines) {
            if (machine.machineID == id) {
                displayMachine(machine);
                return;
            }
        }
        cout << RED << "\t\tError: Machine ID not found!" << RESET << endl;
    }

    void displayAllMachines() {
        clearScreen();
        if (machines.empty()) {
            cout << RED << "\n\n\t\tNo machines available to display!" << RESET << endl;
            return;
        }

        cout << CYAN << left << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(15) << "Type"
             << setw(15) << "Operational"
             << "Maintenance" << RESET << endl;
        cout << string(65, '-') << endl;

        for (const auto &machine : machines) {
            cout << left << setw(10) << machine.machineID
                 << setw(20) << machine.name
                 << setw(15) << machine.type
                 << setw(15) << (machine.operationalStatus ? "RUNNING" : "NOT RUNNING")
                 << (machine.maintenanceStatus ? "YES" : "NO") << endl;
        }
    }

    void generateMaintenanceReport() {
        clearScreen();
        cout << YELLOW << "\n\n\t\tMachines requiring maintenance:" << RESET << endl;
        bool found = false;

        for (const auto &machine : machines) {
            if (machine.maintenanceStatus) {
                displayMachine(machine);
                found = true;
            }
        }

        if (!found) {
            cout << GREEN << "\t\tNo machines require maintenance!" << RESET << endl;
        }
    }

private:
    void displayMachine(const Machine &machine) {
        cout << GREEN << "\t\tMachine ID: " << RESET << machine.machineID << endl
             << GREEN << "\t\tName: " << RESET << machine.name << endl
             << GREEN << "\t\tType: " << RESET << machine.type << endl
             << GREEN << "\t\tOperational Status: " << RESET << (machine.operationalStatus ? "RUNNING" : "NOT RUNNING") << endl
             << GREEN << "\t\tMaintenance Status: " << RESET << (machine.maintenanceStatus ? "YES" : "NO") << endl
             << endl;
    }
};

void menu() {
    EquipmentMonitoringSystem system;
    int choice;

    do {
        clearScreen();
        cout << BOLD << centerText("\n\n\t\tIndustrial Equipment Monitoring System\n\n", 80) << RESET << endl;
        cout << YELLOW << "\t\t1. Add Machine" << RESET << endl;
        cout << YELLOW << "\t\t2. Remove Machine" << RESET << endl;
        cout << YELLOW << "\t\t3. Update Operational Status" << RESET << endl;
        cout << YELLOW << "\t\t4. Search Machine" << RESET << endl;
        cout << YELLOW << "\t\t5. Display All Machines" << RESET << endl;
        cout << YELLOW << "\t\t6. Generate Maintenance Report" << RESET << endl;
        cout << YELLOW << "\t\t7. Exit" << RESET << endl;
        cout << GREEN << "\n\n\t\t\tEnter your choice: " << RESET;
        cin >> choice;

        switch (choice) {
            case 1:
                system.addMachine();
                break;
            case 2:
                system.removeMachine();
                break;
            case 3:
                system.updateOperationalStatus();
                break;
            case 4:
                system.searchMachine();
                break;
            case 5:
                system.displayAllMachines();
                break;
            case 6:
                system.generateMaintenanceReport();
                break;
            case 7:
                clearScreen();
                cout << YELLOW << "\n\t\tAre you sure you want to exit? (y/n): " << RESET;
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    cout << GREEN << "\n\n\t\tExiting program...\a" << RESET << endl;
                    return;
                }
                break;
            default:
                cout << RED << "\t\tInvalid choice! Please try again." << RESET << endl;
        }
        cout << GREEN << "\n\t\tPress Enter to continue..." << RESET;
        cin.ignore();
        cin.get();
    } while (true);
}

int main() {
    menu();
    return 0;
}
