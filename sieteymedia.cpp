#include <iostream> 
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

const int MAX_CARTAS = 40;

typedef struct {
	int numero;
	string tipo;
}tCarta;

typedef tCarta tArrayCartas[MAX_CARTAS];

typedef struct {
	int cont;
	tArrayCartas array_cartas;
} tMazo;

typedef struct {
	int cont;
	float valorMano;
	tArrayCartas array_cartas;
}tMano;

	
void mostrarMazo(tMazo &mazo);
void mostrarMano(tMano &mano);
bool cargarMazo(tMazo &mazo);
void eliminarCartaMazo(tMazo & mazo, int pos);
void leerCartas(ifstream &entrada, tMazo &mazo);
float sumarValorCarta (tMano &mano);
void sacarCarta(tMazo & mazo, tCarta & carta);
int menu();
int menuJuego(); //Si se planta o  no
int rangoCartas();
void crearMano (tMazo &mazo, tMano & mano, tCarta & carta, int num);
void determinarGanador(float valorJugador, float valorOrdenador);

// num = 3+rand()%(6-3);
int main()
{
	srand(time(NULL));
	
	tMazo mazo;
	tMano manoJugador, manoOrdenador;
	tCarta carta;
	
	int op;
	
	op = menu();
	while(op != 0)
	{
		if (op == 1 && cargarMazo(mazo))
		{
			int num = 3+rand()%(6-3);
			cout << "El numero aleatorio es: " << num << endl;
			crearMano (mazo, manoJugador, carta, num);
			cout << "La mano del jugador es: ";
			mostrarMano(manoJugador);
			cout << endl;
			crearMano (mazo, manoOrdenador, carta, num);
			cout << "La mano del ordenador es: ";
			mostrarMano(manoOrdenador);
			cout << endl;
			manoJugador.valorMano = sumarValorCarta(manoJugador);
			manoOrdenador.valorMano = sumarValorCarta(manoOrdenador);
			cout << "El valor de la mano del jugador es: " << manoJugador.valorMano << endl;
			cout << "El valor de la mano del ordenador es: " << manoOrdenador.valorMano << endl;
			determinarGanador(manoJugador.valorMano, manoOrdenador.valorMano);
			
		} else if (op == 2 && cargarMazo(mazo)){
			//codigo
		} else {
			cout << "No se ha podido cargar el mazo" << endl;
		}
		op = menu();
	}
	

	return 0;
}

int menu()
{
	int op;
	
	cout << "1. Jugar en el modo A" << endl;
	cout << "2. Jugar en el modo B" << endl;
	cout << "0. Salir" << endl;
	cin >> op;
	while(op < 0 || op > 2)
	{
		cout << "Error en la opcion, elija 1, 2 o 0" << endl;
		cout << "1. Jugar en el modo A" << endl;
		cout << "2. Jugar en el modo B" << endl;
		cout << "0. Salir" << endl;
		cin >> op;
	}
	
	return op;
}
bool cargarMazo(tMazo &mazo)
{
	bool abierto;
	ifstream entrada;
	entrada.open("cartas.txt");
	abierto = entrada.is_open();
	if(abierto){
		leerCartas(entrada, mazo);
		random_shuffle(&(mazo.array_cartas[0]), &(mazo.array_cartas[MAX_CARTAS]));
	}
	
	entrada.close();
	return abierto;
}

void leerCartas(ifstream &entrada, tMazo &mazo)
{
	for (int i = 0; i < MAX_CARTAS; i++)
	{
		entrada >> mazo.array_cartas[i].numero;
	}
	mazo.cont = MAX_CARTAS;
}

float sumarValorCarta (tMano &mano)
{
	float suma = 0;
	for(int i = 0; i < mano.cont; i++)
	{
		if(mano.array_cartas[i].numero == 10 || mano.array_cartas[i].numero == 11 || mano.array_cartas[i].numero == 12)
			suma += 0.5;
		else 
			suma += mano.array_cartas[i].numero;
	}
	
	return suma;
}


void sacarCarta(tMazo & mazo, tCarta & carta)
{
	int i = 0;
	bool exito = false;
	if (mazo.cont != 0)
	{
		exito = true;
		carta = mazo.array_cartas[i];
		eliminarCartaMazo (mazo, i);
	}
	//return exito;
}

void eliminarCartaMazo(tMazo & mazo, int pos)
{
	for (int i = pos + 1; i<mazo.cont; i++)
	{
		mazo.array_cartas[i-1] = mazo.array_cartas[i];
	}
	mazo.cont--;
}

void mostrarMazo(tMazo &mazo)
{
	for (int i = 0; i < mazo.cont; i++)
		cout << mazo.array_cartas[i].numero << " ";
}

void mostrarMano(tMano &mano)
{
	for(int i = 0; i < mano.cont; i++)
		cout << mano.array_cartas[i].numero << " ";
}

void crearMano (tMazo &mazo, tMano & mano, tCarta & carta, int num)
{
	mano.cont = 0;
	mano.valorMano = 0;
	for(int i = 0; i < num; i++)
	{
		sacarCarta(mazo, carta);
		mano.array_cartas[i] = carta;
		mano.cont++;
	}
}

void determinarGanador(float valorJugador, float valorOrdenador)
{
	int num;
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
		int resta1 = 7.5 - valorJugador;
		int resta2 = 7.5 - valorOrdenador;
		cout << resta1 << endl;
		cout << resta2 << endl;
		if (resta1 < resta2)
			cout << "Gana el jugador" << endl;
		else
			cout << "Gana el ordenador" << endl;
	}
	else
	{
		num = 1+rand()%(3-1);
		if (num == 1)
			cout << "Gana el jugador" << endl;
		else
			cout << "Gana el ordenador" << endl;
	}
}

void crearMazo(tMazo &mazo)
{
	
	for(int i = 0 ; i < 7; i++){
		int x = 1;
		mazo.array_cartas[i].numero = x;// 1 2 3 4 5 6 7
		x++;
	}
	for(int i = 7 ; i < 10; i++){
		int y = 10;
		mazo.array_cartas[i].numero = y;
		y++;
	}
		
	for(int i = 10; i < 17; i++)
		mazo.array_cartas[i].numero = x+1;
	for(int i = 17; i < 20; i++)
		mazo.array_cartas[i].numero = y+3;
	for(int i = 20; i < 27; i++)
		mazo.array_cartas[i].numero = x+3;
	for(int i = 27; i < 30; i++)
		mazo.array_cartas[i].numero = y+3;
	for(int i = 30; i < 37; i++)
		mazo.array_cartas[i].numero = x+3;
	for(int i = 37; i < MAX_CARTAS; i++)
		mazo.array_cartas[i].numero = y+3;
	
	mazo.cont = MAX_CARTAS;
	
	
}

void mostrar(tMazo &mazo)
{
	
}




