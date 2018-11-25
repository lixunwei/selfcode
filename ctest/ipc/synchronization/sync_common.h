#ifndef __SYNC_COMMON_H
#define __SYNC_COMMON_H

static int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return fcntl(fd, cmd, &lock);
}

//file file_lock
#define read_lock(fd, offset, whence, len) \
            lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define write_lock(fd, offset, whence, len) \
            lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)

#define read_lock_wait(fd, offset, whence, len) \
            lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define write_lock_wait(fd, offset, whence, len) \
            lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)

#define un_lock(fd, offset, whence, len) \
           lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

#define file_r_lock(fd) \
        read_lock(fd, 0, SEEK_SET, 0)
#define file_w_lock(fd) \
        write_lock(fd, 0, SEEK_SET, 0)

#define file_r_lock_wait(fd) \
        read_lock_wait(fd, 0, SEEK_SET, 0)
#define file_w_lock_wait(fd) \
        write_lock_wait(fd, 0, SEEK_SET, 0)

#define file_unlock(fd) \
        un_lock(fd, 0, SEEK_SET, 0)

//error code
#define FCNTL_W_LOCK_FAILED     1
#define FCNTL_UNLOCK_FAILED     1
#endif
