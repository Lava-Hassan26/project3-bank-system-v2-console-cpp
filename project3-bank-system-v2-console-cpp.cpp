#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string clientsFileName = "Clients.txt";
const string delim = "#//#";

void MainMenuScreen();

void TransactionsMenuScreen();

bool IsClientExistInFile(string, string);

enum enMainMenu { ShowClientsList = 1, AddClient, DeleteClient, UpdateClientInfo, FindClient,Transactions, Exit };

enum enTransactionsMenu { Deposit = 1, Withdraw, TotalBalances, MainMenu };

struct stClient
{
    string accountNumber = "";
    string pinCode = "";
    string fullName = "";
    string phoneNumber = "";
    double accountBalance = 0.0;
    bool isMarkedForDelete = false;
};

string ReadString(string message)
{
    string text = "";
    cout << message;
    getline(cin >> ws, text);
    return text;
}

double ReadDoubleNumber(string message)
{
    double number = 0;
    cout << message;
    cin >> number;
    while (cin.fail())
    {
        cin.clear(); // resets the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cout << "INVALIDE! Enter a valid number: ";
        cin >> number;
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return number;
}

string ReadAccountNumber()
{
    string accountNumber = "";
    accountNumber = ReadString("Enter the account number: ");
    return accountNumber;
}

stClient ReadClientInfo()
{
    stClient client;
    client.accountNumber = ReadString("\nEnter Account Number: ");

    while (IsClientExistInFile(client.accountNumber, clientsFileName))
    {
        cout << "\nClient with [" << client.accountNumber << "] already exists!\n";
        client.accountNumber = ReadString("\nEnter Account Number: ");
    }
    client.pinCode = ReadString("\nEnter Pin Code: ");
    client.fullName = ReadString("\nEnter Full Name: ");
    client.phoneNumber = ReadString("\nEnter Phone Number: ");
    client.accountBalance = ReadDoubleNumber("\nEnter Account Balance: ");

    return client;
}

stClient ReadUpdatedClientInfo(string accountNumber)
{
    stClient client;
    client.accountNumber = accountNumber;
    client.pinCode = ReadString("\nEnter Pin Code: ");
    client.fullName = ReadString("\nEnter Full Name: ");
    client.phoneNumber = ReadString("\nEnter Phone Number: ");
    client.accountBalance = ReadDoubleNumber("\nEnter Account Balance: ");

    return client;
}

enMainMenu ReadMainMenuUserChoice()
{
    short userChoice = 0;
    do
    {
        userChoice = ReadDoubleNumber("Enter your choice [1-7]: ");

    } while (userChoice < 1 || userChoice > 7);
    return (enMainMenu)userChoice;
}

enTransactionsMenu ReadTransactionsMenuUserChoice()
{
    short userChoice = 0;
    do
    {
        userChoice = ReadDoubleNumber("Enter your choice [1-4]: ");

    } while (userChoice < 1 || userChoice > 4);
    return (enTransactionsMenu)userChoice;
}

vector<string> SplitStringByDelimiter(string inputText, string delim = " ")
{
    vector<string> vWords;
    string word = "";
    short delimiterPosition = 0;
    delimiterPosition = inputText.find(delim);

    while ((delimiterPosition) != std::string::npos)
    {
        word = inputText.substr(0, delimiterPosition);
        if (word != "")
            vWords.push_back(word);
        inputText.erase(0, delimiterPosition + delim.length());
        delimiterPosition = inputText.find(delim);
    }
    if (inputText != "")
        vWords.push_back(inputText);
    return vWords;
}

string ConvertClientRecordToDataLine(stClient client, string delim)
{
    string line = "";
    line += client.accountNumber + delim;
    line += client.pinCode + delim;
    line += client.fullName + delim;
    line += client.phoneNumber + delim;
    line += to_string(client.accountBalance);
    return line;
}

stClient ConvertDataLineToClientRecord(string lineData, string delim)
{
    vector<string> vClient = SplitStringByDelimiter(lineData, delim);
    stClient client;
    client.accountNumber = vClient[0];
    client.pinCode = vClient[1];
    client.fullName = vClient[2];
    client.phoneNumber = vClient[3];
    client.accountBalance = stof(vClient[4]);

    return client;
}

void AddClientToFile(stClient client, string fileName, string delim)
{
    fstream clientsFile;
    clientsFile.open(fileName, ios::out | ios::app);
    if (clientsFile.is_open())
    {
        string line = ConvertClientRecordToDataLine(client, delim);
        clientsFile << line << endl;
        clientsFile.close();
    }
}

void SaveAllClientsToFile(vector<stClient>& vClients, string fileName, string delim)
{
    fstream clientsFile;
    clientsFile.open(fileName, ios::out);

    if (clientsFile.is_open())
    {
        string currentLline = "";
        for (stClient& client : vClients)
        {
            if (!client.isMarkedForDelete)
            {
                currentLline = ConvertClientRecordToDataLine(client, delim);
                clientsFile << currentLline << endl;
            }
        }
        clientsFile.close();
    }
}

vector<stClient> LoadAllClientsFromFile(string fileName, string delim)
{
    vector<stClient> vClients;
    fstream clientsFile;
    clientsFile.open(fileName, ios::in);

    if (clientsFile.is_open())
    {
        stClient client;
        string currentLline = "";
        while (getline(clientsFile, currentLline))
        {
            client = ConvertDataLineToClientRecord(currentLline, delim);
            vClients.push_back(client);
        }
        clientsFile.close();
    }
    return vClients;
}

bool IsClientExistInFile(string accountNumber, string fileName)
{
    vector <stClient> vClients;
    fstream clientsFile;
    clientsFile.open(fileName, ios::in);//read Mode
    if (clientsFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(clientsFile, Line))
        {
            Client = ConvertDataLineToClientRecord(Line, delim);
            if (Client.accountNumber == accountNumber)
            {
                clientsFile.close();
                return true;
            }
        }
        clientsFile.close();
    }
    return false;
}

