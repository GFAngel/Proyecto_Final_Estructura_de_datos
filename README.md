#include<iostream>
#include<conio.h>

using namespace std;

bool gameover;
int alto = 20;
int ancho = 20;
int serpix;
int serpiy;

void setup();
void draw();
void input();
void logic();

int main() {
    setup();
    while(!gameover) {
        draw();
        input();
        logic();
    }
    return 0;
}

void setup() {
    gameover= false;
    serpix=ancho/ 2;
    serpiy=alto/ 2;
}

void draw() {
    system("cls");
    for (int i= 0; i< alto;i++) {
        for (int j=0;j<ancho;j++) {
            
            if (i == 0||i == alto - 1||j == 0 ||j == ancho - 1) {
                cout <<"#";//el borde que sera las esquinas para limitar el espacio de la serpiente
            }
            
            else if (i == serpiy && j == serpix) {
                cout <<"^";  //cabesa de la serpiento 
            }
            else {
                cout <<" ";
            }
        }
        cout << endl;
    }
}

void input() {
    if(_kbhit()){
        switch (_getch())
        {
        case constant expression:
            /* code */
            break;
        
        default:
            break;
        }
    }
}

void logic() {
    
}
