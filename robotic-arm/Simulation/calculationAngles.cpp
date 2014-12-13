#include <iostream>
#include <math.h>
using namespace std;
const float PI_F=3.14159265358979f;

// Características físicas
const float BaseX=0;						//Posición X de la base del brazo.
const float BaseY=0;						//Posición Y de la base del brazo.
const float LongBrazo=150;			//Longitud del brazo (primer segmento).
const float LongAntBr=150;			//Longitud del antebrazo (segmento intermedio).
const float LongMunec=50;			//Longitud de la muñeca (último segmento).

// Función para calcular la cinemática inversa.
void cinematicaInv(float x, float y, float c, float & anguloBrazo, float & anguloAntebrazo, float & anguloMuneca);


int main() {

	// Objetivos
	float Cabeceo=-PI_F/4;	//Ángulo de la muñeca respecto al plano horizontal.
	float X=50;						//Posición X del efector
	float Y=150+BaseY;		//Posición Y del efector

	// Ángulos de los motores
	float AngBrazo;	//Ángulo del motor del brazo (situado en la base).	
	float AngAntBr;	//Ángulo del motor del antebrazo.
	float AngMunec;	//Ángulo del motor de la muñeca.

	// Calculamos
	cinematicaInv(X,Y,Cabeceo, AngBrazo,AngAntBr,AngMunec);

	// Mostramos el resultado
	cout << X << " " << Y << " " << Cabeceo << endl;
	cout << AngBrazo << " " << AngAntBr << " " << AngMunec << endl;

	return 0;
}


void cinematicaInv(float x, float y, float c, float & anguloBrazo, float & anguloAntebrazo, float & anguloMuneca) {
	float Afx;		//Posición X del efector respecto el motor de la muñeca.
	float Afy;		//Posición Y del efector respecto el motor de la muñeca.
	float LadoA;	//Posición Y del motor de la muñeca respecto a la base.
	float LadoB;	//Posición X del motor de la muñeca respecto a la base.

	float Hipotenusa;

	float Alfa;	//Ángulo de la recta que une la base con el motor de la muñeca respecto al plano horizontal.
	float Beta;	// Ángulo de la recta que une la base con el motor del antebrazo respecto a la recta que une la base con el motor de la muñeca.

	float Gamma;

	Afx = cos(c)*LongMunec;
	LadoB = x-Afx;
	Afy = sin(c)*LongMunec;
	LadoA = y - Afy - BaseY;

	Hipotenusa = sqrt((LadoA*LadoA)+(LadoB*LadoB));

	Alfa = atan2(LadoA,LadoB);

	Beta = acos((LongBrazo*LongBrazo - LongAntBr*LongAntBr + Hipotenusa*Hipotenusa) / (2.0*LongBrazo*Hipotenusa));

	anguloBrazo = Alfa+Beta;
	Gamma = acos((LongBrazo*LongBrazo + LongAntBr*LongAntBr - Hipotenusa*Hipotenusa)/(2.0*LongBrazo*LongAntBr));
	anguloAntebrazo = -(PI_F-Gamma);
	anguloMuneca = c-anguloBrazo-anguloAntebrazo;

	anguloBrazo = anguloBrazo*180.0/PI_F;
	anguloAntebrazo = anguloAntebrazo*180.0/PI_F;
	anguloMuneca = anguloMuneca*180.0/PI_F;
}
