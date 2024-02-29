/* Program name: Final Project
 *  Author: James Ramsey
 *  Date last updated: 3/1/2024
 * Purpose: Database for the smoothie shop Brad's Smoothie Palace
 */

#include "sqlite3.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <limits.h>

void printMainMenu();
int mainMenu();
void addMenu(sqlite3 *db);
void addDistibutor(sqlite3 *db);
void clearInput();
void addEmployee(sqlite3 *db);
void updateMenu(sqlite3 *db);
void updateEmployee(sqlite3 *db);
void updateRawMaterial(sqlite3 *db);
void printPages(sqlite3_stmt *res, int rowsPerPage, int startNum);
void insertPay(sqlite3 *db, int pay, int id);
void insertAmount(sqlite3 *db, int amount, int id);
void deleted(sqlite3 *db, int choice);
void deleteDistributor(sqlite3 *db);
void displayInv(sqlite3 *db);
void viewFruit(sqlite3 *db, int choice);
void displaySmoothies(sqlite3 *db);
void viewSmoothie(sqlite3 *db, int choice);
void printSmoothiePages(sqlite3_stmt *res, int rowsPerPage, int startNum);
std::string getFruit1(sqlite3 *db,int choice);
std::string getFruit2(sqlite3 *db,int choice);
std::string getLiquid(sqlite3 *db,int choice);


int main()
{
	int choice;

	sqlite3 *db;

	int rc;

	rc = sqlite3_open_v2("Smoothie_Palace.db", &db, SQLITE_OPEN_READWRITE, NULL);

	std::cout << "Welcome to Brad's Smoothie Shop" << std::endl;
	choice = mainMenu();
	while (true)
	{
		switch (choice)
		{
		case 1:
			addMenu(db);
			break;
		case 2:
			updateMenu(db);
			break;
		case 3:
			deleteDistributor(db);
			break;
		case 4:
			// Add Transaction;
			break;
		case 5:
			displayInv(db);
			break;
		case 6:
			displaySmoothies(db);
			break;
		case -1:
			return 0;
		default:
			std::cout << "That is not a valid choice." << std::endl;
		}
		std::cout << "\n";
		choice = mainMenu();
	}
}

void printMainMenu()
{
	std::cout << "Please choose an option (enter -1 to quit):  " << std::endl;
	std::cout << "1. Add Distributor or Employee" << std::endl;
	std::cout << "2. Update Employee or Raw Materials" << std::endl;
	std::cout << "3. Delete Distributor" << std::endl;
	std::cout << "4. Add Transaction" << std::endl;
	std::cout << "5. View inventory and suppliers" << std::endl;
	std::cout << "6. View fruit in a smoothie" << std::endl;
	std::cout << "Enter Choice: ";
}

int mainMenu()
{
	int choice = 0;

	printMainMenu();
	std::cin >> choice;
	while ((!std::cin || choice < 1 || choice > 6) && choice != -1)
	{
		if (!std::cin)
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
		std::cout << "That is not a valid choice." << std::endl
				  << std::endl;
		printMainMenu();
		std::cin >> choice;
	}
	return choice;
}

