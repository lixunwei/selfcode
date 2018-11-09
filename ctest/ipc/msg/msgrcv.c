#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    {"help",      no_argument,          0,  'h' },
    {0,           0,                    0,  '0' }
};

const char *helptext =
"fotaserver software v0.1 (2018-10-31)\n"
"Uage:fotaserver [-p|--path path -k|--key key] \n"
"fota server programe\n"
"\t--path path      \tshared message path\n"
"\t--key  key       \tshared message queue key\n";

static void print_help(void)
{
    printf("%s", helptext);
    exit(1);
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
	char *app_path = COMMON_PATH;
	char *fota_path = COMMON_PATH;
	int app_id = MSG_APP_ID;
	int fotamsgid;
	int appmsgid;
	struct msgbuf_bar *buf;
	struct msgbuf_bar *sendbuf;
	struct msgbuf_ctl *ctlbuf;
	uint32_t GetRate = 0;
	uint32_t CurrentRate = 0;
	int arg;
	int index = 0;

	while ((arg = getopt_long(argc, argv, "f:h", opts, &index)) != -1)
	{
		switch(arg)
		{
			case 'p':
				app_path = strdup(optarg);
				break;
			case 'k':
				app_id = (uint32_t)strtoul(optarg, NULL, 0);
				break;
			case 'h':
				print_help();
				break;
		}
	}

	fotamsgid = get_systemv_message(fota_path , MSG_FOTA_SERVER_ID, IPC_CREAT|SVMSG_MODE);
	if (fotamsgid < 0)
		exit(MSG_CREATE_FAILED);

	appmsgid = get_systemv_message(app_path, app_id, IPC_CREAT|SVMSG_MODE);
	if (appmsgid < 0)
		exit(MSG_CREATE_FAILED);

	buf = (struct msgbuf_bar *)malloc(sizeof(struct msgbuf_bar));
	ctlbuf = (struct msgbuf_ctl *)malloc(sizeof(struct msgbuf_ctl));
	sendbuf = (struct msgbuf_bar *)malloc(sizeof(struct msgbuf_bar));
	if (NULL == buf || NULL == ctlbuf || NULL == sendbuf) {
		printf("Memory is not enough\n");
		exit(MSG_MEMORY_NOT_ENOUGH);
	}
	sendbuf->type = MSG_TYPE_BAR;
	buf->rate = 0;
	while(1) {
		msgrcv(fotamsgid, buf, sizeof(struct msgbuf_bar), MSG_TYPE_BAR, IPC_NOWAIT);
		msgrcv(fotamsgid, ctlbuf, sizeof(struct msgbuf_ctl), MSG_TYPE_CTL, IPC_NOWAIT);
		GetRate = buf->rate;
		printf("get rate:%d\n", buf->rate);
		if (ctlbuf->cmd == FOTA_PROCESS_EXIT)
            break;
		if (GetRate == 100) {
			sendbuf->rate = 100;
			msgsnd(appmsgid, sendbuf, sizeof(struct msgbuf_bar), 0);
			break;
		}
		if (CurrentRate < GetRate && CurrentRate < 100) {
			sendbuf->rate = CurrentRate++;
			msgsnd(appmsgid, sendbuf, sizeof(struct msgbuf_bar), 0);
		}
		sleep(2);
	}
	sleep(10);
	msgctl(appmsgid, IPC_RMID, NULL);
	msgctl(fotamsgid, IPC_RMID, NULL);
}
