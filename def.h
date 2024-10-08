/*
  Include and define needed global stuff
*/

#ifndef _DEF_H
#define _DEF_H

/*
  Includes
*/
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/*
  PANIC macro
*/
#ifndef PANIC
#define PANIC(msg, cond) if (cond) { perror (msg); assert (!cond); }
#endif

/*
  Strinf type
*/
typedef char * string;

#endif