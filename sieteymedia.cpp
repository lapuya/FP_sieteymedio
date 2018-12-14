/*
Grupo F
Beatriz Álvarez de Arriba
Laurence Apuya Pangilinan
*/

#include <iostream>
#include <fstream> // Biblioteca de los ficheros
#include <string>
// Bibliotecas para calcular un numero aleatorio
#include <ctime>
#include <cstdlib>
#include <algorithm> // Biblioteca para mezclar las cartas aleatoriamente
using namespace std;

const int NUM_CARTAS = 40;
const int NUM_PROBABILIDAD = 8;

typedef struct
{
	int numero;
}tCarta;

typedef tCarta tArrayCartas[NUM_CARTAS];

typedef struct
{
	int cont;
	tArrayCartas array_cartas;
}tMazo;

typedef struct
{
	int cont;
	float valorMano;
	tArrayCartas array_cartas;
}tMano;

typedef int tProbabilidad[NUM_PROBABILIDAD];

//FUNCIONES PRINCIPALES
void mostrarMenu();
int menu();
void jugarModoA(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta);
void jugarModoB(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta);
void jugarModoC(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta, tProbabilidad array_probabilidad);
void jugarModoD(tMazo &mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta &carta, tProbabilidad array_probabilidad);
bool cargarMazo(tMazo & mazo);
bool determinarGanador(float valorJugador, float valorOrdenador);
bool determinarGanadorD(tMano & manoJugador, tMano & manoOrdenador);
bool guardarPartida(string titulo, tMano manoJugador, tMano manoOrdenador, bool ganaJugador, int opcion);

//FUNCIONES SECUNDARIAS
void leerCartas(ifstream & entrada, tMazo & mazo);
void crearMano(tMazo & mazo, tMano & mano, tCarta & carta, int num);
void inicializarMano(tMano & mano);
void sacarCarta(tMazo & mazo, tCarta & carta);
void eliminarCartaMazo(tMazo & mazo, int pos);
void asignarCarta(tMano & mano, tCarta & carta);
void mostrarMano(const tMano & mano);
float sumarValorCarta(tMano & mano);
void robarJugador(tMazo & mazo, tMano & mano, tCarta & carta, int max_cartas);
string seguirRobando();
bool comprobacionRespuesta(string respuesta);
void robarOrdenador(tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, int max_cartas);
void robarJugadorCD(tMazo & mazo, tMano & mano, tCarta & carta);
void sacarYAsignarCarta(tMazo &mazo, tMano &mano, tCarta &carta);
void robarOrdenadorCD(tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, tProbabilidad array_probabilidad);
bool calcularProbabilidad(tMazo & mazo, tProbabilidad array_probabilidad, tMano & manoJugador);
void crearMazo(tMazo & mazo);

int main()
{
	srand(time(NULL)); // Semilla que ponemos para generar numeros aleatorios
	// Variables
	tMazo mazo;
	tMano manoJugador, manoOrdenador;
	tCarta carta;
	tProbabilidad array_probabilidad;
	int opcion, numPartidas = 0;
	bool ganaJugador = false, seHaPodidoJugar;

	opcion = menu(); // Mostramos el menu

	while (opcion != 5) // Mientras que no pulse salir seguimos ejecutando
	{
		seHaPodidoJugar = false;
		if (opcion == 1 && cargarMazo(mazo)) // Modo A
		{
			jugarModoA(mazo, manoJugador, manoOrdenador, carta);
			seHaPodidoJugar = true;
		}
		else if (opcion == 2 && cargarMazo(mazo)) // Modo B
		{
			jugarModoB(mazo, manoJugador, manoOrdenador, carta);
			seHaPodidoJugar = true;
		}
		else if (opcion == 3 && cargarMazo(mazo)) // Modo C
		{
			jugarModoC(mazo, manoJugador, manoOrdenador, carta, array_probabilidad);
			seHaPodidoJugar = true;
		}
		else if (opcion == 4) // Modo D
		{
			jugarModoD(mazo, manoJugador, manoOrdenador, carta, array_probabilidad);
			seHaPodidoJugar = true;
		}
		else
			cout << "No se ha podido jugar" << endl;

		// Determinamos ganador
		if (seHaPodidoJugar) {
			numPartidas++;
			if (manoJugador.valorMano <= 7.5) // Si el jugador se pasa de 7.5 directamente se acaba la partida
			{
				if (opcion == 4)
					ganaJugador = determinarGanadorD(manoJugador, manoOrdenador);
				else
					ganaJugador = determinarGanador(manoJugador.valorMano, manoOrdenador.valorMano);
			}
			else
			{
				cout << "Gana el ordenador" << endl;
				manoOrdenador.valorMano = 0;
			}

			// Guardamos la partida en fichero
			string aCadena = to_string(numPartidas); // Convertimos el numero a string, para poder poner el nombre del fichero
			if (guardarPartida(aCadena, manoJugador, manoOrdenador, ganaJugador, opcion))
				cout << "Partida guardada" << endl;

			cout << endl;
		}

		opcion = menu(); // Volvemos a preguntar
	}

	return 0;
}

