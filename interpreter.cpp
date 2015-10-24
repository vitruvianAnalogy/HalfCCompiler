#include <iostream>
#include<fstream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

bool DEBUG = true;
struct Instruction {
  string name;
  string parameter1;
  string parameter2;
};
struct List{
	int index;
	int value;
};

map<string, int> symbol_table;
map<string, int> symbol_table_arr;
vector<Instruction> instructions;
int program_counter;
stack<string> register_zero;

string trim(string &str) {
   str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
   return str;
}

void create_symbol_table_vars(string line) {
  int pos;
  string var;
  pos = line.find(",");
  while (pos>0) {
    var = line.substr(0,pos);
    line = line.substr(pos+1);
    pos = line.find(",");

    symbol_table[trim(var)] = 0;
  }

  symbol_table[trim(line)] = 0;
}

void create_symbol_table_arrayvars(string line) {
	/*int pos;
	string var;
	pos = line.find(",");
	while (pos>0) {
		var = line.substr(0, pos);
		line = line.substr(pos + 1);
		pos = line.find(",");
		List l;
		l.index = 0; l.value = 0;
		symbol_table[trim(var)] = l;
	}
	pos = line.find(":");
	line = line.substr(0,pos);
	List l;
	l.index = 0; l.value = 0;
	symbol_table[trim(line)] = l;*/
	int pos;
	 string var;

	 //finding size of an array
	int posColon=0;
	posColon = line.find(":");

	string varSize = line.substr(posColon+1);
	varSize = trim(varSize);
	int posBracket = varSize.find("[");
	int posEndBracket = varSize.find("]");
	varSize = varSize.substr(posBracket+1,posEndBracket);
	//cout<<"size of array is "<<varSize<<endl;
	int value = stoi(varSize);

	//extracting array variables
	pos = line.find(",");
	while (pos>0) {
		var = line.substr(0, pos);
		line = line.substr(pos + 1);
		pos = line.find(",");

		for (int i = 0; i < value; i++)
		{
			char str[15];
			sprintf(str,"%d",i);
			string varTemp = var + str;
			//cout<<"array name is "<<varTemp<<endl;
			symbol_table_arr[trim(varTemp)] = 0;
		}
		//cout<<"element in c2 is "<<symbol_table_arr["c2"]<<endl;
	}
	pos = line.find(":");
	line = line.substr(0,pos);
	for (int i = 0; i < value; i++)
	{
		char str[15];
		sprintf(str,"%d",i);
		string varTemp = line + str;
		symbol_table_arr[trim(varTemp)] = 0;
	}
}

void create_symbol_table_labels(string line) {
  int pos, pos_equal;
  string var;
  pos = line.find(",");
  while (pos>0) {
    var = line.substr(0,pos);
    line = line.substr(pos+1);
    pos = line.find(",");
    // label
    pos_equal = var.find("=");
    string name = var.substr(0,pos_equal);
    int value = stoi(var.substr(pos_equal+1));
    symbol_table[trim(name)] = value;
  }
  // label
  pos_equal = line.find("=");
  string name = line.substr(0,pos_equal);
  int value = stoi(line.substr(pos_equal+1));
  symbol_table[trim(name)] = value;
}

bool insert_instruction(string line) {
  Instruction i;
  int pos;
  // instruction
  pos = line.find(" ");
  if (pos == -1) return false;
  i.name = line.substr(0,pos);
  i.name = trim(i.name);
  line = line.substr(pos+1);
  // first parameter
  pos = line.find(",");
  if (pos == -1) return false;
  string p1 = line.substr(0,pos);
  i.parameter1 = trim (p1);
  line = line.substr(pos+1);
  // second parameter
  i.parameter2 = trim(line);
  instructions.push_back(i);
  return true;
}

bool load_file(char* file) {
  ifstream source;
  string line;
  source.open(file);
  if (source.is_open()) {
    if (DEBUG) cout << "* 1. Reading the file: "<<file<<"."<<endl;
    if (DEBUG) cout << "* 2. Getting variables... ";
    getline(source, line);
    create_symbol_table_vars(line);
    if (DEBUG) cout << "done."<<endl;
	if (DEBUG) cout << "* 3. Getting Array variables... ";
	getline(source, line);
	create_symbol_table_arrayvars(line);
	if (DEBUG) cout << "done." << endl;
    if (DEBUG) cout << "* 4. Getting labels... ";
    getline(source, line);
    create_symbol_table_labels(line);
    if (DEBUG) cout << "done."<<endl;
    if (DEBUG) cout << "* 5. Loading instructions... ";
    while (!source.eof()) {
      getline(source, line);
      if (!line.empty()) {
        if (!insert_instruction(line)) return false;
      }
    }
    if (DEBUG) cout << "done."<<endl;
  } else {
    cout << "* ERROR - unable to open file: "<<file<<endl;
  }
  source.close();
  return true;;
}

