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
            this->daemonfd = negotiationfd;
            this->connectionDone = true;
            return negotiationfd;
        }

        // if execution reaches here, the for loop reached its end, which means the number of tries was exceeded
        throw std::runtime_error("Exceeded number of tries. Daemon may be down.");
    }

    int CARnaryClient::negotiate(int daemonfd, const std::string &serviceName, std::uint16_t minHeartbeatRate) {

        // can't negotiate if not connected
        if(!this->connectionDone) {
            throw std::runtime_error("Can't negotiate without connecting first!");
        }

        // create the negotiation
        this->negotiation.systemPID = getpid();
        this->negotiation.serviceName = serviceName;
        this->negotiation.minHeartbeatRate = minHeartbeatRate;

        // provide the negotiation to the daemon
        if(send(daemonfd, &(this->negotiation), sizeof(struct negotiation_t), 0) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error providing the negotiation to the daemon!");
        }

        // receive the updated negotiation, containing the allocated watcher port
        if(recv(daemonfd, &(this->negotiation), sizeof(struct negotiation_t), MSG_TRUNC | MSG_WAITALL) < 0) {
            // reply with a nack
            std::uint8_t res = (std::uint8_t) WATCHER_NACK;
            if(send(daemonfd, &res, sizeof(std::uint8_t), 0) < 0) {
                std::cerr << strerror(errno) << std::endl;
                throw std::runtime_error("Error replying to the daemon with a NACK.");
            }
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error receiving the negotiation from the daemon!");
        }

        // reply with an ACK
        std::uint8_t res = (std::uint8_t) WATCHER_ACK;
        if(send(daemonfd, &res, sizeof(std::uint8_t), 0) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error replying to the daemon with an ACK.");
        }

        // connect to the watcher
        try {
            this->watcherfd = lib::Utils::createClientSocket("127.0.0.1", this->negotiation.monitoringPort, lib::TCP_SOCKET);
        } catch(std::runtime_error& ex) {
            throw ex;
        }

        this->negotiationDone = true;

        return this->watcherfd;
    }

    void CARnaryClient::ping() {

        // can't ping if not negotiated
        if(!this->negotiationDone) {
            throw std::runtime_error("Can't ping without negotiating!");
        }

        // send an heartbeat
        std::uint8_t val = KEEPALIVE;
        if(send(this->watcherfd, &val, sizeof(std::uint8_t), 0) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error sending the heartbeat!");
        }

        // receive the acknowledgement
        if(recv(this->watcherfd, &val, sizeof(std::uint8_t), MSG_WAITALL | MSG_TRUNC) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error receiving acknowledgement from the watcher!");
        }

        if(val != WATCHER_ACK) {
            // something wrong happened on the watcher side
            throw std::runtime_error("Something wrong happened on the watcher!");
        }
    }

    void CARnaryClient::emergency() {

        // can't enter emergency if not negotiated
        if(!this->negotiationDone) {
            throw std::runtime_error("Can't signal emergency without negotiating!");
        }

        // send an emergency heartbeat
        std::uint8_t val = PANIC;
        if(send(this->watcherfd, &val, sizeof(std::uint8_t), 0) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error sending the emergency heartbeat!");
        }

        // receive the acknowledgement
        if(recv(this->watcherfd, &val, sizeof(std::uint8_t), MSG_TRUNC | MSG_WAITALL) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error receiving the acknowledgement from the watcher!");
        }

        if(val != WATCHER_ACK) {
            // something wrong happened on the watcher side
            throw std::runtime_error("Something wrong happened on the watcher!");
        }

        // SIGTERM the daemon
        if(kill(this->negotiation.daemonPID, SIGTERM) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Error signaling the daemon to enter emergency!");
        }
    }

    void CARnaryClient::cleanup() {
        // TODO
    }

} // carnary::client