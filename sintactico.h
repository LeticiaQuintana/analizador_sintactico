#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define TAM_CONJUNTOS 12

void reconocer(int token_esperado);
void error_sintactico();
void scanto(int synch_set[]);
void check_input(int conjunto_primero[], int synch_set[]);
void json();
void element(int synch_set[]);
void array(int synch_set[]);
void a(int synch_set[]);
void element_list(int synch_set[]);
void e(int synch_set[]);
void object(int synch_set[]);
void o(int synch_set[]);
void attributes_list(int synch_set[]);
void at(int synch_set[]);
void attribute(int synch_set[]);
void attribute_name(int synch_set[]);
void attribute_value(int synch_set[]);
int error_de_sintaxis = 0;
