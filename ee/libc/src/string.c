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
 * Standard ANSI string functions to complement the ASM funcs
 */

#include <limits.h>
#include <string.h>
#include <stdlib.h>

#if defined(F_strcoll) || defined(DOXYGEN)
int strcoll(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}
#endif

#if defined(F_strxfrm) || defined(DOXYGEN)
size_t strxfrm(char *dest, const char *src, size_t n)
{
	strncpy(dest, src, n);
	return n;
}
#endif

#if defined(F_strdup) || defined(DOXYGEN)
char *strdup(const char *s) {
	size_t str_size = strlen(s);
	char * r = (char *)malloc(str_size + 1);

	return strcpy(r, s);
}
#endif

#if defined(F_strcasecmp) || defined(DOXYGEN)
int	strcasecmp(const char * string1, const char * string2)
{
	while (*string1 != '\0' && tolower(*string1) == tolower(*string2))
    {
      string1++;
      string2++;
    }

	return tolower(*(unsigned char *) string1) - tolower(*(unsigned char *) string2);
}
#endif


#if defined(F_strncasecmp) || defined(DOXYGEN)
int	 strncasecmp(const char * string1, const char * string2, size_t n)
{
	if (n == 0)
		return 0;

	while ((n-- != 0) && (tolower(*string1) == tolower(*string2)))
    {
		if ((n == 0) || (*string1 == '\0') || (*string2 == '\0'))
			break;

		string1++;
		string2++;
	}

	return tolower(*(unsigned char *) string1) - tolower(*(unsigned char *) string2);
}
#endif

#if defined(F_strtok) || defined(DOXYGEN)
char* strtok(char * strToken, const char * strDelimit)
{
	static char* start;
	static char* end;

	if (strToken != NULL)
		start = strToken;
	else
	{
		if (*end == 0)
			return 0;

		start=end;
	}

	if(*start == 0)
	{
		return 0;
	}

	// Strip out any leading delimiters
	while (strchr(strDelimit, *start))
	{
		// If a character from the delimiting string
		// then skip past it

		start++;

		if (*start == 0)
			return 0;
	}

	if (*start == 0)
		return 0;

	end=start;


	while (*end != 0)
	{
		if (strchr(strDelimit, *end))
		{
			// if we find a delimiting character
			// before the end of the string, then
			// terminate the token and move the end
			// pointer to the next character
			*end = 0;
			end++;
			return start;
		}

		end++;
	}


	// reached the end of the string before finding a delimiter
	// so dont move on to the next character
	return start;
}
#endif

#if defined(F_strrchr) || defined(DOXYGEN)
char* strrchr(const char * string, int c)
{
	/* use the asm strchr to do strrchr */
	char* lastmatch;
	char* result;

	/* if char is never found then this will return 0 */
	/* if char is found then this will return the last matched location
	   before strchr returned 0 */

	lastmatch = 0;
	result = strchr(string,c);

	while ((int)result != 0)
	{
		lastmatch=result;
		result = strchr(lastmatch+1,c);
	}

	return lastmatch;
}
#endif

#if defined(F_strstr) || defined(DOXYGEN)
char *	strstr(const char * string, const char * substring)
{
	char* strpos;

	if (string == 0)
		return 0;

	if (strlen(substring)==0)
		return (char*)string;

	strpos = (char*)string;

	while (*strpos != 0)
	{
		if (strncmp(strpos, substring, strlen(substring)) == 0)
		{
			return strpos;
		}

		strpos++;
	}

	return 0;
}
#endif


#if defined(F_strupr) || defined(DOXYGEN)
char * strupr(char *str)
{
	char * strptr = str;

	// loop thru each char in string
	while (*strptr != '\0')
    {
      // if char is lowercase, convert to uppercase
      if(islower(*strptr))
      	*strptr = toupper(*strptr);
      strptr++;
    }

	return str;
}
#endif

#if defined(F_strlwr) || defined(DOXYGEN)
char * strlwr(char *str)
{
	char * strptr = str;

	// loop thru each char in string
	while (*strptr != '\0')
    {
      // if char is uppercase, convert to lowercase
      if(isupper(*strptr))
      	*strptr = tolower(*strptr);
      strptr++;
    }

	return str;
}
#endif


#if defined(F_tolower) || defined(DOXYGEN)
int tolower(int c)
{
	if (isupper(c))
		c+=32;

	return c;
}
#endif

#if defined(F_toupper) || defined(DOXYGEN)
int toupper(int c)
{
	if (islower(c))
		c-=32;

	return c;
}
#endif

