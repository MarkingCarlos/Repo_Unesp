#ifndef OS_PROJECT_PROCESS_H
#define OS_PROJECT_PROCESS_H

#include <stdio.h>

#include "../tools/list.h"

#define FETCH_INSTR_ADDR(x) ((x)->pc++)

typedef enum ProcessState {
    NEW,
    BLOCKED,
    READY,
    RUNNING,
    TERMINATED
} process_state_t;

typedef struct Process {
   
    char *name;
    int id;
    int pc; 
    int code_len;

    int seg_id;
    int seg_size;

    
    int priority;
    int remaining;
    process_state_t state;

    int io_read_count;
    int io_write_count;
   
    list_t* o_files;

  
    char **semaphores;
} process_t;



/**
 * It returns 1 if both process has the same id.
 * Otherwise, 0 is returned.
 *
 * @param p1 the first process
 * @param p2 the second process
 *
 * @return 1 if both process has the same id;
 *         otherwise, 0 is returned.
 */
int proc_cmp(void* p1, void *p2);


/**
 * It creates the process read from the file
 * specified in the file path, and add this
 * file to the process control block (PCB).
 *
 * @param filepath the file path containing
 *                 the process specifications
 */
void process_create(const char* filepath);

/**
 * It finishes the specified process removing
 * it from the PCB and from schedulable process
 * queue. Further, the memory allocated for the
 * process is freed.
 *
 * @param proc the process to be finished
 */
void process_finish(process_t* proc);



/**
 * It returns 1 if this process has already opened
 * the inode with the specified inumber. Otherwise,
 * 0 is returned.
 *
 * @param process the process to be checked
 * @param inumber the inode number
 *
 * @return 1 if this process has already opened
 *            the inode with the specified inumber.
 *            Otherwise, 0 is returned.
 */
int has_opened_file(process_t* process, int inumber);

#endif 