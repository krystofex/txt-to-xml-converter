#include "../lib/pugixml/src/pugixml.hpp"

#include <iostream>
#include <sstream>
#include <tuple>

std::string GetPrefix(std::string text)
{
    if (text.find(":") < text.length())
        return text.substr(0, text.find(":"));
    else
        return "";
};

std::tuple<std::string, std::string> ChooseByPrefix(const std::string segmentTxt, pugi::xml_node element)
{
    const std::string prefix = GetPrefix(segmentTxt);
    int restLength = 0;
    pugi::xml_node choice;
    // if there is a prefix select node by prefix
    if (prefix == "")
        choice = element.child("Choice").first_child();

    // else select first node (without declared prefix)
    else
    {
        choice = element.child("Choice")
                     .find_child_by_attribute("Element", "prefix", prefix.c_str());
        restLength = prefix.length() + 1;
    }

    const std::string name = choice.attribute("name").value();

    return std::make_tuple(name, segmentTxt.substr(restLength));
}