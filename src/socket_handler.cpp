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
#include "frame_destructor.h"
#include "telemetry_handler.h"
#include "frame_constructor.h"
#include "spdlog/spdlog.h"

#define __TI_SOCKET_PORT__ 8080
#define SOCKET_BUF_SZ 1024

socket_handler::socket_handler(const char * address) {
    port = __TI_SOCKET_PORT__;
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

    std::cout << "Socket listening on port " << port << std::endl;
    return 0;
}

/**
 * @brief Takes argument for a pointer to an unsigned char
 * and transmits it across the open socket.
 * @param data Unsigned char * containing bytes to be written
 * @param size Size of data being transmitted
 * @return Returns -1 if write failed, or the number of bytes written if call succeeded
 */
ssize_t socket_handler::write_socket_uc(unsigned char *data, int size) const {
    // If serial port isn't open, error out
    if (sock < 0) {
        return -1;
    }
    ssize_t written = send(sock, data, size, 0);
    if (written < 1) {
        return -1;
    }
    spdlog::trace("Wrote '{}' (size={}) to {}", data, written, port);
    return written;
}

/**
 * @brief Reads available data from open socket
 * @return Returns nullptr if nothing was read, or the data read
 * from socket if read was successful (and data was available).
 *
 * CRITICAL NOTE: delete MUST be called when done with the value returned. If this
 * is not done, a memory leak will be created. To avoid this issue, use read_socket_s
 */
unsigned char *socket_handler::read_socket_uc() const {
    // If socket isn't open, error out
    if (sock < 0) {
        return nullptr;
    }
    // Allocate heap space for receive buffer
    auto buf = new unsigned char[estts::ti_socket::TI_SOCKET_BUF_SZ];
    // Use read system call to read data in sock to buf
    auto r = read(sock, buf, estts::ti_socket::TI_SOCKET_BUF_SZ);
    if (r < 1) {
        // Can't receive a negative number of bytes ;)
        return nullptr;
    }
    // Add null terminator at the end of transmission for easier processing by parent class(s)
    buf[r] = '\0';
    spdlog::trace("Read '{}' from {}", buf, port);
    return buf;
}

/**
 * @brief Writes string to open socket
 * @param data String argument
 * @return Number of bytes transferred across open socket
 */
estts::Status socket_handler::write_socket_s(const std::string &data) const {
    // If serial port isn't open, error out
    if (sock < 0) return estts::ES_UNINITIALIZED;
    // Cast string to const unsigned char *, then cast away const to pass
    // to method that writes unsigned char
    auto csc_data = const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(data.c_str()));
    if (this->write_socket_uc(csc_data, (int) data.length()) < 0) return estts::ES_UNSUCCESSFUL;
    return estts::ES_OK;
}

/**
 * @brief Reads available data from socket and returns data as string
 * @return Returns translated string of received data
 */
std::string socket_handler::read_socket_s() const {
    // If serial port isn't open, error out
    if (sock < 0) {
        return "";
    }
    // Read serial data
    auto read = this->read_socket_uc();
    if (read == nullptr) {
        return "";
    }
    // Type cast unsigned char (auto) to a char *
    // Then call std::string constructor
    std::string string_read(reinterpret_cast<char const *>(read));
    delete read;
    return string_read;
}


[[noreturn]] void socket_handler::handle_communication() {
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    for ( ; ; ) {
        int addrlen = sizeof(address);
        char buffer[SOCKET_BUF_SZ] = {0};
        if ((sock = accept(socket_fd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen))<0)
        {
            printf("Error %d from accept(): %s", errno, strerror(errno));
            return;
        }
        spdlog::trace("Waiting for handshake");
        read( sock , buffer, SOCKET_BUF_SZ);
        spdlog::trace("Got {}", buffer );
        send(sock , buffer , strlen(buffer) , 0 );
        spdlog::trace("Handshake complete");

        std::string raw;
        while ((raw = read_socket_s()).empty()) {
            break;
        }

        printf("%s\n",raw.c_str());
        // sleep_until(system_clock::now() + seconds(2));

        auto command_destructor = new frame_destructor(raw);
        auto command = command_destructor->destruct_ax25();
        delete command_destructor;

        auto telem_handle = new telemetry_handler(command);
        auto telem = telem_handle->process_command();
        delete telem_handle;

        for (auto & i : telem) {
            auto constructor = new frame_constructor(i);
            auto frame = constructor->construct_ax25();
            this->write_socket_s(frame);
        }
        close(sock);
    }
}
