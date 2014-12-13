#include <Servo.h>

// CONSTANTES

const float PI_F=3.14159265358979;

// Características físicas
const float BaseX=0;						//Posición X de la base del brazo.
const float BaseY=0;						//Posición Y de la base del brazo.
const float LongBrazo=150;			//Longitud del brazo (primer segmento).
const float LongAntBr=150;			//Longitud del antebrazo (segmento intermedio).
const float LongMunec=50;			//Longitud de la muñeca (último segmento).


// PINES

// Motores (salidas PWD) ??
const int PIN_BRAZO=9;
const int PIN_ANTEBRAZO=10;
const int PIN_MUNECA=11;
Servo brazo, antebrazo, muneca;

// Sensores (entradas analogicas)
const int PIN_X=A0;
const int PIN_Y=A1;
const int PIN_CABECEO=A2;


// FUNCIONES

// Función para calcular la cinemática inversa.
void cinematicaInv(float x, float y, float c, float & anguloBrazo, float & anguloAntebrazo, float & anguloMuneca);


// INICIALIZACION

void setup() {
//  brazo.attach(PIN_BRAZO);
//  antebrazo.attach(PIN_ANTEBRAZO);
//  muneca.attach(PIN_MUNECA);
  
//  pinMode(PIN_X,INPUT);
//  pinMode(PIN_Y,INPUT);
//  pinMode(PIN_CABECEO,INPUT);  
  
  Serial.begin(9600);
}


// BUCLE PRINCIPAL
void loop() {

  // Leemos los objetivos de los sensores
  float X;		//Posición X del efector
  float Y;		//Posición Y del efector
  float Cabeceo;	//Ángulo de la muñeca respecto al plano horizontal.
//  X = 50;
//  Y = 150+BaseY;
  Cabeceo = -PI_F/4.0;
  X = analogRead(PIN_X);
  Y = analogRead(PIN_Y);
//  Cabeceo = analogRead(PIN_CABECEO);

  X = map(X,0,1023,-200,200) + BaseX;
  Y = map(Y,0,1023,-200,200) + BaseY;
//  Cabeceo = map(Cabeceo,0,1023,-PI_F,PI_F);
  
  // Ángulos de los motores
  float AngBrazo;	//Ángulo del motor del brazo (situado en la base).	
  float AngAntBr;	//Ángulo del motor del antebrazo.
  float AngMunec;	//Ángulo del motor de la muñeca.

  // Calculamos
  cinematicaInv(X,Y,Cabeceo, AngBrazo,AngAntBr,AngMunec);

  // Ajustamos los motores
//  brazo.write(AngBrazo);
//  antebrazo.write(AngAntBr);
//  muneca.write(AngMunec);  
  
  // Imprimimos en el monitor serie la informacion de entrada y de los motores
  Serial.println("");
  Serial.println("Sensores: ");
  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Cabeceo: ");
  Serial.println(Cabeceo);

  Serial.println("Motores: ");
  Serial.print("Brazo: ");
  Serial.print(AngBrazo);
  Serial.print("  AngAntBr: ");
  Serial.print(AngAntBr);
  Serial.print("  AngMunec: ");
  Serial.println(AngMunec);
  
  delay(1000);
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
  Gamma = acos((LongBrazo*LongBrazo + LongAntBr*LongAntBr - Hipotenusa*Hipotenusa)/(2.0*LongBrazo*LongAntBr));
  anguloAntebrazo = anguloAntebrazo*180.0/PI_F + 180.0;
  anguloMuneca = anguloMuneca*180.0/PI_F + 180.0;
}

