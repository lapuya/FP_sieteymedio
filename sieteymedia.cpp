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

typedef tCarta tArrayCartas [NUM_CARTAS];

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

typedef int tProbabilidad [NUM_PROBABILIDAD];

void mostrarMenu ();
int menu ();
bool cargarMazo (tMazo & mazo);
void leerCartas (ifstream & entrada, tMazo & mazo);
void jugarModoA (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta);
void crearMano (tMazo & mazo, tMano & mano, tCarta & carta, int num);
void inicializarMano (tMano & mano);
void sacarCarta (tMazo & mazo, tCarta & carta);
void eliminarCartaMazo (tMazo & mazo, int pos);
void asignarCarta (tMano & mano, tCarta & carta);
void mostrarMano (const tMano & mano);
float sumarValorCarta (tMano & mano);
bool determinarGanador(float valorJugador, float valorOrdenador);
void jugarModoB (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta);
void robarJugador (tMazo & mazo, tMano & mano, tCarta & carta, int max_cartas);
bool comprobacionRespuesta (string respuesta);
void robarOrdenador (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, int max_cartas);
void jugarModoC (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta, tProbabilidad array_probabilidad);
void robarJugadorC (tMazo & mazo, tMano & mano, tCarta & carta);
void robarOrdenadorC (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, tProbabilidad array_probabilidad);
float calcularProbabilidad (tMazo & mazo, tProbabilidad array_probabilidad, tMano & manoJugador);
void jugarModoD (tMazo &mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta &carta, tProbabilidad array_probabilidad);
void crearMazo (tMazo & mazo);
bool determinarGanadorD(tMano & manoJugador, tMano & manoOrdenador);
bool guardarPartida (string titulo, tMano manoJugador, tMano manoOrdenador, bool ganaJugador, int opcion);

int main ()
{
	srand(time(NULL)); // Semilla que ponemos para generar numeros aleatorios
	// Variables
	tMazo mazo;
	tMano manoJugador, manoOrdenador;
	tCarta carta;
	tProbabilidad array_probabilidad;
	int opcion, numPartidas = 0;
	bool ganaJugador = false;

	opcion = menu (); // Mostramos el menu

	while (opcion != 5) // Mientras que no pulse salir seguimos ejecutando
	{
		numPartidas++;
		if (opcion == 1 && cargarMazo (mazo)) // Modo A
			jugarModoA (mazo, manoJugador, manoOrdenador, carta);
		else if (opcion == 2 && cargarMazo (mazo)) // Modo B
			jugarModoB (mazo, manoJugador, manoOrdenador, carta);
		else if (opcion == 3 && cargarMazo (mazo)) // Modo C
			jugarModoC (mazo, manoJugador, manoOrdenador, carta, array_probabilidad);
		else // Modo D
			jugarModoD (mazo, manoJugador, manoOrdenador, carta, array_probabilidad);

		if (manoJugador.valorMano <= 7.5) // Si el jugador se pasa de 7.5 directamente se acaba la partida
		{
			if(opcion == 4)
				ganaJugador = determinarGanadorD (manoJugador, manoOrdenador);
			else
				ganaJugador = determinarGanador (manoJugador.valorMano, manoOrdenador.valorMano);
		}
		else
			cout << "Gana el ordenador" << endl;

		string aCadena = to_string (numPartidas); // Convertimos el numero a string, para poder poner el nombre del fichero
		if(guardarPartida (aCadena, manoJugador, manoOrdenador, ganaJugador, opcion))
			cout << "Partida guardada" << endl;
		opcion = menu();
	}
	return 0;
}

void mostrarMenu ()
{
	cout << "1- Modo A" <<endl;
	cout << "2- Modo B" <<endl;
	cout << "3- Modo C" <<endl;
	cout << "4- Modo D" <<endl;
	cout << "5- Finalizar" <<endl;
	cout << "Elija opcion: ";
}

