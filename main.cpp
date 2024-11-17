#include "Server.h"
#include <iostream>
#include <string.h>

using namespace std;

static int port = 8080;
static int workerCount = 10;

void configureParameters(int argc, char* argv[]) {
    // 서버 옵션 추출
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--port=", 7) == 0) {
            port = stoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--worker=", 9) == 0) {
            workerCount = stoi(argv[i] + 9);
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        configureParameters(argc, argv);

        Server server(port, workerCount);
        server.run();
    } catch (const exception& ex) {
        cerr << "[Error]: " << ex.what() << endl;
        return 1;
    }
    return 0;
}


