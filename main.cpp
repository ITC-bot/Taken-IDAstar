#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <ctime>

using namespace std;

const int N = 4; // tamaño del tablero
const int MAX_DEPTH = 100; // profundidad máxima
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

    // Función para imprimir el tablero
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

// Función heurística para estimar la distancia de Manhattan
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

// Función para generar los sucesores de un estado
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

// Función de búsqueda en profundidad limitada con poda de subárboles
void search(State state, int g, int bound, int (*heuristic)(const State&), vector<State>& solution, bool& found_solution) {
    int f = g + heuristic(state);
    if (f > bound) {
        return; // excede la cota superior
    }
    if (heuristic(state) == 0) {
        found_solution = true;
        solution.push_back(state); // agrega el estado solución al vector
        return; // solución encontrada
    }
    int min_t = INF;
    vector<State> succ = successors(state);
    for (State s : succ) {
        if (s == state) {
            continue;
        }
        search(s, g+1, bound, heuristic, solution, found_solution);
        if (found_solution) {
            solution.push_back(state); // agrega el estado actual al vector
            return;
        }
    }
}

// Función de búsqueda IDA*
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
            return solution; // no se encontró solución
        } else {
            bound++;
        }
    }
}

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
        cout << "No se encontró solución" << endl;
    } else {
        cout << "Solucion encontrada con costo " << solution.size()-1 << ":" << endl;
        for (State s : solution) {
            s.print();
        }
    }
    return 0;
}

/*
Codigo para solo imprimir la solucion final
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
    if (!solution.empty()) {
        State final_state = solution.back();
        cout << "Tablero final:" << endl;
        final_state.print();
    } else {
        cout << "No se encontró solución." << endl;
    }
    return 0;
}
*/
