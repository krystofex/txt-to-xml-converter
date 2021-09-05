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
    for (int i = 0; i < text.length(); i++)
        if (text[i] == '\n')
            text.erase(text.begin() + i);
    return text;
};
