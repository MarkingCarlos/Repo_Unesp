#include <stdio.h>
#include <stdlib.h>

#include "list.h"


list_node_t *list_node_create(void *content) {
    register int i;
    list_node_t *node = (list_node_t *)malloc(sizeof(list_node_t));


    if (!node) {
        printf("Sem memoria... \n");
        return NULL;
    }

    node->content = content;
    node->next = NULL;
    node->prev = NULL;

    return node;
}


list_t* IniciaLista() {
    list_t* list = (list_t *)malloc(sizeof(list_t));

    
    if (!list) {
        printf("Sem memoria...\n");
        exit(0);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


void AdicionaLista(list_t *list, void *content) {
    list_node_t *node = list_node_create(content);

    if (!node)
        return;

    
    if (!list->tail)
        list->head = node;
    else {
        list->tail->next = node;
        node->prev = list->tail;
    }

    list->tail = node;
    list->size++;
}


list_node_t *BuscaLista(list_t *list, void *content, int (*matcher)(void *, void *)) {
    list_node_t* matched;
    for (matched = list->head; matched != NULL; matched = matched->next)
        if (matcher(matched->content, content))
            return matched;
    return NULL;
}


int list_empty(list_t *list) {
    return list->head == NULL;
}


list_node_t *list_remove_head(list_t *list) {
    list_node_t *temp = list->head;

    if (list->head) {
        if (list->head->next)
            list->head->next->prev = NULL;
        else list->tail = NULL;
        list->head = list->head->next;

        
        temp->next = NULL;
        temp->prev = NULL;
        list->size--;
    }

    return temp;
}

list_node_t* list_remove(list_t* list, void* content) {
    list_node_t* prev = NULL;
    list_node_t* curr = list->head;

    while (curr) {
        if (curr->content == content) {
            if (prev) {
                prev->next = curr->next;
            } else {
                list->head = curr->next;
            }

            if (curr == list->tail) {
                list->tail = prev;
            }

            list->size--;
            return curr;
        }

        prev = curr;
        curr = curr->next;
    }

    return NULL;
}



void RemoveNode(list_t *list, list_node_t *node) {
    if (node->prev)
        node->prev->next = node->next;
    else if (list->head == node) list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else if (list->tail == node) list->tail = node->prev;

    list->size--;
}


list_node_t *list_remove_tail(list_t *list) {
    list_node_t *temp = list->tail;

    if (list->tail) {
        if (list->tail->prev)
            list->tail->prev->next = NULL;
        else list->head = NULL;
        list->tail = list->tail->prev;

        
        temp->next = NULL;
        temp->prev = NULL;
        list->size--;
    }

    return temp;
}


void list_free(list_t *list) {
    if (list) {
        list_node_t *curr = list->head;
        list_node_t *temp;

        while (curr) {
            temp = curr->next;
            free(curr);
            curr = temp;
        }

        free(list);
    }
}

