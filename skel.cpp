#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#define MAX 5
using namespace std;
 
// ---------------------------------------------------------
// 1. LIST INFORMATION (per assignment spec) - LOCK THESE NAMES FIRST
// ---------------------------------------------------------
struct Account {
    string accNo;       // 5 digits, kept as string to preserve leading zeros
    string accName;
    string birthday;    // "MM/DD/YYYY"
    string contactNum;
    double balance;      // Initial Deposit, min 5000
    string pin;          // stores ENCRYPTED pin, max 6 digits, 4 min
};

// ---------------------------------------------------------
// 2. ATMAccounts  (same shape/logic as Person, array-based List ADT)
// ---------------------------------------------------------
class ATMAccounts {
private:
    Account pd[MAX];
    int last;

    bool isFull();
    bool isEmpty();
    int locate(string accNo);      // same role as Person::locate, keyed on accNo
public:
    void makeNull();
    void addRec(Account x);
    void delRec(string accNo);
    int find(string accNo);        // public wrapper so Transactions can check enrollment/PIN
    Account& get(int index);       // access by index, needed by balance/withdraw/deposit/etc.
    void display();
    void save();
    void retrieve();
};

bool ATMAccounts::isFull() {
    return (last == MAX - 1);
}
bool ATMAccounts::isEmpty() {
    return (last == -1);
}
int ATMAccounts::locate(string accNo) {
    for (int i = 0; i <= last; i++)
        if (accNo == pd[i].accNo)
            return i;
    return -1;
}
void ATMAccounts::makeNull() {
    last = -1;
}
void ATMAccounts::addRec(Account x) {
    if (isFull()) {
        cout << "List is full.\n";
        system("pause");
    } else {
        last++;
        pd[last] = x;
    }
}
void ATMAccounts::delRec(string accNo) {
    int i, p;
    if (isEmpty()) {
        cout << "Nothing to delete.\n";
        system("pause");
    } else {
        p = locate(accNo);
        if (p < 0) {
            cout << "Not Found.\n";
            system("pause");
        } else {
            for (i = p; i < last; i++)
                pd[i] = pd[i + 1];
            last--;
        }
    }
}
int ATMAccounts::find(string accNo) {
    return locate(accNo);   // -1 if not found, else index
}
Account& ATMAccounts::get(int index) {
    return pd[index];
}
void ATMAccounts::display() {
    system("cls");
    cout << left << setw(10) << "ACC NO"
         << setw(20) << "NAME"
         << setw(12) << "BALANCE" << endl;
    for (int i = 0; i <= last; i++)
        cout << left << setw(10) << pd[i].accNo
             << setw(20) << pd[i].accName
             << setw(12) << pd[i].balance << endl;
    system("pause");
}
void ATMAccounts::save() {
    ofstream file("accounts.csv");
    if (!file) {
        cout << "File Error.";
        return;
    }
    for (int i = 0; i <= last; i++)
        file << pd[i].accNo << "," << pd[i].accName << "," << pd[i].birthday
             << "," << pd[i].contactNum << "," << pd[i].balance << "," << pd[i].pin << endl;
    file.close();
}
void ATMAccounts::retrieve() {
    ifstream file("accounts.csv");
    if (!file) {
        cout << "File Error.";
        return;
    }
    string line, bal;
    Account fd;
    while (getline(file, line)) {
        if (line.empty())
            continue;
        stringstream ss(line);
        getline(ss, fd.accNo, ',');
        getline(ss, fd.accName, ',');
        getline(ss, fd.birthday, ',');
        getline(ss, fd.contactNum, ',');
        getline(ss, bal, ',');
        fd.balance = stod(bal);
        getline(ss, fd.pin, ',');
        addRec(fd);
    }
    file.close();
}

// ---------------------------------------------------------
// 3. PIN ENCRYPTION - decide the scheme together, then implement
// ---------------------------------------------------------
string encryptPin(string pin) {
    // reverse ang pin then shift up by 3
    // e.g. user pin is 1234 -> 4321 -> 7654
    string reversed = "";
    for (int i = pin.length() - 1; i >= 0; i--) {   // start sa last digit
        reversed += pin[i];
    }

    string encrypted = reversed;
    for (int i = 0; i < encrypted.length(); i++) {
        int digit = encrypted[i] - '0'; // converts from string to int
            digit = (digit + 3) % 10;
            encrypted[i] = digit + '0'; // converts back from int to string
    }  return encrypted;
}
string decryptPin(string encrypted) {
    // exact opposite naman dito
    string shifted = encrypted;
    for (int i = 0; i < shifted.length(); i++) {
        int digit = shifted[i] - '0'; // converts from string to int
        digit = (digit + 10 - 3) % 10; // shift down by 3
        shifted[i] = digit + '0';
    }
    string original = "";
    for (int i = shifted.length() - 1; i >= 0; i--) {
        original += shifted[i]; // baliktarin back to original
    }    
    return original;
}

// ---------------------------------------------------------
// 4. FLASH DRIVE AS ATM CARD
// ---------------------------------------------------------
bool cardPresent(string drivePath) {
    ifstream file(drivePath + "pin.code");
    if (!file) {
        cout << "Please insert card.";
        return false;
    }
    return true;
}

