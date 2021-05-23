/* DIMOVSKI KIRJAN - 312CB */
#include "tema2.h"

int searchPID(TMLista L, int PID)
{
	TMLista temp = L;
	if(temp == NULL)
	{
		return 0;
	}
	if(temp->PID == PID)
	{
		return 1;
	}
	return searchPID(temp->next, PID);
}

int assignPID(AMLista L)
{
	int i;
	int PID;
	for(i = 1; i < 128; i++)
	{
		if(!searchPID(*L, i))
		{
			PID = i;
			break;
		}
	}
	return PID;
}


void *InitQ(size_t d)
{
	TCoada aux;
	aux = (TCoada)malloc(sizeof(Coada));
	aux->dim = d;
	aux->front = aux->rear = NULL;
	return (void *)aux;
}

TLista alocCelCo(void *ae)
{
	TLista aux = (TLista)malloc(sizeof(Tnode));
	aux->data = malloc(sizeof(TDataC));
	aux->next = NULL;
	memcpy(aux->data, ae, sizeof(TDataC));
	return aux;
}

void Enqueue(void *s, void *ae)
{
	TLista aux = alocCelCo(ae);
	if(((TCoada)s)->rear == NULL)
	{
		((TCoada)s)->front = ((TCoada)s)->rear = aux;
		return;
	}

	(((TLista)((TCoada)s)->rear))->next = aux;
	((TCoada)s)->rear = aux;
}

void deQueue(void *s, void *ae)
{
	if(((TCoada)s)->front == NULL)
	{
		return;
	}

	TLista temp = ((TCoada)s)->front;
	memcpy(ae, temp->data, sizeof(TDataC));
	((TCoada)s)->front = (((TLista)((TCoada)s)->front))->next;
	if(((TCoada)s)->front == NULL)
	{
		((TCoada)s)->rear = NULL;
	}
	temp->next = NULL;
	free(temp->data);
	free(temp);
}

void* InitS(size_t d)
{
	TStiva aux;
	aux = (TStiva)malloc(sizeof(Stiva));
	aux->dim = d;
	aux->top = NULL;
	return (void *)aux;
}

TLista alocCel(void *ae, int size)
{
	TLista aux;
	aux = (TLista)malloc(sizeof(Tnode));
	aux->data = malloc(size);
	aux->next = NULL;
	memcpy(aux->data, ae, size);
	return aux;
}

void push(void *s, void *ae, int size)
{
	TLista aux = alocCel(ae, size);
	if(((TStiva)s)->top == NULL)
	{
		((TStiva)s)->top = aux;
		return;
	}
	aux->next = (((TStiva)s)->top);
	(((TStiva)s)->top) = aux;
}

void pop(void *s, void *ae, int size)
{
	if(((TStiva)s)->top == NULL)
	{
		return;
	}

	memcpy(ae, ((TLista)(((TStiva)s)->top))->data, size);
	TLista temp = (((TLista)((TStiva)s)->top));
	((TStiva)s)->top = ((TLista)(((TStiva)s)->top))->next;
	temp->next = NULL;
	free(temp->data);
	free(temp);
}

void DisplayStack(void *s)
{
	if(((TStiva)s)->top == NULL)
	{
		return;
	}
	void *aux = InitS(sizeof(long));
	if(!aux)
	{
		return;
	}
	void *x = malloc(sizeof(int));
	if(!x)
	{
		free(aux);
		return;
	}
	while(((TStiva)s)->top != NULL)
	{
		pop(s, x, sizeof(int));
		push(aux, x, sizeof(int));
	}
	while(((TStiva)aux)->top != NULL)
	{
		pop(aux, x, sizeof(int));
		printf(" %d", *(int*)x);
		push(s, x, sizeof(int));
	}
	printf(".\n");
	free(x);
	free(aux);
}

