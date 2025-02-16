#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "file_handler.c"

#define INSERT_COMMAND "insert into"
#define MAX_WORDS 10  // Número máximo de palavras esperadas
#define MAX_WORD_LENGTH 50  // Tamanho máximo de cada palavra

char *pet_table_fields[3] = {"code_client", "name", "pet_type_code"};
char *pet_type_table_fields[3] = {"", "", ""};
char *client_table_fields[3] = {"", "", ""};

COMMAND_TO_DO check_syntax(char *statement) {

    if(!strncmp(statement, INSERT_COMMAND, 6)) {
        return DO_INSERT;
    }

    return COMMAND_NOT_RECOGNIZED;
};

char* sliceString(const char* str, int start, int end) {
    int length = end - start;

    char* slicedStr = malloc((length + 1) * sizeof(char));
    if (slicedStr == NULL) {
        fprintf(stderr, "Não foi possivel alocar memoria.\n");
        return NULL;
    }

    strncpy(slicedStr, str + start, length);

    slicedStr[length] = '\0';

    return slicedStr;
}

char *check_table(const char *statement, COMMAND_TO_DO command) {
    char *table = NULL;
    if(command == DO_INSERT) {
        table = sliceString(statement, 12, 16);
        if(!strcmp(table, "pet(")) {
            free(table);
            strcpy(table, "pet");
            return table;
        }
        free(table);

        table = sliceString(statement, 12, 20);
        if(!strcmp(table, "pet_type")) {
            return table;
        }
        free(table);

        table = sliceString(statement, 12, 18);
        if(!strcmp(table, "client")) {
            return table;
        }
    }
    free(table);
    return NULL;
}

int extract_words(const char *quote, const char *keyword, char words[MAX_WORDS][MAX_WORD_LENGTH]) {
    int i = 0, j = 0, k = 0;
    int is_inside_parenthesis = 0;

    const char *parenthesis_pos = strstr(quote, "(");

    if (!parenthesis_pos) {
        return 0;
    }

    const char *keyword_pos = strstr(quote, keyword);
    if (!keyword_pos || keyword_pos >= parenthesis_pos) {
        return 0;
    }

    while (quote[i] != '\0') {
        if (quote[i] == '(') {
            is_inside_parenthesis = 1;
            i++;
        } else if (quote[i] == ')') {
            is_inside_parenthesis = 0;
            i++;
        } else if (is_inside_parenthesis) {
            if (quote[i] == ',') {
                words[k][j] = '\0';  // Finaliza a palavra atual
                k++;  // Próxima palavra
                j = 0;  // Reinicia o índice da palavra
                i++;
            } else if (quote[i] != ' ') {  // Ignora espaços
                words[k][j] = quote[i];  // Adiciona caractere à palavra
                j++;
                i++;
            } else {
                i++;  // Ignora espaços
            }
        } else {
            i++;  // Avança na frase
        }
    }

    words[k][j] = '\0';  // Finaliza a última palavra
    return k + 1;  // Num palavras
}

int get_fields(char *statement, COMMAND_TO_DO command, char *table) {
    char finded_fields[MAX_WORDS][MAX_WORD_LENGTH];
    int num_fields = extract_words(statement, table, finded_fields);

    for (int i = 0; i < num_fields; i++) {
        if(strcmp(finded_fields[i], pet_table_fields[i]) != 0) {
            printf("Campo '%s' nao existe na tabela %s", finded_fields[i], table);
            return 0;
        }
    }
    return 1;
}



void add_command(command **fila_de_comandos, char *statement) {

    char *table = NULL;

    switch(check_syntax(statement)) {
        case DO_INSERT:
            table = check_table(statement, DO_INSERT);
            if(!table) {
                printf("Tabela nao reconhecida");
                free(table);
                return;
            };
            get_fields(statement, DO_INSERT, table);
            free(table);
        break;
        case COMMAND_NOT_RECOGNIZED:
            printf("Comando nao reconhecido!");
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

    command *lista = NULL;
    printf("Testando comando insert: \n");
    add_command(&lista, "insert into pet(code_client, jhkhkj, pet_type_code)");

//    client *lista_deserializada = deserialize("client_list.bin");
//    show_list(lista_deserializada);

//    free(lista_deserializada);
    return 0;
}