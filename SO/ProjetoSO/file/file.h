#ifndef OS_PROJECT_FILE_H
#define OS_PROJECT_FILE_H

#include <time.h>

#include "../process/process.h"

#define INODE_NUMBER(x) ((x) * 948 + 5)


#define INVERSE_INODE_NUMBER(x) (((x) - 5) / 948)

#include "../tools/list.h"
#include "../process/process.h"



typedef enum FilePermission {

    F_PERM_OWNER_R = (1 << 8),
    F_PERM_OWNER_W = (1 << 7),
    F_PERM_OWNER_X = (1 << 6),

    F_PERM_GROUP_R = (1 << 5),
    F_PERM_GROUP_W = (1 << 4),
    F_PERM_GROUP_X = (1 << 3),


    F_PERM_OTHER_R = (1 << 2),
    F_PERM_OTHER_W = (1 << 1),
    F_PERM_OTHER_X = (1 << 0),
} file_perm_t;

typedef struct {

    int id;


    int o_count;


    short mode;

    time_t last_accessed;

    time_t last_modified;

    long size;
} inode_t;

typedef struct FileDescriptorList {

    list_t* inode_list;
} ilist_t;


typedef struct FileTable {

    ilist_t* ilist;


    int next_inode_id;
} file_table_t;

typedef struct FileSystemOperationRequest {

    process_t* proc;

    int track;


    int read;
} fs_op_request_t;


inode_t* CriarNode(int id);


ilist_t* CreateList();


void file_table_init(file_table_t* file_table);

inode_t* BuscaNode(file_table_t* file_table, int inumber);


void fs_read_request(file_table_t* file_table, process_t* process, int block);


void fs_write_request(file_table_t* file_table, process_t* process, int block);


inode_t *fs_open_request(file_table_t *file_table, const char *owner, int inumber);


void fs_close_request(file_table_t* file_table, process_t* process, int inumber);

#endif 
