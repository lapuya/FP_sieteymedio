/*
Grupo F
Beatriz Álvarez de Arriba
Laurence Apuya Pangilinan
*/

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;

const int MAX_CARTAS = 40;
const int NUM_PROBABILIDAD = 8;

typedef struct
{
	int numero;
	string tipo; // No se usa para nada
}tCarta;

typedef tCarta tArrayCartas [MAX_CARTAS];

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

void mostrarMenu ();//+++++++++++
int menu ();//+++++++++
bool cargarMazo (tMazo & mazo);//+++++++++
void leerCartas(ifstream & entrada, tMazo & mazo);//+++++++++++++
float sumarValorCarta (tMano & mano);//+++++++++++++++
void crearMano (tMazo & mazo, tMano & mano, tCarta & carta, int num);//++++++++++++
void sacarCarta(tMazo & mazo, tCarta & carta);//+++++++++
void eliminarCartaMazo(tMazo & mazo, int pos);//+++++++++++
void mostrarMazo(const tMazo & mazo);
void mostrarMano(const tMano & mano);//++++++++++
void determinarGanador(float valorJugador, float valorOrdenador);//+++++++++++
void determinarGanadorD(tMano & manoJugador, tMano & manoOrdenador);
void robarJugador (tMazo & mazo, tMano & mano, tCarta & carta, int max_cartas);//+++++++++++
void robarOrdenador (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, int max_cartas);//++++++++++
void robarJugadorC (tMazo & mazo, tMano & mano, tCarta & carta);
void robarOrdenadorC (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, tProbabilidad array_probabilidad);
float calcularProbabilidad (tMazo & mazo, tProbabilidad array_probabilidad, tMano & manoJugador);
void inicializarMano(tMano & mano);//++++++++++
void asignarCarta(tMano & mano, tCarta & carta);//++++++++++++
void crearMazo (tMazo & mazo);
void mostrar (tMazo & mazo);
bool guardarPartidaModoD (string titulo, tMano manoJugador, tMano manoOrdenador);

