#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm> 


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
                    cout<<(temp == cabeza?'~':'~');
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

//buaca minas
struct Pos {
    int fila;
    int columna;

    bool operator<(const Pos& other) const {
        return (fila < other.fila) || (fila == other.fila && columna < other.columna);
    }

    bool operator==(const Pos& other) const {
        return fila == other.fila && columna == other.columna;
    }
};

// Funcion para contar minas adyacentes
int contarMinasAdyacentes(const vector<Pos>& minas, int fila, int columna, int filas, int columnas) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; 
            int f = fila + i;
            int c = columna + j;
            if (f >= 0 && f < filas && c >= 0 && c < columnas) {
                if (binary_search(minas.begin(), minas.end(), Pos{f, c})) {
                    count++;
                }
            }
        }
    }
    return count;
}
//Funcion para colocar minas
void colocarMinas(vector<Pos>& minas, int filas, int columnas, size_t nminas) {
    srand(time(0)); 
    while (minas.size() < nminas) {
        int f = rand() % filas;
        int c = rand() % columnas;
        Pos nueva = {f, c};
        if (find(minas.begin(), minas.end(), nueva) == minas.end()) {
            minas.push_back(nueva);
        }
    }
    sort(minas.begin(), minas.end()); // Necesario para busqueda binaria
    cout << "Posiciones de las minas: " << endl;
    for (const auto& mina : minas) {
        cout << "Fila: " << mina.fila << ", Columna: " << mina.columna << endl;
    }
}

void revelarCasillasVacias(int fila, int columna, const vector<Pos>& minas, vector<Pos>& descubiertas, int filas, int columnas) {
    // Verificar si la posicion ya fue descubierta
    if (find(descubiertas.begin(), descubiertas.end(), Pos{fila, columna}) != descubiertas.end()) {
        return;
    }
    
    // A�adir a descubiertas
    descubiertas.push_back({fila, columna});
    
    // Si tiene minas adyacentes, no revelar mas
    if (contarMinasAdyacentes(minas, fila, columna, filas, columnas) > 0) {
        return;
    }
    
    // Revelar casillas adyacentes
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int f = fila + i;
            int c = columna + j;
            if (f >= 0 && f < filas && c >= 0 && c < columnas) {
                if (!binary_search(minas.begin(), minas.end(), Pos{f, c})) {
                    revelarCasillasVacias(f, c, minas, descubiertas, filas, columnas);
                }
            }
        }
    }
}