void clearInput()
{
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

void printPages(sqlite3_stmt *res, int rowsPerPage, int startNum)
{
	int stop, i = 1;
	do
	{
		stop = sqlite3_step(res);
		if (stop != SQLITE_ROW)
			break;
		if (sqlite3_column_type(res, 0) != SQLITE_NULL)
			std::cout << sqlite3_column_text(res, 0) << " - ";
		if (sqlite3_column_type(res, 1) != SQLITE_NULL)
			std::cout << sqlite3_column_text(res, 1) << " ";
		if (sqlite3_column_type(res, 2) != SQLITE_NULL)
			std::cout << sqlite3_column_text(res, 2) << " ";
		if (sqlite3_column_type(res, 3) != SQLITE_NULL)
			std::cout << sqlite3_column_text(res, 2) << " ";
		std::cout << std::endl;
		i++;

	} while (i <= rowsPerPage);
}

#pragma region Add option

void addMenu(sqlite3 *db)
{
	int choice;
	std::cout << "1. Add a distributor" << std::endl;
	std::cout << "2. Add an employee" << std::endl;
	std::cin >> choice;
	while ((choice > 2 && choice < 1) || !choice)
	{
		clearInput();
		std::cout << "Invalid input" << std::endl;
		std::cin >> choice;
	}
	switch (choice)
	{
	case 1:
		addDistibutor(db);
		break;
	case 2:
		addEmployee(db);
		break;
	default:
		break;
	}
}

void addDistibutor(sqlite3 *db)
{
	int choice;
	std::string name, type;
	sqlite3_stmt *res;
	std::string query = "INSERT INTO Distributor(Distributor_name, Distributor_type) ";
	query += "Values(@name,@type)";

	std::cout << "What is the name of the Distributor?" << std::endl;
	std::cin >> name;

	std::cout << "What type of distributor is it?" << std::endl;
	std::cout << "1. Fruit vendor" << std::endl;
	std::cout << "2. Liquid vendor" << std::endl;
	std::cin >> choice;

	while ((choice > 2 || choice < 1) || !choice)
	{
		clearInput();
		std::cout << "Invalid input" << std::endl;
		std::cin >> choice;
	}
	switch (choice)
	{
	case 1:
		type = "Fruits";
		break;
	case 2:
		type = "Liquids";
		break;
	default:
		break;
	}
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &res, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert invoice." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}
	rc = sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@name"), name.c_str(), -1, SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert name." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}
	rc = sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@type"), type.c_str(), -1, SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert type." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}
	sqlite3_step(res);
	sqlite3_finalize(res);
	std::cout << "Distributor added" << std::endl;
}

void addEmployee(sqlite3 *db)
{
	int pay;
	std::string Fname, Lname;
	sqlite3_stmt *res;
	std::string query = "INSERT INTO Employee(Employee_Fname,Employee_Lname,Employee_pay) ";
	query += "Values(@Fname,@Lname,@pay)";

	std::cout << "What is the Employee's first name?" << std::endl;
	std::cin >> Fname;

	std::cout << "What is the Employee's last name?" << std::endl;
	std::cin >> Lname;

	std::cout << "How much will the employee earn (Must be between 15 and 60)" << std::endl;
	std::cin >> pay;

	while ((pay > 60 || pay < 15) || !pay)
	{
		clearInput();
		std::cout << "Invalid input" << std::endl;
		std::cin >> pay;
	}

	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &res, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert invoice." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	rc = sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@Fname"), Fname.c_str(), -1, SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert First name." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	rc = sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@Lname"), Lname.c_str(), -1, SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert Last name." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	rc = sqlite3_bind_int(res, sqlite3_bind_parameter_index(res, "@pay"), pay);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert invoice." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	sqlite3_step(res);
	sqlite3_finalize(res);
	std::cout << "Employee added" << std::endl;
}

#pragma endregion

#pragma region Update option

void updateMenu(sqlite3 *db)
{

	int choice;
	std::cout << "1. Update an Employee pay" << std::endl;
	std::cout << "2. Upadate a Raw Material amount" << std::endl;
	std::cin >> choice;
	while ((choice > 2 && choice < 1) || !choice)
	{
		clearInput();
		std::cout << "Invalid input" << std::endl;
		std::cin >> choice;
	}
	switch (choice)
	{
	case 1:
		updateEmployee(db);
		break;
	case 2:
		updateRawMaterial(db);
		break;
	default:
		break;
	}
}

