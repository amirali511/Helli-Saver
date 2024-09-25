/*
  HHH     HHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAAAAAAAAAAA   VVV              VVV   EEEEEEEEE   RRRRRRRRRR
  HHH     HHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAAAAAAAAAAA    VVV            VVV    EEEEEEEEE   RRR    RRR
  HHH     HHH   EEE         LLL         LLL         III   SSS         AAA      AAA     VVV          VVV     EEE         RRR    RRR
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAAAAAAAAAAA      VVV        VVV      EEEEEEEEE   RRRRRRRRRR
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III   SSSSSSSSS   AAA      AAA       VVV      VVV       EEEEEEEEE   RRRRRR
  HHH     HHH   EEE         LLL         LLL         III         SSS   AAA      AAA        VVV    VVV        EEE         RRR RRR
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III   SSSSSSSSS   AAA      AAA         VVV  VVV         EEEEEEEEE   RRR  RRR
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III   SSSSSSSSS   AAA      AAA          VVVVVV          EEEEEEEEE   RRR   RRR
*/

/*
  Screen Saver for Linux
*/
#include "def.h"

/*
  Global vars
*/
xcb_connection_t * conn;
uint32_t saverID;
xcb_void_cookie_t cookie;
const string version = "1.0";

/*
  Close function
*/
static void close_saver (xcb_connection_t * conn);

int
main (void)
{
  /*
    Basic screen setup
  */
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
  
  /*
    Keyboard handling and setup
  */
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
  
  /*
    Screen setup
  */
  saverID = xcb_generate_id (conn);
  if (saverID == -1)
    PANIC ("Could not generate an ID for the screen, aborting...\n", 
           saverID == -1);
  
  uint32_t values[2];
  values[0] = 0x242424;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_PRESS;

  /*
    Create window manager
  */
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

  /*
    Graphics context for the rectangles
  */
  xcb_gcontext_t gc = xcb_generate_id (conn);
  cookie = xcb_create_gc (conn,
                          gc,
                          saverID,
                          0,
                          NULL);

  if (xcb_request_check (conn, cookie))
    PANIC ("Could not create the graphics context, aborting...\n",
           xcb_request_check (conn, cookie));

  srand (time (NULL));

  /*
    Generic event for the following event loop, Stem Cell
  */
  xcb_generic_event_t * ev;

  while (true) {

    /*
      Clearing the window
    */
    cookie = xcb_clear_area (conn,
                             0,
                             saverID,
                             0, 0,
                             scr->width_in_pixels, 
                             scr->height_in_pixels);

    if (xcb_request_check (conn, cookie))
      PANIC ("Could not clear the screen, aborting...\n",
             xcb_request_check (conn, cookie));
    
    /*
      Creating the reactangle randomly
    */
    for (int i = 0; i < 10; i++) {
      int x = rand() % 800;
      int y = rand() % 600;
      int width = rand() % 100 + 50;
      int height = rand() % 100 + 50;
      uint32_t color = rand() % 0xFFFFFF;

      /*
        Changing the gc for the rectangles
      */
      cookie = xcb_change_gc(conn, 
                             gc, 
                             XCB_GC_FOREGROUND, 
                             &color);

      if (xcb_request_check (conn, cookie))
        PANIC ("Could not change the graphics context, aborting...\n",
               xcb_request_check (conn, cookie));
      
      /*
        Creating the rectangles and writing them
      */
      xcb_rectangle_t rectangle = {x, y, width, height};
      xcb_poly_fill_rectangle(conn, saverID, gc, 1, &rectangle);
    }

    if (xcb_flush (conn) <= 0)
      PANIC ("Could not flush the connection, aborting...\n",
             xcb_flush (conn) <= 0);

    usleep (100000);

    ev = xcb_poll_for_event (conn);
    
    if (ev != NULL) {

      /*
        Event handling
      */
      if (ev->response_type == 0)
        PANIC ("Event is empty, aborting...\n", 
               ev->response_type == 0);

      switch (ev->response_type & ~0x80) {
        case XCB_KEY_PRESS:

          xcb_key_press_event_t * ev = (xcb_key_press_event_t *) ev;
          if (ev->state == XCB_MOD_MASK_4 && ev->detail == exit_keycode)
            free (ev);
            close_saver (conn);          
          break;
      }
    }
  }

  return EXIT_SUCCESS;
}

/*
  Close the connection and the screen saver
*/
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