//
// Created by carlostojal on 12-09-2023.
//

#ifndef CARNARY_SERVER_UTILS_H
#define CARNARY_SERVER_UTILS_H

enum socket_type_t {
    TCP_SOCKET,
    UDP_SOCKET
};

namespace carnary::server {
    class Utils {

    public:
        /*! \brief Create a socket.
         *
         * @param port The port to use.
         * @param type The type of socket (TCP_SOCKET/UDP_SOCKET)
         * @return The socket file descriptor
         */
        static int createSocket(int port, socket_type_t type);
    };
}

#endif