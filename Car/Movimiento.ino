#define PALANTE 0x01
#define PATRAS 0x02

int IN3 = 5; 
int IN4 = 4;
int IN1 = 1;
int IN2 = 2;
int ENA = 6;
int ENB = 7;
int pinEncoder=9;

int numCambios=0;



void setup()
{
  pinMode (IN1, OUTPUT);    // Input4 conectada al pin 4 
  pinMode (IN2, OUTPUT);    // Input3 conectada al pin 5
  pinMode (IN3, OUTPUT);    // Input4 conectada al pin 4 
  pinMode (IN4, OUTPUT);    // Input3 conectada al pin 5
  pinMode (ENA, OUTPUT);    // Input4 conectada al pin 4 
  pinMode (ENB, OUTPUT);    // Input3 conectada al pin 5
}
void loop()
{
  LineaRecta(PALANTE,120); //Direccion avanceo retroceso, y veocidad 
  Distancia(50);
  Gira(90,50); //giro en grados, y velocidad de giro
  LineaRecta(PALANTE,120);
  Distancia(30);

  LineaRecta(PATRAS,120);
  Distancia(30);
  Gira(-90,50);
  LineaRecta(PATRAS,120);
  Distancia(50);  
  
  

}



void LineaRecta(int y,int velocidad){
  switch (y){
    case 1:
      digitalWrite (IN4, HIGH); //Las dos ruedas TIENEN que girar en el mismo sentido
      digitalWrite (IN3, LOW);
      
      digitalWrite (IN2, HIGH);
      digitalWrite (IN1, LOW);
      
    break;
    
    case 2:
    digitalWrite (IN4, LOW); //Las dos ruedas TIENEN que girar en el mismo sentido
      digitalWrite (IN3, HIGH);
      
      digitalWrite (IN2, LOW);
      digitalWrite (IN1, HIGH);
    
    break;
  }
  
      analogWrite (ENA, velocidad);
      analogWrite (ENB, velocidad);
  
  
  } 
 
void Distancia( int dientes){  //Sabemos que cada diente equivale a 1cm aprox.
  int numCambios=0;
  int anterior=0;
  int encoder=0;
  
  
  while( numCambios < dientes ){
  encoder = digitalRead(pinEncoder);
    numCambios += anterior ^ encoder;
    anterior=encoder; 
    if (encoder != anterior) {
      anterior=encoder;
      numCambios += 1;
    }
  } 
}  

void Gira(int grados,int velGiro){
  int gradosDiente = 1; //Numero de ranuras del encoder que hace que gire un grado
  int dientes = 0;
  
  dientes=grados/gradosDiente;
  
  if (grados > 0){
    
      digitalWrite (IN4, HIGH); //Las dos ruedas TIENEN que girar en DISTINTO sentido
      digitalWrite (IN3, LOW);
      
      digitalWrite (IN2, LOW);
      digitalWrite (IN1, HIGH);
  }
  else{
      dientes = -1*dientes;
      digitalWrite (IN4, LOW); //Las dos ruedas TIENEN que girar en DISTINTO sentido
      digitalWrite (IN3, HIGH);
      
      digitalWrite (IN2, HIGH);
      digitalWrite (IN1, LOW);   
  }
  
  analogWrite (ENA, velGiro);
  analogWrite (ENB, velGiro);
  Distancia(dientes);
  


}
  
