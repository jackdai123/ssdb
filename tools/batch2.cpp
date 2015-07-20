#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#include "net/link.h"
#include "net/fde.h"

void bench( const char * ip, int port, int num, const char * begin_time, const char * end_time, const char * id ) {
	Fdevents * fdes = new Fdevents();
	std::vector<Link *> * free_links = new std::vector<Link *>();

	for ( int i = 0; i < num; i++ ) {
		Link * link = Link::connect( ip, port );
		if (!link) {
			fprintf(stderr, "connect error! %s\n", strerror(errno));
			exit(0);
		}
		fdes->set( link->fd(), FDEVENT_IN, 0, link );
		free_links->push_back( link );
	}

	int total = atoi(end_time) - atoi(begin_time);
	int num_send = 0;
	int finished = 0;
	char key[64] = {0};

	while ( true ) {
		while(!free_links->empty()){
			if(num_send == total){
				break;
			}
			Link *link = free_links->back();
			free_links->pop_back();
			snprintf( key, sizeof(key), "%d:%s", atoi(begin_time)+num_send, id );
			link->send( "get", key );
			link->flush();
			num_send++;
		}

		const Fdevents::events_t *events;
		events = fdes->wait(50);
		if(events == NULL){
			printf("events.wait error: %s", strerror(errno));
			break;
		}

		for(int i=0; i<(int)events->size(); i++){
			const Fdevent *fde = events->at(i);
			Link *link = (Link *)fde->data.ptr;

			int len = link->read();
			if(len <= 0){
				printf("fd: %d, read: %d, delete link", link->fd(), len);
				exit(0);
			}

			const std::vector<Bytes> *resp = link->recv();
			if(resp == NULL){
				printf("error");
				break;
			}else if(resp->empty()){
				continue;
			}else{
				if(resp->at(0) != "ok"){
					printf("bad response: %s", resp->at(0).String().c_str());
					exit(0);
				}
				free_links->push_back(link);
				if(++finished == total){
					return;
				}
			}
		}
	}

	for ( int i = 0; i < num; i++ ) {
		(*free_links)[i]->close();
	}
	delete free_links;
	delete fdes;
}

int main(int argc, char **argv){
	const char * ip = (argc >= 2)? argv[1] : "127.0.0.1";
	int port = (argc >= 3)? atoi(argv[2]) : 8888;
	int concurrent_sum = atoi(argv[3]);
	const char * begin_time = argv[4];
	const char * end_time = argv[5];
	const char * id = argv[6];

	bench( ip, port, concurrent_sum, begin_time, end_time, id );

	return 0;
}
