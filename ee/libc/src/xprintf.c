/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Some portions of this file Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Various *printf functions.
 */

/* Code borrowed from mysql's xprintf.c, by Richard Hipp */
/* This xprintf.c file on which this one is based is in public domain. */

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stddef.h>

#ifndef PS2LIB_STR_MAX
#define PS2LIB_STR_MAX 4096
#endif

/*
** The maximum number of digits of accuracy in a floating-point conversion.
*/
#define MAXDIG 20

#if defined(F_vxprintf) || defined(DOXYGEN)
/*
** Conversion types fall into various categories as defined by the
** following enumeration.
*/

enum e_type {    /* The type of the format field */
   RADIX,            /* Integer types.  %d, %x, %o, and so forth */
   FLOAT,            /* Floating point.  %f */
   EXP,              /* Exponentional notation. %e and %E */
   GENERIC,          /* Floating or exponential, depending on exponent. %g */
   SIZE,             /* Return number of characters processed so far. %n */
   STRING,           /* Strings. %s */
   PERCENT,          /* Percent symbol. %% */
   CHAR,             /* Characters. %c */
   ERROR,            /* Used to indicate no such conversion type */
/* The rest are extensions, not normally found in printf() */
   CHARLIT,          /* Literal characters.  %' */
   SEEIT,            /* Strings with visible control characters. %S */
   MEM_STRING,       /* A string which should be deleted after use. %z */
   ORDINAL,          /* 1st, 2nd, 3rd and so forth */
};

/*
** Each builtin conversion character (ex: the 'd' in "%d") is described
** by an instance of the following structure
*/
typedef struct s_info {   /* Information about each format field */
  int  fmttype;              /* The format field code letter */
  int  base;                 /* The base for radix conversion */
  char *charset;             /* The character set for conversion */
  int  flag_signed;          /* Is the quantity signed? */
  char *prefix;              /* Prefix on non-zero values in alt format */
  enum e_type type;          /* Conversion paradigm */
} info;

/*
** The following table is searched linearly, so it is good to put the
** most frequently used conversion types first.
*/
static info fmtinfo[] = {
  { 'd',  10,  "0123456789",       1,    0, RADIX,      },
  { 's',   0,  0,                  0,    0, STRING,     },
  { 'S',   0,  0,                  0,    0, SEEIT,      },
  { 'z',   0,  0,                  0,    0, MEM_STRING, },
  { 'c',   0,  0,                  0,    0, CHAR,       },
  { 'o',   8,  "01234567",         0,  "0", RADIX,      },
  { 'u',  10,  "0123456789",       0,    0, RADIX,      },
  { 'x',  16,  "0123456789abcdef", 0, "x0", RADIX,      },
  { 'X',  16,  "0123456789ABCDEF", 0, "X0", RADIX,      },
  { 'r',  10,  "0123456789",       0,    0, ORDINAL,    },
  { 'f',   0,  0,                  1,    0, FLOAT,      },
  { 'e',   0,  "e",                1,    0, EXP,        },
  { 'E',   0,  "E",                1,    0, EXP,        },
  { 'g',   0,  "e",                1,    0, GENERIC,    },
  { 'G',   0,  "E",                1,    0, GENERIC,    },
  { 'i',  10,  "0123456789",       1,    0, RADIX,      },
  { 'n',   0,  0,                  0,    0, SIZE,       },
  { 'S',   0,  0,                  0,    0, SEEIT,      },
  { '%',   0,  0,                  0,    0, PERCENT,    },
  { 'b',   2,  "01",               0, "b0", RADIX,      }, /* Binary notation */
  { 'p',  16,  "0123456789ABCDEF", 0, "x0", RADIX,      }, /* Pointers */
  { '\'',  0,  0,                  0,    0, CHARLIT,    }, /* Literal char */
};
#define NINFO  (sizeof(fmtinfo)/sizeof(info))  /* Size of the fmtinfo table */

/*
** If NOFLOATINGPOINT is defined, then none of the floating point
** conversions will work.
*/
#ifndef NOFLOATINGPOINT
/*
** "*val" is a double such that 0.1 <= *val < 10.0
** Return the ascii code for the leading digit of *val, then
** multiply "*val" by 10.0 to renormalize.
**
** Example:
**     input:     *val = 3.14159
**     output:    *val = 1.4159    function return = '3'
**
** The counter *cnt is incremented each time.  After counter exceeds
** 16 (the number of significant digits in a 64-bit float) '0' is
** always returned.
*/
static int getdigit(long double *val, int *cnt){
  int digit;
  long double d;
  if( (*cnt)++ >= MAXDIG ) return '0';
  digit = (int)*val;
  d = digit;
  digit += '0';
  *val = (*val - d)*10.0;
  return digit;
}
#endif

