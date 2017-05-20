#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
using namespace std;

//========================== Стек машина ===========================
class STACK
{
private:
	vector <string> liner, RAM_ID;
	vector <int> RAM_INT, Machine_Stack;
	vector <bool> Id_Flag;

	int n = 1;

	void On_Input()
	{

		regex
			INT("[0-9]+"),
			ID("[A-Za-z0-9_]*");

		for (; n < liner.size(); n++)
		{
			if (liner[n] == ":=") On_Assign();
			else if ((liner[n] == "+")||(liner[n] == "-")||(liner[n] == "*")||(liner[n] == "/")) On_Op();
			else if ((liner[n] == "<") || (liner[n] == ">") || (liner[n] == "-")) On_Log();
			else if (liner[n] == "$") Goto();
			else if (liner[n] == "$!") Goto_on_false();
			else if (regex_match(liner[n], INT)) On_Int();
			else if (regex_match(liner[n], ID)) On_Id();
		}

	}

int a, b;

	void On_Assign()
	{
		a = On_Addr();
		RAM_INT[Machine_Stack.back()] = a;
		Machine_Stack.pop_back();
		Id_Flag.pop_back();
		return;
	}

	void On_Log()
	{
		a = On_Addr();
		b = On_Addr();
		Machine_Stack.push_back(Logic_ret(liner[n][0]));
		Id_Flag.push_back(0);
		return;
	}

	bool Logic_ret(char Ch)
	{
		switch (Ch)
		{
		case '<': return !(b < a);
		case '>': return !(b > a);
		case '=': return !(b = a);
		}
	}

	void On_Op()
	{
		a = On_Addr();
		b = On_Addr();
		switch (liner[n][0])
		{
		case '+': Machine_Stack.push_back(b + a); break;
		case '-': Machine_Stack.push_back(b - a); break;
		case '*': Machine_Stack.push_back(b * a); break;
		case '/': Machine_Stack.push_back(b / a); break;
		}
		Id_Flag.push_back(0);
		return;
	}

	int On_Addr() //Возвращает значение из стека или RAM
	{
		int St = 0;

		if (Id_Flag.back() == 0) St = Machine_Stack.back();

		else St = RAM_INT[Machine_Stack.back()];

		Machine_Stack.pop_back();
		Id_Flag.pop_back();

		return St;
	}

	void On_Id() //Запись значения из RAM в стек
	{
		for (int i=1; i < RAM_ID.size(); i++)
		{
			if (RAM_ID[i] == liner[n])
			{
				Machine_Stack.push_back(i);
				Id_Flag.push_back(1);
				return;
			}
		}
		cout << "\nCompilling error ID is not initiate!\n";
		system("pause");
		exit(0);
	}

	void On_Int() //Запись числа в стек
	{
		Machine_Stack.push_back(stoi(liner[n]));
		Id_Flag.push_back(0);
		return;
	}


	void Goto() //Безусловный переход по метке
	{
		n = stoi(liner[n+1]);
		return;
	}

	void Goto_on_false() //Условный переход по лжи
	{
		if (Machine_Stack.back() == 0) Goto();
		else n++;
		Machine_Stack.pop_back();
		Id_Flag.pop_back();
		return;
	}

public:

vector<int> Output_RAM(vector <string> line, vector <string> ID, vector <int> INT)
{
	liner = line; RAM_ID = ID; RAM_INT = INT;
	On_Input();
	return RAM_INT;
}

};