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
