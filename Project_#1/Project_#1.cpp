#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include <iomanip>
using namespace std;

void ShowMainMenu();
void ShowTransactiosMenue();
void ShowManageUserMenu();
void Login();
const string ClientFile = "D:\\MyFile.txt";
const string UserFile = "D:\\UserFile.txt";

struct sClient
{
	string AccountNum, PinCode, Name, Phone;
	double AccountBalance;
	bool MarkOfDelete = false;
};

struct sUser
{
	string UserName;
	string Password;
	short Permission = -1;
	bool MarkOfDelete = false;
};

sUser UserPer;

vector<string> SplitString(string str, string Spertator = "#//#")
{
	short Pos = 0;
	vector<string> vString;
	string sWord = "";
	while ((Pos = str.find(Spertator)) != std::string::npos)
	{
		sWord = str.substr(0, Pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		str.erase(0, Pos + Spertator.length());
	}
	if (str != "")
	{
		vString.push_back(str);
	}

	return vString;
}

sClient ConvertLineToRecord(string str, string Spertator = "#//#")
{
	vector<string> vString;
	sClient Client1;
	vString = SplitString(str, Spertator);

	Client1.AccountNum = vString[0];
	Client1.PinCode = vString[1];
	Client1.Name = vString[2];
	Client1.Phone = vString[3];
	Client1.AccountBalance = stod(vString[4]);

	return Client1;


}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string S1 = "";
	S1 += Client.AccountNum + Seperator;
	S1 += Client.PinCode + Seperator;
	S1 += Client.Name + Seperator;
	S1 += Client.Phone + Seperator;
	S1 += to_string(Client.AccountBalance);

	return S1;
}

bool CLientExistsByAccountNumber(string AccountNumber, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	vector<sClient> vClient;

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNum == AccountNumber)
			{
				return true;
				MyFile.close();
			}
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

sClient ReadNewClient()
{
	sClient Client;

	cout << "Enter Account Number ? ";
	getline(cin >> ws, Client.AccountNum);

	while (CLientExistsByAccountNumber(Client.AccountNum, ClientFile))
	{
		cout << "\nClient With [ " << Client.AccountNum <<
			" ] already Exists , Error Enter Another Account Number ? ";
		getline(cin >> ws, Client.AccountNum);
	}
	cout << "Enter Pin Code ? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;

	return Client;


}

vector<sClient> LoadClientsDataFromFile(string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	vector<sClient> vClient;

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	else
	{
		cout << "Error";
	}
	return vClient;
}

void PrintClientRecordLine(sClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNum;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
	vector<sClient> vClients = LoadClientsDataFromFile(ClientFile);

	cout << "\n\t\t\t\t Client List (" << vClients.size() << ") Client(s).";
	cout << "\n___________________________________________________________________";
	cout << "__________________________________\n";

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n___________________________________________________________________";
	cout << "__________________________________\n";

	if (vClients.size() == 0)
	{
		cout << "\nNo Clients Available In the System .";
	}
	else
	{
		for (sClient C : vClients)
		{
			PrintClientRecordLine(C);
			cout << endl;
		}
	}
	cout << "\n___________________________________________________________________";
	cout << "__________________________________\n";
}

void PrintClientCard(sClient Client)
{
	cout << "\nAccount Number  :" << Client.AccountNum;
	cout << "\nPin Code        :" << Client.PinCode;
	cout << "\nName            :" << Client.Name;
	cout << "\nPhone           :" << Client.Phone;
	cout << "\nAccount Balance :" << Client.AccountBalance << endl;

}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
	for (sClient C : vClients)
	{
		if (C.AccountNum == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

sClient ChanageClientRecord(string AccountNumber)
{
	sClient Client;

	Client.AccountNum = AccountNumber;

	cout << "Enter Pin Code ? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNum == AccountNumber)
		{
			C.MarkOfDelete = true;
			return true;
		}
	}

	return false;
}

void SaveClientDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkOfDelete == false)
			{
				DataLine = ConvertRecordToLine(C);

				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}

}

string ReadAccountNumber()
{
	string AccountNumber;

	cout << "Please Enter Account Number ? ";
	cin >> AccountNumber;

	return AccountNumber;
}

void AddDataLineToFile(string FileName, string sDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << sDataLine << endl;

		MyFile.close();
	}
}

