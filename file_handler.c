#include<stdio.h>
#include "struct_data.c"

void serialize_client(client *lista, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Nao foi possivel abrir o arquivo.");
        return;
    }

    client *current = lista;

        while(current) {
            fwrite(current->name, sizeof(char), 50, file);
            fwrite(current->address, sizeof(char), 100, file);
            fwrite(&(current->code), sizeof(int), 1, file);
            fwrite(current->birth, sizeof(char), 20, file);
            fwrite(current->phone, sizeof(char), 20, file);
            current = current->next;
        }


    fclose(file);
}

void serialize_pet_type(pet_type *lista, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Não foi possivel abrir o arquivo!");
        return;
    }

    pet_type *current = lista;

        while(current) {
            fwrite(&(current->code), sizeof(int), 1, file);
            fwrite(current->desc, sizeof(char), 100, file);
            current = current->next;
        }


    fclose(file);
    printf("Linked list saved to %s\n", filename);
}

void serialize_pet(pet *lista, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("\nNao foi possivel abrir a tabela! \nCriando tabela...\n");
        return;
    }

    pet *current = lista;

        while(current) {
            fwrite(&(current->code), sizeof(int), 1, file);
            fwrite(current->pet_type_code, sizeof(char), 3, file);
            fwrite(current->name, sizeof(char), 40, file);
            fwrite(current->code_client, sizeof(char), 3, file);
            current = current->next;
        }


    fclose(file);
}

pet* deserialize_pet(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("\nTabela pet nao encontrada!\n");
        return NULL;
    }

    pet *head = NULL;
    pet *current = NULL;

    while(1) {

        pet *new_pet = malloc(sizeof(pet));
        new_pet->next = NULL;

        size_t new_pet_code = fread(&(new_pet->code), sizeof(int), 1, file);
        size_t new_pet_type_code = fread(new_pet->pet_type_code, sizeof(char), 3, file);
        size_t new_pet_name = fread(new_pet->name, sizeof(char), 40, file);
        size_t new_pet_code_client = fread(new_pet->code_client, sizeof(char), 3, file);

        if (new_pet_code != 1 || new_pet_type_code != 3 || new_pet_name != 40 || new_pet_code_client != 3) {
            free(new_pet);
            break;
        }

        if(!head) {
            head = new_pet;
            current = head;
        } else {
            current->next = new_pet;
            current = current->next;
        }
    }

    fclose(file);
    return head;
}

client* deserialize_client(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Não foi possível abrir o arquivo!\n");
        return NULL;
    }

    // A cabeça da fila só será usada uma vez, quando ela for nula,
    // depois usaremos apenas o ponteiro current para ir ligando os outros valores
    client *head = NULL;
    client *current = NULL;

    while(1) {
        // Por que aqui não foi usado createClient? Porque aqui eu preciso de um "objeto" vazio
        client *new_client = malloc(sizeof(client));
        new_client->next = NULL;

        size_t name_readed = fread(new_client->name, sizeof(char), 50, file);
        size_t address_readed = fread(new_client->address, sizeof(char), 100, file);
        size_t code_readed = fread(&(new_client->code), sizeof(int), 1, file);
        size_t birth_readed = fread(new_client->birth, sizeof(char), 20, file);
        size_t phone_readed = fread(new_client->phone, sizeof(char), 20, file);

        // Eu queria melhorar esse if :P
        if (name_readed != 50 || address_readed != 100 || code_readed != 1 || birth_readed != 20 || phone_readed != 20) {
            free(new_client);
            break;
        }
        // head->primeiro_cliente
        // current -> new_cliente
        if(!head) {
            head = new_client;
            current = head;
        } else {
            current->next = (struct client*) new_client;
            current = (client*) current->next;
        }
    }

    fclose(file);
    return head;
}

pet_type* deserialize_pet_type(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Não foi possível abrir o arquivo!");
        return NULL;
    }

    pet_type *head = NULL;
    pet_type *current = NULL;

    while(1) {

        pet_type *new_pet_type = malloc(sizeof(pet_type));
        new_pet_type->next = NULL;

        size_t new_pet_type_code = fread(&(new_pet_type->code), sizeof(int), 1, file);
        size_t new_pet_type_desc = fread(new_pet_type->desc, sizeof(char), 100, file);

        if (new_pet_type_code != 1 || new_pet_type_desc != 100) {
            free(new_pet_type);
            break;
        }

        if(!head) {
            head = new_pet_type;
            current = head;
        } else {
            current->next = new_pet_type;
            current = current->next;
        }
    }

    fclose(file);
    return head;
}