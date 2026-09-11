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
    // TODO: e.g. shift each digit by a fixed key
    // para raw di kita sa saved file yung real pin, shift tayo up by 3 digits
    // so like eg. 1234 real pin, magiging 4567
    return pin;
}
string decryptPin(string encrypted) {
    // TODO: reverse of encryptPin
    // exact opposite naman dito
    // eg. encrypted pin is 4567, magiging 1234 which is nung real pin
    return encrypted;
}

// ---------------------------------------------------------
// 4. FLASH DRIVE AS ATM CARD
// ---------------------------------------------------------
bool cardPresent(string drivePath) {
    // TODO: check for pin.code file at drivePath; if missing, print
    // "Please insert card." and return false
    return false;
}
void writeCard(string drivePath, string accNo, string encryptedPin) {
    // TODO: write accNo + encryptedPin to drivePath + "/pin.code"
}
bool readCard(string drivePath, string &accNo, string &encryptedPin) {
    // TODO: read accNo + encryptedPin from drivePath + "/pin.code"
    return false;
}

// ako na from encryptPin to readCard

// ===========================================================
// MODULE 1: REGISTRATION   (JC)
// ===========================================================
void regAccount(ATMAccounts &list, string drivePath) {
    Account acc;
    // TODO:
    // 1. Prompt accName, birthday, contactNum, initial deposit (validate >= 5000)
    // 2. Generate/prompt 5-digit accNo, check list.find() to avoid duplicates
    // 3. Prompt PIN (4 digits + ENTER key, max 6 digits)
    // 4. acc.pin = encryptPin(rawPin)
    // 5. list.addRec(acc)
    // 6. writeCard(drivePath, acc.accNo, acc.pin)
    // 7. list.save()
}

// ===========================================================
// MODULE 2: TRANSACTIONS
// ===========================================================
// All transaction functions should call cardPresent() first, then
// list.find() + compare decryptPin() against the card's PIN.

// --- JC: Balance Inquiry + Change PIN ---
void balInquiry(ATMAccounts &list, string accNo) {
    // TODO: int i = list.find(accNo); display list.get(i).balance
}
void changePin(ATMAccounts &list, string drivePath, string accNo, string newPin) {
    // TODO: encryptPin(newPin), update pd[i].pin via list.get(i), rewrite card file
}

// --- DEX: Withdraw, Deposit, Fund Transfer ---
void withdraw(ATMAccounts &list, string accNo, double amount) {
    // TODO: find index, validate sufficient balance, update balance, list.save()
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