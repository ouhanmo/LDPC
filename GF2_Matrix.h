/*
Comm Lab Final Project
Hanmo Ou 14/6/19
*/
#ifndef GF2_MATRIX
#define GF2_MATRIX

#include <iostream>

using namespace std;

class GF2_Matrix{
public:
	// Constructor and Destructor
	GF2_Matrix(); // default
	GF2_Matrix(unsigned, unsigned); // zero
	GF2_Matrix(GF2_Matrix&); // copy 
	GF2_Matrix(istream&); // input
	~GF2_Matrix(); 
	
	// Operators
	GF2_Matrix& operator=(const GF2_Matrix&);
	GF2_Matrix operator*(const GF2_Matrix&);
	
	// Get Functions
	unsigned row() const {return _nRow;}
	unsigned col() const {return _nCol;}
	unsigned getValue(unsigned r, unsigned c) const { return _matrix[r][c];}
	void setValue(unsigned r, unsigned c, bool v){ _matrix[r][c] = v;}

	// Public Functions
	void calc_generator();
	GF2_Matrix get_generator() const;
	void print() const;
	GF2_Matrix transpose() const;
	void write(ostream&) const;
	

private:
	// Data Members
	unsigned _nRow, _nCol;
	bool ** _matrix;

	// Private Functions
	void allocate_matrix();
	void rowop_add(unsigned, unsigned);
	void rowop_swap(unsigned, unsigned);
	void reset();

};

#endif