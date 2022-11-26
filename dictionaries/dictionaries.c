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

typedef struct dict_node {
    const char *value;
    const char *key;
    size_t key_len;
    struct dict_node *next;
} dict_node;

typedef struct dict_table {
    dict_node **nodes;
    size_t hashmap_size;
} dict_table;

dict_node *dict_search_value_from_key(dict_table *table, const char *key, const size_t key_len);

dict_table *dict_table_new(const size_t hashmap_size) {
    dict_table *table = malloc(sizeof(struct dict_table));
    if (!table) {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(dict_node *));
    if (!table->nodes) {
        free(table);
        return NULL;
    }
    return table;
}

dict_node *dict_insert(dict_table *table, const char *value, const char *key, const size_t key_len) {
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (dict_search_value_from_key(table, key, key_len) != NULL) {
        return NULL;
    }
    dict_node *head = table->nodes[index];
    if (!head) {
        table->nodes[index] = malloc(sizeof(dict_node));
        if (!table->nodes[index]) {
            return NULL;
        }
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->value = value;
        table->nodes[index]->next = NULL;
        return table->nodes[index];
    }
    dict_node *new_item = malloc(sizeof(dict_node));
    if (!new_item) {
        return NULL;
    }
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->value = value;
    new_item->next = NULL;
    dict_node *tail = head;
    while (head) {
        tail = head;
        head = head->next;
    }
    tail->next = new_item;
    return new_item;
}

dict_node *dict_search_value_from_key(dict_table *table, const char *key, const size_t key_len) {
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (!table->nodes[index]) {
        return NULL;
    }
    return table->nodes[index];
}

dict_node *dict_remove(dict_table *table, const char *key, const size_t key_len){
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (!table->nodes[index]) {
        return NULL;
    }
    table->nodes[index]->key = NULL;
    table->nodes[index]->key_len = 0;
    table->nodes[index]->next = NULL;
    table->nodes[index]->value = NULL;
    free(table->nodes[index]); //For some reason it doesn't free, needs fixing
    return table->nodes[index];
}

int main(int argv, char **argc) {
    const size_t hashmap_size = 128;
    dict_table *table = dict_table_new(hashmap_size);
    dict_insert(table, "^o3o^", "one", 4);
    dict_insert(table, "^o3o^", "two", 4);
    dict_insert(table, "^ono^", "one", 4);
    dict_insert(table, "^owo^", "three", 6);
    //dict_remove(table, "two", 4);

    for (size_t i = 0; i < hashmap_size; i++)
    {
        if(table->nodes[i] != NULL){
            printf("%s with key %s\n", table->nodes[i]->value, table->nodes[i]->key);
        }
    }

    dict_node *found_node = dict_search_value_from_key(table, "two", 4);
    if (found_node != NULL) {
        printf("found: %s\n", found_node->value);
    } else {
        printf("found nothing\n");
    }
}