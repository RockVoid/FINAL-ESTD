#include<stdio.h>
#include<string.h>

#define INSERT_COMMAND "insert into"

typedef struct {
    int code;
    char *name;
    char *phone;
    char *birth;
    char *address;
} client;

typedef struct {
    int code;
    char *desc;
} pet_type;

typedef struct {
    int code;
    int *code_client;
    char *name;
    char *pet_type_code;
} pet;

typedef struct {
    char *desc;
    struct comando *next;
} command;

typedef enum {
    DO_INSERT,
    COMMAND_NOT_RECOGNIZED
} COMMAND_TO_DO;

COMMAND_TO_DO check_syntax(char *desc) {

    if(!strncmp(desc, INSERT_COMMAND, 6)) {
        return DO_INSERT;
    }

    return COMMAND_NOT_RECOGNIZED;
};

void add_command(command **fila_de_comandos, char *statement) {
    switch(check_syntax(statement)) {
        case DO_INSERT:
            printf("Voce deu um insert");
            break;
        case COMMAND_NOT_RECOGNIZED:
            printf("Comando não reconhecido!");
            break;
    }
}

int main() {

    command *fila_de_comandos = NULL;

    add_command(&fila_de_comandos, "insert into tipo_pet(codigo, desc) values(1, 'cachorro');");

    // INSERT INTO $TABLE(FIELD1,FIELD2,FIELD3) VALUES(CORRESPONDENT_FIELD1,CORRESPONDENT_FIELD2,CORRESPONDENT_FIELD3);

    return 0;
}














