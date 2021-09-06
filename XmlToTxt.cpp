#include "./lib/pugixml/src/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "./src/debug/timer.h"
#include "./src/files.h"

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
    definitionSetsDocument.load_file(INPUT_SET_PATH); // load sets
    pugi::xml_node definitionSets = definitionSetsDocument.child("Sets");
    inputXML.load_file(INPUT_XML_PATH); // load xml to convert

    for (const pugi::xml_node &set : inputXML)
    {
        const pugi::xml_node currentDefinitionSet = definitionSets.find_child_by_attribute("setID", set.attribute("setID").value());
        const int numberOfElementsInSet = currentDefinitionSet.select_nodes("Element").size();
        const int numberOfElementsInInput = std::distance(set.begin(), set.end());

        outputTxt += set.attribute("setID").value();

        for (int ffseq = 1; ffseq < numberOfElementsInSet + 1; ffseq++)
        {
            if (ffseq > numberOfElementsInInput)
                break;

            const auto name = currentDefinitionSet.find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str()).attribute("name").value();
            const pugi::xml_node currentDefinitionElement = currentDefinitionSet.find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str());

            if (set.child(name).text() != 0)
            {
                outputTxt += "/";
                outputTxt += set.child(name).text().get();
            }
            else if (currentDefinitionElement.child("Choice") != 0)
            {
                std::cout << "Choice\n";
            }
            else if (currentDefinitionElement.child("Sequence") != 0)
            {
                std::cout << "Sequence\n";
            }
            else if (set.child(name) == 0)
                outputTxt += "/-";
        }

        outputTxt += "//\n";
    }

    std::cout << outputTxt << std::endl;
    SaveTxt(outputTxt, OUTPUT_TXT_PATH);
}
