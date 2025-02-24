#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "file_handler.c"

#define INSERT_COMMAND "insert into"
#define MAX_WORDS 4  // Número máximo de palavras esperadas
#define MAX_WORD_LENGTH 50  // Tamanho máximo de cada palavra

char *pet_table_fields[3] = {"code_client", "name", "pet_type_code"};
char *pet_type_table_fields[2] = {"desc"};
char *client_table_fields[4] = {"name", "phone", "birth", "address"};

char finded_values[MAX_WORDS][MAX_WORD_LENGTH];
char table[9];

COMMAND_TO_DO check_syntax(char *statement) {

    char *select_all_from_pet_type = "select * from pet_type;";
    char *select_all_from_pet = "select * from pet;";
    char *select_all_from_client = "select * from client;";

    char *delete_all_from_pet = "delete * from pet;";
    char *delete_all_from_client = "delete * from client;";
    char *delete_all_from_pet_type = "delete * from pet_type;";

    if(!strncmp(statement, INSERT_COMMAND, 6)) {
        return DO_INSERT;
    }

    if(!strcmp(statement, select_all_from_pet)) {
        return SELECT_ALL_FROM_PET;
    }

    if(!strcmp(statement, select_all_from_pet_type)) {
        return SELECT_ALL_FROM_PET_TYPE;
    }

    if(!strcmp(statement, select_all_from_client)) {
        return SELECT_ALL_FROM_CLIENT;
    }

    if(!strcmp(statement, delete_all_from_pet)) {
        return DELETE_ALL_FROM_PET;
    }

    if(!strcmp(statement, delete_all_from_client)) {
        return DELETE_ALL_FROM_CLIENT;
    }

    if(!strcmp(statement, delete_all_from_pet_type)) {
        return DELETE_ALL_FROM_PET_TYPE;
    }

    return COMMAND_NOT_RECOGNIZED;
};

void clear_characters_of_table() {
    for(int i = 0; i < sizeof(table); i++) {
        table[i] = '\0';
    }
};

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

        fields_of_table_ends = 8;

        strncpy(table, statement + fields_of_table_start, fields_of_table_ends);
        if(!strcmp(table, "pet_type")) {
            return;
        }

       // Gambs
        clear_characters_of_table();
        strncpy(table, (statement + fields_of_table_start), 6);
        if(!strcmp(table, "client")) {
            return;
        } else {
            clear_characters_of_table();
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

int verify_fields(char *statement, COMMAND_TO_DO command) {
    char finded_fields[MAX_WORDS][MAX_WORD_LENGTH];
    int num_fields = 0;
    char fields[50];
    int fields_of_table_start = 0;
    int fields_of_table_ends = 0;


    if(!strcmp(table, "pet")) {
        fields_of_table_start = 11;
        fields_of_table_ends = 37;

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
                printf("Campo '%s' nao existe na tabela %s\n", finded_fields[i], table);
                return 0;
            }
        }
    }

    if(!strcmp(table, "client")) {
        fields_of_table_start = 12;
        fields_of_table_ends = 35;

        strncpy(fields, statement + fields_of_table_start, fields_of_table_ends);

        switch(command) {
            case DO_INSERT:
                num_fields = extract_words(fields, table, finded_fields);
            break;
            default:
                return num_fields;
        }

        for (int i = 0; i < 4; i++) {
            if(strcmp(finded_fields[i], client_table_fields[i]) != 0) {
                printf("Campo '%s' nao existe na tabela %s", finded_fields[i], table);
                return 0;
            }
        }

        return 1;
    }

    if(!strcmp(table, "pet_type")) {
        fields_of_table_start = 11;
        fields_of_table_ends = 22;

        strncpy(fields, statement + fields_of_table_start, fields_of_table_ends);

        switch(command) {
            case DO_INSERT:
                num_fields = extract_words(fields, table, finded_fields);
            break;
            default:
                return num_fields;
        }

        // Por que um loop que só executa uma vez?
        // Para manter um padrão e manter o principio Open-Closed caso a tabela venha a ter mais campos no futuro
        for (int i = 0; i < 1; i++) {
            if(strcmp(finded_fields[i], pet_type_table_fields[i]) != 0) {
                printf("Campo '%s' nao existe na tabela %s", finded_fields[i], table);
                return 0;
            }
        }
    }

    return 1;
}

