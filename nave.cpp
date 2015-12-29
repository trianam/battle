#include"nave.h"

Nave :: Nave(){
	xPos = 0;
	yPos = 0;
	orientamento = ORIZ;
	lunghezza = 0;
	strcpy (sprite, "");
	colpite = 0;
	danni = 0;
}

Nave :: ~Nave(){}

void Nave :: inizializza(int x, int y, orientamenti orient, int lungh, char* nuSprite){
	xPos = x;
	yPos = y;
	orientamento = orient;
	lunghezza = lungh;
	strcpy(sprite, nuSprite);
	colpite = 0;
	danni = 0;
}

int Nave :: getDanni(){
	return danni;
}

int Nave :: getXPos(){
	return xPos;
}

int Nave :: getYPos(){
	return yPos;
}

orientamenti Nave :: getOrientamento(){
	return orientamento;
}

int Nave :: getLunghezza(){
	return lunghezza;
}

char* Nave :: getSprite(){
	return sprite;
}

void Nave :: colpisci(){
	float dannappo = 0;
	if (danni != 100){
		colpite++;
		dannappo = float(colpite) / float(lunghezza);
		dannappo *= 100.0;
		danni = dannappo;
	}
	return;
}