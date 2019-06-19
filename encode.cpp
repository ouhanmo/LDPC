/*
Comm Lab Final Project
Hanmo Ou 19/6/19
*/
#include <iostream>
#include <fstream>

#include "GF2_Matrix.h"
#include "LDPC_Mgr.h"

using namespace std;

int main(int argc, char** argv){
	ifstream ifs(argv[1]);
    ifstream ifs2(argv[2]);
	ofstream ofs(argv[3]);
	LDPC_Mgr mgr(ifs);
	mgr.encode(ifs2, ofs);
	return 0;
}