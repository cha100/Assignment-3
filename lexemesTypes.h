/*
 * lexemesTypes.h
 *
 * Created on: Oct 27, 2014
 *
 */

#ifndef LEXEMESTYPES_H_
#define LEXEMESTYPES_H_

//Use only the following libraries:
#include <string>
#include <cctype>

using namespace std;

namespace ensc251{

  //tables
  const string tableOfOperators[]={"+","-","/","*","=","<",">","++","--","<<",">>","!=","&","&&","|","||","[","]","=="};
  const char tableOfPunctuators[]={';',',','.','(',')','{','}'};
  const string tableOfKeywords[]={"auto","bool","break","case","char","const","continue","default","do","double","else",
		  "enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static",
		  "struct","switch","typedef","union","unsigned","void","volatile","while","asm","_ss","interrupt","_cs","cdecl",
		  "near","_ds","far","pascal","_es","huge","catch","class","delete","friend","inline","new","operator","overload",
		  "private","protected","public","template","this","throw","try","virtual"};
  const string booleanValues[]={"true","false"};
  const char integers[] ={'1','2','3','4','5','6','7','8','9', '0'};
  //assuming " is the only delimiter for string literals (*** is '? ***
  const char strLiteral[] = {'"'};

  //custom data type:
  enum LexemeType {
	  T_Identifier,
	  T_Operator,
	  T_Punctuator,
	  T_Keyword,
	  T_Boolean,
	  T_IntegerLiteral,
	  T_FloatLiteral,
	  T_StringLiteral,
	  T_Unknown                     //if token does not belong to above classes then place it in this category
  };
  
  //helper functions: for all of them
  //Input: a string value (char in case of isPunctuator() function)
  //Output: true/false depending on whether the input value is of type as denoted by the corresponding function name.
  //NOTE:- You need to DEFINE these functions in source file "lexemesTypes.cpp".
  bool isIdentifier(const string&);
  bool isOperator(const string&);
  bool isPunctuator(char);
  bool isKeyword(const string&);
  bool isBooleanValue(const string&);
  bool isIntegerLiteral(const string&);
  bool isFloatLiteral(const string&);
  bool isStringLiteral(const string&);
}

#endif /* LEXEMESTYPES_H_ */
