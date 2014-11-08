//Code by    : Jesse Kazemir
//Student ID : 301227704
//SFU email  : jkazemir@sfu.ca
//Last edited: Oct. 29, 2014


#ifndef PARSERCLASSES_H_
#define PARSERCLASSES_H_

//Use only the string library DO NOT add any other libraries
#include <string>
#include "lexemesTypes.h"

using namespace std;

//Declare your variables for storing delimiters here:

//typedef for custom data type defined in header file "lexemesTypes.h"
typedef ensc251::LexemeType TokenClass;

//Token class for a doubly-linked list of string tokens
class Token 
{
private:
	Token *next; //Next pointer for doubly linked list
	Token *prev; //Previous pointer for doubly linked list
	string stringRep; //Token value
	TokenClass stringType; //Token type (to which class this token belongs to, for e.g. identifier, keywords...etc)
	 
	//Allow TokenList class to access Token member variables marked private
	friend class TokenList;

public:
	//Default Constructor, pointers initialized to NULL
	Token() : next(NULL), prev(NULL) { }
		
	//Constructor with string initialization, pointers initialized to NULL
	Token(const string &stringRep) : next(NULL), prev(NULL), stringRep(stringRep) { }

	//Returns the Token's *next member 
	Token* getNext ( ) const 
	{
		return next;
	}


	//Sets the Token's *next member
	void setNext (Token* next )
	{
		this->next = next;
		return;
	}


	//Returns the Token's *prev member 
	Token* getPrev ( ) const
	{
		return prev;
	}


	//Sets the Token's *prev member
	void setPrev (Token* prev )
	{
		this->prev = prev;
		return;
	}


	//Returns a reference to the Token's stringRep member variable
	const string& getStringRep ( ) const 
	{
		return stringRep;
	}

	//Sets the token's stringRep variable
	void setStringRep (const string& stringRep )
	{
		this->stringRep = stringRep;
	}


	//**************************************************//
	//					DEFINE THESE					//
	//**************************************************//

	//Returns a reference to the Token's stringType member variable
	const TokenClass getStringType ( ) const
	{
		return stringType;
	}

	//Sets the token's stringType variable
	void setStringType (const TokenClass& stringType ) 
	{
		this->stringType = stringType;
	}

};

//A doubly-linked list class consisting of Token elements
class TokenList {
private:
	Token *head; //Points to the head of the token list (doubly linked)
	Token *tail; //Points to the tail of the function list (doubly linked)
	
public:
	//Default Constructor, Empty list with pointers initialized to NULL
	TokenList() : head(NULL), tail(NULL) { }
	
	//Returns a pointer to the head of the list
	Token* getFirst() const
	{
		return head;
	}


	//Returns a pointer to the tail of the list
	Token* getLast() const
	{
		return tail;
	}


	//Creates a new token for the string input, str
	//Appends this new token to the TokenList
	//On return from the function, it will be the last token in the list
	void append(const string &str); //example comment

	//Appends the token to the TokenList if not null
	//On return from the function, it will be the last token in the list
	void append(Token *token);

    //Removes the token from the linked list if it is not null
	//Deletes the token
	//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
	void deleteToken(Token *token);

	//Input: a pointer to a token
	//Output: it won't return anything, but within function, it should set the token class (i.e. token->stringType)
	//Note: one can invoke this function before adding token to the token list
	void setTokenClass(Token *token);
};

//A class for tokenizing a string of C++  code into tokens
class Tokenizer {
private:
	/*State tracking variables for processing a single string*/
	bool processingInlineComment; //True if processing an In-line comment //
	bool processingBlockComment;  //True if processing a Block Comment /* */
	bool processingIncludeStatement; //True if processing an include statement <> ""
	bool complete; //True if finished processing the current string
	
	size_t offset; //Current position in string
	size_t tokenLength; //Current token length
	string *str; //A pointer to the current string being processed

	//Include any helper functions here
	//e.g. trimming whitespace, comment processing

