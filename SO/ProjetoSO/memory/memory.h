#ifndef OS_PROJECT_MEMORY_H
#define OS_PROJECT_MEMORY_H

#include "../tools/list.h"
#include "../process/instruction.h"
#include "../process/process.h"
#include "../semaphore/semaphore.h"

#include <stdio.h>
#include <stdlib.h>

#define KILOBYTE (1024)
#define MEGABYTE (1024 * KILOBYTE)
#define GIGABYTE (1024 * MEGABYTE)

#define MAX_MEM_SIZE (1 * GIGABYTE)

#define INSTRUCTIONS_PER_PAGE (256)
#define PAGE_SIZE (8 * KILOBYTE)
#define PAGE_NUMBER(x) ((x) / INSTRUCTIONS_PER_PAGE)
#define PAGE_OFFSET(x) ((x) % INSTRUCTIONS_PER_PAGE)

typedef struct Page {
    instr_t code[INSTRUCTIONS_PER_PAGE];
    int used;
} page_t;

typedef struct Segment {

    int id;

    int size;

    page_t* page_table;
    int page_count;
    int page_qtd;
} segment_t;

typedef struct Segment_Table {

    list_t* seg_list;

    int seg_list_size;

    int remaining;
} segment_table_t;


typedef struct {

    process_t* proc;

 
    instr_t* code;
} memory_request_t;


void segment_table_init(segment_table_t* seg_table);


segment_t* ProcuraSegmento(segment_table_t* seg_table, int sid);


void LiberaSegmento(segment_table_t* seg_table, int sid);


void mem_req_init(memory_request_t* req, process_t* proc, instr_t* code);

void mem_req_load(memory_request_t* req, segment_table_t* seg_table);

#endif 

