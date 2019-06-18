.PHONY: all clean

objects:= GF2_Matrix.o LDPC_Mgr.o

all: encode decode

encode: encode.o
	g++ -o $@ -std=c++11 encode.o $(objects)

decode: decode.o
	g++ -o $@ -std=c++11 decode.o $(objects)

encode.o: encode.cpp $(objects)
	g++ -c -std=c++11 encode.cpp

decode.o: decode.cpp $(objects)
	g++ -c -std=c++11 decode.cpp

LDPC_Mgr.o : LDPC_Mgr.h LDPC_Mgr.cpp GF2_Matrix.o
	g++ -c -std=c++11 LDPC_Mgr.cpp LDPC_Mgr.h

GF2_Matrix.o : GF2_Matrix.cpp GF2_Matrix.h 
	g++ -c -std=c++11 GF2_Matrix.cpp GF2_Matrix.h

clean :
	rm main *.o *.gch