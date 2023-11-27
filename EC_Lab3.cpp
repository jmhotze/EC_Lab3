#include <iostream>
#include <iomanip>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual ~Account() = default;

    virtual void displayDetails() const {
        std::cout << "Account Details for " << getAccountType() << " Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    virtual void deposit(double amount) {
        balance += amount;
        std::cout << "Deposited $" << amount << " into the account." << std::endl;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the account." << std::endl;
        } else {
            std::cout << "Insufficient funds for withdrawal." << std::endl;
        }
    }

    friend Account operator+(const Account& lhs, const Account& rhs) {
        if (typeid(lhs) == typeid(rhs)) {
            Account result("Temp", "Temp", 0);  // Temporary account for the result
            result.balance = lhs.balance + rhs.balance;
        return result;
        } else { 
            std::cerr << "Error: Cannot add accounts of different types." << std::endl;
            return Account("Error","Error", 0);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "Account Details for " << account.getAccountType() << " Account (ID: " << account.accountNumber << "):" << std::endl;
        os << "   Holder: " << account.accountHolder << std::endl;
        os << "   Balance: $" << std::fixed << std::setprecision(2) << account.balance << std::endl;
        return os;
    }

    virtual std::string getAccountType() const {
        return "Generic";
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }

    void withdraw(double amount) override {
        const double minBalance = 100;  // Minimum balance required for a savings account
        if (balance - amount >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the savings account." << std::endl;
        } else {
            std::cout << "Withdrawal failed. Minimum balance requirement not met." << std::endl;
        }
    }

    std::string getAccountType() const override {
        return "Savings";
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double overdraft)
        : Account(number, holder, initialBalance), overdraftLimit(overdraft) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (balance - amount >= -overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the current account." << std::endl;
        } else {
            std::cout << "Withdrawal failed. Overdraft limit exceeded." << std::endl;
        }
    }

    std::string getAccountType() const override {
        return "Current";
    }
};

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);

    savings.displayDetails();
    current.displayDetails();

    // Transfer 300 from savings to current
    // Try as I can, I can't quite get the overloaded + operator to work. I hope I still receive at least partial extra credit, but I would need to figure out a way to allow the different account types to be added, which I can't really think of.
    current = dynamic_cast<CurrentAccount&>(current + savings);

    std::cout << "Account Details after transfer:" << std::endl;
    std::cout << savings;
    std::cout << current;

    return 0;
}