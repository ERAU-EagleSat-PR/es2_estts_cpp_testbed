//
// Created by Hayden Roszell on 1/3/22.
//

#ifndef ESTTS_TI_SOCKET_HANDLER_H
#define ESTTS_TI_SOCKET_HANDLER_H

#include <sys/socket.h>
#include <netinet/in.h>

class socket_handler {
private:
    int socket_fd, sock;
    struct sockaddr_in address;
    int open_socket();
    int configure_socket();

public:
    explicit socket_handler(const char * address);

    [[noreturn]] void handle_communication() const;
};


#endif //ESTTS_TI_SOCKET_HANDLER_H
