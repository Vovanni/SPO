/*
Лексер для C-подобного языка
CBL(C_Based_Language).

Выполнил студент Кондратьев В.С.
Учебная группа ИВБО-08-14
*/
#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
using namespace std;

//========================== Лексер ===========================
class LEXER {

private:

	struct Lex_out		//Описание структуры для вывода значений из функции лексера
	{					//для вывода значений из функции лексера
		vector <int> ID;
		vector <string> Tokens;
		bool Err;
	};

	struct Lex_out lexer(string s1)
	{
		ifstream in("Example.cbl"); //Открываем файл для считывания информации 

		vector <int> Tokens_ID;    //Создаем векторы хранения токенов
		Tokens_ID.resize(1, 0);
		vector <string> Tokens;    //и их идентификаторов
		Tokens.resize(1, "START");
		bool Err = false;

		regex								//Регулярныe выражения		 
		  //IF("if"),
			INT("int"),
			ELSE("else"),
			//DO("do"),
			WHILE("while"),
			//PR(":="),
			//SYMBOL("[\(]|[\)]|[\{]|[\}]|[;]"),
			OP1("[\+]|[-]"),
			OP2("[/]|[\*]"),
			LOGIC("[=]|[<]|[>]"),
			INTEGER("[0-9]+"),
			DIGIT("[0-9]"),
			LETTER("[A-Za-z]"),
			ID("[A-Za-z0-9_]*");

		s1 += ' ';

		for (int n = 1; n < s1.size(); n++)    //Повторяем действия в цикле пока
		{										//указатель на последний элемент строки меньше ее размера.
			int size = s1.size();

			if (s1[0] == ' ') goto ZAPIS_2;

			else if (s1[0] == ';') { Tokens_ID.push_back(1); goto ZAPIS; } //Проверка символов
			else if (s1[0] == '(') { Tokens_ID.push_back(2); goto ZAPIS; }
			else if (s1[0] == ')') { Tokens_ID.push_back(3); goto ZAPIS; }
			else if (s1[0] == '{') { Tokens_ID.push_back(4); goto ZAPIS; }
			else if (s1[0] == '}') { Tokens_ID.push_back(5); goto ZAPIS; }

			else if (regex_match(s1.substr(0, 1), OP1)) //Проверка операций + -
			{
				Tokens_ID.push_back(6); goto ZAPIS;
			}

			else if (regex_match(s1.substr(0, 1), OP2)) //Проверка операций * /
			{
				Tokens_ID.push_back(16); goto ZAPIS;
			}

			else if (regex_match(s1.substr(0, 1), LOGIC)) //Проверка логических операций
			{
				Tokens_ID.push_back(7); goto ZAPIS;
			}

			else if ((s1[0] == ':')&(2 < size)) //Проверка :=
			{
				if (s1[1] == '=') { n = 2; Tokens_ID.push_back(8); goto ZAPIS; }
				else goto UNCKNOWN_SYMBOL;      //Неизвестная последовательность символов
			}

			else if ((s1[0] == 'i')&(2 < size)) //Проверка IF
			{
				if (s1[1] == 'f') { n = 2; Tokens_ID.push_back(9); goto ZAPIS; }
				else if (3 < size) //Проверка INT
				{
					if (regex_match(s1.substr(0, 3), INT)) { n = 3; Tokens_ID.push_back(10); goto ZAPIS; }
					else goto LETTER_1;             //Возможно это идентификатор
				}

				else goto LETTER_1;             //Возможно это идентификатор
			}

			else if ((s1[0] == 'd')&(2 < size)) //Проверка DO
			{
				if (s1[1] == 'o') { n = 2; Tokens_ID.push_back(11); goto ZAPIS; }
				else goto LETTER_1;             //Возможно это идентификатор
			}

			else if ((s1[0] == 'e')&(4 < size)) //Проверка ELSE
			{
				if (regex_match(s1.substr(0, 4), ELSE)) { n = 4; Tokens_ID.push_back(12); goto ZAPIS; }
				else goto LETTER_1;             //Возможно это идентификатор
			}

			else if ((s1[0] == 'w')&(5 < size)) //Проверка WHILE
			{
				if (regex_match(s1.substr(0, 5), WHILE)) { n = 5; Tokens_ID.push_back(13); goto ZAPIS; }
				else goto LETTER_1;             //Возможно это идентификатор
			}

			else if (regex_match(s1.substr(0, n), DIGIT)) //Проверка на цифру
			{

				for (; regex_match(s1.substr(0, n), INTEGER); n++) //Блок чтения чтения числа из строки
				{
					switch (n)
					{
					case 1:
						Tokens_ID.push_back(14);
						Tokens.push_back(s1.substr(0, n));
						break;

					default:
						Tokens.pop_back();
						Tokens.push_back(s1.substr(0, n));
						break;
					}
				}
				n--; goto ZAPIS_2;
			}

			else
			{

			LETTER_1:

				if (regex_match(s1.substr(0, n), LETTER)) //Проверка на идентификатор
				{
					for (; regex_match(s1.substr(0, n), ID); n++) //Блок чтения чтения идентификатора из строки
					{
						switch (n)
						{
						case 1:
							Tokens_ID.push_back(15);
							Tokens.push_back(s1.substr(0, n));
							break;

						default:
							Tokens.pop_back();
							Tokens.push_back(s1.substr(0, n));
							break;
						}
					}
					n--; goto ZAPIS_2;
				}


				else			//Когда встречается неизвестный символ выводит ошибку
				{

				UNCKNOWN_SYMBOL:

					s1 = s1.substr(1);
					n = 0;
					Err = true;
					goto EXIT;
				}
			}
			goto EXIT;

		ZAPIS:

			Tokens.push_back(s1.substr(0, n));

		ZAPIS_2:

			s1 = s1.substr(n);
			n = 0;

		}         //end for

	EXIT:

		Lex_out Func_Ret;
		Func_Ret.ID = Tokens_ID;
		Func_Ret.Tokens = Tokens;
		Func_Ret.Err = Err;
		return Func_Ret;
	}

public:

	vector <string> Tokens(string s1)   //Возвращает список токенов
	{
		Lex_out Out;
		Out.Tokens = lexer(s1).Tokens;
		return(Out.Tokens);
	}

	vector <int> Tokens_ID(string s1)   //Идентификаторы данных токенов
	{
		Lex_out Out;
		Out.ID = lexer(s1).ID;
		return(Out.ID);
	}

	bool Error(string s1)               //Возвращает истину если есть неизвестный символ
	{
		Lex_out Out;
		Out.Err = lexer(s1).Err;
		return(Out.Err);
	}

};