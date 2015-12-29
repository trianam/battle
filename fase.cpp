#include<stdio.h>
#include<STDLIB.H>
#include"Fase.h"

Fase :: Fase(){
	for(int i = 0; i < XGRIGLIA; i++){
		for(int j = 0; j < YGRIGLIA; j++){
			enemyGrid[i][j] = VUOTO;
		}
	}
	randomize();
	aiMem.fase = RANDOM;
}

Fase :: ~Fase(){
}

int Fase :: cercaNave(int x, int y){
	for (int i =0; i < NUMNAVI; i++){			//scorre le navi
		if (enemyNavi[i].getOrientamento() == ORIZ){	//se Š orizzontale
			if (enemyNavi[i].getYPos() == y)	//se la y corrisponde
				if ( (x >= enemyNavi[i].getXPos()) && (x < (enemyNavi[i].getXPos()+ enemyNavi[i].getLunghezza())) ) //se la x Š compresa nella zona della nave
					return i;		//ritorna il numero della nave
		}
		else{                                    	//se Š verticale
			if (enemyNavi[i].getXPos() == x)        //se la x corrisponde
				if ( (y >= enemyNavi[i].getYPos()) && (y < (enemyNavi[i].getYPos()+ enemyNavi[i].getLunghezza())) ) //se la y Š compresa nella zona della nave
					return i;		//ritorna il numero della nave
		}
	}
	return -1;	//se non vi Š una nave
}

void Fase :: avanzaV(aiv& verso){
	if (verso == UP)
		verso = DX;
	else if (verso == DX)
		verso = DN;
	else if (verso == DN)
		verso = SX;
	else if (verso == SX)
		verso = UP;

	return;
}

void Fase :: calcolaSomm(aiv verso, int& sommX, int& sommY){
	switch(verso){
		case UP:
			sommX = 0;
			sommY = -1;
			break;
		case DX:
			sommX = 1;
			sommY = 0;
			break;
		case DN:
			sommX = 0;
			sommY = 1;
			break;
		case SX:
			sommX = -1;
			sommY = 0;
			break;
	}
	return;
}

void Fase :: aggiornaImposs(){
	int y, x;
	for (y=0;y < YGRIGLIA; y++){			//esamina caselle
		for (x = 0; x< XGRIGLIA; x++){
			if (enemyGrid[x][y] == SPRNAVE){	//se Š uno sparo a nave
				if (x != 0)	//se non Š a bordo sx
					if (enemyGrid[x-1][y] == VUOTO)	//se la casella a SX Š vuota
						enemyGrid[x-1][y] = IMPOSS;	//questa Š un colpo impossibile
				if (x != XGRIGLIA-1)	//se non Š a bordo DX
					if (enemyGrid[x+1][y] == VUOTO)	//se la casella a DX Š vuota
						enemyGrid[x+1][y] = IMPOSS;	//questa Š un colpo impossibile

				if (y != 0)	//se non Š a bordo UP
					if (enemyGrid[x][y-1] == VUOTO)	//se la casella sopra Š vuota
						enemyGrid[x][y-1] = IMPOSS;	//questa Š un colpo impossibile
				if (y != YGRIGLIA-1)	//se non Š a bordo DN
					if (enemyGrid[x][y+1] == VUOTO)	//se la casella sotto Š vuota
						enemyGrid[x][y+1] = IMPOSS;	//questa Š un colpo impossibile
			}
		}
	}
	return;
}

