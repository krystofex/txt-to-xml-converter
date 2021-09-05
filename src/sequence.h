#include "../lib/pugixml/src/pugixml.hpp"

#include <regex>
#include <iostream>

void RegExpSequence(pugi::xml_node sequence, pugi::xml_node outputElement, std::string segmentTxt)
{
    std::string RegExp;
    for (const pugi::xml_node &sequenceElement : sequence)
    {
        RegExp += "(";
        RegExp += sequenceElement.attribute("RegExp").value();
        RegExp += ")";
    }
    const std::regex rgx(RegExp);
    std::smatch matches;
    if (std::regex_search(segmentTxt, matches, rgx))
    {
        int i = 1;
        for (const pugi::xml_node &sequenceElement : sequence)
            outputElement.append_child(sequenceElement.attribute("name").value()).text() = matches[i++].str().c_str();
    }
}

void LengthSequence(pugi::xml_node sequence, pugi::xml_node outputElement, const std::string segmentTxt)
{
    // if sequence organized by length
    if (sequence.first_child().attribute("length") != 0)
    {
        int positionInString = 0;
        for (const pugi::xml_node &sequenceElement : sequence)
        {
            const int sequenceElementLength = atoi(sequenceElement
                                                       .attribute("length")
                                                       .value());
            const std::string sequenceElementName = sequenceElement
                                                        .attribute("name")
                                                        .value();
            outputElement
                .append_child(sequenceElementName.c_str())
                .text() = segmentTxt.substr(positionInString, sequenceElementLength).c_str();

            positionInString += sequenceElementLength;
        }
    }
}

void Sequence(pugi::xml_node sequence, pugi::xml_node outputElement, const std::string segmentTxt)
{
    // if sequence organized by regex
    if (sequence.first_child().attribute("RegExp") != 0)
        RegExpSequence(sequence, outputElement, segmentTxt);

    // if sequence organized by length
    else if (sequence.first_child().attribute("length") != 0)
        LengthSequence(sequence, outputElement, segmentTxt);
}
