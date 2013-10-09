// PolyCal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define TYPESTRING	"="
//#define ASSINGSTRING	":="
//#define DEGREESTRING	"^"
//#define ENDSTRING		";"

#define RETURN_ERROR -1

enum EOperator
{
	TYPE,
	ASSING,
	DEGREE,
	END,
	EXIT,
};

char * GetOperator(int type)
{	
	static char * szOperator[] = { "=",":=","^",";","bye" };

	return szOperator[type];
}

struct POLY
{
	string id;
	TERMS terms; 
};

vector<POLY> POLYS;

enum EError
{
	NOTDEFINE,
	INVALIDID,
	XINVALIDID,
	TOOMANYNUMBERS,
};

void ErrorMsg(int type)
{
	static char * msg[] = {
		"not defined",
		"invalid id!! use correct id",
		"x is reserved for term",
		"numbers cannot greater than 10 digit"
	};
	printf("=> Error : %s \n",msg[type]);
}

bool IsDigit(char c)
{
	if(c >= '0' && c <= '9')
		return true;
	else
		return false;
}

bool IsLetter(char c)
{
	if(c >= 'a' && c <= 'z')
		return true;
	if(c >= 'A' && c <=  'Z')
		return true;

	return false;
}

bool IsSpace(char c)
{
	return c == ' ';
}


bool BinaryTerm(TERM a,TERM b)
{
	if(a.degree > b.degree)
		return true;
	else
		return false;
}

bool CheckIdValid(string str)
{	
	for(int i = 0; i < str.size(); i++)
	{
		if(IsLetter(str[i]) == false && IsDigit(str[i]) == false)
		{
			ErrorMsg(INVALIDID);
			return false;
		}
	}

	if(IsLetter(str[0]) == false)
	{
		ErrorMsg(INVALIDID);
		return false;
	}
	
	if(str == "x")
	{
		ErrorMsg(XINVALIDID);
		return false;
	}

	return true;
}

int GetNumber(const char * str,int * pReadNum,int * pNumber)
{
	string operand;
	int i = 0;
	if(IsDigit(str[i]))
	{
		while(IsDigit(str[i]))
		{
			operand.append(1,str[i++]);
			if(i > 10)
			{
				ErrorMsg(TOOMANYNUMBERS);
				return RETURN_ERROR;
			}
		}

		*pReadNum = i;
		*pNumber = atoi(operand.c_str());
		
		return true;
	}

	*pNumber = 1;
	*pReadNum = 0;
	return false;
}

enum EState
{
	S_EXIT,
	S_NORMAL,
	//S_OPERAND,
	//S_OPERATOR,
};

int main(int argc, char* argv[])
{
	printf("Welcome!! I'm a ploynomail calculator\n");
	
	//string strCommand;		//명령줄 buffer
/*
	if(argc > 1)	//파일로 읽기.
	{
		FILE * fp = fopen(argv[1],"rt");
		char c;
		while( (c = getc(fp)) != -1 )
			str.append(1,c);
	}
	else			//명령줄로 읽기.
	*/
	
	int state = S_NORMAL;

	while(state != S_EXIT)
	{
		printf("> ");
		char buf[255];
		gets(buf);
		string str(buf);
		//입력 받은 모든 공백을 제거한다.
		str.erase(remove_if(str.begin(), str.end() , IsSpace) , str.end());
		
		int i = 0;
		
		//exit 체크
		int nFind;
		nFind = str.find(GetOperator(EXIT));
		if (nFind != -1)
		{
			state = S_EXIT;
			break;
		}
		
		//assing 체크
		nFind = str.find(GetOperator(ASSING));
		
		POLY poly;

		if (nFind != -1)	//type 문
		{			
			poly.id.append(str.begin(),nFind);
			if(CheckIdValid(poly.id) == false)
				continue;
			i+= nFind+sizeof(GetOperator(ASSING));
		}
		//이름이 없을 경우 id.empty() == true


		//state = S_OPERATOR;
		TERMS& termVector = poly.terms;

		while( i < str.size() )
		{
			TERM term;
			//실제 다항식 계산.
			string strOperand,strOperator;

			int readNum,value;
			int result = GetNumber(str.c_str()+i,&readNum,&value);
			if(result == RETURN_ERROR)
				break;
			else if(result == true)
			{
				while(IsDigit(str[i]))
					strOperand.append(1,str[i++]);
				term.coefficient = atoi(strOperand.c_str());
			}
			else
				term.coefficient = 1;

			strOperand.erase();
			if(IsLetter(str[i]))
			{
				
			}

			termVector.push_back(term);
			
		}
		
		sort(termVector.begin(),termVector.end(),BinaryTerm);


	}

	printf("Press any key to continue");
	char t;
	scanf("%c",&t);
	return 0;
}