int menu ()
{
	int opcion;

	mostrarMenu ();
	cin >> opcion;
	while (cin.fail() || opcion < 1 || opcion > 5)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10,'\n'); // Ignora hasta 10 caracteres siguientes en la misma linea
			cout << "No es un numero" << endl;
		}
		else
			cout << "Numero no valido" << endl;
		mostrarMenu ();
		cin >> opcion;
	}
	return opcion;
}

bool cargarMazo (tMazo & mazo)
{
	bool abierto;
	ifstream entrada;
	string nombre_archivo;

	cout << "Introduce el nombre del archivo: ";
	cin >> nombre_archivo;
	entrada.open (nombre_archivo);
	abierto = entrada.is_open();
	if (abierto)
		leerCartas (entrada, mazo);
	else
		cout << "Hubo problemas al abrir el archivo o no existe." << endl;
	entrada.close();
	return abierto;
}

void leerCartas (ifstream & entrada, tMazo & mazo)
{
	string basura;

	for (int i = 0; i < NUM_CARTAS; i++)
	{
		entrada >> mazo.array_cartas[i].numero;
		getline (entrada, basura); // Para saltar la fila
	}
	mazo.cont = NUM_CARTAS;
}

void jugarModoA (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta)
{
	int num = 3 + rand() % (6 - 3); // Calcula el numero de cartas (aleatoriamente) que tienen que robar los jugadores
	cout << "El numero aleatorio es: " << num << endl;
	crearMano (mazo, manoJugador, carta, num);
	cout << "La mano del jugador es: ";
	mostrarMano(manoJugador);
	cout << endl;
	manoJugador.valorMano = sumarValorCarta(manoJugador);
	cout << "El valor de la mano del jugador es: " << manoJugador.valorMano << endl;
	if (manoJugador.valorMano <= 7.5)
	{
		crearMano (mazo, manoOrdenador, carta, num);
		cout << "La mano del ordenador es: ";
		mostrarMano(manoOrdenador);
		cout << endl;
		manoOrdenador.valorMano = sumarValorCarta(manoOrdenador);
		cout << "El valor de la mano del ordenador es: " << manoOrdenador.valorMano << endl;
	}
}

void crearMano (tMazo & mazo, tMano & mano, tCarta & carta, int num)
{
	inicializarMano (mano);
	for (int i = 0; i < num; i++)
	{
		sacarCarta (mazo, carta);
		asignarCarta (mano, carta);
	}
}

void inicializarMano (tMano & mano)
{
	mano.cont = 0;
	mano.valorMano = 0;
}

void sacarCarta (tMazo & mazo, tCarta & carta)
{
	int i = 0;

	if (mazo.cont != 0)
	{
		carta = mazo.array_cartas[i];
		eliminarCartaMazo (mazo, i);
	}
}

void eliminarCartaMazo (tMazo & mazo, int pos)
{
	for (int i = pos + 1; i < mazo.cont; i++)
		mazo.array_cartas[i-1] = mazo.array_cartas[i];
	mazo.cont--;
}

void asignarCarta (tMano & mano, tCarta & carta)
{
	mano.array_cartas[mano.cont] = carta;
	mano.cont++;
}

void mostrarMano (const tMano & mano)
{
	for (int i = 0; i < mano.cont; i++)
		cout << mano.array_cartas[i].numero << " ";
}

