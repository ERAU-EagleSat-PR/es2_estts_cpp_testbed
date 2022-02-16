//
// Created by Hayden Roszell on 1/3/22.
//

#ifndef ESTTS_TI_SOCKET_HANDLER_H
#define ESTTS_TI_SOCKET_HANDLER_H

#include <sys/socket.h>
#include <vector>
#include <netinet/in.h>
#include "constants.h"

class socket_handler {
private:
    int port;
    int socket_fd, sock;
    struct sockaddr_in address{};

    int open_socket();

    int configure_socket();

public:
    explicit socket_handler(int address);

    [[noreturn]] void handle_communication();

    estts::Status await_connection();

    estts::Status await_session();

    estts::Status handle_session();

    ssize_t write_socket_uc(unsigned char *data, int size) const;

    unsigned char *read_socket_uc() const;

    estts::Status write_socket_s(const std::string &data) const;

    std::string read_socket_s() const;

    estts::Status transmit_telem(const std::vector<estts::estts_telemetry *>& telem) const;

    ~socket_handler();
};


#endif //ESTTS_TI_SOCKET_HANDLER_H
