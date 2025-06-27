#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

void process_log_init() {

    process_log_list = IniciaLista();
}


void process_log(const char* process_name, const int remaining, const int pc, const int sid, const int f_op_count) {
    proc_log_info_t* log = (proc_log_info_t *)malloc(sizeof(proc_log_info_t));

    if (!log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->name = strdup(process_name);
    log->remaining = remaining;
    log->pc = pc;
    log->id = sid;
    log->is_proc = 1;
    log->f_op_count = f_op_count;

    AdicionaLista(process_log_list, log);
}

void process_np_log() {
    proc_log_info_t* log = (proc_log_info_t *)malloc(sizeof(proc_log_info_t));

    if (!log) {
        printf("Sem memoria... \n");
        exit(EXIT_FAILURE);
    }

    log->is_proc = 0;

    AdicionaLista(process_log_list, log);
}

void disk_log_init() {
    disk_general_log = (disk_log_t*)malloc(sizeof(disk_log_t));

    if (!disk_general_log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    disk_general_log->r_req_count = 0;
    disk_general_log->w_req_count = 0;

    disk_log_list = IniciaLista();
}


void disk_log(const char* process_name, const int proc_id,
              const int track, const int read, const int turnaround) {
    disk_log_info_t* log = (disk_log_info_t *)malloc(sizeof(disk_log_info_t));

    if (!log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->proc_name = strdup(process_name);
    log->is_read = read;
    log->track = track;
    log->proc_id = proc_id;
    log->turnaround = turnaround;

    if (read)
        disk_general_log->r_req_count++;
    else disk_general_log->w_req_count++;

    AdicionaLista(disk_log_list, log);
}


void io_log_init() {
    io_general_log = (io_log_t *)malloc(sizeof(io_log_t));


    if (!io_general_log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    io_general_log->r_bytes = 0;
    io_general_log->w_bytes = 0;
    io_general_log->p_time = 0;

    io_log_list = IniciaLista();
}


void io_fs_log(const char* process_name, const int inumber, const io_log_fs_flag_t opt) {
    io_log_info_t* log = (io_log_info_t *)malloc(sizeof(io_log_info_t));

    if (!log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->fs_log = (io_log_fs_t *)malloc(sizeof(io_log_fs_t));

    if (!log->fs_log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->type = IO_LOG_FILE_SYSTEM;
    log->fs_log->proc_name = strdup(process_name);
    log->fs_log->inumber = inumber;
    log->fs_log->opt = opt;

    if (log->fs_log->opt & IO_LOG_FS_READ)
        io_general_log->r_bytes += (1 + (rand() & 65535));
    else if (log->fs_log->opt & IO_LOG_FS_WRITE)
        io_general_log->w_bytes += (1 + (rand() & 65535));

    AdicionaLista(io_log_list, log);
}

void io_disk_log(const char* process_name, const int read) {
    io_log_info_t* log = (io_log_info_t *)malloc(sizeof(io_log_info_t));

    if (!log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->disk_req_log = (io_log_disk_req_t *)malloc(sizeof(io_log_disk_req_t));

    if (!log->disk_req_log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->type = IO_LOG_DISK_REQUEST;
    log->disk_req_log->proc_name = strdup(process_name);
    log->disk_req_log->read = read;


    AdicionaLista(io_log_list, log);
}


void io_print_log(const char* process_name, const int duration) {
    io_log_info_t* log = (io_log_info_t *)malloc(sizeof(io_log_info_t));

    if (!log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->print_log = (io_log_print_t *)malloc(sizeof(io_log_print_t));

    if (!log->print_log) {
        printf("Sem memoria... \n");
        exit(EXIT_FAILURE);
    }

    log->type = IO_LOG_PRINT;
    log->print_log->proc_name = strdup(process_name);
    log->print_log->duration = duration;


    io_general_log->p_time += duration;


    AdicionaLista(io_log_list, log);
}


void res_acq_log_init() {

    res_acq_log_list = IniciaLista();
}

void res_acq_log(const char* process_name, const char* sem_name, const int acq, const int blocked) {
    res_acq_log_t* log = (res_acq_log_t *)malloc(sizeof(res_acq_log_t));

    
    if (!log) {
        printf("Sem memoria...\n");
        exit(EXIT_FAILURE);
    }

    log->proc_name = process_name == NULL ? NULL : strdup(process_name);
    log->sem_name = strdup(sem_name);
    log->acq = acq;
    log->blocked = blocked;

    AdicionaLista(res_acq_log_list, log);
}
