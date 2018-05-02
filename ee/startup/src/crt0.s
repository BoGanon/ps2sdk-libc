# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2001-2007 ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# Standard startup file.

   # Check weak symbols
   # Usage: ckwk $8,sym,1f
   .macro ckwk reg,sym,lbl
   la   \reg, \sym
   beqz   \reg, \lbl
   nop
   jalr   \reg
   nop
   .endm
   
   # Provided by libc
   .weak   atexit
   .type   atexit, @function
   .weak   exit
   .type   exit, @function

   # Old C++ ctor and dtor functions
   .type   _init, @function
   .type   _fini, @function
   # New C++ ctor and dtor arrays
   #.type   __libc_init_array, @function
   #.type   __libc_fini_array, @function

   # Provided by ps2sdk but reusable for newlib
   .weak   __libc_init
   .type   __libc_init, @function
   .weak   __libc_deinit
   .type   __libc_deinit, @function

   .set   noat
   .set	  noreorder

   .text
   .align   3

   .globl  _start
   .ent    _start
_start:

zerobss:
   # clear bss area
   la   $2, _fbss
   la   $3, _end

1:
   sltu   $1, $2, $3
   beq   $1, $0, 2f
   nop
   sq   $0, ($2)
   addiu   $2, $2, 16
   j   1b
   nop
2:

setupthread:
   # setup current thread for creating threads
   la   $4, _gp
   la   $5, _stack
   la   $6, _stack_size
   la   $7, _args
   la   $8, _root
   move   $gp, $4
   addiu   $3, $0, 60
   syscall         # SetupThread(_gp, _stack, _stack_size, _args, _root)

setupmem:
   # setup stack and make room for storing arguments
   move   $sp, $2
   addiu   $sp, $sp,-96

   # initialize heap
   la   $4, _end
   la   $5, _heap_size
   addiu   $3, $0, 61
   syscall         # SetupHeap(_end, _heap_size)

   # writeback data cache
   jal FlushCache  # FlushCache(0)
   move   $4, $0


#parseargs:
#   # call ps2sdk argument parsing
#
#   la   $16, _args
#
#   la   $8, _ps2sdk_args_parse
#   beqz   $8, 1f
#   lw   $4, ($16)
#
#   jalr   $8      # _ps2sdk_args_parse(argc, argv)
#   addiu   $5, $16, 4
#1:

setup_libc:
   # Initialize the kernel first (Apply necessary patches).
   jal _InitSys
   nop

   # init libc
   ckwk $8,__libc_init,1f
1:
   # add destructors using atexit() (weak)
   # la $4, __libc_fini_array
   la $4, _fini
   ckwk   $8,atexit,1f
1:
   # call _init for constructors
   # jal   __libc_init_array
   jal   _init
   nop

runmain:
   # call main
   ei

   la   $16, _args
   lw   $4, ($16)
   jal   main       # main(argc, argv)
   addiu $5, $16, 4

destroy_libc:
   # fall through to libc exit (weak)
   la   $8, exit
   beqz   $8, 1f
   move   $4, $2

   jr   $8          # exit(retval) (noreturn)
   nop
1:
   # deinit libc
   ckwk   $8,__libc_deinit,1f
1:
   # no libc exit
   j   Exit         # Exit(retval) (noreturn)
   nop
   .end   _start

   .align   3
   
   # _exit(retval) (noreturn) called by libc
   .globl   _exit
   .ent   _exit
   .text

_exit:
   j   Exit           # Exit(retval) (noreturn)
   nop

   .end   _exit

   .ent   _root

_root:
   addiu   $3, $0, 35
   syscall          # ExitThread() (noreturn)
   .end   _root

   .bss
   .align   6
_args:
   .space   4+16*4+256   # argc, 16 arguments, 256 bytes payload
