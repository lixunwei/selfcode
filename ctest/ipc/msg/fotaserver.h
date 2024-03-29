#ifndef __FOTASERVER_H
#define __FOTASERVER_H

struct msgbuf_bar {
    long type;
    uint32_t rate;
};

struct msgbuf_ctl {
    long type;
    uint32_t cmd;
};
//Message create key
#define COMMON_PATH     "/usr/bin/ab_full_update"
#define MSG_FOTA_SERVER_ID  777
#define MSG_APP_ID          1

//Message type
#define MSG_TYPE_BAR    1
#define MSG_TYPE_CTL    2

enum CTL_INFO {
    FOTA_PROCESS_START = 1,
    FOTA_PROCESS_EXIT  = 2,
    FOTA_CTL_INFO_MAX,
};

//Message queue permission
#define MSG_R (0400)
#define MSG_W (0200)
#define SVMSG_MODE (0666)

//fotaserver exit code
#define MSG_ARGUEMNT_ERROR			1
#define MSG_CREATE_KEY_FAILED 		2
#define MSG_CREATE_FAILED 			3
#define MSG_MEMORY_NOT_ENOUGH 		4

#endif
