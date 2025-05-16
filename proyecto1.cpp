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

struct Nodo {//cuerpo de la serpiente
    int x, y;
    Nodo* siguiente;
};

Nodo* cabeza=nullptr; // Inicio de la serpiente
Nodo* cola= nullptr;   // Final de la serpiente 

int comidaX, comidaY;


void setup() {
    srand(time(0));
    gameover = false;
    dir = STOP;
    score = 0;

    
    cabeza = new Nodo{ancho / 2,alto/2, nullptr};//central la cabesa de la serpiente
    cola = cabeza;
    comidaX = rand() % (ancho - 2) + 1;//colocar la manzana en lugares ramdon y evitar que salga en los bordes
    comidaY = rand() % (alto - 2) + 1;
}

void draw() {
    system("cls");

    for (int i =0; i < alto; i++) {
        for (int j =0; j < ancho; j++) {
            bool impreso = false;
            if (i == 0||i==alto-1||j==0||j==ancho-1) {
                cout<<"#";
                continue;
            }

            
            if (i ==comidaY && j == comidaX) {// Comida
                cout<<"8";
                continue;
            }

            
            Nodo* temp = cabeza;
            while (temp != nullptr) {
                if (temp->x==j && temp->y==i) {// Cuerpo de la serpiente
                    cout<<(temp == cabeza?'^':'*');
                    impreso=true;
                    break;
                }
                temp=temp->siguiente;
            }

            if(!impreso)cout<<" ";
        }
        cout<<endl;
    }

    cout<<"Puntaje:"<< score << endl;
    cout<<"Controles: W A S D | Salir: X"<< endl;
}

// Entrada del usuario
void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            if (dir!= DOWN) dir =UP;
            break;
        case 's':
            if (dir!= UP) dir =DOWN;
            break;
        case 'a':
            if (dir!= RIGHT) dir =LEFT;
            break;
        case 'd':
            if (dir!= LEFT) dir =RIGHT;
            break;
        case 'x':
            gameover = true;
            break;
        }
    }
}

bool colisionCuerpo(int x, int y) {
    Nodo* temp =cabeza->siguiente;
    while (temp != nullptr) {
        if (temp->x==x && temp->y==y) return true;
        temp = temp->siguiente;
    }
    return false;
}

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
    if (dir==STOP) return;

    int nuevoX =cabeza->x;
    int nuevoY =cabeza->y;

    switch (dir) {
    case LEFT:  nuevoX--;break;
    case RIGHT: nuevoX++;break;
    case UP:    nuevoY--;break;
    case DOWN:  nuevoY++;break;
    default: break;
    }

    // Verificar colisión con bordes
    if (nuevoX<=0||nuevoX>=ancho-1||nuevoY<=0||nuevoY>=alto -1) {
        gameover = true;
        return;
    }

    if (colisionCuerpo(nuevoX, nuevoY)) {
        gameover = true;
        return;
    }

    Nodo* nuevo = new Nodo{nuevoX, nuevoY, cabeza};
    cabeza = nuevo;

    // Verificar si comió
    if (nuevoX==comidaX && nuevoY == comidaY) {
        score++;
        comidaX=rand() % (ancho - 2) + 1;
        comidaY=rand() % (alto - 2) + 1;
    } else {
        eliminarCola(); 
    }
}


void limpiar() {
    while (cabeza!=nullptr) {
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;// Liberar memoria al finalizar
    }
}

int main() {
    

    setup();
    while(!gameover) {
        draw();
        input();
        logic();
        Sleep(100); 
    }

    cout<<"\nGAME OVER - Puntaje final: "<< score << endl;
    limpiar();
    system("pause");
    return 0;
}
