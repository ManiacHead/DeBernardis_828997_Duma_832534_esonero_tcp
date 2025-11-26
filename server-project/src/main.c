/*
 * main.c
 *
 * TCP Server - Template for Computer Networks assignment
 *
 * This file contains the boilerplate code for a TCP server
 * portable across Windows, Linux and macOS.
 */

//Date Modified: 26 nov 2025

#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include <time.h>


int port;

const char* supported_cities[] = {"Bari","Roma","Milano","Napoli","Torino","Palermo","Genova","Bologna","Firenze","Venezia"};


int main(int argc, char *argv[]) {
	srand(time(NULL));
	if (argc == 3 && strcmp(argv[1], "-p") == 0){
		port = atoi(argv[2]);
	}
	else
	{
		port = SERVER_PORT;
	}

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (my_socket < 0) {
		printf("socket creation failed.\n");
		return -1;
	}

	//Assegazione indirizzo
	struct sockaddr_in sock_add;
	sock_add.sin_family = AF_INET;
	sock_add.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sock_add.sin_port = htons(port);

	if(bind(my_socket, (struct sockaddr *)&sock_add, sizeof(sock_add)) < 0)
	{
		printf("bind() failed.\n");
		closesocket(my_socket);
		return -1;
	}

	//Listening Initialization
	if (listen (my_socket, QLEN) < 0) {
		printf("listen() failed.\n");
		closesocket(my_socket);
		return -1;
	}

	//Main Server Loop
	struct sockaddr_in client_add;
	int client_socket;
	int client_len;
	printf( "Waiting for a client to connect...\n");
	while (1) {
	client_len = sizeof(client_add); //set the size of the client address
	if ((client_socket=accept(my_socket, (struct sockaddr *)&client_add, &client_len)) < 0 )
	{
		printf("accept() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}

	// Ricezione dati dal client
	weather_request_t request;
	int n = recv(client_socket, (char*)&request, sizeof(request), 0);
	if (n <= 0) {
		printf("Errore di ricevimento dal client\n");
		closesocket(client_socket);
	}
	printf("Richiesta '%c %s' dal client IP %s\n", request.type, request.city, inet_ntoa(client_add.sin_addr));

	// Preparazione ed elaborazione risposta
	weather_response_t response;
	memset(&response, 0, sizeof(response));

	if (request.type != 't' && request.type != 'h' && request.type != 'w' && request.type != 'p'){
		response.status = 2;
	}
	else if (!is_supported_city(request.city)) {
		response.status = 1;
	}
	else {
		response.status = 0;
		response.type = request.type;
		switch (request.type) {
		case 't':
			{
				response.value = get_temperature();
				break;
			}
		case 'h':
			{
				response.value = get_humidity();
				break;
			}
		case 'w':
			{
				response.value = get_wind();
				break;
			}
		case 'p':
			{
				response.value = get_pressure();
				break;
			}
		}
	}

	// Invio dati al Client
	if (send(client_socket, (char*)&response, sizeof(response), 0) != sizeof(response))
	{
		printf("Errore: messaggio di risposta non recapitato.\n");
	}

	closesocket(client_socket);
	}
	printf("Server terminated.\n");

	closesocket(my_socket);
	clearwinsock();
	return 0;
}


int is_supported_city(const char* city) {
    for (int i = 0; i < 10; i++) {
        const char* c = supported_cities[i];
        int match = 1;

        size_t len_c = strlen(c);
        size_t len_city = strlen(city);
        if (len_c != len_city) continue;

        for (size_t j = 0; j < len_c; j++) {
            if (tolower(c[j]) != tolower(city[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}


float get_temperature(void)
{
	return -10.0f + (rand() / (float)RAND_MAX) * 50.0f;
}
float get_humidity(void)
{
	return 20.0f + (rand() / (float)RAND_MAX) * 80.0f;
}
float get_wind(void)
{
	return (rand() / (float)RAND_MAX) * 100.0f;
}
float get_pressure(void)
{
	return 950.0f + (rand() / (float)RAND_MAX) * 100.0f;
}