int main ()
{
	srand(time(NULL));

	tMazo mazo;
	tMano manoJugador, manoOrdenador;
	tCarta carta;
	tProbabilidad array_probabilidad;

	int opcion, num, max_cartas, numPartidasModoD = 0;

	opcion = menu ();

	while (opcion != 5)
	{
		if (opcion == 1 && cargarMazo(mazo))
		{
			num = 3 + rand() % (6 - 3); // Calcula el numero de cartas (aleatoriamente) que tienen que robar los jugadores
			cout << "El numero aleatorio es: " << num << endl;
			crearMano (mazo, manoJugador, carta, num);
			cout << "La mano del jugador es: ";
			mostrarMano(manoJugador);
			cout << endl;
			manoJugador.valorMano = sumarValorCarta(manoJugador);
			if (manoJugador.valorMano <= 7.5)
			{
				crearMano (mazo, manoOrdenador, carta, num);
				cout << "La mano del ordenador es: ";
				mostrarMano(manoOrdenador);
				cout << endl;
				manoOrdenador.valorMano = sumarValorCarta(manoOrdenador);
				cout << "El valor de la mano del jugador es: " << manoJugador.valorMano << endl;
				cout << "El valor de la mano del ordenador es: " << manoOrdenador.valorMano << endl;
				determinarGanador(manoJugador.valorMano, manoOrdenador.valorMano);
			}
			else
				cout << "Gana el ordenador" << endl;
		}
		else if (opcion == 2 && cargarMazo(mazo))
		{
			max_cartas = 3+rand()%(6-3); // Calcula el numero de cartas (aleatoriamente) que tienen que robar los jugadores
			cout << "El numero aleatorio es: " << max_cartas << endl;
			robarJugador (mazo, manoJugador, carta, max_cartas);
			cout << "La mano del jugador es: ";
			mostrarMano(manoJugador);
			cout << endl;
			manoJugador.valorMano = sumarValorCarta(manoJugador);
			if (manoJugador.valorMano <= 7.5)
			{
				robarOrdenador (mazo, manoOrdenador, manoJugador, carta, max_cartas);
				cout << "La mano del ordenador es: ";
				mostrarMano(manoOrdenador);
				cout << endl;
				determinarGanador(manoJugador.valorMano, manoOrdenador.valorMano);
			}
			else
				cout << "Gana el ordenador" << endl;
		}
		else if (opcion == 3 && cargarMazo(mazo))
		{
			robarJugadorC (mazo, manoJugador, carta);
			cout << "La mano del jugador es: ";
			mostrarMano(manoJugador);
			cout << endl;
			robarOrdenadorC (mazo, manoOrdenador, manoJugador, carta, array_probabilidad);
			cout << "La mano del ordenador es: ";
			mostrarMano(manoOrdenador);
			cout << endl;
			determinarGanador(manoJugador.valorMano, manoOrdenador.valorMano);
		}
		else if (opcion == 4)
		{
			crearMazo (mazo);
			robarJugadorC(mazo, manoJugador, carta);
			robarOrdenadorC(mazo, manoOrdenador, manoJugador, carta, array_probabilidad);
			determinarGanadorD(manoJugador, manoOrdenador);
			numPartidasModoD++;
			string aCadena = to_string(numPartidasModoD);
			if(guardarPartidaModoD(aCadena, manoJugador, manoOrdenador))
				cout << "Partida guardada" << endl;
		}

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
	while (opcion < 1 || opcion > 5)
	{
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

void leerCartas(ifstream & entrada, tMazo & mazo)
{
	for (int i = 0; i < MAX_CARTAS; i++)
		entrada >> mazo.array_cartas[i].numero;
	mazo.cont = MAX_CARTAS;
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

void crearMano (tMazo &mazo, tMano & mano, tCarta & carta, int num)
{
	inicializarMano(mano);
	for(int i = 0; i < num; i++)
	{
		sacarCarta(mazo, carta);
		asignarCarta(mano, carta);
	}
}

void sacarCarta(tMazo & mazo, tCarta & carta)
{
	int i = 0;
	if (mazo.cont != 0)
	{
		carta = mazo.array_cartas[i];
		eliminarCartaMazo (mazo, i);
	}
}

void eliminarCartaMazo(tMazo & mazo, int pos)
{
	for (int i = pos + 1; i < mazo.cont; i++)
		mazo.array_cartas[i-1] = mazo.array_cartas[i];
	mazo.cont--;
}

void mostrarMazo(const tMazo &mazo)
{
	for (int i = 0; i < mazo.cont; i++)
		cout << mazo.array_cartas[i].numero << " ";
}

void mostrarMano(const tMano &mano)
{
	for(int i = 0; i < mano.cont; i++)
		cout << mano.array_cartas[i].numero << " ";
}

void determinarGanador(float valorJugador, float valorOrdenador)
{
	int num, resta1, resta2;

	if(valorJugador == 7.5)
		cout << "Gana el jugador" << endl;
	else if (valorOrdenador == 7.5)
		cout << "Gana el ordenador" << endl;
	else if (valorJugador > 7.5 && valorOrdenador > 7.5)
		cout << "Habeis perdido" << endl;
	else if (valorJugador < valorOrdenador && valorOrdenador > 7.5)
		cout << "Gana el jugador" << endl;
	else if (valorOrdenador < valorJugador && valorJugador > 7.5)
		cout << "Gana el ordenador" << endl;
	else if (valorJugador < 7.5 && valorOrdenador < 7.5)
	{
		resta1 = 7.5 - valorJugador;
		resta2 = 7.5 - valorOrdenador;
		if (resta1 < resta2)
			cout << "Gana el jugador" << endl;
		else
			cout << "Gana el ordenador" << endl;
	}
	else
	{
		cout << "Habeis empatado, se elegira a un ganador de forma aleatoria: " << endl;
		num = 1+rand()%(3-1);
		if (num == 1)
			cout << "Gana el jugador" << endl;
		else
			cout << "Gana el ordenador" << endl;
	}
}

void determinarGanadorD(tMano & manoJugador, tMano & manoOrdenador)
{
	int num, resta1, resta2;

	if(manoJugador.valorMano == 7.5)
		cout << "Gana el jugador" << endl;
	else if (manoOrdenador.valorMano == 7.5)
		cout << "Gana el ordenador" << endl;
	else if (manoJugador.valorMano > 7.5 && manoOrdenador.valorMano > 7.5)
		cout << "Habeis perdido" << endl;
	else if (manoJugador.valorMano < manoOrdenador.valorMano && manoOrdenador.valorMano > 7.5)
		cout << "Gana el jugador" << endl;
	else if (manoOrdenador.valorMano < manoJugador.valorMano && manoJugador.valorMano > 7.5)
		cout << "Gana el ordenador" << endl;
	else if (manoJugador.valorMano < 7.5 && manoOrdenador.valorMano < 7.5)
	{
		resta1 = 7.5 - manoJugador.valorMano;
		resta2 = 7.5 - manoOrdenador.valorMano;
		if (resta1 < resta2)
			cout << "Gana el jugador" << endl;
		else
			cout << "Gana el ordenador" << endl;
	}
	else
	{
		if(manoJugador.cont < manoOrdenador.cont)
			cout << "Habeis empatado. Gana el jugador porque tiene menos cartas en la mano." << endl;
		else if (manoOrdenador.cont < manoJugador.cont)
			cout << "Habeis empatado. Gana el ordenador porque tiene menos cartas en la mano." << endl;
		else
		{
			cout << "Habeis empatado, se elegira a un ganador de forma aleatoria: " << endl;
			num = 1+rand()%(3-1);
			if (num == 1)
				cout << "Gana el jugador" << endl;
			else
				cout << "Gana el ordenador" << endl;
		}
	}
}

void robarJugador (tMazo & mazo, tMano & mano, tCarta & carta, int max_cartas) //NUEVA
{
	string respuesta;
	inicializarMano(mano);
	sacarCarta (mazo, carta);
	asignarCarta(mano, carta);
	cout << "La mano del jugador es: ";
	mostrarMano(mano);
	cout << endl;
	mano.valorMano = sumarValorCarta (mano);
	cout << "Quieres seguir robando? (si/no): ";
	cin >> respuesta;
	while (mano.cont < max_cartas && respuesta == "si" && mano.valorMano < 7.5)
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
		}
		else
			cout << "Te has pasado de 7.5" << endl;
	}
}

void robarOrdenador (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, int max_cartas) //NUEVA
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

void robarJugadorC (tMazo & mazo, tMano & mano, tCarta & carta) //NUEVA
{
	string respuesta;
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
	while (mano.cont < mazo.cont && respuesta == "si")
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
	}
}

void robarOrdenadorC (tMazo & mazo, tMano & mano, tMano & manoJugador, tCarta & carta, tProbabilidad array_probabilidad) //NUEVA
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

void inicializarMano(tMano & mano)
{
	mano.cont = 0;
	mano.valorMano = 0;
}

void asignarCarta(tMano & mano, tCarta & carta)
{
	mano.array_cartas[mano.cont] = carta;
	mano.cont++;
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

	/*cout << "Figura: " << array_probabilidad[0] << endl;
	cout << "Uno: " << array_probabilidad[1] << endl;
	cout << "Dos: " << array_probabilidad[2] << endl;
	cout << "Tres: " << array_probabilidad[3] << endl;
	cout << "Cuatro: " << array_probabilidad[4] << endl;
	cout << "Cinco: " << array_probabilidad[5] << endl;
	cout << "Seis: " << array_probabilidad[6] << endl;
	cout << "Siete: " << array_probabilidad[7] << endl;*/

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
	for (int i = 36; i < MAX_CARTAS; i++)
		mazo.array_cartas[i].numero = 12;

	mazo.cont = MAX_CARTAS;
	// Mezcla las cartas aleatoriamente
	random_shuffle(&(mazo.array_cartas[0]), &(mazo.array_cartas[MAX_CARTAS]));
}

void mostrar (tMazo & mazo)
{
	for (int i = 0; i < MAX_CARTAS; i++)
		cout << mazo.array_cartas[i].numero;
	cout << endl;
}

bool guardarPartidaModoD (string titulo, tMano manoJugador, tMano manoOrdenador) {
	ofstream salida;
	salida.open (titulo);
	bool abierto = salida.is_open();
	if (abierto)
	{
		salida << "Las cartas del jugador fueron: ";
		for (int i = 0; i < manoJugador.cont; i++)
			salida << manoJugador.array_cartas[i].numero << " ";
		salida << endl;
		salida << "Las cartas del ordenador fueron: ";
		for (int i = 0; i < manoOrdenador.cont; i++)
			salida << manoOrdenador.array_cartas[i].numero << " ";
	}
	salida.close();

	return abierto;
}
