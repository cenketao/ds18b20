
LIBNAME=mycrypto
SQLITE3=sqlite3
SQLITEPATH=`pwd`/../../usr/local/lib/
SQLITEINCLUDE=`pwd`/../../usr/local/include/
LIBPATH=`pwd`/../src/
LIBINCLUDE=`pwd`/../src/

all:run

run:
	gcc *.c -l${LIBNAME} -L${LIBPATH} -I${LIBPATH} -l${SQLITE3} -L${LIBPATH} -I${LIBINCLUDE}

