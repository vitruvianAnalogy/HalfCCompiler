#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<vector>
#include<stack>
#include<list>
#include<map>
using namespace std;

class LIST {

public:
	string type;
	string scope;
	int value;
	//LIST::LIST(){}
	//LIST::LIST(string a, string b, int c) {
	//	type = a;
	//	scope = b;
	//	value = c;
	LIST(){}
	LIST(string a, string b, int c) {
		type = a;
		scope = b;
		value = c;
	}
};
string newLabel;
int firstCase = 1;
class Token
{
public:
	string name;
	string type;
	int line;
	Token(string name, string type, int line)
	{
		this->name = name;
		this->type = type;
		this->line = line;
	}
};
vector<Token> myTokens;
string firstLine;
string secondLine;
string restOfTheLines = "";
map<string, LIST> mymap;
int temp = 0;
int temp2 = 0;

void program();
void var_section();
void body();
void id_list();
void stmt_list();
void stmt();
void assign_stmt();
void print_stmt();
void while_stmt();
void if_stmt();
void switch_stmt();
void expr();
void primary();
void op();
void case_list(string caseIdentifier,string newLabel2);
void default_case();
void relop();
void condition();
void cases(string caseIdentifier,string newLabel2);
int currentToken = 0;
int state_machine[13][11];
int pc = 1;
string relopVariable = "empty";
string opVariable = "empty";
int counter = 0;
bool insert_symbol_table(string token, int value);



/*
void int_var_decl();
void array_var_decl();
void var_access();
void term();
void factor();

*/
string labelGenerator()
{
	counter++;
	ostringstream convert;
	convert << counter;
	string counterString = convert.str();
	string returnValue = "#e" + counterString;
	return returnValue;
}


void errorMessage(string missingVariable){
	cout << "EXPECTED " << missingVariable;
}

string global_type = "int";
string global_scope = "global";

bool insert_symbol_table(string token, int value)
{
	if (mymap.find(token) == mymap.end())
	{
		mymap[token].scope = global_scope; //.Listpush_back(LIST(global_type, global_scope, value));
		mymap[token].type = global_type;
		mymap[token].value = value;
		if (token.at(0) != '#')
		{
			if (temp == 0)
			{
				firstLine = token;
				temp++;
			}
			else
			{
				firstLine = firstLine + "," + token;
			}
		}
		else
		{
			if (temp2 == 0)
			{
				secondLine = token;
				temp2++;
				ostringstream convert;
				convert << mymap[token].value;
				string counterString = convert.str();
				secondLine = secondLine + "=" + counterString;
			}
			else
			{
				secondLine = secondLine + "," + token;
				ostringstream convert;
				convert << mymap[token].value;
				string counterString = convert.str();
				secondLine = secondLine + "=" + counterString;
			}
		}


		return true;
	}
	else
	{
		return false;
	}
}

