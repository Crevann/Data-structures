#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t djb33x_hash(const char *key, const size_t keylen) {
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

typedef struct set_node {
    const char *key;
    size_t key_len;
    struct set_node *next;
} set_node;

typedef struct set_table {
    set_node **nodes;
    size_t hashmap_size;
} set_table;

set_node *set_search(set_table *table, const char *key, const size_t key_len);

set_table *set_table_new(const size_t hashmap_size) {
    set_table *table = malloc(sizeof(struct set_table));
    if (!table) {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(set_node *));
    if (!table->nodes) {
        free(table);
        return NULL;
    }
    return table;
}

set_node *set_insert(set_table *table, const char *key, const size_t key_len) {
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (set_search(table, key, key_len) != NULL) {
        return NULL;
    }
    set_node *head = table->nodes[index];
    if (!head) {
        table->nodes[index] = malloc(sizeof(set_node));
        if (!table->nodes[index]) {
            return NULL;
        }
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->next = NULL;
        return table->nodes[index];
    }
    set_node *new_item = malloc(sizeof(set_node));
    if (!new_item) {
        return NULL;
    }
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    set_node *tail = head;
    while (head) {
        tail = head;
        head = head->next;
    }
    tail->next = new_item;
    return new_item;
}

set_node *set_search(set_table *table, const char *key, const size_t key_len) {
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (!table->nodes[index]) {
        return NULL;
    }
    return table->nodes[index];
}

set_node *set_remove(set_table *table, const char *key, const size_t key_len){
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (!table->nodes[index]) {
        return NULL;
    }
    table->nodes[index]->key = NULL;
    table->nodes[index]->key_len = 0;
    table->nodes[index]->next = NULL;
    free(table->nodes[index]);
    return table->nodes[index];
}

int main(int argv, char **argc) {
    const size_t hashmap_size = 128;
    set_table *table = set_table_new(hashmap_size);
    set_insert(table, "^o3o^", 6);
    set_insert(table, "^o3o^", 6);  // Already exist, should be blocked
    set_insert(table, "^owo^", 6);
    set_insert(table, "^ouo^", 6);
    set_insert(table, "^ono^", 6);
    set_insert(table, "^oovoo^", 8);
    set_remove(table, "^oovoo^", 8);

    for (size_t i = 0; i < hashmap_size; i++)
    {
        if(table->nodes[i] != NULL){
            printf("%s with hash %i\n", table->nodes[i]->key, djb33x_hash(table->nodes[i]->key, table->nodes[i]->key_len));
        }
    }

    set_node *found_node = set_search(table, "^owo^", 6);
    if (found_node != NULL) {
        printf("found: %s\n", found_node->key);
    } else {
        printf("found nothing\n");
    }
}