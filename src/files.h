#include <sstream>
#include <fstream>

void ReplaceAll(std::string &source, const std::string &from, const std::string &to)
{
    std::string newString;
    newString.reserve(source.length()); // avoids a few memory allocations

    std::string::size_type lastPos = 0;
    std::string::size_type findPos;

    while (std::string::npos != (findPos = source.find(from, lastPos)))
    {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);

    source.swap(newString);
}

std::string LoadTxtFile(const std::string path)
{
    std::ifstream f(path);
    std::string text;
    if (f)
    {
        std::ostringstream os;
        os << f.rdbuf(); // reading data
        text = os.str();
    }
    ReplaceAll(text, "\r\n", "\n"); // convert line endings from CLRF to LF
    return text;
};

void SaveTxt(const std::string input, const std::string location)
{
    std::ofstream outputFile;

    outputFile.open(location);
    outputFile << input;
    outputFile.close();
}