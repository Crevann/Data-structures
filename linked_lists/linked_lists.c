#include <stddef.h>  // required for NULL
#include <stdio.h>
#include <stdlib.h>

#define linked_list (list_node **)&my_linked_list

typedef struct list_node {
    struct list_node *next;
} list_node;

typedef struct string_item {
    struct list_node node;
    const char *string;
} string_item;

list_node *list_get_tail(list_node **head) {
    list_node *current_node = *head;
    list_node *last_node = NULL;
    while (current_node) {
        last_node = current_node;
        current_node = current_node->next;
    }
    return last_node;
}

list_node *list_append(list_node **head, list_node *item) {
    list_node *tail = list_get_tail(head);
    if (!tail) {
        *head = item;
    } else {
        tail->next = item;
    }
    item->next = NULL;
    return item;
}

list_node *list_pop(list_node **head) {
    list_node *current_head = *head;
    if (!current_head) {
        return NULL;
    }
    *head = (*head)->next;
    current_head->next = NULL;
    return current_head;
}

list_node *list_remove(list_node **head, list_node *item) {
    list_node *node_to_edit = *head;
    if (item == *head) {
        *head = (*head)->next;
        free(item);
        return item;
    }
    while (node_to_edit->next != item) {
        node_to_edit = node_to_edit->next;
    }
    node_to_edit->next = item->next;
    free(item);
    return item;
}

list_node *list_reverse(list_node **head) {
    list_node *current_item = *head;
    list_node *next_item = (*head)->next;
    list_node *previous_item = NULL;
    while (current_item) {
        next_item = current_item->next;
        if(!next_item) { *head = current_item; }
        current_item->next = previous_item;
        previous_item = current_item;
        current_item = next_item;
    }
    return *head;
}

string_item *string_item_new(const char *string) {
    string_item *item = malloc(sizeof(string_item));
    if (!item) {
        return NULL;
    }
    item->string = string;
    return item;
}

int main(int argc, char **argv) {
    string_item *my_linked_list = NULL;
    list_node *node_to_remove = list_append(linked_list,
                                            (list_node *)string_item_new("Hello World"));
    list_append(linked_list,
                (list_node *)string_item_new("Test001"));
    list_node *node_to_remove_2 = list_append(linked_list,
                                              (list_node *)string_item_new("Test002"));
    list_append((list_node **)&my_linked_list,
                (list_node *)string_item_new("Last Item of the Linked List"));
    //list_remove(linked_list, node_to_remove);
    //list_remove(linked_list, node_to_remove_2);
    list_reverse(linked_list);
    string_item *string_item = my_linked_list;
    while (string_item) {
        printf("%s\n", string_item->string);
        string_item = (struct string_item *)string_item->node.next;
    }
    return 0;
}