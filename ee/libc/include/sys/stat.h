/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * POSIX stat defines, types, and symbols.
 */

#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#ifndef __time_t_defined
typedef unsigned long time_t;
#define __time_t_defined
#endif

#ifndef __blkcnt_t_defined
typedef long long blkcnt_t;
#define __blkcnt_t_defined
#endif

#ifndef __blksize_t_defined
typedef signed blksize_t;
#define __blksize_t_defined
#endif

#ifndef __dev_t_defined
typedef int dev_t;
#define __dev_t_defined
#endif

#ifndef __gid_t_defined
typedef int gid_t;
#define __gid_t_defined
#endif

#ifndef __uid_t_defined
typedef int uid_t;
#define __uid_t_defined
#endif

#ifndef __ino_t_defined
typedef unsigned ino_t;
#define __ino_t_defined
#endif

#ifndef __mode_t_defined
typedef int mode_t;
#define __mode_t_defined
#endif

#ifndef __nlink_t_defined
typedef int nlink_t;
#define __nlink_t_defined
#endif

#ifndef __off_t_defined
typedef long long off_t;
#define __off_t_defined
#endif

/** type of file */
#define	S_IFMT		0170000
/** directory */
#define	S_IFDIR		0040000
/** regular */
#define	S_IFREG		0100000
/** symbolic link */
#define	S_IFLNK		0120000

#define	S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define	S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define	S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)

#define S_IRWXU  0700	/* Owner +rwx */
#define S_IRUSR  0400	/* Owner read */
#define S_IWUSR  0200	/* Owner write */
#define S_IXUSR  0100	/* Owner exec */
#define S_IRWXG   070	/* Group +rwx */
#define S_IRGRP   040	/* Group read */
#define S_IWGRP   020	/* Group write */
#define S_IXGRP   010	/* Group exec */
#define S_IRWXO    07	/* Everyone +rwx */
#define S_IROTH    04	/* Everyone read */
#define S_IWOTH    02	/* Everyone write */
#define S_IXOTH    01	/* Everyone exec */

#define S_ISUID 04000	/* Set User ID */
#define S_ISGID 02000	/* Set Group ID */
#define S_ISVTX 01000	/* Sticky bit */

struct stat {
	dev_t st_dev;		/* Device ID */
	ino_t st_ino;		/* File inode number */
	mode_t st_mode;		/* File mode */
	nlink_t st_nlink;	/* Number of hard links */

	uid_t st_uid;		/* User ID */
	gid_t st_gid;		/* Group ID */
	off_t st_size;		/* Size in bytes */

	time_t st_mtime;	/* modification time */
	time_t st_atime;	/* access time */
	time_t st_ctime;	/* creation time */

	blksize_t st_blksize;	/* Size of each block */
	blkcnt_t st_blocks;	/* Number of blocks */
};

	/* Unimplemented. Use stat() */
int     fstat(int fd, struct stat *sbuf);
int     mkdir(const char *path, mode_t mode);
int     stat(const char *path, struct stat *sbuf);


#endif /* SYS_STAT_H */