/*
** Setting the size of the BUFFER involves trade-offs.  No %d or %f
** conversion can have more than BUFSIZE characters.  If the field
** width is larger than BUFSIZE, it is silently shortened.  On the
** other hand, this routine consumes more stack space with larger
** BUFSIZEs.  If you have some threads for which you want to minimize
** stack space, you should keep BUFSIZE small.
*/
#define BUFSIZE 100  /* Size of the output buffer */

/*
** The root program.  All variations call this core.
**
** INPUTS:
**   func   This is a pointer to a function taking three arguments
**            1. A pointer to the list of characters to be output
**               (Note, this list is NOT null terminated.)
**            2. An integer number of characters to be output.
**               (Note: This number might be zero.)
**            3. A pointer to anything.  Same as the "arg" parameter.
**
**   arg    This is the pointer to anything which will be passed as the
**          third argument to "func".  Use it for whatever you like.
**
**   fmt    This is the format string, as in the usual print.
**
**   ap     This is a pointer to a list of arguments.  Same as in
**          vfprint.
**
** OUTPUTS:
**          The return value is the total number of characters sent to
**          the function "func".  Returns -1 on a error.
**
** Note that the order in which automatic variables are declared below
** seems to make a big difference in determining how fast this beast
** will run.
*/

enum e_pf_flags {
/* True if "-" flag is present */
PF_FLAG_LEFTJUSTIFY    = 1,
/* True if "+" flag is present */
PF_FLAG_PLUSSIGN       = 2,
/* True if " " flag is present */
PF_FLAG_BLANKSIGN      = 4,
/* True if "#" flag is present */
PF_FLAG_ALTERNATEFORM  = 8,
/* True if field width constant starts with zero */
PF_FLAG_ZEROPAD        = 16,
/* True if "l" flag is present */
PF_FLAG_LONG           = 32,
/* True if "ll" flag is present */
PF_FLAG_LONGLONG       = 64,
/* True if "=" flag is present */
PF_FLAG_CENTER         = 128,
/* True if decimal point should be shown */
PF_FLAG_DP             = 256,
/* True if trailing zeros should be removed */
PF_FLAG_RTZ            = 512,
/* True to force display of the exponent */
PF_FLAG_EXP            = 1024,
/* True if an error is encountered */
PF_FLAG_ERROR          = 2048
};

