#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#include "SSDB_client.h"

pthread_mutex_t lock;
long long sum;
long long process = 0;
const char * ip;
int port;
std::string key2="host";
std::string value="153.28";
time_t iTime = 1436964957;

void * func( void * arg ) {
	long long current_process = 0;
	char buf[1024];
	time_t current_time = 0;

	ssdb::Client *client = ssdb::Client::connect(ip, port);
	if(client == NULL){
		printf("fail to connect to server!\n");
		return 0;
	}
	
	while( true ) {
		pthread_mutex_lock( &lock );
		current_process = process;
		process += 50000;
		current_time = iTime++;
		pthread_mutex_unlock( &lock );

		if ( current_process >= sum ) {
			break;
		}

		struct tm tm_result;
		localtime_r( &current_time, &tm_result );
		std::map<std::string, std::string> kvs;
		for ( int i = 0; i < 50000; i++ ) {
			snprintf( buf, sizeof(buf), "%04d:%02d:%02d:%02d:%s%05d:%02d:%02d", tm_result.tm_year+1900, tm_result.tm_mon+1, tm_result.tm_mday,
					tm_result.tm_hour, key2.c_str(), i, tm_result.tm_min, tm_result.tm_sec );
			if ( i == 49999 ) printf( "%s\n", buf );
			kvs.insert( make_pair(buf, value) );
		}

		client->multi_set( kvs );
	}

	delete client;
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	ip = (argc >= 2)? argv[1] : "127.0.0.1";
	port = (argc >= 3)? atoi(argv[2]) : 8888;
	sum = atoll(argv[3]);
	int pthread_num = atoi(argv[4]);
	std::vector<pthread_t> vecPid;

	pthread_mutex_init( &lock, NULL );

	for ( int i = 0; i < pthread_num; i++ ) {
		pthread_t p_id;
		pthread_create( &p_id, NULL, func, NULL );
		vecPid.push_back( p_id );
	}

	for ( int i = 0; i < pthread_num; i++ ) {
		pthread_join( vecPid[i], NULL );
	}

	return 0;
}
