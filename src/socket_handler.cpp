//
// Created by Hayden Roszell on 1/3/22.
//

#include <chrono>
#include <thread>
#include <stdexcept>
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include "socket_handler.h"
#include "frame_destructor.h"
#include "telemetry_handler.h"
#include "frame_constructor.h"
#include "spdlog/spdlog.h"

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

#define SOCKET_BUF_SZ 1024

socket_handler::socket_handler(int port) {
    this->port = port;
    sock = socket_fd = -1;

    SPDLOG_DEBUG("Initializing ESTTS Testbed Socket Server");

    if (0 != open_socket()) {
        SPDLOG_ERROR("Failed to open socket.");
        throw std::runtime_error("Failed to open socket.");
    }
    if (0 != configure_socket()) {
        SPDLOG_ERROR("Failed to configure socket.");
        throw std::runtime_error("Failed to configure socket.");
    }
}

int socket_handler::open_socket() {
    // Creating socket file descriptor
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        SPDLOG_ERROR("Error {} from socket(): {}", errno, strerror(errno));
        return -1;
    }
    SPDLOG_TRACE("Opened socket with fd {}", socket_fd);
    return 0;
}

int socket_handler::configure_socket() {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(socket_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        SPDLOG_ERROR("Error {} from bind(): {}", errno, strerror(errno));
        return -1;
    }
    if (listen(socket_fd, 3) < 0) {
        SPDLOG_ERROR("Error {} from listen(): {}", errno, strerror(errno));
        return -1;
    }
    SPDLOG_INFO("Socket listening on port {}", port);
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
    if (sock < 0){
        return -1;
    }
    ssize_t written;
    written = send(sock, data, size, 0);
    if (written < 1) {
        return -1;
    }

    SPDLOG_TRACE("Wrote '{}' (size={}) to {}", data, written, port);
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
    try {
        auto r = read(sock, buf, estts::ti_socket::TI_SOCKET_BUF_SZ);
        if (r < 1) {
            // Can't receive a negative number of bytes ;)
            return nullptr;
        }
        // Add null terminator at the end of transmission for easier processing by parent class(s)
        buf[r] = '\0';
        SPDLOG_TRACE("Read '{}' from {}", buf, port);
        return buf;
    } catch (const std::exception &e) {
        SPDLOG_ERROR("Failed to write to socket: {}", e.what());
    }
    return nullptr;
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
    for (;;) {
start:
        if (estts::ES_OK != await_connection()) {
            return;
        }

await:  // If this returns, we have a session. If it doesn't, client probably seg faulted :/
        if (estts::ES_OK != await_session())
            goto start;

        auto resp = handle_session();
        if (resp == estts::ES_SESSION_CLOSED) {
            SPDLOG_INFO("Closing connection");
            shutdown(sock, SHUT_WR);
            goto start;
        }
        else if (resp == estts::ES_OK)
            goto await;
    }
}

socket_handler::~socket_handler() {
    close(socket_fd);
}

estts::Status socket_handler::transmit_telem(const std::vector<estts::estts_telemetry *>& telem) const {
    for (auto &i: telem) {
        auto constructor = new frame_constructor(i);
        auto frame = constructor->construct_ax25();
        if (estts::ES_OK != this->write_socket_s(frame)) {
            return estts::ES_UNSUCCESSFUL;
        }
        delete i;
        sleep_until(system_clock::now() + milliseconds (200));
    }
    return estts::ES_OK;
}

estts::Status socket_handler::await_session() {
    std::string buffer;
    do {
        SPDLOG_DEBUG("No handshake currently present, sending stream.");
        auto temp_telem = telemetry_handler();
        auto frames = temp_telem.grab_telem_stream();

        if (estts::ES_OK != transmit_telem(frames))
            SPDLOG_ERROR("Stream transmission failed");

        SPDLOG_TRACE("Stream transmission finished");
        sleep_until(system_clock::now() + seconds(5));
    }
    while ((buffer = read_socket_s()).empty());

    SPDLOG_TRACE("Got {}", buffer);
    if (buffer != estts::ax25::NEW_SESSION_FRAME) {
        SPDLOG_WARN("Error on client side.");
        SPDLOG_INFO("Closing connection");
        shutdown(sock, SHUT_WR);
        return estts::ES_UNSUCCESSFUL;
    }
    write_socket_s(buffer);
    SPDLOG_TRACE("Handshake complete");
    return estts::ES_OK;
}

estts::Status socket_handler::handle_session() {
    while (true) {
        std::string raw;
        int elapsed = 0;
        do {
            if (elapsed >= estts::ti_socket::MAX_RETRIES) {
                // If we don't receive an end frame within the max wait time, don't exit return back to sending telemetry
                SPDLOG_WARN("Didn't receive a packet within max wait time ({} sec). Closing connection.", estts::ti_socket::WAIT_TIME_SEC * estts::ti_socket::MAX_RETRIES);
                close(sock);
                return estts::ES_OK;
            }
            elapsed++;
            sleep_until(system_clock::now() + seconds(estts::ti_socket::WAIT_TIME_SEC));
        }
        while ((raw = read_socket_s()).empty());

        if (raw == estts::ax25::END_SESSION_FRAME) {
            // If we get an end session frame, we don't want the socket to close.
            write_socket_s(raw);
            return estts::ES_OK;
        } else if (raw == "close") {
            // If we get close, ESTTS called the session handler destructor
            return estts::ES_SESSION_CLOSED;
        }

        auto command_destructor = new frame_destructor(raw);
        auto command = command_destructor->destruct_ax25();
        delete command_destructor;

        auto telem_handle = new telemetry_handler(command);
        auto telem = telem_handle->process_command();
        delete telem_handle;

        transmit_telem(telem);
    }
}

estts::Status socket_handler::await_connection() {
    int addrlen = sizeof(address);
    SPDLOG_INFO("Waiting for new connections");
    if ((sock = accept(socket_fd, (struct sockaddr *) &address,
                       (socklen_t *) &addrlen)) < 0) {
        printf("Error %d from accept(): %s", errno, strerror(errno));
        return estts::ES_UNSUCCESSFUL;
    }
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    // ESTTC socket handler should send 'flush'
    do {} while (read_socket_s().empty());

    return estts::ES_OK;
}