#if defined(F_isupper) || defined(DOXYGEN)
int isupper(int c)
{
	if (c < 'A')
		return 0;

	if (c > 'Z')
		return 0;

	// passed both criteria, so it
	// is an upper case alpha char
	return 1;
}
#endif

#if defined(F_islower) || defined(DOXYGEN)
int islower(int c)
{
	if (c < 'a')
		return 0;

	if (c > 'z')
		return 0;

	// passed both criteria, so it
	// is a lower case alpha char
	return 1;
}
#endif

#if defined(F_isalpha) || defined(DOXYGEN)
int isalpha(int c)
{
	if (islower(c) || isupper(c))
		return 1;

	return 0;
}
#endif

#if defined(F_isdigit) || defined(DOXYGEN)
int isdigit(int c)
{
	if (c < '0')
		return 0;

	if (c > '9')
		return 0;

	// passed both criteria, so it
	// is a numerical char
	return 1;
}
#endif

#if defined(F_isalnum) || defined(DOXYGEN)
int isalnum(int c)
{
	if (isalpha(c) || isdigit(c))
		return 1;

	return 0;
}
#endif

#if defined(F_iscntrl) || defined(DOXYGEN)
int iscntrl(int c)
{
	if (c < 0x20)
		return 1;

	if (c == 0x7F)
		return 1;

	return 0;
}
#endif

#if defined(F_isgraph) || defined(DOXYGEN)
int isgraph(int c)
{
	if (iscntrl(c))
		return 0;

	if (isspace(c))
		return 0;

	return 1;
}
#endif

#if defined(F_isprint) || defined(DOXYGEN)
int isprint(int c)
{
	if (iscntrl(c))
		return 0;

	return 1;
}
#endif

#if defined(F_ispunct) || defined(DOXYGEN)
int ispunct(int c)
{
	if (iscntrl(c))
		return 0;

	if (isalnum(c))
		return 0;

	if (isspace(c))
		return 0;

	// It's a printable character
	// thats not alpha-numeric, or a space
	// so its a punctuation character
	return 1;
}
#endif

#if defined(F_isspace) || defined(DOXYGEN)
int isspace(int c)
{
	if ((c>=0x09) && (c<=0x0D))
		return 1;

	if (c==0x20)
		return 1;

	return 0;
}
#endif

#if defined(F_isxdigit) || defined(DOXYGEN)
int isxdigit(int c)
{
	if (isdigit(c))
		return 1;

	if ((c>='a') && (c<='f'))
		return 1;

	if ((c>='A') && (c<='F'))
		return 1;

	return 0;
}
#endif


// sjis<->ascii conversion routines by Peter Sandström

struct charmap_t {
	unsigned short sjis;
	unsigned char ascii;
};

#if defined(F__sjis_internals) || defined(DOXYGEN)
/* What was the original encoding?
   Assuming ISO-8859-1 or ISO-8859-15. */
struct charmap_t sjis_conversion[] = {
    { 0x4081, 0x20 }, // ' '
    { 0x6d81, 0x5B }, // '['
    { 0x6e81, 0x5D }, // ']'
    { 0x7c81, 0x2D }, // '-'
    { 0x5b81, 0xB0 }, // '°'
    { 0x4581, 0xA5 }, // '¥'
    { 0x4481, 0x2E }, // '.'
    { 0x7B81, 0x2B }, // '+'
    { 0x9681, 0x2A }, // '*'
    { 0x5E81, 0x2F }, // '/'
    { 0x4981, 0x21 }, // '!'
    { 0x6881, 0x22 }, // '"'
    { 0x9481, 0x23 }, // '#'
    { 0x9081, 0x24 }, // '$'
    { 0x9381, 0x25 }, // '%'
    { 0x9581, 0x26 }, // '&'
    { 0x6681, 0x27 }, // '''
    { 0x6981, 0x28 }, // '('
    { 0x6a81, 0x29 }, // ')'
    { 0x8181, 0x3D }, // '='
    { 0x6281, 0x7C }, // '|'
    { 0x8f81, 0x5C }, // '\'
    { 0x4881, 0x3F }, // '?'
    { 0x5181, 0x5F }, // '_'
    { 0x6f81, 0x7B }, // '{'
    { 0x7081, 0x7D }, // '}'
    { 0x9781, 0x40 }, // '@'
    { 0x4781, 0x3B }, // ';'
    { 0x4681, 0x3A }, // ':'
    { 0x8381, 0x3C }, // '<'
    { 0x8481, 0x3E }, // '>'
    { 0x4d81, 0x60 }, // '`'
    { 0, 0 }
};

