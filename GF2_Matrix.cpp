/*
Comm Lab Final Project
Hanmo Ou 14/6/19
*/
#include <iostream>
#include <bitset>
#include <cassert>

#include "GF2_Matrix.h"

using namespace std;

GF2_Matrix::GF2_Matrix(){
    //cout << "Construct Default"  << endl;
    _matrix = 0;
    _nRow   = 0;
    _nCol   = 0;
}

GF2_Matrix::GF2_Matrix(unsigned nRow, unsigned nCol){
    //cout << "Construct Zero" << nRow << " " << nCol << endl; 
    _nRow = nRow;
    _nCol = nCol;
    allocate_matrix();
}

GF2_Matrix::GF2_Matrix(istream& is){
    
    is >> _nRow;
    is >> _nCol;
    //cout << "Construct File" << _nRow << " " << _nCol << endl;
    allocate_matrix();
    for(unsigned  r = 0; r < _nRow ; r++){
        for(unsigned c = 0; c < _nCol; c++){
            is >> _matrix[r][c];
        }
    }
}

GF2_Matrix::GF2_Matrix(const GF2_Matrix& cp){
    //cout << "Copy" << endl;
    _nCol = cp._nCol;
    _nRow = cp._nRow;
    allocate_matrix();
    for(unsigned r = 0; r < _nRow ; r++){
        for(unsigned c = 0; c < _nCol; c++){
            _matrix[r][c] = cp._matrix[r][c];
        }
    }
}

GF2_Matrix::~GF2_Matrix(){
    //cout << "DESTRUCT:" << _nRow << " " << _nCol << endl;
    reset();
}

GF2_Matrix&
GF2_Matrix::operator=(const GF2_Matrix& rhs){
    //cout << "Assign" << endl;
    reset();
    _nCol = rhs._nCol;
    _nRow = rhs._nRow;
    allocate_matrix();
    for(unsigned r = 0; r < _nRow ; r++){
        for(unsigned c = 0; c < _nCol; c++){
            _matrix[r][c] = rhs._matrix[r][c];
        }
    }
    return *this;
}

GF2_Matrix
GF2_Matrix::operator*(const GF2_Matrix& m2){
    assert(_nCol == m2._nRow);
    GF2_Matrix ret(_nRow, m2._nCol);
    for(unsigned r = 0; r < _nRow ; r++){
        for(unsigned c = 0; c < m2._nCol; c++){
            for(unsigned i = 0; i < _nCol; i++){
                ret._matrix[r][c] = ret._matrix[r][c] ^ (_matrix[r][i] & m2._matrix[i][c]);
            }
        }
    }
    return ret;

}


void
GF2_Matrix::allocate_matrix(){
    //cout << "Allocate " << _nRow << "  " << _nCol << endl;
    _matrix = new bool*[_nRow];
    for(unsigned i = 0; i < _nRow; i++){
        _matrix[i] = new bool[_nCol] ; 
    }
    for(unsigned i = 0; i < _nRow; i++){
        for(unsigned j = 0; j < _nCol; j++){
            _matrix[i][j] = 0;
        } 
    }
    //cout << "Allocated: " << _matrix << endl;

}

void 
GF2_Matrix::calc_generator(){
    assert(_nCol >= _nRow);
    unsigned c_start = _nCol - _nRow;
    // cout << "c_start = " << c_start << endl;
    // Column 
    for(unsigned c = 0 ; c < _nRow; c++){
        unsigned r_nz = c;
        // Find first non-zero
        for(unsigned r = c; r < _nRow; r++){
            if (_matrix[r][c_start + c] == 1){
                r_nz = r;
                break;
            }
        }
        // Swap to target row
        // Target Row = C
        rowop_swap(c, r_nz);
        // Cancel out other rows
        for(unsigned r = 0; r < _nRow; r++){
            if(_matrix[r][c_start + c] == 1){
                rowop_add(c, r);
            }
        }
        // print();
        //cout << endl;
    }

}

GF2_Matrix
GF2_Matrix::get_generator() const {
    GF2_Matrix g(_nCol, _nCol - _nRow);
    // Identity
    for(unsigned i = 0; i < _nCol - _nRow; i++){
        g._matrix[i][i] = 1;
    }
    for(unsigned i = 0; i < _nRow; i++){
        for(unsigned j = 0; j < _nCol - _nRow; j++){
            g._matrix[i + _nCol - _nRow][j] = _matrix[i][j];
        }
    }
    //cout << "Finished Generator" << endl;
    // g.print();
    return g;
}

void 
GF2_Matrix::print() const {
    // return;
    for(unsigned r = 0; r < _nRow; r++){
        for(unsigned c = 0; c < _nCol; c++){
            cout << _matrix[r][c] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void
GF2_Matrix::rowop_add(unsigned f, unsigned t){
    assert(f < _nRow && t < _nCol);
    if (f == t){
        return;
    }
    for(unsigned c = 0; c < _nCol; c++){
        _matrix[t][c] = _matrix[t][c] ^ _matrix[f][c];
    }
}

void 
GF2_Matrix::rowop_swap(unsigned r1, unsigned r2){
    if (r1 == r2){
        return;
    }
    bool* temp;
    temp = _matrix[r1];
    _matrix[r1] = _matrix[r2];
    _matrix[r2] = temp;
}

GF2_Matrix
GF2_Matrix::transpose() const{
    GF2_Matrix ret(_nCol, _nRow);
    for(unsigned r = 0; r < _nRow; r++){
        for(unsigned c = 0; c < _nCol; c++){
            ret._matrix[c][r] = _matrix[r][c];
        }
    }
    return ret;
}

void 
GF2_Matrix::reset(){
    if (!_matrix){
        return;
    }
    //cout << "Reset1" << endl;
    //cout << _nRow << endl;
    for(unsigned i = 0; i < _nRow; i++){
        //cout <<  _matrix[i]  << endl;
        if (_matrix[i] != 0) delete [] _matrix[i] ; 
    }
    //cout << "Reset2" << endl;
    delete [] _matrix;
    _matrix = 0;
}

void 
GF2_Matrix::write(ostream& os) const {
    os << _nRow << " " << _nCol << endl;
    for(unsigned r = 0; r < _nRow; r++){
        for(unsigned c = 0; c < _nCol; c++){
            os << _matrix[r][c] << " ";
        }
        os << endl;
    }
}