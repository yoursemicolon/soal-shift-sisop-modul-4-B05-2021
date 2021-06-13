#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

/* 
    ** COMPILE
    gcc -Wall `pkg-config fuse --cflags` SinSeiFS_B05.c -o SinSeiFS_B05 `pkg-config fuse --libs`

    ** CREASE FUSE DIRECTORY
    mkdir fusedir

    ** EXECUTE
    ./SinSeiFS_B05 -f fusedir

    ** UNMOUNT
    fusermount -u fusedir
*/

static const *dirpath = "/home/yoursemicolon/Downloads";

char *atoz = "AtoZ_";
char *key = "SISOP";
char *rx = "RX_";
char *a_is_a = "A_is_a";

// create log system
void writingLog(char *str, int logType)
{
    time_t rawtime;
    time(&rawtime);
    struct tm *info = = localtime(&rawtime);

    char curtime[30];
    strftime(curtime, 30, "%d%m%Y-%H:%M:%S", info);

    FILE *logFile = fopen(logpath, "a");

    if (logType == 1)
    {
        fprintf(logFile, "INFO::%s:%s\n", curtime, str);
    }
    else if (logType == 2)
    {
        fprintf(logFile, "WARNING::%s:%s\n", curtime, str);
    }
    fclose(logFile);
}

// log for mkdir and rename
void writingLog2(const char *from, char *to)
{
    FILE *fp = fopen("/home/yoursemicolon/fs.log", "a");

    fprintf(str, "%s -> %s\n", from, to);
    fclose(fp);
}

void encodeRot13(char *str)
{
    if (strcmp(str, ".") == 0 || strcmp(str, "..") == 0)
        return;
    
    printf("Encode path Rot13: %s\n", str);

    int lenght = strlen(str);
    for (int i = 0; i < lenght; i++)
    {
        if (str[i] == '/')
            continue;
        if (str[i] == '.')
            break;

        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            if(str[i] < 'N')
            {
                str[i] += 13;
            }
            else if(str[i] >= 'N')
            {
                str[i] -= 13;
            }
        }
        else if(str[i] >= 'a' && str[i] <= 'z')
        {
            if(str[i] < 'n')
            {
                str[i] += 13;
            }
            else if(str[i] >= 'n')
            {
                str[i] -= 13;
            }
        }
    }
}

void decodeRot13(char *str)
{
    if (strcmp(str, ".") == 0 || strcmp(str, "..") == 0 || strstr(str, "/") == NULL)
        return;

    printf("Decode path Rot13: %s\n", str);

    int length = strlen(str), s = 0;
    for (int i = length-1; i >= 0; i--)
    {
        if (str[i] == '/')
            break;

        if (str[i] == '.') // titik terakhir
        {
            length = i;
            break;
        }
    }

    for (int i = 0; i < length; i++)
    {
        if (str[i] == '/')
        {
            s = i + 1;
            break;
        }
    }

    for (int i = s; i < length; i++)
    {
        if (str[i] == '/')
        {
            continue;
        }
        
        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            if(str[i] < 'N')
            {
                str[i] += 13;
            }
            else if(str[i] >= 'N')
            {
                str[i] -= 13;
            }
        }
        else if(str[i] >= 'a' && str[i] <= 'z')
        {
            if(str[i] < 'n')
            {
                str[i] += 13;
            }
            else if(str[i] >= 'n')
            {
                str[i] -= 13;
            }
        }
    }
}

void encodeAtbash(char *str)
{
    if (strcmp(str, ".") == 0 || strcmp(str, "..") == 0)
        return;

    printf("Encode path Atbash: %s\n", str);

    int lenght = strlen(str);
    for (int i = 0; i < lenght; i++)
    {
        if (str[i] == '/')
            continue;
        if (str[i] == '.')
            break;

        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = 'Z' + 'A' - str[i];
        }
        else if (st[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = 'z' + 'a' - str[i];
        }
    }
}

