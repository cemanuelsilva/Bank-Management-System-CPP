#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------//
//------------------------------- CLASSES ---------------------------------------------------//
//-------------------------------------------------------------------------------------------//

class User {
public:
    std::string username;
    std::string password;
    float money;
    std::vector<std::string> transactionHistory;

    User(const std::string& username, const std::string& password)
        : username(username), password(password), money(0.0) {}
};

class Bank {
private:
    std::unordered_map<std::string, User> users;

public:
    void addUser(const std::string& username, const std::string& password) {
        users.emplace(username, User(username, password));
    }

    User* getUser(const std::string& username) {
    auto it = users.find(username);
    return (it != users.end()) ? &it->second : nullptr;
}

    void displayUserCredentials() const {
        for (const auto& entry : users) {
            std::cout << "Username: " << entry.second.username << ", Password: " << entry.second.password << std::endl;
        }
    }
};

Bank bank;  // Instantiate the Bank class


//-------------------------------------------------------------------------------------------//
//----------------------------- FUNCTIONS ---------------------------------------------------//
//-------------------------------------------------------------------------------------------//


void clearConsole() {
    std::cout << "\033[2J\033[H";
}

void register_User() {
    std::string username;
    std::string password;

    std::cout << "---------------" << std::endl;
    std::cout << "Register" << std::endl;
    std::cout << "---------------" << std::endl;

    while (true) {
        std::cout << "Insert your username: ";
        std::cin >> username;

        if (bank.getUser(username) != nullptr) {
            clearConsole();
            std::cout << "Username already used." << std::endl;
        } else {
            break;
        }
    }

    std::cout << "Insert your password: ";
    std::cin >> password;

    std::cout << "Completed the register!\n" << std::endl;
    bank.addUser(username, password);
}

void displayTransactionHistory(const User& user) {
    std::cout << "---------------" << std::endl;
    std::cout << "Transaction History" << std::endl;
    std::cout << "---------------" << std::endl;

    for (const auto& transaction : user.transactionHistory) {
        std::cout << transaction << std::endl;
    }

    std::cout << "---------------" << std::endl;
}

void checkMoney(const User& user) {
    std::cout << "Name: " << user.username << std::endl;
    std::cout << "Balance: " << user.money << std::endl;
}

void sendMoney(User& user) {
    std::string importUsername;
    float importQuantity;

    std::cout << "Insert the username to transfer to: ";
    std::cin >> importUsername;

    while (true) {
        std::cout << "Insert the balance to transfer: ";
        std::cin >> importQuantity;

        if (user.money < importQuantity) {
            clearConsole();
            std::cout << "Insufficient balance." << std::endl;
        } else {
            user.money -= importQuantity;
            User* user_import = bank.getUser(importUsername);

            if (user_import != nullptr) {
                user_import->money += importQuantity;
                std::cout << "Transfer successful." << std::endl;
                std::cout << "Your new balance: " << user.money << std::endl;
                std::cout << "Receiver's new balance: " << user_import->money << std::endl;
                user.transactionHistory.push_back("Transfer to " + user_import->username + ": -" + std::to_string(importQuantity));
            } else {
                std::cout << "Receiver not found." << std::endl;
                // Refund the money to the sender if receiver not found
                user.money += importQuantity;
                std::cout << "Refunded. Your new balance: " << user.money << std::endl;
            }
            break;
        }
    }
}

void addMoney(User& user) {
    float importMoney;

    std::cout << "How much do you want to import? ";
    std::cin >> importMoney;

    user.money += importMoney;

    std::cout << "New balance: " << user.money << std::endl;
    user.transactionHistory.push_back("Deposit: +" + std::to_string(importMoney));
}


void withdrawMoney(User& user) {
    float withdrawMoney;

    while(true){
    
        std::cout << "How much do you want to withdraw? " << std::endl;;
        std::cin >> withdrawMoney;

        if(withdrawMoney > user.money){
            std::cout << "Balance insuficient. " << std::endl;;
        }
        else{
            break;
        }
    }
    
    user.money -= withdrawMoney;
    std::cout << "New balance: " << user.money << std::endl;
    user.transactionHistory.push_back("Withdraw: -" + std::to_string(withdrawMoney));
}

void login_menu(const User& user) {
    int menu_choice = 0;

    std::cout << "---------------" << std::endl;
    std::cout << "Welcome " + user.username << std::endl;
    std::cout << "---------------" << std::endl;

    while (menu_choice != 6) {
        std::cout << "---------------" << std::endl;
        std::cout << "Bank Management" << std::endl;
        std::cout << "---------------" << std::endl;

        std::cout << "1 - Check money" << std::endl;
        std::cout << "2 - Send Money" << std::endl;
        std::cout << "3 - Deposit Money" << std::endl;
        std::cout << "4 - Withdraw Money" << std::endl;
        std::cout << "5 - Transaction History" << std::endl;
        std::cout << "6 - Exit" << std::endl;
        std::cout << "---------------" << std::endl;

        std::cout << "Insert your option: ";
        std::cin >> menu_choice;

        switch (menu_choice) {
            case 1:
                clearConsole();
                checkMoney(user);
                break;
            case 2:
                clearConsole();
                sendMoney(const_cast<User&>(user));
                break;
            case 3:
                clearConsole();
                addMoney(const_cast<User&>(user));  // Pass a non-const reference using const_cast
                break;    
            case 4:
                clearConsole();
                withdrawMoney(const_cast<User&>(user));  // Pass a non-const reference using const_cast
                break;
            case 5:
                clearConsole();
                displayTransactionHistory(user);
                break;
            case 6:
                break;    
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 6.\n" << std::endl;
                break;
        }
    }
}


void login() {
    std::string username;
    std::string password;

    std::cout << "---------------" << std::endl;
    std::cout << "Login" << std::endl;
    std::cout << "---------------" << std::endl;

    while (true) {
        std::cout << "Insert your username: ";
        std::cin >> username;

        const User* user = bank.getUser(username);

        if (user == nullptr) {
            clearConsole();
            std::cout << "Wrong username." << std::endl;
        } else {
            break;
        }
    }

    while (true) {
        std::cout << "Insert your password: ";
        std::cin >> password;

        const User* user = bank.getUser(username);

        if (user != nullptr && user->password == password) {
            clearConsole();
            login_menu(*user);  // Pass a reference, not a pointer
            break;
        } else {
            clearConsole();
            std::cout << "Wrong password." << std::endl;
        }
    }
}

int menu() {
    int menu_choice = 0;

    while (menu_choice != 3) {
        std::cout << "---------------" << std::endl;
        std::cout << "Bank Management" << std::endl;
        std::cout << "---------------" << std::endl;

        std::cout << "1 - Login" << std::endl;
        std::cout << "2 - Register" << std::endl;
        std::cout << "3 - Debug Entries" << std::endl;
        std::cout << "---------------" << std::endl;

        std::cout << "Insert your option: ";
        std::cin >> menu_choice;

        switch (menu_choice) {
        case 1:
            clearConsole();
            login();
            return 0;
        case 2:
            clearConsole();
            register_User();
            break;
        case 3:
            clearConsole();
            bank.displayUserCredentials();
            break;
        default:
            std::cout << "Invalid choice. Please enter a number between 1 and 3.\n" << std::endl;
            break;
        }
    }
    return 1;  // Return false when the user chooses to exit the program
}

//-------------------------------------------------------------------------------------------//
//------------------------------- MAIN ------------------------------------------------------//
//-------------------------------------------------------------------------------------------//



int main() {

    menu();

    return 0;
}
