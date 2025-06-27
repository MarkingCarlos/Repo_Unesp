#include <stdio.h>
#include <stdlib.h>

#include "../tools/constants.h"
#include "../terminal/log.h"

#include "file.h"

#if OS_DEBUG || OS_FS_DEBUG
#define LOG_FS(fmt) printf(fmt)
#define LOG_FS_A(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define LOG_FS(fmt)
#define LOG_FS_A(fmt, ...)
#endif 

inode_t* CriarNode(int id) {
    inode_t* inode = (inode_t *)malloc(sizeof(inode_t));


    if (!inode) {
        printf("Sem memoria o suficiente.\n");
        exit(EXIT_FAILURE);
    }

    inode->id = id;
    inode->o_count = 0;
    inode->mode = F_PERM_OWNER_R | F_PERM_OWNER_W | F_PERM_OWNER_X | F_PERM_OTHER_R | F_PERM_OTHER_W;
    inode->last_accessed = -1;
    inode->last_modified = -1;
    inode->size = (100 * (1 + (rand() % 10))) + (rand() % RAND_MAX);

    return inode;
}


ilist_t* CreateList() {
    ilist_t* ilist = (ilist_t *)malloc(sizeof(ilist_t));


    if (!ilist) {
        printf("Sem memoria... \n");
        exit(EXIT_FAILURE);
    }

    ilist->inode_list = IniciaLista();

    return ilist;
}

void file_table_init(file_table_t* file_table) {
    file_table->ilist = CreateList();
}


inode_t* BuscaNode(file_table_t* file_table, int inumber) {
    FOREACH(file_table->ilist->inode_list, inode_t*) {
        if (it->id == inumber)
            return it;
    }

    return NULL;
}

void fs_read_request(file_table_t* file_table, process_t* process, int block) {
    const int inumber = INODE_NUMBER(block);

    inode_t* inode = BuscaNode(file_table, inumber);

    
    if (!inode) {

        inode = fs_open_request(file_table, process->name, inumber);

        time(&inode->last_modified);

        LOG_FS_A("Um inode %d foi marcado como arquivo ativo pelo processo %s (%d) durante uma operação de leitura.\n", inumber, process->name, process->id);
    }

    if (!has_opened_file(process, inumber)) {
        AdicionaLista(process->o_files, inumber);

        inode->o_count++;

        LOG_FS_A("O processo %s está ativando pela primeira vez o inode %d durante uma operação de leitura.\n", process->name, inumber);
    }

    time(&inode->last_accessed);

    LOG_FS_A("Processo %s está lendo de %d.\n", process->name, inumber);
}


void fs_write_request(file_table_t* file_table, process_t* process, int block) {
    const int inumber = INODE_NUMBER(block);

    inode_t* inode = BuscaNode(file_table, inumber);

    if (!inode) {

        inode = fs_open_request(file_table, process->name, inumber);

        time(&inode->last_accessed);

        LOG_FS_A("Um inode %d foi marcado como arquivo ativo pelo processo %s (%d) durante uma operação de escrita.\n", inumber, process->name, process->id);

    }

    if (!has_opened_file(process, inumber)) {
        AdicionaLista(process->o_files, inumber);

        inode->o_count++;

        LOG_FS_A("O processo %s está ativando pela primeira vez o inode %d durante uma operação de escrita.\n", process->name, inumber);
    }


    time(&inode->last_modified);

    LOG_FS_A("O processo %s está escrevendo no inode %d.\n", process->name, inumber);
}

inode_t *fs_open_request(file_table_t *file_table, const char *owner, int inumber) {
    inode_t *inode = CriarNode(inumber);

    AdicionaLista(file_table->ilist->inode_list, inode);

    io_fs_log(owner, inumber, IO_LOG_FS_F_OPEN);
    sem_post(&io_mutex);

    return inode;
}


void fs_close_request(file_table_t* file_table, process_t* process, int inumber) {
    FOREACH(file_table->ilist->inode_list, inode_t*) {
        if (it->id == inumber) {
            it->o_count--;


            if (it->o_count == 0) {

                RemoveNode(file_table->ilist->inode_list, curr_node);

                io_fs_log(process->name, inumber, IO_LOG_FS_F_CLOSE);
                sem_post(&io_mutex);
            }

            break;
        }
    }
}