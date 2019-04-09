#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

// Grabo los movimientos del jug0(dx[0]), y jug1(dx[1]).
// jug0 movimientos de abajo, jug1 de arriba
int dx[2][4] = {{1, 1, 2, 2}, {-1,  -1, -2, -2}};
int dy[2][4] = {{-1, 1, -2, 2} , {-1, 1, -2, 2}};


int Otro(int jugador) // Retorna el "Otro" jugador
{
    if(jugador == 1) return 0;
    else if(jugador == 0) return 1;
}

struct Casilla
{
	int jugador = -1;          //-1:Vacia  |  0:Jugador0  |  1:Jugador1
};

class Tablero
{
public:
	bool enJuego = true; //Si el juego continua... jugar!!!
	int puntaje_jug0 = 0; // Humanus
    int puntaje_jug1 = 0; // IA
    Casilla matriz[8][8];
    vector<pair<int,int>> fichas_jug0;
    vector<pair<int,int>> fichas_jug1;

    Tablero();
    void ingresarJugada(int jugador, int x1, int y1, int x2, int y2); // x1,y1... explicados en este metodo
    bool esJugadaValida(int jugador, int x1, int y1, int x2, int y2);
	bool hayMovimientosPosibles(int jugador);
    void Impresion();

private:
    void eliminarFicha(int x, int y);
};

Tablero::Tablero()
{
    // Ubicacion inicial de las fichas
    for (int i = 1; i <= 7; i += 2)
    {
        matriz[0][i].jugador = 0;
        matriz[1][i-1].jugador = 0;
        matriz[2][i].jugador = 0;
        fichas_jug0.emplace_back(0, i);
        fichas_jug0.emplace_back(1, i-1);
        fichas_jug0.emplace_back(2, i);

        matriz[5][i-1].jugador = 1;
        matriz[6][i].jugador = 1;
        matriz[7][i-1].jugador = 1;
        fichas_jug1.emplace_back(5, i-1);
        fichas_jug1.emplace_back(6, i);
        fichas_jug1.emplace_back(7, i-1);
    }
}

void Tablero::ingresarJugada(int jugador, int x1, int y1, int x2, int y2)
{
    // x: fila, y: columna
    //x1, y1 son coord. iniciales (posicion inicial)
    //x2, y2 son coord. finales   (posicion final)
    // Ejecuto esta funcion si la jugada es valida

    // Mover ficha
    matriz[x1][y1].jugador = -1;      // Vacio las coord. iniciales, -1 significa vacio
    matriz[x2][y2].jugador = jugador; // Ajusto las coords.final

    // Actualizo posicion ficha
    if(jugador == 0)
    {
        auto it = find(fichas_jug0.begin(), fichas_jug0.end(), make_pair(x1, y1));
        fichas_jug0.erase(it);
        fichas_jug0.emplace_back(x2, y2);
    }
    else
    {
        auto it = find(fichas_jug1.begin(), fichas_jug1.end(), make_pair(x1, y1));
        fichas_jug1.erase(it);
        fichas_jug1.emplace_back(x2, y2);
    }


    //Si me muevo para comer ficha
    if (abs(x1 - x2) == 2 && abs(y1 - y2) == 2)
    {
        int x3 = (x1 + x2) / 2;
        int y3 = (y1 + y2) / 2;
        eliminarFicha(x3, y3); // Se que en medio hay una ficha enemiga mediante la funcion de validar
    }
}

bool Tablero::esJugadaValida(int jugador, int x1, int y1, int x2, int y2)
{
    // No vale fuera de rango!! asi Oscar lo digo!!!!
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 ||
        x1 > 7 || y1 > 7 || x2 > 7 || y2 > 7)
        return false;

    //Si la ficha no pertenece al jugador
    if (matriz[x1][y1].jugador != jugador)
        return false;

    // Si el destino no esta vacio
    if (matriz[x2][y2].jugador != -1)
        return false;

    //Validar movimiento legal en cualquier jugador
    //Si hago un Movimiento simple
    if (abs(y1 - y2) == 1 && abs(x1 - x2) == 1)
        return true;
    else //Si hago Movimiento Capturar Ficha
    if(abs(y1 - y2) == 2 && abs(x1 - x2) == 2)
    {
        int x3 = (x1 + x2)/2;
        int y3 = (y1 + y2)/2;
        return matriz[x3][y3].jugador == Otro(jugador); // Si el medio es enemigo
    }
    return false;
}

