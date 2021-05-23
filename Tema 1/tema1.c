/* KIRJAN DIMOVSKI 312CB*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "tema1.h"
/* sum of digits used for the card id*/
int sum(long long int id)
{
	int remain = 0;
	int suma = 0;
	while(id != 0)
	{
		remain = id % 10;
		suma = suma + remain;
		id = id / 10;
	}
	return suma;
}
/*checks if there is anything else than a digit*/
long long int myatoiLL(char *number)
{
	int l=strlen(number);
	int i;
	for(i = 0; i < l; i++)
	{
		if(!isdigit(number[i])) return 0;
	}
	return atoll(number);
}

int myatoi(char *number)
{
	int l=strlen(number);
	int i;
	for(i = 0; i < l; i++)
	{
		if(!isdigit(number[i])) return 0;
	}
	return atoi(number);
}

int searchcard(LSCL cur, long long id)
{
	LSCL head = cur;
	TLista x = NULL;

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while(x != NULL)
			{
				if(id == x->info.cardnumber)
				{
					return 1;
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}

	return 0;
}
/*traverses the entire history list of a card and prints it*/
void printhistory(TLhistory L)
{
	char* cancel = "cancel";
	char* balance = "balance_inquiry";
	char* recharge = "recharge";
	char* withdraw = "cash_withdrawal";
	while (L != NULL)
	{
		if(strcmp(L->infoH.operation, cancel) == 0 ||
			strcmp(L->infoH.operation, balance) == 0)
		{
			printf("(%s, %s %lld)", L->infoH.status, L->infoH.operation,
				L->infoH.idowner);
			if(L->next != NULL)
			{
				printf(", ");
			}
		}
		else if(strcmp(L->infoH.operation, recharge) == 0 ||
			strcmp(L->infoH.operation, withdraw) == 0)
		{
			printf("(%s, %s %lld %d)", L->infoH.status, L->infoH.operation,
				L->infoH.idowner, L->infoH.sum);
			if(L->next != NULL)
			{
				printf(", ");
			}
		}
		else
		{
			printf("(%s, %s %lld %s)", L->infoH.status, L->infoH.operation,
				L->infoH.idowner, L->infoH.pin);
			if(L->next != NULL)
			{
				printf(", ");
			}
		}
		L = L->next;
	}
}
/*traverses through the entire card list*/
void printlist(TLista L, long long id)
{
	TLhistory h = NULL;
	if(L != NULL)
	{
		if(id == -1)
		{
			printf("\n");
			while(L != NULL)
			{
				h = L->info.infoH;
				printf("(card number: %lli, PIN: %s, expiry date: %s, CVV: %d,"
					" balance: %d, status: %s, ", L->info.cardnumber,
					L->info.PIN, L->info.date,L->info.CVV,
					L->info.balance, L->info.status);
						printf("history: [");
						printhistory(h);
						printf("])\n");

				L = L->next;
			}
		}
		else
		{
			while(L != NULL)
			{
				if(id == L->info.cardnumber)
				{
					h = L->info.infoH;
					printf("(card number: %lli, PIN: %s, expiry date: %s, CVV: %d,"
						" balance: %d, status: %s,", L->info.cardnumber,
						L->info.PIN, L->info.date,L->info.CVV,
						L->info.balance, L->info.status);
					printf("history: [");
					printhistory(h);
					printf("])\n");
				}
				L = L->next;
			}
		}
	}
}
/*traverses through the entire LCS list*/
void printlsc(LSCL L, long long int id)
{
	TLhistory h = NULL;
	if(L != NULL)
	{
		if(id == -1)
		{
			while(L != NULL)
			{
				TLista x = L->nextcard;
				printf("pos%d: [", L->position);
				printlist(x, id);
				printf("]\n");
				L = L->nextLSC;
			}
		}
		else
		{
			while(L != NULL)
			{
				if(L->nextcard != NULL)
				{
					TLista x = L->nextcard;
					while(x != NULL)
					{
						if(x->info.cardnumber == id)
						{
							h = x->info.infoH;
							printf("(card number: %lli, PIN: %s, expiry date: %s, CVV: %d,"
								" balance: %d, status: %s, ", 
								x->info.cardnumber, x->info.PIN, x->info.date,
								x->info.CVV, x->info.balance, x->info.status);
							printf("history: [");
							printhistory(h);
							printf("])");
						}
						x = x->next;
					}
				}
				L = L->nextLSC;
			}
		}
	}
}
/*adds a node at the beginning of the history list*/
TLhistory createlistofhistory(ALhistory P, char* operation, long long int id,
	char* pin, char* status, int balance, long long int rec)
{
	TLhistory L = (TLhistory)malloc(sizeof(TChistory));

	strcpy(L->infoH.status,status);
	strcpy(L->infoH.operation, operation);
	strcpy(L->infoH.pin, pin);
	L->infoH.sum = balance;
	L->infoH.reciever = rec;
	L->infoH.idowner = id;

	L->next = *P;

	*P = L;
	return L;
}
/*adds a node at the beginning of the card list*/
TLista createlistofcards(ALista P, long long int id, char* pin,
	char* date, int cvv, char* status, int balance)
{
	TLista L = (TLista)malloc(sizeof(TCnode));

	L->info.cardnumber = id;
	strcpy(L->info.PIN, pin);
	strcpy(L->info.date, date);
	strcpy(L->info.status, status);
	L->info.pin_status = -1;
	L->info.CVV = cvv;
	L->info.balance = balance;
	L->next = NULL;
	L->info.infoH = NULL;
	L->info.blocked = -1;
	char* newstatus = "NEW";
	memcpy(L->info.status, newstatus, 4);


	L->next = *P;

	*P = L;
	return L;
}
/*creates the initial LCS then checks if there is a need for more positions*/
LSCL createlistofLSC(int position, LSCL cur)
{
	int i = 0;
	LSCL p = NULL;
	LSCL head = cur;
	LSCL temp = NULL;
	if(head != NULL) 
	{
		temp = head;
		while(temp != NULL) 
		{
			i++;
			temp = temp->nextLSC;
		}
	}
	for(; i<=position; i++)
	{
		temp = (LSCL)malloc(sizeof(TLSC));
		temp->position=i;
		temp->nextLSC = NULL;
		temp->nextcard = NULL;
		if(head == NULL)
		{
			head = temp;
		}
		else
		{
			p = head;
			while(p->nextLSC != NULL)
			{
				p = p->nextLSC;
			}
			p->nextLSC = temp;
		}
	}
	return head;
}
/*attaches the list to the pointer of LSC*/
LSCL addcardstoLSC(LSCL cur, int pos, long long int id, char* pin,
	char* date, int CVV, char* status, int balance)
{
	LSCL head = cur;
	TLista p = NULL;
	TLista temp = NULL;

	while(head != NULL)
	{
		if(head->position == pos)
		{
			if(head->nextcard == NULL)
			{
				head->nextcard = createlistofcards(&temp,
					id, pin, date, CVV, status, balance);
				return head;
			}
			else
			{
				p = head->nextcard;
				head->nextcard = createlistofcards(&p,
					id, pin, date, CVV, status, balance);

			}
		}
		head = head->nextLSC;
	}
	return head;
}
/*attaches the history to the pointer of the card*/
LSCL addhistorytocard(LSCL cur, char* operation, long long int id,
	char* pin, char* status, int balance, long long int rec)
{
	LSCL head = cur;
	TLista PC = NULL;
	TLhistory tempH = NULL;
	TLhistory PH = NULL;

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			PC = head->nextcard;
			while(PC != NULL)
			{
				if(PC->info.cardnumber == id)
				{
					if (PC->info.infoH == NULL)
					{
						PC->info.infoH = createlistofhistory(&tempH,
							operation, id, pin, status, balance, rec);
						return head;
					}
					else
					{
						PH = PC->info.infoH;		
						PC->info.infoH = createlistofhistory(&PH, operation,
							id, pin, status, balance, rec);
					}
				}
				PC = PC->next;
			}
		}
		head = head->nextLSC;
	}
	return head;
}
/*checks if the  element is at the beginning of the LSC list
deletes it, otherwise enters the card list and searches it there*/
void deleteCard(LSCL P, long long int id)
{
	TLista x = NULL;
	TLista temp = NULL;


	while(P != NULL)
	{
		if(P->nextcard != NULL && (P->nextcard)->info.cardnumber == id)
		{
			x = P->nextcard;
			P->nextcard = (P->nextcard)->next;
			free(x);
		}
		else
		{
			x = P->nextcard;
			while(x != NULL)
			{
				if(x->next != NULL && (x->next)->info.cardnumber == id)
				{
					temp = x->next;
					x->next = x->next->next;
					free(temp);
					break;
				}
				x= x->next;
			}
		}
		P = P->nextLSC;
	}
}