int vxprintf(func,arg,format,ap)
  void (*func)(char*,int,void*);
  void *arg;
  const char *format;
  va_list ap;
{
  register const char *fmt;   /* The format string. */
  register int c;             /* Next character in the format string */
  register char *bufpt;       /* Pointer to the conversion buffer */
  register int  precision;    /* Precision of the current field */
  register int  length;       /* Length of the field */
  register int  idx;          /* A general purpose loop counter */
  register enum e_pf_flags flags = 0;  /* Flags */
  int count;                  /* Total number of characters output */
  int width;                  /* Width of the current field */
  unsigned long long llvalue; /* Value for integer types */

  long double realvalue;      /* Value for real types */
  info *infop;                /* Pointer to the appropriate info structure */
  char buf[BUFSIZE];          /* Conversion buffer */
  char prefix;                /* Prefix character. "+" or "-" or " " or '\0'. */
  enum e_type xtype;          /* Conversion paradigm */
  char *zMem = 0;             /* String to be freed */
  static char spaces[] =
     "                                                    ";
#define SPACESIZE (sizeof(spaces)-1)
#ifndef NOFLOATINGPOINT
  int  exp;                 /* exponent of real numbers */
  long double rounder;      /* Used for rounding floating point values */
  int nsd;                  /* Number of significant digits returned */
#endif

  fmt = format;                     /* Put in a register for speed */
  count = length = 0;
  bufpt = 0;
  for(; (c=(*fmt))!=0; ++fmt){
    flags = 0;
    if( c!='%' ){
      register int amt;
      bufpt = (char *)fmt;
      amt = 1;
      while( (c=(*++fmt))!='%' && c!=0 ) amt++;
      (*func)(bufpt,amt,arg);
      count += amt;
      if( c==0 ) break;
    }
    if( (c=(*++fmt))==0 ){
      flags = (flags & ~PF_FLAG_ERROR) | PF_FLAG_ERROR;
      (*func)("%",1,arg);
      count++;
      break;
    }
    /* Find out what flags are present */
    do{
      switch( c ){
        case '-':   flags = (flags & ~PF_FLAG_LEFTJUSTIFY) | PF_FLAG_LEFTJUSTIFY;     c = 0;   break;
        case '+':   flags = (flags & ~PF_FLAG_PLUSSIGN) | PF_FLAG_PLUSSIGN;           c = 0;   break;
        case ' ':   flags = (flags & ~PF_FLAG_BLANKSIGN) | PF_FLAG_BLANKSIGN;         c = 0;   break;
        case '#':   flags = (flags & ~PF_FLAG_ALTERNATEFORM) | PF_FLAG_ALTERNATEFORM; c = 0;   break;
        case '0':   flags = (flags & ~PF_FLAG_ZEROPAD) | PF_FLAG_ZEROPAD;             c = 0;   break;
        case '=':   flags = (flags & ~PF_FLAG_CENTER) | PF_FLAG_CENTER;               c = 0;   break;
        default:                                       break;
      }
    }while( c==0 && (c=(*++fmt))!=0 );
    if( (flags & PF_FLAG_CENTER) )
      flags &= ~PF_FLAG_LEFTJUSTIFY;
    /* Get the field width */
    width = 0;
    if( c=='*' ){
      width = va_arg(ap,int);
      if( width<0 ){
        flags = (flags & ~PF_FLAG_LEFTJUSTIFY) | PF_FLAG_LEFTJUSTIFY;
        width = -width;
      }
      c = *++fmt;
    }else{
      while( isdigit(c) ){
        width = width*10 + c - '0';
        c = *++fmt;
      }
    }
    if( width > BUFSIZE-10 ){
      width = BUFSIZE-10;
    }
    /* Get the precision */
    if( c=='.' ){
      precision = 0;
      c = *++fmt;
      if( c=='*' ){
        precision = va_arg(ap,int);
#ifndef COMPATIBILITY
        /* This is sensible, but SUN OS 4.1 doesn't do it. */
        if( precision<0 ) precision = -precision;
#endif
        c = *++fmt;
      }else{
        while( isdigit(c) ){
          precision = precision*10 + c - '0';
          c = *++fmt;
        }
      }
      /* Limit the precision to prevent overflowing buf[] during conversion */
      if( precision>BUFSIZE-40 ) precision = BUFSIZE-40;
    }else{
      precision = -1;
    }
    /* Get the conversion type modifier */
    if( c=='l' ){
      c = *++fmt;
      /* Support to parse %ll */
      if (c=='l' ){
        flags = (flags & ~PF_FLAG_LONGLONG) | PF_FLAG_LONGLONG;
        c = *++fmt;
      }else{
        flags = (flags & ~PF_FLAG_LONG) | PF_FLAG_LONG;
      }
    }

    /* Fetch the info entry for the field */
    infop = 0;
    for(idx=0; idx<NINFO; idx++){
      if( c==fmtinfo[idx].fmttype ){
        infop = &fmtinfo[idx];
        break;
      }
    }
    /* No info entry found.  It must be an error. */
    if( infop==0 ){
      xtype = ERROR;
    }else{
      xtype = infop->type;
    }

    /*
    ** At this point, variables are initialized as follows:
    **
    **   flag_alternateform          TRUE if a '#' is present.
    **   flag_plussign               TRUE if a '+' is present.
    **   flag_leftjustify            TRUE if a '-' is present or if the
    **                               field width was negative.
    **   flag_zeropad                TRUE if the width began with 0.
    **   flag_long                   TRUE if the letter 'l' (ell) prefixed
    **                               the conversion character.
    **   flag_blanksign              TRUE if a ' ' is present.
    **   width                       The specified field width.  This is
    **                               always non-negative.  Zero is the default.
    **   precision                   The specified precision.  The default
    **                               is -1.
    **   xtype                       The class of the conversion.
    **   infop                       Pointer to the appropriate info struct.
    */
    switch( xtype ){
      case ORDINAL:
      case RADIX:
        if( flags & PF_FLAG_LONG ){
            if( infop->flag_signed ){
	      signed long t = va_arg(ap,signed long);
	      llvalue = t;
	    }else{
	      unsigned long t = va_arg(ap,unsigned long);
	      llvalue = t;
	    }
	}else if( flags & PF_FLAG_LONGLONG ){
            if( infop->flag_signed ){
	      signed long long t = va_arg(ap,signed long long);
	      llvalue = t;
	    }else{
	      unsigned long long t = va_arg(ap,unsigned long long);
	      llvalue = t;
	    }
	}else{
	  if( infop->flag_signed ){
	    signed int t = va_arg(ap,signed int) & 0xffffffffUL;
	    llvalue = t;
	  }else{
	    unsigned int t = va_arg(ap,unsigned int) & 0xffffffffUL;
	    llvalue = t;
	  }
	}
#ifdef COMPATIBILITY
        /* For the format %#x, the value zero is printed "0" not "0x0".
        ** I think this is stupid. */
        if( llvalue==0 ) flags &= ~PF_FLAG_ALTERNATEFORM;
#else
        /* More sensible: turn off the prefix for octal (to prevent "00"),
        ** but leave the prefix for hex. */
        if( llvalue==0 && infop->base==8 ) flags &= ~PF_FLAG_ALTERNATEFORM;
#endif
        if( infop->flag_signed ){
          if( *(long long*)&llvalue<0 ){
	    llvalue = -*(long long*)&llvalue;
            prefix = '-';
          }else if( flags & PF_FLAG_PLUSSIGN )  prefix = '+';
          else if( flags & PF_FLAG_BLANKSIGN )  prefix = ' ';
          else                       prefix = 0;
        }else                        prefix = 0;
        if( (flags & PF_FLAG_ZEROPAD) && (precision<width-(prefix!=0)) ){
          precision = width-(prefix!=0);
	}
        bufpt = &buf[BUFSIZE];
        if( xtype==ORDINAL ){
          long a,b;
          a = llvalue%10;
          b = llvalue%100;
          bufpt -= 2;
          if( a==0 || a>3 || (b>10 && b<14) ){
            bufpt[0] = 't';
            bufpt[1] = 'h';
          }else if( a==1 ){
            bufpt[0] = 's';
            bufpt[1] = 't';
          }else if( a==2 ){
            bufpt[0] = 'n';
            bufpt[1] = 'd';
          }else if( a==3 ){
            bufpt[0] = 'r';
            bufpt[1] = 'd';
          }
        }
        {
          register char *cset;      /* Use registers for speed */
          register int base;
          cset = infop->charset;
          base = infop->base;
          do{                                           /* Convert to ascii */
            *(--bufpt) = cset[llvalue%base];
            llvalue = llvalue/base;
          }while( llvalue>0 );
	}
        length = (int)(&buf[BUFSIZE]-bufpt);
	if(infop->fmttype == 'p'){
	  precision = 8;
	  if (!(flags & PF_FLAG_ALTERNATEFORM)) flags |= PF_FLAG_ALTERNATEFORM;
        }

        for(idx=precision-length; idx>0; idx--){
          *(--bufpt) = '0';                             /* Zero pad */
	}
        if( prefix ) *(--bufpt) = prefix;               /* Add sign */
        if( (flags & PF_FLAG_ALTERNATEFORM) && infop->prefix ){  /* Add "0" or "0x" */
          char *pre, x;
          pre = infop->prefix;
          if( *bufpt!=pre[0] ){
            for(pre=infop->prefix; (x=(*pre))!=0; pre++) *(--bufpt) = x;
	  }
        }

        length = (int)(&buf[BUFSIZE]-bufpt);
        break;
      case FLOAT:
      case EXP:
      case GENERIC:
        realvalue = va_arg(ap,double);
#ifndef NOFLOATINGPOINT
        if( precision<0 ) precision = 6;         /* Set default precision */
        if( precision>BUFSIZE-10 ) precision = BUFSIZE-10;
        if( realvalue<0.0 ){
          realvalue = -realvalue;
          prefix = '-';
	}else{
          if( flags & PF_FLAG_PLUSSIGN )          prefix = '+';
          else if( flags & PF_FLAG_BLANKSIGN )    prefix = ' ';
          else                         prefix = 0;
	}
        if( infop->type==GENERIC && precision>0 ) precision--;
        rounder = 0.0;
#ifdef COMPATIBILITY
        /* Rounding works like BSD when the constant 0.4999 is used.  Wierd! */
        for(idx=precision, rounder=0.4999; idx>0; idx--, rounder*=0.1);
#else
        /* It makes more sense to use 0.5 */
        if( precision>MAXDIG-1 ) idx = MAXDIG-1;
        else                     idx = precision;
        for(rounder=0.5; idx>0; idx--, rounder*=0.1);
#endif
        if( infop->type==FLOAT ) realvalue += rounder;
        /* Normalize realvalue to within 10.0 > realvalue >= 1.0 */
        exp = 0;
        if( realvalue>0.0 ){
          int k = 0;
          while( realvalue>=1e8 && k++<100 ){ realvalue *= 1e-8; exp+=8; }
          while( realvalue>=10.0 && k++<100 ){ realvalue *= 0.1; exp++; }
          while( realvalue<1e-8 && k++<100 ){ realvalue *= 1e8; exp-=8; }
          while( realvalue<1.0 && k++<100 ){ realvalue *= 10.0; exp--; }
          if( k>=100 ){
            bufpt = "NaN";
            length = 3;
            break;
          }
	}
        bufpt = buf;
        /*
        ** If the field type is GENERIC, then convert to either EXP
        ** or FLOAT, as appropriate.
        */
        flags = (flags & ~PF_FLAG_EXP) | ((xtype==EXP) ? PF_FLAG_EXP : 0);
        if( xtype!=FLOAT ){
          realvalue += rounder;
          if( realvalue>=10.0 ){ realvalue *= 0.1; exp++; }
        }
        if( xtype==GENERIC ){
          if (!(flags & PF_FLAG_ALTERNATEFORM))
            flags = (flags & ~PF_FLAG_RTZ) | PF_FLAG_RTZ;
          if( exp<-4 || exp>precision ){
            xtype = EXP;
          }else{
            precision = precision - exp;
            xtype = FLOAT;
          }
	}else{
	  flags = (flags & ~PF_FLAG_RTZ);
	}
        /*
        ** The "exp+precision" test causes output to be of type EXP if
        ** the precision is too large to fit in buf[].
        */
        nsd = 0;
        if( xtype==FLOAT && exp+precision<BUFSIZE-30 ){
          flags = flags & ~PF_FLAG_DP;
          flags |= ((precision>0 || (flags & PF_FLAG_ALTERNATEFORM)) ? PF_FLAG_DP : 0);
          if( prefix ) *(bufpt++) = prefix;         /* Sign */
          if( exp<0 )  *(bufpt++) = '0';            /* Digits before "." */
          else for(; exp>=0; exp--) *(bufpt++) = getdigit(&realvalue,&nsd);
          if( flags & PF_FLAG_DP ) *(bufpt++) = '.';           /* The decimal point */
          for(exp++; exp<0 && precision>0; precision--, exp++){
            *(bufpt++) = '0';
          }
          while( (precision--)>0 ) *(bufpt++) = getdigit(&realvalue,&nsd);
          *(bufpt--) = 0;                           /* Null terminate */
          if( (flags & PF_FLAG_RTZ) && (flags & PF_FLAG_DP) ){     /* Remove trailing zeros and "." */
            while( bufpt>=buf && *bufpt=='0' ) *(bufpt--) = 0;
            if( bufpt>=buf && *bufpt=='.' ) *(bufpt--) = 0;
          }
          bufpt++;                            /* point to next free slot */
	}else{    /* EXP or GENERIC */
          flags = flags & ~PF_FLAG_DP;
          flags |= ((precision>0 || (flags & PF_FLAG_ALTERNATEFORM)) ? PF_FLAG_DP : 0);
          if( prefix ) *(bufpt++) = prefix;   /* Sign */
          *(bufpt++) = getdigit(&realvalue,&nsd);  /* First digit */
          if( flags & PF_FLAG_DP ) *(bufpt++) = '.';     /* Decimal point */
          while( (precision--)>0 ) *(bufpt++) = getdigit(&realvalue,&nsd);
          bufpt--;                            /* point to last digit */
          if( (flags & PF_FLAG_RTZ) && (flags & PF_FLAG_DP) ){          /* Remove tail zeros */
            while( bufpt>=buf && *bufpt=='0' ) *(bufpt--) = 0;
            if( bufpt>=buf && *bufpt=='.' ) *(bufpt--) = 0;
          }
          bufpt++;                            /* point to next free slot */
          if( exp || (flags & PF_FLAG_EXP) ){
            *(bufpt++) = infop->charset[0];
            if( exp<0 ){ *(bufpt++) = '-'; exp = -exp; } /* sign of exp */
            else       { *(bufpt++) = '+'; }
            if( exp>=100 ){
              *(bufpt++) = (exp/100)+'0';                /* 100's digit */
              exp %= 100;
  	    }
            *(bufpt++) = exp/10+'0';                     /* 10's digit */
            *(bufpt++) = exp%10+'0';                     /* 1's digit */
          }
	}
        /* The converted number is in buf[] and zero terminated. Output it.
        ** Note that the number is in the usual order, not reversed as with
        ** integer conversions. */
        length = (int)(bufpt-buf);
        bufpt = buf;

        /* Special case:  Add leading zeros if the flag_zeropad flag is
        ** set and we are not left justified */
        if( (flags & PF_FLAG_ZEROPAD) && !(flags & PF_FLAG_LEFTJUSTIFY)
            && length < width){
          int i;
          int nPad = width - length;
          for(i=width; i>=nPad; i--){
            bufpt[i] = bufpt[i-nPad];
          }
          i = prefix!=0;
          while( nPad-- ) bufpt[i++] = '0';
          length = width;
        }
#endif
        break;
      case SIZE:
        *(va_arg(ap,int*)) = count;
        length = width = 0;
        break;
      case PERCENT:
        buf[0] = '%';
        bufpt = buf;
        length = 1;
        break;
      case CHARLIT:
      case CHAR:
        c = buf[0] = (xtype==CHAR ? va_arg(ap,int) : *++fmt);
        if( precision>=0 ){
          for(idx=1; idx<precision; idx++) buf[idx] = c;
          length = precision;
	}else{
          length =1;
	}
        bufpt = buf;
        break;
      case STRING:
      case MEM_STRING:
        zMem = bufpt = va_arg(ap,char*);
        if( bufpt==0 ) bufpt = "(null)";
        length = strlen(bufpt);
        if( precision>=0 && precision<length ) length = precision;
        break;
      case SEEIT:
        {
          int i;
          int c;
          char *arg = va_arg(ap,char*);
          for(i=0; i<BUFSIZE-1 && (c = *arg++)!=0; i++){
            if( c<0x20 || c>=0x7f ){
              buf[i++] = '^';
              buf[i] = (c&0x1f)+0x40;
            }else{
              buf[i] = c;
            }
          }
          bufpt = buf;
          length = i;
          if( precision>=0 && precision<length ) length = precision;
        }
        break;
      case ERROR:
        buf[0] = '%';
        buf[1] = c;
        flags &= ~PF_FLAG_ERROR;
        idx = 1+(c!=0);
        (*func)("%",idx,arg);
        count += idx;
        if( c==0 ) fmt--;
        break;
    }/* End switch over the format type */
    /*
    ** The text of the conversion is pointed to by "bufpt" and is
    ** "length" characters long.  The field width is "width".  Do
    ** the output.
    */
    if( !(flags & PF_FLAG_LEFTJUSTIFY) ){
      register int nspace;
      nspace = width-length;
      if( nspace>0 ){
        if( flags & PF_FLAG_CENTER ){
          nspace = nspace/2;
          width -= nspace;
          flags = (flags & ~PF_FLAG_LEFTJUSTIFY) | PF_FLAG_LEFTJUSTIFY;
	}
        count += nspace;
        while( nspace>=SPACESIZE ){
          (*func)(spaces,SPACESIZE,arg);
          nspace -= SPACESIZE;
        }
        if( nspace>0 ) (*func)(spaces,nspace,arg);
      }
    }
    if( length>0 ){
      (*func)(bufpt,length,arg);
      count += length;
    }
    if( xtype==MEM_STRING && zMem ){
      free(zMem);
    }
    if( flags & PF_FLAG_LEFTJUSTIFY ){
      register int nspace;
      nspace = width-length;
      if( nspace>0 ){
        count += nspace;
        while( nspace>=SPACESIZE ){
          (*func)(spaces,SPACESIZE,arg);
          nspace -= SPACESIZE;
        }
        if( nspace>0 ) (*func)(spaces,nspace,arg);
      }
    }
  }/* End for loop over the format string */
  return ((flags & PF_FLAG_ERROR) ? -1 : count);
} /* End of function */
#endif

