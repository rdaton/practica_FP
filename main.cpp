#include <stdio.h>
#include <string>
#include <iostream>
#include <time.h>       //para random
#include <cassert>  
using namespace std;

//mucho código visto en http://www.cplusplus.com/reference/

//¡Ojo! concatenar y sumar strings es muy costoso y lento (en java,
//se suelen usar objetos de la clase stringBuffer, p.e.
//este es un programita pequeño, y no voy a usar operaciones
//rápidas de concatenación de strings


//por otro lado hay muchas funciones pequeñas, que en realidad
//deberían ser inline ...
//el problema es que no puedes poner un breakpoint en una inline
//por lo que son dificiles de depurar (al menos que uses -Od)

//en cuanto a seguridad, no estoy comprobando la longitud de ninguna
//cadena de entrada... Cuando debería hacerlo...
//me voy a fiar de que string es más o menos seguro

//acerca del rendimiento, lo ideal sería usar cadenas binarias 
//para almacenar las posiciones de los colores... (tipo 01110111 ), en vez
//de [C] [Z] [M] ,etc..
//me he quedado sin tiempo, y lo haré de forma poco eficiente...
//estos días os pasaré la versión eficiente

//Constantes

const int TAM_CODIGO=4;

const string textoBienvenida="\n \
		\
[ Este programa no usa tildes por motivos tecnicos ] \n \n \
				\
Mastermind \n \
				\
==========  \n \n \
	\
Descubre el codigo secreto! En cada partida, pensare un codigo de \n \
	\
colores que tendras que adivinar. En cada intento que hagas te \n \
	\
dare pistas, diciendote cuantos colores de los que has dicho estan \n \
	\
bien colocados, y cuantos no. \n \n \
	\
Averigua el codigo secreto en el menor numero posible de intentos! \n \n";

const int nOpciones=3;
const string opciones[]= {\
	"Salir \n \n", \
	"Jugar con un codigo sin colores repetidos \n", \
	"Jugar con un codigo con colores repetidos \n", \
};

const int maxIntentos=15;
const string eligeOpcion="Elige una opcion: ";
const string eligeOtraVez="Opcion incorrecta. Prueba otra vez: ";
const string dMuestraCodigo="[INFO para depuración] Código Secreto: ";
const string strEncontro= "Enhorabuena! Lo encontraste!";
const string strPerdio= "Ha perdido! Ha superado los " \
						+to_string(maxIntentos) \
						+" intentos";
const string strEligeOtraVezCadena="Cadena incorrecta. Prueba otra vez: ";

//Tipos de datos
const int nColores=6;
const int primerColor=1;
//aprovecho que los enum son ordinales, y se almacenan como enteros
//desde 0 ..
//así podré usarlos de indice para recuperar letra de colores
typedef enum tColor {INCORRECTO, ROJO, AZUL, VERDE, AMARILLO, MARRON,\
	 BLANCO} tColor;
const char misLetrasColores[]={'*','R','Z','V','A','M','B'};

typedef tColor tCodigo [TAM_CODIGO];


//Funciones asociados a tipos de datos
char color2char (tColor unColor)
{	
	return misLetrasColores[unColor];
}

//tengo un bug raro con el return
//que me sale un string muy raro inacabado,
//si intento acceder a los dos ultimos miembros de letrasColores
string mostrarColoresMenosDos ()
{
	string salida="";
	for (int i=primerColor;i<nColores-1;i++)
	{
		salida+=color2char((tColor)i);
		salida+=", ";
	};
return salida;
}

//cadena de instrucciones.... no influye mucho
const string strLeeCodigoMenosDos="Introduce el codigo (palabra de "+ \
				to_string(TAM_CODIGO)+" letras con alguna de " \
				+mostrarColoresMenosDos();

//la voy a hacer universal, a costa de hacer un bucle... (lo suyo
//sería usar una tabla hash... pero como que canta un poco
//ponerlo en una práctica de primero

tColor char2color(char unChar)
{
	//convierto a mayuscula
	char otroChar=toupper(unChar);
	bool enc=false;
	int i=primerColor;
	while(!enc && (i<=nColores))
	{
		enc=(misLetrasColores[i]==otroChar);
		i++;
	}
	if(enc)  //en C, se pueden hacer muchas trampas.. y aqui hago una
		return (tColor) (i-1);
		else
		return (tColor) 0; //color 0 es color 'INCORRECTO'
}


void codigoAleatorio(tCodigo& codigo, bool admiteRepetidos)
{
	int numeroAleatorio=0;
	//array dinámico de colores (de momento hago un [0,1,2,3,4,5...]
	int longColores=nColores;
	tColor conjuntoColores[longColores+primerColor];
	for (int i=0;i<=longColores+primerColor;i++)
	{
		conjuntoColores[i]=(tColor) i;
	}
	
	if (admiteRepetidos) //fácil
	{
		for (int i=0;i<TAM_CODIGO;i++)
		{
			numeroAleatorio=rand()%longColores+primerColor;
			//debug
			
			printf ("%d",numeroAleatorio);
			printf ("%c",color2char(conjuntoColores[numeroAleatorio]));
			
			//fin debug
			codigo[i]=conjuntoColores[numeroAleatorio];
		}
		
	}
	else
	{	//lo suyo sería lanzar excepción, pero me da pereza.
		assert (longColores>=TAM_CODIGO);
		for (int i=0;i<TAM_CODIGO;i++)
		{
			numeroAleatorio=rand()%longColores+primerColor;
			codigo[i]=conjuntoColores[numeroAleatorio];
			//muevo array de colores disponibles a la izquierda
			//y reduzco longitud de array con una unidad
			//i.e. machaco el color que cogí 
			for (int j=numeroAleatorio;j<=longColores;j++)
				{
					conjuntoColores[j]=conjuntoColores[j+1];
				}				
			longColores--;
		}
	}
}

