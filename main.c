/*   
-------------------------------------------------------------------------------------
* Name          : sourabh bahubali baragale
* Student id    : 24024_114
* Date          : 28th Feb 2025 
* Title         : Inverted Search
-------------------------------------------------------------------------------------
*/

#include "main.h"

int main(int argc, char *argv[])
{
    Slist *head = NULL;
    hash_t table[HASH_SIZE];
    if (argc < 1)
    {
        printf("INFO: please follow ./a.out<space>filenames for command line input.\n");
        return 0;
    }
    else
    {
        printf("Validating Files");
        loading();
        if(read_and_validate(argc, argv, &head)==SUCCESS)
        {
            create_hash_table(table, HASH_SIZE);
        }
    }
    int op;
    Slist *head2=NULL;
    int count=0,flag=0;
    while (1)
    {
        printf("--------------------------------------------------------------------------\n");
        printf("                                HOME\n");
        printf("--------------------------------------------------------------------------\n");
        printf("1. Create Database\n2. Display Database\n3. Save Database\n4. Search word\n5. Update Database\n6. Exit\n");
        printf("--------------------------------------------------------------------------\n");
        printf("Enter your option : ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:       
            if(count>0)
            {
                if(flag==1)
                {
                    Slist *res_head=NULL;
                    if(compare_and_store(&head,&head2,&res_head)==SUCCESS)
                    {
                        if (create_data_base(res_head, table) == SUCCESS)
                        {
                            printf("INFO : Database created successfully.\n");
                            flag=0;
                            count++;
                        }
                        else
                            printf("ERROR : Database creation Failed!\n");
                    }
                    else
                        printf("INFO : Provided file(s) already exist in database!\n");
                }
                else
                    printf("INFO : Cannot create Database! since database is already created.\n");
                break;
            }
            if (create_data_base(head, table) == SUCCESS)
            {
                printf("INFO : Database created successfully.\n");
                count++;
            }
            else
                printf("ERROR : Database creation Failed!\n");
            break;
        case 2:
            display_HT(table, HASH_SIZE);
            break;
        case 4:
            printf("--------------------------------------------------------------------------\n");
            printf("                                SEARCH\n");
            printf("--------------------------------------------------------------------------\n");
            char word[50];
            printf("Enter the word: ");
            scanf("%s", word);
            printf("\n");
            if (search_data(word, table) == FAILURE)
                printf("INFO: THE word %s is not present in the database\n", word);
            printf("--------------------------------------------------------------------------\n");
            break;
        case 3:
            if(save_database(table,HASH_SIZE)==SUCCESS)
                printf("INFO : Database saved Successfully.\n");
            else
                printf("ERROR : Saving Unsuccessfull!\n");
            break;
        case 5 :
            if(count>0)
            {
                printf("INFO : Cannot Update Database! since database is already created/Updated, Exit and try again :)\n");
                break;
            }
            char file[SIZE];
            printf("Enter the backup file name : ");
            scanf("%s",file);
            if(update_database(file,table,&head2)==SUCCESS)
            {
                printf("INFO : Database Updated Successfully.\n");
                count++;
                flag=1;
            }
            else
                printf("ERROR : Database Update Failed!\n");
            print_list(head);
            break;
        case 6:
            return 0;
        default:
            printf("INFO : Invalid Input!\n");
            break;
        }
    }
}