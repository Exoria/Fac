/*
 *  server.h IPv6 version
 *
 *  Created by Patrice Torguet on 17/01/12.
 *
 */
#ifndef SERVER6_H
#define SERVER6_H

#include <netinet/in.h>
#include <string.h>

#include <set>
#include <functional>

using namespace std;

struct less_addr : public binary_function<struct sockaddr_in, struct sockaddr_in, bool> {
	bool operator()(const struct sockaddr_in6 i1, const struct sockaddr_in6 i2) {
		// on compare les deux @ IP
		int cmp = memcmp(&i1.sin6_addr, &i2.sin6_addr, sizeof(struct in6_addr));
		if (cmp<0) // @IP i1 < @IP i2
			return 1;
		// si elles sont égales, on compare les ports
		if ((cmp == 0) && // @IP i1 == @IP i2
			(i1.sin6_port < i2.sin6_port)) // port i1 < port i2
			return 1;
		return 0;
	}
};
	
class Server {
	private:
		int sock;
		set<struct sockaddr_in6, less_addr> clients;
		int nextId;
	public:
		Server();
		
		int init();
		
		int manageUpdate(char buf[], int length);
		
		int mainLoop();
};

#endif
