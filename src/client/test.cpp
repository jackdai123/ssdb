#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#include "SSDB_client.h"

int main(int argc, char **argv){
	const char * ip = (argc >= 2)? argv[1] : "127.0.0.1";
	int port = (argc >= 3)? atoi(argv[2]) : 8888;
	const char * key_start = argv[3];
	const char * key_end = argv[4];
	const char * id = argv[5];

	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
		printf("fail to connect to server!\n");
		return 0;
	}
	
	std::vector<std::string> ret;
	client->scan_id( key_start, key_end, id, &ret);
	printf( "scan_id ret %u\n", ret.size() );

	delete client;
	return 0;
}
