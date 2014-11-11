/*
 * lexemesTypes.cpp
 *
 * Created on: Oct 27, 2014
 *
 */

#include "lexemesTypes.h"

namespace ensc251{

  //***Complete the implementation of the following functions: ***

  //This function returns TRUE if the input is identifier else FALSE
  bool isIdentifier(const string &lexeme) 
  {
	  bool check = false;
	  if (lexeme.at(0) == '_' || lexeme.at(0)>='A' && lexeme.at(0)<='Z' || lexeme.at(0)>='a' && lexeme.at(0)<='z')
	  {
		  check = true;
	  }

	  return check;	  
  }

  //This function returns TRUE if the input is one of the operators defined in the variable
  //"tableOfOperators" in "lexemesTypes.h" else it returns FALSE
  bool isOperator(const string &lexeme) 
  { 
	  bool check = false;
	  int size = sizeof(tableOfOperators)/sizeof(*tableOfOperators);
	  for (int i = 0; i <= size; i++)
	  {
		  if (lexeme == tableOfOperators[i])
		  {
			  check = true;
			  break;
		  }
	  }
	 return check;
  }

  //This function returns TRUE if the input is one of the punctuators defined in the variable
  //"tableOfPunctuators" in "lexemesTypes.h" else it returns FALSE
  bool isPunctuator(char lexeme)
  { 
	  bool check = false;
	  int size = sizeof(tableOfPunctuators)/sizeof(*tableOfPunctuators);
	  for (int i = 0; i < size; i++)
	  {
		  if (lexeme == tableOfPunctuators[i])
		  {
			  check = true;
			  break;
		  }
	  }
	 return check;
  }

  //This function returns TRUE if the input is one of the keywords defined in the variable
  //"tableOfKeywords" in "lexemesTypes.h" else it returns FALSE
  bool isKeyword(const string &lexeme) 
 { 
	bool check = false;
	int size =sizeof(tableOfKeywords)/sizeof(*tableOfKeywords);
	for (int i = 0; i < size; i++)
	{
		if (lexeme == tableOfKeywords[i])
		{
		  check = true;
		  break;
		}
	}
	return check;
  }
  
  //This function returns TRUE if the input is one of the boolean defined in the variable
  //"booleanValues" in "lexemesTypes.h" else it returns FALSE
  bool isBooleanValue(const string &lexeme) 
  { 
	  bool check = false;
	  int size = sizeof(booleanValues)/sizeof(*booleanValues);
	 
	  for (int i = 0; i < size; i++)
	  {
		  if (lexeme == booleanValues[i])
		  {
			  check = true;
			  break;
		  }
	  }
	 return check;
  }

  //This function returns TRUE if the input is integer literal else FALSE
  bool isIntegerLiteral(const string &lexeme) 
  {
	
		bool check = false;

		//rudimentary check for binary or hex numbers
		if(lexeme.length() > 1)
		{
			if(lexeme.at(0) == '0' && lexeme.at(1) =='x' || lexeme.at(1) =='X' || lexeme.at(1) =='b' || lexeme.at(1) =='B')
			{
				check = true; 
				return check;
			}
		}
			
		for(int i = 0; i < lexeme.length(); i++)
		{				
			if (lexeme.at(i) >='0' && lexeme.at(i)<='9' || lexeme.at(i)=='u' || lexeme.at(i)=='U' 
						|| lexeme.at(i)=='l' || lexeme.at(i)=='L')
			{
				check = true;
		    }
			else 
			{
				check = false;
				break;
			}
			
		}
	  return check;
	}

  //This function returns TRUE if the input is float literal else FALSE
  bool isFloatLiteral(const string &lexeme) 
  { 
	bool check = false;
	if (lexeme.at(0) >= '0' && lexeme.at(0) <='9')
	{
		check = true;
	}
	if (lexeme.at(0) == '.' && lexeme.at(1) >= '0' && lexeme.at(1) <='9' )
	{
		check = true;
	}

	 return check;
  }

  //This function returns TRUE if the input is string literal else FALSE
  bool isStringLiteral(const string &lexeme) 
  {
	  bool check = false;
	  if(lexeme.at(0) == '"'|| lexeme.at(0)=='<' && lexeme.at(lexeme.size() - 1) == '>')
	  {
		  check = true;
	  }
	  return check;
  }

}
