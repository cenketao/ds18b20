APP=myapp
LIBNAME=mycrypto
SQLITE3=sqlite3
SQLITEPATH=`pwd`/../../usr/local/lib/
SQLITEINCLUDE=`pwd`/../../usr/local/include/
LIBPATH=`pwd`/../src/
LIBINCLUDE=`pwd`/../src/

all:run

run:
	gcc project.c -o ${APP} -l${LIBNAME} -L${LIBPATH} -I${LIBPATH} -lsqlite3 -L${LIBPATH} -I${LIBINCLUDE}

