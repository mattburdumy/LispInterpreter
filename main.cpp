//
//  main.cpp
//  Lisp Reader
//
//  Created by Matt Burdumy on 9/10/13.
//  Copyright 2013 Georgetown University. All rights reserved.
//


/*****************************************************************
 *                         MAIN LOOP                              *
 *****************************************************************/

#include "lispHeader.h"
#include <stdlib.h>
#include <string>

using namespace std;

int main()
{
    
    initNames();
    globalEnv = emptyEnv();
    
    quittingtime = 0;
    while (!quittingtime)
    {
        readInput();
        
        if (inputBuf.substr(pos,4) == "quit")  
            quittingtime = 1;
        else if( (inputBuf[pos] == '(') &&
                inputBuf.substr(skipblanks(pos+1),6) == "define") 
        {
		    prName(parseDef());
		    cout <<endl;
        }
        else {
			currentExp = parseExp(); // current expresssion is the global pointer to the parse tree.
			prValue(eval(currentExp, emptyEnv() )); 
			cout <<endl<<endl;
        }
        inputBuf.clear(); // flush input buffer
        pos = 0; // reset pos
	}// while
    
    return 1;
}
