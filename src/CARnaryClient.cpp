//
// Created by carlostojal on 13-09-2023.
//

#include <carnary/CARnaryClient.h>

namespace carnary::client {
    int CARnaryClient::tryConnect(std::uint8_t numTries) {
        // negotiation socket file descriptor
        int negotiationfd;

        // heartbeat socket file descriptor
        int heartbeatfd;

        for(std::uint8_t n = 0; n < numTries; n++) {
            // create the socket
            try {
                negotiationfd = carnary::lib::Utils::createClientSocket("127.0.0.1", DAEMON_TCP_NEGOTIATION_PORT,
                                                                        lib::TCP_SOCKET);
            } catch (std::runtime_error &ex) {
                // if a trouble happened, try again
                continue;
            }

            // if execution reaches this point, the socket was created and connected
            return negotiationfd;
        }

        // if execution reaches here, the for loop reached its end, which means the number of tries was exceeded
        throw std::runtime_error("Exceeded number of tries. Daemon may be down.");
    }

    void CARnaryClient::negotiate(int daemonfd, const std::string &serviceName, std::uint16_t minHeartbeatRate,
                                  pid_t myPID) {

        // TODO
    }

    void CARnaryClient::cleanup() {
        // TODO
    }

} // carnary::client