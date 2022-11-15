#include "pcb.h"
#include "pthread.h"
#include "../core/machine.h"

#include "stdlib.h"

#include "stdio.h"
void append_to_queue(struct pcb* new_pcb,u32 cpu_index)
{

	pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].queue_mutex);

	//printf("PID=> %d", new_pcb->priority);

	//Para escribirlo mas facil
	struct queue* queue = g_machine.cpu_ptr[cpu_index].run_queue.normal_queue;

	if(queue->bitmap[new_pcb->priority] == 0)
		queue->bitmap[new_pcb->priority] =1;

	//First time insertion
	if(queue->queue_ptr[new_pcb->priority] == NULL)
	{
		struct node* new_node = malloc(sizeof (struct node));
		new_node->pcb = new_pcb;
		new_node->next = NULL;
		queue->queue_ptr[new_pcb->priority] = new_node;
	}
	else
	{
		struct node* tmp = queue->queue_ptr[new_pcb->priority];

		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		//Hemos llegado al final, annadimos el pcb
		struct node* new_node = malloc(sizeof (struct node));
		new_node->pcb = new_pcb;
		new_node->next = NULL;

		tmp->next = new_node;
	}
	pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].queue_mutex);
}

void delete_last_pcb(u32 priority,u32 cpu_index){
	pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].queue_mutex);

	struct queue* queue = g_machine.cpu_ptr[cpu_index].run_queue.normal_queue;
	struct node* actual = queue->queue_ptr[priority];

	if (actual == NULL){
		queue->bitmap[priority] =0;
	}
	else{
		struct node* anterior; //Tenemos que guardar una referencia al anterior nodo al ultimo
		while (actual->next != NULL){
			anterior = actual;
			actual = actual->next;
		}

		if(actual->next == NULL){ //En el caso que sea el primero de la lista a borrar, usamos el puntero del queue correspondiente.
			free(queue->queue_ptr[priority]);
			queue->queue_ptr[priority] = NULL;
		}
		else
		{
			anterior->next = NULL;
			free(actual);
		}

		if( queue->queue_ptr[priority] == NULL){
			//Comprobamos si es el ultimo
			queue->bitmap[priority] =0;
		}
	}
	pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].queue_mutex);
}

struct pcb* get_last_pcb(u32 priority,u32 cpu_index){

	pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].queue_mutex);

	struct queue* queue = g_machine.cpu_ptr[cpu_index].run_queue.normal_queue;
	struct node* tmp = queue->queue_ptr[priority];

	if (tmp == NULL){
		pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].queue_mutex);
		return NULL;
	}
	else
	{
		while (tmp->next != NULL){
			tmp = tmp->next;
		}
		pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].queue_mutex);
		return tmp->pcb;
	}
}
void swap_queues(u32 cpu_index)
{
	pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].queue_mutex);

	struct queue* aux = g_machine.cpu_ptr[cpu_index].run_queue.expired_queue;

	g_machine.cpu_ptr[cpu_index].run_queue.expired_queue = g_machine.cpu_ptr[cpu_index].run_queue.normal_queue;

	g_machine.cpu_ptr[cpu_index].run_queue.normal_queue = aux;

	pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].queue_mutex);



}

