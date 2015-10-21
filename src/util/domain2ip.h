#ifndef UTIL_DOMAIN2IP_H
#define UTIL_DOMAIN2IP_H

#include <string.h>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>

inline static
int domain2ip(const std::string & domain, std::string & ip)
{
	struct addrinfo *answer, hint, *curr;
	char ipstr[16] = {0};
	bzero(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	int ret = getaddrinfo(domain.c_str(), NULL, &hint, &answer);
	if (ret != 0) {
		return -1;
	}

	for (curr = answer; curr != NULL; curr = curr->ai_next) {
		inet_ntop(AF_INET, &(((struct sockaddr_in *)(curr->ai_addr))->sin_addr), ipstr, 16);
		ip = ipstr;
	}

	freeaddrinfo(answer);
	return 0;
}

#endif
