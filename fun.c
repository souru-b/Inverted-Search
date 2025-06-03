#include "main.h"

int create_hash_table(hash_t *table, int size)
{

    for (int i = 0; i < size; i++)
    {

        table[i].index = i;
        // printf("main\n");
        table[i].link = NULL;
    }
    return SUCCESS;
}

void display_HT(hash_t *arr, int size)
{
    // hash_t *temp;
    int flag=0;
    for(int i=0;i<size;i++)
    {
        if(arr[i].link!=NULL)
        {
            flag=1;
            break;
        }
    }
    if(flag==0)
    {
        printf("INFO : Database is Empty\n");
        return;
    }
    printf("--------------------------------------------------------------------------\n");
    printf("%-10s %-10s %-15s %-15s %-10s", "Index", "word", "file count", "file name", "word count\n");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < size; i++)
    {
        if (arr[i].link != NULL)
        {
            if (arr[i].link->main_link != NULL)
            {
                main_node *temp = arr[i].link;
                while (temp != NULL)
                {
                    printf("%-10d %-14s %-14d", i, temp->word, temp->file_count);
                    sub_node *temp2 = temp->sub_link;
                    while (temp2 != NULL)
                    {
                        printf("%-15s %-10d", temp2->file_name, temp2->word_count);
                        if (temp2->link != NULL)
                        {
                            printf("\n");
                            printf("%-40s", "");
                        }
                        temp2 = temp2->link;
                    }
                    temp = temp->main_link;
                    printf("\n");
                }
                printf("\n");
            }
            else
            {
                printf("%-10d %-14s %-14d", i, arr[i].link->word, arr[i].link->file_count);
                sub_node *temp2 = arr[i].link->sub_link;
                while (temp2 != NULL)
                {
                    printf("%-15s %-10d", temp2->file_name, temp2->word_count);
                    if (temp2->link != NULL)
                    {
                        printf("\n");
                        printf("%-40s", "");
                    }
                    temp2 = temp2->link;
                }
                printf("\n");
            }
        }
    }
}

int insert_at_last(Slist **head, char *data)
{
    Slist *new = malloc(sizeof(Slist));
    if (new == NULL)
        return FAILURE;
    strcpy(new->f_name, data);
    if (*head == NULL)
    {
        *head = new;
        new->link = NULL;
        return SUCCESS;
    }
    Slist *temp = *head;
    while (temp != NULL)
    {
        if (temp->link == NULL)
        {
            temp->link = new;
            new->link = NULL;
            return SUCCESS;
        }
        temp = temp->link;
    }
}

int search_data(char *data, hash_t *table)
{
    int ind;
    if (data[0] >= 'a' && data[0] <= 'z')
        ind = data[0] - 'a';
    else if (data[0] >= 'A' && data[0] <= 'Z')
        ind = data[0] - 'A';
    else
        ind = 26;
    main_node *temp = table[ind].link;
    while (temp != NULL)
    {
        if (!strcmp(data, temp->word))
        {
            sub_node *temp2 = temp->sub_link;
            int i = 1;
            printf("The word %s is present in %d file(s) : \n\n", data, temp->file_count);
            while (temp2 != NULL)
            {
                printf("%d. In %s for %d time(s)\n\n", i++, temp2->file_name, temp2->word_count);
                temp2 = temp2->link;
            }
            return SUCCESS;
        }
        temp = temp->main_link;
    }
    return FAILURE;
}

void print_list(Slist *head)
{
    if (head == NULL)
    {
        printf("INFO : List is empty\n");
    }
    else
    {
        while (head)
        {
            printf("%s -> ", head->f_name);
            head = head->link;
        }

        printf("NULL\n");
    }
}

