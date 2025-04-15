#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "ej1.h"

#define MAX_TYPE 8
#define RUN(filename, action) pfile=fopen(filename,"a"); action; fclose(pfile);
#define NL(filename) pfile=fopen(filename,"a"); fprintf(pfile,"\n"); fclose(pfile);

char *filename_ej1 =  "salida.caso.propio.ej1.txt";
void test_ej1a(char* filename);
void test_ej1b(char* filename);

#if USE_ASM_IMPL
#define string_proc_list_create_impl string_proc_list_create_asm
#define string_proc_node_create_impl string_proc_node_create_asm
#define string_proc_list_add_node_impl string_proc_list_add_node_asm
#define string_proc_list_concat_impl string_proc_list_concat_asm
#else
#define string_proc_list_create_impl string_proc_list_create
#define string_proc_node_create_impl string_proc_node_create
#define string_proc_list_add_node_impl string_proc_list_add_node
#define string_proc_list_concat_impl string_proc_list_concat
#endif


int main() {
	srand(0);
	remove(filename_ej1);
	test_ej1a(filename_ej1);
	test_ej1b(filename_ej1);
	return 0;
}


void test_ej1a(char* filename)
{
	FILE* pfile;
	// create an
	RUN(filename, fprintf(pfile, "== Ejercicio 1a ==\n");) NL(filename)
	RUN(filename, fprintf(pfile, "Creando lista vacia\n");) NL(filename)
	string_proc_list* list = string_proc_list_create_impl();
	string_proc_list_destroy(list);
	RUN(filename, fprintf(pfile, "======================== Libera memoria =======================\n");) NL(filename)
	RUN(filename, fprintf(pfile, "Creando nodo vacio\n");) NL(filename)
	string_proc_node* node	= string_proc_node_create_impl(0, "");
	string_proc_node_destroy(node);
	RUN(filename, fprintf(pfile, "======================== Libera memoria =======================\n");) NL(filename)
	list	= string_proc_list_create_impl();
	RUN(filename, fprintf(pfile, "Creando lista vacia\n");) NL(filename)
	RUN(filename, string_proc_list_print(list, pfile);) NL(filename)
	RUN(filename, fprintf(pfile, "Agregando estrellas:\n");) NL(filename)
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sol");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "polaris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "rigel");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pollux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "deneb");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "adhara");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "betelgeuse");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sirio");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "procyon");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "altair");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "achenar");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "fomalhaut");
	RUN(filename, string_proc_list_print(list, pfile);) NL(filename)
	RUN(filename, fprintf(pfile, "======================== Libera memoria =======================\n");) NL(filename)
	string_proc_list_destroy(list);
	RUN(filename, fprintf(pfile, "Creando lista vacia\n");) NL(filename)
	list = string_proc_list_create_impl();
	RUN(filename, fprintf(pfile, "Agregando constelaciones y misiones:\n");) NL(filename)
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "geminis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pisis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cefeo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "libra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "auriga");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sagitario");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "crux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cisne");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "orion");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "centauro");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "juno");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hubble");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "irazusta");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "terra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cassini-huygens");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "artemis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "columbia");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "kepler");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "aqua");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sputnik");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "insight");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "messenger");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "osiris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "rex");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "chandra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "curiosity");

	RUN(filename, string_proc_list_print(list, pfile);) NL(filename)
	RUN(filename, fprintf(pfile, "======================== Libera memoria =======================\n");) NL(filename)
	string_proc_list_destroy(list);
	RUN(filename, fprintf(pfile, "======================== Fin del test 1a =======================");)
}	


void test_ej1b(char* filename)
{
	FILE* pfile;
	// create an
	RUN(filename, fprintf(pfile, "== Ejercicio 1b ==\n");) NL(filename)
	string_proc_list*  list = string_proc_list_create_impl();
	RUN(filename, fprintf(pfile, "Agregando constelaciones y misiones:\n");) NL(filename)
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "geminis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pisis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cefeo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "libra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "auriga");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sagitario");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "crux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sol");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "polaris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "rigel");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pollux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "deneb");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "adhara");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "betelgeuse");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sirio");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cisne");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "orion");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "centauro");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "juno");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hubble");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sol");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "polaris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "rigel");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pollux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "deneb");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "adhara");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "betelgeuse");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sirio");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "procyon");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "altair");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "achenar");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "geminis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pisis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cefeo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "libra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "auriga");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sagitario");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "crux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cisne");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cefeo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "libra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "auriga");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sagitario");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "crux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cisne");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "orion");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "centauro");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "juno");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hubble");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "geminis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pisis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cefeo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "libra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "auriga");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sagitario");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "crux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cisne");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "orion");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "centauro");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "juno");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hubble");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sol");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "polaris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "rigel");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pollux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "deneb");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "adhara");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "betelgeuse");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sirio");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cefeo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "libra");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "auriga");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "sagitario");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pavo");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "crux");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cisne");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "luna");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "ganimedes");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "titan");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "calisto");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "io");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "europa");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "luna");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "oberon");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "titania");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "rhea");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "tritón");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "encélado");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "mimas");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "titán");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "dione");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "luna");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "umbriel");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "ariel");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "miranda");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "galemede");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "apollo 11");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "soyuz");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "iss");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hubble");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "cassini-huygens");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "juno");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "artemis");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "chandrayaan-2");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "ceres");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "vesta");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "pallas");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hygiea");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "iris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "eris");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "juno");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "hebe");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "palas");
	string_proc_list_add_node_impl(list, rand() % MAX_TYPE, "victoria");


	char* new_hash_1 = string_proc_list_concat_impl(list, 0, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_1);)
	free(new_hash_1);

	char* new_hash_2 = string_proc_list_concat_impl(list, 1, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_2);)
	free(new_hash_2);

	char* new_hash_3 = string_proc_list_concat_impl(list, 2, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_3);)
	free(new_hash_3);

	char* new_hash_4 = string_proc_list_concat_impl(list, 3, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_4);)
	free(new_hash_4);

	char* new_hash_5 = string_proc_list_concat_impl(list, 4, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_5);)
	free(new_hash_5);

	char* new_hash_6 = string_proc_list_concat_impl(list, 5, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_6);)
	free(new_hash_6);
	
	char* new_hash_7 = string_proc_list_concat_impl(list, 6, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_7);)
	free(new_hash_7);

	char* new_hash_8 = string_proc_list_concat_impl(list, 7, "junta-constelaciones-mision-estrellas:");
	RUN(filename, fprintf(pfile, "%s\n",new_hash_8);)
	free(new_hash_8);
	string_proc_list_destroy(list);

	RUN(filename, fprintf(pfile, "======================== Fin del test 1b =======================");)
	}

