//
//  lispheader.h
//  Lisp Reader
//
//  Created by Matt Burdumy on 10/11/13.
//  Copyright 2013 Georgetown University. All rights reserved.
//

#ifndef Lisp_Reader_lispheader_h
#define Lisp_Reader_lispheader_h


#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <stdlib.h>

void readInput();
bool checkParands();
void clearComments();

using namespace std;

const char arrow[2] = {'-','>'};
const char miniArrow = '>';

string inputBuf;


/*****************************************************************
 *                     DECLARATIONS                              *
 *****************************************************************/
typedef int NUMBER;
typedef int NAME;
const int  NAMELENG = 20;      /* Maximum length of a name */
const int  MAXNAMES = 100;     /* Maximum number of different names */
const int  MAXINPUT = 500;     /* Maximum length of an input */
const char*   PROMPT = "-> ";
const char*   PROMPT2 = "> ";
const char  COMMENTCHAR = ';';
const int   TABCODE = 9;        /* in ASCII */

struct EXPLISTREC;
typedef EXPLISTREC* EXPLIST;
enum EXPTYPE {VALEXP,VAREXP,APEXP};
struct EXPREC
{
    EXPTYPE etype; //what type of expression
    NUMBER num;
    NAME varble;
    NAME optr;
    EXPLIST args;
};

typedef EXPREC* EXP;  //forward declaration
struct EXPLISTREC
{
    EXP head; // points to 'node' EXPREC
    EXPLIST tail; // points to next expression 
};


struct VALUELISTREC
{
    NUMBER  head;
    VALUELISTREC*  tail;
};

typedef VALUELISTREC* VALUELIST;  //forward declaration
struct NAMELISTREC
{
    NAME   head; // this var's position in printNames
    NAMELISTREC* tail;
};

typedef NAMELISTREC* NAMELIST;  //forward declaration
struct  ENVREC
{
    NAMELIST vars; // pointer to list of NAMES
    VALUELIST values; // pointer to lis of VALUES
};

typedef ENVREC* ENV;   //forward declaration
struct  FUNDEFREC
{
    NAME  funname;
    NAMELIST  formals;
    EXP  body;
    FUNDEFREC*  nextfundef;
};

typedef FUNDEFREC* FUNDEF;  //forward declaration
FUNDEF  fundefs = NULL;   //points to the head of the linked list of all the funcions
ENV globalEnv;     // keeps track of all the global variables
EXP currentExp;    //pointer to the current expression
int   inputleng, pos;       // pos is at the current character of inputBuf

char*   printNames[MAXNAMES];   // symbol table
int   numNames, numBuiltins;    

int   quittingtime;  //HALT when true



/*****************************************************************
 *                     DATA STRUCTURE OP'S                       *
 *****************************************************************/

ENV emptyEnv() // return an empty environment
{
    ENV env = new ENVREC;
    return env;
}



/* mkVALEXP - return an EXP of type VALEXP with num n            */

EXP mkVALEXP ( NUMBER n)
{
    EXP e;
    e = new EXPREC;
    e->etype = VALEXP;
    e->num = n;
    return e;
}/* mkVALEXP */


/* mkVAREXP - return an EXP of type VAREXP with varble nm  */

EXP mkVAREXP ( NAME nm) // *********** check me
{ 
    EXP e;
    e = new EXPREC;
    e->etype = VAREXP;
    e->varble = nm;
    
    return e; 
   
    
}/* mkVAREXP */


/* mkAPEXP - return EXP of type APEXP w/ optr op and args el     */

EXP mkAPEXP (NAME op, EXPLIST el) // *********** CHECK ME
{
    
    EXP e;
    e = new EXPREC;
    e->etype = APEXP;
    e->optr = op;
    e->args = el; 
    return e;
   
    
}/* mkAPEXP */

/* mkExplist - return an EXPLIST with head e and tail el         */

EXPLIST mkExplist (EXP e, EXPLIST el)
{
   
    EXPLIST list;
    list = new EXPLISTREC;
    list->head = e;
    list->tail = el;
    return list;
    
    
}/* mkExplist */

/* mkNamelist - return a NAMELIST with head n and tail nl        */

NAMELIST mkNamelist (NAME nm, NAMELIST nl)
{
    
    NAMELIST n = new NAMELISTREC;
    n->head = nm;
    n->tail = nl;
    return n;
    
}/* mkNamelist */

/* mkValuelist - return an VALUELIST with head n and tail vl     */

VALUELIST mkValuelist (NUMBER n,  VALUELIST vl)
{
    VALUELIST list = new VALUELISTREC;
    list->head = n;
    list->tail = vl;
    return list;
    
    
}/* mkValuelist */


/* newFunDef - add new function fname w/ parameters nl, body e   */