void StackOfMemory(AMLista L, int PID)
{
	if(*L == NULL)
	{
		return;
	}

	TMLista temp = *L;
	int i = 0;
	while(temp != NULL)
	{
		if(temp->PID == PID)
		{
			if(((TStiva)temp->stack)->top == NULL)
			{
				printf("Empty stack PID %d.\n", temp->PID);
				i = 1;
			}
			else
			{
				printf("Stack of PID %d:", temp->PID);
				DisplayStack(temp->stack);
				i = 1;
			}
		}
		temp = temp->next;
	}

	if(i == 0)
	{
		printf("PID %d not found.\n", PID);
	}
}

void getProcess(void *running, void *finished, void *waiting, int PID)
{
	int i = 0;
	void *aux = InitQ(sizeof(long));
	if(!aux)
	{
		return;
	}
	void *x = malloc(sizeof(TDataC));
	if(!x)
	{
		free(aux);
		return;
	}
	if(((TCoada)running)->front != NULL && 
		(*(TDataC*)(((TLista)((TCoada)running)->front))->data).PID == PID)
	{
		printf("Process %d is running (remaining_time: %d).\n",
			(*(TDataC*)(((TLista)((TCoada)running)->front))->data).PID,
			(*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time);
		i = 1;
	}

	if(((TCoada)waiting)->front != NULL)
	{
		while(((TCoada)waiting)->front != NULL)
		{
			if((*(TDataC*)(((TLista)((TCoada)waiting)->front))->data).PID == PID)
			{
				printf("Process %d is waiting (remaining_time: %d).\n", 
					(*(TDataC*)(((TLista)((TCoada)waiting)->front))->data).PID,
					(*(TDataC*)(((TLista)((TCoada)waiting)->front))->data).remaining_time);
				i = 1;
			}
			deQueue(waiting, x);
			Enqueue(aux, x);
		}
		while(((TCoada)aux)->front != NULL)
		{
			deQueue(aux, x);
			Enqueue(waiting, x);
		}
	}

	if(((TCoada)finished)->front != NULL)
	{
		while(((TCoada)finished)->front != NULL)
		{
			if((*(TDataC*)(((TLista)((TCoada)finished)->front))->data).PID == PID)
			{
				printf("Process %d is finished.\n", 
					(*(TDataC*)(((TLista)((TCoada)finished)->front))->data).PID);
				i = 1;
			}
			deQueue(finished, x);
			Enqueue(aux, x);
		}
		while(((TCoada)aux)->front != NULL)
		{
			deQueue(aux, x);
			Enqueue(finished, x);
		}
	}
	
	if(i == 0)
	{
		printf("Process %d not found.\n", PID);
	}
	free(aux);
	free(x);
}

void pushProcessStack(AMLista L, int PID, int four_signed_bytes_data)
{
	TMLista temp = *L;
	int i = 0;
	while(temp != NULL)
	{
		if(temp->PID == PID)
		{
			if(temp->stack == NULL)
			{
				void *x = malloc(sizeof(int));
				memcpy(x, &four_signed_bytes_data, sizeof(four_signed_bytes_data));
				temp->stack = InitS(sizeof(long));
				push(temp->stack, x, sizeof(x));
				i = 1;
				free(x);
			}
			else
			{
				if(temp->free_memory <= 3)
				{
					printf("Stack overflow PID %d.\n", temp->PID);
					return;
				}
				void *x = malloc(sizeof(int));
				memcpy(x, &four_signed_bytes_data, sizeof(four_signed_bytes_data));
				push(temp->stack, x, sizeof(x));
				temp->free_memory -= 4;
				i = 1;
				free(x);
			}
		}
		temp = temp->next;
	}

	if(i == 0)
	{
		printf("PID %d not found.\n", PID);
	}
}

void popProcessStack(AMLista L, int PID)
{
	TMLista temp = *L;
	int i = 0;
	while(temp != NULL)
	{
		if(temp->PID == PID)
		{
			if(((TStiva)temp->stack)->top == NULL)
			{
				printf("Empty stack PID %d.\n", temp->PID);
				i = 1;
			}
			else
			{
				void *x = malloc(sizeof(int));
				if(!x)
				{
					return;
				}
				pop(temp->stack, x, sizeof(int));
				temp->free_memory += 4;
				i = 1;
				free(x);
			}
		}
		temp = temp->next;
	}
	if(i == 0)
	{
		printf("PID %d not found.\n", PID);
	}
}

void sortedQueue(void *s, void *ae)
{
	if(((TCoada)s)->front == NULL)
	{
		Enqueue(s, ae);
		return;
	}
	void *aux = InitQ(sizeof(long));
	if(!aux)
	{
		return;
	}
	int i = 1;
	void *x = malloc(sizeof(TDataC));
	if(!x)
	{
		free(aux);
		return;
	}
	while(((TCoada)s)->front != NULL)
	{
		deQueue(s, x);
		if(i == 1)
		{
			if((*(TDataC*)x).priority > (*(TDataC*)ae).priority)
			{
				Enqueue(aux, x);
				if(((TCoada)s)->rear == NULL)
				{
					Enqueue(aux, ae);
					i = 0;
				}
			}
			else if((*(TDataC*)x).priority < (*(TDataC*)ae).priority)
			{
				Enqueue(aux, ae);
				i = 0;
				Enqueue(aux, x);
			}
			else if((*(TDataC*)x).priority == (*(TDataC*)ae).priority)
			{
				if((*(TDataC*)x).remaining_time < (*(TDataC*)ae).remaining_time)
				{
					Enqueue(aux, x);
					if(((TCoada)s)->rear == NULL)
					{
						Enqueue(aux, ae);
						i = 0;
					}
				}
				else if((*(TDataC*)x).remaining_time > (*(TDataC*)ae).remaining_time)
				{
					Enqueue(aux, ae);
					i = 0;
					Enqueue(aux, x);
				}
				else if((*(TDataC*)x).remaining_time == (*(TDataC*)ae).remaining_time)
				{
					if((*(TDataC*)x).PID < (*(TDataC*)ae).PID)
					{
						Enqueue(aux, x);
						if(((TCoada)s)->rear == NULL)
						{
							Enqueue(aux, ae);
							i = 0;
						}
					}
					else if((*(TDataC*)x).PID > (*(TDataC*)ae).PID)
					{
						Enqueue(aux, ae);
						Enqueue(aux, x);
						i = 0;
					}
				}
			}
		}
		else
		{
			Enqueue(aux, x);
		}
		
	}
	while(((TCoada)aux)->front != NULL)
	{
		deQueue(aux, x);
		Enqueue(s, x);
	}

	free(aux);
	free(x);
}

void DisplayQueue(void *s, char* queue)
{
	char wait[10] = "waiting";
	char finished[10] = "finished";
	void *aux = InitQ(sizeof(long));
	if(!aux)
	{
		return;
	}
	void *x = malloc(sizeof(TDataC));
	if(!x)
	{
		free(aux);
		return;
	}
	if(strcmp(queue, wait) == 0)
	{	
		printf("Waiting queue:\n");
		printf("[");
		while(((TCoada)s)->front != NULL)
		{
			deQueue(s, x);
			if(((TCoada)s)->front == NULL)
			{
				printf("(%d: priority = %d, remaining_time = %d)",
				(*(TDataC*)x).PID, (*(TDataC*)x).priority, (*(TDataC*)x).remaining_time);
			}
			else
			{
				printf("(%d: priority = %d, remaining_time = %d),\n",
				(*(TDataC*)x).PID, (*(TDataC*)x).priority, (*(TDataC*)x).remaining_time);
			}
		Enqueue(aux, x);
		}
		printf("]\n");
		while(((TCoada)aux)->front != NULL)
		{
			deQueue(aux, x);
			Enqueue(s, x);
		}
	}
	else if(strcmp(queue, finished) == 0)
	{
		printf("Finished queue:\n");
		printf("[");
		while(((TCoada)s)->front != NULL)
		{
			deQueue(s, x);
			if(((TCoada)s)->front == NULL)
			{
				printf("(%d: priority = %d, executed_time = %d)",
				(*(TDataC*)x).PID, (*(TDataC*)x).priority, (*(TDataC*)x).executed_time);
			}
			else
			{
				printf("(%d: priority = %d, executed_time = %d),\n",
				(*(TDataC*)x).PID, (*(TDataC*)x).priority, (*(TDataC*)x).executed_time);
			}
			Enqueue(aux, x);
		}
		printf("]\n");
		while(((TCoada)aux)->front != NULL)
		{
			deQueue(aux, x);
			Enqueue(s, x);
		}
	}
	free(x);
	free(aux);
}

void deleteStack(TMLista L)
{
	void *x = malloc(sizeof(int));
	if(!x)
	{
		return;
	}
	if(L->stack != NULL)
	{
		while(((TStiva)L->stack)->top != NULL)
		{
			pop(L->stack, x, sizeof(int));
		}
	}
	free(L->stack);
	free(x);
}

void deleteProcess(AMLista L, int PID)
{
	TMLista temp = *L;
	TMLista prev;

	if(temp != NULL && temp->PID == PID)
	{
		*L = temp->next;
		deleteStack(temp);
		free(temp);
		return;
	}

	while(temp != NULL && temp->PID != PID)
	{
		prev = temp;
		temp = temp->next;
	}

	if(temp == NULL)
	{
		return;
	}
	prev->next = temp->next;
	deleteStack(temp);
	free(temp);
}

void ExecuteProcess(void *running, void *finished, void *waiting, AMLista L, int runtime, int T)
{
	void *x = malloc(sizeof(TDataC));
	if(!x)
	{
		return;
	}
	int PID;
	if(((TCoada)running)->front == NULL)
	{
		free(x);
		return;
	}
	while(runtime > 0)
	{
		if(((TCoada)running)->front==NULL) 
		{
			free(x);
			return;
		}
		if(runtime > (*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time)
		{
			(*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time -=
			(*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time;
			if((*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time <= 0)
			{
				runtime -= (*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time
				+ (*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time;
				(*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time = 0;
				PID = (*(TDataC*)(((TLista)((TCoada)running)->front))->data).PID;
				deQueue(running, x);
				Enqueue(finished, x);
				if(((TCoada)waiting)->front != NULL)
				{
					deQueue(waiting, x);
					Enqueue(running, x);
				}
				deleteProcess(L, PID);
			}
			else
			{
				runtime -= (*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time;
				if(((TCoada)waiting)->front != NULL)
				{
					(*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time = T;
					deQueue(waiting, x);
					Enqueue(running, x);
					deQueue(running, x);
					sortedQueue(waiting, x);
				} 
				else 
				{
					(*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time = T;		
				}
			}
		}
		else
		{
			(*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time -= runtime;
			if((*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time <= 0)
			{
				runtime -= runtime + (*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time;
				(*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time = 0;
				PID = (*(TDataC*)(((TLista)((TCoada)running)->front))->data).PID;
				deQueue(running, x);
				Enqueue(finished, x);
				if(((TCoada)waiting)->front != NULL)
				{
					deQueue(waiting, x);
					Enqueue(running, x);
				}
				deleteProcess(L, PID);
			}
			else
			{
				(*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time -= runtime;
				runtime -= runtime;
				if((*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time == 
					0 && ((TCoada)waiting)->front != NULL)
				{
					(*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time = T;
					deQueue(waiting, x);
					Enqueue(running, x);
					deQueue(running, x);
					sortedQueue(waiting, x);
				}
				else if((*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time == 0)
				{
					(*(TDataC*)(((TLista)((TCoada)running)->front))->data).allowed_remaining_time = T;		
				}
			}
		}
	}
	free(x);
}

void finishTime(void *running, void *waiting, void *finished)
{
	int time = 0;
	void *x = malloc(sizeof(TDataC));
	if(!x)
	{
		return;
	}
	if(((TCoada)running)->front != NULL)
	{
		time += (*(TDataC*)(((TLista)((TCoada)running)->front))->data).remaining_time;
		deQueue(running, x);
		Enqueue(finished, x);
	}
	if(((TCoada)waiting)->front != NULL)
	{
		while(((TCoada)waiting)->front != NULL)
		{
			time += (*(TDataC*)(((TLista)((TCoada)waiting)->front))->data).remaining_time;
			deQueue(waiting, x);
			Enqueue(finished, x);
		}
	}

	printf("Total time: %d\n", time);
	free(x);
}

int checkMemoryOccupied(AMLista L)
{
	int memoryOccupied = 0;
	TMLista temp = *L;

	while(temp != NULL)
	{
		memoryOccupied += temp->mem_size_in_bytes;
		temp = temp->next;
	}
	return memoryOccupied;
}

int checkMemory(AMLista L, int mem_size_in_bytes)
{
	int entireMemory = 3*1024*1024;
	int memoryOccupied = checkMemoryOccupied(L);
	TMLista temp = *L;
	TMLista last = *L;

	if(temp == NULL)
	{
		return 0;
	}
	if(temp->start_memory != 0)
	{
		if(temp->start_memory - mem_size_in_bytes >= 0)
		{
			return 4;
		}
	}
	while(temp != NULL && temp->next != NULL)
	{
		if((temp->next->start_memory - temp->end_memory) >= mem_size_in_bytes)
		{
			return 2;
		}
		temp = temp->next;
	}

	while(last->next != NULL)
	{
		last = last->next;
	}
	if((entireMemory - last->end_memory) >= mem_size_in_bytes)
	{
		return 1;
	}
	else if((entireMemory - memoryOccupied) >= mem_size_in_bytes)
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

void updateStart(AMLista L)
{
	TMLista temp = *L;
	int newstart = 0;
	temp->start_memory = 0;
	temp->end_memory = temp->mem_size_in_bytes;
	newstart = temp->end_memory;
	temp = temp->next;
	while(temp != NULL)
	{
		temp->start_memory = newstart;
		temp->end_memory = temp->start_memory + temp->mem_size_in_bytes;
		newstart += temp->mem_size_in_bytes;
		temp = temp->next;
	}
}

void defragment(AMLista L, TMLista new_process)
{
	TMLista current;

	if(*L == NULL || (*L)->PID >= new_process->PID)
	{
		new_process->next = *L;
		*L = new_process;
	}
	else
	{
		current = *L;
		while(current->next != NULL &&
			current->next->PID < new_process->PID)
		{
			current = current->next;
		}
		new_process->next = current->next;
		current->next = new_process;
	}
}

int addProccess(AMLista L, long int mem_size_in_bytes,
	long int exec_time_in_ms, int priority, void *info, int T)
{
	TMLista new_process = (TMLista)malloc(sizeof(TMnode));
	if(!new_process)
	{
		return -1;
	}

	new_process->mem_size_in_bytes = mem_size_in_bytes;
	new_process->exec_time_in_ms = exec_time_in_ms;
	new_process->priority = priority;
	new_process->PID = assignPID(L);
	new_process->next = NULL;
	new_process->free_memory = mem_size_in_bytes;
	new_process->stack = InitS(sizeof(long));
	if(!new_process->stack)
	{
		free(new_process);
		return -1;
	}

	(*(TDataC*)info).PID = new_process->PID;
	(*(TDataC*)info).priority = priority;
	(*(TDataC*)info).remaining_time = exec_time_in_ms;
	(*(TDataC*)info).executed_time = exec_time_in_ms;
	(*(TDataC*)info).allowed_remaining_time = T;

	if(checkMemory(L, mem_size_in_bytes) == -1)
	{
		printf("Cannot reserve memory for PID %d.\n", new_process->PID);
		free(new_process);
		return 0;
	}
	else if(checkMemory(L, mem_size_in_bytes) == 4)
	{
		new_process->start_memory = 0;
		new_process->end_memory = mem_size_in_bytes;
		new_process->next = *L;
		*L = new_process;
		printf("Process created successfully: PID: %d,"
			" Memory starts at 0x%#lx.\n",
			new_process->PID, new_process->start_memory);
	}
	else if(checkMemory(L, mem_size_in_bytes) == 0)
	{
		new_process->start_memory = 0;
		new_process->end_memory = mem_size_in_bytes;
		new_process->next = *L;
		*L = new_process;
		printf("Process created successfully: PID: %d,"
			" Memory starts at 0x%#lx.\n",
			new_process->PID, new_process->start_memory);
	}
	else if(checkMemory(L, mem_size_in_bytes) == 1)
	{
		TMLista last = *L;
		while(last->next != NULL)
		{
			last = last->next;
		}
		new_process->start_memory = last->end_memory;
		new_process->end_memory = last->end_memory + new_process->mem_size_in_bytes;

		last->next = new_process;
		printf("Process created successfully: PID: %d,"
			" Memory starts at %#lx.\n",
			new_process->PID, new_process->start_memory);
	}
	else if(checkMemory(L, mem_size_in_bytes) == 2)
	{
		int i = 0;
		TMLista counter = *L;
		while(counter != NULL && counter->next != NULL)
		{
			if((counter->next->start_memory - counter->end_memory) >= mem_size_in_bytes)
			{
				break;
			}
			i++;
			counter = counter->next;
		}

		int j;

		for(j = 0; j<=i; j++)
		{
			if(j==i)
			{
				new_process->start_memory = (*L)->end_memory;
				new_process->end_memory = (*L)->end_memory + new_process->mem_size_in_bytes;
				new_process->next = (*L)->next;
				(*L)->next=new_process;
			}
			else
			{
				L = &(*L)->next;
			}
		}
		printf("Process created successfully: PID: %d,"
			" Memory starts at %#lx.\n",
			new_process->PID, new_process->start_memory);
	}
	else if(checkMemory(L, mem_size_in_bytes) == 3)
	{
		TMLista defragmented = NULL;

		TMLista current = *L;

		while(current != NULL)
		{
			TMLista next = current->next;
			defragment(&defragmented, current);
			current = next;
		}
		*L = defragmented;
		updateStart(L);
		TMLista last = *L;
		
		while(last->next != NULL)
		{
			last = last->next;
		}

		new_process->start_memory = last->end_memory;
		new_process->end_memory = last->end_memory + new_process->mem_size_in_bytes;

		last->next = new_process;
		printf("Process created successfully: PID: %d,"
			" Memory starts at %ld.\n",
			new_process->PID, new_process->start_memory);
	}
	return 1;
}

void deleteMemory(AMLista L)
{
	TMLista temp = *L;
	TMLista next = NULL;
	
	while(temp != NULL)
	{	
		next = temp->next;
		deleteStack(temp);
		free(temp);
		temp = next;
	}
	free(next);
}

struct Operation parseOperation(char* one_operation_string)
{
	char* operation;
	char finish[10] = "finish";
	char add[10] = "add";
	char get[10] = "get";
	char pop[10] = "pop";
	char run[10] = "run";
	char push[10] = "push";
	char print[10] = "print";
	char stack[10] = "stack";
	struct Operation op;
	int i = 0;

	operation = strtok(one_operation_string, " ");

	op.mem_size_in_bytes = -1;
	op.exec_time_in_ms = -1;
	op.priority = -1;
	op.PID = -1;
	op.four_signed_bytes_data = -1;


	while(operation)
	{
		if(i==0)
		{
			strcpy(op.operation, operation);
			if(strcmp(op.operation, finish) == 0)
			{
				break;
			}
		}
		if(i==1)
		{
			if(strcmp(op.operation, add) == 0)
			{
				op.mem_size_in_bytes = atoi(operation);
			}
			else if(strcmp(op.operation, get) == 0
				|| strcmp(op.operation, pop) == 0
				|| strcmp(op.operation, push) == 0)
			{
				op.PID = atoi(operation);
			}
			else if(strcmp(op.operation, print) == 0)
			{
				strcpy(op.second_operation, operation);
			}
			else if(strcmp(op.operation, run) == 0)
			{
				op.runtime = atoi(operation);
			}
		}
		if(i==2)
		{
			if(strcmp(op.operation, add) == 0)
			{
				op.exec_time_in_ms = atoi(operation);
			}
			else if(strcmp(op.operation, push) == 0)
			{
				op.four_signed_bytes_data = atoi(operation);
			}
			else if(strcmp(op.second_operation, stack) == 0)
			{
				op.PID = atoi(operation);
			}
		}
		if(i==3)
		{
			if(strcmp(op.operation, add) == 0)
			{
				op.priority = atoi(operation);
			}
			else if(strcmp(op.operation, push) == 0)
			{
				op.four_signed_bytes_data = atoi(operation);
			}
		}
		i++;
		operation = strtok(NULL, " ");
	}
	return op;
}


int main(int argc, char* argv[])
{
	int number_of_commands = 0;
	int i;
	int T;
	char **commands = (char **)malloc(30000 * sizeof(char*));
	for(i = 0; i < 30000; i++)
	{
		commands[i] = (char *)malloc(200 * sizeof(char));
	} 
	char buf[200];
	struct Operation operations;
	TMLista memory = NULL;


	char add[10] = "add";
	char push[10] = "push";
	char pop[10] = "pop";
	char get[10] = "get";
	char run[10] = "run";
	char print_process[10] = "print";
	char finish[10] = "finish";
	char wait[10] = "waiting";
	char finQ[10] = "finished";
	char stack[10] = "stack";

	FILE *input_file = fopen(argv[1], "r");

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

	void *running = InitQ(sizeof(long));
	if(!running)
	{
		return -1;
	}
	void *waiting = InitQ(sizeof(long));
	if(!waiting)
	{
		free(running);
		return -1;
	}
	void *finished = InitQ(sizeof(long));
	if(!finished)
	{
		free(running);
		free(waiting);
		return -1;
	}
	void *c = malloc(sizeof(TDataC));
	if(!c)
	{
		free(running);
		free(waiting);
		free(finished);
		return -1;
	}

	FILE *ot;
	ot = freopen(argv[2], "a+", stdout);
	for(i = 0; i<number_of_commands; i++)
	{
		operations=parseOperation(commands[i]);
		if(i==0)
		{
			T = atoi(operations.operation);
		}
		if(strcmp(operations.operation, add) == 0)
		{
			if(addProccess(&memory, operations.mem_size_in_bytes,
				operations.exec_time_in_ms, operations.priority, c, T))
			{
				(*(TDataC*)c).allowed_remaining_time = T;
				if(((TCoada)running)->front == NULL)
				{
					Enqueue(running, c);
				}
				else
				{
					sortedQueue(waiting, c);
				}
			}
		}
		else if(strcmp(operations.operation, push) == 0)
		{
			pushProcessStack(&memory, operations.PID, operations.four_signed_bytes_data);
		}
		else if(strcmp(operations.operation, pop) == 0)
		{
			popProcessStack(&memory, operations.PID);
		}
		else if(strcmp(operations.operation, print_process) == 0)
		{
			if(strcmp(operations.second_operation, wait) == 0)
			{
				DisplayQueue(waiting, operations.second_operation);
			}
			else if(strcmp(operations.second_operation, finQ) == 0)
			{
				DisplayQueue(finished, operations.second_operation);
			}
			else if(strcmp(operations.second_operation, stack) == 0)
			{
				StackOfMemory(&memory, operations.PID);
			}
		}
		else if(strcmp(operations.operation, finish) == 0)
		{
			finishTime(running, waiting, finished);
		}
		else if(strcmp(operations.operation, get) == 0)
		{
			getProcess(running, finished, waiting, operations.PID);
		}
		else if(strcmp(operations.operation, run) == 0)
		{
			ExecuteProcess(running, finished, waiting, &memory, operations.runtime, T);
		}
	}
	fclose(ot);

	free(running);
	free(waiting);
	while(((TCoada)finished)-> front != NULL)
	{
		deQueue(finished, c);
	}
	free(finished);
	free(c);
	if(memory != NULL)
	{
		deleteMemory(&memory);
	}
	for(i = 0; i < 30000; i++)
	{
		free(commands[i]);
	}
	free(commands);
	return 0;
}