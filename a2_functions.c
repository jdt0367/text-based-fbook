#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"

// Your solution goes here
/*
    Function to convert a string to lowercase.
*/
void lowercase(char *string){
    for(int i = 0; i < strlen(string); i++){
        string[i] = tolower(string[i]);
    }
    return;
}

/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, const char *password){
    user_t *new = (user_t*)malloc(sizeof(user_t));
    strcpy(new->username, username);
    strcpy(new->password, password);
    new->next = NULL;
    new->friends = NULL;
    new->posts = NULL;
    
    if(users == NULL){ //if first user, give the linked list its first node
        return new;
    }

    user_t *current = users;
    while((current->next != NULL) && (strcmp(new->username, current->next->username) > 0)){
        current = current->next;
    }
    //now we know that we are at the index where current->next's username is larger than username
    //username should be inserted in between current and current->next
   
    new->next = current->next;
    current->next = new;
    return users;
}

/*
   Function that registers a new user.
*/
void register_user(user_t *users){
    char username[30];
    char password[15];
    printf("Please enter your username: ");
    scanf(" %29s", username);
    lowercase(username);
    if (find_user(users, username) != NULL){
        printf("Username already in use! User not added\n");
        return;
    }
    _Bool valid_pwd = false;
    do{
        printf("Please choose your password: ");
        scanf(" %14s", password);
        if(strlen(password) < 8){
            printf("Password must be between 8 and 15 characters\n");
        } else{
            valid_pwd = true;
        }
    } while(!valid_pwd);
    add_user(users, username, password);
    return;
}

/*
   Function that signs into an existing account and displays the app menu.
*/
void signed_in(user_t *user, user_t *users){
    _Bool running = true;
    while(running){
        printf("***********************************************\n");
        printf("                   %s's Page                   \n", user->username);
        printf("***********************************************\n");
        printf("1. Change password\n");
        printf("2. Manage posts (add/remove)\n");
        printf("3. Manage friends (add/remove)\n");
        printf("4. View a friend's posts\n");
        printf("5. Exit to main menu\n");
        printf("Enter your choice:");
        int choice;
        scanf("%d", &choice);
        switch(choice){
            case 1: //Change password
                change_password(user);
                break;
            case 2: //Manage posts
                manage_posts(user, users);
                break;
            case 3: //Manage friends
                manage_friends(user, users);
                break;
            case 4: //See friend posts
                printf("Please enter the username of the friend whose posts you would like to see: ");
                char username[30];
                scanf(" %29s", username);
                if (find_friend(user,username) == NULL){
                    printf("Friend not found\n");
                    break;
                }
                display_all_user_posts(find_user(users,username));
                break;
            case 5:
                running = false;
                printf("Goodbye!");
                return;
            default:
                printf("Invalid choice\n");
                break;
        }
    }
}