int Fase :: addNave(int x, int y, orientamenti orient, int lungh, int pos, char* sprite){

	//CONTROLLA SE NON ESCE DAI BORDI
	if ((x < 0) || (y < 0))
		return 2;
	if (orient == VERT){
		if (((y + lungh-1) >= YGRIGLIA) || (x >= XGRIGLIA))
			return 2;
	}
	if (orient == ORIZ){
		if (((x + lungh-1) >= XGRIGLIA) || (y >= YGRIGLIA))
			return 2;
	}


	//CONTROLLA SE NON INCROCIA O Š ACCANTO AD ALTRE NAVI
	if(orient == VERT){	//se Š verticale
		if (y != 0)			//se non Š a bordo sup
			if (enemyGrid[x][y-1] == NAVE)	//se ha una nave accanto sopra
				return 3;
		for(int i= y; i < y+lungh; i++){		//scorre le y
			if (enemyGrid[x][i] == NAVE)	//se Š occupata
				return 3;
			if (x != XGRIGLIA-1)		//se non Š al bordo DX
				if (enemyGrid[x+1][i] == NAVE)	//se ha una nave accanto a DX
					return 3;
			if (x != 0)                   	//se non Š a bordo SX
				if (enemyGrid[x-1][i] == NAVE)	//se ha una nave accando a SX
					return 3;
		}
		i--;
		if (i != YGRIGLIA-1)		//se non Š a bordo inf
			if (enemyGrid[x][i+1] == NAVE)	//se ha una nave accanto sotto
				return 3;
	}
	else{			//se Š orizzontale
		if (x != 0)			//se non Š a bordo SX
			if (enemyGrid[x-1][y] == NAVE)	//se ha una nave accanto a SX
				return 3;
		for(int i= x; i < x+lungh; i++){		//scorre le x
			if (enemyGrid[i][y] == NAVE)	//se Š occupata
				return 3;
			if (y != YGRIGLIA-1)
				if (enemyGrid[i][y+1] == NAVE)
					return 3;
			if (y != 0)		//se non Š al bordo sup
				if (enemyGrid[i][y-1] == NAVE)	//se ha una nave accanto sopra
					return 3;
		}
		i--;
		if (i != XGRIGLIA-1)		//se non Š a bordo DX
			if (enemyGrid[i+1][y] == NAVE)	//se ha una nave accanto a DX
				return 3;
	}


	//AGGIORNA STATO
	enemyNavi[pos].inizializza(x, y, orient, lungh, sprite);//aggiorna il vettore di navi
	if(orient == VERT){	//se Š verticale
		for(int i= y; i < y+lungh; i++)		//scorre le y
			enemyGrid[x][i] = NAVE;	//aggiorna griglia
	}
	else{			//se Š orizzontale
		for(int i= x; i < x+lungh; i++)		//scorre le x
			enemyGrid[i][y] = NAVE;	//aggiorna griglia
	}


	return 0;
}

int Fase :: randomNavi(){
	char sprite[DIMSPRITE];
	char value[3];

	orientamenti appo;
	//nave da 3 caselle
	do
		(random(2) == 0)? (appo = ORIZ) : (appo = VERT);			//sceglie a caso l'orientamento
	while(addNave (random(XGRIGLIA), random(YGRIGLIA), appo, 3, 5, "nave5\\griglia.bmp"));	//ritenta finche non Š libero (con le coordinate casuali e la lunghezza a 3)

	//navi da 2 caselle
	for (int i=0; i<2; i++){	//per 2 volte
		strcpy(sprite, "nave");
		itoa(i+3, value, 10);
		strcat(sprite, value);
		strcat(sprite, "\\griglia.bmp");
		do
			(random(2) == 0)? (appo = ORIZ) : (appo = VERT);			//sceglie a caso l'orientamento
		while(addNave (random(XGRIGLIA), random(YGRIGLIA), appo, 2, i+3, sprite));	//ritenta finche non Š libero (con le coordinate casuali e la lunghezza a 2)
	}

	//navi da 1 casella
	for (i=0; i<3; i++){	//per 3 volte
		strcpy(sprite, "nave");
		itoa(i, value, 10);
		strcat(sprite, value);
		strcat(sprite, "\\griglia.bmp");
		do{
			(random(2) == 0)? (appo = ORIZ) : (appo = VERT);			//sceglie a caso l'orientamento
		}while(addNave (random(XGRIGLIA), random(YGRIGLIA), appo, 1, i, sprite));	//ritenta finche non Š libero (con le coordinate casuali e la lunghezza a 1)
	}

	return 0;
}


colpi Fase :: spara(int x, int y){
	if ((x >= 0) && (x < XGRIGLIA) && (y >= 0) && (y < YGRIGLIA)){	//se Š entro i limiti della griglia
		switch(enemyGrid[x][y]){
			case SPRVUOTO:
				break;
			case SPRNAVE:
				break;
			case VUOTO:
				enemyGrid[x][y] = SPRVUOTO;
				return ACQUA;
			case NAVE:
				int indice = cercaNave(x, y);
				if (indice != -1){	//se non da errore cercaNave
					enemyGrid[x][y] = SPRNAVE;
					enemyNavi[indice].colpisci();
					if (enemyNavi[indice].getDanni() == 100)
						return AFFONDATO;
					else
						return COLPITO;
				}
		}
	}
	return ERRORE;
}

