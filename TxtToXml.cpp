#include "./lib/pugixml/src/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "./src/debug/timer.h"
#include "./src/sequence.h"
#include "./src/loadFiles.h"
#include "./src/prefix.h"

#define DEBUG

#define INPUT_SET_PATH "./datasets/sets_family.xml"
#define INPUT_TXT_PATH "./datasets/family.txt"
#define OUTPUT_XML_PATH "./build/output.xml"

pugi::xml_document definitionSets, outputXML;

void ConvertSet(const std::string setTxt)
{
    const std::string setId = setTxt.substr(0, setTxt.find('/'));
    // find tag Set with same setID and get its name
    const pugi::xml_node definitionSet = definitionSets
                                             .child("Sets")
                                             .find_child_by_attribute("Set", "setID", setId.c_str());
    const std::string setName = definitionSet
                                    .attribute("name")
                                    .value();
    // create set tag
    pugi::xml_node set = outputXML.append_child(setName.c_str());
    set.append_attribute("setID") = setId.c_str();

    int ffSeq = 1;
    std::string segmentTxt;
    std::stringstream test(setTxt.substr(setTxt.find("/") + 1));

    // for every segment in text input
    while (getline(test, segmentTxt, '/'))
    {
        if (segmentTxt != "-")
        {
            // element by ffSeq
            pugi::xml_node element = definitionSet
                                         .find_child_by_attribute("Element", "ffSeq", std::to_string(ffSeq).c_str());

            set.append_child(element
                                 .attribute("name")
                                 .value());

            pugi::xml_node outputElement = set.child(
                element
                    .attribute("name")
                    .value());
            pugi::xml_node sequence = element.child("Sequence");

            if (element.child("Choice") != 0)
            {
                const std::string chooseBy = (std::string)element
                                                 .attribute("choice")
                                                 .value();
                // choice by prefix
                if (chooseBy == "prefix")
                {
                    // returns [name, value]
                    std::tuple<std::string, std::string> prefixElement = ChooseByPrefix(segmentTxt, element);
                    // create element
                    outputElement
                        .append_child(std::get<0>(prefixElement).c_str())
                        .text() = std::get<1>(prefixElement).c_str();
                }
                // choice by length
                else if (chooseBy == "length")
                {
                    pugi::xml_node choiceElement = element
                                                       .child("Choice")
                                                       .find_child_by_attribute("Element", "length", std::to_string(segmentTxt.length()).c_str());
                    sequence = choiceElement.child("Sequence");
                    if (sequence != 0)
                        Sequence(sequence, outputElement, segmentTxt);
                    else
                        outputElement
                            .append_child(choiceElement
                                              .attribute("name")
                                              .value())
                            .text() = segmentTxt.c_str();
                }
                // choice by regex
                else
                {
                    const std::string prefix = GetPrefix(segmentTxt);

                    for (pugi::xml_node choiceElement : element.child("Choice"))
                    {

                        // get regex
                        const std::regex rgx(choiceElement.attribute("RegExp").value());
                        const char *choiceName = choiceElement.attribute("name").value();
                        const char *choicePrefix = choiceElement.attribute("prefix").value();
                        const char *segmentTxtWithoutPrefix = segmentTxt.substr(prefix.length() + 1).c_str();

                        // check if regex matches
                        if (regex_match((prefix == "") ? segmentTxt : segmentTxtWithoutPrefix, rgx))
                        {
                            const pugi::xml_node sequence = choiceElement.child("Sequence");

                            // segmentTxt has prefix and choice element has prefix
                            if (prefix != "" && choicePrefix == prefix)
                            {
                                if (sequence != 0)
                                    Sequence(sequence, outputElement, segmentTxtWithoutPrefix);
                                else
                                    outputElement.append_child(choiceName).text() = segmentTxtWithoutPrefix;
                            }
                            // if segmentTxt does not have prefix
                            else if (prefix == "" && choiceElement.attribute("prefix") == 0)
                            {
                                if (sequence != 0)
                                    Sequence(sequence, outputElement, segmentTxt);
                                else
                                    outputElement.append_child(choiceName).text() = segmentTxt.c_str();
                            }
                        }
                    }
                }
            }
            else if (sequence != 0)
                Sequence(sequence, outputElement, segmentTxt);

            else
                outputElement.text() = segmentTxt.c_str();
        }
        ffSeq++;
    }
}

int main()
{
#ifdef DEBUG
    Timer timer;
#endif
    const std::string inputTxt = LoadTxtFile(INPUT_TXT_PATH);

    definitionSets.load_file(INPUT_SET_PATH); // load sets

    int start = 0;
    const std::string del = "//";
    int end = inputTxt.find(del);

    // loop through every set
    while (end != -1)
    {
        const std::string setTxt = inputTxt.substr(start, end - start);
        start = end + del.size();
        end = inputTxt.find(del, start);

        ConvertSet(setTxt);
    }
    outputXML.save_file(OUTPUT_XML_PATH);
}
