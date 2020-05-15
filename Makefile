obj = RC4.o utils.o SM4.o DES.o AES128.o SHA2.o SHA3.o SM3.o BM.o

main:$(obj) main.o
	g++ main.o $(obj) -o main

main.o:main.cpp
	g++ -c main.cpp -o main.o
$(obj):BM/*.cpp Compression/*.cpp Encryption/*.cpp RC4/*.cpp
	g++ -c BM/*.cpp Compression/*.cpp Encryption/*.cpp RC4/*.cpp

clean:
	rm *.o main