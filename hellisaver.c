/*
  Screen Saver for HelliWM
*/

#include "def.h"

xcb_connection_t * conn;
xcb_screen_t * scr;
uint32_t scrID;


static void close_saver (xcb_connection_t * conn);

int
main (void)
{
  int screenn;
  conn = xcb_connect (NULL, &screenn);
  if (xcb_connection_has_error (conn))
    PANIC ("Could not open the connection to X server, aborting...\n", xcb_connection_has_error (conn));

}

static void
close_saver (xcb_connection_t * conn)
{
  if (conn != NULL) {
    xcb_disconnect (conn);
    exit (0);
  } else {
    PANIC ("No connection to remove, aborting...\n", conn == NULL);
    exit (1);
  }
}