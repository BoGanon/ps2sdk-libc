/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Simple standard C library implementation.
 */

#if defined(F_strerror) || defined(DOXYGEN)
#define E_USE_NAMES
#endif

#include "errno.h"

#include <fcntl.h>

#include <stdio.h>
#include <malloc.h>
#include <sio.h>

#include <string.h>
#include <limits.h>

void ps2sdk_stdio_init_();

/* Internal I/O flags. */
#define IOREAD                    0x0010
#define IOWRT                     0x0020
#define IORW                      0x0030
#define IOMYBUF                   0x0040
#define IONOBUF                   0x0080
#define IOBUFMASK                 0x00C0
#define IOEOF                     0x0100
#define IOERR                     0x0200

extern char direct_pwd_[256];
extern int stdio_initialised_;

typedef struct {
  int  fd;
  int  cnt;
  int  flag;
  int  has_putback;
  int  putback;
  char *buf;
  size_t size;
} file_;

#if defined(F_clearerr) || defined(DOXYGEN)
void clearerr(FILE *stream)
{
  file_ *f = (file_ *)stream;
  f->flag &= ~(IOERR|IOEOF);
}
#endif

#if defined(F_fclose) || defined(DOXYGEN)
int fclose(FILE *stream)
{
  file_ *f = (file_ *)stream;
  int ret = EOF;

  if (stream == stdout || stream == stderr)
    return 0;

  /* test the file stream. */
  if ((f->fd >= 0)) {
    if ((ret=close(f->fd)) >= 0) {
      if (f->flag & IOMYBUF)
        free(f->buf);
      f->fd = -1;
      f->cnt = 0;
      f->flag = 0;
      f->buf = NULL;
      ret = 0;
    } 
    else {
      ret = EOF;
    }
  }
  else {
    ret = EOF;
  }

  return (ret);
}
#endif


#if defined(F_fcloseall) || defined(DOXYGEN)
extern file_ iob_[];
int _fcloseall(void)
{
  int  i, ret = 0;
  file_ *iob;

  /* process all open files except for stdout, stdin and stderr. */
  for (i = 3, iob = iob_+3; i < _NFILE; ++i, ++iob) {
    if (iob->fd >= 0) {
      /* attempt to close the current file. */
      if ((fclose(iob) == 0) && (ret >= 0)) ++ret;
      else ret = EOF;
    }
  }
  return (ret);
}
#endif


#if defined(F_feof) || defined(DOXYGEN)
int feof(FILE *stream)
{
  file_ *f = (file_ *)stream;
  return ((f->flag & IOEOF) != 0);
}
#endif


#if defined(F_ferror) || defined(DOXYGEN)
int ferror(FILE *stream)
{
  file_ *f = (file_ *)stream;
  return ((f->flag & IOERR) != 0);
}
#endif

#if defined(F_fflush) || defined(DOXYGEN)
int fflush(FILE *stream)
{
  file_ *f = (file_ *)stream;

  if (f->flag & _IONBF)
    return 0;

  if (stream == stdin)
    /* Reset buffer. */
    f->cnt = 0;
  else if (f->flag & IOWRT) {
    /* Write contents of buffer to file. */
    /* unknown/invalid I/O buffer type. */
    errno = EBADF;
  }
  return (EOF);
}
#endif


#if defined(F_fflushall) || defined(DOXYGEN)
extern file_ iob_[];
int _fflushall(void)
{
  int  i, ret = 0;
  file_ *iob;

  /* process all open files except for stdout, stdin and stderr. */
  for (i = 3, iob = iob_+3; i < _NFILE; ++i, ++iob) {
    if (iob->fd >= 0) {
      /* attempt to flush the current file. */
      if ((fflush(iob) == 0) && (ret >= 0)) ++ret;
      else ret = EOF;
    }
  }
  return (ret);
}
#endif


#if defined(F_fgetc) || defined(DOXYGEN)
int fgetc(FILE *stream)
{
  unsigned char c;
  int ret;
  file_ *f = (file_ *)stream;

  if ((stream == stdout) || (stream == stderr)) {
      /* cannot read from stdout or stderr. */
      errno = EINVAL;
      f->flag = (f->flag & ~IOERR)|IOERR;
      ret = EOF;
  }

  if(fread(&c, 1, 1, f) == 1){
    ret = (int)c;
  }
  else{
    f->flag = (f->flag & ~IOEOF)|IOEOF;
    ret = EOF;
  }

  return (ret);
}
#endif