void deleteListofhistory(TLhistory P)
{
	TLhistory current = P;
	TLhistory next = NULL;

	while(current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}

	P = NULL;
}

void deleteListofcards(TLista P)
{
	TLista current = P;
	TLista next = NULL;
	TLhistory x = NULL;

	while(current != NULL)
	{
		next = current->next;
		x = current->info.infoH;
		deleteListofhistory(x);
		free(current);
		current = next;
	}

	P = NULL;
}

void deleteListofLSCL(LSCA P)
{
	LSCL current = *P;
	LSCL next = NULL;
	TLista x = NULL;

	while(current != NULL)
	{
		next = current->nextLSC;
		x = current->nextcard;
		deleteListofcards(x);
		free(current);
		current = next;
	}

	*P = NULL;
}

/*parses the command line and applies the correct values*/
struct Operation parseOperation(char* one_operation_string)
{
	char* operation;
	struct Operation op;
	int i = 0;

	char add[30] = "add_card";
	char insert[30] = "insert_card";
	char delete[30] = "delete_card";
	char recharge[30] = "recharge";
	char changeP[30] = "pin_change";
	char withd[30] = "cash_withdrawal";

	operation = strtok(one_operation_string, " ");
	char* newstatus = "NEW";
	op.idowner = -1;
	op.reciever = -1;
	op.pin[0] = 0;
	op.pin[1] = 0;
	op.pin[2] = 0;
	op.pin[3] = 0;
	op.pin[4] = 0;
	op.CVV = -1;
	memcpy(op.card_status, newstatus, 4);
	op.balance = 0;
	op.blocked = -1;
	op.pin_status = -1;
	op.addsum = 0;