void  newFunDef (NAME fname,  NAMELIST nl, EXP e)
{
    FUNDEF f = new FUNDEFREC;
    f->funname = fname;
    f->formals = nl;
    f->body = e;
    f->nextfundef= fundefs;
    fundefs = f;
    
}// newFunDef


/* initNames - place all pre-defined names into printNames */
/* this is our symbol table */

void initNames()
{
    int i =0;
    fundefs = 0;
    printNames[i] = "if"; i++;
    printNames[i] = "while"; i++;
    printNames[i] = "set"; i++;
    printNames[i] = "begin"; i++;
    printNames[i] = "+"; i++;
    printNames[i] = "-"; i++;
    printNames[i] = "*"; i++;
    printNames[i] = "/"; i++;
    printNames[i] = "="; i++;
    printNames[i] = "<"; i++;
    printNames[i] = ">"; i++;
    printNames[i] = "print";
    numNames = i;
    numBuiltins = i;
}//initNames

/* install - insert new name into printNames  */

NAME install ( char* nm)
{
    int i = 0;
    while (i <= numNames)
        if (strcmp( nm,printNames[i] ) == 0)
            break;
        else
            i++;
    if (i > numNames)
    {
        numNames = i;
        printNames[i] = new char[strlen(nm) + 1];
        strcpy(printNames[i], nm);
    }
    return i;
}// install

/* prName - print name nm              */

void prName ( NAME nm)
{
    cout<< printNames[nm];
} //prName

/*****************************************************************
 *                        INPUT                                  *
 *****************************************************************/

/* isDelim - check if c is a delimiter   */

int isDelim (char c)
{
    return ( ( c == '(') || ( c == ')') ||( c == ' ')||( c== COMMENTCHAR) );
}

/* skipblanks - return next non-blank position in inputBuf */

int skipblanks (int position)
{
    
    while (inputBuf[position] == ' ')
    {
        position = position + 1;
    }
    return position;
}


/* parseName - return (installed) NAME starting at inputBuf[pos]*/

NAME parseName()
{
    char nm[20]; // array to accumulate characters
    int leng; // length of name
    leng = 0;
    pos = skipblanks(pos);
    while ( (pos <= inputBuf.size()) && !isDelim(inputBuf[pos]) )
    {
	    ++leng;
	    nm[leng-1] = inputBuf[pos];
        ++pos;
    }
    if (leng == 0)
    {
        cout<<"Error: expected name, instead read: "<< inputBuf[pos]<<endl;
        exit(1);
    }
    nm[leng] = '\0';
    pos = skipblanks(pos); // skip blanks after name
    return ( install(nm) );
}// parseName


/* isDigits - check if sequence of digits begins at pos   */

int isDigits (int pos)
{
    if ( ( inputBuf[pos] < '0' ) ||  ( inputBuf[pos] > '9' ) )
        return 0;
    while ( ( inputBuf[pos] >='0') && ( inputBuf[pos] <= '9') )
        ++pos;
    if( pos < inputBuf.length())
    {
    if (!isDelim(inputBuf[pos] ))
        return 0;
    }
    return 1;
}// isDigits


/* isNUM - check if a number begins at pos  */

int isNUM (int pos) // this needs explaining.
{
    return ( isDigits(pos) || 
            ( (inputBuf[pos] == '-') && isDigits(pos+1))
            ||( (inputBuf[pos] == '+') && isDigits(pos+1)));
}// isNUM

/* parseVal - return number starting at inputBuf[pos]   */

NUMBER parseVal()
{
    int nums[25];
    int numDigits = 0;
    int final = 0;
    pos = skipblanks(pos);
    bool negative = false;
    
    while(isNUM(pos)) // grab the digits from user input and advance pos
    {
        if (inputBuf[pos] == '-')
        {
            negative = true;
            pos++;
        }
        nums[numDigits] = atoi(inputBuf.substr(pos,1).c_str());
        numDigits++;
        pos++;
    }
        int i =0;
    
   

    while (numDigits>0)
    {
        final = final + (nums[i] * pow(10, numDigits -1));
        numDigits--;
        i++;
    }
    if (negative)
        final = final * -1;
    
    return final;
    

}// parseVal

EXPLIST parseEL(); // forward declaration
/* parseExp - return EXP starting at inputBuf[pos]  */

EXP parseExp()
{
    
    // decide if VarEXP, ValExp or OptExp
    pos = skipblanks(pos);
    
    if(inputBuf[pos] == '(')
    {
        pos++;
        // create APEXP
        NAME temp = parseName();
        EXPLIST temp2 = parseEL();
        
        return mkAPEXP(temp,temp2);
    }
    if (isNUM(pos))
    {
        //create valexp
        return mkVALEXP(parseVal());
    }
    //create varexp
    return mkVAREXP(parseName());
    

    //this and parseEL() are mutually recursive
    
}// parseExp

