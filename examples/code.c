#include "CDirExplorer.h"

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    const char* directory_path = ".";
    ast_t* root = create_ast_from_directory(directory_path);
    if (!root) {
        fprintf(stderr, "Failed to create AST.\n");
        exit(EXIT_FAILURE);
    }

    printf("AST Structure:\n");
    print_ast_custom(root, 0);

    // Ejemplo de búsqueda: en este caso se busca el archivo "ast.c"
    // La ruta debe incluir el nombre del nodo raíz, por ejemplo: "./src/ast.c"
    char* search_path = strdup("./src/CDirExplorer.c");
    ast_t* found_node = search_node_by_route(search_path, root, get_node_name);
    free(search_path);

    if (found_node) {
        printf("\nFound node: %s\n", get_node_name(found_node));
    } else {
        printf("\nNode not found\n");
    }

    free_ast_t(root, free_dir_entry);
    //free_ast_recursive(root);

    puts("Exit...");
    return 0;
}
