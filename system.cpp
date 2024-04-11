#include <iostream>
#include <fstream> // File Handling
#include <vector>
#include <sstream>
#include <windows.h>
using namespace std;

struct transacInfo {
    string date;
    int count;
};

void adminModule();
void clientModule();
void seeRecord();
void showViolations();
void payDues();
void viewTransactions();
void encodeRecord();
void addViolations();
int calculatePenalty(int fines, int days);
int calculateDays();
int setDaysinMonth(int currentMonth, int currentYear);
transacInfo getBusyDay(vector<string>list);

int main ()
{
    // TODO: Menu
    // Transaction Log-in: as Admin or as Client
    // Admin - Modify Record
    // User - See Record, Pay Record, List/ Price Violation
    int op;
    cout << "\t\t ===================================================== \t\t\n";
    cout << "\t\t       Welcome to LTO Violation Management System      \t\t\n";
    cout << "\t\t ===================================================== \t\t\n";
    do {
        cout << "[1]. Admin \n[2]. Client \nTransact as: ";
        cin >> op;

        if (op == 1) {
            system("cls");
            adminModule();
        } else if (op == 2) {   
            system("cls");      
            clientModule();
        } else {
            cout << "Please choose from the 2 options only.\n";
        }
    } while (op > 2);
    system("cls");
    return 0;
}

void adminModule() 
{
    int ch;
    string password;
    do {
        cout << "Enter Password: ";
        cin >> password;
        if (password != "admin") {
            cout << "Wrong password. Try Again.\n";
        }
    } while (password != "admin");

    cout << "Login successful. Welcome Admin!\n";
    do {
        cout << "\t\t ===== ADMIN MENU ===== \t\t\n";
        cout << "[1]. Encode to Records \n[2]. Add List of Violations \n[3]. View Transactions \n[4]. Exit \n";
        cout << "Choice: ";
        cin >> ch;
        switch (ch)
        {
            case 1:
                system("cls");
                encodeRecord();
                break;
            case 2:
                system("cls");
                addViolations();
                break;
            case 3:                
                system("cls");
                viewTransactions();
                break;
            case 4:
                return;
            default:
                cout << "Invalid Option.\n";
                break;
        }
    } while (ch != 4);
}

void clientModule() 
{
    int ch;
    string password; 
    do {
        cout << "Enter Password: ";
        cin >> password;
        if (password != "client") {
            cout << "Wrong password. Try Again.\n";
        } 
    } while (password != "client");

    cout << "Login successful. Welcome Client!\n";
    do {
        cout << "\t\t ===== CLIENT MENU ===== \t\t\n";
        cout << "[1]. See Personal Record \n[2]. Pay Dues \n[3]. Show List of Violations \n[4]. Exit \n";
        cout << "Choice: ";
        cin >> ch;
        switch (ch)
        {
            case 1:
                system("cls");
                seeRecord();
                break;
            case 2:
                system("cls");
                payDues();
                break;
            case 3:
                system("cls");            
                showViolations();
                break;
            case 4:
                return;
            default:
                cout << "Invalid Option.\n";
                break;
        }
    } while (ch != 4);
    cout << "Thanks for using our Management System!\n";
}