void Fase :: aiSpara(){
	colpi appo;
	int sommX =0;
	int sommY =0;
	int appoRandom;
	aiv appoVerso;

	switch (aiMem.fase){
		case RANDOM:	//se Š fase random
			do{
				aiMem.x = random(XGRIGLIA);	//sceglie coordinate a caso
				aiMem.y = random(YGRIGLIA);
				if (enemyGrid[aiMem.x][aiMem.y] == IMPOSS)	//se Š un colpo impossibile
					appo = ERRORE;				//non lo fa
				else
					appo = spara(aiMem.x, aiMem.y);
			}while (appo == ERRORE);		//finchŠ non va bene

			if (appo == AFFONDATO)		//se affondato(nave da 1)
				aggiornaImposs();	//aggiorna colpi impossibili

			if (appo == COLPITO){		//se Š stata colpita una nave
				aiMem.fase = SEARCH;	//passa in modalit… ricerca
				appoRandom = random(4);	//sceglie casualmente un verso di partenza
				switch(appoRandom){
					case 0:
						appoVerso = UP;
						break;
					case 1:
						appoVerso = DX;
						break;
					case 2:
						appoVerso = DN;
						break;
					case 3:
						appoVerso = SX;
						break;
				}
				aiMem.verso = appoVerso;	//orienta il verso casualmente
			}
			break;

		case SEARCH:
			do{
				calcolaSomm(aiMem.verso, sommX, sommY);	//calcola sommatori
				if (enemyGrid[aiMem.x+sommX][aiMem.y+sommY] == IMPOSS)	//se Š un colpo impossibile
					appo = ERRORE;				//non lo fa
				else
					appo = spara(aiMem.x + sommX, aiMem.y + sommY);	//spara
				if (appo == ERRORE){			//se Š errore
					avanzaV(aiMem.verso);		//cambia verso
				}
			}while(appo == ERRORE);			//finchŠ non va bene
			switch(appo){
				case ACQUA:			//se Š acqua
					avanzaV(aiMem.verso);  //cambia verso
					break;

				case AFFONDATO:			//se Š affondato (nave da 2)
					aiMem.fase = RANDOM;	//torna a modalit… casuale
					aggiornaImposs();	//aggiorna griglia colpi impossibili
					break;

				case COLPITO: 			//se colpito (nave da 3)
					aiMem.fase = PROSEG;	//vai a modalit… proseguimento
					aiMem.proseg= 2;	//imposta proseguitore
					break;
			}
			break;

		case PROSEG:
			do{
				calcolaSomm(aiMem.verso, sommX, sommY);	//calcola sommatori
				if (enemyGrid[aiMem.x + (sommX * aiMem.proseg)][aiMem.y + (sommY * aiMem.proseg)] == IMPOSS)	//se Š un colpo impossibile
					appo = ERRORE;				//non lo fa
				else
					appo = spara(aiMem.x + (sommX * aiMem.proseg), aiMem.y + (sommY * aiMem.proseg) );//spara
				if (appo == ERRORE){
					avanzaV(aiMem.verso);
					avanzaV(aiMem.verso);		//cambia verso (opposto)
					aiMem.proseg = 1;	//reimposta proseguitore
				}
			}while(appo == ERRORE);

			switch(appo){
				case ACQUA:			//se acqua
					avanzaV(aiMem.verso);
					avanzaV(aiMem.verso);		//cambia verso (opposto)
					aiMem.proseg = 1;	//reimposta proseguitore
					break;
				case AFFONDATO:			//se affondato
					aiMem.fase = RANDOM;	//torna alla modalit… casuale
					aggiornaImposs();	//aggiorna griglia colpi impossibili
					break;
				case COLPITO:		//se la nave fosse pi— lunga di 3
					aiMem.proseg++;
					break;
			}
			break;
	}
	return;
}

int Fase :: posNave(int numero, int& x, int&y, orientamenti& orient, int& lungh){
	if ((numero < 0) || (numero >= NUMNAVI))
		return 1;

	x = enemyNavi[numero].getXPos();
	y = enemyNavi[numero].getYPos();
	orient = enemyNavi[numero].getOrientamento();
	lungh = enemyNavi[numero].getLunghezza();
	return 0;
}

int Fase :: numNave(int x, int y, int& numero){
	int appo;
	appo = cercaNave(x, y);
	if (appo == -1)
		return 1;
	else{
		numero = appo;
		return 0;
	}
}

int Fase :: danniNave(int numero, int& danni){
	if ((numero < 0) || (numero >= NUMNAVI))
		return 1;

	danni = enemyNavi[numero].getDanni();
	return 0;
}

int Fase :: spriteNave(int numero, char*& sprite){
	if ((numero < 0) || (numero >= NUMNAVI))
		return 1;

	sprite = enemyNavi[numero].getSprite();
	return 0;
}

int Fase :: getCasella(int x, int y, caselle& wazzo){
	if ((x < 0)||(x >= XGRIGLIA)||(y < 0)||(y >= YGRIGLIA))	//se Š fuori dai limiti
		return 1;
	wazzo = enemyGrid[x][y];	//ritorna il valore della casella
	return 0;
}








