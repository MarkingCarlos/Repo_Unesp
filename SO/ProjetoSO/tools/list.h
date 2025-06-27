#ifndef OS_PROJECT_LIST_H
#define OS_PROJECT_LIST_H

#define FOREACH(LIST, GENERIC_TYPE)                                                                     \
            GENERIC_TYPE it = ((LIST)->head == NULL) ? NULL : (GENERIC_TYPE)((LIST)->head)->content;    \
            for (list_node_t* curr_node = (LIST)->head;                                                 \
                 curr_node != NULL; curr_node = curr_node->next,                                        \
                                    it = (curr_node == NULL ? NULL : (GENERIC_TYPE)curr_node->content)) \


typedef struct ListNode {

    void *content;


    struct ListNode *next;


    struct ListNode *prev;
} list_node_t;


typedef struct List {

    list_node_t *head;


    list_node_t *tail;


    int size;
} list_t;


list_node_t *list_node_create(void *content);


list_t* IniciaLista();


void AdicionaLista(list_t *list, void *content);


list_node_t *BuscaLista(list_t *list, void *content, int (*matcher)(void *, void *));


int list_empty(list_t *list);


list_node_t *list_remove_head(list_t *list);


list_node_t *list_remove_tail(list_t *list);

list_node_t* list_remove(list_t* list, void* content);



void RemoveNode(list_t *list, list_node_t *node);


void list_free(list_t *list);

#endif 