bool Tablero::hayMovimientosPosibles(int jugador)
{
    // Viendo cual vector de fichas usar
    vector<pair<int,int>> vec_fichas;
    if(jugador == 0) vec_fichas = fichas_jug0;
    else if(jugador == 1) vec_fichas = fichas_jug1;

    //Verificando si el jugador tiene jugadas validas
    for(pair<int,int> ficha: vec_fichas)
    {
        int x = ficha.first;
        int y = ficha.second;
        for(int i=0; i<5; ++i)
            if(esJugadaValida(jugador, x, y, x+dx[jugador][i], y+dy[jugador][i]))
                return true;
    }
    return false;
}

void Tablero::Impresion()
{
	cout << "    ";
	for (int i = 0; i < 8; ++i) cout << i << " ";
	cout << endl << endl;

	for (int i = 0; i < 8; ++i)
	{
		cout << i << "-- ";
		for (int j = 0; j < 8; ++j)
			if (matriz[i][j].jugador == -1) cout << "* "; // Si la casilla esta vacia
			else if (matriz[i][j].jugador) cout << "1 ";
			else                           cout << "0 ";
		cout << endl;
	}
	cout << endl;
	cout << "Puntos Jug. 0: " << puntaje_jug0 << "  ";
	cout << "Puntos Jug. 1: " << puntaje_jug1 << endl;
}

void Tablero::eliminarFicha(int x, int y)
{
    int jugador = matriz[x][y].jugador;
    if(jugador == 0)
    {
        puntaje_jug1 += 1;
        auto it = find(fichas_jug0.begin(), fichas_jug0.end(), make_pair(x, y));
        fichas_jug0.erase(it);
    }
    else
    {
        puntaje_jug0 += 1;
        auto it = find(fichas_jug1.begin(), fichas_jug1.end(), make_pair(x, y));
        fichas_jug1.erase(it);
    }
    matriz[x][y].jugador = -1; // Vacio la casilla
}

class IA
{
public:
    const int JUGADOR_IA = 1;
public:
    Tablero* tablero;
    int maxprof;

public:
    IA(Tablero* tab, int maxprof): tablero(tab), maxprof(maxprof) {;}
    void hacerJugada(int& x1, int& y1, int& x2, int& y2);
    int minimax(int jugador, Tablero tab, int prof);
};

void IA::hacerJugada(int& x1, int& y1, int& x2, int& y2)
{
    Tablero tab = *tablero; // Copio

    int maxPuntJug, minPuntOtro;
    maxPuntJug = tab.puntaje_jug1, minPuntOtro = tab.puntaje_jug0+1; // +1 para arreglar el caso en que la IA, haga lo que haga, no puede evitar que el tonto humano le gane un punto.
    for(pair<int,int> ficha: tab.fichas_jug1)
    {
        int x = ficha.first;
        int y = ficha.second;

        for(int i=0; i<5; ++i)
        {
            int newx = x+dx[JUGADOR_IA][i];
            int newy = y+dy[JUGADOR_IA][i];
            if(tab.esJugadaValida(JUGADOR_IA, x, y, newx, newy))
            {
                Tablero t2 = tab;
                t2.ingresarJugada(JUGADOR_IA, x, y, newx, newy);
                int puntJug= t2.puntaje_jug1;
                int puntOtro = minimax(Otro(JUGADOR_IA), t2, 0);

                if(puntJug > maxPuntJug)
                {
                    maxPuntJug = puntJug;
                    x1 = x; x2 = newx;
                    y1 = y; y2 = newy;
                    minPuntOtro = puntOtro;
                }
                else if(puntJug == maxPuntJug)
                {
                    if(puntOtro <= minPuntOtro)
                    {
                        minPuntOtro = puntOtro;
                        x1 = x; x2 = newx;
                        y1 = y; y2 = newy;
                    }
                }
            }
        }
    }
}