unsigned char isSpecialSJIS(short sjis)
{
    struct charmap_t *s = &sjis_conversion[0];
    do {
	if (s->sjis == sjis) return s->ascii;
 	s++;
    } while (s->sjis != 0);
    return 0;
}

short isSpecialASCII(unsigned char ascii)
{
    struct charmap_t *s = &sjis_conversion[0];
    do {
	if (s->ascii == ascii) return s->sjis;
 	s++;
    } while (s->ascii != 0);
    return 0;
}
#else
extern struct charmap_t * sjis_conversion;
unsigned char isSpecialSJIS(short sjis);
short isSpecialASCII(unsigned char ascii);
#endif

#if defined(F_strcpy_ascii) || defined(DOXYGEN)
int strcpy_ascii(char* ascii_buff, const short* sjis_buff)
{
    int i;
    short ascii, sjis;

    int len = strlen((const char *)sjis_buff)/2;

    for (i=0;i<len;i++) {
	sjis = sjis_buff[i];
	if ((ascii = isSpecialSJIS(sjis)) != 0) {
	} else {
	    ascii = ((sjis & 0xFF00) >> 8) - 0x1f;
	    if (ascii>96) ascii--;
	}
	ascii_buff[i] = ascii;
    }
    ascii_buff[i]=0;
    return len;
}
#endif

#if defined(F_strcpy_sjis) || defined(DOXYGEN)
int strcpy_sjis(short* sjis_buff, const char* ascii_buff)
{
    int i;
    short ascii, sjis;

    int len = strlen(ascii_buff);

    for (i=0;i<len;i++)	{
	ascii = ascii_buff[i];
	if ((sjis = isSpecialASCII(ascii)) != 0) {
	} else {
	    if (ascii>96) ascii++;
	    sjis = ((ascii + 0x1f) << 8) | 0x82;
	}
        sjis_buff[i] = sjis;
    }
    sjis_buff[i]=0;
    return len;
}
#endif

#if defined(F_strpbrk) || defined(DOXYGEN)
char *strpbrk(const char *s, const char *accept)
{
    const char *needle;
    for (; *s; s++) {
        for (needle = accept; *needle; needle++) {
            if (*s == *needle)
                return (char *) s;
        }
    }

    return NULL;
}
#endif

#if defined(F_strspn) || defined(DOXYGEN)
size_t strspn(const char *s, const char *accept) {
    const char *c;

    for (c = s; *c; c++) {
        if (!strchr(accept, *c))
            return c - s;
    }

    return c - s;
}
#endif

#if defined(F_strcspn) || defined(DOXYGEN)
size_t strcspn(const char *s, const char *reject) {
    const char *c;

    for (c = s; *c; c++) {
        if (strchr(reject, *c))
            return c - s;
    }

    return c - s;
}
#endif

#if defined(F_ctype) || defined(DOXYGEN)
const unsigned char ctype[256] = {
    _C,     _C,    _C,    _C,    _C,    _C,    _C,    _C,
    _C,     _C|_S, _C|_S, _C|_S, _C|_S, _C|_S, _C,    _C,
    _C,     _C,    _C,    _C,    _C,    _C,    _C,    _C,
    _C,     _C,    _C,    _C,    _C,    _C,    _C,    _C,
    _S|_B,  _P,    _P,    _P,    _P,    _P,    _P,    _P,
    _P,	    _P,    _P,    _P,    _P,    _P,    _P,    _P,
    _N,	    _N,    _N,    _N,    _N,    _N,    _N,    _N,
    _N,	    _N,    _P,    _P,    _P,    _P,    _P,    _P,
    _P,	    _U|_X, _U|_X, _U|_X, _U|_X, _U|_X, _U|_X, _U,
    _U,	    _U,    _U,    _U,    _U,    _U,    _U,    _U,
    _U,	    _U,    _U,    _U,    _U,    _U,    _U,    _U,
    _U,	    _U,    _U,    _P,    _P,    _P,    _P,    _P,
    _P,	    _L|_X, _L|_X, _L|_X, _L|_X, _L|_X, _L|_X, _L,
    _L,	    _L,    _L,    _L,    _L,    _L,    _L,    _L,
    _L,	    _L,    _L,    _L,    _L,    _L,    _L,    _L,
    _L,	    _L,    _L,    _P,    _P,    _P,    _P,    _C
};
#endif
