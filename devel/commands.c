#include <fcgi_stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbaccess.h"
#include "http_parser.h"
#include "requestBSposition.h"
#include <syslog.h>

#define KEY_LENGTH	128
#define VALUE_LENGTH	128
#define REQUEST_BS_KEY	"hy7a8b"

struct key_value {
	char key[KEY_LENGTH];
	char value[VALUE_LENGTH];
};

#define MAX_PARAM_NUM	32

static struct key_value key_value_array[MAX_PARAM_NUM];

//#define get_param_value(name)	get_value(key_value_array, name)

static int get_key_value(char* str, struct key_value* kv)
{
	char* tmp;
	tmp = strsep(&str, "=");
	if (tmp) {
		strcpy(kv->key, tmp);
		tmp = strsep(&str, "=");
		if (tmp) {
			strcpy(kv->value, tmp);
			return 0;
		}
	}
	return -1;
}

static int sep_key_value(char* param, struct key_value kvs[], int num)
{
	int n = 0;
	char* tmp = NULL;

	while (param) {
		tmp = strsep(&param, "&");
		get_key_value(tmp, &kvs[n]);
		//printf("<br/>%s: %s --> %s\n", __func__, kvs[n].key, kvs[n].value);
		++n;
		if (n >= num)
			return -2;
	}

	//printf("<br/> num is %d <br/>\n", n);
	return n;
}

static char* get_value(struct key_value kvs[], const char* key)
{
	int i = 0;
	while (kvs[i].key[0] != 0) {
		if (!strcmp(key, kvs[i].key))
			return kvs[i].value;
		++i;
	}
	return NULL;
}

char* get_param_value(const char* key)
{
	return get_value(key_value_array, key);
}

int init_param(char* param)
{
	int num;

	memset(key_value_array, 0, sizeof(key_value_array));
	num = sep_key_value(param, key_value_array, MAX_PARAM_NUM);

#if 0
	{
		int i;
		for (i = 0; i < num; ++i) {
			printf("<br/> key & value <br/>\n");
			printf("<br/> %s = %s\r\n", key_value_array[i].key, key_value_array[i].value);
		}
		printf("<br/>\r\n");
	}
#endif

	return num;
}

static int on_BSbody_callback(http_parser *parser, const char *at, size_t length)
{
	char *id;
	char *time;
	char *lat;
	char *lon;
	char *svolt;
	int voltage;
	id = get_param_value("id");
        time = get_param_value("time");        
	svolt = get_param_value("voltage");
	voltage = atoi(svolt);
	char *param = malloc(length);
	strcpy(param, at);
	char *tem;
	char *key = "<lbs>";
	if(strstr(at, key) == NULL){
		free(param);
		return -1;
	}
	char *data[2];
	int i= 0;
	while(param) {
		tem = strsep(&param, "<>ablnost/\n\r");
		if(*tem != '\0' && *tem != '0')
			data[i++] = tem;	
	}
	lat = data[0];
	lon = data[1];
	add_position(id, time, lon, lat, voltage, '0');
	free(param);
	return 0;
}

static void output_LBS(const char* id, const struct lbs_data *ld)
{
	printf("<lbs>\n");
	printf("<id>%s</id>\n", id);
	printf("<time>%s</time>\n", ld->time);
	printf("<mnc>%s</mnc>\n", ld->mnc);
	printf("<mcc>%s</mcc>\n", ld->mcc);
	printf("<cid>%s</cid>\n", ld->cid);
	printf("<lac>%s</lac>\n", ld->lac);
	printf("</lbs>\n");
}
static void output_position(const char* clientid, const struct position_data *pd)
{
	printf("<position>\n");
	printf("<id>%s</id>\n", clientid);
	printf("<type>%s</type>\n", pd->type);
	printf("<time>%s</time>\n", pd->time);
	printf("<master>%s</master>\n", pd->master);
	printf("<longitude>%s</longitude>\n", pd->longitude);
	printf("<laitude>%s</laitude>\n", pd->latitude);
	printf("<bat>%s</bat>\n", pd->bat);
	printf("</position>\n");
}

static void output_device_info(const char* clientid, const struct device_data *dd)
{
	printf("<tracer>\n");
	printf("<id>%s</id>\n", clientid);
	printf("<master>%s</master>\n", dd->master);
	printf("<pw>%s</pw>\n", dd->passworld);
	printf("<activated>%s</activated>\n", dd->activated);
	printf("</tracer>\n");
}

