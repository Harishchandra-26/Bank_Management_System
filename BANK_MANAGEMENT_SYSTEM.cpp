#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
using namespace std;

class Customer {
private:
    static int accountCounter;
    int accountNumber;
    string name;
    string accountType;
    double balance;
public:
    Customer(string name, string aType, double balance) {
        this->name = name;
        this->accountType = aType;
        this->balance = balance;
        this->accountNumber = 10000+ (++accountCounter);
    }
    Customer(int accountNumber, string name, string aType, double balance) {
        this->accountNumber = accountNumber;
        this->name = name;
        this->accountType = aType;
        this->balance = balance;
    }

    int getAccountNumber() const {
        return accountNumber;
    }
    void setBalance(double newBalance) {
        balance = newBalance;
    }

    virtual bool isEligibleForLoan() = 0;

    virtual double getInterestRate() = 0;

    virtual double getLoanDiscount(const string& loanType) = 0; // New method for loan discount based on type

    void showCustomerDetails() {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << name << "\nBalance: Rs." << balance << "\nAccount Type: " << accountType << endl;
    }

    string getName() const {
        return name;
    }
    string getAccountType() const {
        return accountType;
    }
    double getBalance() const {
        return balance;
    }
};

// Static Account Number Counter Initialized
int Customer::accountCounter = 0;

class SavingsCustomer : public Customer {
public:
    SavingsCustomer(string name, string aType = "Savings", double balance = 0.0)
        : Customer(name, aType, balance) {}

    SavingsCustomer(int accountNumber, string name, string aType, double balance)
        : Customer(accountNumber, name, aType, balance) {}

    bool isEligibleForLoan() override {
        return getBalance() >= 25000;
    }
    double getInterestRate() override {
        return 8.0; // Base interest rate for Savings Customer
    }
    double getLoanDiscount(const string& loanType) override { // Discount based on loan type
        if (loanType == "Home Loan") return 0.1; // 10% discount
        if (loanType == "Car Loan") return 0.05; // 5% discount
        if (loanType == "Gold Loan") return 0.15; // 15% discount
        if (loanType == "Personal Loan") return 0.1; // 10% discount
        if (loanType == "Education Loan") return 0.1; // 10% discount
        return 0.0; // No discount for other types
    }
};

class CurrentCustomer : public Customer {
public:
    CurrentCustomer(string name, string aType = "Current", double balance = 0.0)
        : Customer(name, aType, balance) {}

    CurrentCustomer(int accountNumber, string name, string aType, double balance)
        : Customer(accountNumber, name, aType, balance) {}

    bool isEligibleForLoan() override {
        return getBalance() >= 50000;
    }
    double getInterestRate() override {
        return 12.0; // Base interest rate for Current Customer
    }
    double getLoanDiscount(const string& loanType) override { // Discount based on loan type
        if (loanType == "Home Loan") return 0.05; // 5% discount
        if (loanType == "Car Loan") return 0.1; // 10% discount
        if (loanType == "Gold Loan") return 0.2; // 20% discount
        if (loanType == "Personal Loan") return 0.05; // 5% discount
        if (loanType == "Education Loan") return 0.1; // 10% discount
        return 0.0; // No discount for other types
    }
};

class StudentCustomer : public Customer {
private:
    string studentID;
public:
    StudentCustomer(string name, string aType = "Student", double balance = 0.0, string sID = "")
        : Customer(name, aType, balance), studentID(sID) {}

    StudentCustomer(int accountNumber, string name, string aType, double balance, string sID)
        : Customer(accountNumber, name, aType, balance), studentID(sID) {}

    bool isEligibleForLoan() override {
        return getBalance() >= 10000;
    }
    double getInterestRate() override {
        return 5.0; // Base interest rate for Student Customer
    }
    double getLoanDiscount(const string& loanType) override { // Discount based on loan type
        if (loanType == "Home Loan") return 0.15; // 15% discount
        if (loanType == "Car Loan") return 0.1; // 10% discount
        if (loanType == "Gold Loan") return 0.1; // 10% discount
        if (loanType == "Personal Loan") return 0.05; // 5% discount
        if (loanType == "Education Loan") return 0.2; // 20% discount
        return 0.0; // No discount for other types
    }
};

class Loan {
public:
    double calculateMonthlyPayment(Customer* customer, double principal, int years, const string& loanType) {
    double annualRate = customer->getInterestRate() * (1 - customer->getLoanDiscount(loanType));
    double monthlyRate = annualRate / 100 / 12;
    int months = years * 12;

    if (monthlyRate == 0) {
        return principal / months; 
    }
    double monthlyPayable = (principal * monthlyRate) / (1 - pow(1 + monthlyRate, -months));
    return monthlyPayable;
}
    double totalPayable(Customer* customer, double principal, int years, const string& loanType) {
        double monthlyPayment = calculateMonthlyPayment(customer, principal, years, loanType);
        return monthlyPayment * years * 12;
    }
    double totalPayable(double knownMonthlyPayable, int years) {
        return knownMonthlyPayable * years * 12;
    }
};

