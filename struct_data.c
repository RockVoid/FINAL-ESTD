//
// Created by roque on 10/02/2025.
//

typedef struct {
    int code;
    char name[50];
    char phone[20];
    char birth[20];
    char address[100];
    struct client *next;
} client;

typedef struct {
    int code;
    char desc[100];
} pet_type;

typedef struct {
    int code;
    int *code_client;
    char name[40];
    char *pet_type_code;
} pet;

typedef struct {
    char desc[200];
    struct comando *next;
} command;

typedef enum {
    DO_INSERT,
    COMMAND_NOT_RECOGNIZED
} COMMAND_TO_DO;
