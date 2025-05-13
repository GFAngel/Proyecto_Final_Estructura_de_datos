#include<iostream>
#include<conio.h>

using namespace std;
bool gameover;
int alto = 20;
int ancho= 20;//esto sera el tamañoque tendra para la serpiente
void setup(); void draw(); void input(); void logic();

int main(){
    setup();
    while(!gameover){
        draw();
        input();
        logic();
    }
    return 0;
}

void setup(){//para finalizar el juego
    gameover= false;
}

void draw(){
    system("cls");
    for(int i=0;i<alto;i++){
        for (int j=0; j < ancho; j++)
        {
            if(i == 0 || i == alto - 1 || j == 0 || j == ancho - 1){
                cout<<"#";
            }
            else{
                cout<<" ";
            }
        }
        cout<<" ";
    }
}

void input(){

}
void logic(){

}