class FileManager {
private:
    string filePath;
public:
    FileManager(string path) : filePath(path) {}

    vector<Customer*> loadAllCustomersFromFile() {
        ifstream inFile(filePath);
        vector<Customer*> customers;
        if (!inFile) {
            cerr << "Error opening file for reading!" << endl;
            return customers;
        }

        int accountNumber;
        string name, accountType;
        double balance;
        while (inFile >> accountNumber >> name >> accountType >> balance) {
            if (accountType == "Savings") {
                customers.push_back(new SavingsCustomer(accountNumber, name, accountType, balance));
            }
            else if (accountType == "Current") {
                customers.push_back(new CurrentCustomer(accountNumber, name, accountType, balance));
            }
            else if (accountType == "Student") {
                string studentID;
                inFile >> studentID;
                customers.push_back(new StudentCustomer(accountNumber, name, accountType, balance, studentID));
            }
        }
        inFile.close();
        return customers;
    }

    void saveCustomerDetails(Customer& customer) {
        ofstream outFile(filePath, ios::app);
        if (!outFile) {
            cerr << "Error opening file for writing!" << endl;
            return;
        }
        outFile << customer.getAccountNumber() << " " << customer.getName() << " " 
                << customer.getAccountType() << " " << customer.getBalance() << endl;
        outFile.close();
        cout << "Customer details saved successfully." << endl;
    }

    void updateCustomerFile(const vector<Customer*>& customers) {
        ofstream outFile(filePath);
        if (!outFile) {
            cerr << "Error opening file for updating!" << endl;
            return;
        }

        for (Customer* customer : customers) {
            outFile << customer->getAccountNumber() << " " << customer->getName() << " " 
                    << customer->getAccountType() << " " << customer->getBalance() << endl;
        }
        outFile.close();
    }

    void saveLoanDetails(Loan& loan, Customer* customer, double principal, int years, const string& loanType) {
        ofstream outFile(filePath, ios::app);
        if (!outFile) {
            cerr << "Error opening file for writing!" << endl;
            return;
        }

        double monthlyPayment = loan.calculateMonthlyPayment(customer, principal, years, loanType);
        double totalPayment = loan.totalPayable(customer, principal, years, loanType);

        outFile << "Customer Name: " << customer->getName() << endl; // Include customer's name
        outFile << "Loan Type: " << loanType << endl;
        outFile << "Loan Principal: Rs." << principal << endl;
        outFile << "Loan Duration: " << years << " years " << endl;
        outFile << "Monthly Payment: Rs." << monthlyPayment << endl;
        outFile << "Total Payment: Rs." << totalPayment << endl << endl;

        outFile.close();
    }

    void logTransaction(Customer& customer, const string& transactionType, double amount, double newBalance) {
        ofstream outFile("transactions_" + to_string(customer.getAccountNumber()) + ".txt", ios::app);
        if (!outFile) {
            cerr << "Error opening transaction file for writing!" << endl;
            return;
        }
        outFile << transactionType << " of Rs." << amount << " | New Balance: Rs." << newBalance << endl;
        outFile.close();
    }

     void deleteCustomerFile(int accountNumber) {
        ifstream inFile(filePath);
        ofstream tempFile("temp.txt");
        if (!inFile || !tempFile) {
            cerr << "Error opening files!" << endl;
            return;
        }

        int accNumber;
        string name, accountType;
        double balance;
        bool found = false;

        while (inFile >> accNumber >> name >> accountType >> balance) {
            if (accNumber != accountNumber) {
                tempFile << accNumber << " " << name << " " << accountType << " " << balance << endl;
            } else {
                found = true;
            }
        }
        inFile.close();
        tempFile.close();

        remove(filePath.c_str());
        rename("temp.txt", filePath.c_str());

        if (found) {
            cout << "Customer account with account number " << accountNumber << " deleted successfully." << endl;
        } else {
            cout << "Account number " << accountNumber << " not found!" << endl;
        }
    }


    void viewAllCustomers() {
        ifstream inFile(filePath);
        if (!inFile) {
            cerr << "Error opening file for reading!" << endl;
            return;
        }
        cout << "\n--- All Customers ---" << endl;
        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }

    void viewAllLoans() {
        ifstream inFile("loans.txt");
        if (!inFile) {
            cerr << "Error opening loans file!" << endl;
            return;
        }

        string line;
        cout << "All Loans Details:" << endl;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }

