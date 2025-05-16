#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> 

using namespace std;

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

// Función para contar minas adyacentes
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
void colocarMinas(vector<Pos>& minas, int filas, int columnas, int nminas) {
    srand(time(0)); 
    while (minas.size() < nminas) {
        int f = rand() % filas;
        int c = rand() % columnas;
        Pos nueva = {f, c};
        if (find(minas.begin(), minas.end(), nueva) == minas.end()) {
            minas.push_back(nueva);
        }
    }
    sort(minas.begin(), minas.end()); // Necesario para búsqueda binaria
}

void revelarCasillasVacias(int fila, int columna, const vector<Pos>& minas, vector<Pos>& descubiertas, int filas, int columnas) {
    // Verificar si la posición ya fue descubierta
    if (find(descubiertas.begin(), descubiertas.end(), Pos{fila, columna}) != descubiertas.end()) {
        return;
    }
    
    // Añadir a descubiertas
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
        system("cls");
        cout << "Numero de minas: " << nminas << " | Banderas restantes: " << nbanderas << " | Estado: " << estado << endl;

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
                            cout << "[" << minasCerca << "]"; // número de minas adyacentes
                        } else {
                            cout << "[ ]"; // casilla vacía sin minas cerca
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
            // Verificar si ya está descubierta o tiene bandera
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
                // Revelar casilla y posiblemente areas vacías
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
        cout << "¡Has ganado! Estado: " << estado << endl;
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
}
