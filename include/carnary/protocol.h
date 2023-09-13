//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_PROTOCOL_H
#define CARNARY_SERVER_PROTOCOL_H

#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <cstdint>
#include <semaphore.h>

// the TCP port to reserve to the negotiation
#define DAEMON_TCP_NEGOTIATION_PORT 6666

/*! \brief Heartbeat value sent over the watcher socket. */
enum heartbeat_t {
    KEEPALIVE,
    PANIC // when a panic is received, all the services should be signaled to terminate. in redundancy, signals are also used.
};

/*! \brief Negotiation response. */
enum negotiation_res_t {
    ERR_PID_NOT_PROVIDED,
    ERR_CREATING_WATCHER,
    WATCHER_ACK,
    WATCHER_NACK
};

/*! \brief Status of the negotiation. */
enum negotiation_status_t {
    CREATED, // the negotiation was created, but the negotiation was not started yet
    NEGOTIATING, // the negotiation process is occurring
    FAILED_NEGOTIATION, // the negotiation process failed
    NEGOTIATION_OK, // the negotiation is working as expected
    RUNTIME_FAILURE // the negotiation failed during its runtime
};

/*! \brief Daemon failure code (return value). */
enum failure_code_t {
    DAEMON_SUCCESS,
    DAEMON_PROCESS_INIT_FAILURE,
    DAEMON_INIT_FAILURE,
    EMERGENCY
};

/*! \brief The negotiation structure. */
struct negotiation_t {
    pid_t systemPID = -1;
    pid_t watcherPID = -1;
    pid_t daemonPID = -1;

    std::uint8_t monitoringPort;
    std::uint16_t minHeartbeatRate;

    std::string serviceName;
    negotiation_status_t status = CREATED;

    struct sockaddr_in clientAddr;

    int daemonWatcherPipe[2];

    sem_t daemonWatcherSemaphore;
};

#endif //CARNARY_SERVER_PROTOCOL_H