int get_values(char *statement, COMMAND_TO_DO command) {
    int num_values = 0;
    int values_of_table_start = 0;
    int values_of_table_end = 0;
    char result[100];

    if(!strcmp(table, "pet")) {
        values_of_table_start = 50;
        values_of_table_end = 55;
    }

    if(!strcmp(table, "client")) {
        values_of_table_start = 48;
        values_of_table_end = 80;
    }

    if(!strcmp(table, "pet_type")) {
        values_of_table_start = 27;
        values_of_table_end = 40;
    }

    if(command == DO_INSERT) {
        strncpy(result, statement + values_of_table_start, values_of_table_end);
        num_values = extract_words(result, "values", finded_values);

        if(!num_values) return 0;
    }

    return num_values;
}

int count_ids_of_table(const char* bin_filename) {
    int count = 0;

    // Open the binary file for reading and writing
    FILE *file = fopen(bin_filename, "rb+");

    // If the file doesn't exist, create it and initialize the count to 0
    if (file == NULL) {
        file = fopen(bin_filename, "wb");
        if (file == NULL) {
            printf("Arquivo nulo!");
            return -1;
        }
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
        file = fopen(bin_filename, "rb+");
        if (file == NULL) {
            printf("Arquivo nulo!");
            return -1;
        }
    }

    fread(&count, sizeof(int), 1, file);
    count++;
    // Move de volta para o inicio do arquivo
    rewind(file);
    // escrevendo o novo valor no count
    fwrite(&count, sizeof(int), 1, file);
    fclose(file);

    return count;
}

client *create_client() {
    client *new_client = malloc(sizeof(client));

    // Essa IDE reclama mais que minhas kenga :P
    int code_of_client = count_ids_of_table("client_table_count.bin");

    strcpy(new_client->name, finded_values[0]);
    strcpy(new_client->phone, finded_values[1]);
    strcpy(new_client->birth, finded_values[2]);
    strcpy(new_client->address,  finded_values[3]);
    new_client->code = code_of_client;

    new_client->next = NULL;

    return new_client;
}

void show_client_list(client* head) {
    if (!head) {
        printf("A lista esta vazia!\n");
        return;
    }
    client* aux = head;
    while (aux) {
        printf("\n========================================================\n");
        printf("\nCode: %d\nName: %s\nAddress: %s\nPhone: %s\nBirth: %s",
            aux->code, aux->name, aux->address, aux->phone, aux->birth);
        //printf("Name: %s\nAddress: %s\nCode: %d\nBirth: %s\nPhone: %s", aux->name, aux->address, aux->code, aux->birth, aux->phone);
        aux = aux->next;
    }
}

void add_client(client **list) {
    client* new_client = create_client();
    new_client->next = *list;
    *list = new_client;
}

