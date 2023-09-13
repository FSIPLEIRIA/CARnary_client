//
// Created by carlostojal on 12-09-2023.
//

#include <carnary/Utils.h>

namespace carnary::lib {

    int Utils::createServerSocket(int port, carnary::lib::socket_type_t type) {

        int sockfd = -1;

        // create a generic socket
        try {
            sockfd = Utils::createSocket(type);
        } catch(std::runtime_error& ex) {
            throw ex;
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

    int Utils::createClientSocket(const std::string& ipAddress, int port, carnary::lib::socket_type_t type) {

        int sockfd = -1;

        try {
            sockfd = Utils::createSocket(type);
        } catch(std::runtime_error& ex) {
            throw ex;
        }

        struct sockaddr_in serverAddr{};

        // pre-fill the address with zeros
        memset(&serverAddr, 0, sizeof(struct sockaddr_in));

        // fill the address structure
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
        serverAddr.sin_port = htons(port);

        // connect
        if(connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr_in)) < 0) {
            throw std::runtime_error("Error connecting to the server!");
        }

        return sockfd;
    }

    int Utils::createSocket(socket_type_t type) {

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

        return sockfd;
    }
}