void updateEmployee(sqlite3 *db)
{
	std::string query = "SELECT Employee_id, Employee_Fname, Employee_Lname, Employee_pay FROM Employee ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	int pay;
	int i = 0, choice = 0, rowsPerPage, totalRows;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes);

		sqlite3_stmt *pRes2;
		std::cout << std::left;
		int res;
		do
		{
			res = sqlite3_step(pRes);
			i++;

		} while (res == SQLITE_ROW);
		totalRows = i - 1;
		sqlite3_reset(pRes);
		std::cout << "There are " << i - 1 << " rows in the result.  How many do you want to see per page?" << std::endl;
		std::cin >> rowsPerPage;
		while (!std::cin || rowsPerPage < 0)
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
			}
			std::cout << "That is not a valid choice! Try again!" << std::endl;
			std::cout << "There are " << i << " rows in the result.  How many do you want to see per page?" << std::endl;
		}
		if (rowsPerPage > i)
			rowsPerPage = i;
		i = 0;

		while (choice == 0 || choice == -1)
		{
			if (i == 0)
				std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page):" << std::endl;
			else if (i + rowsPerPage < totalRows)
				std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page or -1 to go to the previous page):" << std::endl;
			else
				std::cout << "Please choose the customer you want to see rentals for (enter -1 to go to the previous page):" << std::endl;
			printPages(pRes, rowsPerPage, i);
			std::cin >> choice;

			while (!(std::cin) || choice < -1 || choice > totalRows)
			{
				if (!std::cin)
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
				std::cout << "That is not a valid choice! Try again!" << std::endl;
				std::cin >> choice;
			}
			if (choice == 0)
			{
				i = i + rowsPerPage;

				if (i >= totalRows)
				{
					i = totalRows - rowsPerPage;
					sqlite3_reset(pRes);
					for (int j = 0; j < i; j++)
					{
						sqlite3_step(pRes);
					}
				}
			}
			else if (choice == -1)
			{
				i = i - rowsPerPage;
				if (i < 0)
					i = 0;
				sqlite3_reset(pRes);
				for (int j = 0; j < i; j++)
					sqlite3_step(pRes);
			}
		}
		sqlite3_reset(pRes);
	}

	std::cout << "What is the employee's new pay? (Must be between 15 and 60)" << std::endl;
	std::cin >> pay;
	while ((pay > 60 || pay < 15) || !pay)
	{
		clearInput();
		std::cout << "Invalid input" << std::endl;
		std::cin >> pay;
	}

	insertPay(db, pay, choice);
	std::cout << "Pay has been updated" << std::endl;
}

void insertPay(sqlite3 *db, int pay, int choice)
{
	sqlite3_stmt *res;
	std::string id;
	sqlite3_stmt *res2;
	std::string query2 = "SELECT Employee_id FROM Employee ";

	int rc = sqlite3_prepare_v2(db, query2.c_str(), -1, &res2, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res2);
		std::cout << "unable to select." << sqlite3_errmsg(db) << std::endl;
		std::cout << query2 << std::endl;
		return;
	}

	for (int i = 0; i < choice; i++)
	{
		sqlite3_step(res2);
	}
	id = reinterpret_cast<const char *>(sqlite3_column_text(res2, 0));
	sqlite3_finalize(res2);

	std::string query = "UPDATE Employee SET Employee_pay = " + std::to_string(pay) + " Where Employee_id = " + id + ";";
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &res, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert invoice." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	sqlite3_step(res);
	sqlite3_finalize(res);
}

void updateRawMaterial(sqlite3 *db)
{
	std::string query = "SELECT Raw_Material_id, Material_name, Material_quantity FROM Raw_Materials ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	int amount;
	int i = 0, choice = 0, totalRows;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes);
		std::cout << std::left;
		int res;
		do
		{
			res = sqlite3_step(pRes);
			i++;

		} while (res == SQLITE_ROW);
		totalRows = i - 1;
		sqlite3_reset(pRes);
		std::cout << "There are " << i - 1 << " rows in the result.  Select one to update" << std::endl;

		i = 0;

		while (choice < 1)
		{

			printPages(pRes, totalRows, i);
			std::cin >> choice;

			while (!(std::cin) || choice < 1 || choice > totalRows)
			{
				if (!std::cin)
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
				std::cout << "That is not a valid choice! Try again!" << std::endl;
				std::cin >> choice;
			}
		}
		sqlite3_reset(pRes);
	}

	std::cout << "What is the material's new amount? (Must be between 100 and 1000)" << std::endl;
	std::cin >> amount;
	while ((amount > 1000 || amount < 100) || !amount)
	{
		clearInput();
		std::cout << "Invalid input" << std::endl;
		std::cin >> amount;
	}

	insertAmount(db, amount, choice);
	std::cout << "Amount has been updated" << std::endl;
}

