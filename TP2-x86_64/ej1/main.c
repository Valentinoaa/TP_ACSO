#include "ej1.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

/**
*	crea y destruye a una lista vacía
*/
void test_create_destroy_list(){
	string_proc_list * list	= string_proc_list_create_asm();
	string_proc_list_destroy(list);
}

/**
*	crea y destruye un nodo
*/
void test_create_destroy_node(){
	string_proc_node* node	= string_proc_node_create_asm(0, "hash");
	string_proc_node_destroy(node);
}

/**
 * 	crea una lista y le agrega nodos
*/
void test_create_list_add_nodes()
{	
	string_proc_list * list	= string_proc_list_create_asm();
	string_proc_list_add_node_asm(list, 0, "hola");
	string_proc_list_add_node_asm(list, 0, "a");
	string_proc_list_add_node_asm(list, 0, "todos!");
	string_proc_list_destroy(list);
}

/**
 * 	crea una lista y le agrega nodos. Luego aplica la lista a un hash.
*/

void test_list_concat()
{
	string_proc_list * list	= string_proc_list_create();
	string_proc_list_add_node(list, 0, "hola");
	string_proc_list_add_node(list, 0, "a");
	string_proc_list_add_node(list, 0, "todos!");	
	char* new_hash = string_proc_list_concat(list, 0, "hash");
	string_proc_list_destroy(list);
	free(new_hash);
}

/**
* Corre los test a se escritos por lxs alumnxs	
*/
void run_tests(){

	/* Aqui pueden comenzar a probar su codigo */
	test_create_destroy_list();

	test_create_destroy_node();

	test_create_list_add_nodes();

	test_list_concat();
}

int main (void){
	run_tests();
	return 0;    
}