#if defined(F_xprintf) || defined(DOXYGEN)
/*
** This non-standard function is still occasionally useful....
*/
int xprintf(
  void (*func)(char*,int,void*),
  void *arg,
  const char *format,
  ...
){
  va_list ap;
  va_start(ap,format);
  return vxprintf(func,arg,format,ap);
}
#endif

/*
** Now for string-print, also as found in any standard library.
** Add to this the snprint function which stops added characters
** to the string at a given length.
**
** Note that snprint returns the length of the string as it would
** be if there were no limit on the output.
*/
struct s_strargument {    /* Describes the string being written to */
  char *next;                   /* Next free slot in the string */
  char *last;                   /* Last available slot in the string */
};

void __sout(char *, int, void *);
#if defined(F___sout) || defined(DOXYGEN)
void __sout(txt,amt,arg)
  char *txt;
  int amt;
  void *arg;
{
  register char *head;
  register const char *t;
  register int a;
  register char *tail;
  a = amt;
  t = txt;
  head = ((struct s_strargument*)arg)->next;
  tail = ((struct s_strargument*)arg)->last;
  if( tail ){
    while( a-- >0 && head<tail ) *(head++) = *(t++);
  }else{
    while( a-- >0 ) *(head++) = *(t++);
  }
  *head = 0;
  ((struct s_strargument*)arg)->next = head;
}
#endif

