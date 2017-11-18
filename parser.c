/*GRAMÁTICA
 * 
 * json			--------------------> element eof
 * element		--------------------> object | array
 * array		--------------------> [a
 * a			--------------------> element-list] | ]
 * element-list 	--------------------> element e
 * e			--------------------> ,element e | ε
 * object		--------------------> {o
 * o			--------------------> attributes-list} | }
 * attributes-list	--------------------> attribute at
 * at			--------------------> ,attribute at | ε
 * attribute		--------------------> attribute-name : attribute-value
 * attribute-name	--------------------> string
 * attribute-value	--------------------> element | string | number | true | false | null
 * 
 * */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sintactico.h"
#include "anlex.c"


void reconocer(int token_esperado){

	if(t.compLex == token_esperado){
			sigLex();
	}
}

void error_sintactico(){
	printf("Lin %d: Error Sintactico. No se esperaba %s.\n",numLinea, t.pe->lexema);
}

void scanto(int synch_set[]){

	int i = 0;

	while (t.compLex != synch_set[i] && t.compLex != EOF){
		if(synch_set[i] == '\0'){
			sigLex();
			i = 0;
		}
		i += 1;
	}
	sigLex();
}

void check_input(int conjunto_primero[], int synch_set[]){

	int ban = 0, i = 0, sincronizacion[TAM_CONJUNTOS];

	while(ban != 1){
		if(t.compLex == conjunto_primero[i]){
			ban = 1;
		}
		i += 1;
	}
	i = 0;
	int w = 0;
	/*Si el token no está en el conjunto primero*/
	if(ban == 0){
		/*union de los conjuntos primero y siguiente*/
		while(conjunto_primero[i] != '\0'){
			sincronizacion[w] = conjunto_primero[i];
			w += 1;
			i += 1;
		}
		i = 0;
		while(synch_set[i] != '\0'){
			sincronizacion[w] = conjunto_primero[i];
			w += 1;
			i += 1;
		}

		error_sintactico();
		error_de_sintaxis = 1;
		scanto(sincronizacion);
	}
}

void json(){

	/*int conjunto_primero[] = {L_CORCHETE, L_LLAVE};*/
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	element(conjunto_siguiente);
}

void element(int synch_set[]){

	int conjunto_primero[] = {L_CORCHETE, L_LLAVE, '\0'};
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == L_CORCHETE){
			array(conjunto_siguiente);
	}
	else if(t.compLex == L_LLAVE){
			object(conjunto_siguiente);
	}
	check_input(conjunto_siguiente, conjunto_primero);
}

void array(int synch_set[]){

	int conjunto_primero[] = {L_CORCHETE, '\0'};
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == L_CORCHETE){
		reconocer(L_CORCHETE);
		a(conjunto_siguiente);
	}
	check_input(conjunto_siguiente, conjunto_primero);
}

void a(int synch_set[]){

	int conjunto_primero[] = {L_CORCHETE, R_CORCHETE, L_LLAVE, '\0'};
	int conjunto_siguiente[] = {L_CORCHETE, COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == L_LLAVE || t.compLex == L_CORCHETE){
		element_list(conjunto_siguiente);
		reconocer(R_CORCHETE);
	}
	else if(t.compLex == R_CORCHETE){
		reconocer(R_CORCHETE);
	}
	check_input(conjunto_siguiente, conjunto_primero);
}

void element_list(int synch_set[]){

	int conjunto_primero[] = {L_CORCHETE, L_LLAVE, '\0'};
	int conjunto_siguiente[] = {R_CORCHETE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
		element(conjunto_siguiente);
		e(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
	
}

void e(int synch_set[]){

	int conjunto_primero[] = {COMA, '\0'};
	int conjunto_siguiente[] = {R_CORCHETE, '\0'};

	if(t.compLex == R_CORCHETE){
			return;
	}

	check_input(conjunto_primero, synch_set);

	if(t.compLex == COMA){
		reconocer(COMA);
		element(conjunto_siguiente);
		e(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void object(int synch_set[]){

	int conjunto_primero[] = {L_LLAVE,'\0'};
	int conjunto_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == L_LLAVE){
		reconocer(L_LLAVE);
		o(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void o(int synch_set[]){

	int conjunto_primero[] = {R_LLAVE, LITERAL_CADENA, '\0'};
	int conjunto_siguiente[] = {LITERAL_CADENA, COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == LITERAL_CADENA){
		reconocer(R_LLAVE);
		attributes_list(conjunto_siguiente);
	}
	else if(t.compLex == R_LLAVE){
		reconocer(R_LLAVE);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void attributes_list(int synch_set[]){

	int conjunto_primero[] = {LITERAL_CADENA, '\0'};
	int conjunto_siguiente[] = {R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == LITERAL_CADENA){
		attribute(conjunto_siguiente);
		at(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);

}

void at(int synch_set[]){

	int conjunto_primero[] = {COMA, '\0'};
	int conjunto_siguiente[] = {R_LLAVE, '\0'};

	if(t.compLex == R_LLAVE){
		return;
	}

	check_input(conjunto_primero, synch_set);

	if(t.compLex == COMA){
		reconocer(COMA);
		attribute(conjunto_siguiente);
		at(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void attribute(int synch_set[]){

	int conjunto_primero[] = {LITERAL_CADENA, '\0'};
	int conjunto_siguiente[] = {COMA, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == LITERAL_CADENA){
		reconocer(DOS_PUNTOS);
		attribute_name(conjunto_siguiente);
		attribute_value(conjunto_siguiente);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void attribute_name(int synch_set[]){

	int conjunto_primero[] = {LITERAL_CADENA, '\0'};
	int conjunto_siguiente[] = {DOS_PUNTOS, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == LITERAL_CADENA){
		reconocer(LITERAL_CADENA);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

void attribute_value(int synch_set[]){

	int conjunto_primero[] = {L_CORCHETE, L_LLAVE, LITERAL_CADENA, LITERAL_NUM, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int conjunto_siguiente[] = {COMA, R_LLAVE, '\0'};

	check_input(conjunto_primero, synch_set);

	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
		element(conjunto_siguiente);
	}
	else if(t.compLex == LITERAL_CADENA){
		reconocer(LITERAL_CADENA);
	}
	else if(t.compLex == LITERAL_NUM){
		reconocer(LITERAL_NUM);
	}
	else if(t.compLex == PR_TRUE){
		reconocer(PR_TRUE);
	}
	else if(t.compLex == PR_FALSE){
		reconocer(PR_FALSE);
	}
	else if(t.compLex == PR_NULL){
		reconocer(PR_NULL);
	}

	check_input(conjunto_siguiente, conjunto_primero);
}

int main(int argc, char* args[]){

	if(argc > 1)
	{		
        if(!(archivo=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }

        sigLex();
        json();

        if(error_de_sintaxis == 0) {
			printf("El programa es sintacticamente correcto \n");
		}
        fclose(archivo);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}