void insertAmount(sqlite3 *db, int amount, int choice)
{
	sqlite3_stmt *res;
	std::string id;
	sqlite3_stmt *res2;
	std::string query2 = "SELECT Distributor_id FROM distributor ";

	int rc = sqlite3_prepare_v2(db, query2.c_str(), -1, &res2, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res2);
		std::cout << "unable to select." << sqlite3_errmsg(db) << std::endl;
		std::cout << query2 << std::endl;
		return;
	}

	for (int i = 0; i < choice; i++)
	{
		sqlite3_step(res2);
	}
	id = reinterpret_cast<const char *>(sqlite3_column_text(res2, 0));
	sqlite3_finalize(res2);

	std::string query = "UPDATE Raw_Materials SET Material_quantity = " + std::to_string(amount) + " WHERE Raw_Material_id = " + id + ";";
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &res, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to insert invoice." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	sqlite3_step(res);
	sqlite3_finalize(res);
}

#pragma endregion

#pragma region Delete option

void deleteDistributor(sqlite3 *db)
{
	std::string query = "SELECT Distributor_id, Distributor_name, Distributor_type FROM Distributor ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	int pay;
	int i = 0, choice = 0, rowsPerPage, totalRows;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes);
		std::cout << std::left;
		int res;
		do
		{
			res = sqlite3_step(pRes);
			i++;

		} while (res == SQLITE_ROW);
		totalRows = i - 1;
		sqlite3_reset(pRes);
		std::cout << "There are " << i - 1 << " rows in the result.  How many do you want to see per page?" << std::endl;
		std::cin >> rowsPerPage;
		while (!std::cin || rowsPerPage < 0)
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
			}
			std::cout << "That is not a valid choice! Try again!" << std::endl;
			std::cout << "There are " << i << " rows in the result.  How many do you want to see per page?" << std::endl;
		}
		if (rowsPerPage > i)
			rowsPerPage = i;
		i = 0;

		while (choice == 0 || choice == -1)
		{
			if (i == 0)
				std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page):" << std::endl;
			else if (i + rowsPerPage < totalRows)
				std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page or -1 to go to the previous page):" << std::endl;
			else
				std::cout << "Please choose the customer you want to see rentals for (enter -1 to go to the previous page):" << std::endl;
			printPages(pRes, rowsPerPage, i);
			std::cin >> choice;

			while (!(std::cin) || choice < -1 || choice > totalRows)
			{
				if (!std::cin)
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
				std::cout << "That is not a valid choice! Try again!" << std::endl;
				std::cin >> choice;
			}
			if (choice == 0)
			{
				i = i + rowsPerPage;

				if (i >= totalRows)
				{
					i = totalRows - rowsPerPage;
					sqlite3_reset(pRes);
					for (int j = 0; j < i; j++)
					{
						sqlite3_step(pRes);
					}
				}
			}
			else if (choice == -1)
			{
				i = i - rowsPerPage;
				if (i < 0)
					i = 0;
				sqlite3_reset(pRes);
				for (int j = 0; j < i; j++)
					sqlite3_step(pRes);
			}
		}
		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);
	}

	deleted(db, choice);
}

void deleted(sqlite3 *db, int choice)
{
	sqlite3_stmt *res;
	std::string id;
	sqlite3_stmt *res2;
	std::string query2 = "SELECT Distributor_id FROM distributor ";

	int rc = sqlite3_prepare_v2(db, query2.c_str(), -1, &res2, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res2);
		std::cout << "unable to select." << sqlite3_errmsg(db) << std::endl;
		std::cout << query2 << std::endl;
		return;
	}

	for (int i = 0; i < choice; i++)
	{
		sqlite3_step(res2);
	}
	id = reinterpret_cast<const char *>(sqlite3_column_text(res2, 0));
	sqlite3_finalize(res2);

	std::string query = "DELETE FROM Distributor WHERE distributor_id=" + id + ";";
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &res, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_finalize(res);
		std::cout << "unable to delete." << sqlite3_errmsg(db) << std::endl;
		std::cout << query << std::endl;
		return;
	}

	sqlite3_step(res);
	sqlite3_finalize(res);
	std::cout << "Distributor has been deleted" << std::endl;
}

