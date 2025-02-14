/********* nodes.h ********
    Student Name 	= Jordan Trach
    Student Number	= 101293174
*/
#ifndef __A2_BONUS_NODES_H__
#define __A2_BONUS_NODES_H__

/********** DON'T MODIFY **********/
// Structure to represent a linked list of users
typedef struct user
{
    char username[30];
    char password[15];
    struct friend *friends;
    struct post *posts;
    struct user *next;
} user_t;

// Structure to represent a linked list of a user's friends
typedef struct friend
{
    char username[30];
    struct post **posts;
    struct friend *next;
}
friend_t;

// Structure to represent a linked list of a user's posts
typedef struct post
{
    char content[250];
    struct post *next;
} post_t;

#endif
