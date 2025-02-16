#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "file_handler.c"

#define INSERT_COMMAND "insert into"
#define MAX_WORDS 10  // Número máximo de palavras esperadas
#define MAX_WORD_LENGTH 50  // Tamanho máximo de cada palavra

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

// Função para extrair palavras dentro de parênteses
int extract_words(const char *quote, char words[MAX_WORDS][MAX_WORD_LENGTH]) {
    int i = 0, j = 0, k = 0;
    int is_inside_parenthesis = 0;  // Flag para indicar se estamos dentro dos parênteses

    // Percorre a frase
    while (quote[i] != '\0') {
        if (quote[i] == '(') {
            is_inside_parenthesis = 1;  // Entrou nos parênteses
            i++;
        } else if (quote[i] == ')') {
            is_inside_parenthesis = 0;  // Saiu dos parênteses
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
    return k + 1;  // Retorna o número de palavras encontradas
}

void getFields(char *statement, COMMAND_TO_DO command, char *table) {
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int num_words = extract_words(statement, words);
    printf("Palavras encontradas:\n");
    for (int i = 0; i < num_words; i++) {
        printf("%s\n", words[i]);
    }

    if(command == DO_INSERT && !strcmp(table, "pet")) {
    }

    if(command == DO_INSERT && !strcmp(table, "pet_type")) {
        printf("Voce ta querendo inserir na tabela pet_type!");
    }

    if(command == DO_INSERT && !strcmp(table, "client")) {
        printf("Voce ta querendo inserir na tabela client!");
    }
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
            getFields(statement, DO_INSERT, table);
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
    add_command(&lista, "insert into pet(code_client, name, pet_type_code) values(1, 'Mario', 1)");

//    client *lista_deserializada = deserialize("client_list.bin");
//    show_list(lista_deserializada);

//    free(lista_deserializada);
    return 0;
}