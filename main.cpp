#include <iostream>
#include <fstream>

#include "GF2_Matrix.h"
#include "LDPC_Mgr.h"

using namespace std;

int main(int argc, char** argv){
	ifstream ifs(argv[1]);
	GF2_Matrix a(ifs);
	cout << "Before Row Operations" << endl;
	a.print();
	GF2_Matrix b=a;
	b.calc_generator();
	cout << "After Row Operations" << endl;
	b.print();
	ifstream ifs2(argv[1]);
	LDPC_Mgr mgr(ifs2);
	ifstream ifs3(argv[2]);
	ofstream ofs(argv[3]);
	mgr.encode(ifs3, ofs);


	return 0;
}