#ifndef DBACCESS_H
#define DBACCESS_H

#define MAX_LONGTITUDE_LENGTH	32

struct position_data {
	char time[MAX_LONGTITUDE_LENGTH];
	char master[MAX_LONGTITUDE_LENGTH];
	char longitude[MAX_LONGTITUDE_LENGTH];
	char latitude[MAX_LONGTITUDE_LENGTH];
	char bat[5];//add by hzc
	char type[1];
};

struct lbs_data {
	char time[MAX_LONGTITUDE_LENGTH];
	char mnc[5];
	char mcc[5];
	char cid[10];
	char lac[10];
};

struct device_data {
	char master[MAX_LONGTITUDE_LENGTH];
	char passworld[MAX_LONGTITUDE_LENGTH];
	char activated[MAX_LONGTITUDE_LENGTH];
};

struct edition_info {
	char name[MAX_LONGTITUDE_LENGTH];
	char version[MAX_LONGTITUDE_LENGTH];
	char updatetime[MAX_LONGTITUDE_LENGTH];
	char changes[200];
	char url[100];
};

int update_LBS(const char *id, const char* mnc, const char *mcc, const char *cid, const char *lac, const char *time);

int add_position(const char *clientid, const char* time, const char *longitude,
		  const char *latitue, int voltage, char type);
int add_device(const char *ID,const char *Master,const char *Password,const char *Activated);

int get_postions(const char* clientid, const char* starttime, const char* endtime, struct position_data *pd, int max_num);

int get_last_positon(const char*clientid, struct position_data* pd);

int get_LBS(const char* id, struct lbs_data* ld);

int get_device_info(const char*clientid, struct device_data* dd);

int get_last_edition(struct edition_info* ei);
#endif
