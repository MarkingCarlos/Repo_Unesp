#ifndef OS_PROJECT_TERMINAL_H
#define OS_PROJECT_TERMINAL_H

#include "../tools/math.h"
#include "../memory/memory.h"

#include "log.h"

#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>

#define ENTER_KEY 10
#define BOX_OFFSET 2
#define TITLE_OFFSET 2
#define BOX_SIZE 2

#undef WIN_CREATE
#define WIN_CREATE(winn) win_##winn##_create()

#undef FWIN_CREATE
#define FWIN_CREATE(winn) win_##winn##_t *WIN_CREATE(winn)

#undef WIN_REFRESH
#define WIN_REFRESH(win) refresh_##win##_log()

#undef FWIN_REFRESH
#define FWIN_REFRESH(win) _Noreturn void *WIN_REFRESH(win)

#undef WIN_REFRESH_TITLE
#define WIN_REFRESH_TITLE(win) refresh_##win##_window_title()

#undef FWIN_REFRESH_TITLE
#define FWIN_REFRESH_TITLE(win) void WIN_REFRESH_TITLE(win)

#undef WIN_MEM_BUFFER_RS_SIZE
#define WIN_MEM_BUFFER_RS_SIZE (64)

#undef WIN_DISK_BUFFER_RS_SIZE
#define WIN_DISK_BUFFER_RS_SIZE (64)

#undef LOOP
#define LOOP while (1)

static const int EXIT = -1;
static const long SECOND_IN_US = 1000000L;

typedef struct {
    WINDOW *main_window;
    WINDOW *text_window;
    WINDOW *title_window;
} win_t;

typedef struct {

    win_t win;

    char *buffer_rs;
} win_memory_t;

typedef struct {

    win_t win;

    char *buffer_rs;
} win_disk_t;

typedef enum main_menu_choices {
    CREATE
} main_menu_choices;

typedef struct menu_choice_t {
    const char* text;
    int value;
} menu_choice_t;

typedef struct menu_t {
    menu_choice_t* choices;
    int choices_count;
    const char* title;
    int height;
    int width;
    MENU* curses_menu;
    ITEM** items;
    win_t* menu_window;
} menu_t;

typedef struct {
    int begin_y;
    int begin_x;
} coordinates_t;


#define MAIN_MENU(M)                                                           \
    M("Criar processo", CREATE)                                                \
    M("Sair", EXIT)                                                            \

void menu_loop(menu_t* main_menu, void (*func)(int));

void init_screen();
void end_screen(void);

void start_menu_and_loop(menu_t* menu);
menu_t* create_menu(int choices_count,
                    menu_choice_t choices[static choices_count],
                    const char* title);
int get_type_from_string(menu_t* menu, const char* buffer);

win_t* init_menu_window(const menu_t* menu);
int begin_terminal();
void delete_menu(menu_t* menu);
void free_menu_window(win_t* window);

char* get_input_from_window(char* title, coordinates_t coordinates,
                            int buffer_size);
void print_with_window(char* string, char* title, int y, int x);

FWIN_REFRESH(process);
FWIN_REFRESH(memory);
FWIN_REFRESH(disk);
FWIN_REFRESH(io);
FWIN_REFRESH(res_acq);
_Noreturn void* refresh_logs();

win_t* init_io_log();
win_t* init_process_log();
FWIN_CREATE(memory);
FWIN_CREATE(disk);
win_t* init_res_acq_log();

FWIN_REFRESH_TITLE(disk);
FWIN_REFRESH_TITLE(memory);
FWIN_REFRESH_TITLE(res_acq);


static process_t* get_process_sid(int sid);


static int max_seg_size();

static int page_inuse_index(const segment_t* segment);


static int sem_blocked_count();

#endif 
