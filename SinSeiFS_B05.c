#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

static const char *dirpath = "/home/yoursemicolon/Downloads";
static const char *logpath = "/home/yoursemicolon/SinSeiFS.log";

void writeInfo(char *text, char *path)
{
    char *info = "INFO";
    char curtime[30];
    time_t t = time(NULL);

    struct tm *p1 = localtime(&t);
    strftime(curtime, 30, "%d%m%Y-%H:%M:%S", pi);

    char log[1000];
    sprintf(log, "%s::%s:%s::%s", info, curtime, text, path);

    FILE *out = fopen(logpath, "a");
    fprintf(out, "%s\n", log);
    fclose(out);

    return 0;
}

void writeWarning(char *text, char *path)
{
    char *info = "WARNING";
    char curtime[30];
    time_t t = time(NULL);

    struct tm *p1 = localtime(&t);
    strftime(curtime, 30, "%d%m%Y-%H:%M:%S", p1);

    char log[1000];
    sprintf(log, "%s::%s:%s::%s", info, curtime, text, path);

    FILE *out = fopen(logpath, "a");
    fprintf(out, "%s\n", log);
    fclose(out);

    return 0;
}

char ext[100000] = "\0";
int id = 0;

void substring(char *s, char *sub, int p, int l)
{
    int c = 0;
    while (c < l)
    {
        sub[c] = s[p + c];
        c++;
    }
    sub[c] = '\0';
}

void decodeString(char *arg, int stop)
{
    if (strcmp(arg, ".") == 0 || strcmp(arg, "..") == 0)
        return;
    for (int i = 0; i < stop - 1; i++)
    {
        arg[i] = atbashChiper(arg[i]);
    }
}

void decode(char *arg)
{
    if (strcmp(arg, ".") == 0 || strcmp(arg, "..") == 0)
        return;

    char *slash = strstr(arg, "/");
    if (slash != NULL)
    {
        int stop = strlen(slash);
        for (int i = stop - 1; i >= 0; i--)
        {
            if (slash[i] == '/')
                break;
            if (slash[i] == '.')
            {
                stop = i;
                break;
            }
        }
        decodeString(slash + 1, stop);
    }
}

char atbashChiper(char str)
{

    if (str >= 'A' && str <= 'Z')
    {
        str = 'Z' + 'A' - str;
        return str;
    }
    else if (str >= 'a' && str <= 'z')
    {
        str = 'z' + 'a' - str;
        return str;
    }

    return str;
}

char *checkPath(char *str)
{
    bool encr;
    int start, id;
    encr = 0;
    start = 1;
    id = strchr(str + start, '/') - str - 1;
    char curpos[1024];
    while (id < strlen(str))
    {
        strcpy(curpos, "");
        strncpy(curpos, str + start, id - start + 1);
        curpos[id - start + 1] = '\0';
        if (encr)
        {
            // encrypt(curpos, 0);
            strncpy(str + start, curpos, id - start + 1);
        }
        if (!encr && strstr(str + start, "AtoZ_") == str + start)
            encr = 1;
        start = id + 2;
        id = strchr(str + start, '/') - str - 1;
    }
    id = strlen(str);
    id--;
    strncpy(curpos, str + start, id - start + 1);
    curpos[id - start + 1] = '\0';
    if (encr)
    {
        // encrypt(curpos, 1);
        strncpy(str + start, curpos, id - start + 1);
    }
    return str;
}

char *getPath(char *fpath, char *dirpath, const char *path)
{
    strcpy(fpath, dirpath);
    if (!strcmp(path, "/"))
        return fpath;
    if (path[0] != '/')
    {
        fpath[strlen(fpath) + 1] = '\0';
        fpath[strlen(fpath)] = '/';
    }

    sprintf(fpath, "%s%s", fpath, path);
    return fpath;
}

{
    char fpath[1000];
    getPath(fpath, dirpath, path);

    // if(strcmp(path,"/") == 0)
    // {
    //     path = dirpath;
    //     sprintf(fpath,"%s",path);
    // } else sprintf(fpath, "%s%s",dirpath, path);

    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;

    dp = opendir(checkPath(path));

    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0))
            break;
    }
    closedir(dp);
    return 0;
}

// Nomor 1b rename
static int xmp_rename(const char *from, const char *to)
{
    int res;
    char fpath[1000], path2[1000];
    char *a = strstr(to, "/AtoZ");

    if (strstr(tp, "/AtoZ"))
    {
        // decrypt(strstr(tp, "/AtoZ"));
    }

    sprintf(fpath, "%s%s", dirpath, from);
    sprintf(path2, "%s%s", dirpath, to);

    res = rename(fpath, path2);
    if (res == -1)
        return -errno;

    return 0;
}

// Nomor 1d
void writeLog(const char arg1[], const char arg2[], int num_encv)
{
    FILE *fp = fopen(logpath, "a+");

    char str[1000];
    if (mode == 1)
    {
        sprintf(str, "RENAME %s menjadi %s", arg1, arg2);
        fprintf(fp, "%s\n", str);
    }
    else if (mode == 2)
    {
        sprintf(str, "MKDIR %s", arg2);
        fprintf(fp, "%s\n", str)
    }

    fclose(fp);
}