int IA::minimax(int jugador, Tablero tab, int prof)
{
//    cout << "Prof " << prof << endl;
//    tab.Impresion();
    if(prof == maxprof) return 1000;
    vector<pair<int,int>> vec_fichas;
    if(jugador == 0) vec_fichas = tab.fichas_jug0;
    else             vec_fichas = tab.fichas_jug1;

    int maxPuntJug, minPuntOtro;
    if(jugador == 0) maxPuntJug = tab.puntaje_jug0, minPuntOtro = tab.puntaje_jug1;
    else             maxPuntJug = tab.puntaje_jug1, minPuntOtro = tab.puntaje_jug0;
    for(const auto& ficha: vec_fichas)
    {
        int x = ficha.first;
        int y = ficha.second;

        for(int i=0; i<5; ++i)
        {
            int newx = x+dx[jugador][i];
            int newy = y+dy[jugador][i];
            if(tab.esJugadaValida(jugador, x, y, newx, newy))
            {
                Tablero t2 = tab;
                t2.ingresarJugada(jugador, x, y, newx, newy);
                int puntJug, puntOtro;
                if(jugador == 0) puntJug = t2.puntaje_jug0;
                else             puntJug = t2.puntaje_jug1;
                puntOtro = minimax(Otro(jugador), t2, prof+1);

                if(puntJug > maxPuntJug)
                {
                    maxPuntJug = puntJug;
                    minPuntOtro = puntOtro;
                }
                else if(puntJug == maxPuntJug)
                {
                    if(puntOtro <= minPuntOtro)
                        minPuntOtro = puntOtro;
                }
            }
        }
    }
    return maxPuntJug-minPuntOtro;
}

void turnoHumanus(int& x1, int& y1, int& x2, int& y2)
{
    cout << "TURNO JUGADOR HUMANO" << endl;
    cout << "Usa las fichas con numero 0" << endl;
    cout << "Ingrese fila columna de ficha a mover: ";  cin >> x1 >> y1;
    cout << "Ingrese fila columna de destino: ";        cin >> x2 >> y2;
    cout << endl;
}

void turnoIA(IA& ia, int& x1, int& y1, int& x2, int& y2)
{
    cout << "TURNO DE LA IA" << endl;

    ia.hacerJugada(x1, y1, x2, y2);
    cout << "IA quiere mover la ficha (" << x1 << " " << y1 << ")" << endl;
    cout << "IA quiere mover hacia    (" << x2 << " " << y2 << ")" << endl;
    cout << endl;
}

int main()
{
    int maxprof;
    cout << "Maxima profundidad IA: "; cin>>maxprof;
	Tablero tablero;
	tablero.Impresion();
	IA ia(&tablero, maxprof);

	int jugador = 0;
	while(tablero.enJuego)
	{
	    int x1, y1, x2, y2;
	    if(jugador == 0)      turnoHumanus(x1, y1, x2, y2);
	    else                  turnoIA (ia, x1, y1, x2, y2);

        if (tablero.esJugadaValida(jugador, x1, y1, x2, y2))
        {
            tablero.ingresarJugada(jugador, x1, y1, x2, y2);
            jugador = Otro(jugador);
            tablero.enJuego = tablero.hayMovimientosPosibles(jugador);
            tablero.Impresion();
        }
        else
        {
            cout << "Jugada Invalida" << endl;
        }
	}
	if(tablero.puntaje_jug0 > tablero.puntaje_jug1)
	    cout << "Humanos Ganan!!!... el humano aun no es obsoleto" << endl;
    else if(tablero.puntaje_jug0 == tablero.puntaje_jug1)
        cout << "Empate!... Singularidad tecnologica alcanzada" << endl;
    else
        cout << "IA Gana... Humano destruido" << endl;
	return 0;
}