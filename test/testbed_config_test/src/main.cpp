#include <iostream>
#include "socket_handler.h"
#include "spdlog/spdlog.h"

int main() {
    spdlog::set_level(spdlog::level::trace);
    auto socket = new socket_handler("172.0.0.1");
    socket->handle_communication();
}