#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
using namespace std;

//========================== Лексер ===========================
class PARSER
{

private:
	
	vector <string> Tokens, liner, ID_BUFFER, Char_Stack;
	vector <int> Tokens_ID, INT_BUFFER;

	int n;

	bool Err_Flag;

	bool LastClose = false;

	void set_n(int count) { n = count; }
	
	void Set_Liner() { liner.resize(1, "START"); Char_Stack.resize(1, "START"); ID_BUFFER.resize(1, "START"); INT_BUFFER.resize(1, 0); }

	void Program()
	{
		while ((Tokens_ID[n] != 5) || (LastClose == false)) { Statement(); }
		LastClose = false;
		return;
	}
	
	void Statement() //Тело программы
	{
		n++;
		switch (Tokens_ID[n])
		{
			case 1: //;
			{
				//liner.push_back(Tokens[n]);
				return;
			}
			case 3: return;
			case 4: //{
			{
				Program();
				return;
			}
			case 5: //}
			{
				LastClose = true;
				return;
			}
			case 9: //if
			{
				IF(); 
				return;
			}
			case 13: //while
			{
				WHILE();
				return;
			}
			case 15: //ID
			{
				ID();
				return;
			}
			default: 
			{
				//cout << "Syntax error in [Statement]!" << '\n';
				return;
			}
		}
	}

	void ID() //Встречен ID
	{
		liner.push_back(Tokens[n]);
		n++;
		if (Tokens_ID[n] == 8) PR();
		else
		{
			cout << "Syntax error in [ID]!" << '\n';
			n--;
		}
		return;
	}

	void PR() //Встречено присваивание :=
	{
		Char_Stack.push_back(Tokens[n]); //Присваивание извлекается последним по приоритету
		n++;
		if ((Tokens_ID[n] == 14) || (Tokens_ID[n] == 15)|| (Tokens_ID[n] == 2))
		{ 
			OP(); 
		}
		else
		{
			cout << "Syntax error in [:=]!" << '\n';
			system("pause");
			exit(0);
		}
		return;
	}

	void IF() //Встречен IF
	{
		int F0, Addr_F0;
		//liner.push_back("$");
		n++;
		if (Tokens_ID[n] == 2)
		{
			Logic(); // (a<b)
			liner.push_back("$!");
			liner.push_back("F0");
			Addr_F0 = liner.size() - 1; //addr_w1
			//liner.push_back(";");
		}
		else
		{
			cout << "Syntax error in ['(' Logic]!" << '\n';
			system("pause");
			exit(0);
		}

		Statement(); //IF Body

		liner.push_back("@");
		F0 = liner.size() - 1;

		if (Tokens_ID[n + 1] == 12)
		{
			n++;
			ELSE(); //Пришло еще и ELSE
		}

		liner[Addr_F0] = to_string(F0);
		return;
	}

	void ELSE() //Встречен IF
	{
		Statement(); //Else Body
		return;
	}


	void WHILE()
	{
	int W0, W1, W2, Addr_W1, Addr_W2;
	liner.push_back("@");
	W0=liner.size()-1; //w0
	//liner.push_back("$");
	n++;
	if (Tokens_ID[n] == 2)
	{
		Logic(); // (a<b)
		liner.push_back("$!");
		liner.push_back("W1");
		Addr_W1=liner.size()-1; //addr_w1
		//liner.push_back(";");
		liner.push_back("$");
		liner.push_back("W2");
		Addr_W2 = liner.size()-1; //addr_w2
		//liner.push_back(";");
		liner.push_back("@");
		W1=liner.size()-1; //w1
	}
	else
	{
		cout << "Syntax error in ['(' Logic]!" << '\n';
		system("pause");
		exit(0);
	}

	Statement(); // WHILE Body
	liner.push_back("$");
	liner.push_back(to_string(W0));
	//liner.push_back(";");
	liner.push_back("@");
	W2=liner.size()-1; //w2
	liner[Addr_W1] = to_string(W1);
	liner[Addr_W2] = to_string(W2);
	return;
	}