	//Removes any spaces or tabs at the beginning of str
	//If white space is removed, offset will change
	void removeWhite()
	{
		//if (str->length() <= offset)
		//{
		//	//Character is at the end of the string, no white space to remove
		//	return;
		//}
		while (str->length() > offset && (str->at(offset) == ' ' || str->at(offset) == '	'))
		{
			//Loops while there is a character to test, and it is a space or a tab
			offset++;
		}
		return;
	}

	//Counts the number of characters until the next white space or operator, saves it in tokenLength
	//Offset will be changed to point to the next character after the characters counted
	//Precondition: the token must start with a letter or number, and therefore must be a keyword, 
	// variable/function name, data type, or literal.
	void proccessGeneral()
	{
		char firstChar = str->at(offset); //First character in the token being checked, is not changed by the while loop
		char c1 = str->at(offset); // c1 is the current character being checked, is iterated through by the while loop

		tokenLength = 0;
		string nonPermisChars = "	 !#%&|,:;*/=(){}[]\"'<>?~^"; //Includes TAB and SPACE
						// Doesn't include '.' or '-' or '+' because these are sometimes
						// in the middle of tokens ( 6.7 , 5e-4 , 5e+4)


		//Check for white space or non-permissible characters (operators)
		while (nonPermisChars.find(c1) == -1) //Returns false if c1 is in the string nonPermisChars
		{
			if (c1 == '.' && !isdigit(firstChar) && firstChar != '.') 
			{
				//Detected '.' and the first character in the token was not a number or a decimal.
				//This token must be a variable/function name, and '.' must be a separate token.
				//tokenLength and offset are already as desired
				break;
			} // Else '.' was not detected, OR '.' was detected as the first character OR
			  // the first character in the token was a number. This token must be a 
			  // decimal literal, the '.' is included in the token.
			else if  ( (c1 == '-' || c1 == '+') &&								  //If the current character is - AND
					!( ( str->at(offset-1) == 'e' || str->at(offset-1) == 'E') && //one of the following fails, the -
						isdigit(str->at(offset+1) ) &&							  //is a single token.
						(isdigit(firstChar) || firstChar == '.') ) )
			{
				// c1 is '-' or '+', but it is not of the form 5e-2 because (1) the previous character is not e
				//														 or (2) the next character is not a number
				//														 or (3) the first character of the token was not a number
				
				break; // tokenLength and offset are already as desired
			}

			offset++;
			tokenLength++;

			if (str->length() <= offset) // Make sure offset is not at the end of str
			{
				//end of line, therefore end of token.
				break; //tokenLength and offset are already as desired
			}

			c1 = str->at(offset);
		}

		//Variables are already changed, nothing needs to be returned
		return;
	}

	//Counts the number of characters until either the block comment ends (*/) or until 
	//a new line is needed. processingBlockComment may or may not be changed
	//Precondition: offset cannot be greater than or equal to the number of characters in str
	void processBlockComment()
	{
		if (str->length() <= offset+1)
		{
			//At this point offset points the last character on the line
			tokenLength = 1;
			offset++; //Point offset to after the last character to detect end of line next iteration
			return;
		}
		
		char c1 = str->at(offset);	  // c1 is the current character being checked
		char c2 = str->at(offset+1);  // c2 is the second character being checked (used only for detecting "*/")

		if (c1 == '*' && c2 == '/')
		{
			//This means there is no body inside the block comment, the next token is */
			processingBlockComment = false;
			tokenLength = 2;
			offset += 2;
			return;
		}

		tokenLength = 0;

		while (!(c1 == '*' && c2 == '/'))
		{
			//Increment offset and tokenLength
			offset++;
			tokenLength++;
			
			if (str->length() <= offset+1)
			{
				break; //Detected end of string
			}

			//Increment characters for next pass
			c1 = str->at(offset);
			c2 = str->at(offset+1);
		}

		//FIND WHICH CONDITION WAS DETECTED
		
		if (str->length() <= offset+1 && c1 == '*' && c2 == '/') 
		{
			//Both end of string AND */ were detected
			processingBlockComment = false; //Keep offset ponting to the * and leave tokenLength as it is
			return;
		}
		else if (str->length() <= offset+1)
		{
			//ONLY detected end of line
			tokenLength++; // Set tokenLength to include c1
			offset++; // Set offset to after c1 to detect end of line for next token
			return; // processingBlockComment stays true, there has not yet been a "*/"
		}

		else if (c1 == '*' && c2 == '/')
		{
			// ONLY */ was detected
			processingBlockComment = false; // At this point offset is already pointing at the '*' in "*/" for the next token
			return; // Offset and tokenLength are already as they need to be
		}
	}