/* parseEL - return EXPLIST starting at inputBuf[pos]  */

EXPLIST parseEL()
{
    pos = skipblanks(pos);
    if (inputBuf[pos] == ')' )
    {
        pos = skipblanks(pos+1);
        return 0;
    }
    EXP temp = parseExp();
    EXPLIST temp2 = parseEL();
    return mkExplist(temp, temp2);
    
    
    //this and parseEXP() are mutually recursive
    
}// parseEL

/* parseNL - return NAMELIST starting at inputBuf[pos]  */

NAMELIST parseNL()
{
    pos = skipblanks(pos);
    if (inputBuf[pos] == ')' )
    {
        pos = skipblanks(pos+1);
        return 0;
    }
    NAME nm = parseName();
    NAMELIST nl = parseNL();
    return mkNamelist(nm,nl);
    
}// parseNL

/* parseDef - parse function definition at inputBuf[pos]   */

NAME parseDef()
{
    NAME fname;        // function name
    NAMELIST nl;       // formal parameters
    EXP e;             // body
    pos = skipblanks(pos+1); // skip '( ..'
    pos = skipblanks(pos+6); // skip 'define ..'
    fname = parseName();
    pos = skipblanks(pos+1); // skip '( ..'
    nl = parseNL();
    e = parseExp();
    pos = skipblanks(pos+1); // skip ') ..'
    newFunDef(fname, nl, e);
    return ( fname);
}// parseDef

/*****************************************************************
 *                     Matt Reader Code                          *
 *****************************************************************/



void readInput()
{
    
    bool print = false;
    
    while( print == false)
    {
        std::cout<<arrow;
        getline(cin, inputBuf);
        clearComments();
        print =checkParands();
        
        while ( print == false)
        {
            string additional;
            std::cout<<miniArrow;
            getline(cin, additional);
            inputBuf.append(additional);
            clearComments();
            print = checkParands();
        }
    }
    
    return;
    
}

bool checkParands()
{
    size_t parandCheck;
    int parands = 0;
    
    inputBuf.find_first_of('(');
    
    if (parandCheck == string::npos)
        return true;
    
    for (int i = 0; i < inputBuf.size(); i++)
    {
        if(inputBuf[i] == '(')
            parands++;
        if(inputBuf[i] == ')')
        {
            if (parands == 0)
            {
                inputBuf.insert(0, "ERROR BAD PARANDS: ");
                return  true;
            }
            parands--;
        }
        
        
    }
    
    if (parands == 0) 
        return true;
    else if (parands <0)
    {
        inputBuf.insert(0, "ERROR BAD PARANTHESIS: ");
        return true;
    }
    else
        return false;
}

void clearComments()
{
    size_t location;
    
    location = inputBuf.find_first_of(';');
    
    if(location == string::npos) // i.e. no comments in the string
        return;
    else
        inputBuf.erase(location,string::npos); // erases portion of string starting at the location of the first ;
    
    
    return;
    
}

//some auxilary function that you Definitely want ti have
//You  may need additional functions
//===================================================================

//The following computes the values for operators 5-12

int lengthVL(VALUELIST vl)
{
    int length = 0;
    if (vl == NULL) 
        return 0;
    length++;
    while(vl->tail != NULL)
    {
        vl = vl->tail;
        length++;
    }
    
    // Double check me...
    
    
    return length;
}

int arity( int op) // returns number of parameters this operator requires
{
    if(op == 11)
        return 1;
    else
        return 2;
}

void prValue(int arg)
{
    cout<<arg;
    return;
}

bool isBound(NAME varble, ENV env) // if the given varble is in the given environemnt env
{
    NAMELIST list = env->vars;
    while (list != NULL)
    {
        if( list->head == varble)
            return true;
        list = list->tail;
    }
    return false; 
}

NUMBER fetch(NAME varble, ENV env)
{
    NAMELIST names = env->vars;
    VALUELIST vals = env->values;
    
    while (names != NULL)
    {
        if( names->head == varble)
            return vals->head;
        
        // traverse both lists
        names = names->tail;
        vals = vals->tail;
    }
    
    cout<<"Fetch ERROR"<<endl;
    exit(1);
    
}

NUMBER applyValueOp ( int op,  VALUELIST vl)
{
    NUMBER  n, n1, n2;
    
    if ( arity(op) != lengthVL(vl) )// correct number of parameters
    {
        cout <<	"Wrong number of arguments to  ";
        prName(op);
        cout <<endl;
    }
    n1 = vl->head; // 1st actual
    if (arity(op) == 2) 
        n2 = vl->tail->head; //2nd actual
    
    switch (op)
    {
        case 4 : n = n1 + n2; break;
        case 5 : n = n1 - n2; break;// do it
        case 6 : n = n1 * n2; break;//do it
        case 7 : n = n1 / n2; break;//do it
        case 8 : n = n1 == n2; break;//do it
        case 9 : n = n1 < n2; break; //do it
        case 10: n = n1 > n2; break;//do it
        case 11: prValue(n1); cout<<endl; n = n1;break;
    };//switch
    return n;
}// applyValueOp

