//
// Created by carlostojal on 13-09-2023.
//

#ifndef CARNARY_LIB_CARNARYCLIENT_H
#define CARNARY_LIB_CARNARYCLIENT_H

#include <carnary/Utils.h>
#include <carnary/protocol.h>

namespace carnary::client {

    /*! \brief Main client implementation. */
    class CARnaryClient {

    private:

        /*! \brief Daemon socket file descriptor. */
        int daemonfd;

        /*! \brief Watcher socket file descriptor. */
        int watcherfd;

        /*! \brief Is the client connected to the daemon? */
        bool connectionDone = false;

        /*! \brief Is the client connected to the watcher? */
        bool negotiationDone = false;

    public:

        /*! \brief Create a server socket.
         *
         * @return Daemon negotiation socket file descriptor.
         */
        int tryConnect(std::uint8_t numTries = 1);

        /*! \brief Start negotiation with the daemon.
         *
         * @param daemonfd Negotiation file descriptor obtained using "tryConnect"
         * @param serviceName Service description.
         * @param minHeartbeatRate Minimum heartbeat rate this system should provide.
         * @param myPID PID of this system, obtained using "getpid()" system call.
         * @return Watcher socket file descriptor.
         */
        int negotiate(int daemonfd, const std::string& serviceName, std::uint16_t minHeartbeatRate);

        /*! \brief Send an heartbeat. */
        void ping();

        /*! \brief Call this if an emergency occurred (camera lost connection, exceptions, etc.) */
        void emergency();

        /*! \brief Close socket file descriptors. */
        void cleanup();
    };

} // carnary::client

#endif //CARNARY_LIB_CARNARYCLIENT_H
