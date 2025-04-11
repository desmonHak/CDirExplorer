#include "CDirExplorer.h"

int main(int argc, char* argv[]) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    // Si se pasa un argumento, usarlo como ruta del directorio; de lo contrario, usar el directorio actual
    const char* directory_path = (argc > 1) ? argv[1] : ".";

    // Crear el AST a partir del directorio especificado
    ast_t* root = create_ast_from_directory(directory_path);
    if (!root) {
        fprintf(stderr, "Failed to create AST for directory: %s\n", directory_path);
        exit(EXIT_FAILURE);
    }

    // Imprimir la estructura del árbol AST
    printf("AST Structure for directory: %s\n", directory_path);
    print_ast_custom(root, 0);

    // Construir la ruta de búsqueda correctamente
    char search_path[1024];
    snprintf(search_path, sizeof(search_path), "%s/src/CDirExplorer.c", directory_path);

    // Buscar el nodo correspondiente al archivo "CDirExplorer.c"
    ast_t* found_node = search_node_by_route(search_path, root, get_node_name);

    if (found_node) {
        printf("\nFound node: %s\n", get_node_name(found_node));
    } else {
        printf("\nNode not found %s\n", search_path);
    }

    // Liberar la memoria del árbol AST
    free_ast_t(root, free_dir_entry);

    puts("Exit...");
    return 0;
}
