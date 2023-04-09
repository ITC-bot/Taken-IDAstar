/*Hecho por Marcos Abaroa para la materia de Inteligencia Artificial I
Programa que resuelve el juego del 15 o taken implementando IDA*
Notas generales:
-Aveces la solucion posiblemente no sea encontrada debido a la profundidad y la dificultad del puzzle generado
-Si se quiere ver paso por paso lo que se hizo, se puede cambiar al main que esta comentado en la parte de abajo del codigo*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <ctime>

using namespace std;

const int N = 4; // tamaño del tablero
const int MAX_DEPTH = 100; // profundidad maxima
const int INF = numeric_limits<int>::max(); // valor infinito

// Estructura para representar el estado del juego
struct State {
    int board[N][N]; // matriz del tablero
    int x, y; // coordenadas del espacio en blanco

    State() {
        // inicializa el tablero en orden
        int val = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = val++;
            }
        }
        board[N-1][N-1] = 0;
        x = N-1;
        y = N-1;
    }
    // Verifica si dos estados son iguales
    bool operator==(const State& other) const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] != other.board[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    // Funcion para imprimir el tablero
    void print() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

// Funcion heuristica para estimar la distancia de Manhattan
int heuristic(const State& state) {
    int distance = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value = state.board[i][j];
            if (value != 0) {
                int targetX = (value - 1) / N;
                int targetY = (value - 1) % N;
                distance += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return distance;
}

// Funcion para generar los sucesores de un estado
vector<State> successors(const State& state) {
    vector<State> succ;
    int x = state.x;
    int y = state.y;
    if (x > 0) {
        State s = state;
        swap(s.board[x][y], s.board[x-1][y]);
        s.x = x-1;
        succ.push_back(s);
    }
    if (x < N-1) {
        State s = state;
        swap(s.board[x][y], s.board[x+1][y]);
        s.x = x+1;
        succ.push_back(s);
    }
    if (y > 0) {
        State s = state;
        swap(s.board[x][y], s.board[x][y-1]);
        s.y = y-1;
        succ.push_back(s);
    }
    if (y < N-1) {
        State s = state;
        swap(s.board[x][y], s.board[x][y+1]);
        s.y = y+1;
        succ.push_back(s);
    }
    return succ;
}

// Funcion de busqueda en profundidad
void search(State state, int g, int bound, int (*heuristic)(const State&), vector<State>& solution, bool& found_solution) {
    int f = g + heuristic(state);
    if (f > bound) {
        return;
    }
    if (heuristic(state) == 0) {
        found_solution = true;
        solution.push_back(state);
        return; // solucion encontrada
    }
    int min_t = INF;
    vector<State> succ = successors(state);
    for (State s : succ) {
        if (s == state) {
            continue;
        }
        search(s, g+1, bound, heuristic, solution, found_solution);
        if (found_solution) {
            solution.push_back(state);
            return;
        }
    }
}

// Funcion de busqueda IDA*
vector<State> ida_star_search(State state, int (*heuristic)(const State&)) {
    vector<State> solution;
    int bound = heuristic(state);
    while (true) {
        bool found_solution = false;
        search(state, 0, bound, heuristic, solution, found_solution);
        if (found_solution) {
            reverse(solution.begin(), solution.end());
            return solution;
        } else if (bound == INF) {
            return solution; // no se encontro solucion
        } else {
            bound++;
        }
    }
}

//Main para imprimir solo la solucion
int main() {
    State initial;
    srand(time(nullptr));
    for (int i = 0; i < 100; i++) {
        vector<State> succ = successors(initial);
        int rand_index = rand() % succ.size();
        initial = succ[rand_index];
    }
    cout << "Tablero inicial:" << endl;
    initial.print();
    vector<State> solution = ida_star_search(initial, heuristic);
    if (solution.empty()) {
        cout << "No se encontro solucion" << endl;
    } else {
        cout << "Solucion encontrada con costo " << solution.size()-1 << endl;
        cout << " " << endl;
        State final_state = solution.back();
        cout << "Tablero final:" << endl;
        final_state.print();
    }
    return 0;
}

/*
//Main para imprimir paso por paso
int main() {
    State initial;
    srand(time(nullptr));
    for (int i = 0; i < 100; i++) {
        vector<State> succ = successors(initial);
        int rand_index = rand() % succ.size();
        initial = succ[rand_index];
    }
    cout << "Tablero inicial:" << endl;
    initial.print();
    vector<State> solution = ida_star_search(initial, heuristic);
    if (solution.empty()) {
        cout << "No se encontr� soluci�n" << endl;
    } else {
        cout << "Solucion encontrada con costo " << solution.size()-1 << ":" << endl;
        for (State s : solution) {
            s.print();
        }
    }
    return 0;
}
*/