void mostrarMenu()
{
	// Sacamos por pantalla todas las opciones del menu
	cout << "1- Modo A" << endl;
	cout << "2- Modo B" << endl;
	cout << "3- Modo C" << endl;
	cout << "4- Modo D" << endl;
	cout << "5- Finalizar" << endl;
	cout << "Elija opcion: ";
}

int menu()
{
	int opcion;

	mostrarMenu();
	cin >> opcion;
	while (cin.fail() || opcion < 1 || opcion > 5)
	{
		if (cin.fail()) // Introducimos un caracter (distinto de numero)
		{
			cin.clear();
			cin.ignore(10, '\n'); // Ignora hasta 10 caracteres siguientes en la misma linea
			cout << "No es un numero" << endl;
		}
		else
			cout << "Numero no valido" << endl;

		mostrarMenu();
		cin >> opcion;
	}

	return opcion;
}

bool cargarMazo(tMazo & mazo)
{
	bool abierto;
	ifstream entrada;
	string nombre_archivo;

	cout << "Introduce el nombre del archivo: "; // Se pide por pantalla el nombre del archivo
	cin >> nombre_archivo;

	entrada.open(nombre_archivo);
	abierto = entrada.is_open();
	if (abierto)  // Si el archivo se ha conseguido abrir, leemos cartas
		leerCartas(entrada, mazo);
	else
		cout << "Hubo problemas al abrir el archivo o no existe." << endl;

	entrada.close();

	return abierto;
}

void leerCartas(ifstream & entrada, tMazo & mazo)
{
	string basura;

	for (int i = 0; i < NUM_CARTAS; i++)
	{
		entrada >> mazo.array_cartas[i].numero;
		getline(entrada, basura); // Para saltar la fila
	}
	mazo.cont = NUM_CARTAS; // Numero de cartas = 40
}

void jugarModoA(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta)
{
	int num = 3 + rand() % (6 - 3); // Calcula el numero de cartas (aleatoriamente) que tienen que robar los jugadores
	cout << "El numero aleatorio es: " << num << endl;

	cout << "------TURNO JUGADOR------" << endl;
	crearMano(mazo, manoJugador, carta, num);
	cout << "La mano es: ";
	mostrarMano(manoJugador);
	cout << endl;
	manoJugador.valorMano = sumarValorCarta(manoJugador);
	cout << "El valor de la mano es: " << manoJugador.valorMano << endl;

	if (manoJugador.valorMano <= 7.5) // Si el jugador se pasa, se acaba la partida automaticamente
	{
		// Creamos la mano del ordenador
		cout << "------TURNO ORDENADOR------" << endl;
		crearMano(mazo, manoOrdenador, carta, num);
		cout << "La mano es: ";
		mostrarMano(manoOrdenador);
		cout << endl;
		manoOrdenador.valorMano = sumarValorCarta(manoOrdenador);
		cout << "El valor de la mano es: " << manoOrdenador.valorMano << endl;
	}
}