/*
   Function that searches if the user has a specific friend.
*/
friend_t *find_friend(user_t *user, const char *friend){
    friend_t *current = user->friends;
    while(current != NULL){
        if(strcmp(friend, current->username) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

/*
   Function that searches if the user is available in the database 
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username){
    user_t *current = users;
    while(current != NULL){
        if(strcmp(username, current->username) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

user_t *credential(user_t *users){
    char username[30];
    char password[15];
    user_t *user;
    printf("Please enter your username: ");
    scanf(" %29s", username);
    user = find_user(users,username);
    if (user == NULL){
        printf("User not found\n");
        return NULL;
    }
    printf("Please enter your password: ");
    scanf(" %14s", password);
    if (strcmp(user->password, password) != 0){
        printf("Incorrect Password\n");
        return NULL;
    }
    return user;
}

void change_password(user_t *user){
    assert(user!=NULL);
    printf("Please enter your current password: ");
    char old[15];
    scanf(" %14s", old);
    if (strcmp(old, user->password) == 0){
        printf("Please enter your new password: ");
        char new[15];
        scanf(" %14s", new);
        strcpy(user->password, new);
        return;
    }
    printf("Incorrect password\n");
    return;
}


/*
   Function that creates a new friend's node.
   Return the newly created node.
*/
friend_t *create_friend(const char *username, user_t *users){
    friend_t *new = malloc(sizeof(friend_t));
    assert(new != NULL);
    strcpy(new->username, username);
    new->next = NULL;
    new->posts = &find_user(users, username)->posts;
    return new;
}


/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend, user_t *users){
    assert(user!=NULL);
    friend_t *new = create_friend(friend, users);
    friend_t *current = user->friends;

    if(user->friends == NULL){ //if first user, give the linked list its first node
        user->friends = new;
        return;
    }
    
    while((current->next != NULL) && (strcmp(new->username, current->next->username) > 0)){
        if (strcmp(new->username, current->next->username) == 0)
        {
            printf("Friend already added\n");
            return;
        }
        
        current = current->next;
    }
    //next friend in list is now the first one greater than friend name
    new->next = current->next;
    current->next = new;
    return;
}

/*
   Function that removes a friend from a user's friend list.
   Return true of the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name){
    if(user != NULL){
        friend_t *current = user->friends;
        if(strcmp(friend_name, current->username) == 0){
            friend_t *reserve = current;
            user->friends = current->next;
            free(reserve);
            return true;
        }
        while((current->next != NULL) && (strcmp(friend_name, current->next->username) != 0)){
            current = current->next;
        }
        //current->next is now the appropriate friend to delete
        if ((strcmp(friend_name, current->next->username) == 0)){ //if we have not found friend, skip and return false
            friend_t *reserve = current->next;
            current->next = current->next->next;
            free(reserve);
            return true;
        }
    }
    return false;
}

void manage_friends(user_t *user, user_t *users){
    assert(user!=NULL);
    int choice;
    char friend[30];
    while(true){
        printf("***********************************************\n");
        printf("                  MANAGE FRIENDS               \n");
        printf("***********************************************\n");
        display_user_friends(user);
        printf("1. Add new friend\n");
        printf("2. Remove friend\n");
        printf("3. Back\n");
        printf("Enter your choice:");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                printf("Please enter the username of your new friend: ");
                scanf(" %29s", friend);
                if (find_user(users,friend) == NULL){
                    printf("User not found\n");
                    break;
                }
                add_friend(user, friend, users);
                break;
            case 2:
                printf("Please enter the username of the friend you wish to remove: ");
                scanf(" %29s", friend);
                if (find_user(users,friend) == NULL){
                    printf("User not found\n");
                    break;
                }
                if (!delete_friend(user, friend)){
                    printf("Friend not found");
                }
                break;
            case 3:
                return;
        }    
    }
    return;
}

/*
   Submenu function that allows the user to manage their posts.
*/
void manage_posts(user_t *user, user_t *users){
    assert(user != NULL);
    int choice;
    while(true){
        printf("***********************************************\n");
        printf("                  MANAGE POSTS                    \n");
        printf("***********************************************\n");
        printf("1. Share new post\n");
        printf("2. Remove last post\n");
        printf("3. Back\n");
        printf("Enter your choice:");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                printf("Type your post and hit <ENTER> when finished: ");
                char text[250];
                scanf(" %249[^\n]s", text);
                add_post(user, text);
                printf("Post added!\n");
                break;
            case 2:
                _Bool deleted = delete_post(user);
                if (!deleted){
                    printf("No posts found\n");
                    break;
                }
                printf("Post deleted!\n");
                break;
            case 3:
                return;
        }    
    }
    return;
}

/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text){
    post_t *newpost = (post_t*)malloc(sizeof(post_t));
    assert(newpost != NULL);
    strcpy(newpost->content, text);
    newpost->next = NULL;
    return newpost;
}

/*
   Function that adds a post to a user's timeline following a stack.
*/
void add_post(user_t *user, const char *text){
    post_t *newpost = create_post(text);
    assert(newpost != NULL);
    if (user->posts == NULL){
        user->posts = newpost;
        return;
    }
    newpost->next = user->posts;
    user->posts = newpost;
    return;
}

/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user){
    if(user != NULL && user->posts != NULL){
        post_t *reserve = user->posts;
        user->posts = user->posts->next;
        free(reserve);
        return true;
    }
    return false;
}

/*Function to print a specific post*/
void print_post(post_t *post, user_t *user){

    printf("\n%s Posted:\n", user->username);
    printf(">>>%s\n\n", post->content);
    return;
}

/*
   Function that  displays a specific user's posts (All posts)
*/
void display_all_user_posts(user_t *user){
    assert(user != NULL);
    printf("***********************************************\n");
    printf("%s's Posts:\n", user->username);
    display_posts_by_n(user, 3);
    printf("***********************************************\n");
    return;
}

/*
   Function that displays "number" of posts for a given user.
   After displaying "number" posts, it prompts if you want to display
   more posts.
   If there are no more post or the user types “n” or “N”, the function returns. 
*/
void display_posts_by_n(user_t *users, int number){
    assert(users != NULL);
    post_t *current_post = users->posts;
    char yn;
    _Bool run = true;
    while(current_post != NULL && run){
        for(int i = 0; i < 3; i++){
            print_post(current_post, users);
            current_post = current_post->next;
            if (current_post == NULL){
                printf("End of posts.\n");
                return;
            }
        }
        printf("Would you like to display more posts (Y/N): "); //doesnt actually take anything
        scanf(" %c", &yn);
        if (yn == 'N' || yn == 'n')
            run = false;
    }
    return;
}


/*
   Function that displays a specific user's friends (All friends)
*/
void display_user_friends(user_t *user){
    assert(user != NULL);
    printf("***********************************************\n");
    printf("%s's Friends:\n", user->username);
    int num = 1;
    friend_t *current_friend = user->friends;
    while (current_friend != NULL){
        printf("%d) %s\n", num, current_friend->username);
        num++;
        current_friend = current_friend->next;
    }
    printf("***********************************************\n");
    return;
}

/*
   Function that free all users from the database before quitting the application.
*/
void teardown(user_t *users){
    if (users == NULL){
        return;
    }
    teardown(users->next);
    free(users);
    return;
}

/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu(){
    printf("***********************************************\n");
    printf("                  MAIN MENU                    \n");
    printf("***********************************************\n");
    printf("1. Register a new user\n");
    printf("2. Sign in\n");
    printf("3. Exit\n");
    printf("Enter your choice:");
    return;
}


/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token, users);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
