//
// Created by roque on 10/02/2025.
//

typedef struct client{
    int code;
    char name[50];
    char phone[20];
    char birth[20];
    char address[100];
    struct client *next;
} client;

typedef struct pet_type{
    int code;
    char desc[100];
    struct pet_type *next;
} pet_type;

typedef struct pet{
    int code;
    char pet_type_code[2];
    char name[40];
    char *code_client[2];
    struct pet *next;
} pet;

typedef struct {
    char desc[200];
    struct comando *next;
} command;

typedef enum {
    DO_INSERT,
    COMMAND_NOT_RECOGNIZED
} COMMAND_TO_DO;