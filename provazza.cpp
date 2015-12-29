#include"interfac.cpp"
#include"sorcio.cpp"
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>


enum pippo{a =9, b =4, c =2};

void main(){
	int gdriver=DETECT, gmode, errorcode;
	char *drivername;

	clrscr();
	initgraph (&gdriver, &gmode, "c:\\borlandc\\bgi");   	//inizzializza grafica
	errorcode = graphresult();    			//controlla se errore
	if (errorcode != grOk){                         //se errore
		printf ("\nErrore grafico %d: %s ", errorcode, grapherrormsg (errorcode));//visualizza mess
		puts ("\nPremere un tasto per uscire");
		getch();
		exit(1);				//esce
	}


	Sorcio topo;
	Interfac disegna;
	disegna.base();
	int i, j;
	for (i=0;i <= 5; i++)
		for (j = 0; j<= 5; j++)
			disegna.casella(j,i,"altro\\mare.bmp",0);


	topo.reset();
	topo.cursEnab();
	int x, y, p;
	char string[45];
	char appo[10];
	do{
		topo.getStatus(x,y,p);
		strcpy(string, "X: ");
		itoa(x,appo,10);
		strcat(string, appo);
		strcat(string,"     Y: ");
		itoa(y,appo,10);
		strcat(string, appo);
		strcat(string,"     P: ");
		itoa(p,appo,10);
		strcat(string, appo);
		disegna.info(string);
		delay(100);
	}while((x != 0) || (y != 0));

	disegna.bottone(0, "altro\\bottvert.bmp");
	getch();
	disegna.bottone(0, "altro\\bottoriz.bmp");

	//disegna.bottone(0, "nave4\\bottone.bmp");
	//disegna.bottone(0, "altro\\bottnull.bmp");
	disegna.casella(2,2,"nave5\\griglia.bmp", 1);
	disegna.casella(2,5,"nave5\\griglia.bmp", 0);
	getch();
	disegna.casella(2,3,"spari\\sprvuoto.bmp", 0);
	disegna.casella(2,5,"spari\\sprnave.bmp",0);
	disegna.casella(3,5,"spari\\sprnave.bmp",0);
	disegna.casella(4,5,"spari\\sprnave.bmp",0);
	//disegna.casella(1,2,sprNave,0);
	disegna.info("0123456789abcdefghilmnopqrstuvzABCDEFGHILMNOP");
//	disegna.info("999999999999999999999999999999999999999999999");
//	disegna.bottone(2, bottonappo);

	getch();
	closegraph();					//chiude grafica
}
