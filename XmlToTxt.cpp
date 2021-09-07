#include "./lib/pugixml/src/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "./src/debug/timer.h"
#include "./src/files.h"

#define DEBUG

#define INPUT_SET_PATH "./datasets/sets_family.xml"
#define INPUT_XML_PATH "./build/output2.xml"
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
        for (int ffseq = 1; ffseq < numberOfElementsInSet + 1 + ((currentDefinitionSet.attribute("groupField") != 0) ? 1 : 0); ffseq++)
        {
            pugi::xml_node currentDefinitionElement = currentDefinitionSet.find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str());
            auto name = currentDefinitionSet.find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str()).attribute("name").value();
            bool gof = false;
            if (currentDefinitionElement == 0)
            {
                gof = true;
                currentDefinitionElement = currentDefinitionSet
                                               .child("GroupOfFields")
                                               .find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str());
                name = currentDefinitionSet
                           .child("GroupOfFields")
                           .find_child_by_attribute("ffSeq", std::to_string(ffseq).c_str())
                           .attribute("name")
                           .value();
            }
            pugi::xml_node currentElement = set.child(name);
            if (gof)
                currentElement = set
                                     .child("GroupOfFields")
                                     .child(name);

            if (currentElement == 0)
            {
                outputTxt += "/-";
            }
            else if (currentElement.text() != 0)
            {
                outputTxt += "/";
                outputTxt += currentElement.text().get();
            }
            else if (currentDefinitionElement.child("Choice") != 0)
            {
                std::string chooseBy = currentDefinitionElement.attribute("choice").value();
                if (chooseBy == "prefix")
                {
                    std::string prefix = currentDefinitionElement
                                             .child("Choice")
                                             .find_child_by_attribute("name", currentElement.first_child().name())
                                             .attribute("prefix")
                                             .value();
                    outputTxt += "/" + prefix;
                    if (prefix.length() > 0)
                        outputTxt += ":";
                    outputTxt += currentElement.first_child().text().get();
                }
                else
                {
                    outputTxt += "/";
                    for (const pugi::xml_node &sequenceElement : currentElement)
                    {
                        std::string text = sequenceElement.text().get();
                        outputTxt += ((text.length() > 0) ? text : " ");
                    }
                }
            }
            else if (currentDefinitionElement.child("Sequence") != 0)
            {
                outputTxt += "/";
                for (const pugi::xml_node &sequenceElement : currentElement)
                {
                    std::string text = sequenceElement.text().get();
                    outputTxt += ((text.length() > 0) ? text : " ");
                }
            }
        }

        outputTxt += "//\n";
    }

    SaveTxt(outputTxt, OUTPUT_TXT_PATH);
}
