/*
    PrepFS, by Zane Ashby
    Preprocess files as they're read using an external program.

    Compile using: gcc -o prepfs prepfs.c -lfuse -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26
    Run with: ./prepfs mountpoint -b dir_you_want_to_mirror -pp 'gpp -x "%s"'


    Modified from FUSE "Hello World" example, original notice below:

    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2005  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU GPL.
 */

#include <fuse.h>
#include <fuse/fuse_opt.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <dirent.h>

struct options {
    char *base_dir;
    char *pp_path;
} options;

static struct fuse_opt prepfs_opts[] =
{
    { "-b %s", offsetof(struct options, base_dir), 0},
    { "-pp %s", offsetof(struct options, pp_path), 0}
};

static char *get_extended_path(const char *path)
{
    char *extended_path = malloc(sizeof(char) * PATH_MAX + 1);
    extended_path[0] = '\0';
    strncat(extended_path, options.base_dir, PATH_MAX);
    strncat(extended_path, path, PATH_MAX);

    return extended_path;
}

static int prepfs_getattr(const char *path, struct stat *stbuf)
{
    char *expath = get_extended_path(path);

    stat(expath, stbuf); /* Return unmodified stat, although filesize will be wrong */

    free(expath);

    return 0;
}

static int prepfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char *expath = get_extended_path(path);

    DIR *dirp = opendir(expath);

    free(expath);

    struct dirent *ent;

    while ((ent = readdir(dirp)) != NULL) {
        filler(buf, ent->d_name, NULL, 0);
    }

    closedir(dirp);

    return 0;
}

static int prepfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char *expath = get_extended_path(path);

    char pp_temp[PATH_MAX + strlen(options.pp_path)];
    sprintf(pp_temp, options.pp_path, expath);

    free(expath);

    size = 0;

    FILE *fp = popen(pp_temp, "r");

    while (!feof(fp)) {
        fread(buf + size, sizeof(char), 1, fp);
        size += 1;
    }

    pclose(fp);

    size -= 1;

    return size;
}

static struct fuse_operations prepfs_oper = {
    .getattr    = prepfs_getattr,
    .readdir    = prepfs_readdir,
    .read        = prepfs_read,
};

int main(int argc, char *argv[])
{
    int ret;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    memset(&options, 0, sizeof(struct options));

    if (fuse_opt_parse(&args, &options, prepfs_opts, 0) == -1) {
        return -1;
    }

    if (!options.base_dir || !options.pp_path) {
        fprintf(stderr, "Usage: %s mountpoint -b basedir -pp preprocessor (eg, gpp -x \"%%s\")\n", argv[0]);
        return -1;
    }

    ret = fuse_main(args.argc, args.argv, &prepfs_oper, NULL);

    fuse_opt_free_args(&args);

    return ret;
}
