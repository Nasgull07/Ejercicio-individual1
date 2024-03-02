#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

MYSQL *conn;

void conectarBD() {

	conn = mysql_init(NULL);
	if (conn==NULL) {
	printf ("Error al crear la conexion: %u %s\n",
	mysql_errno(conn), mysql_error(conn));
	exit (1);
	}

	conn = mysql_real_connect (conn, "localhost","root", "mysql", NULL, 0, NULL, 0);
	if (conn==NULL)
	{
	printf ("Error al inicializar la conexion: %u %s\n",
	mysql_errno(conn), mysql_error(conn));
	exit (1);
	}
}

void ejecutarConsulta(char *sql) {
	if (mysql_query(conn, sql) != 0) {
		fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}
}

int main() {
	conectarBD();
	
	// Ejecutar la consulta para crear la base de datos y las tablas
	ejecutarConsulta("DROP DATABASE IF EXISTS JUEGO");
	ejecutarConsulta("CREATE DATABASE JUEGO");
	ejecutarConsulta("USE JUEGO");
	ejecutarConsulta("CREATE TABLE jugadores(ID integer primary key not null, NOMBRE varchar(20) not null, CONTRASEÑA varchar(20)not null, PUNTOS integer, VICTORIAS integer)");
	ejecutarConsulta("CREATE TABLE partidas(ID integer primary key not null, FECHA varchar(20), TIEMPO time, JUGADORES integer)");
	ejecutarConsulta("CREATE TABLE relacionjp (ID integer primary key not null, ID_J integer not null, foreign key (ID_J) references jugadores(ID), ID_P integer not null, foreign key (ID_P) references partidas(ID))");
	
	// Insertar datos en las tablas
	ejecutarConsulta("INSERT INTO jugadores (ID, NOMBRE, CONTRASEÑA, PUNTOS, VICTORIAS) VALUES (1, 'Jugador1', 'contraseña1', 100, 10), (2, 'Jugador2', 'contraseña', 200, 15), (3, 'Jugador3', 'contraseña3', 150, 12)");
	ejecutarConsulta("INSERT INTO partidas (ID, FECHA, TIEMPO, JUGADORES) VALUES (1, '03-01', '12:00:00', 3), (2, '03-02', '15:30:00', 2), (3, '03-03', '18:45:00', 1)");
	ejecutarConsulta("INSERT INTO relacionjp (ID, ID_J, ID_P) VALUES (1, 1, 1), (2, 2, 1)");
	

	// consulta

	ejecutarConsulta("SELECT DISTINCT jugadores.NOMBRE FROM (jugadores, partidas, relacionjp) WHERE relacionjp.ID_P = '1' AND relacionjp.ID_J = jugadores.ID");
	
	mysql_close(conn);
	return 0;
}