void writeCard(string drivePath, string accNo, string encryptedPin) {
    ofstream file (drivePath + "pin.code");
        file << accNo << "," << encryptedPin << endl;
     file.close();
}
bool readCard(string drivePath, string &accNo, string &encryptedPin) {
    ifstream file (drivePath + "pin.code");
        if (!file) {
            cout << "File error!" << endl;
            return false;
        }
    string line;
    getline(file, line);
        stringstream ss(line);
        getline(ss, accNo, ',');
        getline (ss, encryptedPin);
    return true;
}

// ===========================================================
// MODULE 1: REGISTRATION   (JC)
// ===========================================================
void regAccount(ATMAccounts &list, string drivePath) {
    system("cls");
    if (list.isFull()) {
        cout << "System Error: Maximum number of accounts (" << MAX << ") reached.\n";
        cout << "Cannot register a new account at this time.\n";
        system("pause");
        return; 
    }
    Account acc;
    cout << "--- NEW ACCOUNT REGISTRATION ---\n";
    
    do {
        cout << "Enter 5-digit Account Number: ";
        cin >> acc.accNo;
        if (acc.accNo.length() != 5) 
            cout << "Error: Must be exactly 5 digits.\n";
        else if (list.find(acc.accNo) != -1) 
            cout << "Error: Account number already exists in the system.\n";
    } while (acc.accNo.length() != 5 || list.find(acc.accNo) != -1);
    
    cin.ignore();
    
    cout << "Enter Account Name: ";
    getline(cin, acc.accName);
    
    cout << "Enter Birthday (MM/DD/YYYY): ";
    getline(cin, acc.birthday);
    
    cout << "Enter Contact Number: ";
    getline(cin, acc.contactNum);
    
    do {
        cout << "Enter Initial Deposit (Min 5000): Php ";
        cin >> acc.balance;
        if (acc.balance < 5000)
            cout << "Error: Minimum initial deposit is Php 5000.\n";
    } while (acc.balance < 5000);
    
    string rawPin;
    do {
        cout << "Enter PIN Code (4 to 6 digits): ";
        cin >> rawPin;
        if (rawPin.length() < 4 || rawPin.length() > 6)
            cout << "Error: PIN must be between 4 and 6 digits.\n";
    } while (rawPin.length() < 4 || rawPin.length() > 6);

    acc.pin = encryptPin(rawPin);
    list.addRec(acc);
    writeCard(drivePath, acc.accNo, acc.pin);
    list.save();
    
    cout << "\nRegistration Successful! ATM Card data written.\n";
    system("pause");
}

// ===========================================================
// MODULE 2: TRANSACTIONS
// ===========================================================
// All transaction functions should call cardPresent() first, then
// list.find() + compare decryptPin() against the card's PIN.

// --- JC: Balance Inquiry + Change PIN ---
void balInquiry(ATMAccounts &list, string accNo) {
    int i = list.find(accNo);
    cout << "\nAccount Name: " << list.get(i).accName;
    cout << "\nCurrent Balance: Php " << fixed << setprecision(2) << list.get(i).balance << "\n";
    system("pause");
}

void changePin(ATMAccounts &list, string drivePath, string accNo) {
    int i = list.find(accNo);
    string newPin;
    
    do {
        cout << "Enter New PIN (4 to 6 digits): ";
        cin >> newPin;
        if (newPin.length() < 4 || newPin.length() > 6)
            cout << "Error: PIN must be between 4 and 6 digits.\n";
    } while (newPin.length() < 4 || newPin.length() > 6);
    
    list.get(i).pin = encryptPin(newPin);
    writeCard(drivePath, accNo, list.get(i).pin);
    list.save(); 
    
    cout << "PIN Code successfully changed.\n";
    system("pause");
}

// --- DEX: Withdraw, Deposit, Fund Transfer ---
void withdraw(ATMAccounts &list, string accNo, double amount) {
    int i = list.find(accNo);
    if (i < 0) {
        cout << "Account not found.\n";
    } else if (amount <= 0) {
        cout << "Error: Amount must be greater than zero.\n";
    } else if (amount > list.get(i).balance) {
        cout << "Error: Insufficient balance.\n";
    } else {
        list.get(i).balance -= amount;
        list.save();
        cout << "Withdrawal successful.\n";
        cout << "New Balance: Php " << fixed << setprecision(2) << list.get(i).balance << "\n";
    }
    system("pause");
}
void deposit(ATMAccounts &list, string accNo, double amount) {
    // TODO: find index, update balance, list.save()
}
void fundTransfer(ATMAccounts &list, string fromAcc, string toAcc, double amount) {
    // TODO: list.find(toAcc) must be >= 0 (enrolled accounts only),
    // then subtract from fromAcc, add to toAcc
}

// ===========================================================
// MAIN DRIVER
// ===========================================================
int menu();

int main() {
    ATMAccounts list;
    list.makeNull();
    list.retrieve();

    string drivePath = "D:/";   // adjust per testing setup

    while (true) {
        switch (menu()) {
        case 1: /* regAccount(list, drivePath); */ break;
        case 2: /* transaction menu -> check cardPresent() first */ break;
        case 3: list.display(); break;
        case 4: list.save(); exit(0);
        default: cout << "Invalid input.\n"; system("pause");
        }
    }
    return 0;
}
int menu() {
    int op;
    system("cls");
    cout << "ATM MENU\n";
    cout << "1. Register New Account\n";
    cout << "2. Transactions\n";
    cout << "3. Display All Accounts (testing only)\n";
    cout << "4. Exit\n";
    cout << "Select[1-4]: ";
    cin >> op;
    return op;
}
