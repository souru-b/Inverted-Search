#ifndef INVERT_H
#define INVERT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SIZE 50
#define HASH_SIZE 27
#define SUCCESS 1
#define FAILURE 0
#define RESET "\033[0m"
#define YELLoW "\033[33m"
typedef struct node
{
    char f_name[SIZE];
    struct node *link;
} Slist;

typedef struct node1
{
    int word_count;
    char file_name[SIZE];
    struct node1 *link;
} sub_node;

typedef struct node2
{
    int file_count;
    char word[SIZE];
    struct node2 *main_link;
    sub_node *sub_link;
} main_node;

typedef struct hash
{
    int index;
    main_node *link;
} hash_t;

int create_data_base(Slist *head, hash_t *table);
int read_and_validate(int argc, char *argv[], Slist **head);
int create_hash_table(hash_t *table, int size);
int read_and_store(FILE *fptr, hash_t *table, char *f_name);
void display_HT(hash_t *arr, int size);
void print_list(Slist *head);
int insert_at_last(Slist **head, char *data);
int search_data(char *data, hash_t *table);
int save_database(hash_t *table,int size);
int update_database(char* file,hash_t *table, Slist **head);
int validate_file(char* file);
int compare_and_store(Slist **head1,Slist **head2,Slist **res_head);
int insert_into_table(hash_t *table,int index, char *word, int file_count, FILE *fptr, Slist **head);
#endif