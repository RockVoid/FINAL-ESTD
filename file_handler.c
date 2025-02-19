#include<stdio.h>
#include "struct_data.c"

void serialize_client(client *lista, const char* filename) {
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
            fwrite(current->birth, sizeof(char), 20, file);
            fwrite(current->phone, sizeof(char), 20, file);
            current = current->next;
        }


    fclose(file);
    printf("Linked list saved to %s\n", filename);
}

void serialize_pet_type(pet_type *lista, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Não foi possivel abrir o arquivo!");
        return;
    }

    pet_type *current = lista;

        while(current) {
            fwrite(current->desc, sizeof(char), 100, file);
            fwrite(&(current->code), sizeof(int), 1, file);
            current = current->next;
        }


    fclose(file);
    printf("Linked list saved to %s\n", filename);
}

void serialize_pet(pet *lista, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Não foi possivel abrir o arquivo!");
        return;
    }

    pet *current = lista;

        while(current) {
            fwrite(current->name, sizeof(char), 40, file);
            fwrite(&(current->code), sizeof(int), 1, file);
            fwrite(&(current->code_client), sizeof(int), 1, file);
            fwrite(current->pet_type_code, sizeof(char*), 1, file);
            current = current->next;
        }


    fclose(file);
    printf("Linked list saved to %s\n", filename);
}

client* deserialize_client(const char* filename) {
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
        size_t birthReaded = fread(newClient->birth, sizeof(char), 20, file);
        size_t phoneReaded = fread(newClient->phone, sizeof(char), 20, file);

        // Eu queria melhorar esse if :P
        if (nameReaded != 50 || addressReaded != 100 || codeReaded != 1 || birthReaded != 20 || phoneReaded != 20) {
            free(newClient);
            break;
        }
// head->primeiro_cliente
// current -> new_cliente
        if(!head) {
            head = newClient;
            current = head;
        } else {
            current->next = (struct client*) newClient;
            current = (client*) current->next;
        }
    }

    fclose(file);
    return head;
}