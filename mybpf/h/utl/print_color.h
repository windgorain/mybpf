/*================================================================
*   Created by LiXingang 
*   Histroy: 从 types.h 2005/2/25 迁移至此
*   Description: 
*
================================================================*/
#ifndef _PRINT_COLOR_H
#define _PRINT_COLOR_H
#ifdef __cplusplus
extern "C"
{
#endif

#define SHELL_COLOR_CLOSE "\033[0m"
#define SHELL_COLOR_HIGH  "\033[1m" 
#define SHELL_COLOR_LOW   "\033[2m" 
#define SHELL_COLOR_LINE  "\033[4m" 
#define SHELL_COLOR_FLASH "\033[5m" 
#define SHELL_COLOR_REFLECT "\033[7m" 
#define SHELL_COLOR_BLANKING "\033[8m" 

#define SHELL_BG_COLOR_BLACK   "\033[40m"
#define SHELL_BG_COLOR_RED     "\033[41m"
#define SHELL_BG_COLOR_GREEN   "\033[42m"
#define SHELL_BG_COLOR_YELLOW  "\033[43m"
#define SHELL_BG_COLOR_BLUE    "\033[44m"
#define SHELL_BG_COLOR_PURPLE  "\033[45m"
#define SHELL_BG_COLOR_CYAN    "\033[46m"
#define SHELL_BG_COLOR_WHITE   "\033[47m"

#define SHELL_FONT_COLOR_BLACK   "\033[30m"
#define SHELL_FONT_COLOR_RED     "\033[31m"
#define SHELL_FONT_COLOR_GREEN   "\033[32m"
#define SHELL_FONT_COLOR_YELLOW  "\033[33m"
#define SHELL_FONT_COLOR_BLUE    "\033[34m"
#define SHELL_FONT_COLOR_PURPLE  "\033[35m"
#define SHELL_FONT_COLOR_CYAN    "\033[36m"
#define SHELL_FONT_COLOR_WHITE   "\033[37m"
#define SHELL_FONT_ADD_LINE      "\033[38m"
#define SHELL_FONT_DEL_LINE      "\033[39m"

#define SHELL_FONT_COLOR_H_BLACK   "\033[90m"
#define SHELL_FONT_COLOR_H_RED     "\033[91m"
#define SHELL_FONT_COLOR_H_GREEN   "\033[92m"
#define SHELL_FONT_COLOR_H_YELLOW  "\033[93m"
#define SHELL_FONT_COLOR_H_BLUE    "\033[94m"
#define SHELL_FONT_COLOR_H_PURPLE  "\033[95m"
#define SHELL_FONT_COLOR_H_CYAN    "\033[96m"
#define SHELL_FONT_COLOR_H_WHITE   "\033[97m"

#define PRINT_COLOR(_color, _fmt, ...)      do { \
    printf(_color _fmt SHELL_COLOR_CLOSE, ##__VA_ARGS__); \
    fflush(stdout); } while(0)

#define PRINTLN_COLOR(_color, _fmt, ...)   printf(_color _fmt SHELL_COLOR_CLOSE "\n", ##__VA_ARGS__)   

#define PRINT_BLACK(_fmt, ...)      PRINT_COLOR(SHELL_FONT_COLOR_BLACK, _fmt, ##__VA_ARGS__)   
#define PRINT_RED(_fmt, ...)        PRINT_COLOR(SHELL_FONT_COLOR_RED, _fmt, ##__VA_ARGS__)   
#define PRINT_GREEN(_fmt, ...)      PRINT_COLOR(SHELL_FONT_COLOR_GREEN, _fmt, ##__VA_ARGS__)   
#define PRINT_YELLOW(_fmt, ...)     PRINT_COLOR(SHELL_FONT_COLOR_YELLOW, _fmt, ##__VA_ARGS__)   
#define PRINT_BLUE(_fmt, ...)       PRINT_COLOR(SHELL_FONT_COLOR_BLUE, _fmt, ##__VA_ARGS__)   
#define PRINT_PURPLE(_fmt, ...)     PRINT_COLOR(SHELL_FONT_COLOR_PURPLE, _fmt, ##__VA_ARGS__)   
#define PRINT_CYAN(_fmt, ...)       PRINT_COLOR(SHELL_FONT_COLOR_CYAN, _fmt, ##__VA_ARGS__)   
#define PRINT_WHITE(_fmt, ...)      PRINT_COLOR(SHELL_FONT_COLOR_WHITE, _fmt, ##__VA_ARGS__)   

#define PRINT_HBLACK(_fmt, ...)     PRINT_COLOR(SHELL_FONT_COLOR_H_BLACK, _fmt, ##__VA_ARGS__)   
#define PRINT_HRED(_fmt, ...)       PRINT_COLOR(SHELL_FONT_COLOR_H_RED, _fmt, ##__VA_ARGS__)   
#define PRINT_HGREEN(_fmt, ...)     PRINT_COLOR(SHELL_FONT_COLOR_H_GREEN, _fmt, ##__VA_ARGS__)   
#define PRINT_HYELLOW(_fmt, ...)    PRINT_COLOR(SHELL_FONT_COLOR_H_YELLOW, _fmt, ##__VA_ARGS__)   
#define PRINT_HBLUE(_fmt, ...)      PRINT_COLOR(SHELL_FONT_COLOR_H_BLUE, _fmt, ##__VA_ARGS__)   
#define PRINT_HPURPLE(_fmt, ...)    PRINT_COLOR(SHELL_FONT_COLOR_H_PURPLE, _fmt, ##__VA_ARGS__)   
#define PRINT_HCYAN(_fmt, ...)      PRINT_COLOR(SHELL_FONT_COLOR_H_CYAN, _fmt, ##__VA_ARGS__)   
#define PRINT_HWHITE(_fmt, ...)     PRINT_COLOR(SHELL_FONT_COLOR_H_WHITE, _fmt, ##__VA_ARGS__)   


#define PRINTLN_BLACK(_fmt, ...)    PRINTLN_COLOR(SHELL_FONT_COLOR_BLACK, _fmt, ##__VA_ARGS__)   
#define PRINTLN_RED(_fmt, ...)      PRINTLN_COLOR(SHELL_FONT_COLOR_RED, _fmt, ##__VA_ARGS__)   
#define PRINTLN_GREEN(_fmt, ...)    PRINTLN_COLOR(SHELL_FONT_COLOR_GREEN, _fmt, ##__VA_ARGS__)   
#define PRINTLN_YELLOW(_fmt, ...)   PRINTLN_COLOR(SHELL_FONT_COLOR_YELLOW, _fmt, ##__VA_ARGS__)   
#define PRINTLN_BLUE(_fmt, ...)     PRINTLN_COLOR(SHELL_FONT_COLOR_BLUE, _fmt, ##__VA_ARGS__)   
#define PRINTLN_PURPLE(_fmt, ...)   PRINTLN_COLOR(SHELL_FONT_COLOR_PURPLE, _fmt, ##__VA_ARGS__)   
#define PRINTLN_CYAN(_fmt, ...)     PRINTLN_COLOR(SHELL_FONT_COLOR_CYAN, _fmt, ##__VA_ARGS__)   
#define PRINTLN_WHITE(_fmt, ...)    PRINTLN_COLOR(SHELL_FONT_COLOR_WHITE, _fmt, ##__VA_ARGS__)   

#define PRINTLN_HBLACK(_fmt, ...)   PRINTLN_COLOR(SHELL_FONT_COLOR_H_BLACK, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HRED(_fmt, ...)     PRINTLN_COLOR(SHELL_FONT_COLOR_H_RED, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HGREEN(_fmt, ...)   PRINTLN_COLOR(SHELL_FONT_COLOR_H_GREEN, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HYELLOW(_fmt, ...)  PRINTLN_COLOR(SHELL_FONT_COLOR_H_YELLOW, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HBLUE(_fmt, ...)    PRINTLN_COLOR(SHELL_FONT_COLOR_H_BLUE, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HPURPLE(_fmt, ...)  PRINTLN_COLOR(SHELL_FONT_COLOR_H_PURPLE, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HCYAN(_fmt, ...)    PRINTLN_COLOR(SHELL_FONT_COLOR_H_CYAN, _fmt, ##__VA_ARGS__)   
#define PRINTLN_HWHITE(_fmt, ...)   PRINTLN_COLOR(SHELL_FONT_COLOR_H_WHITE, _fmt, ##__VA_ARGS__)   



#ifndef PRINTFL
#define PRINTFL() PRINTLN_GREEN("%s(%d)", __FILE__, __LINE__)
#endif


#define PRINTFLM_COLOR(_color, _fmt, ...) PRINT_COLOR(_color, "[%s:%d] " _fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define PRINTFLM_COLOR_LN(_color, _fmt, ...) PRINTLN_COLOR(_color, "[%s:%d] " _fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define PRINTFLM_BLACK(fmt, ...)    PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_BLACK, fmt, ##__VA_ARGS__)
#define PRINTFLM_GREEN(fmt, ...)    PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_GREEN, fmt, ##__VA_ARGS__)
#define PRINTFLM_RED(fmt, ...)      PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_RED, fmt, ##__VA_ARGS__)
#define PRINTFLM_YELLOW(fmt, ...)   PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_YELLOW, fmt, ##__VA_ARGS__)
#define PRINTFLM_CYAN(fmt, ...)     PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_CYAN, fmt, ##__VA_ARGS__)
#define PRINTFLM_PURPLE(fmt, ...)   PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_PURPLE, fmt, ##__VA_ARGS__)
#define PRINTFLM_BLUE(fmt, ...)     PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_BLUE, fmt, ##__VA_ARGS__)
#define PRINTFLM_WHITE(fmt, ...)    PRINTFLM_COLOR_LN(SHELL_FONT_COLOR_WHITE, fmt, ##__VA_ARGS__)

#define PRINTFLM(fmt, ...) PRINTFLM_WHITE(fmt, ##__VA_ARGS__)
#define PRINTFLM_ERR(fmt, ...) PRINTFLM_RED(fmt, ##__VA_ARGS__)
#define PRINTFLM_WARN(fmt, ...) PRINTFLM_YELLOW(fmt, ##__VA_ARGS__)


void PrintColor_Black(const char *fmt, ...);
void PrintColor_Red(const char *fmt, ...);
void PrintColor_Green(const char *fmt, ...);
void PrintColor_Yellow(const char *fmt, ...);
void PrintColor_Blue(const char *fmt, ...);
void PrintColor_Purple(const char *fmt, ...);
void PrintColor_Cyan(const char *fmt, ...);
void PrintColor_White(const char *fmt, ...);

void PrintColor_HBlack(const char *fmt, ...);
void PrintColor_HRed(const char *fmt, ...);
void PrintColor_HGreen(const char *fmt, ...);
void PrintColor_HYellow(const char *fmt, ...);
void PrintColor_HBlue(const char *fmt, ...);
void PrintColor_HPurple(const char *fmt, ...);
void PrintColor_HCyan(const char *fmt, ...);
void PrintColor_HWhite(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif 