void crearMano(tMazo & mazo, tMano & mano, tCarta & carta, int num)
{
	inicializarMano(mano);

	for (int i = 0; i < num; i++) // Sacamos tantas veces el numero aleatorio
	{
		sacarCarta(mazo, carta);
		asignarCarta(mano, carta);
	}
}

void inicializarMano(tMano & mano)
{
	// Para saber cuantas cartas tiene en la mano y su valor
	mano.cont = 0;
	mano.valorMano = 0;
}

void sacarCarta(tMazo & mazo, tCarta & carta)
{
	int i = 0;

	if (mazo.cont != 0)
	{
		carta = mazo.array_cartas[i];
		eliminarCartaMazo(mazo, i);	// Borramos del mazo la carta sacada
	}
}

void eliminarCartaMazo(tMazo & mazo, int pos)
{
	for (int i = pos + 1; i < mazo.cont; i++)
		mazo.array_cartas[i - 1] = mazo.array_cartas[i]; // Movemos todas las cartas a la posicion anterior
	mazo.cont--;
}

void asignarCarta(tMano & mano, tCarta & carta)
{
	mano.array_cartas[mano.cont] = carta; // Lo introducimos en la mano del jugador
	mano.cont++;
}

void mostrarMano(const tMano & mano)
{
	for (int i = 0; i < mano.cont; i++)
		cout << mano.array_cartas[i].numero << " ";
}

float sumarValorCarta(tMano & mano)
{
	float suma = 0;

	for (int i = 0; i < mano.cont; i++)	// Sumamos el valor de toda la mano
	{	// 10, 11 y 12 valen 0.5
		if (mano.array_cartas[i].numero == 10 || mano.array_cartas[i].numero == 11 || mano.array_cartas[i].numero == 12)
			suma = suma + 0.5;
		else
			suma = suma + mano.array_cartas[i].numero;
	}

	return suma;
}

bool determinarGanador(float valorJugador, float valorOrdenador)
{
	int num, resta1, resta2;
	bool ganaJugador = false;

	if (valorJugador == valorOrdenador)
	{
		cout << "Habeis empatado, se elegira a un ganador de forma aleatoria: " << endl;
		num = 1 + rand() % (3 - 1); // Elige un ganador de manera aleatoria
		if (num == 1)
		{
			cout << "Gana el jugador" << endl;
			ganaJugador = true;
		}
		else
			cout << "Gana el ordenador" << endl;
	}
	else 		// Si no han empatado
	{
		if (valorJugador < valorOrdenador && valorOrdenador > 7.5) 	// Si el ordenador se pasa y el jugador no
		{
			cout << "Gana el jugador" << endl;
			ganaJugador = true;
		}
		else
		{
			// Saber quien se ha acercado mas a 7.5
			resta1 = 7.5 - valorJugador;
			resta2 = 7.5 - valorOrdenador;
			if (resta1 < resta2)
			{
				cout << "Gana el jugador" << endl;
				ganaJugador = true;
			}
			else
				cout << "Gana el ordenador" << endl;
		}
	}

	return ganaJugador;
}

void jugarModoB(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta)
{
	int max_cartas = 3 + rand() % (6 - 3); // Calcula el numero de cartas (aleatoriamente) que tienen que robar los jugadores
	cout << "El numero aleatorio es: " << max_cartas << endl;

	cout << "------TURNO JUGADOR------" << endl;
	robarJugador(mazo, manoJugador, carta, max_cartas);		// Pueden elegir si robar o no
	cout << "La mano es: ";
	mostrarMano(manoJugador);
	cout << endl;
	manoJugador.valorMano = sumarValorCarta(manoJugador);
	cout << "El valor de la mano es: " << manoJugador.valorMano << endl;
	if (manoJugador.valorMano <= 7.5)
	{
		cout << "------TURNO ORDENADOR------" << endl;
		robarOrdenador(mazo, manoOrdenador, manoJugador, carta, max_cartas);
		cout << "La mano es: ";
		mostrarMano(manoOrdenador);
		cout << endl;
		manoOrdenador.valorMano = sumarValorCarta(manoOrdenador);
		cout << "El valor de la mano es: " << manoOrdenador.valorMano << endl;
	}
}

