/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# Declarations for externally defined I/O.
*/

#ifndef __KERNEL_UNISTD_H__
#define __KERNEL_UNISTD_H__

/** @defgroup unistd POSIX I/O functions.

    These functions are defined in either libfileio or libfileXio.*/

#include <sys/stat.h>

/** @addtogroup unistd
    @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/** Closes a file descriptor.

    @param  fd File descriptor to close

    @retval 0  Success
    @retval <0 Failure
*/
int close(int fd);

/** Creates a link from path.

    @param  path File to link
    @param  link Link to be made

    @retval 0    Success
    @retval <0   Failure

    @warning     fileio: Unimplemented
    @note        I'm not sure if hard or soft links are implementation defined
                 by the device's I/O driver.
*/
int link(const char *path, const char *link);

/** Reposition file offset.

    @param fd      File descriptor to reposition
    @param offset  Offset from whence.
    @param whence  Starting position for offset.

    @retval >=0    Offset in bytes from beginning of file
    @retval <0     Failure

    @note fileio:  Limited to INT_MAX.
    @note fileXio: Uses 64-bit offset to allow offsets up to UINT_MAX.

    @todo Add lseek64()
*/
off_t lseek(int fd, off_t offset, int whence);

/** Create a new directory.

    @param path Name of new directory to create
    @param mode Mode for the newly created directory

    @retval 0   Success
    @retval <0  Failure

    @warning    fileio: mode is not supported.
*/
int mkdir(const char *path, mode_t mode);

/** Opens a file descriptor specified by path.

    @param path  Name of file to open
    @param flags Flags to control the opening of the file see <sys/fcntl.h>
    @param mode  A single integer indicating file permissions

    @retval 0    Success
    @retval <0   Failure

    @warning     fileio: Permissions are not supported.
*/
int open(const char* path, int flags, ...);

/** Reads a file descriptor.

    @param fd    File descriptor to read
    @param buf   Buffer for the read
    @param count Number of bytes to read

    @retval >=0	 Number of bytes read
    @retval <0	 Failure
*/
int read(int fd, void* buf, size_t count);

/** Remove a file.

    @param path Name of file to be removed.

    @retval 0   Success
    @retval <0  Failure

    @warning    fileio: Has a major bug on unpatched kernels. The call falls 
			through to fioMkdir() so a subsequent check and removal 
			of a directory of the same filename is needed.
*/
int remove(const char *path);

/** Rename a file.

    @param old Name of file to rename
    @param new New name of file

    @retval 0  Success
    @retval <0 Failure

    @warning   fileio: Unimplemented.
*/
int rename(const char *old, const char *new);

/** Remove a directory.

    @param path Directory to be removed.

    @retval 0   Success
    @retval <0  Failure
*/
int rmdir(const char *path);


/** Retrieve information about a file.

   @param path  File for information retrieval.
   @param st    Buffer for information retrieved.

   @retval 0    Success
   @retval <0   Failure

   @warning     Only file access flags are supported at this time.
   @warning     fileio: does not suspend interrupts when using fioGetstat()

   @todo Convert PS2 time to Unix epoch time for stat.
*/
int stat(const char *path, struct stat *st);

/** Remove a link to a file.

    @param path Name of link to be removed.

    @retval 0   Success
    @retval <0  Failure

    @note       Utilizes remove() for removal.

    @warning    fileio: Has a major bug on unpatched kernels. The remove call 
			falls through to fioMkdir() so a subsequent check and
			removal of a directory of the same filename is needed.
*/
int unlink(const char *path);

/** Write information to a file.

   @param fd    An open file descriptor.
   @param buf   Buffer to write from.
   @param count Number of bytes to write.

   @retval >=0  Number of bytes written 
   @retval <0   Failure
*/
int write(int fd, const void *buf, size_t count);

/** End of addtogroup unistd
    @}
*/

#endif /* __KERNEL_UNISTD_H__ */