void seeRecord() {
    fstream infile; // Declare File Pointer
    string name;
    bool isFound = false;

    // Open File
    infile.open("records.csv", ios::in);
    if (!infile.is_open()) {
        cout << "Cannot open file.\n";
        return;
    }
    // Prompt for name search
    cout << "Search for a name: ";
    cin.ignore();
    getline(cin, name);

    vector<string> row;
    string line, word;

    // Load the data in a buffer
    while (getline(infile, line)) {

        row.clear();        // Clear to handle the next line of the file
        stringstream ss(line);        // Break the line into words

        // Extract values separated by commas
        while (getline(ss, word, ',')) {
            row.push_back(word);
        }

        // Debugging output
        //for (const auto& field : row) {
        //    cout << "[" << field << "] ";
        //}

        // Check if vector is not empty, has at least 3 elements, and row[2] matches the name you searched for
        if (!row.empty() && row.size() > 2 && row[2] == name) {
            isFound = true;

            // Process the date to display it correctly
            stringstream dateStream(row[0]);
            string month, day, year;
            dateStream >> month >> day >> year;

            // Display Information        
            cout << "=============================================\n";
            cout << "Date: " << month << " " << day << " " << year << "\n";
            cout << "Time: " << row[1] << "\n";
            cout << "Full Name: " << row[2] << "\n";
            cout << "Violation Code: " << row[3] << "\n";
            cout << "License Number: " << row[4] << "\n";
            cout << "Plate Number: " << row[5] << "\n";
            cout << "Fine: " << row[6] << "\n";
            cout << "Status: " << row[7] << "\n";
        }
    }

    if (!isFound) {
        cout << "No record found for '" << name << "'.\n";
    }
    infile.close(); // Close the file after reading
    system("pause");
    system("cls");
}

void payDues()
{
    fstream fin, fout;
    vector<string> row;
    string name, date, type, searchName, line, word;
    int totalPenalty, daysDelayed, cashOnHand, totalFines = 0;
    string infile = "records.csv";
    string outfile = "transactions.csv";
    bool isAdded = false, isFound = false;
    // TODO: Open File
    fin.open(infile, ios::in);
    fout.open(outfile, ios::app);
    // Check for NULL
    if (!fin.is_open())
    {
        cout << "Cannot open file: " << infile << "\n";
        return;
    } else if (!fout.is_open()) {
        cout << "Cannot open file: " << outfile << "\n";
        return;        
    }
    // Search Name, if name found, display fines
    cout << "Search Name: ";
    cin.ignore();
    getline(cin, searchName);

    while (getline(fin, line))
    {
        row.clear();
        stringstream extract(line);
        // Extract values
        while (getline(extract, word, ',')) {
            row.push_back(word);
        }
        if (!row.empty() && row.size() > 2 && row[2] == searchName)
        {
            isFound = true;
            cout << "Name found. You have Pending Balances:\n";
            cout << "Date: " << row[0] << "\n";
            cout << "Fines: " << row[6] << "\n";
            totalFines += stoi(row[6]);
        }
    }

    if (isFound)
    {
        cout << "Total Fines: " << totalFines << "\n";
        // Calculate total payment with penalty.
        daysDelayed = calculateDays();
        totalPenalty = calculatePenalty(totalFines, daysDelayed);
        cout << "\t\t === PAYMENT INFORMATION === \t\t\n";
        cout << "Number of Days Delayed: " << daysDelayed << "\n";
        cout << "Total Amount w/ Penalty Applied: " << totalPenalty << "\n";
        do {
            cout << "Enter Cash on Hand: ";
            cin >> cashOnHand;

        } while (cashOnHand < totalPenalty);
        int change = cashOnHand - totalPenalty;
        // Enter Info: name, date, amount, transac type
        cout << "Please enter the following informations to save your transactions.\n";
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Date (Month Day Year): ";
        getline(cin, date);
        cout << "Transaction Type: ";
        getline(cin, type);
        // If paid, append to transactions file
        fout << name << "," << date << "," << cashOnHand << "," << type << "\n";
        isAdded = true;
        if (!isAdded)
        {
            cout << "There was an error adding the information to the database.\n";
        }

        cout << "Information successfully added to the database.\n";
        system("cls");

        cout << "Thank you for your payment. Please review the details provided in your Official Receipt.\n";
        cout << "\t\t == OFFICIAL RECEIPT == \t\t\n";
        cout << "Name: " << name << "\n";
        cout << "Date: " << date << "\n";
        cout << "Transaction Type: " << type << "\n";
        cout << "Amount Paid: " << cashOnHand << "\n";
        cout << "Change: " << change << "\n";
    } else {
        cout << "No record found for " << searchName << ". \n";        
    }

    // Close File
    fin.close();
    fout.close();
    system("pause");
    system("cls");
}

