//Code by    : Jesse Kazemir
//Student ID : 301227704
//SFU email  : jkazemir@sfu.ca
//Last edited: Oct. 29, 2014


//Use only the following libraries:
#include "parserClasses.h"
#include <string>

//Complete the implementation of the following member functions:

//****TokenList class function definitions******

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) 
{
	//Create new node
	Token *newToken = new Token;
	newToken->stringRep = str;

	//Append token
	append(newToken);

	return;
}

//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token)
{
	//Return if token is null
	if (token == NULL)
	{
		return;
	}

	//Reassign Pointers
	token->prev = tail;
	token->next = NULL;
	if (tail != NULL)
	{
		//If the list has at least one element
		tail->next = token;
	}
	else
	{
		//In this case TokenList is empty, so the last element is also the first
		head = token;
	}
	tail = token;
	
	setTokenClass(token);
	return;
}


//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{
	//Return if null
	if (token == NULL)
	{
		return;
	}

	//Reassign pointers
	Token *prevToken = token->prev;
	Token *nextToken = token->next;
	if (prevToken != NULL)
	{
		prevToken->next = nextToken;
	}
	if (nextToken != NULL)
	{
		nextToken->prev = prevToken;
	}

	//Reassign Head/Tail
	if (token == head)
	{
		head = nextToken;
	}
	if (token == tail)
	{
		tail = prevToken;
	}
	
	//Delete token
	delete token;

	return;
}


	//**************************************************//
	//					DEFINE THIS					//
	//**************************************************//


//Input: a pointer to a token
//Output: it won't return anything, but within function, it should set the token class (i.e. token->stringType)
//Note: one can invoke this function before adding token to the token list
void TokenList::setTokenClass(Token *token) 
{ 
	string temp = token->getStringRep();
	char t = temp[0];

	
	if(ensc251::isKeyword(temp))
	{
		token->setStringType(ensc251::T_Keyword);
	}

	else if(ensc251::isBooleanValue(temp))
	{
		token->setStringType(ensc251::T_Boolean);
	}

	else if(ensc251::isIntegerLiteral(temp))
	{
		token->setStringType(ensc251::T_IntegerLiteral);
	}

	else if(ensc251::isFloatLiteral(temp))
	{
		token->setStringType(ensc251::T_FloatLiteral);
	}

	else if(ensc251::isStringLiteral(temp))
	{
		token->setStringType(ensc251::T_StringLiteral);
	}

	else if(ensc251::isIdentifier(temp))
	{
		token->setStringType(ensc251::T_Identifier);
	}
	else if(ensc251::isOperator(temp))
	{
		token->setStringType(ensc251::T_Operator);
	}
	else if(ensc251::isPunctuator(t))
	{
		token->setStringType(ensc251::T_Punctuator);
	}
	else
	{
		token->setStringType(ensc251::T_Unknown);
	}
}

//****Tokenizer class function definitions******