bool findToken(string token)
{
	if (mymap.find(token) != mymap.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//void insertToken(string token)
//{
//	/*
//	Insert token in the symbol table
//	*/
//}




//checks delimiters
bool isDelimiter(char line)
{
	if (line == ';' || line == ',' || line == ':' || line == '(' || line == ')' || line == '[' || line == ']' || line == '{' || line == '}')
		return true;
	else
		return false;
}

//checks operators
bool isOperator(char line)
{
	if (line == '+' || line == '-' || line == '*' || line == '%' || line == '/' || line == '=' || line == '<' || line == '>' || line == '!' || line == '|')
		return true;
	else
		return false;
}

//Writes
void WriteFile(string strTemp, string message, ofstream *outputFile, int lineNumber)
{
	/*cout << message << "\t" << strTemp;
	cout << "\n";

	*outputFile << message << "\t" << strTemp;
	*outputFile << "\n";*/
	myTokens.push_back(Token(strTemp, message, lineNumber));

}
void WriteRestLine(ofstream *outputFile)
{
	cout << restOfTheLines;
	cout << "\n";

	*outputFile << restOfTheLines;
	*outputFile << "\n";

}
void WriteFirstLine(ofstream *outputFile)
{
	cout << firstLine;
	cout << "\n";

	*outputFile << firstLine;
	*outputFile << "\n";

}
void WriteSecondLine(ofstream *outputFile)
{
	cout << secondLine;
	cout << "\n";
	if (secondLine == "")
	{
		secondLine = "#dummy=1";
	}
	*outputFile << secondLine;
	*outputFile << "\n";

}

//alphabets from g-z
bool isAlphabet(char alphabet)
{
	if ((alphabet >= 71 && alphabet <= 90 && alphabet != 88 && alphabet != 120) || (alphabet >= 103 && alphabet <= 122 && alphabet != 88 && alphabet != 120))
		return true;
	else
		return false;
}

//all the keywords
bool isKeyword(string keyword)
{
	if (keyword == "if" || keyword == "else" || keyword == "while" || keyword == "return" || keyword == "int" || keyword == "float" || keyword == "void" || keyword == "char" || keyword == "string" || keyword == "boolean" || keyword == "for" || keyword == "IF" || keyword == "ELSE" || keyword == "WHILE" || keyword == "RETURN" || keyword == "INT" || keyword == "FLOAT" || keyword == "VOID" || keyword == "CHAR" || keyword == "STRING" || keyword == "BOOLEAN" || keyword == "FOR" || keyword == "SWITCH" || keyword == "CASE" || keyword == "PRINT" || keyword == "print" || keyword == "\tif" || keyword == "\telse" || keyword == "\twhile" || keyword == "\treturn" || keyword == "\tint" || keyword == "\tfloat" || keyword == "\tvoid" || keyword == "\tchar" || keyword == "\tstring" || keyword == "\tboolean" || keyword == "\tfor" || keyword == "\tIF" || keyword == "\tELSE" || keyword == "\tWHILE" || keyword == "\tRETURN" || keyword == "\tINT" || keyword == "\tFLOAT" || keyword == "\tVOID" || keyword == "\tCHAR" || keyword == "\tSTRING" || keyword == "\tBOOLEAN" || keyword == "\tFOR" || keyword == "\tSWITCH" || keyword == "\tCASE" || keyword == "\tPRINT" || keyword == "\tprint")
		return true;
	else
		return false;
}

void program()
{
	var_section();
	body();
	pc++;
	cout << "OPR " << "0, 0" << "\n";
	restOfTheLines = restOfTheLines + "OPR " + "0, 0" + "\n";
}

void var_section()
{
	id_list();

	if (myTokens[currentToken].name != ";")
	{
		errorMessage(";");
	}
	else
	{
		currentToken++;
	}
}

void id_list()
{
	if (myTokens[currentToken].type == "IDENTIFIER")
	{
		currentToken++;
		if (findToken(myTokens[currentToken - 1].name))
		{
			cout << "Duplicate variable " << myTokens[currentToken - 1].name;
		}
		else
		{
			insert_symbol_table(myTokens[currentToken - 1].name, pc);
		}

		if (myTokens[currentToken].name == ",")
		{
			currentToken++;
			id_list();
		}
	}

}

void body()
{

	if (myTokens[currentToken].name != "{")
	{
		errorMessage("{");
	}
	else
	{
		global_scope = "main";
		currentToken++;
		stmt_list();
		if (myTokens[currentToken].name != "}")
		{
			errorMessage("}");
		}
		else
		{
			currentToken++;
		}
	}


	//if (myTokens[currentToken].name != "main")
	//{
	//	errorMessage("main");
	//}
	//else
	//{
	//	currentToken++;

	//	if (myTokens[currentToken].name != "(")
	//	{
	//		errorMessage("(");
	//	}
	//	else
	//	{
	//		currentToken++;
	//		if (myTokens[currentToken].name != ")")
	//		{
	//			errorMessage(")");
	//		}
	//		else
	//		{
	//			currentToken++;
	//			if (myTokens[currentToken].name != "{")
	//			{
	//				errorMessage("{");
	//			}
	//			else
	//			{
	//				global_scope = "main";
	//				currentToken++;
	//				stmt_list();
	//				if (myTokens[currentToken].name != "}")
	//				{
	//					errorMessage("}");
	//				}
	//				else
	//				{
	//					currentToken++;
	//				}
	//			}
	//		}
	//	}
	//}

}

void op()
{
	if (myTokens[currentToken].name == "+")
	{
		opVariable = "2";
		currentToken++;
	}
	else if (myTokens[currentToken].name == "-")
	{
		opVariable = "3";
		currentToken++;
	}
	else if (myTokens[currentToken].name == "*")
	{
		opVariable = "4";
		currentToken++;
	}
	else if (myTokens[currentToken].name == "/")
	{
		opVariable = "5";
		currentToken++;
	}
	else
	{
		cout << "WRONG OPERATOR ";
	}

}

void stmt_list()
{
	stmt();
	if (myTokens[currentToken].name == "}")
	{
		//currentToken++; checking } in body as well so no need to increment currentToken
		//incrementing currentToken in body as well
	}
	else
	{
		stmt_list();
	}
}

void stmt()
{
	if (myTokens[currentToken].type == "IDENTIFIER")
		assign_stmt();
	else if (myTokens[currentToken].name == "print" || myTokens[currentToken].name == "\tprint")
		print_stmt();
	else if (myTokens[currentToken].name == "while" || myTokens[currentToken].name == "WHILE")
		while_stmt();
	else if (myTokens[currentToken].name == "if" || myTokens[currentToken].name == "IF")
		if_stmt();
	else if (myTokens[currentToken].name == "switch" || myTokens[currentToken].name == "SWITCH")
	{
		switch_stmt();
	}		
	else
	{
		cout << "none of the valid statements";
	}
}

void assign_stmt()
{
	string variable = myTokens[currentToken].name;
	if (myTokens[currentToken].type != "IDENTIFIER")
	{
		cout << "NOT AN ASSIGN";
	}
	else
	{
		currentToken++;
		if (myTokens[currentToken].name != "=")
		{
			cout << "NOT AN ASSIGN";
		}
		else
		{
			currentToken++;
			if (myTokens[currentToken + 1].name == "+" || myTokens[currentToken + 1].name == "-" || myTokens[currentToken + 1].name == "*" || myTokens[currentToken + 1].name == "/")
				expr();
			else
				primary();
			if (myTokens[currentToken].name != ";")
			{
				errorMessage(";");
			}
			else
			{
				currentToken++;
				pc++;
				cout << "STO " << variable << ", 0" << "\n";
				restOfTheLines = restOfTheLines + "STO " + variable + ", 0" + "\n";
			}
		}

	}
}

void expr()
{
	primary();
	op();
	primary();
	pc++;
	cout << "OPR " << opVariable << ", 0" << "\n";
	restOfTheLines = restOfTheLines + "OPR " + opVariable + ", 0" + "\n";

}

void primary()
{
	if (myTokens[currentToken].type == "IDENTIFIER")
	{
		currentToken++;
		if (findToken(myTokens[currentToken - 1].name))
		{
			pc++;
			cout << "LOD " << myTokens[currentToken - 1].name << ", 0" << "\n";
			restOfTheLines = restOfTheLines + "LOD " + myTokens[currentToken - 1].name + ", 0" + "\n";
		}
		else
			errorMessage("IDENTIFIER");
	}
	else if (myTokens[currentToken].type == "INTEGER")
	{
		pc++;
		cout << "LIT " << myTokens[currentToken].name << ", 0" << "\n";
		restOfTheLines = restOfTheLines + "LIT " + myTokens[currentToken].name + ", 0" + "\n";
		currentToken++;
	}
}
void condition()
{
	primary();
	relop();
	if (myTokens[currentToken - 1].name == "!"&&myTokens[currentToken].name == "=")
	{
		relopVariable = "16";
		currentToken++;
	}
	else if (myTokens[currentToken - 1].name == "<"&&myTokens[currentToken].name == "=")
	{
		relopVariable = "14";
		currentToken++;
	}
	else if (myTokens[currentToken - 1].name == ">"&&myTokens[currentToken].name == "=")
	{
		relopVariable = "13";
		currentToken++;
	}
	if (myTokens[currentToken - 1].name == "!"&&myTokens[currentToken].name == "=")
	{
		relopVariable = "16";
		currentToken++;
	}
	else if (myTokens[currentToken - 1].name == "<"&&myTokens[currentToken].name == ">")
	{
		relopVariable = "16";
		currentToken++;
	}
	primary();
	pc++;
	cout << "OPR " << relopVariable << ", 0" << "\n";
	restOfTheLines = restOfTheLines + "OPR " + relopVariable + ", 0" + "\n";
}

void relop()
{
	if (myTokens[currentToken].name == ">")
	{
		relopVariable = "11";
		currentToken++;
	}
	else if (myTokens[currentToken].name == "<")
	{
		relopVariable = "12";
		currentToken++;
	}
	else if (myTokens[currentToken].name == "!=")
	{
		relopVariable = "13";
		currentToken++;
	}
	else
	{
		errorMessage("relational operator");
	}
}

void print_stmt()
{
	if (myTokens[currentToken].name == "print")
	{
		currentToken++;
		if (myTokens[currentToken].type == "IDENTIFIER")
		{
			currentToken++;
			pc++;
			cout << "LOD " << myTokens[currentToken - 1].name << ", 0" << "\n";
			restOfTheLines = restOfTheLines + "LOD " + myTokens[currentToken - 1].name + ", 0" + "\n";
			if (myTokens[currentToken].name == ";")
			{
				currentToken++;
				pc++;
				cout << "OPR 21, 0" << "\n";
				restOfTheLines = restOfTheLines + "OPR 21, 0" + "\n";
			}
		}
	}
}

void while_stmt()
{
	string newLabel = labelGenerator();
	string newLabel2 = labelGenerator();
	if (myTokens[currentToken].name == "while" || myTokens[currentToken].name == "WHILE")
	{
		currentToken++;

		insert_symbol_table(newLabel, pc);
	}
	condition();
	pc++;
	cout << "JMC " << newLabel2 << ", false" << "\n";
	restOfTheLines = restOfTheLines + "JMC " + newLabel2 + ", false" + "\n";
	body();
	pc++;
	cout << "JMP " << newLabel << ", 0" << "\n";
	restOfTheLines = restOfTheLines + "JMP " + newLabel + ", 0" + "\n";
	insert_symbol_table(newLabel2, pc);
}

void if_stmt()
{
	if (myTokens[currentToken].name == "IF" || myTokens[currentToken].name == "if")
	{
		string label = labelGenerator();
		currentToken++;
		condition();
		pc++;
		cout << "JMC " << label << ", false" << "\n";
		restOfTheLines = restOfTheLines + "JMC " + label + ", false" + "\n";
		body();
		insert_symbol_table(label, pc);
	}
}
void switch_stmt()
{
	string newLabel2=labelGenerator();
	if (myTokens[currentToken].name == "SWITCH" || myTokens[currentToken].name == "switch")
	{
		currentToken++;
		if (myTokens[currentToken].type == "IDENTIFIER")
		{
			string caseIdentifier = myTokens[currentToken].name;
			currentToken++;
			if (myTokens[currentToken].name == "{")
			{				
				currentToken++;
				case_list(caseIdentifier,newLabel2);
				
				if (myTokens[currentToken].name == "}")
				{
					currentToken++;
				}
				else
				{
					default_case();
					if (myTokens[currentToken].name != "}")
					{
						errorMessage("}");
					}
					else
					{
						currentToken++;
					}
				}
			}
			insert_symbol_table(newLabel2, pc+2);
		}
	}
}

void case_list(string caseIdentifier,string newLabel2)
{
	cases(caseIdentifier,newLabel2);
	
	if (myTokens[currentToken].name == "case" || myTokens[currentToken].name == "CASE")
	{
		insert_symbol_table(newLabel, pc + 1);
		case_list(caseIdentifier,newLabel2);
	}

}

void cases(string caseIdentifier,string newLabel2)
{
	if (myTokens[currentToken].name == "case" || myTokens[currentToken].name == "CASE")
	{
		newLabel = labelGenerator();
		currentToken++;
		pc++;
		//if (firstCase == 1)
		//{
		//	firstCase++;
		//}
		//else
		//{
		//	insert_symbol_table(newLabel, pc + 1);
		//}
		restOfTheLines = restOfTheLines + "LOD " + caseIdentifier + ",0" + "\n";
		if (myTokens[currentToken].type == "INTEGER")
		{
			pc++;
			restOfTheLines = restOfTheLines + "LIT " + myTokens[currentToken].name + ", 0" + "\n";
			pc++;
			restOfTheLines = restOfTheLines + "OPR " + "15,0" + "\n";
			currentToken++;
			
			pc++;
			restOfTheLines = restOfTheLines + "JMC " + newLabel + ", false" + "\n";
			//string newLabel2 = labelGenerator();

			if (myTokens[currentToken].name == ":")
			{
				currentToken++;
				body();
				restOfTheLines = restOfTheLines + "JMP " +newLabel2  + ",0"+"\n";
			}
			//string JMCLabel = labelGenerator();
			//restOfTheLines = restOfTheLines + "this is jmc label" + JMCLabel;
			//insert_symbol_table(newLabel2, pc + 1);
		}
	}
}

void default_case()
{
	if (myTokens[currentToken].name == "DEFAULT")
	{
		currentToken++;
		if (myTokens[currentToken].name == ":")
		{
			insert_symbol_table(newLabel, pc + 2);
			currentToken++;
			body();
		}
	}
}

//number from 0-7
bool isOctalLiterals(char letter)
{
	if (letter >= 49 && letter <= 55)
		return true;
	else
		return false;
}

//numbers 8 and 9
bool isHexaDecimalNumber(char letter)
{
	if (letter >= 56 && letter <= 57)
		return true;
	else
		return false;
}

//alphabets from a-f
bool isHexaDecimalAlpha(char letter)
{
	if ((letter >= 65 && letter <= 70) || (letter >= 97 && letter <= 102))
		return true;
	else
		return false;
}

//DFA
int calculateNextState(string current_state, char letter)
{
	int new_state;
	int col = 99;

	//check the inputs
	if (isAlphabet(letter))
		col = 6;
	//else if (isNumber(letter))
	//	col = 2;
	else if (isOctalLiterals(letter))
		col = 2;
	else if (isHexaDecimalNumber(letter))
		col = 3;
	else if (isHexaDecimalAlpha(letter))
		col = 4;
	else if (letter == '$')
		col = 9;
	else if (letter == '.')
		col = 8;
	else if (letter == '_')
		col = 10;
	else if (letter == '0')
		col = 1;
	else if (letter == 'x' || letter == 'X')
		col = 0;

	new_state = atoi(current_state.c_str());
	if (col == 99)
	{
		return 9;
	}

	return state_machine[new_state][col];
}

//Lexer method
string Lexer(string strTemp)
{
	if (strTemp == "")
		return "empty";

	string state = "";
	for (int i = 0; i <= (strTemp.length() - 1); i++)
	{
		int temp = (calculateNextState(state, strTemp[i]));
		state = static_cast<ostringstream*>(&(ostringstream() << temp))->str();
	}

	if (state == "DQDQ") return "STRING";
	else if (state == "3" || state == "2") return "INTEGER";
	else if (state == "4") return "OCTAL";
	else if (state == "6") return "HEXADECIMAL";
	else if (state == "SQDOTSQ") return "CHARACTER";
	else if (state == "8") return "FLOAT";
	else if (state == "7") return "IDENTIFIER";
	else if (state == "9") return "UNDEFINED";

}

//main method
int main(int argc, char* argv[])
{
	//myMapType mymap;
	//populate state machine
	state_machine[0][0] = 7;	state_machine[0][1] = 3;	state_machine[0][2] = 2;	state_machine[0][3] = 2;	state_machine[0][4] = 7;	state_machine[0][5] = 7; state_machine[0][6] = 7; state_machine[0][7] = 7; state_machine[0][8] = 1; state_machine[0][9] = 7; state_machine[0][10] = 7;
	state_machine[1][0] = 9;	state_machine[1][1] = 8;	state_machine[1][2] = 8;	state_machine[1][3] = 8;	state_machine[1][4] = 9;	state_machine[1][5] = 9; state_machine[1][6] = 9; state_machine[1][7] = 9; state_machine[1][8] = 9; state_machine[1][9] = 9; state_machine[1][10] = 9;
	state_machine[2][0] = 7;	state_machine[2][1] = 2;	state_machine[2][2] = 2;	state_machine[2][3] = 2;	state_machine[2][4] = 9;	state_machine[2][5] = 9; state_machine[2][6] = 9; state_machine[2][7] = 9; state_machine[2][8] = 10; state_machine[2][9] = 9; state_machine[2][10] = 9;
	state_machine[3][0] = 5;	state_machine[3][1] = 4;	state_machine[3][2] = 4;	state_machine[3][3] = 9;	state_machine[3][4] = 9;	state_machine[3][5] = 9; state_machine[3][6] = 9; state_machine[3][7] = 9; state_machine[3][8] = 1; state_machine[3][9] = 9; state_machine[3][10] = 9;
	state_machine[4][0] = 9;	state_machine[4][1] = 4;	state_machine[4][2] = 4;	state_machine[4][3] = 9;	state_machine[4][4] = 9;	state_machine[4][5] = 9; state_machine[4][6] = 9; state_machine[4][7] = 9; state_machine[4][8] = 9; state_machine[4][9] = 9; state_machine[4][10] = 9;
	state_machine[5][0] = 9;	state_machine[5][1] = 6;	state_machine[5][2] = 6;	state_machine[5][3] = 6;	state_machine[5][4] = 6;	state_machine[5][5] = 6; state_machine[5][6] = 9; state_machine[5][7] = 9; state_machine[5][8] = 9; state_machine[5][9] = 9; state_machine[5][10] = 9;
	state_machine[6][0] = 9;	state_machine[6][1] = 6;	state_machine[6][2] = 6;	state_machine[6][3] = 6;	state_machine[6][4] = 6;	state_machine[6][5] = 6; state_machine[6][6] = 9; state_machine[6][7] = 9; state_machine[6][8] = 9; state_machine[6][9] = 9; state_machine[6][10] = 9;
	state_machine[7][0] = 7;	state_machine[7][1] = 7;	state_machine[7][2] = 7;	state_machine[7][3] = 7;	state_machine[7][4] = 7;	state_machine[7][5] = 7; state_machine[7][6] = 7; state_machine[7][7] = 7; state_machine[7][8] = 9; state_machine[7][9] = 9; state_machine[7][10] = 7;
	state_machine[8][0] = 9;	state_machine[8][1] = 8;	state_machine[8][2] = 8;	state_machine[8][3] = 8;	state_machine[8][4] = 9;	state_machine[8][5] = 9; state_machine[8][6] = 9; state_machine[8][7] = 9; state_machine[8][8] = 9; state_machine[8][9] = 9; state_machine[8][10] = 9;
	state_machine[9][0] = 9;	state_machine[9][1] = 9;	state_machine[9][2] = 9;	state_machine[9][3] = 9;	state_machine[9][4] = 9;	state_machine[9][5] = 9; state_machine[9][6] = 9; state_machine[9][7] = 9; state_machine[9][8] = 9; state_machine[9][9] = 9; state_machine[9][10] = 9;
	state_machine[10][0] = 9;	state_machine[10][1] = 8;	state_machine[10][2] = 8;	state_machine[10][3] = 8;	state_machine[10][4] = 9;	state_machine[10][5] = 9; state_machine[10][6] = 9; state_machine[10][7] = 9; state_machine[10][8] = 9; state_machine[10][9] = 9; state_machine[10][10] = 9;
	state_machine[11][0] = 9;	state_machine[11][1] = 4;	state_machine[11][2] = 4;	state_machine[11][3] = 9;	state_machine[11][4] = 9;	state_machine[11][5] = 9; state_machine[11][6] = 9; state_machine[11][7] = 9; state_machine[11][8] = 9; state_machine[11][9] = 9; state_machine[11][10] = 9;
	state_machine[12][0] = 9;	state_machine[12][1] = 6;	state_machine[12][2] = 6;	state_machine[12][3] = 6;	state_machine[12][4] = 6;	state_machine[12][5] = 6; state_machine[12][6] = 9; state_machine[12][7] = 9; state_machine[12][8] = 9; state_machine[12][9] = 9; state_machine[12][10] = 9;

	//check if number of arguments are 2
	if (argc != 3)
	{
		cout << "Number of arguments incorrect\n";
		return 1;
	}

	cout << "Name of the input file is " << argv[1] << "\n";

	//Temporary TOKEN
	string strTemp = "";
	int lineNumber = 0;
	//read into input stream
	ifstream infile(argv[1]);			//input stream
	ofstream outputFile(argv[2]);		//output stream
	string line;						//line variable
	string category;					//category of the token

	//read the file
	if (infile.is_open())
	{
		while (getline(infile, line))
		{
			lineNumber++;
			for (int i = 0; i < line.length(); i++)
			{
				//reads one character at a time
				char c = line.at(i);
				if (c == '\t')
					continue;
				//if (c == 32)
				//	continue;

				//check for double quote
				if (c == 34)
				{
					int flag = 0;
					//send the token accumulated until now into the Lexer
					category = Lexer(strTemp);
					if (category != "empty")
					{
						if (category == "IDENTIFIER"&& isKeyword(strTemp))
							category = "KEYWORD";
						WriteFile(strTemp, category, &outputFile, lineNumber);
					}
					strTemp = "";
					if (c == 32)
						continue;
					strTemp = strTemp + c;		//concatenate after opening quote
					for (int j = i + 1; j < line.length(); j++)	//concatenate until closing quote
					{
						c = line.at(j);			//the next char after quote is read
						if (c == 34 && (line.at(j - 1)) != 92)			//check for closing quote
						{
							flag = flag + 1;
							//concatenate the last quote
							if (c == 32)
								continue;
							strTemp = strTemp + c;
							category = "STRING";
							i = j;
							WriteFile(strTemp, category, &outputFile, lineNumber);
							strTemp = "";
							break;
						}
						//and concatenated
						if (c == 32)
							continue;
						strTemp = strTemp + c;
						i = j;
					}
					if (flag != 1)
					{
						category = "UNDEFINED";
						WriteFile(strTemp, category, &outputFile, lineNumber);
						strTemp = "";
					}
					strTemp = "";
				}
				else if (c == 39)	//check for single quote
				{
					int length = 1;
					int flagTwo = 0;
					//send the token accumulated until now into the Lexer
					category = Lexer(strTemp);
					if (category != "empty")
					{
						if (category == "IDENTIFIER"&&isKeyword(strTemp))
							category = "KEYWORD";
						WriteFile(strTemp, category, &outputFile, lineNumber);
					}

					strTemp = "";
					if (c == 32)
						continue;
					strTemp = strTemp + c;
					for (int j = i + 1; j < line.length(); j++)
					{
						c = line.at(j);
						length = length + 1;
						if (c == 39 && (line.at(j - 1)) != 92)
						{
							flagTwo = flagTwo + 1;
							strTemp = strTemp + c;
							category = "CHARACTER";
							i = j;
							if (length>3 || length == 2)
							{
								category = "UNDEFINED";
								WriteFile(strTemp, category, &outputFile, lineNumber);
								strTemp = "";
								break;
							}

							WriteFile(strTemp, category, &outputFile, lineNumber);
							strTemp = "";
							break;
						}
						if (c == 92)
							length = length - 1;
						if (c == 32)
							continue;
						strTemp = strTemp + c;
					}
					if (flagTwo != 1)
					{
						category = "UNDEFINED";
						WriteFile(strTemp, category, &outputFile, lineNumber);
						strTemp = "";
					}
				}
				else
				{
					if (i == line.length() - 1)
					{
						if (!isDelimiter(c))
						{
							if (c == 32)
								continue;
							strTemp = strTemp + c;
						}
					}
					if (c == '\t' || isDelimiter(c) || isOperator(c) || c == 32 || (i == line.length() - 1))
					{
						if (strTemp != "" && strTemp != " ")
						{
							category = Lexer(strTemp);
							if (category != "empty")
							{
								if (category == "IDENTIFIER" && isKeyword(strTemp))
									category = "KEYWORD";
								WriteFile(strTemp, category, &outputFile, lineNumber);
							}
							strTemp = "";
						}
						if (i == line.length() - 1)
						{
							if (!isDelimiter(c))
							{
								strTemp = "";
							}
							else
								strTemp = c;
						}
						else
							strTemp = c;

						if (isDelimiter(c))
						{
							category = "DELIMITER";
							WriteFile(strTemp, category, &outputFile, lineNumber);
							strTemp = "";
						}
						if (c == ' ')
						{
							strTemp = "";
						}
						if (isOperator(c))
						{
							category = "OPERATOR";
							WriteFile(strTemp, category, &outputFile, lineNumber);
							strTemp = "";
						}

					}
					else
					{
						if (c == 32)
							continue;
						strTemp = strTemp + c;
					}
				}
			}
		}
		infile.close();
	}

	program();
	if (restOfTheLines != "")
	{
		WriteFirstLine(&outputFile);
		WriteSecondLine(&outputFile);
		WriteRestLine(&outputFile);
	}
	cout << pc;
}