void setgame(int filas, int columnas, int nminas) {
    char matriz[100][100];
    vector<Pos> minas;
    vector<Pos> descubiertas;
    int nbanderas = nminas;
    string estado = ":)";

    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            matriz[i][j] = ' ';

    // Colocar minas aleatorias
    colocarMinas(minas, filas, columnas, nminas);

    bool juegoTerminado = false;
    bool jugadorGano = false;

    while (!juegoTerminado) {
 
        cout <<"Numero de minas(M) :" << nminas << " | Banderas restantes(F) : " << nbanderas << " | Estado: " << estado << endl;

        // Mostrar matriz oculta
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                Pos actual = {i, j};
                if (find(descubiertas.begin(), descubiertas.end(), actual) != descubiertas.end()) {
                    if (binary_search(minas.begin(), minas.end(), actual)) {
                        cout << "[M]"; // mina 
                    } else {
                        int minasCerca = contarMinasAdyacentes(minas, i, j, filas, columnas);
                        if (minasCerca > 0) {
                            cout << "[" << minasCerca << "]"; // numero de minas adyacentes
                        } else {
                            cout << "[ ]"; // casilla vacia sin minas cerca
                        }
                    }
                } else if (matriz[i][j] == 'F') {
                    cout << "[F]";
                } else {
                    cout << "[ ]";
                }
            }
            cout << endl;
        }

        cout << "1) Buscar mina"<<endl;
        cout << "2) Colocar/Quitar bandera"<<endl;
        cout << "Seleccione una opcion: (1/2)"<<endl;
        int op;
        cin >> op;

        int x, y;
        cout << "Ingrese la posicion de la fila entre (0-" << filas - 1 << "): ";
        cin >> x;
        cout << "Ingrese la posicion de la columna entre (0-" << columnas - 1 << "): ";
        cin >> y;
        
        // Validar entrada
        if (x < 0 || x >= filas || y < 0 || y >= columnas) {
            cout << "Posicion invalida. Presione Enter para continuar...";
            cin.ignore();
            cin.get();
            continue;
        }
        
        Pos seleccion = {x, y};

        if (op == 1) { // Buscar mina
            // Verificar si ya esta descubierta o tiene bandera
            if (find(descubiertas.begin(), descubiertas.end(), seleccion) != descubiertas.end() || 
                matriz[x][y] == 'F') {
                continue;
            }
            
            if (binary_search(minas.begin(), minas.end(), seleccion)) {
                estado = ":(";
                juegoTerminado = true;
                // Revelar todas las minas al perder
                for (const auto& m : minas) {
                    descubiertas.push_back(m);
                }
            } else {
                // Revelar casilla y posiblemente areas vacias
                revelarCasillasVacias(x, y, minas, descubiertas, filas, columnas);
            }
        } else if (op == 2) { // Colocar/Quitar bandera
            // No permitir colocar banderas en casillas descubiertas
            if (find(descubiertas.begin(), descubiertas.end(), seleccion) != descubiertas.end()) {
                continue;
            }
            
            if (matriz[x][y] == 'F') {
                matriz[x][y] = ' ';
                nbanderas++;
            } else if (nbanderas > 0) {
                matriz[x][y] = 'F';
                nbanderas--;
            }
        }

        // Verificar si ganaste
        bool todasMinasMarcadas = true;
        for (const auto& m : minas) {
            if (matriz[m.fila][m.columna] != 'F') {
                todasMinasMarcadas = false;
                break;
            }
        }
        
        bool todasNoMinasDescubiertas = true;
        for (int i = 0; i < filas && todasNoMinasDescubiertas; ++i) {
            for (int j = 0; j < columnas && todasNoMinasDescubiertas; ++j) {
                Pos actual = {i, j};
                if (!binary_search(minas.begin(), minas.end(), actual)) {
                    if (find(descubiertas.begin(), descubiertas.end(), actual) == descubiertas.end()) {
                        todasNoMinasDescubiertas = false;
                    }
                }
            }
        }
        
        if ((todasMinasMarcadas && nbanderas == 0) || todasNoMinasDescubiertas) {
            estado = "8)";
            jugadorGano = true;
            juegoTerminado = true;
            // Revelar todas las minas al ganar
            for (const auto& m : minas) {
                if (matriz[m.fila][m.columna] != 'F') {
                    matriz[m.fila][m.columna] = 'F';
                }
            }
        }
    }

    system("cls");
    if (jugadorGano) {
        cout << "Has ganado! Estado: " << estado << endl;
    } else {
        cout << "BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM!!!!!!!! Has perdido. Estado: " << estado << endl;
    }

    // Mostrar tablero final
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            Pos actual = {i, j};
            if (binary_search(minas.begin(), minas.end(), actual)) {
                cout << "[M]";
            } else if (matriz[i][j] == 'F') {
                cout << "[F]";
            } else {
                int minasCerca = contarMinasAdyacentes(minas, i, j, filas, columnas);
                if (minasCerca > 0) {
                    cout << "[" << minasCerca << "]";
                } else {
                    cout << "[ ]";
                }
            }
        }
        cout << endl;
    }

    cout << "Desea reiniciar la partida? 1.Si 2.Salir al menu (1/2)";
    int r;
    cin >> r;
    if (r == 1) {
        setgame(filas, columnas, nminas);
    } else {
        cout << "Volviendo al menu";
    }
}

int main(){
    int op2;
    cout<<"que deseas jugar 1-busca mina 2-serpiente";
    cin>>op2;
    switch (op2)
    {
    case 1:
    int opc1;
    int difficulty;
    
    cout<<"Buscaminas"<<endl;
    cout<<"1).Nuevo juego"<<endl;
    cout<<"2).Regresar al menu"<<endl;
    cout<<"Seleccione una opcion (1/2): ";
    cin>>opc1;
    
    if(opc1==1){
        cout<<"Ingrese el nivel de dificultad: "<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"1)Principiante [4]X[4] (7 minas)"<<endl;
        cout<<"2)Intermedio [8]X[10] (10 minas)"<<endl;
        cout<<"3)Experto [15]X[17] (40 minas)"<<endl; 
        cout<<"(1/2/3): ";
        cin>>difficulty;
        switch(difficulty){
            case 1:     cout<<"Dificultad principiante"<<endl;
                        setgame(4,4,7);
                        break;
                        
            case 2:     cout<<"Difucultad Intermedio"<<endl;
                        setgame(8,10,10); 
                        break;
                        
            case 3:     cout<<"Dificultad Experto"<<endl;
                        setgame(15,17,40); 
                        break;
            default:    cout<<"Dificultad no valida";
                        break;
        }
    }
    else if(opc1==2){
        //Regresa a un menu que se integra con el juego de la serpiente
        cout<<"Regresando al Menu principal."<<endl;
    }
    else cout<<"Por favor seleccione una opcion valida."<<endl; 
    
    return 0;
        break;
    case 2:
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
        break;
    default:
        break;
    }
    
}
