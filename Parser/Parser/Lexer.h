/*
������ ��� C-��������� �����
CBL(C_Based_Language).

�������� ������� ���������� �.�.
������� ������ ����-08-14
*/
#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <regex>   // ���������� ���������
#include <fstream> // ������ � �������
using namespace std;

//========================== ������ ===========================
class LEXER {

private:

	struct Lex_out		//�������� ��������� ��� ������ �������� �� ������� �������
	{					//��� ������ �������� �� ������� �������
		vector <int> ID;
		vector <string> Tokens;
		bool Err;
	};

	struct Lex_out lexer(string s1)
	{
		ifstream in("Example.cbl"); //��������� ���� ��� ���������� ���������� 

		vector <int> Tokens_ID;    //������� ������� �������� �������
		Tokens_ID.resize(1, 0);
		vector <string> Tokens;    //� �� ���������������
		Tokens.resize(1, "START");
		bool Err = false;

		regex								//���������e ���������		 
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

		for (int n = 1; n < s1.size(); n++)    //��������� �������� � ����� ����
		{										//��������� �� ��������� ������� ������ ������ �� �������.
			int size = s1.size();

			if (s1[0] == ' ') goto ZAPIS_2;

			else if (s1[0] == ';') { Tokens_ID.push_back(1); goto ZAPIS; } //�������� ��������
			else if (s1[0] == '(') { Tokens_ID.push_back(2); goto ZAPIS; }
			else if (s1[0] == ')') { Tokens_ID.push_back(3); goto ZAPIS; }
			else if (s1[0] == '{') { Tokens_ID.push_back(4); goto ZAPIS; }
			else if (s1[0] == '}') { Tokens_ID.push_back(5); goto ZAPIS; }

			else if (regex_match(s1.substr(0, 1), OP1)) //�������� �������� + -
			{
				Tokens_ID.push_back(6); goto ZAPIS;
			}

			else if (regex_match(s1.substr(0, 1), OP2)) //�������� �������� * /
			{
				Tokens_ID.push_back(16); goto ZAPIS;
			}

			else if (regex_match(s1.substr(0, 1), LOGIC)) //�������� ���������� ��������
			{
				Tokens_ID.push_back(7); goto ZAPIS;
			}

			else if ((s1[0] == ':')&(2 < size)) //�������� :=
			{
				if (s1[1] == '=') { n = 2; Tokens_ID.push_back(8); goto ZAPIS; }
				else goto UNCKNOWN_SYMBOL;      //����������� ������������������ ��������
			}

			else if ((s1[0] == 'i')&(2 < size)) //�������� IF
			{
				if (s1[1] == 'f') { n = 2; Tokens_ID.push_back(9); goto ZAPIS; }
				else if (3 < size) //�������� INT
				{
					if (regex_match(s1.substr(0, 3), INT)) { n = 3; Tokens_ID.push_back(10); goto ZAPIS; }
					else goto LETTER_1;             //�������� ��� �������������
				}

				else goto LETTER_1;             //�������� ��� �������������
			}

			else if ((s1[0] == 'd')&(2 < size)) //�������� DO
			{
				if (s1[1] == 'o') { n = 2; Tokens_ID.push_back(11); goto ZAPIS; }
				else goto LETTER_1;             //�������� ��� �������������
			}

			else if ((s1[0] == 'e')&(4 < size)) //�������� ELSE
			{
				if (regex_match(s1.substr(0, 4), ELSE)) { n = 4; Tokens_ID.push_back(12); goto ZAPIS; }
				else goto LETTER_1;             //�������� ��� �������������
			}

			else if ((s1[0] == 'w')&(5 < size)) //�������� WHILE
			{
				if (regex_match(s1.substr(0, 5), WHILE)) { n = 5; Tokens_ID.push_back(13); goto ZAPIS; }
				else goto LETTER_1;             //�������� ��� �������������
			}

			else if (regex_match(s1.substr(0, n), DIGIT)) //�������� �� �����
			{

				for (; regex_match(s1.substr(0, n), INTEGER); n++) //���� ������ ������ ����� �� ������
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

				if (regex_match(s1.substr(0, n), LETTER)) //�������� �� �������������
				{
					for (; regex_match(s1.substr(0, n), ID); n++) //���� ������ ������ �������������� �� ������
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


				else			//����� ����������� ����������� ������ ������� ������
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

	vector <string> Tokens(string s1)   //���������� ������ �������
	{
		Lex_out Out;
		Out.Tokens = lexer(s1).Tokens;
		return(Out.Tokens);
	}

	vector <int> Tokens_ID(string s1)   //�������������� ������ �������
	{
		Lex_out Out;
		Out.ID = lexer(s1).ID;
		return(Out.ID);
	}

	bool Error(string s1)               //���������� ������ ���� ���� ����������� ������
	{
		Lex_out Out;
		Out.Err = lexer(s1).Err;
		return(Out.Err);
	}

};