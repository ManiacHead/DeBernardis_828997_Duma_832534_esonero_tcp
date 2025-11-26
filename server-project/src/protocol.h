/*
 * protocol.h
 *
 * Server header file
 * Definitions, constants and function prototypes for the server
 */
#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// Shared application parameters
#define SERVER_PORT 27015  // Server port (change if needed)
#define BUFFER_SIZE 512    // Buffer size for messages
#define QUEUE_SIZE 5       // Size of pending connections queue


#define QLEN 6
#define NO_ERROR 0

#define MAX_CITY_LEN 64


void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

//Strutture Dati
typedef struct {
    char type;              // 't', 'h', 'w', 'p'
    char city[MAX_CITY_LEN]; // Nome città
} weather_request_t;

typedef struct {
    unsigned int status;  // Codice di stato
    char type;            // Echo del tipo richiesto
    float value;          // Valore del dato meteo
} weather_response_t;


//Prototipi funzioni
int is_supported_city(const char* city);
float get_temperature(void);    // -10.0 / 40.0 °C
float get_humidity(void);       // 20.0 / 100.0 %
float get_wind(void);           // 0.0 / 100.0 km/h
float get_pressure(void);       // 950.0 / 1050.0 hPa


#endif /* PROTOCOL_H_ */
