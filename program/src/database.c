/*********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  database.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(05/05/23)
 *         Author:  Cenketao <1481155734@qq.com>
 *      ChangeLog:  1, Release initial version on "05/05/23 16:08:01"
 *                 
 ********************************************************************************/

#include"database.h"
#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include"logger.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;

	for(i=0; i<argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

static sqlite3    *db=NULL;

int sqlite_init(char *db_name)
{
	char *zErrMsg = NULL;
	int  rc;
	char sql[128];

	if(!db_name)
	{
		log_error("ERROR:Invalid input arguments\n");
		return -1;
	}

	/* Open database */
	rc = sqlite3_open(db_name, &db);

	if(rc!=SQLITE_OK )
	{
		log_error( "Can't open database: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -2;
	}

	memset(sql,0,sizeof(sql));
	snprintf(sql,sizeof(sql),"CREATE TABLE IF NOT EXISTS TEMPERATURE(ID INTEGER PRIMARY KEY AUTOINCREMENT,NAME CHAR(20),TEMPERATURE CHAR(20),TIME CHAR(20));");
	/* Create SQL statement */

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		log_warn("Create table error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -3;
	}

	else
	{
		log_info( "Table created successfully\n");

		return 0;
	}
}


int sqlite_insert( message_s pack_info)
{
	char  sql[512] = {0};
	int   rc      = -1;
	char  *zErrMsg = 0;


	memset(sql,0,sizeof(sql));
	sprintf(sql,"INSERT INTO TEMPERATURE VALUES (NULL,'%s','%f','%s');",pack_info.name,pack_info.temp,pack_info.time1);
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		log_error( "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		log_info( "Table insert  successfully");
	}
	return 0;

}


int  get_table(char *send_buf)
{
	char *zErrMsg = 0;
	int  rc;
	char **dbResult;
	int  nRow = 0;
	int  nColumn = 0;//查出的总行数，存储列数
	char sql[128]="SELECT *from TEMPERATURE LIMIT 1";
	/* Open database */

	/* Create SQL statement */

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql,&dbResult, &nRow,&nColumn, &zErrMsg);
	if( rc != SQLITE_OK ){
		log_error( "get table data failure: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	else
	{
		log_info("Table select successfully\n");
	}
	snprintf(send_buf,128,"%s/%s/%s",dbResult[5],dbResult[6],dbResult[7]);

	return 0;
}
int sqlite_delect(void)
{
	char sql[128] = {0};
	int  rc = -1;
	char *zErrMsg = NULL;


	memset(sql,0,sizeof(sql));
	snprintf(sql,sizeof(sql),"DELETE FROM TEMPERATURE WHERE rowid IN (SELECT rowid FROM TEMPERATURE LIMIT 1);");
	rc  = sqlite3_exec(db,sql,callback,0,&zErrMsg);

	if(rc != SQLITE_OK)
	{
		log_error("Sqlite_delete_data error:%s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	log_info("Delete first data successfully!\n");
	return 0;
}
int sqlite_close(void)
{
	char        *zErrMsg = NULL;

	if( SQLITE_OK != sqlite3_close(db) )
	{
		log_error("Error close database: %s\n", zErrMsg );
		sqlite3_free(zErrMsg);
	}
	log_info("successfully close database\n");
	return 0;
}



