#include <assert.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <fcgi_stdio.h>
#include "dbaccess.h"

static MYSQL mysql;
static int initialized = 0;

struct postion_manager {

};

static int init_dbconnect()
{
	//int i = getpid();
	char pid[10];
	//sprintf(pid, "%d", i);
	//syslog(LOG_WARNING, pid);

	void *ret;
	if (!initialized) {
		void *ret;
		mysql_init(&mysql);
		ret = mysql_real_connect(&mysql,
					 "127.0.0.1", "tracer",
					 "tracerpass", "traceinfo", 3306,
					 NULL, 0);
		if (NULL == ret) {
			fprintf(stderr, "conncet mysql db error\n");
			return -1;
		}
		initialized = 1;
	}


	return 0;
}


#define MAX_SQL_LENGTH	1024
#define SQL_QUERY_ERROR_TRY 3
int add_position(const char *clientid, const char* time, const char *longitude,
		  const char *latitue, int voltage, char type)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;

	length = sprintf(cmdbuf, "insert into positions values(\"%s\", \"%s\", \"%s\", \"%s\", \"%d\", \"%c\")", clientid, time, longitude, latitue, voltage, type);
	
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);

	return 0;
}

int add_device(const char *ID,const char *Master,const char *Password,const char *Activated)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;


	length = sprintf(cmdbuf, "select Master, Password, Activated from DeviceInfo where ID=\"%s\"", ID);
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
	result = mysql_store_result(&mysql);
	if(result)
	{
		int numrow;
		numrow = mysql_num_rows(result);
		if(numrow == 0)
		{
			length = sprintf(cmdbuf, "insert into DeviceInfo (ID,Master,Password,Activated) values(\"%s\", \"%s\", \"%s\", \"%d\")", ID, Master, Password, 1);
			err = mysql_real_query(&mysql,  cmdbuf, length);
		}
		else
		{
			length = sprintf(cmdbuf, "update DeviceInfo set Master=\"%s\" , Password= \"%s\",Activated=\"%s\" where ID=\"%s\"", Master, Password,Activated, ID);

			err = mysql_real_query(&mysql,  cmdbuf, length);
		}
	}
	else
	{
		err = -1;
	}
	mysql_free_result(result);
//	mysql_close(&mysql);
	return err;									   //
}

int update_LBS(const char *id, const char* mnc, const char *mcc, const char *cid, const char *lac, const char *time)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;


	length = sprintf(cmdbuf, "select Master, Password, Activated from DeviceInfo where ID=\"%s\"", id);
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
	result = mysql_store_result(&mysql);
	if(result)
	{
		int numrow;
		numrow = mysql_num_rows(result);
		if(numrow == 0)
		{
			//mysql_free_result(result);
			//return 1;
			err = -1;
		}
		else
		{
			length = sprintf(cmdbuf, "update DeviceInfo set mnc=\"%s\" , mcc=\"%s\",cid=\"%s\",lac=\"%s\",time=\"%s\" where ID=\"%s\"", mnc, mcc,cid,lac,time,id);

			err = mysql_real_query(&mysql,  cmdbuf, length);
		}
	}
	else
	{
		err = -1;
	}
	mysql_free_result(result);
//	mysql_close(&mysql);
	return err;									   //
}


static void assign_position_data(struct position_data* pd, const MYSQL_ROW row)
{
	if (row[0])
		strcpy(pd->time, row[0]);
	if (row[1])
		strcpy(pd->master, row[1]);
	if (row[2])
		strcpy(pd->longitude, row[2]);
	if (row[3])
		strcpy(pd->latitude, row[3]);
	if (row[4])
		strcpy(pd->bat, row[4]);
	if(row[5])
		strcpy(pd->type, row[5]);
}

static void assign_LBS_data(struct lbs_data* ld, const MYSQL_ROW row)
{
	if (row[0])
		strcpy(ld->time, row[0]);
	if (row[1])
		strcpy(ld->mnc, row[1]);
	if (row[2])
		strcpy(ld->mcc, row[2]);
	if (row[3])
		strcpy(ld->cid, row[3]);
	if (row[4])
		strcpy(ld->lac, row[4]);
}

static void assign_device_data(struct device_data* dd, const MYSQL_ROW row)
{
	if (row[0])
		strcpy(dd->master, row[0]);
	if (row[1])
		strcpy(dd->passworld, row[1]);
	if (row[2])
		strcpy(dd->activated, row[2]);
}

