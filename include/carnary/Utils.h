//
// Created by carlostojal on 12-09-2023.
//

#ifndef CARNARY_SERVER_UTILS_H
#define CARNARY_SERVER_UTILS_H

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

namespace carnary::lib {

    enum socket_type_t {
        TCP_SOCKET,
        UDP_SOCKET
    };

    class Utils {

    public:

        /*! \brief Create a server socket.
         *
         * @param port The port to listen on.
         * @param type The type of socket (TCP_SOCKET/UDP_SOCKET).
         * @return The socket file descriptor.
         */
        static int createServerSocket(int port, socket_type_t type);

        /*! \brief Create a client socket.
         *
         * @param ipAddress The IP address to connect to.
         * @param port The port to connect to.
         * @param type The type of socket (TCP_SOCKET/UDP_SOCKET).
         * @return The socket file descriptor.
         */
        static int createClientSocket(const std::string& ipAddress, int port, socket_type_t type);

        /*! \brief Create a generic socket (not server nor client).
         *
         * @param type The type of socket (TCP_SOCKET/UDP_SOCKET).
         * @return The socket file descriptor.
         */
        static int createSocket(socket_type_t type);
    };
}

#endif