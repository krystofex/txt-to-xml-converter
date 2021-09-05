all: ttx xtt

clean:
	rm -f */*.out */*.o

# txt to xml
ttx: TxtToXml.cpp pugixml.o
	g++ TxtToXml.cpp ./lib/pugixml/src/pugixml.cpp.o -o ./build/ttx.out

# xml to txt
xtt: XmlToTxt.cpp pugixml.o
	g++ XmlToTxt.cpp ./lib/pugixml/src/pugixml.cpp.o -o ./build/xtt.out

pugixml.o: ./lib/pugixml/src/pugixml.cpp
	g++ ./lib/pugixml/src/pugixml.cpp -c -o ./lib/pugixml/src/pugixml.cpp.o