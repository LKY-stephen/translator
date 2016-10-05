#pragma once
#include "compiler.h"




int compiler::ID_class(){
	int n = express.length();
	ID_struct *idtable = table;
	char ch;
	int i = 0;
	int j = 0;
	int k = 0;
	int error = SUCCESS;
	int error_id = 0;
	while (i < n && j < IDNUM){
		ch = express[i];
		switch (ch){
		case '$':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = DOLLAR;
			j++;
			idtable += 1;
			break; }
		case '^':{
			if (k){
				if (idtable->type == SUBSCRIPT){
					idtable->length = 2;
					idtable->value[k] = ch;
					idtable->type = ALLSCRIPT;
					j++;
					idtable += 1;
					k = 0;
					break;
				}
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = SUPERSCRIPT;
			j++;
			idtable += 1;
			break; }
		case '_':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = SUBSCRIPT;
			k++;
			break; }
		case '(':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = LEFTBRACKET;
			j++;
			idtable += 1;
			break; }
		case ')':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = RIGHTBRACKET;
			j++;
			idtable += 1;
			break; }
		case '{':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = LEFTCURLY;
			j++;
			idtable += 1;
			break; }
		case '}':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			idtable->type = RIGHTCURLY;
			j++;
			idtable += 1;
			break; }
		case ' ':{
			if (k){
				k = 0;
				idtable += 1;
			}
			j++;
			break; }
		case '\\':{
			if (k){
				idtable += 1;
				j++;
				k = 0;
			}
			idtable->length = 1;
			idtable->value[0] = ch;
			k++;
			break; }
		default:
			if (k == 0){
				if ('0' <= ch &&ch <= '9'){
					idtable->length++;
					idtable->value[k] = ch;
					idtable->type = NUMBER;
					k++;
				}
				else if (('a' <= ch) && (ch <= 'z') || ('A' <= ch) && (ch <= 'Z')){
					idtable->length++;
					idtable->value[k] = ch;
					idtable->type = ID;
					k++;
				}
				else{
					return UNKNOWN_CHARACTER;
				}
			}
			else{
				if ('0' <= ch &&ch <= '9'){
					if (idtable->type == NUMBER || idtable->type == ID){
						idtable->length++;
						idtable->value[k] = ch;
						k++;
					}
					else if (idtable->type == SUBSCRIPT){
						idtable += 1;
						j++;
						idtable->length = 1;
						idtable->value[0] = ch;
						idtable->type = NUMBER;
						k = 1;
					}
					else{
						return UNKNOWN_CHARACTER;
					}
				}
				else if (('a' <= ch) && (ch <= 'z') || ('A' <= ch) && (ch <= 'Z')){
					if (idtable->type == ID){
						idtable->length++;
						idtable->value[k] = ch;
						k++;
					}
					else if (idtable->type == NUMBER){
						j++;
						idtable += 1;
						idtable->length = 1;
						k = 0;
						idtable->value[k] = ch;
						idtable->type = ID;
						k++;
					}
					else if (idtable->type == SUBSCRIPT){
						idtable += 1;
						j++;
						idtable->length = 1;
						idtable->value[0] = ch;
						idtable->type = ID;
						k = 1;
					}
					else if (idtable->value[0] == '\\'){
						if (k == 1){
							if (ch == 'i'){
								idtable->length += 1;
								idtable->value[k] = ch;
								idtable->type = INTEGRITY;
								k++;
							}
							else if (ch == 's'){
								idtable->length += 1;
								idtable->value[k] = ch;
								idtable->type = SIGMA;
								k++;
							}
							else if (ch == 'b'){
								idtable->length += 1;
								idtable->value[k] = ch;
								idtable->type = BLANK;
								k++;
							}
							else{
								idtable->length += 1;
								idtable->value[k] = '_';
								k++;
								error = FIXERROR;
							}
						}
						else if (k == 2){
							if (idtable->type == INTEGRITY || error == FIXERROR){
								if (ch == 'n'){
									idtable->length += 1;
									idtable->value[k] = ch;
									idtable->type = INTEGRITY;
									k++;
								}
								else if (idtable->type == INTEGRITY){
									idtable->length += 1;
									idtable->value[k] = '_';
									k++;
									error = FIXERROR;
								}
							}
							else if (idtable->type == SIGMA || error == FIXERROR){
								if (ch == 'u'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k++;
								}
								else if (idtable->type == SIGMA){
									idtable->length += 1;
									idtable->value[k] = '_';
									k++;
									error = FIXERROR;
								}
							}
							else if (idtable->type == BLANK || error == FIXERROR){
								if (ch == 'l'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k++;
								}
								else if (idtable->type == BLANK){
									idtable->length++;
									idtable->value[k] = '_';
									k++;
									error = FIXERROR;
								}
								else
									return UNKNOWN_CHARACTER;
							}
							else{
								return UNKNOWN_CHARACTER;
							}
						}
						else if (k == 3){
							if (idtable->type == SIGMA){
								if (ch == 'm'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k = 0;
									j++;
									idtable += 1;
								}
								else if (error == SUCCESS){
									idtable->length++;
									idtable->value[k] = '_';
									error = FIXERROR;
									j++;
									idtable += 1;
								}
								else
									return UNKNOWN_CHARACTER;
							}
							else if (idtable->type == INTEGRITY){
								if (ch == 't'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k = 0;
									j++;
									idtable += 1;
								}
								else if (error == SUCCESS){
									idtable->length++;
									idtable->value[k] = '_';
									error = FIXERROR;
									j++;
									idtable += 1;
								}
								else
									return UNKNOWNERROR;
							}
							else if (idtable->type == BLANK){
								if (ch == 'a'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k++;
								}
								else if (error == SUCCESS){
									idtable->length++;
									idtable->value[k] = '_';
									error = FIXERROR;
									k++;
								}
								else
									return UNKNOWN_CHARACTER;
							}
							else
								return UNKNOWNERROR;
						}
						else if (k == 4){
							if (idtable->type == BLANK){
								if (ch == 'n'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k++;
								}
								else if (error == SUCCESS){
									idtable->length++;
									idtable->value[k] = '_';
									error = FIXERROR;
									k++;
								}
								else
									return UNKNOWN_CHARACTER;
							}
							else
								return UNKNOWN_CHARACTER;
						}
						else if (k == 5){
							if (idtable->type == BLANK){
								if (ch == 'k'){
									idtable->length += 1;
									idtable->value[k] = ch;
									k = 0;
									j++;
									idtable += 1;
								}
								else if (error == SUCCESS){
									idtable->length++;
									idtable->value[k] = '_';
									error = FIXERROR;
									j++;
									idtable += 1;
								}
								else
									return UNKNOWN_CHARACTER;


							}
							else
								return UNKNOWN_CHARACTER;
						}
						break;}}
				else 
					return UNKNOWN_CHARACTER;}}
		i++;
	}
	if (i < n){
		return ARRAYOVER;
	}
	return error;
}

