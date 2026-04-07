#include "SearchEngineServer.h"

int main(int argc, char *argv[])
{
    string word;
    if (argc < 2) word = "helo";
    else word = argv[1];

    SearchEngineServer server(word);
    server.start();
    return 0;
}