//algún día haré una versión eficiente que se haga con operaciones
//binarias en vez de for

//comparaCodigos compara códigos correctos y bien formateados
void compararCodigos (const tCodigo codigo, const tCodigo hipotesis, \
int& colocados, int &descolocados) 
{
	colocados=0;
	descolocados=0;
	for (int i=0;i<TAM_CODIGO;i++)
	{
		if ((codigo[i])==hipotesis[i])
			{
				colocados++;
			}
			else
			{
				descolocados++;
			}
	}
	
}


/*
void borrarPantalla()
{
	cout << string(25, '\n');
}
*/
void imprimir(const string unString)
{
	printf("%s",unString.c_str());
}

void imprimirOpciones()
{
	for (int i=1;i<nOpciones;i++)
	{	
		imprimir("		"+to_string(i)+". "+opciones[i]);		
	};
	
	imprimir("\n");
	imprimir("		"+to_string(0)+". "+opciones[0]);
}


bool opcionCorrecta(int opt)
{
	return ((opt>=0) && (opt<nOpciones));
}

// clase string tiene, de fábrica, tamaño máximo, así que no van a pasar cosas raras
int stringAentero(string unString)
{
	//atoi cuando recibe string vacío, devuelve 0.... lo voy a apañar
	//devolviendo -1
	
	if (unString.length()==0) return -1;
	return atoi(unString.c_str());
}

int leerOpcion()
{
	string unString="";
	int opt=-1;
	
	imprimir("		");
	imprimir(eligeOpcion);
	getline(cin,unString);
	opt=stringAentero(unString);
	while (!opcionCorrecta(opt))
	{
		imprimir("		");
		imprimir(eligeOtraVez);
		getline(cin,unString);
		opt=stringAentero(unString);
	}
return opt; //la opción es correcta	
}

bool parsearCodigo(tCodigo& unCodigo, string unasLetras)
{
	tColor unColor=INCORRECTO;
	bool error=false;
	if (unasLetras.length() > TAM_CODIGO) return false;
	int i=0;
	while(!error && (i<TAM_CODIGO))
	{
		unColor=char2color(unasLetras[i]);
		error=(unColor==INCORRECTO);
		if(!error)
		{
			unCodigo[i]=unColor;
		}
		i++;		
	}	
	return !error;
}
bool dameCodigoJugador(tCodigo& nuevoCodigo)
{
	string letrasCodigo="";
	bool esCorrecto=false;
	//recupero las dos ultimas letras de colores
	//por ese bug raro de arrays sin acabar
	string unString="";
	unString+=color2char((tColor)(nColores-1));
	unString+=" o ";
	unString+=color2char((tColor)nColores);	
	imprimir(strLeeCodigoMenosDos+unString+"): ");
	//almaceno y valido codigoJugador
	int i=0;
	while(!esCorrecto)
	{
		if (i>0)
		{
			imprimir("	");
			imprimir(strEligeOtraVezCadena);
		};
		getline(cin,letrasCodigo);
		esCorrecto=parsearCodigo(nuevoCodigo,letrasCodigo);
		i++;
	}
	return esCorrecto;	
}

string codigoString(const tCodigo unCodigo)
{
	string salida="";
	for (int i=0;i<TAM_CODIGO;i++)
	{
		salida+=color2char(unCodigo[i]);
	};
	return salida;
}
void bucleMain()
{
	tCodigo codigoSecreto;
	tCodigo codigoJugador;
	bool conRepetidos=false;
	bool exito=false;
	int numColocados=0;
	int numDescolocados=0;
	bool salir=false;
	int opt=-1;	
	while(!salir)
	{
		exito=false;
		imprimirOpciones();
		opt=leerOpcion();
		if (opt==0)
		{
			salir=true;
			break;
		};
		conRepetidos=(opt==2);
		//la opción es correcta
		codigoAleatorio(codigoSecreto, conRepetidos);
		imprimir("			");		
		imprimir(dMuestraCodigo);
		imprimir(codigoString(codigoSecreto));
		imprimir("\n");
		int r=0;
		while(!exito &&(r<maxIntentos))
		{
			//inicializo comparadores
			numColocados=0;
			numDescolocados=0;
			dameCodigoJugador(codigoJugador);
			//debug
			//imprimir(codigoString(codigoJugador));
			//imprimir("\n");
			//fin debug
			compararCodigos(codigoSecreto,codigoJugador,numColocados,numDescolocados);
			r++;
			//otra vez, me salto lo de las excepciones...
			//por hacer una comprobación, no hace daño
			assert ((numColocados+numDescolocados)==TAM_CODIGO);
			exito=(numColocados==TAM_CODIGO);
		}
		if (exito)
		{
			imprimir("\n\n");
			imprimir("		");		
			imprimir(strEncontro);
			imprimir("\n\n");
			imprimir("	Te ha costado "+to_string(r)+" intento(s).");
			imprimir("\n\n");
		}
		else
		{
			imprimir("\n\n");
			imprimir("		");		
			imprimir(strPerdio);
			imprimir("\n\n");
		}
		
	}
	
	
}


int main(int argc, char **argv)
{
	
	srand (time(NULL)); //inicalizo random según hora
	imprimir(textoBienvenida);
	
	bucleMain();
	return 0;
   
}
