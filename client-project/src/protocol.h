/*
 * protocol.h
 *
 * Client header file
 * Definitions, constants and function prototypes for the client
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// Shared application parameters
#define SERVER_PORT 27015  // Server port (change if needed)
#define BUFFER_SIZE 512    // Buffer size for messages

#define MAX_CITY_LEN 64

//Prototipi
void StampaInterfaccia(char* IP);

//Strutture Dati
typedef struct {
    char type;
    char city[MAX_CITY_LEN];
} weather_request_t;

typedef struct {
    unsigned int status;
    char type;
    float value;
} weather_response_t;

#endif /* PROTOCOL_H_ */
