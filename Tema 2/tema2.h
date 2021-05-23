/* DIMOVSKI KIRJAN - 312CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct Node
{
	void *data;
	struct Node *next;
}Tnode, *TLista, **ALista;

typedef struct memoryNode
{
	int PID;
	int exec_time_in_ms;
	int priority;
	int mem_size_in_bytes;
	long int start_memory;
	long int free_memory;
	long int end_memory;
	void *stack;
	struct memoryNode *next;
}TMnode, *TMLista, **AMLista;

typedef struct DataC
{
	int PID;
	int remaining_time;
	int executed_time;
	int priority;
	int allowed_remaining_time;
}TDataC;

typedef struct 
{
	size_t dim;
	void *top;
} Stiva, *TStiva;

typedef struct 
{
	size_t dim;
	void *front ,*rear;
} Coada, *TCoada;

struct Operation
{
	char operation[30];
	char second_operation[30];
	int sizeT;
	int mem_size_in_bytes;
	int exec_time_in_ms;
	int priority;
	int PID;
	int runtime;
	signed int four_signed_bytes_data;
};