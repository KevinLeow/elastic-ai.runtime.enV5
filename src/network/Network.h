#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char *ssid;
    char *password;
} NetworkCredentials_t;

/*! \brief tries to connect to network until successful
 *
 * Tries to connect to the network until successful. Is successful if there is already a connection.
 *
 * \param credentials network credentials (SSID, password)
 */
void network_ConnectToNetworkUntilConnected(NetworkCredentials_t credentials);

/*! \brief tries to connect to network
 *
 * Tries to connect to the host broker. Is successful if there is already a connection.
 *
 * \param credentials network credentials (SSID, password)
 * \return true if connection successful or already connected, otherwise false
 */
bool network_ConnectToNetwork(NetworkCredentials_t credentials);

void network_DisconnectFromNetwork(void);

void network_checkConnection(void);

#endif /* NETWORK_H */
