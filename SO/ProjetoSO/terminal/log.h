#ifndef OS_PROJECT_LOG_H
#define OS_PROJECT_LOG_H

#include <semaphore.h>

#include "../tools/list.h"

extern sem_t log_mutex;
extern sem_t mem_mutex;
extern sem_t disk_mutex;
extern sem_t refresh_sem;
extern sem_t io_mutex;
extern sem_t res_acq_mutex;

/* Process Log Related Structures & Function Prototypes */

typedef struct {
    int is_proc;

    char* name;

    int remaining;

    int id;

    int pc;


    int f_op_count;
} proc_log_info_t;


void process_log_init();


void process_log(const char* process_name, const int remaining, const int pc, const int sid, const int f_op_count);


void process_np_log();


typedef struct DiskLog {

    int r_req_count;


    int w_req_count;


    int forward_dir;

    int curr_track;

    int angular_v;

    int pending_requests_size;
} disk_log_t;

typedef struct {

    char *proc_name;


    int proc_id;

    int track;

    int is_read;

    int turnaround;
} disk_log_info_t;


void disk_log_init();


void disk_log(const char* process_name, const int proc_id,
              const int track, const int read, const int turnaround);



typedef struct {

    long w_bytes;


    long r_bytes;

    int p_time;
} io_log_t;

typedef enum IoLogType {

    IO_LOG_PRINT,


    IO_LOG_FILE_SYSTEM,

    IO_LOG_DISK_REQUEST
} io_log_type_t;

typedef struct IoLogPrint {

    char* proc_name;


    int duration;
} io_log_print_t;

typedef enum IoLogFileSystemFlag {

    IO_LOG_FS_READ = 1,
    IO_LOG_FS_WRITE = 2,


    IO_LOG_FS_F_OPEN = 4,
    IO_LOG_FS_F_CLOSE = 8,
} io_log_fs_flag_t;

typedef struct IoLogFileSystem {

    char* proc_name;

    int inumber;

    io_log_fs_flag_t opt;
} io_log_fs_t;

typedef struct IoLogDiskRequest {

    char* proc_name;

    int read;
} io_log_disk_req_t;

typedef struct {

    io_log_type_t type;

    io_log_print_t* print_log;


    io_log_fs_t* fs_log;


    io_log_disk_req_t* disk_req_log;
} io_log_info_t;


void io_log_init();

void io_fs_log(const char* process_name, const int inumber, const io_log_fs_flag_t opt);

void io_disk_log(const char* process_name, const int read);


void io_print_log(const char* process_name, const int duration);


typedef struct {

    const char* proc_name;

    const char* sem_name;

    int acq;

    int blocked;
} res_acq_log_t;

void res_acq_log_init();



void res_acq_log(const char* process_name, const char* sem_name, const int acq, const int blocked);


extern list_t* process_log_list;
extern list_t* disk_log_list;
extern list_t* io_log_list;
extern list_t* res_acq_log_list;

extern disk_log_t* disk_general_log;
extern io_log_t* io_general_log;

#endif 