//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{
	  //************************//
	 //       Initialize       //
	//************************//
	
	//Remove any white space (tab or space) as long as the current token is not a comment
	if (!processingBlockComment && !processingInlineComment)
	{
		removeWhite();
	}


	//Check for end of line
	if (str->length() <= offset)
	{
		complete = true; // Line is complete
		tokenLength = 0; // No more tokens on this line
		offset = 0; //Reset offset
		return; // tokenLength and offset are already as desired
	}


	char c1 = str->at(offset); //This is the character currently being compared, gets iterated through the string



	  //******************************//
	 //    Check State Conditions    //
	//******************************//

	if (processingIncludeStatement)
	{
		processIncludeStatement(); // Updates tokenlength and offset
		return;
	}
	else if (processingInlineComment)
	{
		processInlineComment(); // Updates tokenlength and offset
		return;
	}
	else if (processingBlockComment)
	{
		processBlockComment(); // Updates tokenlength and offset
		return;
	}


	  //****************************************//
	 //    Check Triple Character Operators    //
	//****************************************//

	string tripleOperators = ">>=@<<=";
	// This string is a list of all triple-character operators, separated by @ symbols. @ symbols 
	// are not used in C++ so they should be a safe separator. This string can now be searched for
	// a match with any three characters. If the search returns successfully, the three characters 
	// are a triple-character operator. 

	char c2,c3 = '@'; // If c2 or c3 stays as '@' then it is clear they were never set to a character
				   // in str. This way the code will not test c2 or c3 unless it is not '@'

	if (str->length() > offset+2) //Make sure there is a second and third character to check (not end of string)
	{
		//Create c2 and c3 in order to compare triple-character operators
		c2 = str->at(offset+1);
		c3 = str->at(offset+2);
		
		//Create a tree-character string to search tripleOperators with
		string tripleChar = "";
		tripleChar = tripleChar + c1 + c2 + c3;
	
		//Check for triple operators
		if (tripleOperators.find(tripleChar) != -1)
		{
			//The current token is a triple operator
			tokenLength = 3;
			offset += 3; 
			return;
		}
	}


	  //****************************************//
	 //    Check Double Character Operators    //
	//****************************************//

	string doubleOperators = "==@!=@<=@>=@+=@-=@*=@/=@%=@&=@|=@^=@&&@||@*/@++@--@<<@>>@::@?:@->";
	// This string is a list of all double-character operators (other than // and /*), separated by 
	// @ symbols. @ symbols are not used in C++ so they should be a safe separator. This string 
	// can now be searched for a match with any two characters. If the search returns successfully, 
	// the two characters are a double-character operator. // and /* are not incuded, as they have
	// to update the state.


	if (str->length() > offset+1) //Make sure there is a second character to check (not end of string)
	{
		//Create c2 in order to compare double-character operators
		c2 = str->at(offset+1);
		
		//Create a two-character string to search doubleOperators with
		string doubleChar = "";
		doubleChar = doubleChar + c1 + c2;
	
		//Check for double operators
		if (doubleOperators.find(doubleChar) != -1)
		{
			//The current token is a double operator
			tokenLength = 2;
			offset += 2; 
			return;
		}
		//Check for comments (// or /*)
		else if (c1 == '/')
		{
			if (c2 == '/')
			{
				processingInlineComment = true;
				tokenLength = 2;
				offset += 2; 
				return;
			}
			else if (c2 == '*')
			{
				processingBlockComment = true;
				tokenLength = 2;
				offset += 2; 
				return;
			}
		}
	}


	  //****************************************//
	 //    Check Single Character Operators    //
	//****************************************//

	string singleOperators = "+-*/%()[]{}<>=~^!:;,&|?"; 
	// This string is a list of all single-character operators (other than # . ' or "). 
	// This string can now be searched for a match with any single character. 
	// If the search returns successfully, the character is a single operator. 
	// # . and " are not included, as they are special cases. 
	// Note: '-' is included, as this case is only checking the BEGINNING of a token,
	// and when '-' is at the beginning of a token it is ALWAYS a single-character token.


	if (singleOperators.find(c1) != -1)
	{
		//c1 is a single operator
		tokenLength = 1;
		offset++;
		return;
	}
	else if (c1 == '#')
	{
		processingIncludeStatement = true;
		tokenLength = 1;
		offset++;
		return;
	}
	else if (c1 == '.')
	{
		if (c2 != '@' && !isdigit(c2))
		{
			// The character after the '.' is not a number, therefore 
			// the '.' is a dot operator and is a single character token
			tokenLength = 1;
			offset++;
			return;
		}
		// Else the character after the '.' is a number, therefore
		// the '.' is a decimal and processed later
	}
	else if (c1 == '"')
	{
		processString(); // Updates tokenlength and offset
		return;
	}
	else if (c1 == '\'')
	{
		processChar(); // Updates tokenlength and offset
		return;
	}

	

	  //************************//
	 //      General Case      //
	//************************//

	// For tokens such as:
	//			int				(type)
	//			while			(keyword)
	//			myint			(variable name)
	//			myfunction		(function name)
	//			72				(literal)
	//			6.2				(literal with decimal)
	//			5e-5			(literal in scientific notation)

	proccessGeneral(); // Sets offset and tokenLength

	return;
}


//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *str)
{
	//Set new string
	this->str = str;
	//Reset state variables
	processingInlineComment = false;
	processingIncludeStatement = false;
	complete = false;
	offset = 0;
	tokenLength = 0;
	//Note: processingBlockComment is purposefully unchanged
	
	prepareNextToken();		//Sets tokenLength
	return;
}


//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken()
{
	string nextToken;
	nextToken = str->substr(offset-tokenLength, tokenLength);	//Gets token based on known offset and tokenLength

	tokenLength = 0; // Reset tokenLength
	
	prepareNextToken();
	return nextToken;
}