void robarJugador(tMazo & mazo, tMano & mano, tCarta & carta, int max_cartas)
{
	string respuesta;
	bool correcto;

	inicializarMano(mano);
	sacarYAsignarCarta(mazo, mano, carta); // Funcion que permite sacar del mazo
	respuesta = seguirRobando();

	while (mano.cont < max_cartas && comprobacionRespuesta(respuesta) && mano.valorMano < 7.5) // Comprobamos si la respuesta es valida
	{
		sacarYAsignarCarta(mazo, mano, carta);
		if (mano.valorMano < 7.5)
			respuesta = seguirRobando();
		else
			cout << "Te has pasado de 7.5" << endl;
	}
}

string seguirRobando()
{
	string respuesta;

	cout << "Quieres seguir robando? (si/no): ";
	cin >> respuesta;

	return respuesta;
}

bool comprobacionRespuesta(string respuesta)
{
	bool correcto = true;

	// Comprobamos si la respuesta es correcta dando todas las combinaciones posibles

	while (respuesta != "si" && respuesta != "Si" && respuesta != "SI" && respuesta != "no" && respuesta != "No" && respuesta != "NO")
	{
		cout << "Respuesta erronea" << endl;
		respuesta = seguirRobando();
	}
	if (respuesta == "no" || respuesta == "No" || respuesta == "NO")
		correcto = false;

	return correcto;
}

void robarOrdenador(tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, int max_cartas)
{
	inicializarMano(mano);
	sacarYAsignarCarta(mazo, mano, carta);

	while (mano.cont < max_cartas && mano.valorMano <= manoJugador.valorMano) // Se planta cuando su puntuacion supere a la del humano
		sacarYAsignarCarta(mazo, mano, carta);

}

void jugarModoC(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta, tProbabilidad array_probabilidad)
{
	cout << "------TURNO DEL JUGADOR------" << endl;
	robarJugadorCD(mazo, manoJugador, carta);
	cout << "La mano es: ";
	mostrarMano(manoJugador);
	cout << endl;
	cout << "La puntuacion es: " << manoJugador.valorMano << endl;
	if (manoJugador.valorMano <= 7.5)
	{
		cout << "------TURNO DEL ORDENADOR------" << endl;
		robarOrdenadorCD(mazo, manoOrdenador, manoJugador, carta, array_probabilidad);
		cout << "La mano es: ";
		mostrarMano(manoOrdenador);
		cout << endl;
		cout << "La puntuacion es: " << manoOrdenador.valorMano << endl;
	}
}

void robarJugadorCD(tMazo & mazo, tMano & mano, tCarta & carta)
{
	string respuesta;

	inicializarMano(mano);
	sacarYAsignarCarta(mazo, mano, carta);
	respuesta = seguirRobando();

	while (mano.cont < mazo.cont && comprobacionRespuesta(respuesta)) // Comprobamos si la respuesta es valida
	{
		sacarYAsignarCarta(mazo, mano, carta);
		respuesta = seguirRobando();
	}
}

void sacarYAsignarCarta(tMazo &mazo, tMano &mano, tCarta &carta) {

	// Sacamos, asignamos, mostramos la mano y sumamos los valores de la mano
	sacarCarta(mazo, carta);
	asignarCarta(mano, carta);
	cout << "La mano es: ";
	mostrarMano(mano);
	cout << endl;
	mano.valorMano = sumarValorCarta(mano);
	cout << "La puntuacion es: " << mano.valorMano << endl;
}

void robarOrdenadorCD(tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, tProbabilidad array_probabilidad)
{
	inicializarMano(mano);
	sacarYAsignarCarta(mazo, mano, carta);
	if (manoJugador.valorMano <= 7.5)
	{
		while (mano.cont < mazo.cont && mano.valorMano <= manoJugador.valorMano)
			sacarYAsignarCarta(mazo, mano, carta);

		if (mano.valorMano == manoJugador.valorMano)
		{
			if (calcularProbabilidad(mazo, array_probabilidad, manoJugador)) // Empate: probabilidad de pasarse supera el 50%
				sacarYAsignarCarta(mazo, mano, carta);
		}
	}
}