//forward declaration
NUMBER eval ( EXP e,  ENV rho);





//This eveluates a userdefined function and returns its value

NUMBER applyUserFun ( NAME nm, VALUELIST actuals)
{
    FUNDEF temp = fundefs;
    bool found = false;
    // search fundefs for name nm
    FUNDEF f;
    while (temp != NULL)
    {
        if(nm == temp->funname)
        {
            f = temp;
            found = true;
        }
        temp = temp->nextfundef;
    }
    
    if (found == false)
    {
        cout<<"function not found"<<endl;
        exit(1);
    }
    
    ENV rho = emptyEnv();
    rho->vars = f->formals;
    rho->values = actuals;
    
    return eval(f->body, rho);
    
    //do it
}// applyUserFun

void changeVal(int value, NAME name, ENV env)
{
    NAMELIST names = env->vars;
    VALUELIST vals = env->values;
    
    while(names != NULL)
    {
        if(names->head == name)
        {
            vals->head = value;
            return;
        }
        names = names->tail;
        vals = vals->tail;
        
    }
    cout<<"ERROR in ChangeVAL"<<endl;
    exit(1);
}

void addValue(int value, NAME name, ENV env)
{
    env->vars = mkNamelist(name, env->vars);
    env->values = mkValuelist(value, env->values);
}

//This evelustes the ctrl operators

bool isTrueVal(NUMBER eval)
{
    if (eval == 0)
        return false;
    else
        return true;
}

NUMBER applyCtrlOp ( int op,  EXPLIST args, ENV rho )
{
    NUMBER  n =0;
    if (op == 0)  //if statement
        if ((eval(args->head, rho)))
            return  eval(args->tail->head, rho);
        else return  eval(args->tail->tail->head, rho);
    
    if ( op == 1) //while statement
    {
        while (isTrueVal(eval(args->head, rho)))
        {
            eval(args->tail->head,rho);
            
        }
        return 0;
    }
    if( op == 2)  //set
    {
        n = eval(args->tail->head, rho);
        if(isBound(args->head->varble, rho)) // the variable already exists in this env, change it
            changeVal(n, args->head->varble, rho);
        else  if (isBound(args->head->varble, globalEnv))
            changeVal(n, args->head->varble, globalEnv);// add it to the environment 
        else
            addValue(n, args->head->varble, globalEnv);
        return n;
        
        
    }
    if( op == 3) //begin
    {
        while (args!= NULL) // eval all expressions then return value of last.  
        {
            n = eval(args->head,rho);
            args = args->tail;
        }
        return n;
    }// applyCtrlOp
}

//this evaluates a list of expressions and 
//return the correspoding values in a valuelist

VALUELIST frontEvalList( NUMBER value, VALUELIST list); // forward dec.

VALUELIST evalList ( EXPLIST el, ENV rho)
{
    VALUELIST list = NULL;
    
    while (el != NULL)
    {
        list = frontEvalList(eval(el->head, rho), list);
        el = el->tail;
    }
    return list;
}// evalList


VALUELIST frontEvalList( NUMBER value, VALUELIST list)
{
    
    if (list == NULL)
    {
        VALUELIST temp = new VALUELISTREC;
        temp->head = value;
        temp->tail = NULL;
        return temp;
    }
    else
    {
        VALUELIST temp = list;
        while (temp->tail!= NULL)
            temp = temp->tail;
        VALUELIST addMe = new VALUELISTREC;
        addMe->head= value;
        addMe->tail= NULL;
        temp->tail = addMe;
        return list;
    }
}
//The eval function calls the other appropriate functions
//to evaluate an expresion

NUMBER eval( EXP e,  ENV rho)
{
    switch (e->etype)
    {
        case VALEXP: return (e->num);
        case VAREXP: if (isBound(e->varble, rho))
            return fetch(e->varble, rho);
        else if ( isBound(e->varble, globalEnv))
            return fetch(e->varble, globalEnv);
        else{
            cout << "Undefined variable: ";
            prName(e->varble);
            cout <<endl;
            exit(1);
        }
        case APEXP:  if (e->optr > numBuiltins)
            return  applyUserFun(e->optr, evalList(e->args, rho));
        else {
            if ( e->optr<4 )
                return applyCtrlOp(e->optr, e->args, rho);
            else
                return applyValueOp(e->optr, evalList(e->args, rho));
        }
    }
    return 0;
} // eval


#endif
