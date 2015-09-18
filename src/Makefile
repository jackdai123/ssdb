include ../build_config.mk

OBJS = proc_kv.o proc_hash.o proc_zset.o proc_queue.o \
	backend_dump.o backend_sync.o slave.o \
	serv.o proc_cluster.o cluster.o cluster_store.o cluster_migrate.o
LIBS = ./ssdb/libssdb.a ./util/libutil.a ./net/libnet.a
EXES = ../ssdb-server ../test_level_writer ../test_level_reader


all: ${OBJS} ssdb-server.o test_level_writer.o test_level_reader.o
	${CXX} -o ../ssdb-server ssdb-server.o ${OBJS} ${LIBS} ${CLIBS} client/SSDB_impl.o
	${CXX} -o ../test_level_writer test_level_writer.o ${CLIBS}
	${CXX} -o ../test_level_reader test_level_reader.o ${CLIBS}

cluster_migrate.o: cluster_migrate.h cluster_migrate.cpp
	${CXX} ${CFLAGS} -I./client -c cluster_migrate.cpp
proc_cluster.o: serv.h proc_cluster.cpp
	${CXX} ${CFLAGS} -c proc_cluster.cpp
cluster.o: cluster.h cluster.cpp
	${CXX} ${CFLAGS} -c cluster.cpp
cluster_store.o: cluster_store.h cluster_store.cpp
	${CXX} ${CFLAGS} -c cluster_store.cpp

ssdb-server.o: ssdb-server.cpp
	${CXX} ${CFLAGS} -c ssdb-server.cpp
test_level_writer.o: test_level_writer.cpp
	${CXX} ${CFLAGS} -c test_level_writer.cpp
test_level_reader.o: test_level_reader.cpp
	${CXX} ${CFLAGS} -c test_level_reader.cpp
slave.o: slave.h slave.cpp
	${CXX} ${CFLAGS} -c slave.cpp
backend_dump.o: backend_dump.h backend_dump.cpp
	${CXX} ${CFLAGS} -c backend_dump.cpp
backend_sync.o: backend_sync.h backend_sync.cpp
	${CXX} ${CFLAGS} -c backend_sync.cpp

proc.o: serv.h proc.cpp
	${CXX} ${CFLAGS} -c proc.cpp
proc_kv.o: serv.h proc_kv.cpp
	${CXX} ${CFLAGS} -c proc_kv.cpp
proc_hash.o: serv.h proc_hash.cpp
	${CXX} ${CFLAGS} -c proc_hash.cpp
proc_zset.o: serv.h proc_zset.cpp
	${CXX} ${CFLAGS} -c proc_zset.cpp
proc_queue.o: serv.h proc_queue.cpp
	${CXX} ${CFLAGS} -c proc_queue.cpp

serv.o: serv.h serv.cpp
	${CXX} ${CFLAGS} -c serv.cpp

clean:
	rm -f ${EXES} *.o *.exe *.a

