#include "Machine.h"
#include <stdlib.h>
#include "signal.h"
#include "stdio.h"


void start_shutdown(int dummy);
void read_specs(struct machine_specs *specs);



int main(int argc, char **argv)
{
    srand(time(0));

    //Read specs from file
    struct machine_specs specs;
    read_specs(&specs);



	Init_Machine(specs); //Start the machine

    signal(SIGINT,start_shutdown);

    while (g_machine.is_running)
    {

    }


    Deinit_Machine();

    return 0;
}


void start_shutdown(int dummy){

    printf("Starting shutdown...\n");
    g_machine.is_running = 0;
}


/*
 * Specs.txt format
 * 1st line = cpu count
 * 2nd line = core count per cpu
 * 3rd line = scheduler execution frequency
 * 4th line = loader execution frequency
 * 5th line = .elf program count in folder
 */
void read_specs(struct machine_specs *specs){
    char* specs_file = "specs.txt";
    FILE *fp;
    char *line = NULL;

    size_t len = 0;
    ssize_t read;
    int lineindex = 0;

    fp = fopen(specs_file, "r");
    if (fp == NULL)
    {
        printf("Could not open specs file. Please place the specs.txt file in the same folder as the executable.\n");
        exit(EXIT_FAILURE);
    }

    //Read line by line and parse the data
    while ((read = getline(&line, &len, fp)) != -1) {
        switch(lineindex){
        case 0: //cpu_count
            specs->cpu_count = strtoul(line,NULL,10);
            break;
        case 1:
            specs->core_count = strtoul(line,NULL,10);
            break;
        case 2:
            specs->sched_frec = strtoul(line,NULL,10);
            break;
        case 3:
            specs->procgnr_frec  = strtoul(line,NULL,10);
            break;
        case 4:
            specs->program_count = strtoul(line,NULL,10);

            fclose(fp);

            return; //Stop parsing the file
        default:
            break;
        }
        lineindex++;
    }
}