#if defined(F_fgetpos) || defined(DOXYGEN)
int fgetpos(FILE *stream, fpos_t *pos)
{
  long n;
  file_ *f = (file_ *)stream;

  n = (ftell(f) - (f->has_putback ? 1 : 0));
  if (n >= 0) *pos = (fpos_t)n;
  return ((n >= 0) ? 0 : -1);
}
#endif


#if defined(F_fgets) || defined(DOXYGEN)
char *fgets(char *buf, int n, FILE *stream)
{
  char *ret = buf;
  int  c, done;
  file_ *f = (file_ *)stream;

  if ((stream == stdout) || (stream == stderr)) {
    /* cannot read from stdout or stderr. */
    errno = EINVAL;
    ret = NULL;
  }

  //Read until either a newline character is encountered or n-1 characters are read.
  for (done = 0; (!done) && (n >= 2); ) {
    switch(c = fgetc(f)) {
      //Newline character translation: do nothing
      case '\r':
	break;
      /* newline terminates fgets. */
      case '\n':
	*buf++ = (char)c;
	n --;
	done = 1;
	break;
      /* end of file or error. */
      case EOF:
	//If no characters could be read, then return NULL.
	if(buf == ret)
	  ret = NULL;	
	done = 1;
	  break;
      /* store the current character to buf. */
      default:
	*buf++ = (char)c;
	--n;
    }
  }

  //Only if there was at least one character read and if there is enough space.
  if(ret != NULL && n >= 1)
    *buf = '\0';

  return (ret);
}
#endif

#if 0
/* Normalize a pathname by removing . and .. components, duplicated /, etc. */
char* ps2_normalize_path(char *path_name)
{
        int i, j;
        int first, next;
        static char out[255];

        /* First copy the path into our temp buffer */
        strcpy(out, path_name);
        /* Then append "/" to make the rest easier */
        strcat(out,"/");

        /* Convert "//" to "/" */
        for(i=0; out[i+1]; i++) {
                if(out[i]=='/' && out[i+1]=='/') {
                        for(j=i+1; out[j]; j++)
                                out[j] = out[j+1];
                        i--;
                ;}
        }

        /* Convert "/./" to "/" */
        for(i=0; out[i] && out[i+1] && out[i+2]; i++) {
                if(out[i]=='/' && out[i+1]=='.' && out[i+2]=='/') {
                        for(j=i+1; out[j]; j++)
                                out[j] = out[j+2];
                        i--;
                }
        }

        /* Convert "/path/../" to "/" until we can't anymore.  Also
         * convert leading "/../" to "/" */
        first = next = 0;
        while(1) {
                /* If a "../" follows, remove it and the parent */
                if(out[next+1] && out[next+1]=='.' &&
                   out[next+2] && out[next+2]=='.' &&
                   out[next+3] && out[next+3]=='/') {
                        for(j=0; out[first+j+1]; j++)
                                out[first+j+1] = out[next+j+4];
                        first = next = 0;
                        continue;
                }

                /* Find next slash */
                first = next;
                for(next=first+1; out[next] && out[next] != '/'; next++)
                        continue;
                if(!out[next]) break;
        }

        /* Remove trailing "/" */
        for(i=1; out[i]; i++)
                continue;
        if(i >= 1 && out[i-1] == '/')
                out[i-1] = 0;

        return (char*)out;
}
#endif

