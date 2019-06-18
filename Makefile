.PHONY: all clean

objects:= GF2_Matrix.o LDPC_Mgr.o
cflags:= -std=c++11 -g

all: encode decode

encode: encode.o
	g++ -o $@ $(cflags) encode.o $(objects)

decode: decode.o
	g++ -o $@ $(cflags) decode.o $(objects)

encode.o: encode.cpp $(objects)
	g++ -c $(cflags) encode.cpp

decode.o: decode.cpp $(objects)
	g++ -c $(cflags) decode.cpp

LDPC_Mgr.o : LDPC_Mgr.h LDPC_Mgr.cpp GF2_Matrix.o
	g++ -c $(cflags) LDPC_Mgr.cpp LDPC_Mgr.h

GF2_Matrix.o : GF2_Matrix.cpp GF2_Matrix.h 
	g++ -c $(cflags) GF2_Matrix.cpp GF2_Matrix.h

clean :
	rm *.o *.gch encode decode