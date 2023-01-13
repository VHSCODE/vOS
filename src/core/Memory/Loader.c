#include "Loader.h"
#include "time.h"

#include "stdio.h"
#include "stdlib.h"


#include "string.h"
void Emit_Process(u32 cpu_index);
void Read_Binary(char* filename,struct pcb *pcb);

void Loader_Routine(struct Timer *timer){

    srand(time(NULL));
    

    while(g_machine.is_running){


        pthread_mutex_lock(&timer->timer_mutex);
        pthread_cond_wait(&timer->timer_tick,&timer->timer_mutex);

        // if tick del Timer, entonces creamos un proceso con Emit_Process
        
        //printf("Generando proceso\n");

        Emit_Process(timer->cpu_index);
        pthread_mutex_unlock(&timer->timer_mutex);

    }
    free(timer);
}


void Emit_Process(u32 cpu_index){
	struct pcb* pcb = malloc(sizeof (struct pcb));

    pcb->pid = rand() % INT16_MAX;
    pcb->priority = rand() % PRIORITY_COUNT;

    pcb->counting.cycles_allowed = MAX_CYCLES_ALLOWED;
    pcb->counting.cpu_cycles_run = 0;

    char filename[50];
    const char* tail = ".elf";

    strcpy(filename,"prog");
    char buffer[10];
    //Get a random binary and get the segments

    u32 rand_number = rand() % (g_machine.specs.program_count);

    sprintf(buffer,"%03d",rand_number); //This might cause a buffer overflow ?

    strcat(filename,buffer);
    strcat(filename,tail);


    pcb->mm.code_size = 0;
    pcb->mm.data_size = 0;

    strcpy(pcb->filename,filename);

    Read_Binary(filename, pcb);
    //Allocate the page table and copy to physical memory
    Allocate_Page_Table_and_copy(cpu_index, pcb);

    append_to_normal_queue(pcb, cpu_index);
}


void Read_Binary(char* filename,struct pcb *pcb){
    FILE *fp;
    char *line = NULL;

    size_t len = 0;
    ssize_t read;
    int lineindex = 0;
    int data_start = 0;

    u32 code_index = 0;
    u32 data_index = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error while opening %s. Aborting\n",filename);
        exit(EXIT_FAILURE);
    }
    //Read line by line and parse the data
    while ((read = getline(&line, &len, fp)) != -1) {
        if (lineindex == 0){ //.text address start

            line+= 6; //Move pointer to the start of the value
            pcb->state.proc_program_counter = strtoul(line,NULL,16);
        }
        else if(lineindex == 1){ //.data address start

        }
        else{

            if(strcmp(line,"F0000000\n") == 0){
                pcb->mm.code[code_index] = strtoul(line,NULL,16);
                pcb->mm.code_size++;
                data_start= 1;
            }
            else if(!data_start){
                pcb->mm.code[code_index] = strtoul(line,NULL,16);
                pcb->mm.code_size++;
                code_index++;
            }
            else{
                pcb->mm.data[data_index] = strtoul(line,NULL,16);
                pcb->mm.data_size++;
                data_index++;
            }
        }
        lineindex++;
    }
    fclose(fp);
}