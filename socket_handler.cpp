//
// Created by Hayden Roszell on 1/3/22.
//

#include <chrono>
#include <thread>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include "socket_handler.h"

#define __TI_SOCKET_PORT__ 8080
#define SOCKET_BUF_SZ 1024

socket_handler::socket_handler(const char * address) {
    if (0 != open_socket()) {
        perror("Failed to open socket.");
        throw std::runtime_error("Failed to open socket.");
    }
    if (0 != configure_socket()){
        perror("Failed to configure socket.");
        throw std::runtime_error("Failed to configure socket.");
    }
}

int socket_handler::open_socket() {
    // Creating socket file descriptor
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error %d from socket(): %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

int socket_handler::configure_socket() {
    int addrlen = sizeof(address);
    char buffer[SOCKET_BUF_SZ] = {0};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( __TI_SOCKET_PORT__ );

    // Forcefully attaching socket to the port 8080
    if (bind(socket_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        printf("Error %d from bind(): %s", errno, strerror(errno));
        return -1;
    }
    if (listen(socket_fd, 3) < 0)
    {
        printf("Error %d from listen(): %s", errno, strerror(errno));
        return -1;
    }

    std::cout << "Socket listening on port " << __TI_SOCKET_PORT__ << std::endl;

    if ((sock = accept(socket_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        printf("Error %d from accept(): %s", errno, strerror(errno));
        return -1;
    }
    printf("Waiting for handshake\n");
    read( sock , buffer, SOCKET_BUF_SZ);
    printf("Got %s. Sending back the same.\n",buffer );
    send(sock , buffer , strlen(buffer) , 0 );
    printf("Handshake complete\n");
    return 0;
}

[[noreturn]] void socket_handler::handle_communication() const {
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    for ( ; ; ) {
        char buffer[SOCKET_BUF_SZ] = {0};
        read( sock , buffer, SOCKET_BUF_SZ);
        printf("%s\n",buffer );
        sleep_until(system_clock::now() + seconds(2));
        send(sock , buffer , strlen(buffer) , 0 );
    }
}