#if defined(F_vsnprintf) || defined(DOXYGEN)
int vsnprintf(char *buf, size_t n, const char *fmt, va_list ap){
  struct s_strargument arg;
  arg.next = buf;
  arg.last = &buf[n-1];
  *buf = 0;
  return vxprintf(__sout,&arg,fmt,ap);
}
#endif

#if defined(F_snprintf) || defined(DOXYGEN)
int snprintf(char *str, size_t sz, const char *format, ...)
{
	va_list args;
	struct s_strargument arg;
	int ret;

	arg.next = str;
	arg.last = &str[sz-1];

	va_start(args, format);
	ret = vxprintf(__sout, &arg, format, args);
	va_end(args);

	return ret;
}
#endif

#if defined(F_vsprintf) || defined(DOXYGEN)
int vsprintf(char *buf, const char *fmt, va_list ap){
  struct s_strargument arg;
  arg.next = buf;
  arg.last = NULL;
  *buf = 0;
  return vxprintf(__sout,&arg,fmt,ap);
}
#endif

#if defined(F_sprintf) || defined(DOXYGEN)
__attribute__((weak))
int sprintf (char *str, const char *format, ...)
{
	va_list args;
	struct s_strargument arg;
	int ret;

	arg.next = str;
	arg.last = NULL;

	va_start(args, format);
	ret = vxprintf(__sout, &arg, format, args);
	va_end(args);

	return ret;
}
#endif