bool calcularProbabilidad(tMazo & mazo, tProbabilidad array_probabilidad, tMano & manoJugador)
{
	float probabilidad = 0, suma = 0;
	int resta = 0;
	// Creamos un array con los tipos de cartas
	for (int i = 0; i < 8; i++)
		array_probabilidad[i] = 0;
	// Leemos las cartas que quedan en la mazo y las metemos en el array de probabilidad
	for (int i = 0; i < mazo.cont; i++)
	{
		if (mazo.array_cartas[i].numero == 10 || mazo.array_cartas[i].numero == 11 || mazo.array_cartas[i].numero == 12)
			array_probabilidad[0] = array_probabilidad[0] + 1;
		else if (mazo.array_cartas[i].numero == 1)
			array_probabilidad[1] = array_probabilidad[1] + 1;
		else if (mazo.array_cartas[i].numero == 2)
			array_probabilidad[2] = array_probabilidad[2] + 1;
		else if (mazo.array_cartas[i].numero == 3)
			array_probabilidad[3] = array_probabilidad[3] + 1;
		else if (mazo.array_cartas[i].numero == 4)
			array_probabilidad[4] = array_probabilidad[4] + 1;
		else if (mazo.array_cartas[i].numero == 5)
			array_probabilidad[5] = array_probabilidad[5] + 1;
		else if (mazo.array_cartas[i].numero == 6)
			array_probabilidad[6] = array_probabilidad[6] + 1;
		else
			array_probabilidad[7] = array_probabilidad[7] + 1;
	}

	resta = 7.5 - manoJugador.valorMano; // Calculamos desde que cartas tenemos que hacer la probabilidad
	for (int j = resta + 1; j < 8; j++)
		suma = suma + array_probabilidad[j]; // Sumamos desde la posicion valida hasta el final
	probabilidad = suma / mazo.cont; // Calculamos la probabilidad
	cout << "La probabilidad es: " << probabilidad << endl;

	return probabilidad < 0.50;
}

void jugarModoD(tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta, tProbabilidad array_probabilidad)
{
	crearMazo(mazo);
	cout << "------TURNO JUGADOR------" << endl;
	robarJugadorCD(mazo, manoJugador, carta);
	if (manoJugador.valorMano <= 7.5)
	{
		cout << "------TURNO ORDENADOR------" << endl;
		robarOrdenadorCD(mazo, manoOrdenador, manoJugador, carta, array_probabilidad);
	}
}

void crearMazo(tMazo & mazo)
{
	// Creamos el mazo con varios bucles for con la cantidad de cartas de cada tipo
	for (int i = 0; i < 4; i++)
		mazo.array_cartas[i].numero = 1;
	for (int i = 4; i < 8; i++)
		mazo.array_cartas[i].numero = 2;
	for (int i = 8; i < 12; i++)
		mazo.array_cartas[i].numero = 3;
	for (int i = 12; i < 16; i++)
		mazo.array_cartas[i].numero = 4;
	for (int i = 16; i < 20; i++)
		mazo.array_cartas[i].numero = 5;
	for (int i = 20; i < 24; i++)
		mazo.array_cartas[i].numero = 6;
	for (int i = 24; i < 28; i++)
		mazo.array_cartas[i].numero = 7;
	for (int i = 28; i < 32; i++)
		mazo.array_cartas[i].numero = 10;
	for (int i = 32; i < 36; i++)
		mazo.array_cartas[i].numero = 11;
	for (int i = 36; i < NUM_CARTAS; i++)
		mazo.array_cartas[i].numero = 12;

	mazo.cont = NUM_CARTAS;
	random_shuffle(&(mazo.array_cartas[0]), &(mazo.array_cartas[NUM_CARTAS])); // Mezcla las cartas aleatoriamente
}

