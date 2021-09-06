#include <sstream>
#include <fstream>

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
    return text;
};

void SaveTxt(const std::string input, const std::string location)
{
    std::ofstream outputFile;

    outputFile.open(location);
    outputFile << input;
    outputFile.close();
}