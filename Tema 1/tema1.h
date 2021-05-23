/* KIRJAN DIMOVSKI 312CB*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct
{
	char status[30];
	char operation[30];
	long long int idowner;
	long long int reciever;
	int sum;
	char pin[30];
}Thistory;

typedef struct historynode
{
	Thistory infoH;
	struct historynode * next;
}TChistory, *TLhistory, **ALhistory;

typedef struct 
{
	long long int cardnumber;
	char PIN[5];
	char date[30];
	char status[30];
	int CVV;
	int balance;
	int blocked;
	int pin_status;
	TLhistory infoH;
} Tcard;

typedef struct cardnode
{
	Tcard info;
	struct cardnode * next;
} TCnode, *TLista, **ALista;

typedef struct LSCnode
{
	int position;
	struct cardnode * nextcard;
	struct LSCnode * nextLSC;
}TLSC, *LSCL, **LSCA;

struct Operation
{
	char operation[30];
	long long int idowner;
	long long int reciever;
	char pin[5];
	char date[10];
	char idreciever[30];
	char card_status[30];
	int balance;
	int nr_max_cards;
	int CVV;
	int pin_status;
	int blocked;
	int addsum;
};