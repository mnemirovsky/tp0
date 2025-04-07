#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#define PUERTO "4444"

typedef enum
{
	MENSAJE,
	PAQUETE,
}op_code;

typedef enum 
{
	EXIT_CODE_FAILED_GETTING_ADDR_INFO,
	EXIT_CODE_FAILED_CREATING_SOCKET,
	EXIT_CODE_FAILED_SETTING_SOCKET_OPTION,
	EXIT_CODE_FAILED_BINDING_ADDRESS,
	EXIT_CODE_FAILED_LISTENING_TO_ADDRESS,
	EXIT_CODE_FAILED_ACCEPTING_CLIENT
}errors_sv;


extern t_log* logger;

void* recibir_buffer(int*, int);

int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

#endif /* UTILS_H_ */