	//Counts the number of characters until the end of the line
	//When completed, processingInlineComment is false
	void processInlineComment()
	{
		while (str->length() > offset)
		{
			//Iterate offset through the str until end of string is reached
			tokenLength++; //Keep track of token length
			offset++;
		}
		processingInlineComment = false;
		return; // tokenLength is already as desired
	}

	//Sets the length of the next token in a preprocessor statement, stores the value in tokenLength
	//processingIncludeStatement may or may not be changed
	//Precondition: there is at least one character after #
	void processIncludeStatement()
	{		
		tokenLength = 0; //First character is known to exist

		//Check for <filename.extension>
		if (str->at(offset) == '<')
		{
			while (str->at(offset) != '>')
			{
				tokenLength++;
				offset++;
			}
			tokenLength++;
			offset++;
			return;
		}

		char c1 = str->at(offset);

		while (str->length() > offset)
		{
			c1 = str->at(offset);

			if (c1 == ' ' || c1 == '	' || c1 == '<')
			{
				// End of token - we don't have to check for other non-permissable characters because
				// in a syntactically correct include statement white space and <> are the only 
				// characters that separates tokens
				return; // tokenLength and offset are already as desired
			}
		
			tokenLength++;
			offset++;
		}
		// end of line, include statement is finished

		tokenLength++;
		offset++;

		processingIncludeStatement = false;
		return; //tokenLength is already as desired
	}

	//Sets the length of a string token, stores the value in tokenLength
	//Precondition: current character pointed to by offset must be "
	//Modifies tokenLength and offset
	void processString()
	{
		tokenLength = 2; //The opening and closing quotation marks are guaranteed (assuming correct syntax)
		offset++; //Point offset at the first character in the string
		char c1 = str->at(offset);

		while (c1 != '"')
		{
			if (str->at(offset) == '\\')
			{
				//Check for escape character - '\''
				offset += 2;
				tokenLength += 2;
			}
			else
			{
				tokenLength++;
				offset++;
			}
			c1 = str->at(offset);
		}
		offset++; //Point offset to the start of the next token
		return;
	}

	//Sets the length of a char token, stores the value in tokenLength
	//Precondition: current character pointed to by offset must be '
	//Modifies tokenLength and offset
	void processChar()
	{
		tokenLength = 2; //The opening and closing apostrophes are guarenteed (assuming correct syntax)
		offset++; //Point offset at the first character in the string
		char c1 = str->at(offset);

		while (c1 != '\'')
		{
			if (str->at(offset) == '\\')
			{
				//Check for escape character - '\''
				offset += 2;
				tokenLength += 2;
			}
			else
			{
				tokenLength++;
				offset++;
			}
			c1 = str->at(offset);
		}
		offset++; //Point offset to the start of the next token
		return;
	}

	//Computes a new tokenLength for the next token
	//Modifies: size_t tokenLength, and bool complete
	//(Optionally): may modify offset
	//Does NOT modify any other member variable of Tokenizer
	void prepareNextToken();
	
public:
	//Default Constructor- YOU need to add the member variable initializers.
	Tokenizer() : processingInlineComment(false), processingBlockComment(false),
				  processingIncludeStatement(false), complete(true),offset(0),
				  tokenLength(0),str(NULL) {}

	//Sets the current string to be tokenized
	//Resets all Tokenizer state variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	void setString(string *str);

	//Returns true if all possible tokens have been extracted from the current string (string *str)
	bool isComplete() const
	{
		return complete;
	}

	//Returns the next token. Hint: consider the substr function
	//Updates the tokenizer state
	//Updates offset, resets tokenLength, updates processingABC member variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	string getNextToken();
};




#endif /* PARSERCLASSES_H_ */
