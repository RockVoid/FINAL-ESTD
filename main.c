#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define INSERT_COMMAND "insert into"
/*
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

COMMAND_TO_DO check_syntax(char *statement) {

    if(!strncmp(statement, INSERT_COMMAND, 6)) {
        return DO_INSERT;
    }

    return COMMAND_NOT_RECOGNIZED;
};

char *sliceString(char *string, int start, int end) {
    int i;
    int size = (end - start) + 2;
    char *output = (char *) malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = string[start];
    }

    output[size] = '\0';

    return output;
}

char *get_table(char *statement, COMMAND_TO_DO command) {
    if(command == DO_INSERT) {
        // pet 3
        if(!strcmp(sliceString(statement, 12, 15), "pet")) {
            printf("Tabela pet");
            return;
        }

        if(!strcmp(sliceString(statement, 12, 15), "pet")) {
            printf("Tabela pet");
            return;
        }

        if(!strcmp(sliceString(statement, 12, 15), "pet")) {
            printf("Tabela pet");
            return;
        }
        // pet_type 8
        // client 6
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

int main() {

    command *fila_de_comandos = NULL;

    add_command(&fila_de_comandos, "insert into pet(codigo, desc) values(1, 'cachorro');");

    // INSERT INTO $TABLE(FIELD1,FIELD2,FIELD3) VALUES(CORRESPONDENT_FIELD1,CORRESPONDENT_FIELD2,CORRESPONDENT_FIELD3);

    return 0;
}
*/

typedef struct {
    char name[50];
    char address[100];
    int code;
    struct client* next;
} client;

client *create_client(const char *name,const char *address, int code) {
    client *new_client = malloc(sizeof(client));

    strncpy(new_client->name, name, 50);
    strncpy(new_client->address, address, 100);
    new_client->code = code;
    new_client->next = NULL;

    return new_client;
}

void add_client(client **list, const char* name, const char* address, int code) {
    client* new_client = create_client(name, address, code);
    new_client->next = *list;
    *list = new_client;
}

void serialize(client *lista, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Não foi possivel abrir o arquivo!");
        return;
    }

    client *current = lista;
    while(current) {
        fwrite(current->name, sizeof(char), 50, file);
        fwrite(current->address, sizeof(char), 100, file);
        fwrite(&(current->code), sizeof(int), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Linked list saved to %s\n", filename);
}

client* deserialize(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file!");
        return NULL;
    }

    // A cabeça da fila só será usada uma vez, quando ela for nula,
    // depois usaremos apenas o ponteiro current para ir ligando os outros valores
    client *head = NULL;
    client *current = NULL;

    while(1) {
        // Por que aqui não foi usado createClient? Porque aqui eu preciso de um "objeto" vazio
        client *newClient = malloc(sizeof(client));
        newClient->next = NULL;

        size_t nameReaded = fread(newClient->name, sizeof(char), 50, file);
        size_t addressReaded = fread(newClient->address, sizeof(char), 100, file);
        size_t codeReaded = fread(&(newClient->code), sizeof(int), 1, file);

        // Eu queria melhorar esse if :P
        if(nameReaded == 0 || addressReaded == 0 || codeReaded == 0) {
            free(newClient);
            break;
        }

        if(!head) {
            head = newClient;
            current = head;
        } else {
            current->next = newClient;
            current = current->next;
        }
    }

    fclose(file);
    return head;
}

void show_list(client* head) {
    client* aux = head;
    while (aux) {
        printf("Name: %s, Address: %s, Code: %d\n", aux->name, aux->address, aux->code);
        aux = aux->next;
    }
}

int main() {

    client *lista = NULL;
    add_client(&lista, "Roque", "Rua dos Tabajaras", 101);
    serialize(lista, "client_list.bin");
    client *lista_deserializada = deserialize("client_list.bin");
    show_list(lista_deserializada);

    return 0;
}










