#include <stdio.h>
#include <unistd.h>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/cache.h"

int main( int argc, char * argv[] ) {
	leveldb::DB * db;

	leveldb::Options options;
	options.create_if_missing = true;
	options.write_buffer_size = 64*1024*1024;
	options.max_open_files = 100000;
	options.block_cache = leveldb::NewLRUCache( 1 );//read buffer
	options.block_size = 32*1024;
	options.compression = leveldb::kSnappyCompression;

	leveldb::Status status = leveldb::DB::Open( options, "/tmp/testdb", &db );
	assert(status.ok());
	printf( "db open ok\n" );

	std::string value;
	char buf[64] = {0};
	for( int i = 0; i < 1000000; i++ ) {
		snprintf( buf, sizeof(buf), "%d", i );
		leveldb::Status s = db->Get(leveldb::ReadOptions(), buf, &value);
		printf( "db get\n" );
		if (s.ok()) {
			printf( "Get %s ok\n", buf );
		} else {
			printf( "Get %s fail\n", buf );
		}
		sleep(2);
	}

	delete db;
	return 0;
}
