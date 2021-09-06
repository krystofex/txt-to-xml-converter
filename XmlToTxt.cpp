#include "./lib/pugixml/src/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "./src/debug/timer.h"

#define DEBUG

#define INPUT_SET_PATH "./datasets/sets_family.xml"
#define INPUT_XML_PATH "./build/output.xml"
#define OUTPUT_TXT_PATH "./build/output.txt"

pugi::xml_document definitionSetsDocument, inputXML;
std::string outputTxt = "";

int main()
{
#ifdef DEBUG
    Timer timer;
#endif
    std::ofstream outputFile;
    definitionSetsDocument.load_file(INPUT_SET_PATH); // load sets
    pugi::xml_node definitionSets = definitionSetsDocument.child("Sets");
    inputXML.load_file(INPUT_XML_PATH); // load xml to convert

    for (const pugi::xml_node &set : inputXML)
    {
        const pugi::xml_node currentDefinitionSet = definitionSets.find_child_by_attribute("setID", set.attribute("setID").value());
        const int numberOfElements = currentDefinitionSet.select_nodes("Element").size();

        outputTxt += set.attribute("setID").value();

        for (int ffseq = 1; ffseq < numberOfElements + 1; ffseq++)
        {
            const auto name = currentDefinitionSet.find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str()).attribute("name").value();

            if (set.child(name).text() != 0)
            {
                outputTxt += "/";
                outputTxt += set.child(name).text().get();
            }

            else if (set.child(name) == 0)
                outputTxt += "/-";

            std::cout
                << (name) << " - " << set.child(name).text() << " - " << set.child(name).text().get() << std::endl;
        }

        outputTxt += "//\n";
    }

    outputFile.open(OUTPUT_TXT_PATH);
    outputFile << outputTxt;
    outputFile.close();
}