#if defined(F_fopen) || defined(DOXYGEN)
extern file_ iob_[];
/* TODO: Merge fopen and fdopen search for fd */
FILE *fopen(const char *fname, const char *mode)
{
  FILE *ret = NULL;
  int  fd = 0, flag = 0, i, iomode = 0;
  char mode_start = 0;

  // some people won't use our crt0...
  if (!stdio_initialised_)
    ps2sdk_stdio_init_();

  /* ensure file name and mode are not NULL strings. */
  if ((fname != NULL) && (*fname != '\0')) {
    if ((mode != NULL) && (*mode != '\0')) {
      mode_start = *mode;
      /* test the file mode. */
      switch(*mode++) {
        case 'r':
          flag = IOREAD;
          iomode = O_RDONLY;
          break;
        case 'w':
          flag = IOWRT;
          iomode = (O_WRONLY | O_CREAT | O_TRUNC);
          break;
        case 'a':
          flag = IOWRT;
          iomode = (O_WRONLY | O_APPEND | O_CREAT) ;
          break;
        default:
          errno = EINVAL;
          return ret;
      } // switch
      /* test the extended file mode. */
      for (; (*mode != '\0'); ) {
        switch(*mode++) {
          case 'b':
            continue;
          case '+':
            flag = (flag & ~IORW) | IORW;
            if (mode_start == 'r')
              iomode = (O_RDWR);
            if (mode_start == 'w')
              iomode = (O_RDWR | O_CREAT | O_TRUNC);
            if (mode_start == 'a')
              iomode = (O_RDWR | O_APPEND | O_CREAT);
            continue;
          default:
            break;
        } // switch
      } // for
      /* search for an available fd slot. */
      for (i = 3; i < _NFILE; ++i) if (iob_[i].fd < 0) break;
      if (i < _NFILE) {
        if ((fd = open((char *)fname, iomode, 0666)) >= 0) {
          iob_[i].fd = fd;
          iob_[i].cnt = 0;
          iob_[i].flag = flag | _IONBF;
          iob_[i].has_putback = 0;
          ret = &iob_[i];
        }
      } else {
        errno = ENFILE;
      }
    } else {
      errno = EINVAL;
    }
  } else {
    errno = EINVAL;
  }
  return (ret);
}
#endif


#if defined(F_fdopen) || defined(DOXYGEN)
extern file_ iob_[];
FILE *fdopen(int fd, const char *mode)
{
  FILE *ret = NULL;
  int  flag = 0, i, iomode = 0;;
  char mode_start = 0;

  if (fd >= 0 && fd < 3) {
    ret = iob_ + fd;
    return ret;
  }

  if (!stdio_initialised_)
    ps2sdk_stdio_init_();

  /* ensure valid descriptor, and that mode is not a NULL string. */
  if (fd >= 0) {
    if ((mode != NULL) && (*mode != '\0')) {
      mode_start = *mode;
      /* test the file mode. */
      switch(*mode++) {
        case 'r':
          flag = IOREAD;
          iomode = O_RDONLY;
          break;
        case 'w':
          flag = IOWRT;
          iomode = (O_WRONLY | O_CREAT | O_TRUNC);
          break;
        case 'a':
          flag = IOWRT;
          iomode = (O_WRONLY | O_APPEND | O_CREAT) ;
          break;
        default:
          errno = EINVAL;
          return NULL;
      } // switch
      /* test the extended file mode. */
      for (; (*mode != '\0'); ) {
        switch(*mode++) {
          case 'b':
            continue;
          case '+':
            flag = (flag & ~IORW) | IORW;
            if (mode_start == 'r')
              iomode = (O_RDWR);
            if (mode_start == 'w')
              iomode = (O_RDWR | O_CREAT | O_TRUNC);
            if (mode_start == 'a')
              iomode = (O_RDWR | O_APPEND | O_CREAT);
            continue;
          default:
            break;
        } // switch
      } // for
      /* search for an available fd slot. */
      for (i = 3; i < _NFILE; ++i) if (iob_[i].fd < 0) break;
      if (i < _NFILE) {
        iob_[i].fd = fd;
        iob_[i].cnt = 0;
        iob_[i].flag = flag | _IONBF;
        iob_[i].has_putback = 0;
        ret = (iob_ + i);
      } else {
        errno = ENFILE;
      }
    } else {
      errno = EINVAL;
    }
  } else {
    errno = EBADF;
  }

  return (ret);
}
#endif

#if defined(F_fileno) || defined(DOXYGEN)
int fileno(FILE *stream)
{
  file_ *f = (file_ *)stream;

  if (f->fd > 0)
    return f->fd;
  else
    errno = EBADF;

  return -1;
}
#endif

#if defined(F_fputc) || defined(DOXYGEN)
int fputc(int c, FILE *stream)
{
  unsigned char ch = (unsigned char)c;
  return ((fwrite(&ch, 1, 1, stream) == 1) ? 0 : EOF);
}
#endif


