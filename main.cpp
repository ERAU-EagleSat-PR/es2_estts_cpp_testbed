#include <iostream>
#include "socket_handler.h"

int main() {
    auto socket = new socket_handler("172.0.0.1");
    socket->handle_communication();
}