    void viewRecentTransactions(int accountNumber) {
        ifstream inFile("transactions_" + to_string(accountNumber) + ".txt");
        if (!inFile) {
            cerr << "No recent transactions found for account number: " << accountNumber << endl;
            return;
        }
        cout << "\n--- Recent Transactions for Account Number: " << accountNumber << " ---" << endl;
        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
};

class Bank {
private:
    vector<Customer*> customers;
    
public:
    FileManager fileManager;
    Bank(string filePath) : fileManager(filePath) {
        customers = fileManager.loadAllCustomersFromFile();
    }

    ~Bank() {
        for (Customer* customer : customers) {
            delete customer;
        }
    }

    Customer* findCustomerByAccountNumber(int accountNumber) {
        for (Customer* customer : customers) {
            if (customer->getAccountNumber() == accountNumber) {
                return customer;
            }
        }
        return nullptr;
    }

    void deposit(Customer* customer, double amount) {
    if (customer == nullptr) {
        cout << "Customer not found!" << endl;
        return;
    }
    double newBalance = customer->getBalance() + amount;
    customer->setBalance(newBalance);
    fileManager.updateCustomerFile(customers);  
    fileManager.logTransaction(*customer, "Deposit", amount, newBalance);
    cout << "Amount Rs." << amount << " deposited successfully." << endl;
    cout << "New Balance: Rs." << newBalance << endl;
}