int save_database(hash_t *table,int size)
{
    FILE *fptr=fopen("backup.txt","w");
    if(fptr==NULL)
    {
        printf("File open failed!\n");
        return FAILURE;
    }
    for(int i=0;i<size;i++)
    {
        if(table[i].link!=NULL)
        {
            main_node *temp=table[i].link;
            while(temp!=NULL)
            {
                fprintf(fptr,"#");
                fprintf(fptr,"%d;%s;%d;",i,temp->word,temp->file_count);
                sub_node *temp2=temp->sub_link;
                while(temp2!=NULL)
                {
                    fprintf(fptr,"%s;%d;",temp2->file_name,temp2->word_count);
                    temp2=temp2->link;
                }
                fprintf(fptr,"#");
                temp=temp->main_link;
                fprintf(fptr,"\n");
            }
           
        }
    }
    fclose(fptr);
    return SUCCESS;
}

int update_database(char* file,hash_t *table, Slist **head)
{
    int index, file_count;
    char word[SIZE];
    FILE *fptr=fopen(file,"r");
    if(fptr==NULL)
    {
        printf("INFO : file open failed!\n");
        return FAILURE;
    }
    while (fscanf(fptr, "#%d;%[^;];%d;", &index, word, &file_count) != EOF)
    {
        insert_into_table(table,index, word, file_count, fptr,head);
        fscanf(fptr, "%*[^\n]\n");  // to move to next line
    }
    return SUCCESS;
}

int insert_into_table(hash_t *table,int index, char *word, int file_count, FILE *fptr, Slist **head)
{
    main_node *new_main = malloc(sizeof(main_node));
    if (new_main==NULL)
    {
        printf("INFO : main node creation Failed!\n");
        return FAILURE;
    }
    new_main->file_count = file_count;
    strcpy(new_main->word, word);
    new_main->main_link = NULL;
    new_main->sub_link = NULL;
    sub_node *prev_sub = NULL;
    for (int i = 0; i < file_count; i++)
    {
        sub_node *new_sub = malloc(sizeof(sub_node));
        if (new_sub==NULL)
        {
            printf("INFO : sub node creation Failed!\n");
            return FAILURE;
        }
        
        fscanf(fptr, "%[^;];%d;", new_sub->file_name, &new_sub->word_count);
        new_sub->link = NULL;
        Slist *s_temp=*head;
        int flag=0;
        while(s_temp!=NULL)
        {
            if(!strcmp(s_temp->f_name,new_sub->file_name))
            {
                flag=1;
                break;
            }
            s_temp=s_temp->link;
        }
        if(flag!=1)
            insert_at_last(head,new_sub->file_name);
        if (prev_sub == NULL)
        {
            new_main->sub_link = new_sub;
        }
        else
        {
            prev_sub->link = new_sub;
        }
        prev_sub = new_sub;
    }
    if (table[index].link == NULL)
    {
        table[index].link = new_main;
    }
    else
    {
        main_node *temp = table[index].link;
        while (temp->main_link != NULL)
        {
            temp = temp->main_link;
        }
        temp->main_link = new_main;
    }
}

int validate_file(char* file)
{
    if(!strcmp(strstr(file, "."), ".txt"))
    {
        FILE *fptr = fopen(file, "r");
        if (fptr == NULL)
        {
            printf("ERROR: File %s doesn't exist!\n", file);
            return FAILURE;
        }
        else
        {
            fseek(fptr, 0, SEEK_END);
            if (ftell(fptr) == 0)
            {
                printf("ERROR: File %s has no content or Data!\n", file);
                return FAILURE;
            }
        }
        fclose(fptr);
    }
    else
    {
        printf("ERROR: file %s extension is invalid!\n", file);
        return FAILURE;
    }
    return SUCCESS;
}


int compare_and_store(Slist **head1,Slist **head2,Slist **res_head)
{
    Slist *temp2=*head1;
    while(temp2!=NULL)
    {
        Slist *temp1=*head2;
        int flag=0;
        while(temp1!=NULL)
        {
            if(!strcmp(temp2->f_name,temp1->f_name))
            {
                flag=1;
                break;
            }
            temp1=temp1->link;
        }
        if(flag==0)
        {
            insert_at_last(res_head,temp2->f_name);
        }
        temp2=temp2->link;
    }
    if(*res_head==NULL)
        return FAILURE;
    else
        return SUCCESS;
        
}