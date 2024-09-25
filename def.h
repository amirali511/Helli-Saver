/*
  Include and define needed global stuff
*/


#ifndef _DEF_H
#define _DEF_H

#include <xcb/xcb.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PANIC
#define PANIC(msg, cond) if (cond) { perror (msg); assert (!cond); }
#endif

#endif