static void  assign_edition_data(struct edition_info* ei, const MYSQL_ROW row)
{
	if(row[0]) {
		strcpy(ei->version, row[0]);
	}
	if(row[1]) {
		strcpy(ei->updatetime, row[1]);
	}
	if(row[2]) {
		strcpy(ei->changes, row[2]);
	}
	if(row[3]) {
		strcpy(ei->url, row[3]);
	}
	if(row[4]) {
		strcpy(ei->name, row[4]);
	}
}

int get_postions(const char* clientid, const char* starttime, const char* endtime, struct position_data *pd, int max_num)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;

	length = sprintf(cmdbuf, "select positions.time, Master, longitude, latituede ,voltage, type from positions, DeviceInfo where ID=clientid and clientid=\"%s\" and positions.time>=\"%s\" and positions.time<=\"%s\" order by time desc limit %d", clientid, starttime, endtime,max_num);
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
	result = mysql_store_result(&mysql);
	if (result) {
		int numrow;
		MYSQL_ROW row;
		int i;
		numrow = mysql_num_rows(result);
		if (numrow > max_num)
			numrow = max_num;
		for (i = 0; i < numrow; ++i) {
			row = mysql_fetch_row(result);
			assign_position_data(&pd[i], row);
		}

		mysql_free_result(result);	
	//	mysql_close(&mysql);
		return numrow;
	}
	//mysql_close(&mysql);
	return -1;
}

int get_last_positon(const char*clientid, struct position_data* pd)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;

	length = sprintf(cmdbuf, "select positions.time, Master, longitude, latituede ,voltage, type from positions ,DeviceInfo where ID=clientid and clientid=\"%s\" order by time desc limit 1", clientid);
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
	result = mysql_store_result(&mysql);
	if (result) {
		int numrow;
		MYSQL_ROW row;
		int i;
		numrow = mysql_num_rows(result);
		//printf("<numrow>%d</numrow>\n", numrow);
		//assert(numrow <= 1);
		if (numrow == 1) {
			row = mysql_fetch_row(result);
			assign_position_data(pd, row);
		}
		mysql_free_result(result);//hczadd
	//	mysql_close(&mysql);	
		return numrow;
	}
	//mysql_close(&mysql);
	 syslog(LOG_WARNING, "123");
	return -1;
}

int get_LBS(const char* id, struct lbs_data* ld)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;

	length = sprintf(cmdbuf, "select time, mnc, mcc, cid,lac from DeviceInfo where ID=\"%s\"", id);
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
	result = mysql_store_result(&mysql);
	if (result) {
		int numrow;
		MYSQL_ROW row;
		int i;
		numrow = mysql_num_rows(result);
		//printf("<numrow>%d</numrow>\n", numrow);
		//assert(numrow <= 1);
		if (numrow == 1) {
			row = mysql_fetch_row(result);
			assign_LBS_data(ld, row);
		}
		mysql_free_result(result);//hczadd
	//	mysql_close(&mysql);	
		return numrow;
	}
	//mysql_close(&mysql);
	return -1;
}

int get_device_info(const char*clientid, struct device_data* dd)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;

	length = sprintf(cmdbuf, "select Master, Password, Activated from DeviceInfo where ID=\"%s\"", clientid);
	do
	{
		init_dbconnect();
		err = mysql_real_query(&mysql,  cmdbuf, length);
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
	result = mysql_store_result(&mysql);
	if (result) {
		int numrow;
		MYSQL_ROW row;
		int i;
		numrow = mysql_num_rows(result);
		if (numrow == 1) {
			row = mysql_fetch_row(result);
			assign_device_data(dd, row);
		}
		mysql_free_result(result);
		//mysql_close(&mysql);
		return numrow;
	}
	//mysql_close(&mysql);
	return -1;
}

int get_last_edition(struct edition_info* ei)
{
	char cmdbuf[MAX_SQL_LENGTH];
	int length;
//	length = sprintf(cmdbuf,"select * from EditionInfo");
	char* cmd = "select version, updatetime, changes, url, name from EditionInfo order by updatetime desc limit 1";
	int err;
	char tryCnt = SQL_QUERY_ERROR_TRY;
	MYSQL_RES* result;
	do
	{
		init_dbconnect();
        	err = mysql_real_query(&mysql, cmd, strlen(cmd));
		if(err != 0)
		{
			mysql_close(&mysql);
			initialized = 0;
			tryCnt--;
		}
	}while(err && tryCnt);
        result = mysql_store_result(&mysql);
        if (result) {
                int numrow;
                MYSQL_ROW row;
                int i;
                numrow = mysql_num_rows(result);
                if (numrow == 1) {
                        row = mysql_fetch_row(result);
                        assign_edition_data(ei, row);
                }
                mysql_free_result(result);
		//mysql_close(&mysql);
                return numrow;
        }
	//mysql_close(&mysql);
        return -1;
}

