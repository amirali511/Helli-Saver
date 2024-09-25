/*
  Screen Saver for HelliWM
*/

#include "def.h"

xcb_connection_t * conn;
uint32_t saverID;
xcb_void_cookie_t cookie;

static void close_saver (xcb_connection_t * conn);

int
main (void)
{
  int screenn;
  conn = xcb_connect (NULL, &screenn);
  if (xcb_connection_has_error (conn))
    PANIC ("Could not open the connection to X server, aborting...\n", 
           xcb_connection_has_error (conn));

  xcb_setup_t              * stp = xcb_get_setup (conn);
  xcb_screen_iterator_t     iter = xcb_setup_roots_iterator (stp);
  xcb_screen_t             * scr = iter.data;
  if (stp == NULL || &iter == NULL || scr == NULL)
    PANIC ("Could not get the screen, aborting...\n", 
           stp == NULL || &iter == NULL || scr == NULL);
  
  xcb_key_symbols_t * keysyms      = xcb_key_symbols_alloc (conn);
  xcb_keycode_t       exit_keycode = xcb_key_symbols_get_keycode (keysyms, XK_Q)[0];
  if (keysyms == NULL || exit_keycode == NULL)
    PANIC ("Could not get the xcb_keycodes, aborting...\n", 
           keysyms == NULL || exit_keycode == NULL);
  
  cookie = xcb_grab_key (conn,
                         1,
                         scr->root,
                         XCB_MOD_MASK_4,
                         exit_keycode,
                         XCB_GRAB_MODE_ASYNC,
                         XCB_GRAB_MODE_ASYNC);
  
  if (xcb_request_check (conn, cookie))
    PANIC ("Could not grab the keys, aborting...\n", 
           xcb_request_check (conn, cookie));
  
  xcb_key_symbols_free (keysyms);
  
  saverID = xcb_generate_id (conn);
  if (saverID == -1)
    PANIC ("Could not generate an ID for the screen, aborting...\n", 
           saverID == -1);
  
  uint32_t values[2];
  values[0] = 0x242424;
  values[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_PRESS;

  cookie = xcb_create_window (conn,
                              XCB_COPY_FROM_PARENT,
                              saverID,
                              scr->root,
                              0, 0,
                              scr->width_in_pixels,
                              scr->height_in_pixels,
                              0,
                              XCB_WINDOW_CLASS_INPUT_OUTPUT,
                              scr->root_visual,
                              XCB_CW_BACK_PIXEL |
                              XCB_CW_EVENT_MASK,
                              values);
  
  if (xcb_request_check (conn, cookie))
    PANIC ("Could not create the window, aborting...\n", 
           xcb_request_check (conn, cookie));
  
  cookie = xcb_map_window (conn, saverID);
  if (xcb_request_check (conn, cookie))
    PANIC ("Could not map the window, aborting...\n", 
           xcb_request_check (conn, cookie));
  
  if (xcb_flush (conn) <= 0)
    PANIC ("Could not flush the connection, aborting...\n", 
           xcb_flush (conn) <= 0);

  xcb_generic_event_t * ev;

  while (true) {
    ev = xcb_poll_for_event (conn);
    
    if (ev != NULL) {

      if (ev->response_type == 0)
        PANIC ("Event is empty, aborting...\n", 
               ev->response_type == 0);

      switch (ev->response_type & ~0x80) {
        case XCB_KEY_PRESS:

          xcb_key_press_event_t * ev = (xcb_key_press_event_t *) ev;
          if (ev->state == XCB_MOD_MASK_4 && ev->detail == exit_keycode)
            close_saver (conn);
          
          break;
      }
    }
  }

  return EXIT_SUCCESS;

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