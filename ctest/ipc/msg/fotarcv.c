#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "fotaserver.h"

static struct option opts[] = {
    {"path",      required_argument,    0,  'p' },
    {"key",       required_argument,    0,  'k' },
    {"type",      required_argument,    0,  't' },
    {"help",      no_argument,          0,  'h' },
    {0,           0,                    0,  '0' }
};

const char *helptext =
"fotarcv software v0.1 (2018-10-31)\n"
"Uage:fotarcv [-p|--path path -k|--key key | --type num] \n"
"fota server programe\n"
"\t--path path      \tshared message path\n"
"\t--key  key       \tshared message queue key\n"
"\t--type num       \tsend message type\n";

static void print_help(void)
{
    printf("%s", helptext);
    exit(1);
}

static void send_val_check(uint32_t msgtype, uint32_t val)
{
	static uint32_t previous = 0;

	switch(msgtype) {
		case MSG_TYPE_BAR:
		{
			if (val == UINT32_MAX || val > 100) {
				printf("Fota Process should be exist and less than 100\n");
				exit(MSG_ARGUEMNT_ERROR);
			}
			break;
		}
		case MSG_TYPE_CTL:
		{
			if (val == UINT32_MAX || val >= FOTA_CTL_INFO_MAX) {
				printf("Fota ctrl info should be exist and less than %d\n", FOTA_CTL_INFO_MAX);
				exit(MSG_ARGUEMNT_ERROR);
			}
			break;
		}
		default:
			printf("Msgtype is error!!!\n");
			exit(MSG_ARGUEMNT_ERROR);
	}
}

static int get_systemv_message(const char *path, int id, int msgflag)
{
	key_t key;
	int msgid;

	if ((key = ftok(path, id)) < 0) {
		printf("Message Create System V IPC key Failed!!!\n");
		printf("Message path=\"%s\", key=\"%d\" (%s)\n", path, id, strerror(errno));
		return -MSG_CREATE_KEY_FAILED;
	}

	if ((msgid = msgget(key, msgflag)) < 0) {
		printf("SystemV IPC Message ID create failed (%s)\n", strerror(errno));
		return -MSG_CREATE_FAILED;
	}

	return msgid;
}

int main(int argc, char *argv[])
{
	int arg;
	int msgid;
	int index = 0;
	uint32_t id = MSG_FOTA_SERVER_ID;
	uint32_t val = UINT32_MAX;
	uint32_t msgtype = UINT32_MAX;
	struct msgbuf_bar buf;
	struct msgbuf_ctl ctlbuf;
	char *path = COMMON_PATH;

	while ((arg = getopt_long(argc, argv, "f:h", opts, &index)) != -1)
	{
		switch(arg)
		{
			case 'p':
				path = strdup(optarg);
				break;
			case 'k':
				id = (uint32_t)strtoul(optarg, NULL, 0);
				break;
			case 't':
				msgtype = (uint32_t)strtoul(optarg, NULL, 0);
				break;
			case 'h':
				print_help();
				break;
		}
	}
	
	msgid = get_systemv_message(path, id, IPC_CREAT|SVMSG_MODE);
	if (msgid < 0)
		exit(MSG_CREATE_FAILED);

	switch(msgtype) {
		case MSG_TYPE_BAR:
		{
			msgrcv(msgid, &buf, sizeof(struct msgbuf_bar), MSG_TYPE_BAR, IPC_NOWAIT);
			//if (errno != ENOMSG)
				printf("Msgid:%d Type:FotaBar val:%u\n", msgid, buf.rate);
			break;
		}
		case MSG_TYPE_CTL:
		{
			msgrcv(msgid, &ctlbuf, sizeof(struct msgbuf_ctl), MSG_TYPE_CTL, IPC_NOWAIT);
			//if (errno != ENOMSG)
				printf("Msgid:%d Type:FotaCtl val:%u\n", msgid, ctlbuf.cmd);
			break;
		}
	}
}
