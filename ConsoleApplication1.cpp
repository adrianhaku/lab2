#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <limits>  // For numeric_limits
#include <regex>   // For name validation

using namespace std;

// Enumeration for deposit types
enum class DepositType {
    Savings,
    Fixed
};

// Singleton Class for Interest Rates
class InterestRates {
private:
    map<DepositType, double> rates;
    static InterestRates* instance;

    // Private constructor for Singleton
    InterestRates() {
        rates[DepositType::Savings] = 0.03;
        rates[DepositType::Fixed] = 0.05;
    }

public:
    // Get the Singleton instance
    static InterestRates* Instance() {
        if (!instance)
            instance = new InterestRates();
        return instance;
    }

    double getRate(DepositType type) {
        return rates[type];
    }
};

// Initialize the static instance to nullptr
InterestRates* InterestRates::instance = nullptr;

// Deposit class
class Deposit {
private:
    double amount;
    DepositType type;

public:
    Deposit(double amount, DepositType type) : amount(amount), type(type) {}

    double getInterest() {
        InterestRates* rates = InterestRates::Instance();
        return amount * rates->getRate(type);
    }

    void addAmount(double amt) {
        amount += amt;
    }

    double getAmount() {
        return amount;
    }
};

// Client class
class Client {
private:
    int id;
    string name;
    Deposit* deposit;

public:
    Client(int id, string name) : id(id), name(name), deposit(nullptr) {}

    void addDeposit(Deposit* dep) {
        deposit = dep;
    }

    double getInterest() {
        return deposit ? deposit->getInterest() : 0.0;
    }

    void depositAmount(double amt) {
        if (deposit) {
            deposit->addAmount(amt);
        }
    }

    int getId() {
        return id;
    }

    void printClientInfo() {
        cout << "Client ID: " << id << ", Name: " << name << ", Deposit: ";
        if (deposit) {
            cout << deposit->getAmount() << endl;
        }
        else {
            cout << "No deposit" << endl;
        }
    }

    ~Client() {
        delete deposit;
    }
};

// Bank class
class Bank {
private:
    vector<Client*> clients;

public:
    // Function to check if any clients exist
    bool hasClients() {
        return !clients.empty();  // Returns true if the vector is not empty
    }

    void addClient(Client* client) {
        clients.push_back(client);
    }

    Client* findClientById(int clientId) {
        for (auto& client : clients) {
            if (client->getId() == clientId) {
                return client;
            }
        }
        return nullptr;
    }

    void depositToClient(int clientId, double amt) {
        if (!hasClients()) {  // Check if any clients exist
            cout << "No existing client." << endl;
            return;
        }

        Client* client = findClientById(clientId);
        if (client && amt > 0) {
            client->depositAmount(amt);
            cout << "Deposit added to client ID " << clientId << endl;
        }
        else {
            cout << "Invalid client ID or amount." << endl;
        }
    }

    double totalInterest() {
        if (!hasClients()) {  // Check if any clients exist
            cout << "No existing client." << endl;
            return 0.0;
        }

        double total = 0;
        for (auto& client : clients) {
            total += client->getInterest();
        }
        return total;
    }

    void listClients() {
        if (!hasClients()) {  // Check if any clients exist
            cout << "No existing client." << endl;
            return;
        }

        for (auto& client : clients) {
            client->printClientInfo();
        }
    }

    ~Bank() {
        for (auto& client : clients) {
            delete client;
        }
    }
};

// Function to validate numeric input
double getValidDoubleInput() {
    double input;
    while (true) {
        cin >> input;
        if (cin.fail()) {
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            cout << "Invalid input. Please enter amount in digits: ";  // Updated message
        }
        else {
            return input;  // Input is valid, return it
        }
    }
}

// Function to validate client ID (only numbers)
int getValidClientId() {
    int clientId;
    while (true) {
        cin >> clientId;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a numeric Client ID: ";
        }
        else {
            return clientId;
        }
    }
}

// Function to validate client name (only letters)
string getValidClientName() {
    string name;
    regex namePattern("^[a-zA-Z]+$");  // Regular expression for letters only

    while (true) {
        cin >> name;
        if (!regex_match(name, namePattern)) {
            cout << "Invalid input. Please enter letters only for the Client Name: ";
        }
        else {
            return name;
        }
    }
}

// Function to validate deposit type input (only Savings or Fixed)
DepositType getValidDepositType() {
    int type;
    while (true) {
        cout << "Enter Deposit Type (0 for Savings, 1 for Fixed): ";
        cin >> type;

        if (cin.fail() || (type != 0 && type != 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the invalid input
            cout << "Invalid input. Please select 0 for Savings or 1 for Fixed." << endl;
        }
        else {
            return (type == 0) ? DepositType::Savings : DepositType::Fixed;
        }
    }
}

// Function to display instructions (menu options)
void displayInstructions() {
    cout << "\nBank Management System\n";
    cout << "1. Add Client" << endl;
    cout << "2. Add Deposit to Client" << endl;
    cout << "3. Replenish Deposit" << endl;
    cout << "4. Calculate Total Interest" << endl;
    cout << "5. List Clients" << endl;
    cout << "6. Exit" << endl;
}

// Main Menu and User Interaction
void menu() {
    Bank bank;
    bool running = true;

    // Display the instructions once at the beginning
    displayInstructions();

    while (running) {
        cout << "Select an option: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter Client ID: ";
            int id = getValidClientId();

            cout << "Enter Client Name: ";
            string name = getValidClientName();

            bank.addClient(new Client(id, name));
            cout << "Client added successfully.\n";
            break;
        }
        case 2: {
            int clientId;
            cout << "Enter Client ID: ";
            clientId = getValidClientId();

            double amount;
            cout << "Enter Deposit Amount: ";
            amount = getValidDoubleInput();  // Get a valid deposit amount

            // Get and validate deposit type
            DepositType depositType = getValidDepositType();

            Client* client = bank.findClientById(clientId);
            if (client) {
                client->addDeposit(new Deposit(amount, depositType));
                cout << "Deposit added to client ID " << clientId << endl;
            }
            else {
                cout << "Client not found." << endl;
            }
            break;
        }
        case 3: {
            int clientId;
            cout << "Enter Client ID: ";
            clientId = getValidClientId();

            double amount;
            cout << "Enter Amount to Replenish: ";
            amount = getValidDoubleInput();  // Get a valid amount for replenishment

            bank.depositToClient(clientId, amount);
            break;
        }
        case 4: {
            cout << "Total Interest for all clients: " << bank.totalInterest() << endl;
            break;
        }
        case 5: {
            cout << "Listing all clients:\n";
            bank.listClients();
            break;
        }
        case 6:
            running = false;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }
}

int main() {
    menu();
    return 0;
}