void AddNewClient()
{
	sClient Client;

	Client = ReadNewClient();

	AddDataLineToFile(ClientFile, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'y';

	do
	{
		cout << "\nAdding New Client:\n\n";
		AddNewClient();
		cout << "\nClient Added Sucessfuly ,do you want to add more clients  ? y/n ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

bool DeleteAccountByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Anwser = 'y';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\n\nAre you sure to need delete this client ? y/n ? ";
		cin >> Anwser;

		if (toupper(Anwser) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);

			SaveClientDataToFile(ClientFile, vClients);

			vClients = LoadClientsDataFromFile(ClientFile);

			cout << "\nClient Delete Sucessfuly ." << endl;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		cout << "\nClient With Account Number ( " << AccountNumber << " ) is Not Found !";
		return false;
	}

}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Anwser = 'y';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure update of this client ? y/n ? ";
		cin >> Anwser;

		if (toupper(Anwser) == 'Y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNum == AccountNumber)
				{
					C = ChanageClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientDataToFile(ClientFile, vClients);

			cout << "\nClient Update Successfuly ." << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		cout << "\nClient With Account Number ( " << AccountNumber << " ) is Not Found . " << endl;
		return false;
	}
}

void ShowDeleteClientScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tDelete Client Screen\n";
	cout << "\n_______________________________\n";

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();

	DeleteAccountByAccountNumber(AccountNumber, vClients);
}


void ShowUpdateClientScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tUpdate Client Screen\n";
	cout << "\n_______________________________\n";

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddClientScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tAdd Clients Screen\n";
	cout << "\n_______________________________\n";
	AddNewClients();
}

void ShowFindClientScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tFind Client Screen\n";
	cout << "\n_______________________________\n";

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFile);
	sClient Client;
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient With Account Number ( " << AccountNumber << " ) is Not Found ." << endl;
	}
}

void ShowEndScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tProgram Ends :-)\n";
	cout << "\n_______________________________\n";
}
enum enMainMenuOptions
{
	eListClients = 1, eAddNewClients = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactions = 6,
	eManageUser = 7, eLogout = 8
};

void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to Main Menu ......";
	system("pause>0");
	ShowMainMenu();
}

short ReadMainMenuOption()
{
	cout << "Choose what do you want to do? [1 to 8]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

void AccessDeniedScreen()
{
	cout << "\n_______________________________\n";
	cout << "Access Denied ,\n";
	cout << "You Dont Have Permission To Do This ,\n";
	cout << "Please Contact Your Admin.\n";
	cout << "_______________________________\n";
}

void PerfromMainMenuOption(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eListClients:
		system("cls");
		if ((UserPer.Permission & (1 << 0) != 0)||UserPer.Permission==-1)
		{
			ShowAllClientsScreen();
		}
		else
		{
			AccessDeniedScreen();
		}
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClients:
		system("cls");
		if ((UserPer.Permission & (1 << 1) != 0) || UserPer.Permission == -1)
		{
			ShowAddClientScreen();
		}
		else
		{
			AccessDeniedScreen();
		}
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eDeleteClient:
		system("cls");
		if ((UserPer.Permission & (1 << 2) != 0) || UserPer.Permission == -1)
		{
			ShowDeleteClientScreen();
		}
		else
		{
			AccessDeniedScreen();
		}
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClient:
		system("cls");
		if ((UserPer.Permission & (1 << 3) != 0) || UserPer.Permission == -1)
		{
			ShowUpdateClientScreen();
		}
		else
		{
			AccessDeniedScreen();
		}
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		if ((UserPer.Permission & (1 << 4) != 0) || UserPer.Permission == -1)
		{
			ShowFindClientScreen();
		}
		else
		{
			AccessDeniedScreen();
		}
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eTransactions:
		system("cls");
		if ((UserPer.Permission & (1 << 5) != 0) || UserPer.Permission == -1)
		{
			ShowTransactiosMenue();
		}
		else
		{
			AccessDeniedScreen();
		}
		break;
	case enMainMenuOptions::eManageUser:
		system("cls");
		if ((UserPer.Permission & (1 << 6) != 0) || UserPer.Permission == -1)
		{
			ShowManageUserMenu();
		}
		else
		{
			AccessDeniedScreen();
		}
		break;
	case enMainMenuOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}



bool DepositBalanceToClientByAccountNumber(double Amount, string AccountNumber, vector<sClient>& vClients)
{
	char Anwser = 'y';
	cout << "Are you sure you want perfrom this transactions ? y/n ? ";
	cin >> Anwser;
	if (toupper(Anwser) == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNum == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientDataToFile(ClientFile, vClients);
				cout << "\nDone Sucessfuly . New Balance is = " << C.AccountBalance << endl;
				return true;
			}
		}
	}

	return false;
}


void ShowDepositScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tDeposit Screen \n";
	cout << "\n_______________________________\n";

	string AccountNumber = ReadAccountNumber();
	sClient Client;
	vector<sClient> vClients = LoadClientsDataFromFile(ClientFile);

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [ " << AccountNumber << " ] does not exist ." << endl;
		AccountNumber = ReadAccountNumber();

	}

	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount ? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(Amount, AccountNumber, vClients);


}




void ShowWithdrawScreen()
{
	cout << "\n_______________________________\n";
	cout << "\tDeposit Screen \n";
	cout << "\n_______________________________\n";

	string AccountNumber = ReadAccountNumber();
	sClient Client;
	vector<sClient> vClients = LoadClientsDataFromFile(ClientFile);

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [ " << AccountNumber << " ] does not exist ." << endl;
		AccountNumber = ReadAccountNumber();

	}

	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount ? ";
	cin >> Amount;
	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(Amount * -1, AccountNumber, vClients);

}

void PrintClientRecordBalanceLine(sClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNum;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(30) << Client.AccountBalance << endl;

}

void ShowTotalBalances()
{
	vector<sClient> vClients;
	vClients = LoadClientsDataFromFile(ClientFile);

	double TotalBalance = 0;
	cout << "\n\t\t\tBalances List ( " << vClients.size() << " ) Client(s) .";

	cout << "\n_____________________________________________________________";
	cout << "________________________________________\n";

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(30) << "Balance";

	cout << "\n_____________________________________________________________";
	cout << "________________________________________\n";

	if (vClients.size() == 0)
	{
		cout << "\n\t\tNo Clients Avialable In The Sysytem .";
	}
	else
	{
		for (sClient C : vClients)
		{
			PrintClientRecordBalanceLine(C);
			TotalBalance += C.AccountBalance;
		}
	}
	cout << "\n_____________________________________________________________";
	cout << "________________________________________\n";

	cout << "\n\t\t\t\tTotal Balances = " << TotalBalance << endl;

}


enum enTransactionsMenueOptions
{
	eDeposit = 1, eWithdraw = 2,
	eTotalBalance = 3, eMainMenue = 4
};

short ReadTransactionsMenueOption()
{
	short Choise = 0;

	cout << "\nChoose what do you want to do ? [1 to 4] ?";
	cin >> Choise;

	return Choise;
}

void GoBackToTransactiosMenue()
{
	cout << "Enter any key to go back to Transactions Menue ..............";
	system("pause>0");
	ShowTransactiosMenue();
}

void PeformTransactionsMenueOption(enTransactionsMenueOptions TransationsMenuOption)
{
	system("cls");

	switch (TransationsMenuOption)
	{
	case enTransactionsMenueOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactiosMenue();
		break;
	case enTransactionsMenueOptions::eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactiosMenue();
		break;
	case enTransactionsMenueOptions::eTotalBalance:
		system("cls");
		ShowTotalBalances();
		GoBackToTransactiosMenue();
		break;
	case enTransactionsMenueOptions::eMainMenue:
		system("cls");
		ShowMainMenu();
		break;
	}
}

void ShowTransactiosMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";

	PeformTransactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

void ShowMainMenu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage User.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";
	PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}




sUser ConvertUserLineToRecord(string str, string Spertator = "#//#")
{
	vector<string> vString;
	sUser User1;
	vString = SplitString(str, Spertator);

	User1.UserName = vString[0];
	User1.Password = vString[1];
	User1.Permission = stoi(vString[2]);

	return User1;


}

vector<sUser> LoadUsersDataFromFile(string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	vector<sUser>  vUser;

	if (MyFile.is_open())
	{
		string Line;
		sUser  User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);

			vUser.push_back(User);
		}
		MyFile.close();
	}
	else
	{
		cout << "Error";
	}
	return vUser;
}

void PrintUserRecordLine(sUser User)
{
	cout << "| " << left << setw(15) << User.UserName;
	cout << "| " << left << setw(10) << User.Password;
	cout << "| " << left << setw(40) << User.Permission;
}

void ShowAllUsersScreen()
{
	vector<sUser> vUser = LoadUsersDataFromFile(UserFile);

	cout << "\n\t\t\t\t User List (" << vUser.size() << ") User(s).";
	cout << "\n___________________________________________________________________";
	cout << "__________________________________\n";

	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(40) << "Permission";
	cout << "\n___________________________________________________________________";
	cout << "__________________________________\n";

	for (sUser u : vUser)
	{
		PrintUserRecordLine(u);
		cout << endl;
	}

	cout << "\n___________________________________________________________________";
	cout << "__________________________________\n";
}

