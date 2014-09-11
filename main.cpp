//
//  main.cpp
//  Lisp Reader
//
//  Created by Matt Burdumy on 9/10/13.
//  Copyright 2013 Georgetown University. All rights reserved.
//


/*****************************************************************
 *                        MAHE MAIN                              *
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
        
        if (inputBuf.substr(pos,4) == "quit")  // matches(pos, 4, "quit"))
            quittingtime = 1;
        else if( (inputBuf[pos] == '(') &&
                inputBuf.substr(skipblanks(pos+1),6) == "define") // matches(skipblanks(pos+1), 6, "define")  )
        {
		    prName(parseDef());
		    cout <<endl;
        }
        else {
			currentExp = parseExp(); // current expresssion is the global pointer to the parse tree.
			prValue(eval(currentExp, emptyEnv() )); //prValue(eval(currentExp, emptyEnv() ));  <-- ORIG
			cout <<endl<<endl;
        }
        inputBuf.clear(); // flush input buffer
        pos = 0; // reset pos
	}// while
    
    return 1;
}














// ******************** END MAHE CODE **************************//
// *                                                           *//
// *************************************************************//

// Start my reader code -- ignore for now i will integrate at the next step.