char *lastPart(char *str)
{
    if (!strcmp(str, "/"))
        return NULL;
    return strrchr(str, '/') + 1;
}

{
    char fpath[1000];

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    char *slash

        int res;

    res = mkdir(fpath, mode);
    if (res == -1)
        return -errno;

    return 0;
}

// int check_ext(char* file)
// {
// 	id = 0;
// 	while(id < strlen(file) && file[id] != '.') id++;
// 	memset(ext, 0, sizeof(ext));
// 	strcpy(ext, file + id);
// 	return id;
// }

// void split(char *str)
// {
// 	char finalPath[10000000];
// 	sprintf(finalPath, "%s.", str);
// 	pid_t cid;
// 	cid = fork();
// 	if(cid == 0)
// 	{
// 		char *argv[] = {"split", "-b", "1024", "-d", "-a", "3", str, finalPath, NULL};
// 		execv("/usr/bin/split", argv);
// 	}
// 	int status;
// 	while(wait(&status) > 0);
// 	unlink(str);
// }

// void combine(char *str)
// {
// 	char buff[2048];
// 	int id = 0;
// 	char *ext = str + strlen(str) - 4;
// 	if(strcmp(ext, ".000")) return;
// 	ext[0] = '\0';
// 	FILE *combined;
// 	combined = fopen(str, "wb");
// 	while(1)
// 	{
// 		char name[1000000];
// 		sprintf(name, "%s.%03d", str, id);
// 		FILE *each;
// 		each = fopen(name, "rb");
// 		if(!each) break;
// 		fseek(each, 0L, SEEK_END);
// 		rewind(each);
// 		fread(buff, sizeof(buff), ftell(each), each);
// 		fwrite(buff, sizeof(buff), ftell(each), combined);
// 		fclose(each);
// 		unlink(name);
// 		id++;
// 	}
// 	fclose(combined);
// }

// int encrFolder(char *str)
// {
// 	int ans;
// 	char *fi = strtok(str, "/");
// 	ans = 0;
// 	while(fi)
// 	{
// 		char sub[1024];
// 		substring(fi, sub, 0, 6);
// 		if(!strcmp(sub, "encv1_")) ans |= 1;
// 		else if(!strcmp(sub, "encv2_")) ans |= 2;
// 		fi = strtok(NULL, "/");
// 	}
// 	return ans;
// }

// int encrFull(char *str)
// {
// 	int ans;
// 	char *fi = strtok(str, "/");
// 	char *sc = strtok(NULL, "/");
// 	ans = 0;
// 	while(sc)
// 	{
// 		char sub[1024];
// 		substring(fi, sub, 0, 6);
// 		if(!strcmp(sub, "encv1_")) ans |= 1;
// 		else if(!strcmp(sub, "encv2_")) ans |= 2;
// 		fi = sc;
// 		sc = strtok(NULL, "/");
// 	}
// 	return ans;
// }

// void loopAllEnc1(char *str, int flag)
// {
// 	struct dirent *dp;
// 	DIR *dir = opendir(str);

// 	if(!dir) return;

// 	while((dp = readdir(dir)) != NULL)
// 	{
// 		if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
//         {
//         	char path[2000000], name[1000000], newname[1000000];
//         	mixPath(path, str, dp->d_name);
// 			strcpy(name, dp->d_name);
// 			if(flag == 1) mixPath(newname, str, encrypt(name, 1));
// 			else if(flag == -1) mixPath(newname, str, decrypt(name, 1));
// 			if(dp->d_type == DT_REG) rename(path, newname);
// 			else if(dp->d_type == DT_DIR)
// 			{
// 				rename(path, newname);
// 				loopAllEnc1(newname, flag);
// 			}
//         }
// 	}
// }

// void encrypt1(char *str, int flag)
// {
// 	struct stat add;
// 	stat(str, &add);
// 	if(!S_ISDIR(add.st_mode)) return;
// 	loopAllEnc1(str, flag);
// }

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    getPath(fpath, dirpath, path);

    char *a = strstr(path, "AtoZ_");
    if (a != NULL)
        decode(a);

    res = lstat(checkPath(fpath), stbuf);
    writeInfo("LS", fpath);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_access(const char *path, int mask)
{
    char *a = strstr(path, "AtoZ_");
    if (a != NULL)
        decode(a);

    int res;
    char fpath[1000];

    getPath(fpath, dirpath, path);

    res = access(checkPath(fpath), mask);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    char *a = strstr(path, "AtoZ_");
    if (a != NULL)
        decode(a);

    int res;
    char fpath[1000];
    getPath(fpath, dirpath, path);
    res = readlink(checkPath(fpath), buf, size - 1);

    if (res == -1)
        return -errno;
    buf[res] = '\0';
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char *a = strstr(to, "AtoZ_");
    if (a != NULL)
        decode(a);

    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;

    dp = opendir(checkPath(fpath));
    if (dp == NULL)
        return -errno;

    // int flag = encrFolder(fpath);
    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        char nama[1000000];
        strcpy(nama, de->d_name);
        if (flag == 1)
        {
            if (de->d_type == DT_REG)
                decrypt(nama, 1);
            else if (de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
                decrypt(nama, 0);
            res = (filler(buf, nama, &st, 0));
            if (res != 0)
                break;
        }
        else
        {
            res = (filler(buf, nama, &st, 0));
            if (res != 0)
                break;
        }
    }
    closedir(dp);
    writeInfo("CD", fpath);
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);
    checkPath(fpath);
    int res;

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

    writeInfo("MKNOD", fpath);
    return 0;
}

