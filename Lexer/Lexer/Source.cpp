/*
Проверка работы лексер C-подобного языка
CBL(C_Based_Language).

Выполнил студент Кондратьев В.С.
Учебная группа ИВБО-08-14
*/

#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <fstream> // Работа с файлами

#include "Lexer.h" // Класс лексера

using namespace std;

//====================== Основная часть =======================
int main()
{

	ifstream in("Example.cbl"); //Открываем файл для считывания информации 
	string s1,s2;   //Переменная будет считываться в строку
	while (in >> s2) s1+=s2; //Считываем пока можем
	in.close(); // Закрываем файл

	LEXER lex;
	vector <string> tok = lex.Tokens(s1);
	vector <int> tok_id = lex.Tokens_ID(s1);
	bool tok_err = lex.Error(s1);

	for (int n = 1; n < tok_id.size(); n++) //Вывод лексем в консоль
	{
	cout << tok_id[n] << ' ';
	cout << tok[n] << '\n';
	}

	if (tok_err == true)
		cout << "Lexical error!" <<  '\n';

system("pause"); 
}