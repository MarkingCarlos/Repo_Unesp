#include <math.h>

#include "../tools/math.h"
#include "memory.h"


static segment_t* segment_create(memory_request_t* req) {
    segment_t* seg = (segment_t *)malloc(sizeof(segment_t));

    if (!seg) {
        printf("Sem memoria... \n");
        exit(EXIT_FAILURE);
    }

    seg->id = req->proc->seg_id;
    seg->size = req->proc->seg_size * KILOBYTE; /* size measure in bytes */
    seg->page_count = (int) ceil((double) req->proc->code_len / INSTRUCTIONS_PER_PAGE);
    seg->page_table = (page_t *)malloc(sizeof(page_t) * seg->page_count);
    seg->page_qtd = seg->size / PAGE_SIZE;

    return seg;
}

static void PopularSegmento(segment_t* seg, instr_t* code, int code_len) {
    register int i;
    for (i = 0; i < code_len; i++)
        seg->page_table[PAGE_NUMBER(i)].used = 0;
    for (i = 0; i < code_len; i++)
        seg->page_table[PAGE_NUMBER(i)].code[PAGE_OFFSET(i)] = code[i];
}

static void AdcionarSegmento(segment_table_t* seg_table, segment_t* seg) {
    AdicionaLista(seg_table->seg_list, seg);
    seg_table->seg_list_size++;
}

static int TrocaPagina(segment_table_t* seg_table, segment_t* seg) {
    int i;
    int freed = 0;

    FOREACH(seg_table->seg_list, segment_t*) {
        for (i = 0; i < it->page_count; i++) {
            page_t* page = it->page_table + i;

            if (page->used)
                page->used = 0;
            else freed += PAGE_SIZE;

            if (freed >= seg->size)
                break;
        }
    }

    return freed;
}

void segment_table_init(segment_table_t* seg_table) {
    seg_table->seg_list = IniciaLista();
    seg_table->seg_list_size = 0;
    seg_table->remaining = 1024 * 1024 * 1024;
}


segment_t* ProcuraSegmento(segment_table_t* seg_table, int sid) {
    FOREACH(seg_table->seg_list, segment_t*) {
            return it;
    }

    return NULL;
}


void LiberaSegmento(segment_table_t* seg_table, int sid) {
    list_node_t* seg_node;
    for (seg_node = seg_table->seg_list->head; seg_node != NULL;
         seg_node = seg_node->next)
        if (((segment_t *)seg_node->content)->id == sid)
            break;


    if (!seg_node) {
        printf("Operação iniciada"
               " mas o segmento não foi encontrado.\n");
        exit(EXIT_FAILURE);
    }


    RemoveNode(seg_table->seg_list, seg_node);

    segment_t* seg = (segment_t *)seg_node->content;

    seg_table->remaining = MIN(MAX_MEM_SIZE, seg_table->remaining + seg->size);

    free(seg->page_table);
    free(seg);
    free(seg_node);
}

void mem_req_init(memory_request_t* req, process_t* proc, instr_t* code) {
    req->proc = proc;
    req->code = code;
}

void mem_req_load(memory_request_t* req, segment_table_t* seg_table) {
    segment_t* seg = segment_create(req);

    const int new_remaining = seg_table->remaining - seg->size;

    seg_table->remaining = MAX(0, new_remaining);

    if (new_remaining < 0) {

        seg_table->remaining += TrocaPagina(seg_table, seg);
    }


    PopularSegmento(seg, req->code, req->proc->code_len);
    AdcionarSegmento(seg_table, seg);
}

