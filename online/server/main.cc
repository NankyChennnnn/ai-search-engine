#include "SearchEngineServer.h"

int main(int argc, char *argv[])
{
    string word;
    if (argc < 2) word = "helo";
    else 
    {
        for (int i = 1; i < argc; ++i)
        {
            if (i > 1) word += " ";
            word += argv[i];
        }
    }

    SearchEngineServer server(word);
    server.start();
    return 0;
}

