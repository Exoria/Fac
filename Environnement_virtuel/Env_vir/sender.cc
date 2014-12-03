extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
}

int main(int argc, char **argv) 
{  
	int		sock, n;
	struct addrinfo	hints, *res, *ressave;
    socklen_t longueurAdr;
	struct sockaddr *adrDest;

	// Récupération de l'adresse de destination (@IP+port)
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; // le système choisira IPv4 ou IPv6
	hints.ai_socktype = SOCK_DGRAM; // on veut UDP
	// à la place de localhost on peut mettre le nom de la machine
	// ou son adresse IP (v4 ou v6)
    if ( (n = getaddrinfo("localhost", "8123", &hints, &res)) != 0) {
		printf("erreur getaddrinfo : %s\n", gai_strerror(n));
		return 1;
	}
	ressave = res;

	do { // Construction d'un socket compatible avec cette adresse
		sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sock >= 0)
			break;		// ça a marché
	} while ( (res = res->ai_next) != NULL);

	// aucune adresse n'a fonctionné, errno est positionné par socket()
	if (res == NULL) {
		perror("erreur socket");
		return 1;
	}

	// on alloue puis on recopie l'adresse IP + port de destination
	adrDest = (struct sockaddr*)malloc(res->ai_addrlen);
	memcpy(adrDest, res->ai_addr, res->ai_addrlen);
	longueurAdr = res->ai_addrlen;
	// on libère la mémoire allouée par getaddrinfo
	freeaddrinfo(ressave);
	
	#define BUFFERLEN 256                            
	char buf[BUFFERLEN];        // Tampon pour le message


    struct sockaddr *srcAdr; // Contiendra l'@IP et le port de l'?metteur
    srcAdr = (struct sockaddr*) malloc(longueurAdr);
    bzero((char *)srcAdr, longueurAdr);  // init. ? 0

    char machine[NI_MAXHOST];
    char service[NI_MAXSERV];

    while(1)
    {
        sprintf(buf, "!!!");  // On écrit le message dans le tampon
        // Envoie le message
        if (sendto(sock, buf, strlen(buf) + 1, 0, adrDest, longueurAdr) <0) {
              perror("sendto");
              return 1;
        }

        // Exemple avec un seul socket :
        fd_set fdset;
        struct timeval date;
        // attendre au max pendant
        // 50 ms = 50000 us
        date.tv_sec = 0;
        date.tv_usec = 500000;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        while(date.tv_usec > 0)
        {
            // un message est-il arrivé ?
            if (select(sock+1, &fdset, NULL, NULL, &date) > 0)
            {
                if (recvfrom(sock, buf, sizeof(buf), 0, srcAdr, &longueurAdr) < 0) {
                    perror("recvfrom");
                    return 1;
                }

                // on essaie de récupérer l'adresse IP et le port de l'émetteur
                if((n = getnameinfo(srcAdr, longueurAdr, machine, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV)) == 0)
                    printf("recu : %s depuis %s:%s\n",buf, machine, service);
                else {
                    printf("recu : %s\n",buf);
                    printf("erreur getnameinfo : %s\n", gai_strerror(n));
                }
            }
        }
    }
	
    return 0;
 }