string ConvertUserRecordToLine(sUser User, string Seperator = "#//#")
{
	string S1 = "";
	S1 += User.UserName + Seperator;
	S1 += User.Password + Seperator;
	S1 += to_string(User.Permission);

	return S1;
}

bool CLientExistsByUserName(string UserName, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	vector<sUser> vUser;

	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			if (User.UserName == UserName)
			{
				return true;
				MyFile.close();
			}
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return false;
}

short ReadPermission()
{
	char Anwser = 'y';

	cout << "Do you want to give full access ? y/n ? ";
	cin >> Anwser;

	short userPermission = 0;
	if (toupper(Anwser) == 'Y')
	{
		return -1;
	}
	else
	{
		cout << "\nDo you want to give access to ? \n";
		cout << "\nShow Client List ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 0;
		}
		cout << "\nAdd Client List ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 1;
		}
		cout << "\nDelete Client List ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 2;
		}
		cout << "\nUpdate Client List ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 3;
		}

		cout << "\nFind Client List ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 4;
		}

		cout << "\nTransactions ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 5;
		}

		cout << "\nManage Users ? y/n ? ";
		cin >> Anwser;
		if (toupper(Anwser) == 'Y')
		{
			userPermission |= 1 << 6;
		}

		return userPermission;
	}
}