compiler::compiler(string filepath){
	for (int i = 0; i < IDNUM; i++){//init table
		table[i].length = 0;
		for (int j = 0; j < IDLEN; j++){
			table[i].value[j] = '\0';
		}
		table[i].type = EMPTY;
	}
	ifstream file(filepath);
	if (!file){//test file
		cout << "Unable to open file"<<filepath;
		exit(1); // terminate with error  
	}
	getline(file, express);
	//cout << expression<<endl;
};
int compiler::analysis(){
	int n = express.length();
	int *temperrorid=new int[n];
	for (int i = 0; i < n; i++) *temperrorid = n + 10;
	bool errorDetected = false;
	int error = ID_class();
	if (error!=SUCCESS){
		int j = 0;
		for (; table[j].value[0]!= '\0'; j++)
			for (int k = 0; table[j].value[k]!='\0';k++)
				cout << table[j].value[k];
		cout << "_ : that token is unrecongnized token" << endl;
		if (error != FIXERROR)
			return UNKNOWNERROR;
	}
	Parser Parsers( SENTENCE , MAP , TABLE);
	htmlbuilder::Init(OUTPUT);
	int token=0;
	string lexname="";
	int cnt = 0;
	int i = 0;
	int *temp=temperrorid;
	while (cnt < 2) {
		token = table[i].type;
		lexname= string(table[i].value);
		error = Parsers.match(token);
		if (error != SUCCESS ){
			errorDetected = true;
			if (error != FIXERROR){
				cout << "there are some error that we can not deal." << endl;
				*temp = i;
				break;
			}
			*temp = i;
			temp=temp+1;
		}
		if (token == ID || token == NUMBER || token == BLANK)
			Parsers.assignvalue(token != BLANK ? lexname : std::string(" "));
		cnt += token == DOLLAR;
		i++;
	}
	cout << "lexing table:" << endl;
	for (int i = 0; i < 20 && table[i].type != 1013; i++){
		cout << table[i].value << "   " << Parsers.getmarkname(table[i].type) << endl;
	}
	if (errorDetected){
		for (int j = 0;j<20; j++)
			if (j != *temperrorid && *temperrorid!=21)
			cout << table[j].value;
			else{
				cout << '_'<<table[i].value;
				temperrorid++;
			}
			cout << endl;
	}
	if(cnt>=2){
		Parsers.DeterminDepth();
		Parsers.Print();
		htmlbuilder::Finish();
		return SUCCESS;
	}
}