pet* create_pet() {
    pet *new_pet = malloc(sizeof(pet));

    int code_of_pet = count_ids_of_table("pet_table_count.bin");

    strcpy(new_pet->code_client, finded_values[0]);
    new_pet->code = code_of_pet;
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

void show_pet_list(pet* head) {
    if (!head) {
        printf("A lista esta vazia!\n");
        return;
    }
    pet* aux = head;
    while (aux) {
        printf("\n========================================================\n");
        printf("\nCode: %d\nName: %s\nCode_pet: %s\nDono: %s\n", aux->code, aux->name, aux->pet_type_code, aux->code_client);
        aux = aux->next;
    }
}

void show_pet_type_list(pet_type* head) {
    if (!head) {
        printf("A lista esta vazia!\n");
        return;
    }
    pet_type* aux = head;
    while (aux) {
        printf("\n========================================================\n");
        printf("\nCode: %d\nDesc: %s\n", aux->code, aux->desc);
        aux = aux->next;
    }
}

pet_type* create_pet_type() {
    pet_type *new_pet_type = malloc(sizeof(pet_type));

    int code_of_pet_type = count_ids_of_table("pet_type_table_count.bin");

    strcpy(new_pet_type->desc, finded_values[0]);
    new_pet_type->code = code_of_pet_type;

    new_pet_type->next = NULL;

    return new_pet_type;
}

void add_pet_type(pet_type** head) {
    pet_type *new_pet_type = create_pet_type();
    new_pet_type->next = *head;
    *head = new_pet_type;
}

void do_insert() {

    if(!strcmp(table, "pet")) {
        printf("INSERT EM PET\n");
        pet *list = deserialize_pet("pet_table.bin");
        add_pet(&list);
        serialize_pet(list, "pet_table.bin");
    }

    if(!strcmp(table, "client")) {
        printf("INSERT EM CLIENT\n");
        client *list = deserialize_client("client_table.bin");
        add_client(&list);
        serialize_client(list, "client_table.bin");
    }

    if(!strcmp(table, "pet_type")) {
        printf("\nINSERT EM PET TYPE\n");
         pet_type *list = deserialize_pet_type("pet_type_table.bin");
         add_pet_type(&list);
         serialize_pet_type(list, "pet_type_table.bin");
    }
}

/*
pet *buscar(pet *lista, int valor)
{
    pet *aux = lista;
    while (aux && aux->code != valor)
        aux = aux->next;
    return aux;
}
*/

int delete_pet(pet **pet_list, int code) {
    if(!(*pet_list)) {
        printf("Lista vazia ao tentar remover pet com codigo: %d\n", code);
        return 0;
    }
    pet *aux = *pet_list;
    pet *ant = NULL;

    if((*pet_list)->code == code){
        *pet_list = (*pet_list)->next;
        free(aux);
        return 1;
    }
    while(aux && aux->code != code){
        ant = aux;
        aux = aux->next;
    }

    if(!aux) {
        printf("Pet com o codigo %d nao encontrado!", code);
        return 0;
    }

    if(aux->code == code) {
        ant->next = aux->next;
        free(aux);
    }

    return 1;
}

void  delete_all_from_pet(pet **pet_list) {
    if(!(*pet_list)) {
        printf("\nNada para remover da tabela!\n");
        return;
    }

    pet *aux = *pet_list;
    while(aux) {
        *pet_list = (*pet_list)->next;
        free(aux);
        aux = *pet_list;
    }
}

void  delete_all_from_client(client **client_list) {
    if(!(*client_list)) {
        printf("\nNada para remover da tabela!\n");
        return;
    }

    client *aux = *client_list;
    while(aux) {
        *client_list = (*client_list)->next;
        free(aux);
        aux = *client_list;
    }
}

void add_command(char *statement) {

    switch(check_syntax(statement)) {
        case DO_INSERT:
            check_table(statement, DO_INSERT);

            if(table[0] == 0) {
                printf("Tabela nao reconhecida!\n");
                return;
            };

            verify_fields(statement, DO_INSERT);
            get_values(statement, DO_INSERT);
            do_insert();
            break;

        case SELECT_ALL_FROM_PET:
            pet *pet_list = NULL;
            pet_list = deserialize_pet("pet_table.bin");
            show_pet_list(pet_list);
            break;

        case SELECT_ALL_FROM_PET_TYPE:
            pet_type *pet_type_list = NULL;
            pet_type_list = deserialize_pet_type("pet_type_table.bin");
            show_pet_type_list(pet_type_list);
            break;

        case SELECT_ALL_FROM_CLIENT:
            client *client_list = NULL;
            client_list = deserialize_client("client_table.bin");
            show_client_list(client_list);
            break;

        case DELETE_ALL_FROM_PET:
            pet *pet_list_delete = NULL;
            pet_list_delete = deserialize_pet("pet_table.bin");
            delete_all_from_pet(&pet_list_delete);
            serialize_pet(pet_list_delete, "pet_table.bin");

        case DELETE_ALL_FROM_CLIENT:
            client *client_list_delete = NULL;
            client_list_delete = deserialize_client("client_table.bin");
            delete_all_from_client(&client_list_delete);
            serialize_client(client_list_delete, "client_table.bin");
        default: ;
    }
}

int main() {
    // TESTANDO INSERT CLIENT TABLE
    /*
        add_command("insert into client(name, phone, birth, address) values(Roque, 99-9999-9999, 12-08-2001, 'Rua Dos Tabajaras')");
        client *list = NULL;
        list = deserialize_client("client_table.bin");
        show_client_list(list);


        printf("\nTeste pet\n");


        add_command("insert into pet(code_client, name, pet_type_code) values(2, Jack, 3)");
        pet *pet_list = NULL;
        pet_list = deserialize_pet("pet_table.bin");
        show_pet_list(pet_list);
    */

    // TESTANDO INSERT EM CLIENT
    /*
        add_command("insert into pet_type(desc) values(bulldog);");
        pet_type *pet_type_list = NULL;
        pet_type_list = deserialize_pet_type("pet_type_table.bin");
        show_pet_type_list(pet_type_list);
    */
/*
 *
 *  THE SELECT * TEST
    add_command("select * from client;");
    add_command("select * from pet;");
    add_command("select * from pet_type;");
*/

/*
    // TESTANDO DELETE EM PET
    add_command("insert into pet(code_client, name, pet_type_code) values(2, Jack, 3)");
    add_command("insert into pet(code_client, name, pet_type_code) values(2, Jack, 3)");
    add_command("insert into pet(code_client, name, pet_type_code) values(2, Jack, 3)");
    add_command("delete from pet where code = 2;");
    add_command("select * from pet;");
*/


    return 0;
}