/*
** The following section of code handles the mprintf routine, that
** writes to memory obtained from malloc().
*/

/* This structure is used to store state information about the
** write in progress
*/
__attribute__((weak))
struct sgMprintf {
  char *zBase;     /* A base allocation */
  char *zText;     /* The string collected so far */
  int  nChar;      /* Length of the string so far */
  int  nAlloc;     /* Amount of space allocated in zText */
};

void __mout(char *, int, void*);

#if defined(F___mout) || defined(DOXYGEN)
/* The xprintf callback function. */
void __mout(zNewText,nNewChar,arg)
  char *zNewText;
  int nNewChar;
  void *arg;
{
  struct sgMprintf *pM = (struct sgMprintf*)arg;
  if( pM->nChar + nNewChar + 1 > pM->nAlloc ){
    pM->nAlloc = pM->nChar + nNewChar*2 + 1;
    if( pM->zText==pM->zBase ){
      pM->zText = malloc(pM->nAlloc);
      if( pM->zText && pM->nChar ) memcpy(pM->zText,pM->zBase,pM->nChar);
    }else{
      pM->zText = realloc(pM->zText, pM->nAlloc);
    }
  }
  if( pM->zText ){
    memcpy(&pM->zText[pM->nChar], zNewText, nNewChar);
    pM->nChar += nNewChar;
    pM->zText[pM->nChar] = 0;
  }
}
#endif