bool IsClientExistInVector(string accountNumber, vector<stClient>& vClients, stClient& client)
{
    for (stClient& c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            client = c;
            return true;
        }
    }
    return false;
}

bool MarkClientForDeletion(string accountNumber, vector<stClient>& vClients)
{
    for (stClient& c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            c.isMarkedForDelete = true;
            return true;
        }
    }
    return false;
}

void PrintClientCardDetails(const stClient& client)
{
    cout << "\nThe Following are the Client Details";
    cout << "\n***************************************";
    cout << "\nAccount Number : " << client.accountNumber;
    cout << "\nPin Code       : " << client.pinCode;
    cout << "\nFull Name      : " << client.fullName;
    cout << "\nPhone Number   : " << client.phoneNumber;
    cout << "\nAccount Balance: " << client.accountBalance;
    cout << "\n***************************************\n\n";
}

void PrintClientsInfoHeader()
{
    cout << "\n__________________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(15) << "Pin Code";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(20) << "Phone Number";
    cout << "| " << left << setw(15) << "Account Balance";
    cout << "\n__________________________________________________________________________________________________________\n";
}

void ShowClientsListScreen()
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    cout << "\n\n\t\t\t\t\tList Of (" << vClients.size() << ") Clients\n";
    PrintClientsInfoHeader();

    for (stClient& client : vClients)
    {
        cout << "| " << left << setw(20) << client.accountNumber;
        cout << "| " << left << setw(15) << client.pinCode;
        cout << "| " << left << setw(25) << client.fullName;
        cout << "| " << left << setw(20) << client.phoneNumber;
        cout << "| " << left << setw(15) << client.accountBalance << endl;
    }
    cout << "__________________________________________________________________________________________________________\n\n";
}

void ShowClientDetailsByAccountNumber(string accountNumber)
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    stClient client;
    if (IsClientExistInVector(accountNumber, vClients, client))
        PrintClientCardDetails(client);
    else
        cout << "\nClient with account number " << accountNumber << " not found!\n\n";
}

void DeleteClientByAccountNumber(string accountNumber)
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    stClient client;
    char userChoice = 'y';
    if (IsClientExistInVector(accountNumber, vClients, client))
    {
        PrintClientCardDetails(client);
        cout << "Are you sure you want to delete this client? [y/n]? ";
        cin >> userChoice;
        if (tolower(userChoice) == 'y')
        {
            MarkClientForDeletion(accountNumber, vClients);
            SaveAllClientsToFile(vClients, clientsFileName, delim);
            cout << "\nClient Deleted successfully\n\n";
        }
        else
            cout << "\nDeletion Cancelled\n\n";
    }
    else
        cout << "\nClient with account number " << accountNumber << " not found!\n\n";
}

