all: ttx

clean:
	rm -f */*.out */*.o

ttx: TxtToXml.cpp pugixml.o
	g++ TxtToXml.cpp ./lib/pugixml/src/pugixml.cpp.o -o ./build/ttx.out

pugixml.o: ./lib/pugixml/src/pugixml.cpp
	g++ ./lib/pugixml/src/pugixml.cpp -c -o ./lib/pugixml/src/pugixml.cpp.o