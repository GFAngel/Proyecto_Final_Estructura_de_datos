#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ancho = 20;
const int alto = 20;

bool gameover;
int score;

enum Direccion { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direccion dir;

// Nodo para representar el cuerpo de la serpiente
struct Nodo {
    int x, y;
    Nodo* siguiente;
};

Nodo* cabeza = nullptr; // Inicio de la serpiente
Nodo* cola = nullptr;   // Final de la serpiente (para eliminar cuando no come)

int comidaX, comidaY;

// Inicializar juego
void setup() {
    srand(time(0));
    gameover = false;
    dir = STOP;
    score = 0;

    // Crear la cabeza de la serpiente al centro
    cabeza = new Nodo{ancho / 2, alto / 2, nullptr};
    cola = cabeza;

    // Crear comida
    comidaX = rand() % (ancho - 2) + 1;
    comidaY = rand() % (alto - 2) + 1;
}

// Dibujar la pantalla
void draw() {
    system("cls");

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            bool impreso = false;

            // Bordes
            if (i == 0 || i == alto - 1 || j == 0 || j == ancho - 1) {
                cout << "#";
                continue;
            }

            // Comida
            if (i == comidaY && j == comidaX) {
                cout << "o";
                continue;
            }

            // Cuerpo de la serpiente
            Nodo* temp = cabeza;
            while (temp != nullptr) {
                if (temp->x == j && temp->y == i) {
                    cout << (temp == cabeza ? '^' : '*');
                    impreso = true;
                    break;
                }
                temp = temp->siguiente;
            }

            if (!impreso) cout << " ";
        }
        cout << endl;
    }

    cout << "Puntaje: " << score << endl;
    cout << "Controles: W A S D | Salir: X" << endl;
}

// Entrada del usuario
void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'x':
            gameover = true;
            break;
        }
    }
}

// Verifica si hay colisión con el cuerpo
bool colisionCuerpo(int x, int y) {
    Nodo* temp = cabeza->siguiente;
    while (temp != nullptr) {
        if (temp->x == x && temp->y == y) return true;
        temp = temp->siguiente;
    }
    return false;
}

// Elimina el último nodo (cola)
void eliminarCola() {
    if (cabeza == cola) return; // No hay cuerpo, solo cabeza

    Nodo* temp = cabeza;
    while (temp->siguiente != cola) {
        temp = temp->siguiente;
    }

    delete cola;
    cola = temp;
    cola->siguiente = nullptr;
}

// Lógica del juego
void logic() {
    if (dir == STOP) return;

    // Obtener nueva posición
    int nuevoX = cabeza->x;
    int nuevoY = cabeza->y;

    switch (dir) {
    case LEFT:  nuevoX--; break;
    case RIGHT: nuevoX++; break;
    case UP:    nuevoY--; break;
    case DOWN:  nuevoY++; break;
    default: break;
    }

    // Verificar colisión con bordes
    if (nuevoX <= 0 || nuevoX >= ancho - 1 || nuevoY <= 0 || nuevoY >= alto - 1) {
        gameover = true;
        return;
    }

    // Colisión con el cuerpo
    if (colisionCuerpo(nuevoX, nuevoY)) {
        gameover = true;
        return;
    }

    // Crear nuevo nodo en la cabeza
    Nodo* nuevo = new Nodo{nuevoX, nuevoY, cabeza};
    cabeza = nuevo;

    // Verificar si comió
    if (nuevoX == comidaX && nuevoY == comidaY) {
        score++;
        comidaX = rand() % (ancho - 2) + 1;
        comidaY = rand() % (alto - 2) + 1;
    } else {
        eliminarCola(); // Si no comió, eliminamos la cola
    }
}

// Liberar memoria al finalizar
void limpiar() {
    while (cabeza != nullptr) {
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

int main() {
    setup();
    while (!gameover) {
        draw();
        input();
        logic();
        Sleep(100); // Controla velocidad
    }

    cout << "\nGAME OVER - Puntaje final: " << score << endl;
    limpiar();
    system("pause");
    return 0;
}
