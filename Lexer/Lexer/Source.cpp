/*
�������� ������ ������ C-��������� �����
CBL(C_Based_Language).

�������� ������� ���������� �.�.
������� ������ ����-08-14
*/

#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <fstream> // ������ � �������

#include "Lexer.h" // ����� �������

using namespace std;

//====================== �������� ����� =======================
int main()
{

	ifstream in("Example.cbl"); //��������� ���� ��� ���������� ���������� 
	string s1,s2;   //���������� ����� ����������� � ������
	while (in >> s2) s1+=s2; //��������� ���� �����
	in.close(); // ��������� ����

	LEXER lex;
	vector <string> tok = lex.Tokens(s1);
	vector <int> tok_id = lex.Tokens_ID(s1);
	bool tok_err = lex.Error(s1);

	for (int n = 1; n < tok_id.size(); n++) //����� ������ � �������
	{
	cout << tok_id[n] << ' ';
	cout << tok[n] << '\n';
	}

	if (tok_err == true)
		cout << "Lexical error!" <<  '\n';

system("pause"); 
}