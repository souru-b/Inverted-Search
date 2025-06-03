#include "main.h"

int create_data_base(Slist *head, hash_t *table)
{
    Slist *temp = head;
    while (temp != NULL)
    {

        FILE *fptr = fopen(temp->f_name, "r");
        if (fptr == NULL)
        {
            printf("INFO: file open failed!\n");
            return FAILURE;
        }
        // return SUCCESS;
        if (read_and_store(fptr, table, temp->f_name) == FAILURE)
            return FAILURE;
        temp = temp->link;
    }
    return SUCCESS;
    // printf("i : %d\n",i);
}

int read_and_validate(int argc, char *argv[], Slist **head)
{
    for (int i = 1; i < argc; i++)
    {
        if(validate_file(argv[i])==SUCCESS)
        {
            Slist *temp = *head;
            int flag = 0;
            while (temp != NULL)
            {
                // printf("name: %s",temp->f_name);
                if (!strcmp(temp->f_name, argv[i]))
                {
                    flag = 1;
                    break;
                }
                temp = temp->link;
            }
            if (flag != 1)
            {
                if (insert_at_last(head, argv[i]) == SUCCESS)
                    printf("File %s validation successfull and stored in the list\n", argv[i]);
            }
            else
                printf("ERROR : File %s already exist in the list\n", argv[i]);
        }
    }
    print_list(*head);
    return SUCCESS;
}

int read_and_store(FILE *fptr, hash_t *table, char *f_name)
{
    char buff[SIZE];
    int ind;

     // flag for same word case and count for same file case
    while (fscanf(fptr, "%s", buff) != EOF)
    {
        int flag = 0;
        if (buff[0] >= 'a' && buff[0] <= 'z')
            ind = buff[0] - 'a';
        else if (buff[0] >= 'A' && buff[0] <= 'Z')
            ind = buff[0] - 'A';
        else
            ind = 26;
        main_node *temp2 = table[ind].link;
        while (temp2 != NULL)
        {
            if (!strcmp(buff, temp2->word))
            {
                flag = 1;
                sub_node *sub_temp = temp2->sub_link;
                while (sub_temp != NULL)
                {
                    if (!strcmp(f_name, sub_temp->file_name))
                    {
                        sub_temp->word_count++;
                        break;
                    }
                    else
                        sub_temp = sub_temp->link;
                }
                if (sub_temp==NULL)
                {
                    sub_node *new = malloc(sizeof(sub_node));
                    if (new == NULL)
                    {
                        printf("INFO : sub node creation failed!\n");
                        return FAILURE;
                    }
                    strcpy(new->file_name, f_name);
                    new->link = NULL;
                    new->word_count = 1;
                    if (table[ind].link->main_link == NULL)
                    {
                        sub_node *temp_sub = temp2->sub_link;
                        while (temp_sub->link != NULL)
                        {
                            temp_sub = temp_sub->link;
                        }
                        temp_sub->link = new;
                        temp2->file_count++;
                    }
                    else
                    {
                        sub_node *temp_sub = temp2->sub_link;
                        while (temp_sub->link != NULL)
                        {

                            temp_sub = temp_sub->link;
                        }
                        temp_sub->link = new;
                        temp2->file_count++;
                        break;
                    }
                }
                break;
            }
            temp2 = temp2->main_link;
        }
        if (flag == 0)
        {
            main_node *new = malloc(sizeof(main_node));
            if (new == NULL)
            {
                printf("INFO : main node creation failed!\n");
                return FAILURE;
            }
            strcpy(new->word, buff);
            new->file_count = 1;
            if (table[ind].link == NULL)
            {
                table[ind].link = new;
                new->main_link = NULL;
                new->sub_link = NULL;
                sub_node *new = malloc(sizeof(sub_node));
                if (new == NULL)
                {
                    printf("INFO : main node creation failed!\n");
                    return FAILURE;
                }
                strcpy(new->file_name, f_name);
                new->link = NULL;
                new->word_count = 1;
                table[ind].link->sub_link = new;
            }
            else
            {
                main_node *temp = table[ind].link;
                while (temp->main_link != NULL)
                {
                    temp = temp->main_link;
                }
                temp->main_link = new;
                new->main_link = NULL;
                new->sub_link = NULL;
                sub_node *new_sub = malloc(sizeof(sub_node));
                if (new_sub == NULL)
                {
                    printf("INFO : sub node creation failed!\n");
                    return FAILURE;
                }
                strcpy(new_sub->file_name, f_name);
                new_sub->link = NULL;
                new_sub->word_count = 1;
                new->sub_link = new_sub;
            }
            new->main_link = NULL;
        }
    }
    return SUCCESS;
}