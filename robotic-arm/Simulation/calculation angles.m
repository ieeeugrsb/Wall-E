
%Calculo y simulación brazo
%-----------------------------------------------------------------------------------------------------
%Descripción: Calculo y simulación de los angulos de un brazo de 3 grados de libertad dado una posicion X,Y en el plano
%
%Nicolás Guerrero Garcia
%
%Falta por añadir:
%-Detectar cuando el brazo no puede llegar al objetivo
%-Cuando se obtiene un angulo mayor que 180º desechar-> inalcanzable 
%
%Referencia: https://sites.google.com/site/proyectosroboticos/cinematica-inversa-ii

BaseX  = 0;      
BaseY  = 0 ;   
cabeceo=-pi/4;
x=50;
y=150+BaseY;

LongMunec=50;
LongBrazo=150;
LongAntBr=150;

%%
afx=cos(cabeceo)*LongMunec;

ladob=x-afx;
afy=sin(cabeceo)*LongMunec;
ladoa=y-afy-BaseY;
hipotenusa=sqrt((ladoa^2)+(ladob^2));
alfa =atan2(ladoa,ladob);
beta=acos((LongBrazo^2-LongAntBr^2+hipotenusa^2)/(2*LongBrazo*hipotenusa));
AngBrazo=alfa+beta;
gamma=acos((LongBrazo^2+LongAntBr^2-hipotenusa^2)/(2*LongBrazo*LongAntBr));
AngAntBr=-(pi-gamma);
AngMunec= cabeceo-AngBrazo-AngAntBr;



%Puntos para poder dibujar
%--------------------------------

PXa=LongBrazo* cos(AngBrazo);
PYa=LongBrazo*-sin(AngBrazo);

PXb=LongAntBr* cos(AngAntBr+AngBrazo);
PYb=LongAntBr*-sin(AngAntBr+AngBrazo);
PXc=LongMunec*cos(AngMunec+AngAntBr+AngBrazo);
PYc=LongMunec*-sin(AngMunec+AngAntBr+AngBrazo);

%Puntos para dibujar el brazo
%----------------------------
BrazoPY=PYa+BaseY;           
BrazoPX=PXa+BaseX;           

AntBrPY=PYb+PYa+BaseY;    
AntBrPX=PXb+PXa+BaseX;       
 

MunecPY=PYc+PYb+PYa+BaseY;   
MunecPX=PXc+PXb+PXa+BaseX;  

x1=[0 BrazoPX AntBrPX MunecPX];
y1=[0 -BrazoPY -AntBrPY -MunecPY];
plot(x1,y1,'r');
disp('Angulo Brazo (en grados)');
disp(AngBrazo*180/pi);
disp('Angulo Antebrazo (en grados)');
disp((AngAntBr*180/pi)+180);
disp('Angulo muñeca (en grados)');
disp((AngMunec*180/pi)+180);
