#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip		=	config_get_string_value(config, "IP");
	puerto 	=	config_get_string_value(config, "PUERTO");
	valor 	=	config_get_string_value(config, "CLAVE");

	log_info(logger, "%s", ip);
	log_info(logger, "%s", puerto);
	log_info(logger, "%s", valor);
	
	
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */
	leer_consola(logger);
	/*-------
	--------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	int	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	t_paquete *paquete = crear_paquete();

	leer_consola(paquete);

	enviar_paquete(paquete, conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("logger.log", "LOGGER_TP0", 1, LOG_LEVEL_INFO);
	if (nuevo_logger == NULL) {
		perror("Algo raro pasó con el log. No se pudo crear o encontrar el archivo.");
		exit(EXIT_FAILURE);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");
	
	if (nuevo_config == NULL) {
		abort();
	};

	return nuevo_config;
}

void leer_consola(t_paquete* paquete)
{
	char* leido;
	#include <string.h>
#include <stdlib.h>

void leer_consola(t_paquete* paquete) {
    char* leido;

    // Inicializa el buffer del paquete si no está inicializado
    if (paquete->buffer == NULL) {
        paquete->buffer = malloc(sizeof(t_buffer));
        paquete->buffer->size = 0;
        paquete->buffer->stream = malloc(1); // Inicializa con un tamaño mínimo
        paquete->buffer->stream[0] = '\0';
    }

    while (1) {
        leido = readline("> ");

        if (leido) {
            log_info(logger, "%s", leido);

            // Verificar si la línea está vacía
            if (strcmp(leido, "") == 0) {
                free(leido);
                break;
            }

            // Concatenar la línea al buffer del paquete
            paquete->buffer->size += strlen(leido) + 1; // +1 para el '\0'
            paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size);
            strcat(paquete->buffer->stream, leido);
            strcat(paquete->buffer->stream, "\n"); // Agrega un salto de línea

            free(leido);
        }
    }

    // Configura el código de operación del paquete
    paquete->codigo_operacion = OP_CODE_CONSOLA; // Ajusta según sea necesario
}

/*
	while (1) {
		leido = readline("> ");

		if (leido) {
			log_info(logger, "%s", leido);
		}

		if (strcmp(leido, "") == 0) {
			free(leido);
			break;
		}

		printf("%s\n", leido);
		free(leido);
	}
*/	
}



void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	config_destroy(config);
}