/*
** mprintf() works like printf(), but allocations memory to hold the
** resulting string and returns a pointer to the allocated memory.
**
** We changed the name to TclMPrint() to conform with the Tcl private
** routine naming conventions.
*/

#if defined(F_mprintf) || defined(DOXYGEN)
char *mprintf(const char *zFormat, ...){
  va_list ap;
  struct sgMprintf sMprintf;
  char *zNew;
  char zBuf[200];

  va_start(ap,zFormat);
  sMprintf.nChar = 0;
  sMprintf.nAlloc = sizeof(zBuf);
  sMprintf.zText = zBuf;
  sMprintf.zBase = zBuf;
  vxprintf(__mout,&sMprintf,zFormat,ap);
  va_end(ap);
  if( sMprintf.zText==sMprintf.zBase ){
    zNew = malloc( sMprintf.nChar+1 );
    if( zNew ) strcpy(zNew,zBuf);
  }else{
    zNew = realloc(sMprintf.zText,sMprintf.nChar+1);
  }

  return zNew;
}
#endif

/* This is the varargs version of mprintf.
**
** The name is changed to TclVMPrintf() to conform with Tcl naming
** conventions.
*/
#if defined(F_vmprintf) || defined(DOXYGEN)
char *vmprintf(const char *zFormat,va_list ap){
  struct sgMprintf sMprintf;
  char zBuf[200];
  sMprintf.nChar = 0;
  sMprintf.zText = zBuf;
  sMprintf.nAlloc = sizeof(zBuf);
  sMprintf.zBase = zBuf;
  vxprintf(__mout,&sMprintf,zFormat,ap);
  if( sMprintf.zText==sMprintf.zBase ){
    sMprintf.zText = malloc( strlen(zBuf)+1 );
    if( sMprintf.zText ) strcpy(sMprintf.zText,zBuf);
  }else{
    sMprintf.zText = realloc(sMprintf.zText,sMprintf.nChar+1);
  }
  return sMprintf.zText;
}
#endif