static void output_edition_info(const struct edition_info *ei)
{
	printf("<update>\n");
	printf("<name>%s</name>", ei->name);
	printf("<version>%s</version>", ei->version);
	printf("<uptime>%s</uptime>", ei->updatetime);
	printf("<changes>%s</changes>", ei->changes);
	printf("<url>%s</url>", ei->url);
	printf("</update>\n");
}

int getpositions()
{
	int num;
	char* clientid;
	char* starttime;
	char* endtime;
	struct position_data pd[32];

	clientid = get_param_value("clientid");
	starttime = get_param_value("starttime");
	endtime = get_param_value("endtime");

	num = get_postions(clientid, starttime, endtime, pd, 32);
	if (num > 0) {
		int i;
		printf("<positions>\n");
		for (i = 0; i < num; ++i) {
			output_position(clientid, &pd[i]);
		}
		printf("</positions>\n");
	}
        else
	{
		printf("<positions>null</positions>\n");
        }

	return 0;
}

int getLBS()
{
	int num;
	char* id;
	struct lbs_data ld;

	memset(&ld, 0, sizeof(ld));
	id = get_param_value("id");
#if 1
	num = get_LBS(id, &ld);
#endif
	if(num == 1)
	{
		output_LBS(id, &ld);
	}
	else
	{
		printf("<lbs>null</lbs>\n");
	}
	return 0;
}
int getposition()
{
	int num;
	char* clientid;
	struct position_data pd;

	memset(&pd, 0, sizeof(pd));
	clientid = get_param_value("clientid");
#if 1
	num = get_last_positon(clientid, &pd);
#endif
	if(num == 1)
	{
		output_position(clientid, &pd);
	}
	else
	{
		printf("<position>null</position>\n");
	}
	return 0;
}

int reportposition()
{
	int num;
	char* clientid;
	char* time;
	char* longitude;
	char* latitue;
	char* svolt;
	int voltage;

	clientid = get_param_value("clientid");
	time = get_param_value("time");
	longitude = get_param_value("longitude");
	latitue = get_param_value("latitude");
	svolt = get_param_value("voltage");
	voltage = atoi(svolt);

	add_position(clientid, time, longitude, latitue, voltage, '1');
	printf("<tracercmd><result>ok</result></tracercmd>");

	return 0;
}

int reportLBS()
{
	int num;
	char* id;
	char* mnc;
	char* mcc;
	char* cid;
	char* lac;
	char* time;
	char result = 0;
	
	id = get_param_value("id");
	mnc = get_param_value("mnc");
	mcc = get_param_value("mcc");
	cid = get_param_value("cid");
	lac = get_param_value("lac");
	time = get_param_value("time");
	
	char url[50];
	sprintf(url, "http://cellinfo.sinaapp.com/%s/%s/%s/%s/%s", REQUEST_BS_KEY,mcc, mnc, lac, cid);
	http_parser_settings parser_settings;
	memset(&parser_settings, 0, sizeof(parser_settings));
	parser_settings.on_body = &on_BSbody_callback;
	requestBSposition(url, &parser_settings);
	result = update_LBS(id, mnc, mcc, cid, lac, time);
	if(result == 0)
	{
		printf("<tracercmd><cmd>reportLBSres</cmd><result>ok</result></tracercmd>");
	}
	else
	{
		printf("<tracercmd><cmd>reportLBSres</cmd><result>error</result></tracercmd>");
	}
}

int activate()
{
	char* clientid;
	char* master;
	char* password;
	char* activated;
	char result = 0;

	clientid = get_param_value("id");
	master = get_param_value("master");
	password = get_param_value("password");
	activated = get_param_value("activated");
	result = add_device(clientid, master, password,activated);
	if(result == 0)
	{
		printf("<tracercmd><cmd>activateres</cmd><result>ok</result></tracercmd>");
	}
	else
	{
		printf("<tracercmd><cmd>activateres</cmd><result>error</result></tracercmd>");
	}
	return 0;
}

int getinfo()
{
	int num;
	char* id;
	struct device_data dd;

	memset(&dd, 0, sizeof(dd));
	id = get_param_value("id");
	#if 1
	num = get_device_info(id, &dd);
	#endif
	if(num == 1)
	{
		output_device_info(id, &dd);
	}
	else
	{
		printf("<tracer>null</tracer>\n");	
	}
	return 0;
}

int getlastedition()
{
	int num;
	struct edition_info ei;

	memset(&ei, 0, sizeof(ei));
#if 1
	num = get_last_edition(&ei);
#endif
	if(num == 1)
	{
		output_edition_info(&ei);
	}
	else
	{
		printf("<tracer>null</tracer>\n");
	}
	return 0;
}