float sumarValorCarta (tMano & mano)
{
	float suma = 0;

	for (int i = 0; i < mano.cont; i++)
	{
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
		num = 1+rand()%(3-1); // Elige un ganador de manera aleatoria
		if (num == 1)
		{
			cout << "Gana el jugador" << endl;
			ganaJugador = true;
		}
		else
			cout << "Gana el ordenador" << endl;
	}
	else
	{
		if (valorJugador < valorOrdenador && valorOrdenador > 7.5)
		{
			cout << "Gana el jugador" << endl;
			ganaJugador = true;
		}
		else
		{
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

void jugarModoB (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta)
{
	int max_cartas = 3+rand()%(6-3); // Calcula el numero de cartas (aleatoriamente) que tienen que robar los jugadores
	cout << "El numero aleatorio es: " << max_cartas << endl;
	robarJugador (mazo, manoJugador, carta, max_cartas);
	cout << "La mano del jugador es: ";
	mostrarMano(manoJugador);
	cout << endl;
	manoJugador.valorMano = sumarValorCarta(manoJugador);
	cout << "El valor de la mano del jugador es: " << manoJugador.valorMano << endl;
	if (manoJugador.valorMano <= 7.5)
	{
		robarOrdenador (mazo, manoOrdenador, manoJugador, carta, max_cartas);
		cout << "La mano del ordenador es: ";
		mostrarMano(manoOrdenador);
		cout << endl;
		manoOrdenador.valorMano = sumarValorCarta(manoOrdenador);
		cout << "El valor de la mano del ordenador es: " << manoOrdenador.valorMano << endl;
	}
}

void robarJugador (tMazo & mazo, tMano & mano, tCarta & carta, int max_cartas)
{
	string respuesta;
	bool correcto;

	inicializarMano(mano);
	sacarCarta (mazo, carta);
	asignarCarta(mano, carta);
	cout << "La mano del jugador es: ";
	mostrarMano(mano);
	cout << endl;
	mano.valorMano = sumarValorCarta (mano);
	cout << "Quieres seguir robando? (si/no): ";
	cin >> respuesta;
	correcto = comprobacionRespuesta (respuesta); // Comprobamos si la respuesta es valida
	while (mano.cont < max_cartas && correcto == true && mano.valorMano < 7.5)
	{
		sacarCarta (mazo, carta);
		asignarCarta(mano, carta);
		cout << "La mano del jugador es: ";
		mostrarMano(mano);
		cout << endl;
		mano.valorMano = sumarValorCarta (mano);
		if (mano.valorMano < 7.5)
		{
			cout << "Quieres seguir robando? (si/no): ";
			cin >> respuesta;
			correcto = comprobacionRespuesta (respuesta); // Comprobamos si la respuesta es valida
		}
		else
			cout << "Te has pasado de 7.5" << endl;
	}
}

bool comprobacionRespuesta (string respuesta)
{
	bool correcto = true;

	while (respuesta != "si" && respuesta != "Si" && respuesta != "SI" && respuesta != "no" && respuesta != "No" && respuesta != "NO")
	{
		cout << "Quieres seguir robando? (si/no): ";
		cin >> respuesta;
	}
	if (respuesta == "no" || respuesta == "No" || respuesta == "NO")
		correcto = false;
	return correcto;
}

void robarOrdenador (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, int max_cartas)
{
	inicializarMano(mano);
	sacarCarta (mazo, carta);
	asignarCarta(mano, carta);
	cout << "La mano del ordenador es: ";
	mostrarMano(mano);
	cout << endl;
	mano.valorMano = sumarValorCarta (mano);
	while (mano.cont < max_cartas && mano.valorMano < manoJugador.valorMano)
	{
		sacarCarta (mazo, carta);
		asignarCarta(mano, carta);
		cout << "La mano del ordenador es: ";
		mostrarMano(mano);
		cout << endl;
		mano.valorMano = sumarValorCarta (mano);
	}
}

void jugarModoC (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta, tProbabilidad array_probabilidad)
{
	robarJugadorC (mazo, manoJugador, carta);
	cout << "La mano del jugador es: ";
	mostrarMano(manoJugador);
	cout << endl;
	robarOrdenadorC (mazo, manoOrdenador, manoJugador, carta, array_probabilidad);
	cout << "La mano del ordenador es: ";
	mostrarMano(manoOrdenador);
	cout << endl;
}

void robarJugadorC (tMazo & mazo, tMano & mano, tCarta & carta)
{
	string respuesta;
	bool correcto;

	inicializarMano(mano);
	sacarCarta (mazo, carta);
	asignarCarta(mano, carta);
	cout << "La mano del jugador es: ";
	mostrarMano(mano);
	cout << endl;
	mano.valorMano = sumarValorCarta (mano);
	cout << "La puntuacion del jugador es: " << mano.valorMano << endl;
	cout << "Quieres seguir robando? (si/no): ";
	cin >> respuesta;
	correcto = comprobacionRespuesta (respuesta); // Comprobamos si la respuesta es valida
	while (mano.cont < mazo.cont && correcto == true)
	{
		sacarCarta (mazo, carta);
		asignarCarta(mano, carta);
		cout << "La mano del jugador es: ";
		mostrarMano(mano);
		cout << endl;
		mano.valorMano = sumarValorCarta (mano);
		cout << "La puntuacion del jugador es: " << mano.valorMano << endl;
		cout << "Quieres seguir robando? (si/no): ";
		cin >> respuesta;
		correcto = comprobacionRespuesta (respuesta); // Comprobamos si la respuesta es valida
	}
}

void robarOrdenadorC (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, tProbabilidad array_probabilidad)
{
	float probabilidad;
	inicializarMano(mano);
	sacarCarta (mazo, carta);
	asignarCarta(mano, carta);
	cout << "La mano del ordenador es: ";
	mostrarMano(mano);
	cout << endl;
	mano.valorMano = sumarValorCarta (mano);
	cout << "La puntuacion del ordenador es: " << mano.valorMano << endl;
	if (manoJugador.valorMano <= 7.5)
	{
		probabilidad = calcularProbabilidad (mazo, array_probabilidad, manoJugador);
		while (mano.cont < mazo.cont && mano.valorMano <= manoJugador.valorMano && probabilidad < 0.50) // Empate: probabilidad de pasarse supera el 50%
		{
			sacarCarta (mazo, carta);
			asignarCarta(mano, carta);
			cout << "La mano del ordenador es: ";
			mostrarMano(mano);
			cout << endl;
			mano.valorMano = sumarValorCarta (mano);
			cout << "La puntuacion del ordenador es: " << mano.valorMano << endl;
			probabilidad = calcularProbabilidad (mazo, array_probabilidad, manoJugador);
		}
	}
}

float calcularProbabilidad (tMazo & mazo, tProbabilidad array_probabilidad, tMano & manoJugador)
{
	float probabilidad = 0, suma = 0;
	int resta = 0;

	for (int i = 0; i < 8; i++)
		array_probabilidad[i] = 0;

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

	resta = 7.5 - manoJugador.valorMano;
	cout << "La resta es: " << resta << endl;
	for (int j = resta + 1; j < 8; j++)
		suma = suma + array_probabilidad[j];
	cout << "La suma de las cartas es: " << suma << endl;
	cout << "Cartas mazo: "<< mazo.cont << endl;
	probabilidad = suma / mazo.cont;
	cout << "La probabilidad es: " << probabilidad << endl;

	return probabilidad;
}

void jugarModoD (tMazo & mazo, tMano & manoJugador, tMano & manoOrdenador, tCarta & carta, tProbabilidad array_probabilidad)
{
	crearMazo (mazo);
	robarJugadorC(mazo, manoJugador, carta);
	robarOrdenadorC(mazo, manoOrdenador, manoJugador, carta, array_probabilidad);
}

void crearMazo (tMazo & mazo)
{
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

	if(manoJugador.valorMano == manoOrdenador.valorMano)
	{
		if(manoJugador.cont < manoOrdenador.cont)
		{
			cout << "Habeis empatado. Gana el jugador porque tiene menos cartas en la mano." << endl;
			ganaJugador = true;
		}
		else if (manoOrdenador.cont < manoJugador.cont)
			cout << "Habeis empatado. Gana el ordenador porque tiene menos cartas en la mano." << endl;
		else
		{
			cout << "Habeis empatado, se elegira a un ganador de forma aleatoria: " << endl;
			num = 1+rand()%(3-1);
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

bool guardarPartida (string titulo, tMano manoJugador, tMano manoOrdenador, bool ganaJugador, int opcion)
{
	ofstream salida;

	salida.open (titulo);
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
		// Escribimos quien gano en la partida
		if (ganaJugador)
			salida << "Gano el jugador";
		else
			salida << "Gano el ordenador";
	}
	salida.close();
	return abierto;
}