#if defined(F_fputs) || defined(DOXYGEN)
int fputs(const char *s, FILE *stream)
{
  size_t len;
  int temp = strlen(s);

  len = ((fwrite(s, 1, temp, stream) == temp) ? temp : EOF);

  return len;
}
#endif


#if defined(F_fread) || defined(DOXYGEN)
size_t fread(void *buf, size_t r, size_t n, FILE *stream)
{
  size_t ret = 0, read_len = r * n;
  int num_read;
  file_ *f = (file_ *)stream;

  /* cannot read from stdout or stderr. */
  if ((stream == stdout) || (stream == stderr))
    return 0;

  /* attempt to read from the stream file. */
  if (f->has_putback) {
    unsigned char *ptr = (unsigned char *)buf;
    *ptr = f->putback;
    buf = ptr + 1;
    f->has_putback = 0;
    ret++;
    /* subtract 1 to read_len to avoid buffer overflow */
    read_len--;
  }
  num_read = read(f->fd, buf, read_len);
  if (num_read < 0)
    num_read = errno = -1 * num_read;
  else if(num_read < read_len)
    f->flag |= IOEOF;

  ret +=  num_read / r;

  return (ret);
}
#endif


#if defined(F_fseek) || defined(DOXYGEN)
int fseek(FILE *stream, long offset, int origin)
{
  int ret;
  long cur = 0;
  file_ *f = (file_ *)stream;

  if ((stream == stdout) || (stream == stderr)) {
    errno = EBADF;
    /* cannot seek stdout or stderr. */
    return -1;
  }

  /* attempt to seek to offset from current offset. */
  if (origin == SEEK_CUR) {
    cur = lseek(f->fd, 0, origin);

    /* Should probably be INT_MAX... */
    if (offset > LONG_MAX-cur) {
      errno = EINVAL;
      return -1;
    }
    cur += offset;
    ret = lseek(f->fd, (int)offset, origin);

    if (cur == ret) {
      f->has_putback = 0;
      f->flag &= ~IOEOF;
      return 0;
    }
  }

  /* seek to end or from beginning. */
  ret = lseek(f->fd, (int)offset, origin);

  if (ret < 0)
    return -1;

  f->has_putback = 0;
  f->flag &= ~IOEOF;

  return 0;
}
#endif


#if defined(F_fsetpos) || defined(DOXYGEN)
int fsetpos(FILE *stream, const fpos_t *pos)
{
  file_ *f = (file_ *)stream;
  f->has_putback = 0;
  return (fseek(stream, (long)*pos, SEEK_SET));
}
#endif

#if defined(F_ftell) || defined(DOXYGEN)
long ftell(FILE *stream)
{
  long n, ret = -1L;
  file_ *f = (file_*)stream;

  if ((stream == stdout) || (stream == stderr)) {
    /* stdout or stderr is an invalid seek stream argument. */
    errno = EINVAL;
    return -1L;
  }

  if (f->fd < 0) {
    /* file is not open. */
    errno = EBADF;
    ret = -1L;
  }
  else {
    if ((n = lseek(f->fd, 0, SEEK_CUR)) >= 0)
        ret = (long)n;
    else if (n < 0) {
        errno = (n * -1);
          ret = -1L;
    }
    if ((n >= 0) && f->has_putback) ret--;
  }

  return (ret);
}
#endif


#if defined(F_fwrite) || defined(DOXYGEN)
size_t fwrite(const void *buf, size_t r, size_t n, FILE *stream)
{
#ifdef USE_GS
  size_t i, len;
#endif
  int written = 0;
  file_ *f = (file_ *)stream;

  if (f->has_putback)
  {
      fseek(f, -1, SEEK_CUR);
      f->has_putback = 0;
  }

  /* write to stdout. */
#ifdef USE_GS
  for (i = 0, len = (r * n); i < len; ++i) putchar((int)((char *)buf)[i]);
  written = r;
#elif defined(USE_SIO)
  for (i = 0, len = (r * n); i < len; ++i) sio_putc((int)((char *)buf)[i]);
  written = r;
#else
  written = (write(f->fd, (void *)buf, (int)(r * n)) / (int)r);
#endif

  if (written < 0)
     written = 0;

  return written;
}
#endif


