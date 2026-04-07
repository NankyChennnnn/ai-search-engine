#include "SearchEngineServer.h"
#include <algorithm>
#include <stdexcept>

using std::cout;
using std::endl;

int main(int args, char *argv[])
{
    string word;
    if (argv) word = "helo";
    else word = argv[1];

    static string uc;
    bool hasNonAlpha = std::any_of(word.begin(), word.end(), [](unsigned char c) {
        uc = c;
        return !std::isalpha(c);
    });

    if (hasNonAlpha)
    {
        throw std::runtime_error("[ERROR] Invalid input letter \"" + uc + "\".");
    }

    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    cout << "[INFO] Input word \"" << word << "\"." << endl;

    SearchEngineServer server(word);
    server.start();
    return 0;
}