void UpdateClientByAccountNumber(string accountNumber)
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    stClient client;
    char userChoice = 'y';
    if (IsClientExistInVector(accountNumber, vClients, client))
    {
        PrintClientCardDetails(client);
        cout << "Are you sure you want to update this client? [y/n]? ";
        cin >> userChoice;
        if (tolower(userChoice) == 'y')
        {
            for (stClient& c : vClients)
            {
                if (c.accountNumber == accountNumber)
                {
                    c = ReadUpdatedClientInfo(accountNumber);
                    break;
                }
            }
            SaveAllClientsToFile(vClients, clientsFileName, delim);
            cout << "\nClient Updated successfully\n\n";
        }
        else
            cout << "\nUpdating Cancelled\n\n";
    }
    else
        cout << "\nClient with account number " << accountNumber << " not found!\n\n";
}

void AddNewClient()
{
    stClient client = ReadClientInfo();
    AddClientToFile(client, clientsFileName, delim);
}

void AddClients()
{
    char addMore = 'y';
    while (tolower(addMore) == 'y')
    {
        AddNewClient();
        cout << "\n\nClient added successfully\n";
        cout << "Do you want to add more client [Y/N]? ";
        cin >> addMore;
    }
}

void AddNewClientsScreen()
{
    cout << "\n===========================================\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n===========================================\n";
    AddClients();
}

void DeleteClientScreen()
{
    cout << "\n===========================================\n";
    cout << "\tDelete Client Screen";
    cout << "\n===========================================\n";
    DeleteClientByAccountNumber(ReadAccountNumber());
}

void UpdateClientScreen()
{
    cout << "\n===========================================\n";
    cout << "\tUpdate Client Screen";
    cout << "\n===========================================\n";
    UpdateClientByAccountNumber(ReadAccountNumber());
}

void FindClientScreen()
{
    cout << "\n===========================================\n";
    cout << "\tFind Client Screen";
    cout << "\n===========================================\n";
    ShowClientDetailsByAccountNumber(ReadAccountNumber());
}

void ProgramEndsScreen()
{
    cout << "\n==========================================\n";
    cout << "\tProgram Ends Screen";
    cout << "\n==========================================\n";
}

void GoBackToMainMenu()
{
    cout << "Press any key to go back to main menu . . . ";
    system("pause>0");
    system("cls");
    MainMenuScreen();
}

void GoBackToTransactionsMenu()
{
    cout << "Press any key to go back to transactions menu . . . ";
    system("pause>0");
    system("cls");
    TransactionsMenuScreen();
}

void TotalBalancesListScreen()
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    cout << "\n\n\t\t\t\t\tList Of (" << vClients.size() << ") Balances Client\n";
    cout << "\n__________________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(15) << "Account Balance";
    cout << "\n__________________________________________________________________________________________________________\n";
    double totalBalances = 0.0;
    for (stClient& client : vClients)
    {
        cout << "| " << left << setw(20) << client.accountNumber;
        cout << "| " << left << setw(25) << client.fullName;
        cout << "| " << left << setw(15) << client.accountBalance << endl;
        totalBalances += client.accountBalance;
    }
    cout << "__________________________________________________________________________________________________________";
    cout << "\n\t\t\t\tTotal Balances= " << totalBalances << "\n\n";

}

void DepositAmount(double amount, string accountNumber, vector<stClient>& vClients)
{
    for (stClient& client : vClients)
    {
        if (client.accountNumber == accountNumber)
        {
            client.accountBalance += amount;
            break;
        }
    }
}

