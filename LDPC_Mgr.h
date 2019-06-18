/*
Comm Lab Final Project
Hanmo Ou 17/6/19
*/
#ifndef LDPC_MGR 
#define LDPC_MGR

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "GF2_Matrix.h"

using namespace std;

typedef vector<unsigned> IdList;
typedef unordered_map<unsigned long long, float> LTable;

class LDPC_Mgr{
public:
	// Constructors
	LDPC_Mgr(istream&);
	~LDPC_Mgr();

	// Public Functions
	void decode(istream&, ostream&);
	void encode(istream&, ostream&);


private:
	// data members
	unsigned _n, _k;
	GF2_Matrix _checkMatrix, _generateMatrix;

	// decoding 
	LTable _bcTable;
	LTable _cbTable;
	IdList* _adjB;
	IdList* _adjC;
	float minsum(float a, float b);


	// utility functions
	void build_adjacency();
	void initialize_edges();
	unsigned long long getKey(unsigned, unsigned);
};



#endif

