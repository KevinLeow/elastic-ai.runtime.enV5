#ifndef ENV5_NETWORK_CONFIGURATION_H
#define ENV5_NETWORK_CONFIGURATION_H

#include "MQTTBroker.h"
#include "Network.h"

/**
 * Be aware that special characters ned
 *
 */
NetworkCredentials_t NetworkCredentials = {.ssid = "ES-Stud", .password = "curjeq343j"};

MQTTHost_t MQTTHost = {.ip = "192.168.203.99", .port = "1883"};

#endif // ENV5_NETWORK_CONFIGURATION_H