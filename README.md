cmdmax is a tiny Windows application that allows to maximize cmd.exe window to match the full screen size. 
Unlike maximize button the utility changes botch horizontal and vertical size of the console window in characters.
User can specify x/y windows position, width height and buffer size. The application is intended to be used in
scripts / batch jobs, Windows PE, embedded, etc. 

    Usage: cmdmax [<pos_x> <pos_y> <win_w> <win_h> <buf_w> <buf_h>]
           pos_x, pos_y are in pixels
           win_w, win_h, buf_w, buf_h are in characters
           buf_w, buf_h must be greater or equal to win_w, win_h
           if both pos_x, pos_y are set to letter 'n' won't move the window
           if no parameters specified, a maximum possible size will be used

    Examples:
           cmdmax                       (will maximize the window)
           cmdmax 100 100  80 24  80 24            (no scrollbars)
           cmdmax n n  80 24  80 5000  (add buffer without moving)


Download from [releases](https://github.com/tenox7/cmdmax/releases)