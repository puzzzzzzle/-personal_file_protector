//
// Created by khalidzhang on 2019/4/2.
//

#ifndef PERSONAL_FILE_PROTECTOR_WEB_ADAPTOR_H
#define PERSONAL_FILE_PROTECTOR_WEB_ADAPTOR_H

#include <vector>
#include <string>

class WebController {
public:
    static WebController *Instance;
public:
//    MainFileController *mainController;
    int status{};
public:

    WebController() = default;

public:
    int startServer(int port = 2333, std::string host = "localhost");

    int stopServer();

    int getStatus() {
        return status;
    }

};

#endif //PERSONAL_FILE_PROTECTOR_WEB_ADAPTOR_H