void showViolations()
{
    string line, data; 
    vector<string>row;
    string filepath = "violations.csv";
    // TODO: Open Violations File
    fstream file(filepath, ios::in);

    if (!file.is_open()) {
        cout<<"Error opening file: "<< filepath << "\n";
        return;
    }
    // Load line in a Buffer & Print each line
    while (getline(file, line)) {
        stringstream extract(line);
        row.clear();

        while (getline(extract, data, ',')) {
            row.push_back(data);
        }
        cout << row[0] << " | " << row[1] << " | " << row[2] << " | " << row[3] << "\n";
    }
    // Close File 
    file.close();
    system("pause");
    system("cls");
}

void viewTransactions()
{
    fstream tin;
    vector<string> row, datesList;
    transacInfo mostTransac;
    string line, data;
    int transacAmount = 0, count = 0, avgAmnt = 0;
    // Open Transactions File
    tin.open("transactions.csv", ios::in);
    if (!tin.is_open()) {
        cout << "Cannot open file.\n";
        return;
    }
    // Load line in a Buffer
    cout << "\t\t == Transaction Record == \t\t\n";
    cout << "Name | Date | Amount Paid | Transaction Type \n";
    while (getline(tin, line)) {
        stringstream extract(line);
        row.clear();

        while (getline(extract, data, ',')) {
            row.push_back(data);
        }
            datesList.push_back(row[1]); 

        // Process Transactions
        transacAmount += stoi(row[2]);        
        cout << row[0] << " | " << row[1] << " | " << row[2] << " | " << row[3] << "\n";
        count++;
    }
    // TODO: Transaction Analytics
    cout << "\t\t ===== TRANSACTION SUMMARY REPORT ===== \t\t\n";
    cout << "Total Transactions Made: " << count << "\n";
    cout << "Total Amount Gained: " << transacAmount << "\n";
    avgAmnt = (count > 0) ? static_cast<double>(transacAmount) / count : 0;
    cout << "Average Transaction Amount Gained: " << avgAmnt << "\n";
    mostTransac = getBusyDay(datesList);
    cout << "Day with Most Transactions: " << mostTransac.date << "\n";
    cout << "Transactions Made within that Day: " << mostTransac.count << "\n";
    cout << "\t\t ===========  END OF REPORT  ========== \t\t\n";
    // Close File 
    tin.close();
    system("pause");
    system("cls");
}

void encodeRecord()
{
    fstream erin;
    string date, time, fullName, vioCode, liscNum, plateNum, fine, status;
    bool isAdded = false;
    // TODO: Open File (records.csv)
    erin.open("records.csv", ios::app);
    if (!erin.is_open()) {
        cout << "Cannot open file.\n";
        return;
    }
    // Enter Info (in Order): Date, Time, Fullname, Offense, License Number, Plate Number, Fine, Status
    cout << "Please input the following information to encode your record.\n";
    cout << "Enter Date (Month Day Year): ";
    cin.ignore();
    getline(cin, date);
    cout << "Enter Time (Military Format HH:MM): ";
    getline(cin, time);
    cout << "Enter Fullname: ";
    getline(cin, fullName);
    cout << "Enter Violation Code: ";
    getline(cin, vioCode);
    cout << "Enter License Number: ";
    getline(cin, liscNum);
    cout << "Enter Plate Number: ";
    getline(cin, plateNum);
    cout << "Enter Fine: ";
    getline(cin, fine);
    cout << "Enter Status: ";
    getline(cin, status);

    // Append to File
    erin << date << "," << time << "," << fullName << "," << vioCode << "," << liscNum << "," << plateNum << "," << fine << "," << status << "\n";
    isAdded = true;
    if (!isAdded)
    {
        cout << "There was an error adding the information to the database.\n";
    }
    cout << "Information successfully added to the database.\n";
    // Close File
    erin.close();

    system("pause");
    system("cls");
}

