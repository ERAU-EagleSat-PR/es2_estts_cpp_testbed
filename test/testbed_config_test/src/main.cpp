#include <iostream>
#include "socket_handler.h"

#include "spdlog/spdlog.h"

#define __TI_SOCKET_PORT__ 8080

int main() {
    spdlog::set_level(spdlog::level::trace);
    //spdlog::set_pattern("[source %s] [function %!] [line %#] %v");

    auto socket = new socket_handler(__TI_SOCKET_PORT__);
    socket->handle_communication();
}