bool determinarGanadorD(tMano & manoJugador, tMano & manoOrdenador)
{
	int num, resta1, resta2;
	bool ganaJugador = false;
	if (manoJugador.valorMano == manoOrdenador.valorMano) // Si empatan, gana quien tenga menos cartas en la mano
	{
		if (manoJugador.cont < manoOrdenador.cont)
		{
			cout << "Habeis empatado. Gana el jugador porque tiene menos cartas en la mano." << endl;
			ganaJugador = true;
		}
		else if (manoOrdenador.cont < manoJugador.cont)
			cout << "Habeis empatado. Gana el ordenador porque tiene menos cartas en la mano." << endl;
		else
		{
			cout << "Habeis empatado, se elegira a un ganador de forma aleatoria: " << endl; // Aleatorio si tienen el mismo numero de cartas
			num = 1 + rand() % (3 - 1);
			if (num == 1)
			{
				cout << "Gana el jugador" << endl;
				ganaJugador = true;
			}
			else
				cout << "Gana el ordenador" << endl;
		}
	}
	else
	{
		if (manoJugador.valorMano < manoOrdenador.valorMano && manoOrdenador.valorMano > 7.5)
		{
			cout << "Gana el jugador" << endl;
			ganaJugador = true;
		}
		else
		{
			// Gana quien se acerque mas a 7.5
			resta1 = 7.5 - manoJugador.valorMano;
			resta2 = 7.5 - manoOrdenador.valorMano;
			if (resta1 < resta2)
			{
				cout << "Gana el jugador" << endl;
				ganaJugador = true;
			}
			else
				cout << "Gana el ordenador" << endl;
		}
	}

	return ganaJugador;
}

bool guardarPartida(string titulo, tMano manoJugador, tMano manoOrdenador, bool ganaJugador, int opcion)
{
	ofstream salida;

	salida.open(titulo);
	bool abierto = salida.is_open();
	if (abierto)
	{
		// Escribimos el modo en el que hemos jugado
		if (opcion == 1)
			salida << "Modo A" << endl;
		else if (opcion == 2)
			salida << "Modo B" << endl;
		else if (opcion == 3)
			salida << "Modo C" << endl;
		else
			salida << "Modo D" << endl;
		// Escribimos las cartas y la puntuacion de cada uno de los jugadores que han jugado
		salida << "Las cartas del jugador fueron: ";
		for (int i = 0; i < manoJugador.cont; i++)
			salida << manoJugador.array_cartas[i].numero << " ";
		salida << endl;
		salida << "La puntuacion del jugador fue: " << manoJugador.valorMano << endl;
		salida << "Las cartas del ordenador fueron: ";
		for (int i = 0; i < manoOrdenador.cont; i++)
			salida << manoOrdenador.array_cartas[i].numero << " ";
		salida << endl;
		salida << "La puntuacion del ordenador fue: " << manoOrdenador.valorMano << endl;
		// Escribimos quien gano en la partida en el Modo D
		if (opcion == 4)
		{
			//Escribimos por qué gana quien gana (de forma aleatoria)
			if (manoOrdenador.valorMano == manoJugador.valorMano && manoOrdenador.cont > manoJugador.cont)
				salida << "Gano el jugador, porque tenia menos cartas en la mano";
			else if (manoOrdenador.valorMano == manoJugador.valorMano && manoJugador.cont > manoOrdenador.cont)
				salida << "Gano el ordenador, porque tenia menos cartas en la mano";
			else if (manoOrdenador.valorMano == manoJugador.valorMano && manoJugador.cont == manoOrdenador.cont && ganaJugador)
				salida << "Habeis empatado, gano el jugador por eleccion aleatoria";
			else if (manoOrdenador.valorMano == manoJugador.valorMano && manoJugador.cont == manoOrdenador.cont && !ganaJugador)
				salida << "Habeis empatado, gano el ordenador por eleccion aleatoria";
			else if (manoJugador.valorMano > 7.5)
				salida << "Gano el ordenador porque el jugador se ha pasado de 7.5";
			else if (ganaJugador)
				salida << "Gano el jugador";
			else
				salida << "Gano el ordenador";
		}
	// Escribimos quien gana la partida en los otros modos
		else
		{
			if (manoJugador.valorMano > 7.5)
				salida << "Gano el ordenador porque el jugador se ha pasado de 7.5";
			else if (ganaJugador)
				salida << "Gano el jugador";
			else
				salida << "Gano el ordenador";
		}
	}
	salida.close();

	return abierto;
}
