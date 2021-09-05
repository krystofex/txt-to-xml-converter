#include "./lib/pugixml/src/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "./src/debug/timer.h"

#define DEBUG

#define INPUT_SET_PATH "./datasets/sets_family.xml"
#define INPUT_XML_PATH "./build/output.xml"
#define OUTPUT_TXT_PATH ".build/output.txt"

pugi::xml_document definitionSets, inputXML;
std::string outputTxt;

int main()
{
#ifdef DEBUG
    Timer timer;
#endif
    definitionSets.load_file(INPUT_SET_PATH); // load sets
    inputXML.load_file(INPUT_XML_PATH);       // load xml to convert

    for (const pugi::xml_node &set : inputXML)
    {
        std::cout << set.attribute("setID").value() << std::endl;
    }
}
