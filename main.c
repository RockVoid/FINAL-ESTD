#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "file_handler.c"

#define INSERT_COMMAND "insert into"

COMMAND_TO_DO check_syntax(char *statement) {

    if(!strncmp(statement, INSERT_COMMAND, 6)) {
        return DO_INSERT;
    }

    return COMMAND_NOT_RECOGNIZED;
};

char* sliceString(const char* str, int start, int end) {
    // Calculate the length of the substring
    int length = end - start;

    // Allocate memory for the substring (+1 for the null terminator)
    char* slicedStr = (char*)malloc((length + 1) * sizeof(char));
    if (slicedStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Copy the substring from the original string
    strncpy(slicedStr, str + start, length);

    // Add the null terminator
    slicedStr[length] = '\0';

    return slicedStr;
}

void get_table(char *statement, COMMAND_TO_DO command) {
    if(command == DO_INSERT) {
        if(!strcmp(sliceString(statement, 12, 16), "pet(")) {
            printf("Tabela pet\n");
            return;
        }

        if(!strcmp(sliceString(statement, 12, 20), "pet_type")) {
            printf("Tabela pet_type\n");
            return;
        }

        if(!strcmp(sliceString(statement, 12, 18), "client")) {
            printf("Tabela client\n");
            return;
        }
    }
}

void add_command(command **fila_de_comandos, char *statement) {
    switch(check_syntax(statement)) {
        case DO_INSERT:
            get_table(statement, DO_INSERT);
        break;
        case COMMAND_NOT_RECOGNIZED:
            printf("Comando não reconhecido!");
        break;
    }
}

client *create_client(const char *name,const char *address, const int code, const char* phone, const char* birth) {
    client *new_client = malloc(sizeof(client));

    strncpy(new_client->name, name, 50);
    strncpy(new_client->address, address, 100);
    strncpy(new_client->phone, phone, 20);
    strncpy(new_client->birth, birth, 20);

    new_client->code = code;
    new_client->next = NULL;

    return new_client;
}

void add_client(client **list, const char* name, const char* address, const char* phone, const char* birth, const int code) {
    client* new_client = create_client(name, address, code, phone, birth);
    new_client->next = (struct client*) *list;
    *list = new_client;
}

void show_list(client* head) {
    if (!head) {
        printf("A lista esta vazia!\n");
        return;
    }
    client* aux = head;
    while (aux) {
        printf("Name: %s\nAddress: %s\nCode: %d\nBirth: %s\nPhone: %s", aux->name, aux->address, aux->code, aux->birth, aux->phone);
        aux = (client *) aux->next;
    }
}

int main() {

    client *lista = NULL;

    client *lista_deserializada = deserialize("client_list.bin");
    show_list(lista_deserializada);

    free(lista_deserializada);
    return 0;
}