sUser ReadNewUser()
{
	sUser User;

	cout << "Enter User Name ? ";
	getline(cin >> ws, User.UserName);

	while (CLientExistsByUserName(User.UserName, UserFile))
	{
		cout << "\nUser With [ " << User.UserName <<
			" ] already Exists , Error Enter Another User Name ? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password ? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermission();

	return User;


}

void AddNewUser()
{
	sUser User;

	User = ReadNewUser();

	AddDataLineToFile(UserFile, ConvertUserRecordToLine(User));


}

void AddMoreUser()
{
	char AddAnother = 'y';

	do
	{
		cout << "Adding New User : \n";

		AddNewUser();

		cout << "User Added Successfully, do you want to add more Users ? Y/N ? ";

		cin >> AddAnother;

	} while (toupper(AddAnother) == 'Y');
}

string ReadUserName()
{
	string UserName;

	cout << "\nPlease enter UserName ? ";

	getline(cin >> ws, UserName);

	return UserName;
}

void PrintUserCard(sUser User)
{

	cout << "\nThe following are the user details :\n";
	cout << "____________________________________\n";
	cout << "Username       :" << User.UserName << endl;
	cout << "Password       :" << User.Password << endl;
	cout << "Permission     :" << User.Permission << endl;
	cout << "____________________________________\n";

}

bool FindUserByUserName(vector<sUser> vUser, string UserName, sUser& User)
{

	for (sUser u : vUser)
	{
		if (u.UserName == UserName)
		{
			User = u;
			return true;
		}
	}

	return false;
}

bool MarkUserDeleteByUserName(string UserName, vector<sUser>& vUser)
{
	for (sUser& u : vUser)
	{
		if (u.UserName == UserName)
		{
			u.MarkOfDelete = true;
			return true;
		}
	}

	return false;
}

void SaveUserDataToFile(string FileName, vector<sUser> vUser)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string DataLine;
	if (MyFile.is_open())
	{
		for (sUser U : vUser)
		{
			if (U.MarkOfDelete == false)
			{
				DataLine = ConvertUserRecordToLine(U);

				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}

}

bool DeleteUserByUserName(string UserName, vector<sUser>& vUser)
{
	sUser User;
	char Anwser = 'y';

	if (FindUserByUserName(vUser, UserName, User))
	{
		cout << "\n\nAre you sure to need delete this user ? y/n ? ";
		cin >> Anwser;

		if (toupper(Anwser) == 'Y')
		{
			if (UserName != "Admin")
			{
				MarkUserDeleteByUserName(UserName, vUser);

				SaveUserDataToFile(UserFile, vUser);

				LoadUsersDataFromFile(UserFile);

				cout << "\nUser Delete Sucessfuly ." << endl;

				return true;
			}
			else
			{
				cout << "\nYou cannot Delete This User .\n";
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		cout << "\nUser With UserName ( " << UserName << " ) is Not Found !\n";
		return false;
	}

}

sUser ChanageUserRecord(string UserName)
{
	sUser User;

	User.UserName = UserName;

	cout << "Enter Password ? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermission();

	return User;


}

bool UpdateUserByUserName(string UserName, vector<sUser>& vUser)
{
	sUser User;
	char Anwser = 'y';
	if (FindUserByUserName(vUser, UserName, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure update of this user ? y/n ? ";
		cin >> Anwser;

		if (toupper(Anwser) == 'Y')
		{
			for (sUser& U : vUser)
			{
				if (U.UserName == UserName)
				{
					U = ChanageUserRecord(UserName);
					break;
				}
			}

			SaveUserDataToFile(UserFile, vUser);

			cout << "\nUser Update Successfuly ." << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		cout << "\nUser With User Name ( " << UserName << " ) is Not Found . " << endl;
		return false;
	}
}

void FindUserScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tFind User Screen\n";
	cout << "===========================================\n";

	vector<sUser> vUser = LoadUsersDataFromFile(UserFile);
	sUser User;

	string UserName = ReadUserName();

	if (FindUserByUserName(vUser, UserName, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nUser With UserName [ " << UserName << " ] is Not Found In The System ." << endl;
	}

}

void AddNewUserScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tAdd New User Screen\n";
	cout << "===========================================\n";
	AddMoreUser();
}

void DeleteUserScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tDelete User Screen\n";
	cout << "===========================================\n";

	string UserName = ReadUserName();

	vector<sUser> vUser = LoadUsersDataFromFile(UserFile);

	DeleteUserByUserName(UserName, vUser);
}

void UpdateUserScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tUpdate User Screen\n";
	cout << "===========================================\n";

	string UserName = ReadUserName();

	vector<sUser> vUser = LoadUsersDataFromFile(UserFile);

	UpdateUserByUserName(UserName, vUser);
}

enum enManageUserMenu
{
	eListUser = 1, eAddUser = 2,
	eDeleteUser = 3, eUpdateUser = 4,
	eFindUser = 5, eMainMenu = 6
};

short ReadManageUserMenuOption()
{
	short Choise = 0;

	cout << "\nChoose what do you want to do ? [1 to 6] ?";
	cin >> Choise;

	return Choise;
}

void GoBackToManageUserMenu()
{
	cout << "Enter any key to go back to Manage  User Menu ..............";
	system("pause>0");
	ShowManageUserMenu();
}

void PeformManageUserMenuOption(enManageUserMenu ManageUserMenu)
{
	system("cls");

	switch (ManageUserMenu)
	{

	case enManageUserMenu::eAddUser:
		system("cls");
		AddNewUserScreen();
		GoBackToManageUserMenu();
		break;
	case enManageUserMenu::eListUser:
		system("cls");
		ShowAllUsersScreen();
		GoBackToManageUserMenu();
		break;
	case enManageUserMenu::eDeleteUser:
		system("cls");
		DeleteUserScreen();
		GoBackToManageUserMenu();
		break;
	case enManageUserMenu::eUpdateUser:
		system("cls");
		UpdateUserScreen();
		GoBackToManageUserMenu();
		break;
	case enManageUserMenu::eFindUser:
		system("cls");
		FindUserScreen();
		GoBackToManageUserMenu();
		break;
	case enManageUserMenu::eMainMenu:
		system("cls");
		ShowMainMenu();
		break;
	}
}

void ShowManageUserMenu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tManage User Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User .\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "===========================================\n";

	PeformManageUserMenuOption((enManageUserMenu)ReadManageUserMenuOption());
}

bool CheckUserInSystem(sUser &User1)
{
	vector<sUser> vUser = LoadUsersDataFromFile(UserFile);

	for (sUser &u : vUser)
	{
		if (u.UserName == User1.UserName && u.Password == User1.Password)
		{
			User1 = u;
			return true;
		}
	}

	return false;

}

void LoginScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tLogin Screen\n";
	cout << "===========================================\n";
}

void Login()
{
	sUser User;

	LoginScreen();

	cout << "\nEnter Username ? ";
	getline(cin >> ws, User.UserName);

	cout << "\nEnter Password ? ";
	getline(cin >> ws, User.Password);


	while (!CheckUserInSystem(User))
	{
		LoginScreen();
		cout << "\nInvlaid Username/Password!\n";
		cout << "\nEnter Username ? ";
		getline(cin >> ws, User.UserName);

		cout << "\nEnter Password ? ";
		getline(cin >> ws, User.Password);
	}
	UserPer = User;

	ShowMainMenu();

}

int main()
{

	Login();

	
}

