/*
Comm Lab Final Project
Hanmo Ou 17/6/19
*/
#include <iostream>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <utility>

#include "LDPC_Mgr.h"
#include "GF2_Matrix.h"

using namespace std;

LDPC_Mgr::LDPC_Mgr(istream& is){
	_checkMatrix = GF2_Matrix(is);
	_n = _checkMatrix.col();
	_k = _checkMatrix.col() - _checkMatrix.row();
	cout<< "Constructing LDPC Code, n=" << _n << " k=" << _k << endl;
	cout << "Parity Check Matrix H:" << endl;
	_checkMatrix.print();
	_checkMatrix.calc_generator();
	_generateMatrix.print();
	_generateMatrix = _checkMatrix.get_generator();
	cout << "Genertor Matirx G:" << endl;
	_generateMatrix.print();
	GF2_Matrix confirm;
	confirm = _checkMatrix*_generateMatrix;
	cout << "Confirming HG=O" << endl; 
	confirm.print();
	initialize_edges();
	build_adjacency();
}

LDPC_Mgr::~LDPC_Mgr(){
	delete [] _adjB;
	delete [] _adjC;
}

void
LDPC_Mgr::build_adjacency(){
	_adjB = new IdList[_n];
	_adjC = new IdList[_n - _k];
	for(unsigned r = 0; r < _checkMatrix.row(); r++){
		for(unsigned c = 0; c < _checkMatrix.col(); c++){
			if (_checkMatrix.getValue(r,c)){
				(_adjB[c]).push_back(r);
				(_adjC[r]).push_back(c);
			}
		}
	}
}

void
LDPC_Mgr::decode(istream& is, ostream& os){
	cout << "Reading Input from File..." << endl;
	GF2_Matrix input(is);
	assert(input.col() == _checkMatrix.col());
	GF2_Matrix output(input.row(), _k);
	const float hard_value = 10.5;
	float* bitL = new float[_n];
	float* bitLi = new float[_n];
	float* checkL = new float[_n - _k];
	cout << _bcTable.size() << endl;
	// for each line of coded bits
	for(unsigned line = 0; line < input.row(); line++){
		cout << "Decoding Line: " << line + 1 << endl;
		// initialize Ls
		for(unsigned i = 0; i < input.col(); i++){
			if (input.getValue(line,i)){
				bitL[i] = -hard_value;
				bitLi[i] = -hard_value;
			}
			else {
				bitL[i] = hard_value;
				bitLi[i] = hard_value;
			}
		}
		// Start Iteration
		const unsigned iteration = 50;
		float decay = 1, alpha = 1;
		for(unsigned iter = 0; iter < iteration; iter++){
			// initialize_edges();
			// b->c
			for(unsigned iB = 0; iB < _n; iB++){
				for(IdList::iterator j = _adjB[iB].begin() ; j !=_adjB[iB].end(); j++  ){
					assert(_bcTable.find(getKey(iB,*j)) != _bcTable.end());
					_bcTable[getKey(iB,*j)] = bitLi[iB];
					for(IdList::iterator j2 = _adjB[iB].begin() ; j2 !=_adjB[iB].end(); j2++  ){
						assert(_cbTable.find(getKey(iB,*j2)) != _cbTable.end());
						if (j != j2){
							_bcTable[getKey(iB,*j)] += _cbTable[getKey(iB, *j2)];
						}
					}
				}
			}
			// c->b
			for(unsigned jC = 0; jC < _n - _k; jC++){
				for(IdList::iterator i = _adjC[jC].begin() ; i !=_adjC[jC].end(); i++  ){
					assert(_cbTable.find(getKey(*i,jC)) != _cbTable.end());
					_cbTable[getKey(*i, jC)] = 100000;
					for(IdList::iterator i2 = _adjC[jC].begin() ; i2 !=_adjC[jC].end(); i2++  ){
						assert(_bcTable.find(getKey(*i2, jC)) != _bcTable.end());
						if (i != i2){
							_cbTable[getKey(*i, jC)] = minsum(_cbTable[getKey(*i, jC)], _bcTable[getKey(*i2, jC)] );
						}
					}
					bitL[*i] += decay*_cbTable[getKey(*i, jC)];
				}
			}
			decay *= alpha;
		}

		// Modify output matrix
		// only first k bits 
		for(unsigned i = 0; i < _k; i++){
			if (bitL[i] < 0){
				output.setValue(line, i , 1);
			}
			else {
				output.setValue(line, i , 0);  
			}
		}
		


	}
	output.write(os);
}

void 
LDPC_Mgr::encode(istream& is, ostream& os){
	cout << "Reading Input from File..." << endl;
	GF2_Matrix input(is);
	GF2_Matrix output;
	cout << "Encoding..." << endl;
	output = _generateMatrix*(input.transpose());
	cout << "Completed Encoding" << endl;
	cout << "Writing to File..." << endl;
	output.transpose().write(os);
	
}

unsigned long long
LDPC_Mgr::getKey(unsigned b, unsigned c){
	return (unsigned long long) b << 32 | (unsigned int) c;
}

void
LDPC_Mgr::initialize_edges(){

	for(unsigned r = 0; r < _checkMatrix.row(); r++){
		for(unsigned c = 0; c < _checkMatrix.col(); c++){
			if (_checkMatrix.getValue(r,c)){
				_bcTable[getKey(c,r)] = 0;
				_cbTable[getKey(c,r)] = 0;
			}
		}
	}
}

float
LDPC_Mgr::minsum(float a, float b){
	float sign = (a*b > 0 ? 1 : -1 );
	return sign*fmin(abs(a), abs(b));
}