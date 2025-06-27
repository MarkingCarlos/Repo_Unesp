#ifndef OS_PROJECT_DISK_H
#define OS_PROJECT_DISK_H

#include "../tools/list.h"
#include "../process/process.h"

#define DISK_BLOCK(track) ((track) * 521 + 8)
#define INVERSE_DISK_BLOCK(block) (((block) - 8) / 521)


#define DISk_BASE_ANGULAR_V (7200)


#define DISK_TRACK_LIMIT (200)

#define DISK_OPERATION_TIME (5000)
#define DISK_TRACK_MOVE_TIME (100)

typedef struct DiskScheduler {

    list_t* pending_requests;


    int forward_dir;

    int curr_track;


    int angular_v;
} disk_scheduler_t;

typedef struct DiskRequest {

    process_t* process;

    int track;

    int read;


    int turnaround;
} disk_request_t;


void IniciaDisco();


_Noreturn void disk();


void disk_InicializadorScheduler(disk_scheduler_t *disk_scheduler);


disk_request_t* create_disk_request();


void disk_request(process_t* process, disk_scheduler_t *disk_scheduler, int track, int read);

#endif 