void WithdrawByAccountNumber(string accountNumber)
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    stClient client;
    char userChoice = 'y';

    while (!IsClientExistInVector(accountNumber, vClients, client))
    {
        cout << "\nClient with account number " << accountNumber << " not found!\n\n";
        accountNumber = ReadAccountNumber();
    }

    PrintClientCardDetails(client);
    double withdrawAmount = ReadDoubleNumber("Enter Withdraw Amount: ");

    while (withdrawAmount > client.accountBalance)
    {
        cout << "The Amount Exceeds The Balance, You Can Withdraw Up To " << client.accountBalance << endl;
         withdrawAmount = ReadDoubleNumber("Enter Withdraw Amount: ");
    }

    cout << "\nAre you sure you want to withdraw this amount? [y/n]? ";
    cin >> userChoice;

    if (tolower(userChoice) == 'y')
    {
        DepositAmount(-withdrawAmount, accountNumber, vClients);
        SaveAllClientsToFile(vClients, clientsFileName, delim);
        cout << "\nWithdraw Done successfully!\n\n";
    }
    else
        cout << "\nWithdraw Cancelled!\n\n";
}

void DepositByAccountNumber(string accountNumber)
{
    vector<stClient> vClients = LoadAllClientsFromFile(clientsFileName, delim);
    stClient client;
    char userChoice = 'y';

    while (!IsClientExistInVector(accountNumber, vClients, client))
    {
        cout << "\nClient with account number " << accountNumber << " not found!\n\n";
        accountNumber = ReadAccountNumber();
    }

    PrintClientCardDetails(client);
    double depositAmount = ReadDoubleNumber("Enter Deposit Amount: ");
    cout << "Are you sure you want to deposit this amount? [y/n]? ";
    cin >> userChoice;
    if (tolower(userChoice) == 'y')
    {
        DepositAmount(depositAmount, accountNumber, vClients);
        SaveAllClientsToFile(vClients, clientsFileName, delim);
        cout << "\nDeposit Done successfully!\n\n";
    }
    else
        cout << "\nDeposit Cancelled!\n\n";
}

void DepositScreen()
{
    cout << "\n===========================================\n";
    cout << "\t\tDeposit Screen";
    cout << "\n===========================================\n";
    DepositByAccountNumber(ReadAccountNumber());
}

void WithdrawScreen()
{
    cout << "\n===========================================\n";
    cout << "\t\tWithdraw Screen";
    cout << "\n===========================================\n";
    WithdrawByAccountNumber(ReadAccountNumber());
}

void ExecuteTransactionsMenu(enTransactionsMenu userChoice)
{
    switch (userChoice)
    {
    case enTransactionsMenu::Deposit:
        system("cls");
        DepositScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenu::Withdraw:
        system("cls");
        WithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenu::TotalBalances:
        system("cls");
        TotalBalancesListScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenu::MainMenu:
        system("cls");
        MainMenuScreen();
        break;
    default:
        system("cls");
        TransactionsMenuScreen();
    }
}

void ExecuteMainMenu(enMainMenu userChoice)
{
    switch (userChoice)
    {
    case enMainMenu::ShowClientsList:
        system("cls");
        ShowClientsListScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::AddClient:
        system("cls");
        AddNewClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::DeleteClient:
        system("cls");
        DeleteClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::UpdateClientInfo:
        system("cls");
        UpdateClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::FindClient:
        system("cls");
        FindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenu::Transactions:
        system("cls");
        TransactionsMenuScreen();
        break;
    case enMainMenu::Exit:
        system("cls");
        ProgramEndsScreen();
        break;
    }
}

void TransactionsMenuScreen()
{
    cout << "\n===========================================\n";
    cout << "\tTransactions Menu Screen";
    cout << "\n===========================================\n";
    cout << "\t[1] Deposit\n";
    cout << "\t[2] Withdraw\n";
    cout << "\t[3] Total Balances\n";
    cout << "\t[4] Main Menu";
    cout << "\n===========================================\n";
    ExecuteTransactionsMenu(ReadTransactionsMenuUserChoice());
}

void MainMenuScreen()
{
    cout << "\n===========================================\n";
    cout << "\t\tMain Menu Screen";
    cout << "\n===========================================\n";
    cout << "\t[1] Show Clients List\n";
    cout << "\t[2] Add New Clients\n";
    cout << "\t[3] Delete Client\n";
    cout << "\t[4] Update Client Info\n";
    cout << "\t[5] Find Client\n";
    cout << "\t[6] Transactions\n";
    cout << "\t[7] Exit";
    cout << "\n===========================================\n";
    ExecuteMainMenu(ReadMainMenuUserChoice());
}

int main()
{
    MainMenuScreen();

    return 0;
}