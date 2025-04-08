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

	log_info(logger, config_get_string_value(config,"IP"));
	log_info(logger, config_get_string_value(config, "PUERTO"));
	log_info(logger, config_get_string_value(config, "CLAVE"));
	
	
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

void leer_consola(t_log* logger)
{
	char* leido;

	while (1) {
		leido = readline("> ");

		if (leido) {
			log_info(logger, leido);
		}

		if (strcmp(leido, "") == 0) {
			free(leido);
			break;
		}

		printf("%s\n", leido);
		free(leido);
	}
	
}



void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	config_destroy(config);
}