#if defined(F_getc) || defined(DOXYGEN)
int getc(FILE *stream)
{
  unsigned char c;
  int  ret;

  if ((stream == stdout) || (stream == stderr))
      /* cannot read from stdout or stderr. */
      return EOF;

  ret = ((fread(&c, 1, 1, stream) == 1) ? (int)c : EOF);
  
  return (ret);
}
#endif


#if defined(F_getchar) || defined(DOXYGEN)
int getchar(void)
{
  return (getc(stdin));
}
#endif


#if defined(F_gets) || defined(DOXYGEN)
char *gets(char *buf)
{
  char *str = NULL;
  char c = 0;

  if ((str = fgets(buf, INT_MAX, stdin)) != NULL) {
    /* remove the trailing new line (if it exists) */
    c = str[strlen(str) - 1];
    if (c == '\n' || c == '\r')
      str[strlen(str) - 1] = '\0';
  }
  return (str);
}
#endif


#if defined(F_strerror) || defined(DOXYGEN)
char *strerror(int err) {
    return error_to_string(err);
}
#endif


#if defined(F_perror) || defined(DOXYGEN)
void perror(const char *s)
{
  char *err;

  /* print to stderr output. */
  if ((err = strerror(errno)) != NULL) fprintf(stderr, "%s : ", err);
  fputs(s, stderr);
}
#endif


#if defined(F_putc) || defined(DOXYGEN)
/* std I/O data variable. */
#ifdef USE_GS
extern int stdio_stdout_xy_[2];

/** stdio internal function. */
void stdio_update_stdout_xy_(int, int);
#endif

int putc(int c, FILE *stream)
{
  char ch;
  int  ret = 0;

#ifdef USE_GS
  /* print one character to stdout. */
  switch((char)c) {
    case '\a':
      ret = (int)c;
      break;
    case '\b':
      ret = (int)c;
      break
    case '\n':
        /* newline. */
    case '\r':
      /* carriage return. */
      stdio_update_stdout_xy_(0, (stdio_stdout_xy_[1] + 1));
      ret = (int)c;
      break;
    case '\t':
      stdio_update_stdout_xy_((stdio_stdout_xy_[0] + 2), stdio_stdout_xy_[1]);
      ret = (int)c;
      break;
    case '\v':
      stdio_update_stdout_xy_(stdio_stdout_xy_[0], (stdio_stdout_xy_[1] + 2));
      ret = (int)c;
      break;
    default:
      /* write one character to the screen. */
      gsCharOut((stdio_stdout_xy_[0] * 16), (stdio_stdout_xy_[1] * 16), c);
      stdio_update_stdout_xy_((stdio_stdout_xy_[0] + 1), stdio_stdout_xy_[1]);
      ret = (int)c;
  }
#endif
  /* write one character to the stream file. */
  ch = (char)c;
  ret = ((fwrite(&ch, 1, 1, stream) == 1) ? c : EOF);

  return (ret);
}
#endif


#if defined(F_putchar) || defined(DOXYGEN)
int putchar(int c)
{
  return (putc(c, stdout));
}
#endif


#if defined(F_puts) || defined(DOXYGEN)
int puts(const char *s)
{
  int ret;

  for (ret = 0; (*s != '\0'); ++s) {
    /* attempt to print the current character to stdout. */
    if ((putchar(*s) == (int)*s) && (ret >= 0)) ++ret;
    else ret = EOF;
  }
  if ((putchar('\n') == '\n') && (ret >= 0)) ++ret;
  else ret = EOF;
  return (ret);
}
#endif

#if defined(F_rewind) || defined(DOXYGEN)
void rewind(FILE *stream)
{
  fseek(stream, 0, SEEK_SET);
}
#endif

#if defined(F_setbuf) || defined(DOXYGEN)
int setbuf(FILE* stream, char* buf)
{
  if (buf != NULL)
    return setvbuf(stream, buf, _IOFBF, BUFSIZ);

  return setvbuf(stream, NULL, _IONBF, 0);
}
#endif