void addViolations()
{
    string violation, fine, penalty, line, desc;
    string filepath = "violations.csv";
    bool isAdded = false;
    // TODO: Open File (violations.csv)
    fstream file(filepath, ios::app);
    
    //Check if file is opened
    if (!file.is_open()) {
        cout<< "Failed to open the file: " << filepath << "\n";
        return;
    }

    //Ask user to enter info in order: Violation Code, Violation Description, Fine, Penalty
    cout<<"Enter Violation Code: ";
    cin.ignore();
    getline(cin, violation);
    cout<<"Enter Violation Description: ";
    getline(cin, desc);
    cout<<"Enter Fine: ";
    getline(cin, fine);
    cout<<"Enter Additional Penalty: ";
    getline(cin, penalty);
        
    //Append the info to file
    file << violation << "," << desc << "," << fine << "," << penalty << "\n";
    isAdded = true;
    if (!isAdded)
    {
        cout << "There was an error adding the information to the database.\n";
    }

    cout << "Information successfully added to the database.\n";  
    //Close file    
    file.close();

    system("pause");
    system("cls");
}

int calculatePenalty(int fines, int days)
{
    // TODO: If days <= 15, return fines
    // Else if days > 15 && days <= 30, return fines + 150
    // Else return fines + days / 7 * 150
    if (days <= 15) {
        return fines;
    } else if (days > 15 && days <= 30) {
        return fines + 100;
    } else {
        return fines + (days / 7 * 150);
    }
}

int calculateDays()
{
    int issuanceDay, issuanceMonth, issuanceYear;
    int currentDay, currentMonth, currentYear;

    do { 
        cout << "Please input the date of the issuance of the offense and the date today in numeric form.\n";
        cout << "\t\t= Issuance Date =\t\t\n";
        cout << "Year: ";
        cin >> issuanceYear;
        cout << "Month: ";
        cin >> issuanceMonth;
        cout << "Day: ";
        cin >> issuanceDay;
        cout << "\t\t= Current Date =\t\t\n";
        cout << "Year: ";
        cin >> currentYear;
        cout << "Month: ";
        cin >> currentMonth;
        cout << "Day: ";
        cin >> currentDay;

        if (issuanceYear > currentYear || (issuanceYear < 1600 || currentYear < 1600))
        {
            cout << "Please Enter a Valid Year.\n";
        }
        else 
        {
            cout << "Please Enter a Valid Date.\n";
        }

    } while (issuanceYear > currentYear || (issuanceYear < 1600 || currentYear < 1600) || (issuanceYear == currentYear && issuanceMonth > currentMonth) || (issuanceYear == currentYear && issuanceMonth == currentMonth && issuanceDay > currentDay));

    // TODO: Calculate the days between two dates
    int days = 0;

    while (!(issuanceYear == currentYear && issuanceMonth == currentMonth && issuanceDay == currentDay)) {
        days++;
        issuanceDay++;

        if (issuanceDay > setDaysinMonth(issuanceMonth, issuanceYear)) {
            issuanceDay = 1;
            issuanceMonth++;

            if (issuanceMonth > 12) {
                issuanceMonth = 1;
                issuanceYear++;
            }
        }
    }
    return days;
}

int setDaysinMonth(int currentMonth, int currentYear)
{
    switch (currentMonth)
    {
        case 2:
            return (currentYear % 4 == 0) ? 29 : 28;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        default:
            return 30;
    }
}

transacInfo getBusyDay(vector<string>list)
{
    int count = 1, maxVal = 1;
    int listSize = list.size();
    vector<transacInfo> dates(listSize);
    transacInfo busiestDay;
    // Populate dates vector
    for (int i = 0; i < listSize; i++)
    {
        dates[i].date = list[i];
        dates[i].count = 1;
        for (int j = i + 1; j < listSize; j++)
        {
            if (list[i] == list[j])
            {
                dates[i].count++;
            }
        }
        if (dates[i].count > maxVal) {
            maxVal = dates[i].count;
            busiestDay = dates[i];
        }
    }
    return busiestDay;
}