// static int xmp_mkdir(const char *path, mode_t mode)
// {
// 	char fpath[1000];
// 	mixPath(fpath, dirpath, path);

// 	int res;

// 	res = mkdir(cekPath(fpath), mode);
// 	if (res == -1) return -errno;

//     char cek_substr[1024];
//     if(lastPart(fpath) == 0) return 0;
//     char filePath[1000000];
//     strcpy(filePath, lastPart(fpath));
//     substring(filePath, cek_substr, 0, 6);
// 	if(strcmp(cek_substr, "encv1_") == 0) //folder encrypt1
// 	{
// 		encrypt1(fpath, 1);
// 	}
// 	else if(strcmp(cek_substr, "encv2_") == 0) //folder encrypt2
// 	{
// 		encrypt2(fpath, 1);
// 	}
// 	writeI("MKDIR", fpath);
// 	return 0;
// }

static int xmp_unlink(const char *path)
{
    char *a = strstr(path, "AtoZ_");
    if (a != NULL)
        decode(a);

    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = unlink(checkPath(fpath));
    writeWarning("REMOVE", fpath);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_rmdir(const char *path)
{
    char *a = strstr(to, "AtoZ_");
    if (a != NULL)
        decode(a);

    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = rmdir(checkPath(fpath));
    writeWarning("RMDIR", fpath);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
    int res;

    res = symlink(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    char f_from[1000];
    getPath(f_from, dirpath, from);

    char f_to[1000];
    getPath(f_to, dirpath, to);

    char *a = strstr(to, "AtoZ_");
    if (a != NULL)
        decode(a);

    int res;
    res = rename(checkPath(f_from), checkPath(f_to));

    if (res == -1)
        return -errno;

    // int ff_from = 0, ff_to = 0;
    // char cek_substr[1024], cek2[1024];

    // if (lastPart(f_from) == 0)
    //     return 0;
    // char filePath[1000000];

    // strcpy(filePath, lastPart(f_from));
    // substring(filePath, cek_substr, 0, 6);
    // if (strcmp(cek_substr, "AtoZ_") == 0) // encode 1
    // {
    //     ff_from = 1;
    // }

    // if(lastPart(fto) == 0) return 0;
    // strcpy(filePath, lastPart(f_to));
    // substring(filePath, cek_substr, 0 ,6);

    writeInfo("RENAME", fpath);
    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    // char fpath[1000];
    // getPath(fpath, dirpath, path)
    int res;

    res = link(from, to);
    if (res == -1)
        return -errno;

    // writeInfo("LINK", fpath);
    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = chmod(checkPath(fpath), mode);
    writeInfo("CHMOD", fpath);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = lchown(checkPath(fpath), uid, gid);
    writeInfo("CHOWN", fpath);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = truncate(checkPath(fpath), size);
    writeInfo("TRUNCATE", fpath);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    char fpath[1000];
    getPath(fpath, dirpath, path);

    int res;
    struct timeval tv[2];

    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = ts[0].tv_nsec / 1000;
    tv[1].tv_sec = ts[1].tv_sec;
    tv[1].tv_usec = ts[1].tv_nsec / 1000;

    res = utimes(checkPath(fpath), tv);
    writeInfo("UTIMENS", fpath);

    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    char *a = strstr(path, "AtoZ_");
    if (a != NULL)
        decode(a);

    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = open(checkPath(fpath), fi->flags);
    writeInfo("OPEN", fpath);

    if (res == -1)
        return -errno;
    close(res);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);

    char *a = strstr(to, "AtoZ_");
    if (a != NULL)
        decode(a);

    int fd = 0;
    int res = 0;

    (void)fi;
    fd = open(checkPath(fpath), O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    close(fd);

    writeInfo("CAT", fpath);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd, res;
    char fpath[1000];
    getPath(fpath, dirpath, path);

    char *a = strstr(path, "AtoZ_");
    if (a != NULL)
        decode(a);

    (void)fi;
    fd = open(checkPath(fpath), O_WRONLY);
    if (fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    writeInfo("WRITE", fpath);
    close(fd);
    return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);
    int res;

    res = statvfs(checkPath(fpath), stbuf);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    char fpath[1000];
    getPath(fpath, dirpath, path);
    (void)fi;

    int res;
    res = creat(checkPath(fpath), mode);

    if (res == -1)
        return -errno;
    writeInfo("CREATE", fpath);
    close(res);
    return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi)
{
    (void)path;
    (void)fi;
    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
    .link = xmp_link,
    .chmod = xmp_chmod,
    .chown = xmp_chown,
    .truncate = xmp_truncate,
    .utimens = xmp_utimens,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
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