#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "file_handler.c"

#define INSERT_COMMAND "insert into"
#define MAX_WORDS 4  // Número máximo de palavras esperadas
#define MAX_WORD_LENGTH 50  // Tamanho máximo de cada palavra

char *pet_table_fields[3] = {"code_client", "name", "pet_type_code"};
char *pet_type_table_fields[2] = {"code", "desc"};
char *client_table_fields[5] = {"code", "name", "phone", "birth", "address"};

char finded_values[MAX_WORDS][MAX_WORD_LENGTH];
char table[8];

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

void check_table(const char *statement, COMMAND_TO_DO command) {
    int fields_of_table_start = 0;
    int fields_of_table_ends = 0;

    if(command == DO_INSERT) {
        fields_of_table_start = 12;
        fields_of_table_ends = 4;

        strncpy(table, statement + fields_of_table_start, fields_of_table_ends);
        if(!strcmp(table, "pet(")) {
            strcpy(table, "pet");
            return;
        }

        fields_of_table_start = 12;
        fields_of_table_ends = 8;

        strncpy(table, statement + fields_of_table_start, fields_of_table_ends);
        if(!strcmp(table, "pet_type")) {
            return;
        }

        fields_of_table_start = 12;
        fields_of_table_ends = 6;
        strncpy(table, statement + fields_of_table_start, fields_of_table_ends);
        if(!strcmp(table, "client")) {
            return;
        }
    }
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

int verify_fields(char *statement, COMMAND_TO_DO command, char *table) {
    char finded_fields[MAX_WORDS][MAX_WORD_LENGTH];
    int num_fields = 0;
    char fields[50];
    int fields_of_table_start = 11;
    int fields_of_table_ends = 37;

    strncpy(fields, statement + fields_of_table_start, fields_of_table_ends);

    switch(command) {
        case DO_INSERT:
            num_fields = extract_words(fields, table, finded_fields);
            break;
        default:
            return num_fields;
    }

    for (int i = 0; i < 3; i++) {
        if(strcmp(finded_fields[i], pet_table_fields[i]) != 0) {
            printf("Campo '%s' nao existe na tabela %s", finded_fields[i], table);
            return 0;
        }
    }
    return 1;
}

int get_values(char *statement, COMMAND_TO_DO command) {
    int num_values = 0;
    int values_of_table_start = 50;
    int values_of_table_end = 55;
    char result[50];

    if(command == DO_INSERT) {
        strncpy(result, statement + values_of_table_start, values_of_table_end);
        num_values = extract_words(result, "values", finded_values);

        if(!num_values) return 0;
    }

    return num_values;
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
    new_client->next = *list;
    *list = new_client;
}

// Function to count how many times the program is executed
int count_executions() {
    int count = 0;

    // Open the binary file for reading and writing
    FILE *file = fopen("execution_count.bin", "rb+");

    // If the file doesn't exist, create it and initialize the count to 0
    if (file == NULL) {
        file = fopen("execution_count.bin", "wb");
        if (file == NULL) {
            perror("Failed to create file");
            return -1;
        }
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
        file = fopen("execution_count.bin", "rb+");
        if (file == NULL) {
            perror("Failed to open file");
            return -1;
        }
    }

    // Read the current count from the file
    fread(&count, sizeof(int), 1, file);

    // Increment the count
    count++;

    // Move the file pointer to the beginning of the file
    rewind(file);

    // Write the updated count back to the file
    fwrite(&count, sizeof(int), 1, file);

    // Close the file
    fclose(file);

    return count;
}

pet* create_pet() {
    pet *new_pet = malloc(sizeof(pet));

    int code_of_pet = count_executions();
    // Essa IDE reclama mais que minha namorada :P
    char code = code_of_pet + '0';

    strcpy(new_pet->code_client, finded_values[0]);
    strcpy(new_pet->code, &code);
    strcpy(new_pet->name, finded_values[1]);
    strcpy(new_pet->pet_type_code, finded_values[2]);

    new_pet->next = NULL;

    return new_pet;
}

void add_pet(pet **pet_list) {
    pet *new_pet = create_pet();
    new_pet->next = *pet_list;
    *pet_list = new_pet;
}

void show_list(pet* head) {
    if (!head) {
        printf("A lista esta vazia!\n");
        return;
    }
    pet* aux = head;
    while (aux) {
        printf("\nName: %s\nCode_pet: %s\nDono: %s", aux->name, aux->pet_type_code, aux->code_client);
        //printf("Name: %s\nAddress: %s\nCode: %d\nBirth: %s\nPhone: %s", aux->name, aux->address, aux->code, aux->birth, aux->phone);
        aux = aux->next;
    }
}

void do_insert() {

    if(!strcmp(table, "pet")) {
        printf("INSERT EM PET");
        pet *list = deserialize_pet("pet_list.bin");
        add_pet(&list);
        serialize_pet(list, "pet_list.bin");
    }
}

void add_command(char *statement) {

    switch(check_syntax(statement)) {
        case DO_INSERT:
            check_table(statement, DO_INSERT);
            if(!table) {
                printf("Tabela nao reconhecida");
                free(table);
                return;
            };
            verify_fields(statement, DO_INSERT, table);
            get_values(statement, DO_INSERT); // Get values and set operation
            do_insert();
            break;
        case COMMAND_NOT_RECOGNIZED:
            printf("Comando nao reconhecido!");
        break;
    }
}

int main() {

    printf("TEST DESERIALIZE PET_LIST: \n");
    add_command("insert into pet(code_client, name, pet_type_code) values(1, 'Roque', 7)");
    add_command("insert into pet(code_client, name, pet_type_code) values(2, 'Pedro', 4)");

    pet *lista_deserializada = deserialize_pet("pet_list.bin");
    show_list(lista_deserializada);

    free(lista_deserializada);
    return 0;
}