    void withdraw(Customer* customer, double amount) {
        if (customer == nullptr) {
            cout << "Customer not found!" << endl;
            return;
        }
        if (customer->getBalance() >= amount) {
            double newBalance = customer->getBalance() - amount;
            customer->setBalance(newBalance);
            fileManager.saveCustomerDetails(*customer);
            fileManager.logTransaction(*customer, "Withdraw", amount, newBalance);
            cout << "Amount Rs." << amount << " withdrawn successfully." << endl;
            cout << "New Balance: Rs." << newBalance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }
    
    void transferFunds(Customer* sender, Customer* receiver, double amount) {
    if (sender == nullptr || receiver == nullptr) {
        cout << "Sender or Receiver account not found!" << endl;
        return;
    }
    if (sender->getBalance() < amount) {
        cout << "Insufficient balance in sender's account!" << endl;
        return;
    }

    double newSenderBalance = sender->getBalance() - amount;
    double newReceiverBalance = receiver->getBalance() + amount;

    sender->setBalance(newSenderBalance);
    receiver->setBalance(newReceiverBalance);

    // Update file with new balances
    fileManager.updateCustomerFile(customers);

    // Log the transactions for both accounts
    fileManager.logTransaction(*sender, "Transfer Sent", amount, newSenderBalance);
    fileManager.logTransaction(*receiver, "Transfer Received", amount, newReceiverBalance);

    cout << "Transfer of Rs." << amount << " from Account " << sender->getAccountNumber()
         << " to Account " << receiver->getAccountNumber() << " completed successfully." << endl;
    cout << "Sender New Balance: Rs." << newSenderBalance << endl;
    cout << "Receiver New Balance: Rs." << newReceiverBalance << endl;
}

    
    void createCustomer(string name, string accountType, double initialBalance) {
    Customer* newCustomer;
    if (accountType == "Savings") {
        newCustomer = new SavingsCustomer(name, accountType, initialBalance);
    } else if (accountType == "Current") {
        newCustomer = new CurrentCustomer(name, accountType, initialBalance);
    } else if (accountType == "Student") {
        newCustomer = new StudentCustomer(name, accountType, initialBalance);
    } else {
        cout << "Invalid account type!" << endl;
        return;
    }
    customers.push_back(newCustomer);
    fileManager.updateCustomerFile(customers);  
    cout << "Account created successfully!" << endl;
    cout<<" Account Number: "<<newCustomer->getAccountNumber()<<endl;
}

    void applyLoan(Customer* customer, double principal, int years, const string& loanType) {
        if (customer == nullptr) {
            cout << "Customer not found!" << endl;
            return;
        }
        Loan loan;
        double monthlyPayment = loan.calculateMonthlyPayment(customer, principal, years, loanType);
        double totalPayable =loan.totalPayable(customer, principal, years, loanType);
        cout<<"Loan Granted!"<<endl;
        cout << "Monthly Payment for " << loanType << ": Rs." << monthlyPayment << endl;
        cout<<" Total payable amount for "<<loanType<<": Rs."<<totalPayable<<endl;
        fileManager.saveLoanDetails(loan, customer, principal, years, loanType);
    }

    void deleteCustomer(int accountNumber) {
        fileManager.deleteCustomerFile(accountNumber);
        customers.erase(remove_if(customers.begin(), customers.end(),
            [accountNumber](Customer* customer) {
                return customer->getAccountNumber() == accountNumber;
            }), customers.end());
    }

    void viewRecentTransactions(int accountNumber) {
        fileManager.viewRecentTransactions(accountNumber);
    }
};

int main() {
    Bank bank("customers.txt");
    int mainChoice;

    while (true) {
        cout << "\n1. User Panel\n2. Administrator Panel\n3. Exit\nEnter your choice: ";
        cin >> mainChoice;

        if (mainChoice == 3) {
            break;
        }

        switch (mainChoice) {
            case 1: { // User Panel
                int userChoice;
                cout << "\n--- User Panel ---" << endl;
                while (true) {
                    cout << "\n1. Deposit\n2. Withdraw\n3. Apply Loan\n4. Balance Inquiry\n5. Transfer Funds\n6. Exit User Panel\nEnter your choice: ";
                    cin >> userChoice;

                    if (userChoice == 6) {
                        break;
                    }

                    int accountNumber;
                    Customer* customer = nullptr;
                    switch (userChoice) {
                        case 1: {
                            double amount;
                            cout << "Enter Account Number: ";
                            cin >> accountNumber;
                            customer = bank.findCustomerByAccountNumber(accountNumber);
                            cout << "Enter Amount to Deposit: ";
                            cin >> amount;
                            bank.deposit(customer, amount);
                            break;
                        }
                        case 2: {
                            double amount;
                            cout << "Enter Account Number: ";
                            cin >> accountNumber;
                            customer = bank.findCustomerByAccountNumber(accountNumber);
                            cout << "Enter Amount to Withdraw: ";
                            cin >> amount;
                            bank.withdraw(customer, amount);
                            break;
                        }
                        case 3: {
                            double principal;
                            int years;
                            string loanType;
                            cout << "Enter Account Number: ";
                            cin >> accountNumber;
                            customer = bank.findCustomerByAccountNumber(accountNumber);
                            cout << "Enter Loan Principal: Rs.";
                            cin >> principal;
                            cout << "Enter Loan Duration (in years): ";
                            cin >> years;
                            cout << "Enter Loan Type (Home Loan/Car Loan/Gold Loan/Personal Loan/Education Loan): ";
                            cin.ignore();
                            getline(cin,loanType);
                            bank.applyLoan(customer, principal, years, loanType);
                            break;
                        }
                        case 4: {
                            cout << "Enter Account Number: ";
                            cin >> accountNumber;
                            customer = bank.findCustomerByAccountNumber(accountNumber);
                            if (customer) {
                                cout<<"Name :"<<customer->getName()<<endl;
                                cout << "Current Balance: Rs." << customer->getBalance() << endl;
                            } else {
                                cout << "Customer not found!" << endl;
                            }
                            break;
                        } 
                        case 5: { // Transfer Funds
                            int senderAccount, receiverAccount;
                            double amount;
                            cout << "Enter Sender Account Number: ";
                            cin >> senderAccount;
                            cout << "Enter Receiver Account Number: ";
                            cin >> receiverAccount;
                            cout << "Enter Transfer Amount: ";
                            cin >> amount;
                            bank.transferFunds(bank.findCustomerByAccountNumber(senderAccount), bank.findCustomerByAccountNumber(receiverAccount), amount);
                            break;
                        }
                        default:
                            cout << "Invalid choice!" << endl;
                            break;
                    }
                }
                break;
            }
            case 2: { // Administrator Panel
                string password;
                cout << "Enter Administrator Password: ";
                cin >> password;

                // Check the password
                if (password != "admin123") {
                    cout << "Incorrect password! Access denied." << endl;
                    break;
                }

                int adminChoice;
                cout << "\n--- Administrator Panel ---" << endl;
                while (true) {
                    cout << "\n1. Add Account\n2. Delete Account\n3. View All Customers\n4. View All Loans\n5. View Recent Transactions\n6. Exit Admin Panel\nEnter your choice: ";
                    cin >> adminChoice;

                    if (adminChoice == 6) {
                        break;
                    }

                    switch (adminChoice) {
                        case 1: {
                            string name, accountType;
                            double initialBalance;
                            cout << "Enter Name: ";
                            cin >> name;
                            cout << "Enter Account Type (Savings/Current/Student): ";
                            cin >> accountType;
                            cout << "Enter Initial Balance: ";
                            cin >> initialBalance;
                            bank.createCustomer(name, accountType, initialBalance);
                            break;
                        }
                        case 2: {
                            int accountNumber;
                            cout << "Enter Account Number to Delete: ";
                            cin >> accountNumber;
                            bank.deleteCustomer(accountNumber);
                            break;
                        }
                        case 3:
                            bank.fileManager.viewAllCustomers();
                            break;
                        case 4:
                            bank.fileManager.viewAllLoans();
                            break;
                        case 5: {
                            int accountNumber;
                            cout << "Enter Account Number to View Transactions: ";
                            cin >> accountNumber;
                            bank.viewRecentTransactions(accountNumber);
                            break;
                        }
                        default:
                            cout << "Invalid choice!" << endl;
                    }
                }
                break;
            }
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
