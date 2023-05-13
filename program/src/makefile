LIBNAME=mycrypto
INSTPATH=`pwd`/../lib/
SQLITEPATH=`pwd`/../../usr/local/lib/
SQLITEINCLUDE=`pwd`/../../usr/local/include/

CC=gcc
AR=ar

all:static_lib
	@make clear
	@make install
static_lib:
	${CC} -c *.c -lsqlite3 -L${SQLITEPATH} -I${SQLITEINCLUDE} 
	${AR} -rcs lib${LIBNAME}.a *.o
install:
	cp -rf lib${LIBNAME}.* ${INSTPATH}
clear:
	rm -rf *.o

