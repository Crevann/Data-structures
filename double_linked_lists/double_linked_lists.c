#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define linked_list (list_node **)&my_linked_list

typedef struct list_node {
    struct list_node *prev;
    struct list_node *next;
} list_node;

typedef struct string_item {
    list_node node;
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
    item->prev = tail;
    item->next = NULL;
    return item;
}

list_node *list_remove(list_node **head, list_node *item) {
    if(*head == item){
        *head = (*head)->next;
        free(item);
        return item;
    }
    list_node *previous_item = item->prev;
    list_node *next_item = item->next;
    previous_item->next = item->next;
    next_item->prev = item->prev;
    free(item);
    return item;
}

list_node *list_insert(list_node *position, list_node *item, bool previous) {
    list_node *previous_item = position->prev;
    list_node *next_item = position->next;
    if (previous) {
        previous_item->next = item;
        item->next = position;
        item->prev = previous_item;
        position->prev = item;
    } else {
        item->next = next_item;
        item->prev = position;
        next_item->prev = item;
        position->next = item;
    }
    return item;
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
    list_node *head_to_remove = list_append(linked_list, (list_node *)string_item_new("^o3o^"));
    list_node *position_node = list_append(linked_list, (list_node *)string_item_new("^ouo^"));
    list_node *item_to_remove = list_append(linked_list, (list_node *)string_item_new("^o.o^"));
    list_append(linked_list, (list_node *)string_item_new("^owo^"));
    list_insert(position_node, (list_node *)string_item_new("^<3<^"), true);
    list_insert(position_node, (list_node *)string_item_new("^>3>^"), false);
    list_remove(linked_list, head_to_remove);
    list_remove(linked_list, item_to_remove);
    string_item *current_string = my_linked_list;
    while (current_string) {
        printf("%s\n", current_string->string);
        current_string = (string_item *)current_string->node.next;
    }

    return 0;
}