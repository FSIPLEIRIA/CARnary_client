//
// Created by carlostojal on 12-09-2023.
//

#include "Utils.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

int carnary::server::Utils::createSocket(int port, socket_type_t type) {

    // create the socket file descriptor:
    // AF_INET means IPv4
    // SOCK_STREAM means TCP. To use UDP, would use SOCK_DGRAM instead
    // 0 is the protocol value for IP
    int sockfd = socket(AF_INET, type == TCP_SOCKET ? SOCK_STREAM : SOCK_DGRAM, 0);

    // check the socket creation for errors
    if (sockfd == -1) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error creating the negotiation file descriptor");
    }

    struct sockaddr_in serverAddr{};

    // pre-fill the address with zeros
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));

    // fill the address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    // bind the socket to the address
    if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in)) < 0) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error binding the address to the socket");
    }

    // listen to uncoming clients
    if (listen(sockfd, 20) < 0) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error listening for clients");
    }

    return sockfd;
}