	while(operation)
	{
		if(i==0)
		{
			strcpy(op.operation,operation);
		}
		else if(i==1)
		{
			if(operation != NULL)
			{
				op.idowner = myatoiLL(operation);
			}
			else
			{
				break;
			}
		}
		else if(i==2)
		{
			if(strcmp(op.operation, insert) == 0 || strcmp(op.operation, delete) == 0
				|| strcmp(op.operation, add) == 0 || strcmp(op.operation, changeP) == 0)
			{
				strcpy(op.pin,operation);
			}
			else if(strcmp(op.operation, recharge) == 0
				|| strcmp(op.operation, withd) == 0)
			{
				op.addsum = atoi(operation);
			}
		}
		else if(i==3)
		{
			strcpy(op.date,operation);
		}
		else if(i==4)
		{
			op.CVV = atoi(operation);
		}

		i++;
		operation = strtok(NULL, " ");
	}
	return op;
}

void unblockcard(LSCL cur, long long int id)
{
	LSCL head = cur;
	TLista x = NULL;
	char act[8]="ACTIVE";

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while(x != NULL)
			{
				if(id == x->info.cardnumber)
				{
					strcpy(x->info.status, act);
					x->info.blocked = -1;
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}
}

int insertCard(LSCL cur, long long int id, char* pin)
{
	LSCL head = cur;
	char block[8] = "LOCKED";
	TLista x = NULL;
	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while (x != NULL)
			{
				if(id == x->info.cardnumber)
				{
					if(strcmp(pin, x->info.PIN) == 0)
					{
						if(x->info.pin_status == -1 && strcmp(x->info.status, block)!=0)
						{
							printf("You must change your PIN.\n");
							x->info.blocked = -1;
							return 1;
						}
						else if(x->info.pin_status == 0 && strcmp(x->info.status, block)!=0)
						{
							x->info.blocked = -1;
							return 1;
						}
						else if(strcmp(x->info.status, block)==0)
						{
							printf("The card is blocked. Please contact the administrator.\n");
						}
					}
					else
					{
						printf("Invalid PIN\n");
						x->info.blocked++;
						if(x->info.blocked == 2)
						{
							printf("The card is blocked. Please contact the administrator.\n");
							strcpy(x->info.status, block);
							return 0;
						}
					}
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}
	return 0;
}

int pinChange(LSCL L, long long int id, char* pin)
{
	int characters = myatoi(pin);
	int lenght = strlen(pin);
	char act[8] = "ACTIVE";

	LSCL head = L;
	TLista x = NULL;

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while(x != NULL)
			{
				if(x->info.cardnumber == id)
				{
					if( characters == 0 || lenght != 4)
					{
						printf("Invalid PIN\n");
						return 0;
					}
					else
					{
						strcpy(x->info.PIN, pin);
						strcpy(x->info.status, act);
						x->info.pin_status = 0;
						return 1;
					}
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}
	return 0;
}

void balanceCard(LSCL L, long long int id)
{
	LSCL head = L;
	TLista x = NULL;

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while(x != NULL)
			{
				if(x->info.cardnumber == id)
				{
					printf("%d\n", x->info.balance);
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}
}

int rechargeSum(LSCL L, long long int id, int added)
{
	LSCL head = L;
	TLista x = NULL;

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while(x != NULL)
			{
				if(x->info.cardnumber == id)
				{
					if(added % 10 == 0)
					{
						x->info.balance = x->info.balance + added;
						printf("%d\n", x->info.balance);
						return 1;
					}
					else
					{
						printf("The added amount must be multiple of 10\n");
						return 0;
					}
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}
	return 0;
}

int cashWithdraw(LSCL L, long long int id, int added)
{
	LSCL head = L;
	TLista x = NULL;

	while(head != NULL)
	{
		if(head->nextcard != NULL)
		{
			x = head->nextcard;
			while(x != NULL)
			{
				if(x->info.cardnumber == id)
				{
					if(added % 10 == 0)
					{
						if(added > x->info.balance)
						{
							printf("Insufficient funds\n");
							return 0;
						}
						else
						{
							x->info.balance = x->info.balance - added;
							printf("%d\n", x->info.balance);
							return 1;
						}
					}
					else
					{
						printf("The requested amount must be multiple of 10\n");
						return 0;
					}
				}
				x = x->next;
			}
		}
		head = head->nextLSC;
	}
	return 0;
}

int main()
{
	int nr_max_cards;
	char commands[200][200];
	char buf[200];
	int i;
	int number_of_commands = 0;
	int sum_of_digits;
	int position;
	char addC[30] = "add_card";
	char show[30] = "show";
	char delete[30] = "delete_card";
	char insert[30] = "insert_card";
	char cancel[30] = "cancel";
	char unblock[30] = "unblock_card";
	char success[10] = "SUCCESS";
	char balance[30] = "balance_inquiry";
	char withd[30] = "cash_withdrawal";
	char changeP[30] = "pin_change";
	char recharge[30] = "recharge";
	char fail[6] = "FAIL";
	struct Operation operations;

	LSCL y = NULL;
	//LSCL x = NULL;

	FILE *input_file = fopen("input.in", "r");

	if(input_file == NULL)
	{
		fprintf(stderr, "File not found\n");
		return -2;
	}

	while (fgets(buf, 200, input_file))
	{
		buf[strlen(buf) - 1] = '\0';
		strcpy(commands[number_of_commands], buf);
		number_of_commands++;
	}

	fclose(input_file);

	nr_max_cards = atoi(commands[0]);

	FILE *ot;
	ot = freopen("output.out", "a+", stdout);
/*goes through all the commands, parses them and applies the needed operations*/
	for(i = 1; i<number_of_commands; i++)
	{
		operations = parseOperation(commands[i]);
		if(strcmp(operations.operation, addC)== 0)
		{
			sum_of_digits = sum(operations.idowner);

			position = sum_of_digits % nr_max_cards;

			if(searchcard(y, operations.idowner) == 0)
			{
				y = createlistofLSC(position,y);
				addcardstoLSC(y, position, operations.idowner,
					operations.pin, operations.date, operations.CVV,
					operations.card_status, operations.balance);
			}
			else
			{
				printf("The card already exists\n");
			}

		}
		else if(strcmp(operations.operation, show) == 0)
		{
			sum_of_digits = sum(operations.idowner);
			printlsc(y, operations.idowner);
		}
		else if(strcmp(operations.operation, delete) == 0)
		{
			deleteCard(y,operations.idowner);
		}
		else if(strcmp(operations.operation, insert) == 0)
		{
			
			if(insertCard(y,operations.idowner,operations.pin) == 0)
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, fail,
					operations.addsum, operations.reciever);
			}
			else
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, success,
					operations.addsum, operations.reciever);
			}
		}
		else if(strcmp(operations.operation, cancel) == 0)
		{
			addhistorytocard(y,operations.operation,
				operations.idowner, operations.pin, success,
				operations.addsum, operations.reciever);
		}
		else if(strcmp(operations.operation, unblock) == 0)
		{
			unblockcard(y, operations.idowner);
		}
		else if(strcmp(operations.operation, changeP) == 0)
		{
			if(pinChange(y,operations.idowner,operations.pin) == 0)
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, fail,
					operations.addsum, operations.reciever);
			}
			else
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, success,
					operations.addsum, operations.reciever);
			}
		}
		else if(strcmp(operations.operation, balance) == 0)
		{
			balanceCard(y,operations.idowner);
			addhistorytocard(y,operations.operation,
				operations.idowner, operations.pin, success,
				operations.addsum, operations.reciever);
		}
		else if(strcmp(operations.operation, recharge) == 0)
		{
			if(rechargeSum(y,operations.idowner,operations.addsum) == 0)
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, fail,
					operations.addsum, operations.reciever);
			}
			else
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, success,
					operations.addsum, operations.reciever);
			}
		}
		else if(strcmp(operations.operation, withd) == 0)
		{
			if(cashWithdraw(y,operations.idowner,operations.addsum) == 0)
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, fail,
					operations.addsum, operations.reciever);
			}
			else
			{
				addhistorytocard(y,operations.operation,
					operations.idowner, operations.pin, success,
					operations.addsum, operations.reciever);
			}
		}
	}
	fclose(ot);
	deleteListofLSCL(&y);
	return 0;
}