#pragma endregion

// transaction functions here

#pragma region Inventory and suppliers

void displayInv(sqlite3 *db)
{
	std::string query = "SELECT Raw_Material_id, Material_name FROM Raw_Materials ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	int amount;
	int i = 0, choice = 0, totalRows;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes);
		std::cout << std::left;
		int res;
		do
		{
			res = sqlite3_step(pRes);
			i++;

		} while (res == SQLITE_ROW);
		totalRows = i - 1;
		sqlite3_reset(pRes);
		std::cout << "There are " << i - 1 << " rows in the result.  Select one to view" << std::endl;

		i = 0;

		while (choice < 1)
		{

			printPages(pRes, totalRows, i);
			std::cin >> choice;

			while (!(std::cin) || choice < 1 || choice > totalRows)
			{
				if (!std::cin)
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
				std::cout << "That is not a valid choice! Try again!" << std::endl;
				std::cin >> choice;
			}
		}
		sqlite3_reset(pRes);
	}
	viewFruit(db, choice);
	sqlite3_finalize(pRes);
}

void viewFruit(sqlite3 *db, int choice)
{
	std::string query = "SELECT Raw_Materials.Raw_Material_id, Raw_Materials.Material_name, Raw_Materials.Material_quantity, Distributor.Distributor_name FROM Raw_Materials ";
	query += "JOIN Distributor ON Raw_Materials.distributor_id = distributor.distributor_id";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	std::string id, rmName, quantity, dName;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}

	for (int i = 0; i < choice; i++)
		sqlite3_step(pRes);

	id = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 0));
	rmName = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 1));
	quantity = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 2));
	dName = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 3));

	std::cout << " " << std::endl;
	std::cout << "Raw material id: " << id << std::endl;
	std::cout << "Raw material name: " << rmName << std::endl;
	std::cout << "Raw material quantity: " << quantity << std::endl;
	std::cout << "Distributor name: " << dName << std::endl;
	std::cout << " " << std::endl;
}

#pragma endregion

#pragma region Fruit in smoothie option

void displaySmoothies(sqlite3 *db)
{
	std::string query = "SELECT Smoothie_id FROM Smoothies ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	int amount;
	int i = 0, choice = 0, rowsPerPage, totalRows;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes);

		sqlite3_stmt *pRes2;
		std::cout << std::left;
		int res;
		do
		{
			res = sqlite3_step(pRes);
			i++;

		} while (res == SQLITE_ROW);
		totalRows = i - 1;
		sqlite3_reset(pRes);
		std::cout << "There are " << i - 1 << " rows in the result.  How many do you want to see per page?" << std::endl;
		std::cin >> rowsPerPage;
		while (!std::cin || rowsPerPage < 0)
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
			}
			std::cout << "That is not a valid choice! Try again!" << std::endl;
			std::cout << "There are " << i << " rows in the result.  How many do you want to see per page?" << std::endl;
		}
		if (rowsPerPage > i)
			rowsPerPage = i;
		i = 0;

		while (choice == 0 || choice == -1)
		{
			if (i == 0)
				std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page):" << std::endl;
			else if (i + rowsPerPage < totalRows)
				std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page or -1 to go to the previous page):" << std::endl;
			else
				std::cout << "Please choose the customer you want to see rentals for (enter -1 to go to the previous page):" << std::endl;
			printSmoothiePages(pRes, rowsPerPage, i);
			std::cin >> choice;

			while (!(std::cin) || choice < -1 || choice > totalRows)
			{
				if (!std::cin)
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
				std::cout << "That is not a valid choice! Try again!" << std::endl;
				std::cin >> choice;
			}
			if (choice == 0)
			{
				i = i + rowsPerPage;

				if (i >= totalRows)
				{
					i = totalRows - rowsPerPage;
					sqlite3_reset(pRes);
					for (int j = 0; j < i; j++)
					{
						sqlite3_step(pRes);
					}
				}
			}
			else if (choice == -1)
			{
				i = i - rowsPerPage;
				if (i < 0)
					i = 0;
				sqlite3_reset(pRes);
				for (int j = 0; j < i; j++)
					sqlite3_step(pRes);
			}
		}
		sqlite3_reset(pRes);
		viewSmoothie(db, choice);
		sqlite3_finalize(pRes);
	}
}

