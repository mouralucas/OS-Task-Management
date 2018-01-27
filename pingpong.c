//P2 Gestão de tarefas


//Lucas Penha de Moura		1208977
//Jociane franzoni de Lima	1195409

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

task_t maintask, *atual, *ant;
int cont = 0;//incrementa a id da tarefa


void pingpong_init (){
		setvbuf (stdout, 0, _IONBF, 0);
		maintask.id = 0;
	    maintask.prev = NULL;
		maintask.next = NULL;
		atual = &maintask;// endereço da tarefa atual
} // fim de pingpong_init



int task_create (task_t *task, void (*start_routine)(void *),  void *arg){
	(*task).id = cont++;//define o id da tarefa atual
	char *stack; 
	stack = malloc(32768); //define o tamanho da pilha a ser usada
	getcontext(&task->cont); //pega o contexto da tarefa atual
	
	if(stack){
		task->cont.uc_stack.ss_sp = stack ;
     	task->cont.uc_stack.ss_size = 32768;
      	task->cont.uc_stack.ss_flags = 0;
      	task->cont.uc_link = 0;
	}else{
		printf("A pilha nao foi criada");
		exit(1);
	}
	
	makecontext (&((*task).cont),(void *) (*start_routine), 1, arg); //cria o novo contexto
	
	printf("Tarefa %d criada \n", (*task).id);
	
	return (*task).id;
	

}// fim de task_create

int task_switch (task_t *task){

        task_t *ant=atual;
        atual =task;
	    #ifdef DEBUG
  	    printf("Troca de contexto de %d para %d \n" , (*ant).id , (*atual).id);
	    #endif
        swapcontext (&(*ant).cont,&(*atual).cont);
	
	return 0;
}// fim de task_switch

void task_exit (int exit_code){
		task_switch(&maintask);
}// fim de task_exit

int task_id (){
	return atual->id;
}// fim de task_id
