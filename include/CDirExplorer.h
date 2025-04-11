#ifndef C_DIR_EXPLORER_H
#define C_DIR_EXPLORER_H
#include "ast.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif
// Estructura para almacenar la información de un directorio o archivo.
typedef struct {
    char* name;
} dir_entry_t;


char* concat_paths(const char* base, const char* addition);
ast_t* create_ast_from_directory(const char* directory_path);
void print_node_name(ast_t* node);

/**
 * @brief Función para obtener el nombre del nodo, utilizada en búsquedas
 * 
 * @param node ast o nodo del que obtener un string
 * @return char* texto de ese nodo
 */
static inline char* get_node_name(ast_t* node) {
    dir_entry_t* entry = (dir_entry_t*)node->data;
    return entry->name;

}
void print_ast_custom(ast_t* root, int indent);
void free_ast_recursive(ast_t* node);
void free_dir_entry(void* data);

#endif