	void Logic() //Only (ID/INT ><= ID/INT)
	{
			//liner.push_back("!");
			//liner.push_back(Tokens[n]); //(
			n++;
			if ((Tokens_ID[n] == 15) | (Tokens_ID[n] == 14)) liner.push_back(Tokens[n]);
			else 
			{
				cout << "Syntax error in [1st Operand Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			n += 2;
			if ((Tokens_ID[n] == 15) | (Tokens_ID[n] == 14)) liner.push_back(Tokens[n]);
			else
			{
				cout << "Syntax error in [2d Operand Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			n--;
			if (Tokens_ID[n] == 7) liner.push_back(Tokens[n]);
			else
			{
				cout << "Syntax error in [Operator Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			n += 2;
			if (Tokens_ID[n] == 3); //liner.push_back(Tokens[n]); //(
			else
			{
				cout << "Syntax error in [')' Logic]!" << '\n';
				system("pause");
				exit(0);
			}
			return;
	}

	void OP() //Алгоритм Дейкстры
	{
		while (Tokens_ID[n] != 1)
		{
			switch (Tokens_ID[n])
			{
			case 2: // (
			{
				Char_Stack.push_back("STOP");
				n++;
				OP();
				break;
			}
			case 3: // )
			{
				while (Char_Stack.back() != "STOP")
				{
					liner.push_back(Char_Stack.back());
					Char_Stack.pop_back();
				}
				Char_Stack.pop_back(); return;
			}
			case 6: // + -
			{
				if ((Char_Stack.back() == "/") || (Char_Stack.back() == "*"))
				{
					liner.push_back(Char_Stack.back());
					Char_Stack.pop_back();
				}
				Char_Stack.push_back(Tokens[n]); break;
			}
			case 14: // INT
			{
				liner.push_back(Tokens[n]);
				break;
			}
			case 15: // ID
			{
				liner.push_back(Tokens[n]);
				break;
			}
			case 16: // * /
			{
				Char_Stack.push_back(Tokens[n]);
				break;
			}
			default: //Неожиданный символ
			{
				cout << "Syntax error in [Dijkstra's algorithm!]" << '\n';
				system("pause");
				exit(0);
			}

			}
			n++;
		}

		{
		    if (Tokens_ID[n] == 1) // ;
			while (Char_Stack.back() != "START")
			{
				liner.push_back(Char_Stack.back());
				Char_Stack.pop_back();
			}
			else
			{
				cout << "Syntax error in [Dijkstra's algorithm!]" << '\n';
				system("pause");
				exit(0);
			}
		}
		n--;
		return;
	}

	void Init()
	{
	
		if (Tokens_ID[n] == 10) n++;
		else
		{
			cout << "Syntax error in [Init 'int']!" << '\n';
			system("pause");
			exit(0);
		}
		if (Tokens_ID[n] == 4)
		{
			n++;
			while (Tokens_ID[n] != 5)
			{
				if (Tokens_ID[n] == 15) 
				{
					ID_BUFFER.push_back(Tokens[n]);
					n++;
					switch (Tokens_ID[n])
					{
					case 1:
					{
						INT_BUFFER.push_back(0);
						break;
					}
					case 8:
					{
						n++;
						if (Tokens_ID[n] == 14) INT_BUFFER.push_back(stoi(Tokens[n]));
						else
						{
						cout << "Syntax error in [Init after ':=']!" << '\n';
						system("pause");
						exit(0);
						}
						n++;
						if (Tokens_ID[n] == 1){}
						else
						{
							cout << "Syntax error in [Init after in ';' after int]!" << '\n';
							system("pause");
							exit(0);
						}
						break;
					}
					default:
					{
						cout << "Syntax error in [Init after ID]!" << '\n';
						system("pause");
						exit(0);
					}
					}
					n++;
				}
			}
			return;
		}
		else;
		{
			cout << "Syntax error in [Init '{']!" << '\n';
			system("pause");
			exit(0);
		}

	}

public:

	void Set(vector <string> Tokens_Str, vector <int> Tokens_ID_Str)
	{
		Set_Liner();
		Tokens = Tokens_Str;
		Tokens_ID = Tokens_ID_Str;
	}

	vector <string> Init_ID() 
	{
		return ID_BUFFER;
	}

	vector <int> Init_INT()
	{
		return INT_BUFFER;
	}

	int Init_N()
	{
		set_n(1);
		Init();
		return n;
	}

	vector <string> Output()
	{
		set_n(Init_N()+1);
		Program();
		return (liner);
	}

};