void decodeAtbash(char *str)
{
    if (strcmp(str, ".") == 0 || strcmp(str, "..") == 0 || strstr(str, "/") == NULL)
        return;

    printf("Decode path Atbash: %s\n", str);

    int length = strlen(str), s = 0;
    for (int i = length-1; i >= 0; i--)
    {
        if (str[i] == '/')
            break;

        if (str[i] == '.') // titik terakhir
        {
            length = i;
            break;
        }
    }

    for (int i = 0; i < length; i++)
    {
        if (str[i] == '/')
        {
            s = i + 1;
            break;
        }
    }

    for (int i = s; i < length; i++)
    {
        if (str[i] == '/')
        {
            continue;
        }
        
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = 'Z' + 'A' - str[i];
        }
        else if (st[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = 'z' + 'a' - str[i];
        }
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000], str[1000];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    sprintf(str, "LS::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    DIR *dp;
    struct dirent *de;

    (void)offset;
    (void)fi;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;

        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (check1)
            encodeAtbash(de->d_name); // encode yang ada di dalam directory sekarang

        if (check2)
            encodeAtbash(de->d_name);
        encodeRot13(de->d_name);

        res = (filler(buf, de->d_name, &st, 0));
        if (res != 0)
            break;
    }

    sprintf(str, "READDIR::%s", path);
    writingLog(str, 1);

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd, res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    (void)fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    sprintf(str, "READ::%s", path);
    writingLog(str, 1);

    close(fd);
    return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    char *folderPath = strstr(fpathTo, atoz);
    if (folderPath)
        writingLog2(path, fpath);

    res = mkdir(fpath, mode);
    if (res == -1)
        return -errno;

    sprintf(str, "MKDIR::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_rmdir(const char *path)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = rmdir(fpath);
    if (res == -1)
        return -errno;

    sprintf(str, "RMDIR::%s", path);
    writingLog(str, 2);

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    int res;
    char fpathFrom[1000], fpathTo[1000], str[100];

    char *check1 = strstr(to, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(from, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    char *check3 = strstr(to, rx);
    if (check3)
        decodeRot13(check3);
    decodeAtbash(check3);

    sprintf(fpathFrom, "%s%s", dirpath, from);
    sprintf(fpathTo, "%s%s", dirpath, to);

    char *folderPath = strstr(fpathTo, atoz);
    if (folderPath)
        writingLog2(fpathFrom, fpathTo);

    res = rename(fpathFrom, fpathTo);
    if (res == -1)
        return -errno;

    sprintf(str, "RENAME::%s::%s", from, to);
    writingLog(str, 1);

    if (check3 != NULL)
    {
        // enkripsi2(fpathto);
        sprintf(str, "ENCODE2::%s::%s", from, to);
        writingLog(str, 1);
    }

    if (check2 != NULL && check3 == NULL)
    {
        // dekripsi2(fpathto);
        sprintf(str, "DECODE2::%s::%s", from, to);
        writingLog(str, 1);
    }

    if (strstr(to, a_is_a) != NULL)
    {
        // encodeBinary(fpathto);
        sprintf(str, "ENCODE3::%s::%s", from, to);
        writingLog(str, 1);
    }

    if (strstr(from, a_is_a) != NULL && strstr(to, a_is_a) == NULL)
    {
        // decodeBinary(fpathto);
        sprintf(str, "DECODE3::%s::%s", from, to);
        writingLog(str, 1);
    }

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    int fd, res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = truncate(fpath, size);
    if (res == -1)
        return -errno;

    sprintf(str, "TRUNCATE::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd, res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    (void)fi;
    fd = open(fpath, O_WRONLY);
    if (fd == -1)
        return = -errno;

    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    sprintf(str, "WRITE::%s", path);
    writingLog(str, 1);

    close(fd);
    return res;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    (void)fi;
    res = creat(fpath, mode);
    if (res == -1)
        return -errno;

    sprintf(str, "CREATE::%s", path);
    writingLog(str, 1);

    close(res);
    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    struct timeval tv[2];

    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = ts[0].tv_nsec / 1000;
    tv[1].tv_sec = ts[1].tv_sec;
    tv[1].tv_usec = ts[1].tv_nsec / 1000;

    res = utimes(fpath, tv);
    if (res == -1)
        return -errno;

    sprintf(str, "UTIMENS::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_access(const char *path, int mask)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = access(fpath, mask);
    if (res == -1)
        return -errno;

    sprintf(str, "ACCESS::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = open(fpath, fi->flags);
    if (res == -1)
        return -errno;

    sprintf(str, "OPEN::%s", path);
    writingLog(str, 1);

    close(res);
    return 0;
}

static int xmp_unlink(const char *path)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = unlink(fpath);
    if (res == -1)
        return -errno;

    sprintf(str, "UNLINK::%s", path);
    writingLog(str, 2);

    return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = readlink(fpath, buf, size - 1);
    if (res == -1)
        return -errno;

    sprintf(str, "READLINK::%s", path);
    writingLog(str, 1);

    buf[res] = '\0';
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    /* 'mknod(path, mode, rdev)' is more portable */
    if (S_ISREG(mode))
    {
        res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0)
            res = close(res);
    }
    else if (S_ISFIFO(mode))
        res = mkfifo(fpath, mode);
    else
        res = mknod(fpath, mode, rdev);

    if (res == -1)
        return -errno;

    sprintf(str, "MKNOD::%s", path);
    writingLog(str, 1);
}

static int xmp_symlink(const shar *from, const char *to)
{
    int res;
    char fpathFrom[1000], fpathTo[1000], str[100];

    int res;
    char fpathFrom[1000], fpathTo[1000], str[100];

    char *check1 = strstr(to, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(from, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    char *check3 = strstr(to, rx);
    if (check3)
        decodeRot13(check3);
    decodeAtbash(check3);

    sprintf(fpathFrom, "%s%s", dirpath, from);
    sprintf(fpathTo, "%s%s", dirpath, to);

    res = symlink(fpathFrom, fpathTo);
    if (res == -1)
        return -errno;

    sprintf(str, "SYMLINK::%s::%s", from, to);
    writingLog(str, 1);

    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    int res;
    char fpathFrom[1000], fpathTo[1000], str[100];

    char *check1 = strstr(to, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(from, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    char *check3 = strstr(to, rx);
    if (check3)
        decodeRot13(check3);
    decodeAtbash(check3);

    sprintf(fpathFrom, "%s%s", dirpath, from);
    sprintf(fpathTo, "%s%s", dirpath, to);

    res = link(fpathFrom, fpathTo);
    if (res == -1)
        return -errno;

    sprintf(str, "LINK::%s::%s", from, to);
    writingLog(str, 1);

    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = chmod(fpath, mode);
    if (res == -1)
        return -errno;

    sprintf(str, "CHMOD::%s", path);
    writingLog(str, 1);

    return 0;

    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = lchown(fpath, uid, gid);
    if (res == -1)
        return -errno;

    sprintf(str, "CHOWN::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
    int res;
    char fpath[1000], str[100];

    char *check1 = strstr(path, atoz);
    if (check1)
        decodeAtbash(check1);

    char *check2 = strstr(path, rx);
    if (check2)
        decodeRot13(check2);
    decodeAtbash(check2);

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    res = statvfs(fpath, stbuf);
    if (res == -1)
        return -errno;

    sprintf(str, "STATFS::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_release(const char *path, struct fuse_info *fi)
{
    char fpath[1000], str[100];
    sprintf(fpath, "%s%s", dirpath, path);
    (void)fpath;
    (void)fi;

    sprintf(str, "RELEASE::%s", path);
    writingLog(str, 1);

    return 0;
}

static int xmp_fsync(const char *path, int isdatasync, struct fuse_file info *fi)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    (void)fpath;
    (void)isdatasync;
    (void)fi;

    sprintf(str, "FSYNC::%s", path);
    writingLog(str, 1);

    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .open = xmp_open,
    .mknod = xmp_mknod,
    .write = xmp_write,
    .access = xmp_access,
    .readlink = xmp_readlink,
    .symlink = xmp_symlink,
    .link = xmp_link,
    .chmod = xmp_chmod,
    .chown = xmp_chown,
    .truncate = xmp_truncate,
    .utimens = xmp_utimens,
    .statfs = xmp_statfs,
    .create = xmp_create,
    .release = xmp_release,
    .fsync = xmp_fsync,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
