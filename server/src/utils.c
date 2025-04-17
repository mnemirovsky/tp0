#include"utils.h"

t_log* logger;

int iniciar_servidor(void)
{

	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status;

	status = getaddrinfo(NULL, PUERTO, &hints, &servinfo);
	if (status != 0) {
		printf("Error in getaddrinfo");
		exit(EXIT_CODE_FAILED_GETTING_ADDR_INFO); 
	}

	// Creamos el socket de escucha del servidor

	socket_servidor = socket(servinfo->ai_family,
                        servinfo->ai_socktype,
                        servinfo->ai_protocol);

	if (socket_servidor == -1) {
		exit(EXIT_CODE_FAILED_CREATING_SOCKET);
	}

	// Asociamos el socket a un puerto

	status = setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

	if (status != 0) {
		exit(EXIT_CODE_FAILED_SETTING_SOCKET_OPTION);
	}

	status = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	if (status != 0) {
		exit(EXIT_CODE_FAILED_BINDING_ADDRESS);
	}

	// Escuchamos las conexiones entrantes

	status = listen(socket_servidor, SOMAXCONN);

	if (status != 0) {
		exit(EXIT_CODE_FAILED_LISTENING_TO_ADDRESS);
	}
	
	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{


	int socket_cliente = accept(socket_servidor, NULL, NULL);

	if (socket_cliente == -1) {
		exit(EXIT_CODE_FAILED_ACCEPTING_CLIENT);
	}
	
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