void execution_loop() {
  if ( symbol_table.count ("#pc") > 0 ) {
    program_counter = symbol_table["#pc"]-1;
  } else {
    program_counter = 0;
  }
  if (DEBUG) cout << "* 5. Program running... "<<endl;
  bool exit=false;
  do {
    if (DEBUG) cout << "-" <<program_counter << ": ";
    if (DEBUG) cout << instructions[program_counter].name << " ";
    if (DEBUG) cout << instructions[program_counter].parameter1 << " ";
    if (DEBUG) cout << instructions[program_counter].parameter2 << " ;" <<endl;

    if(instructions[program_counter].name.compare("LIT") == 0 )
	{
      register_zero.push(instructions[program_counter].parameter1);
	}
	else if (instructions[program_counter].name.compare("STA") == 0)
	{
		int value = register_zero.top();
		//cout<<"sta store in index "<<index;
		register_zero.pop();
		string index = (string)register_zero.top();
		//cout<<"value that is stored in it is "<<value;
		symbol_table_arr[instructions[program_counter].parameter1 + index] = value;
		register_zero.pop();
	}
	else if (instructions[program_counter].name.compare("LDA") == 0)
	{
		string index = (string)register_zero.top();
		//cout<<"inside lda index is "<<index;
		register_zero.pop();
		int value = symbol_table_arr[instructions[program_counter].parameter1 + index];
		//cout<<instructions[program_counter].parameter1 + index;
		//cout<<"inside value stored in it is "<<value;
		register_zero.push(to_string(value));
	}
	else if (instructions[program_counter].name.compare("LOD") == 0)
	{
      int value = symbol_table [instructions[program_counter].parameter1];
      register_zero.push(to_string(value));
    }
	else if(instructions[program_counter].name.compare("STO") == 0 )
	{
      int value = stoi ((string)register_zero.top());
      symbol_table [instructions[program_counter].parameter1] = value;
      register_zero.pop();
    }
	else if(instructions[program_counter].name.compare("JMP") == 0 )
	{
      if (isdigit (instructions[program_counter].parameter1[0]))
	  {
        program_counter = stoi(instructions[program_counter].parameter1)-2;
      }
	  else
	  {
        //its a label
        if (symbol_table.count (instructions[program_counter].parameter1) != 0 )
		{
          int value = symbol_table [instructions[program_counter].parameter1];
          program_counter = value-2;
        }
		else
		{
          cout << "\tUndefined label: "<< instructions[program_counter].parameter1 << endl;
          return;
        }
      }
    }
	else if(instructions[program_counter].name.compare("JMC") == 0)
	{
      string value = trim(register_zero.top());
      register_zero.pop();
      string param = trim(instructions[program_counter].parameter2);
      if (value == instructions[program_counter].parameter2)
	  {
        if (isdigit (instructions[program_counter].parameter1[0]))
		{
          program_counter = stoi(instructions[program_counter].parameter1)-2;
        }
		else
		{
          //its a label
          if (symbol_table.count (instructions[program_counter].parameter1) != 0 )
		  {
            int value = symbol_table [instructions[program_counter].parameter1];
            program_counter = value-2;
          }
		  else
		  {
            cout << "\tUndefined label: "<< instructions[program_counter].parameter1 << endl;
            return;
          }
        }
      }
    }
	else if(instructions[program_counter].name.compare("OPR") == 0)
	{
      int value1, value2, result;
      switch (stoi(instructions[program_counter].parameter1))
	  {
        case 0: exit = true; break;
        case 1: break;
        case 2:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			result = value1 + value2;
			register_zero.push(to_string(result));
			break;
        case 3:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			result = value1 - value2;
			register_zero.push(to_string(result));
			break;
        case 4:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			result = value1 * value2;
			register_zero.push(to_string(result));
			break;
        case 5:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			result = value1 / value2;
			register_zero.push(to_string(result));
			break;
        case 11:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			if (value1 > value2) {
				register_zero.push("true");
			}
			else {
				register_zero.push("false");
			}
			break;
        case 12:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			if (value1 < value2) {
				register_zero.push("true");
			}
			else {
				register_zero.push("false");
			}
			break;
        case 13:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			if (value1 >= value2) {
				register_zero.push("true");
			}
			else {
				register_zero.push("false");
			}
			break;
        case 14:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			if (value1 <= value2) {
				register_zero.push("true");
			}
			else {
				register_zero.push("false");
			}
			break;
        case 15:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			if (value1 == value2) {
				register_zero.push("true");
			}
			else {
				register_zero.push("false");
			}
			break;
        case 16:
			value2 = stoi(register_zero.top());
			register_zero.pop();
			value1 = stoi(register_zero.top());
			register_zero.pop();
			if (value1 != value2) {
				register_zero.push("true");
			}
			else {
				register_zero.push("false");
			}
			break;
        case 20:
			if (DEBUG) cout << "\t>" << register_zero.top() << endl;
			else cout << register_zero.top();
			register_zero.pop();
			break;
        case 21:
			if (DEBUG) cout << "\t>" << register_zero.top() << "<" << endl << endl;
			else cout << register_zero.top() << endl;
			register_zero.pop();
			break;
        default:
          cout << "\tUndefined operator: "<<instructions[program_counter].parameter1<<endl;
          return;
          break;
      }
    }
    program_counter++;
  } while (!exit && program_counter<instructions.size());
    if (DEBUG) cout << "* 6. Program ends "<<endl;
}

void show() {
  // symbol table
  for(map<string,int>::iterator it = symbol_table.begin(); it != symbol_table.end(); ++it) {
    cout << "-" << it->first << ":" << symbol_table[it->first] << "-" << endl;
  }
  // instructions
  for(vector<Instruction>::iterator it = instructions.begin(); it != instructions.end(); it++) {
    cout<<"-"<< it->name<<"-"<< it->parameter1<<"-" << trim(it->parameter1)<<"-" << endl;
  }
}

int main(int argc, char** argv) {
  if (argc == 2 && argv[1]!=NULL) {
    DEBUG = false;
    if (!load_file(argv[1])) return 1;
  } else if (argc == 3 && argv[1]!=NULL && argv[2]!=NULL){
    if (strcmp (argv[2], "DEBUG")==0) {
      DEBUG = true;
      if (!load_file(argv[1])) return 1;
    }
  } else {
    cout<< "Use interpreter <file_to_execute> [ <DEBUG> ]";
    return 1;
  }
  //show();
  execution_loop();
  return 0;
}
