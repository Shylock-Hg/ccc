/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-29
 *  \email tcath2s@gmail.com
 * */

#pragma once

#define TTY_COLOR_PREFIX "\033["
#define TTY_COLOR_SUFFIX "m"
#define TTY_COLOR_DELIMITER ";"

#define TTY_COLOR_FG_BLACK "30"
#define TTY_COLOR_BG_BLACK "40"
#define TTY_COLOR_FG_RED "31"
#define TTY_COLOR_BG_RED "41"
#define TTY_COLOR_FG_GREEN "32"
#define TTY_COLOR_BG_GREEN "42"
#define TTY_COLOR_FG_YELLOW "33"
#define TTY_COLOR_BG_YELLOW "43"
#define TTY_COLOR_FG_BLUE "34"
#define TTY_COLOR_BG_BLUE "44"
#define TTY_COLOR_FG_MAGENTA "35"
#define TTY_COLOR_BG_MAGENTA "45"
#define TTY_COLOR_FG_CYAN "36"
#define TTY_COLOR_BG_CYAN "46"
#define TTY_COLOR_FG_WHITE "37"
#define TTY_COLOR_BG_WHITE "47"

#define TTY_COLOR_RESET "0"    //< everything back to normal
#define TTY_COLOR_INVERSE "7"  //< swap foreground and background colours
#define TTY_COLOR_INVERSE_OFF "27"
#define TTY_COLOR_BOLD "1"  //< often a brighter shade of the same colour
#define TTY_COLOR_BOLD_OFF "21"
#define TTY_COLOR_UNDERLINE "4"  //< Add underline
#define TTY_COLOR_UNDERLINE_OFF "24"

#define TTY_COLOR_RESET_NOW TTY_COLOR_PREFIX TTY_COLOR_RESET TTY_COLOR_SUFFIX