#if defined(F_asprintf) || defined(DOXYGEN)
int asprintf(char ** strp, const char *zFormat, ...){
  va_list ap;
  struct sgMprintf sMprintf;
  char *zNew;
  char zBuf[200];

  va_start(ap,zFormat);
  sMprintf.nChar = 0;
  sMprintf.nAlloc = sizeof(zBuf);
  sMprintf.zText = zBuf;
  sMprintf.zBase = zBuf;
  vxprintf(__mout,&sMprintf,zFormat,ap);
  va_end(ap);
  if( sMprintf.zText==sMprintf.zBase ){
    zNew = malloc( sMprintf.nChar+1 );
    if( zNew ) strcpy(zNew,zBuf);
  }else{
    zNew = realloc(sMprintf.zText,sMprintf.nChar+1);
  }

  *strp = zNew;

  return sMprintf.nChar+1;
}
#endif

#if defined(F_vasprintf) || defined(DOXYGEN)
int vasprintf(char **strp, const char *format, va_list ap) {
  struct sgMprintf sMprintf;
  char zBuf[200];
  sMprintf.nChar = 0;
  sMprintf.zText = zBuf;
  sMprintf.nAlloc = sizeof(zBuf);
  sMprintf.zBase = zBuf;
  vxprintf(__mout,&sMprintf,format,ap);
  if( sMprintf.zText==sMprintf.zBase ){
    sMprintf.zText = malloc( strlen(zBuf)+1 );
    if( sMprintf.zText ) strcpy(sMprintf.zText,zBuf);
  }else{
    sMprintf.zText = realloc(sMprintf.zText,sMprintf.nChar+1);
  }
  *strp = sMprintf.zText;
  return sMprintf.nChar;
}
#endif

/*
** The following section of code handles the standard fprintf routines
** for pthreads.
*/

void __fout(char *, int, void *);

#if defined(F___fout) || defined(DOXYGEN)
void __fout(zNewText,nNewChar,arg)
  char *zNewText;
  int nNewChar;
  void *arg;
{
	fwrite(zNewText,1,nNewChar,(FILE*)arg);
}
#endif

#if defined(F_fprintf) || defined(DOXYGEN)
/* The public interface routines */
int fprintf(FILE *pOut, const char *zFormat, ...){
  va_list ap;
  int retc;

  va_start(ap,zFormat);
  retc = vxprintf(__fout,pOut,zFormat,ap);
  va_end(ap);
  return retc;
}
#endif

#if defined(F_vfprintf) || defined(DOXYGEN)
int vfprintf(FILE *pOut, const char *zFormat, va_list ap){
  return vxprintf(__fout,pOut,zFormat,ap);
}
#endif


#if defined(F_printf) || defined(DOXYGEN)
int printf(const char *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	ret = vprintf(format, args);
	va_end(args);

	return ret;
}
#endif

#if defined(F_vprintf) || defined(DOXYGEN)
int vprintf(const char *format, va_list args)
{
	static char buf[PS2LIB_STR_MAX];
	int ret;

	ret = vsnprintf(buf, PS2LIB_STR_MAX, format, args);

	write(1, buf, ret);
	return ret;
}
#endif

#if defined(F_putchar) || defined(DOXYGEN)
int putchar( int chr )
{
	write(1, &chr, 1);
	return chr;
}
#endif
