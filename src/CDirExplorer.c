#ifndef C_DIR_EXPLORER_C
#define C_DIR_EXPLORER_C
#include "CDirExplorer.h"

// Concatenación segura de rutas.
char* concat_paths(const char* base, const char* addition) {
    size_t base_len = strlen(base);
    int needs_separator = (base_len > 0 && base[base_len - 1] != PATH_SEPARATOR[0]);
    char* full_path = malloc(base_len + strlen(addition) + (needs_separator ? strlen(PATH_SEPARATOR) : 0) + 1);
    if (!full_path) {
        perror("Memory allocation failure");
        exit(EXIT_FAILURE);
    }
    strcpy(full_path, base);
    if (needs_separator) {
        strcat(full_path, PATH_SEPARATOR);
    }
    strcat(full_path, addition);
    return full_path;
}

// Función para crear un nodo AST a partir de un directorio.
// Para directorios se crea el nodo correspondiente mediante una llamada recursiva,
// y para archivos se crea un nodo hoja.
ast_t* create_ast_from_directory(const char* directory_path) {
    DIR* dir = opendir(directory_path);
    if (!dir) {
        perror("Directory opening failure");
        return NULL;
    }

    // Extraer el nombre del directorio desde la ruta
    const char* dir_name = strrchr(directory_path, PATH_SEPARATOR[0]);
    if (dir_name) {
        dir_name++;
    } else {
        dir_name = directory_path;
    }

    // Crear el nodo raíz para este directorio
    dir_entry_t* root_entry = malloc(sizeof(dir_entry_t));
    if (!root_entry) {
        perror("Memory allocation failure");
        closedir(dir);
        return NULL;
    }
    root_entry->name = strdup(dir_name);
    ast_t* root = create_ast_t(root_entry);
    if (!root) {
        closedir(dir);
        free(root_entry->name);
        free(root_entry);
        return NULL;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Omitir las entradas especiales
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char* full_path = concat_paths(directory_path, entry->d_name);
        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1) {
            fprintf(stderr, "File status error for %s: %s\n", full_path, strerror(errno));
            free(full_path);
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            // Si es un directorio, se crea el nodo recursivamente y se añade directamente
            ast_t* child_dir_node = create_ast_from_directory(full_path);
            if (child_dir_node) {
                push_back_a(root->ramas, child_dir_node);
            }
        } else {
            // Si es un archivo, se crea un nodo hoja
            dir_entry_t* file_entry = malloc(sizeof(dir_entry_t));
            if (!file_entry) {
                perror("Memory allocation failure");
                free(full_path);
                closedir(dir);
                return root; // Retornamos el árbol parcial
            }
            file_entry->name = strdup(entry->d_name);
            ast_t* file_node = create_ast_t(file_entry);
            push_back_a(root->ramas, file_node);
        }
        free(full_path);
    }
    closedir(dir);
    return root;
}

// Función de impresión de un nodo (se espera que node->data sea un pointer a dir_entry_t)
void print_node_name(ast_t* node) {
    dir_entry_t* entry = (dir_entry_t*)node->data;
    printf("%s\n", entry->name);
}



// Función para liberar la estructura dir_entry_t
void free_dir_entry(void* data) {
    dir_entry_t* entry = (dir_entry_t*)data;
    if (entry) {
        free(entry->name);
        free(entry);
    }
}

// Función personalizada para imprimir recursivamente el árbol AST con indentación
void print_ast_custom(ast_t* root, int indent) {
    if (!root)
        return;

    dir_entry_t* entry = (dir_entry_t*)root->data;
    for (int i = 0; i < indent; i++) {
        printf("  "); // Dos espacios para la indentación
    }
    printf("%s\n", entry->name);

    for (size_t i = 0; i < size_a(root->ramas); i++) {
        ast_t* child = (ast_t*)get_element_a(root->ramas, i);
        print_ast_custom(child, indent + 1);
    }
}

void free_ast_recursive(ast_t* node) {
    if (node == NULL) return;

    for (size_t i = 0; i < size_a(node->ramas); i++) {
        ast_t* child = (ast_t*)get_element_a(node->ramas, i);
        free_ast_recursive(child);
    }
    free_dir_entry(node->data);
    free(node);
}

#endif