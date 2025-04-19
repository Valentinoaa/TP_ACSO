#include "ej1.h"

string_proc_list* string_proc_list_create(void){
	string_proc_list* list = malloc(sizeof(string_proc_list));
	if (list == NULL) return NULL;

	list->first = NULL;
	list->last = NULL;

	return list;
}

string_proc_node* string_proc_node_create(uint8_t type, char* hash){
	string_proc_node* node = malloc(sizeof(string_proc_node));
	if (node == NULL) return NULL;

	node->next = NULL;
	node->previous = NULL;
	node->type = type;
	node->hash = hash; // NO copiar, solo apuntar

	return node;
}


void string_proc_list_add_node(string_proc_list* list, uint8_t type, char* hash){
	if (!list) return;

	string_proc_node* new_node = string_proc_node_create(type, hash);
	if (!new_node) return;

	if (list->last == NULL) {
		// Lista vacía
		list->first = new_node;
		list->last = new_node;
	} else {
		// Agrego al final
		new_node->previous = list->last;
		list->last->next = new_node;
		list->last = new_node;
	}
}

char* string_proc_list_concat(string_proc_list* list, uint8_t type, char* hash){
	if (!list || !hash) return NULL;

	size_t total_length = strlen(hash);
	string_proc_node* node = list->first;

	// Calcular longitud total
	while (node != NULL) {
		if (node->type == type && node->hash != NULL) {
			total_length += strlen(node->hash);
		}
		node = node->next;
	}

	// Concatenar
	char* result = malloc(total_length + 1);
	if (!result) return NULL;

	strcpy(result, hash);
	node = list->first;
	while (node != NULL) {
		if (node->type == type && node->hash != NULL) {
			strcat(result, node->hash);
		}
		node = node->next;
	}

	return result;
}



/** AUX FUNCTIONS **/

void string_proc_list_destroy(string_proc_list* list){

	/* borro los nodos: */
	string_proc_node* current_node	= list->first;
	string_proc_node* next_node		= NULL;
	while(current_node != NULL){
		next_node = current_node->next;
		string_proc_node_destroy(current_node);
		current_node	= next_node;
	}
	/*borro la lista:*/
	list->first = NULL;
	list->last  = NULL;
	free(list);
}
void string_proc_node_destroy(string_proc_node* node){
	node->next      = NULL;
	node->previous	= NULL;
	node->hash		= NULL;
	node->type      = 0;			
	free(node);
}


char* str_concat(char* a, char* b) {
	int len1 = strlen(a);
    int len2 = strlen(b);
	int totalLength = len1 + len2;
    char *result = (char *)malloc(totalLength + 1); 
    strcpy(result, a);
    strcat(result, b);
    return result;  
}

void string_proc_list_print(string_proc_list* list, FILE* file){
        uint32_t length = 0;
        string_proc_node* current_node  = list->first;
        while(current_node != NULL){
                length++;
                current_node = current_node->next;
        }
        fprintf( file, "List length: %d\n", length );
		current_node    = list->first;
        while(current_node != NULL){
                fprintf(file, "\tnode hash: %s | type: %d\n", current_node->hash, current_node->type);
                current_node = current_node->next;
        }
}