void printSmoothiePages(sqlite3_stmt *res, int rowsPerPage, int startNum)
{
	int stop, i = 1;
	do
	{
		stop = sqlite3_step(res);
		if (stop != SQLITE_ROW)
			break;
		if (sqlite3_column_type(res, 0) != SQLITE_NULL)
			std::cout << sqlite3_column_text(res, 0) << ": Smoothie " << i << std::endl;
		i++;

	} while (i <= rowsPerPage);
}

std::string getFruit1(sqlite3 *db,int choice)
{
	std::string Fruit;
	std::string query = "SELECT Smoothies.Smoothie_id, Raw_Materials.Material_name FROM Smoothies ";
	query += "JOIN Raw_Materials ON Smoothies.Fruit_1 = Raw_Materials.Raw_Material_id WHERE Smoothies.Fruit_1 = Raw_Materials.Raw_Material_id  ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return Fruit;
	}

	for (int i = 0; i < choice; i++){
		sqlite3_step(pRes);
	}
	Fruit = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 1));
	return Fruit;
}

std::string getFruit2(sqlite3 *db,int choice)
{
	std::string Fruit;
	std::string query = "SELECT Smoothies.Smoothie_id, Raw_Materials.Material_name FROM Smoothies ";
	query += "JOIN Raw_Materials ON Smoothies.Fruit_2 = Raw_Materials.Raw_Material_id WHERE Smoothies.Fruit_2 = Raw_Materials.Raw_Material_id  ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return Fruit;
	}

	for (int i = 0; i < choice; i++){
		sqlite3_step(pRes);
	}
	Fruit = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 1));
	return Fruit;
}

std::string getLiquid(sqlite3 *db,int choice)
{
	std::string Liquid;
	std::string query = "SELECT Smoothies.Smoothie_id, Raw_Materials.Material_name FROM Smoothies ";
	query += "JOIN Raw_Materials ON Smoothies.Liquid = Raw_Materials.Raw_Material_id WHERE Smoothies.Liquid = Raw_Materials.Raw_Material_id  ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return Liquid;
	}

	for (int i = 0; i < choice; i++){
		sqlite3_step(pRes);
	}
	Liquid = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 1));
	return Liquid;
}

void viewSmoothie(sqlite3 *db, int choice)
{
	std::string query = "SELECT Smoothies.Smoothie_id, Smoothies.sell_price FROM Smoothies ";
	sqlite3_stmt *pRes;
	std::string m_strLastError;
	std::string id,F1,F2,Liquid,sellPrice;

	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}

	for (int i = 0; i < choice; i++){
		sqlite3_step(pRes);
	}
		
	id = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 0));
	F1 = getFruit1(db,choice);
	F2 = getFruit2(db,choice);
	Liquid = getLiquid(db,choice);
	sellPrice = reinterpret_cast<const char *>(sqlite3_column_text(pRes, 1));

	std::cout << " " << std::endl;
	std::cout << "Smoothie id: " << id << std::endl;
	std::cout << "Fruit 1: " << F1 << std::endl;
	std::cout << "Fruit 2: " << F2 << std::endl;
	std::cout << "Liquid: " << Liquid << std::endl;
	std::cout << "Sell Price: " << sellPrice << std::endl;
	std::cout << " " << std::endl;
}

#pragma endregion