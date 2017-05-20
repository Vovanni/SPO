/*
������������� C-��������� �����
CBL(C_Based_Language).

�������� ������� ���������� �.�.
������� ������ ����-08-14
*/

#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <fstream> // ������ � �������

#include "Lexer.h" // ����� �������
#include "Parser.h"
#include "Stack_machine.h"

using namespace std;

//====================== �������� ����� =======================
void main()
{

	ifstream in("Example.cbl"); //��������� ���� ��� ���������� ���������� 
	string s1, s2;   //���������� ����� ����������� � ������
	while (in >> s2) s1 += s2; //��������� ���� �����
	in.close(); // ��������� ����

	LEXER lex;
	vector <string> tok = lex.Tokens(s1);
	vector <int> tok_id = lex.Tokens_ID(s1);
	bool tok_err = lex.Error(s1);

	cout << "Tokens:\n" << "ID | Lex\n--------\n";

	for (int n = 1; n < tok_id.size(); n++) //����� ������ � �������
	{
		cout << tok_id[n];
		if (tok_id[n] < 10) cout << "  | ";
		else cout << " | ";
		cout << tok[n] << '\n';
	}

	if (tok_err == true)
		cout << "Lexical error!" << '\n';

	PARSER Pars;
	Pars.Set(tok, tok_id);
	vector <string> Pars_Test = Pars.Output();
	vector <int> RAM_INT = Pars.Init_INT();
	vector <string>  RAM_ID = Pars.Init_ID();


	cout << "\nInit RAM:\n";

	for (int n = 1; n < RAM_INT.size(); n++) //����� �������� � �������
	{
		cout << RAM_ID[n] << " | " << RAM_INT[n] << '\n';
	}

	cout <<"\nPoliz:\n";

	for (int n = 1; n < Pars_Test.size(); n++) //����� ������ � �������
	{
		cout << Pars_Test[n] << ' ';
	}

	STACK Mashine;
	vector <int> Mashine_RAM = Mashine.Output_RAM(Pars_Test,RAM_ID,RAM_INT);

	cout <<"\n\nRAM:\n";

	for (int n = 1; n < Mashine_RAM.size(); n++) //����� �������� � ������� ����� ��������
	{
		cout << RAM_ID[n] << " | " << Mashine_RAM[n] << '\n';
	}

	cout << '\n';
	
	system("pause");
	return;
}