#if defined(F_setvbuf) || defined(DOXYGEN)
int setvbuf(FILE * stream, char *buf, int type, size_t size)
{
  file_ *f = (file_*)stream;

  // File stream was used.
  if (f->flag & IONOBUF)
    return -1;

  // Nonbuffered, ignore buf and size 
  if (type == _IONBF ) {

    f->flag = (f->flag & ~(_IOFBF|_IOLBF|_IONBF)) | _IONBF;
    f->flag = (f->flag & ~(IOBUFMASK)) | IONOBUF;
    return 0;
  }

  // Buffered
  if ((type == _IOLBF) || (type == _IOFBF)) {

    if (f->flag & IONOBUF)
      return -1;

     if (buf == NULL) {
       if (size) {
         f->buf = (char*)malloc(size);
         f->size = size;
       }
       else {
         f->buf = (char*)malloc(BUFSIZ);
	 f->size = BUFSIZ;

         if (f->buf == NULL) {
           f->size = 0;
           return -1;
         }
       }
       // This is our buffer.
       f->flag = (f->flag & ~IOBUFMASK) | IOMYBUF | IONOBUF;
     }
     else {
       f->buf = buf;

       if (size)
         f->size = size;
       else
         f->cnt = BUFSIZ;

       f->flag = (f->flag & ~IOBUFMASK) | IONOBUF;
     }

    f->flag = (f->flag & ~(_IOFBF|_IOLBF|_IONBF)) | type;

    return 0;
  }

  return -1;
}
#endif

#if defined(F_skipatoi) || defined(DOXYGEN)
/**
 * @param s the pointer to the source string pointer.
 */
int __stdio_skip_atoi(const char **s)
{
  int ret = 0;

  for (; (isdigit(**s) != 0); ) ret = ((ret * 10) + (*((*s)++) - '0'));
  return (ret);
}
#endif


#if defined(F_tmpfile) || defined(DOXYGEN)
/* stdio temp name variable. */
extern char stdio_tmpnam_[256];

FILE *tmpfile(void)
{
  return ((tmpnam(NULL) != NULL) ?  fopen(stdio_tmpnam_, "rw+") : NULL);
}
#endif


#if defined(F_tmpnam) || defined(DOXYGEN)
/* stdio temp name variable. */
extern char stdio_tmpnam_[256];

char *tmpnam(char *name)
{
  char *ret = NULL;

  return (ret);
}
#endif


#if defined(F_ungetc) || defined(DOXYGEN)
int ungetc(int c, FILE *stream)
{
  file_ *f = (file_ *)stream;

  if (c == EOF || f->has_putback) {
    /* invalid input, or putback queue full */
    return EOF;
  }

  f->putback = c;
  f->has_putback = 1;
  return c;
}
#endif


#if defined(F_updatestdoutxy) || defined(DOXYGEN)
/* std I/O data variable. */
#ifdef USE_GS
int  stdio_stdout_xy_[2];
#endif
/* Updates the stdout (x, y) screen coordinates.
 * @param x the x screen coordinate.
 * @param y the y screen coordinate.
*/
void stdio_update_stdout_xy__(int x, int y)
{
#ifdef USE_GS
  if ((x * 16) >= gsGetDisplayWidth()) {
    x = 0;
    ++y;
  }
  if ((y * 16) >= gsGetDisplayHeight()) y = 0;
  stdio_stdout_xy__[0] = x;
  stdio_stdout_xy__[1] = y;
#endif
}
#endif


#if defined(F_stdio_internals) || defined(DOXYGEN)
/* stdio data variables. */
int stdio_initialised_ = 0;

/* the present working directory variable. */
char direct_pwd_[256] = "";
char stdio_tmpnam[256];

file_ iob_[_NFILE] = {
  {  0, 0, _IONBF, 0, 0, NULL, 0 }, // stdin
  {  1, 0, _IONBF, 0, 0, NULL, 0 }, // stdout
  {  2, 0, _IONBF, 0, 0, NULL, 0 }, // stderr
};

FILE *stdin = &iob_[0];
FILE *stdout = &iob_[1];
FILE *stderr = &iob_[2];

void ps2sdk_stdio_init_()
{
    int i;

    for (i = 3; i < _NFILE; i++) {
	iob_[i].fd = -1;
	iob_[i].cnt = 0;
	iob_[i].flag = _IONBF;
	iob_[i].has_putback = 0;
	iob_[i].putback = 0;
	iob_[i].buf = NULL;
	iob_[i].size = 0;
    }

    stdio_initialised_ = 1;
}

void ps2sdk_stdio_deinit_()
{
}
#endif

#if defined(F_chdir) || defined(DOXYGEN)
int chdir(const char *path) {
    strcpy(direct_pwd_, path);
    return 0;
}
#endif
