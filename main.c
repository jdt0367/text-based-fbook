/********* main.c *********/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
    //print main menu
    _Bool running = true;
    while(running){
        print_menu();
        int choice;
        scanf("%d", &choice);
        switch(choice){
            case 1: //Register new user
                register_user(users);
                break;
            case 2: //Sign in with current user
                signed_in(credential(users), users);
                break;
            case 3: //Exit
                running = false;
                printf("Goodbye!\n");
                return EXIT_SUCCESS;
            default:
                printf("Invalid choice\n");
                break;
        }
    }
}
