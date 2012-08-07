//
//  Utis.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "Utils.h"

using namespace std;

string loadFile(string _pathFile) 
{
    string result;
    
	ifstream infile;
    
    char* str = new char[_pathFile.length()];
    strcpy(str, _pathFile.c_str());
    
	infile.open(str);
    
    if (!infile) {
        std::cerr << "File not open" << endl;
        return NULL;
    }
    
	while(!infile.eof()) {
		getline(infile, result);
		cout << result << endl;
	}
    
	infile.close();
    
    return result;
}
