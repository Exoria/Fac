#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "server6.h"

/*
 *  server.h IPv4 version
 *
 *  Created by Patrice Torguet on 14/01/08.
 *
 */

#define EXAMPLE_PORT	"8123"

Server::Server() {
	sock = 0;
	nextId = 0;
}
		
int Server::init() {
	int n;
	struct addrinfo	hints, *res, *ressave;

	// Récupération de l'adresse de liaison (@IP Any + port choisi)
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE; // on est serveur ici
	hints.ai_family = AF_INET6; // on choisit IPv6 (AF_INET pour IPv4)
	hints.ai_socktype = SOCK_DGRAM; // on veut UDP
	// NULL correspond à l'adresse Any (0.0.0.0 pour IPv4 et 0::0 pour IPv6)
	if ( (n = getaddrinfo(NULL, EXAMPLE_PORT, &hints, &res)) != 0) {
		printf("erreur getaddrinfo : %s\n", gai_strerror(n));
		return -1;
	}
	ressave = res;

	do { // on essaie de construire un socket compatible
		sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sock < 0)
			continue;		// ça n'a pas marché on passe à la suivante
		// on essaie de se lier au port
		if (bind(sock, res->ai_addr, res->ai_addrlen) == 0)
			break;			// ça a marché

		close(sock);		// erreur de bind, on ferme le socket
	} while ( (res = res->ai_next) != NULL); // passe à la suivante

	// aucune adresse n'a fonctionné, on s'arrête
	if (res == NULL) {
		perror("erreur bind ou socket");
		return -2;
	}

	// on libère la mémoire
	freeaddrinfo(ressave);
	return 1;
}
		
int Server::manageUpdate(char buf[], int length) {
	for(set<struct sockaddr_in6>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (sendto(sock, buf, length, 0, (struct sockaddr*)&(*it), sizeof(struct sockaddr_in6))<0) {
			perror("sendto");
			return -1;
		}
	}
	return 1;
}
		
int Server::mainLoop() {
	char buf[512];
	struct sockaddr_in6 addr;
	socklen_t addrlen;
	int received;

	while(1) {
		addrlen = sizeof(addr);
		if((received = recvfrom(sock, buf, 512, 0, (struct sockaddr*)&addr, &addrlen))<0) {
			perror("recvfrom");
			return -1;
		}
		//printf("recu %d octets de %s,%d\n", received, inet_ntoa(addr.sin_addr), addr.sin_port);
		// nouveau client ?
		if(clients.find(addr) == clients.end()) {
			clients.insert(addr);
			// doit-on lui envoyer un ID ?
			if(buf[0]=='I') {
				memcpy(buf, &nextId, sizeof(int));
				printf("nouveau client d'id %d\n", nextId);
				nextId++;
				// on lui envoie son ID
				if (sendto(sock, buf, sizeof(int), 0, (struct sockaddr*)&addr, sizeof(addr))<0) {
					perror("sendto");
					return -1;
				}
			}
		}
		if(buf[0]!='I') {
			// on diffuse a tous
			manageUpdate(buf, received);
		}
	}
}

int main() {
	Server s;
	if(s.init())
		if (s.mainLoop())
			return 0;
		else
			return 2;
	else
		return 1;
}
