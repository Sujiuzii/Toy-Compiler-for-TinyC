#define _CRT_SECURE_NO_WARNINGS
#include "convert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char TokenText[256];

Token token;
FILE* sFile;

TokenList list ;

int isLetter(char ch)
{
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		return 1;
	}
	return 0;
}

int isDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return 1;
	}
	return 0;
}

int cmpstring(char a[], char b[])
{
	int la = strlen(a);
	int lb = strlen(b);
	if (la != lb)
		return 0;
	for (size_t i = 0; i <= la; i++)
	{
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}

dfastat initToken(char ch)
{
	
	if (strlen(TokenText) > 0)//如果当前进入DFA的字符串不为空
	{
		
		TokenList tokenNode = (TokenList)malloc(sizeof(Token));
		tokenNode->type = token.type;
		strcpy(tokenNode->text, TokenText);
		tokenNode->next = NULL;
		tokenNode->parent = NULL;
		
		if (list == NULL)
		{
			list = tokenNode;
		}
		else
		{
			TokenList p = list;
			while (p->next!=NULL)
			{
				p = p->next;
			}
			p->next = tokenNode;
			tokenNode->parent = p;
		}
		memset(TokenText, '\0',256);
		memset(token.text, '\0', 256);
	}
	dfastat newstat = InitialStat;
	if (isLetter(ch))
	{
		newstat = LetterStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Identifier;
	}
	else if (isDigit(ch))
	{
		newstat = DigitStat;
		TokenText[strlen(TokenText)] = ch;
		token.type = IntLiteral;
	}
	else{
		switch (ch)
		{
		case '+':
			newstat = PlusStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = Plus;
			break;
		case '-':
			newstat = MinusStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = Minus;
			break;
		case '*':
			newstat = TimesStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = Times;
			break;
		case '/':
			newstat = DividedbyStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = Dividedby;
			break;
		case '>':
			newstat = GTStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = GT;
			break;
		case '<':
			newstat = LTStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = LT;
			break;
		case ';':
			newstat = SemiColonStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = SemiColon;
			break;
		case '(':
			newstat = LeftParenStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = LeftParen;
			break;
		case ')':
			newstat = RightParenStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = RightParen;
			break;
		case '=':
			newstat = equalStat;
			break;
		case '{':
			newstat = LeftBraceStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = LeftBrace;
			break;
		case '}':
			newstat = RightBraceStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = RightBrace;
			break;
		case ',':
			newstat = DotStat;
			TokenText[strlen(TokenText)] = ch;
			token.type = Dot;
			break;
		case '&':
			newstat = andStat;
			break;
		case '|':
			newstat = orStat;
			break;
		default:
			break;
		}
	}
	if(newstat == InitialStat)
	{
		newstat = Others;
	}
	
	return newstat;
}

void TokenReader()
{
	memset(TokenText, '\0', 256);
	char c;
	dfastat stat = InitialStat;
    //从源文件中开始读取内容，并得到相关Token
	while (!feof(sFile))
	{
		c = fgetc(sFile);
		
		switch (stat)
		{

		case InitialStat:
			stat = initToken(c);
			break;
		case LetterStat:
			if (isDigit(c) || isLetter(c))
			{
				stat = LetterStat;
				TokenText[strlen(TokenText) ] = c;
			}
			else
			{
				if (cmpstring(TokenText, "main"))
				{
					token.type = Main;
				}
				else if(cmpstring(TokenText, "show")){
					token.type = Show;
				}
				else if(cmpstring(TokenText, "if")){
					token.type = If;
				}
				else if(cmpstring(TokenText, "else")){
					token.type = Else;
				}
				else if(cmpstring(TokenText, "int")){
					token.type = Int;
				}
				else if(cmpstring(TokenText, "char")){
					token.type = Char;
				}
				else if(cmpstring(TokenText, "while")){
					token.type = While;
				}
				else if(cmpstring(TokenText, "continue")){
					token.type = Continue;
				}
				else if(cmpstring(TokenText, "break")){
					token.type = Break;
				}
				else
				{
					token.type = Identifier;
				}
				stat = initToken(c);
			}
			break;

		case DigitStat:
			if(isDigit(c)){
				stat = DigitStat;
				TokenText[strlen(TokenText) ] = c;
			}
			else if(isLetter(c)){
				printf("Error: Identifier should start with letter");
			}
			else{
				token.type = IntLiteral;
				stat = initToken(c);
			}
			break;
		case orStat:
			if(c == '|'){
				stat = OrStat;
				TokenText[0] = '|';
				TokenText[1] = '|';
				token.type = Or;
			}
			else{
				printf("error token '|'");
			}
			break;
		case andStat:
			if(c == '&'){
				stat = OrStat;
				TokenText[0] = '&';
				TokenText[1] = '&';
				token.type = And;
			}
			else{
				printf("error token '&'");
			}
			break;
		case equalStat:
			if(c == '='){
				stat = EqualStat;
				TokenText[0] = '=';
				TokenText[1] = '=';
				token.type = Equal;
			}
			else{
				stat = AssignmentStat;
				TokenText[0] = '=';
				token.type = Assignment;
				stat = initToken(c);
			}
			break;
		case Others:
			stat = initToken(c);
			break;
		default:
			// for other valid character
			stat = initToken(c);
			break;
		}
	}
	if (strlen(TokenText) > 0)
	{
		initToken(c);
	}
}
