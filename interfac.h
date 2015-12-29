/*
Numero Gruppo: 4AI02
Componenti: D'Ambrosio Antonio, Martina Stefano
Titolo esercitazione: Battaglia navale
Data inizio: 13/11/2002
Data fine:
*/
//#include"sprites.h"
#include<graphics.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<mem.h>
#include<GRAPHICS.H>
#include<conio.h>
#include<string.h>

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef struct tagBITMAP              /* the structure for a bitmap. */
{
  word width;	//larghezza
  word height;	//altezza
  byte *data;
} BITMAP;

#define DIMCASELLA 51
#define XBOTTONE 119
#define YBOTTONE 50
#define DIMTOTALSPRITE 100
#define DIMINFO 45


enum zone{GRIGLIA, PULSANTIERA, NULLA};

class Interfac{
	private:
		void fskip(FILE*, int);
		void load_bmp(char*, BITMAP&);
		void draw_bmp_oriz(BITMAP, int, int, int);	//(bitmap, x, y, colore transp)
		void draw_bmp_vert(BITMAP, int, int, int);
		void matrixCopy(int[DIMCASELLA][DIMCASELLA],int[DIMCASELLA][DIMCASELLA]);//copia una matrice in un'altra della dimensione specificata
		int gridEff(int);	//restituisce la coordinata x o y effettiva passandogli quella logica della griglia;
		void buttonPos(int, int&, int&, int&, int&);//(pos, west, nord, est, sud) restituisce le coordinate del bottone con la posizione indicata
		char lastInfo[DIMINFO];	//ultimo info
	public:
		Interfac();
		~Interfac();
		void base();			//disegna la base grafica del gioco
		void casella(int, int, char*,int);//(x logica, y logica, sprite, ruotato?) disegna una casella nella posizione indicata del tipo indicato, indica se disegnare lo sprite diritto (se falso) o ruotato di 90 gradi a sinistra (se vero), col colore indicato come trasparente
		void bottone(int, char*);	//(numero, sprite) disegna il bottone in posizione indicata con la texture indicata
		void bottonSwitchOn(int);      	//colora la selezione del bottone indicato
		void bottonSwitchOff(int);	//colora la deselezione del bottone indicato
		int info(char*);		//visualizza la stringa nel "Message Box"
		zone whotIsThis(int, int);	//restituisce la zona indicata dalle var
		int getCasella(int&, int&); 	//Gli si passa le coord fisiche e restituisce le logiche nelle stesse variabili
		int getBottone(int, int, int&);		//gli si passa le coord fisiche e restituisce la posizione del bottone
		void immagine(int, int, char*, int, int);	//(x assoluta, y, sprite, ruotato, colore invisibile)disegna bitmap in posizione indicata
};

