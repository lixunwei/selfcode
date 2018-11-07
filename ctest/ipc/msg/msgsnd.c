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

static int process_val_check(uint32_t process)
{
	static uint32_t previous = 0;

	if (process == UINT32_MAX) {
		print_help();
		exit(MSG_ARGUEMNT_ERROR);
	}
}

int main(int argc, char *argv[])
{
	int arg;
	int msgid;
	int index = 0;
	key_t msgkey;
	uint32_t key = MSG_FOTA_SERVER_ID;
	uint32_t process = UINT32_MAX;
	struct msgbuf_bar *buf;
	char *path = COMMON_PATH;

	while ((arg = getopt_long(argc, argv, "f:h", opts, &index)) != -1)
	{
		switch(arg)
		{
			case 'p':
				path = strdup(optarg);
				break;
			case 'k':
				key = (uint32_t)strtoul(optarg, NULL, 0);
				break;
			case 'h':
				print_help();
				break;
		}
	}

	if (optind < argc) {
		process = strtoul(argv[optind++], NULL, 0);
	}

	process_val_check(process);
	if ((msgkey = ftok(path, key)) < 0 ) {
		printf("Message Create System V IPC key Failed!!!\n");
		printf("Message path=\"%s\", key=\"%d\" (%s)\n", path, key, strerror(errno));
		exit(MSG_CREATE_KEY_FAILED);
	}

	if ((msgid = msgget(msgkey, IPC_CREAT|SVMSG_MODE)) < 0 ) {
		printf("msg create failed (%s)\n", strerror(errno));
		exit(MSG_CREATE_FAILED);
	}
	buf = malloc(sizeof(struct msgbuf_bar));
	if (NULL == buf) {
		printf("Memory is not enough\n");
		exit(MSG_MEMORY_NOT_ENOUGH);
	}
	buf->type = 1;
	buf->rate = process;
	msgsnd(msgid, buf, sizeof(struct msgbuf_bar), 0);
}
