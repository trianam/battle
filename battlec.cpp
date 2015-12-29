#include"interfac.cpp"
#include"fase.cpp"
#include"sorcio.cpp"
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

#define XGRIGLIA 6
#define YGRIGLIA 6
#define ORIENT 6
#define OK 7
#define EXIT 8
#define BOTTINTRO 0
#define BOTTSPARI 2
#define BOTTMIRA 3
#define BOTTDANNI 4

Interfac disegna;
Sorcio mouse;
Fase umano;
Fase computer;

int cheatNavi = 0;		//codice segreto, se != 0 si vedono navi nemiche
int cheatDColpo = 0;		//codice segreto, se != 0 hai 2 colpi
int cheatCaprio = 0;		//codice segreto, se != 0 uccidi Di Caprio

void inizio();
void umanFase();
void compFase();	//prototipi delle funzioni

void main(){
	char cheat;
	char frase[] = "Benvenuto ammiraglio, preparati alla battaglia. harrrr.";	//frase da scrivere
	char carattere[2];	//carattere da visualizzare
	carattere[1] = '\0';	//per impostare a un solo carattere
	int max = strlen(frase)-1;

	char nave[DIMTOTALSPRITE];
	char appo[3];
	strcpy(nave, "nave");
	itoa(random(NUMNAVI), appo, 10);	//nave della presentazione
	strcat(nave, appo);
	strcat(nave, "\\griglia.bmp");

	disegna.immagine(450,5, "intro\\logo.bmp", 0, WHITE);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	for (int i=0; i<= max; i++){	//scorre stringa
		disegna.immagine(500-(i*8), 300, nave, 0, LIGHTCYAN);		//disegna nave

		disegna.immagine(5,5, "intro\\ste2.bmp", 0, -1);
		delay(5);

		disegna.immagine(5,5, "intro\\ste1.bmp", 0, -1);	//disegna immagini
		delay(5);

		carattere[0] = frase[i];	//legge il carattere
		outtextxy(10+(i*10), 200, carattere);	//e lo visualizza

		disegna.immagine(500-(i*8), 300, "intro\\blackgrd.bmp", 0, LIGHTCYAN);	//cancella nave

		if (kbhit())	//se premuto un tasto
			break;	//salta la presentazione
	}
	disegna.immagine(500-(i*8), 300, nave, 0, LIGHTCYAN);		//disegna nave
	disegna.immagine(500-(i*8), 300, "intro\\fire.bmp", 0, LIGHTCYAN);	//disegna fuoco

	cheat = getch();		//modalit… trucco (se pigiato c, e poi lettera trucco)
	if (cheat == 'c'){
		cheat = getch();
		switch (cheat){
			case 'n':
				cheatNavi = 1;
				break;
			case 'd':
				cheatDColpo = 1;
				break;
			case 'c':
				cheatCaprio = 1;
		}
	}

	cleardevice();
	inizio();
}

void inizio(){
	/**********************INIAZIALIZZAZIONE***************************/
	disegna.base();		//disegna la base grafica
	int i, j;
	for (i=0;i < YGRIGLIA; i++)
		for (j = 0; j< XGRIGLIA; j++)
			disegna.casella(j,i,"altro\\mare.bmp",0);//disegna le caselle con il mare

	char sprite[DIMSPRITE];
	char appo[2];
	for (i=0; i< NUMNAVI; i++){		//disegna bottoni navi
		strcpy(sprite, "nave");
		itoa(i, appo, 10);
		strcat(sprite, appo);
		strcat(sprite, "\\bottone.bmp");
		disegna.bottone(i, sprite);
	}
	disegna.bottone(ORIENT, "altro\\bottoriz.bmp");
	disegna.bottone(OK, "altro\\bottok.bmp");
	disegna.bottone(EXIT, "altro\\bottexit.bmp");	//disegna bottoni orientamento ok e exit


	int attivi[MAXBOTTONI] ={1,1,1,1,1,1,1,0,1};//indica i bottoni attivi (0 disattivo)
	int indice = 0;				//numero di navi inserite
	int x=0, y=0;				//coordinate di appoggio
	int puls=0;
	int logX, logY;				//coordinate logiche griglia
	int numero = -1;			//appoggio per il numero di bottone (-1 nessun bott)
	int nave = -1;				//appoggio numero nave (-1 nessuna nave)
	orientamenti orient = ORIZ;		//orientamento della barca
	int lungh;				//lunghezza
	zone zona;				//indica la zona cliccata
	char car;				//carattere per richiesta di uscita
	mouse.reset();
	mouse.cursEnab();			//attiva mouse

	disegna.info("Seleziona nave, verso e posizionala.");

	do{
		mouse.getStatus(x,y,puls);
		if (puls == 1){			//se premuto il pulsante sinistro del mouse
			disegna.info("");	//cancella barra informazioni
			mouse.getPositionButt(x, y, 0);	//acquisisce posizione del click
			zona = disegna.whotIsThis(x, y);	//acquisisce che zona Š
			if (zona == PULSANTIERA){
				disegna.getBottone(x, y, numero);//numero del bottone
				if (numero != -1){		//se Š un bottone
					if (attivi[numero]){		//se attivo
						if (numero == EXIT){	//se pigiato exit
							mouse.cursDisab();
							disegna.info("Vuoi veramente uscire? (Y/n)");
							do
								car = getch();
							while((car != 'y') && (car != 'n') && (car != '\r'));
							if ((car == 'y') || (car == '\r'))
								exit(0);
							else
								disegna.info("");
								mouse.cursEnab();
						}

						if (numero == ORIENT){	//se Š bottone orientamento
							mouse.cursDisab();	//disabilita il cursore per evitare errore grafico
							if (orient == ORIZ){	//se attualmente Š orizzontale
								orient = VERT;
								disegna.bottone(ORIENT, "altro\\bottvert.bmp");
							}
							else{
								orient = ORIZ;
								disegna.bottone(ORIENT, "altro\\bottoriz.bmp");
							}
							mouse.cursEnab();	//riabilita cursore
						}

						if ((numero >= 0) && (numero < NUMNAVI)){//se Š una nave
							mouse.cursDisab();	//disabilita cursore
							disegna.bottonSwitchOff(nave);	//spegne precedente bottone
							disegna.bottonSwitchOn(numero); //accende nuovo bottone
							mouse.cursEnab();	//riabilita cursore
							nave = numero;
						}
					}
					else
						numero = -1;	//se non attivo azzera il numero
				}
			}
			if (zona == GRIGLIA){
				if (nave != -1){		//se si st… posizionando una nave
					logX = x;
					logY = y;
					disegna.getCasella(logX, logY);
					switch(nave){
						case 0:
						case 1:
						case 2:
							lungh = 1;
							break;
						case 3:
						case 4:
							lungh = 2;
							break;
						case 5:
							lungh = 3;
					}
					strcpy(sprite, "nave");
					itoa(nave, appo, 10);
					strcat(sprite, appo);
					strcat(sprite, "\\griglia.bmp");					//assegna lunghezze a navi
					if (! computer.addNave(logX, logY, orient, lungh, nave, sprite)){	//se aggiunge la nave
						mouse.cursDisab();	//disabilita cursore

						disegna.casella(logX, logY, sprite, orient);

						indice++;		//aggiorna indice
						attivi[nave] = 0;	//disattiva pulsante
						disegna.bottone(nave, "altro\\bottnull.bmp");	//disegna griglia disattivo
						disegna.bottonSwitchOff(nave);	//spegne bottone
						nave = -1;	//azzera indice nave
						if (indice == NUMNAVI){	//se finito di posizionare le navi
							disegna.info("Inserimento completato, premere OK");
							attivi[OK] = 1;		//attiva pulsante di ok
						}
						mouse.cursEnab();	//riabilita cursore
					}
					else
						disegna.info("posizione errata! Prova in un'altra casella");		//avvisa che la nave Š in posizione errata
				}
			}
			delay(500);		//attende un po
		}
	}while(numero != OK);	//finchŠ non Š pigiato ok
	mouse.cursDisab();	//disabilita cursore
	umano.randomNavi();	//inserisce navi del nemico

	umanFase();		//fase umana

	return;
}




void umanFase(){
	caselle casella;
	int x, y, i;		//coordinate e indice appoggi
	int danno;		//danni nave
	orientamenti orient;	//per disegno navi
	int lungh;		//per lunghezza nave, inutile
	char* sprite;	//lo sprite della nave

	disegna.base();		//disegna base
	for (y=0;y < YGRIGLIA; y++)
		for (x = 0; x< XGRIGLIA; x++)
			disegna.casella(x,y,"altro\\mare.bmp",0);//disegna le caselle con il mare


	for (i=0; i< NUMNAVI; i++){    	//disegna navi
		umano.danniNave(i, danno);	//controlla i danni della nave
		if (danno == 100){		//se Š stata distrutta
			umano.posNave(i, x, y, orient, lungh);//acquisisce posizione e dati
			umano.spriteNave(i, sprite);		//sprite
			disegna.casella(x, y, sprite, orient);	//la disegna
		}
	}

	for (y=0;y < YGRIGLIA; y++){
		for (x = 0; x< XGRIGLIA; x++){
			umano.getCasella(x, y, casella);
			if (casella == SPRVUOTO)	//se la casella Š uno sparo a vuoto
				disegna.casella(x,y,"spari\\sprvuoto.bmp", 0);
			if (casella == SPRNAVE)		//se Š uno sparo a nave
				disegna.casella(x,y,"spari\\sprnave.bmp", 0);
		}
	}

	/********trucco navi***************/
	if (cheatNavi){
		caselle appocchio;
		int i,j;
		for (i=0;i < YGRIGLIA; i++){
			for (j = 0; j< XGRIGLIA; j++){
				umano.getCasella(j, i, appocchio);
				switch(appocchio){
					case VUOTO:
						disegna.casella(j,i,"prova\\vuoto.bmp",0);
						break;
					case NAVE:
						disegna.casella(j,i,"prova\\nave.bmp",0);
						break;
					case SPRVUOTO:
						disegna.casella(j,i,"prova\\sprvuoto.bmp",0);
						break;
					case SPRNAVE:
						disegna.casella(j,i,"prova\\sprnave.bmp",0);
						break;
					default:
						disegna.casella(j,i,"prova\\errore.bmp",0);
						break;
				}
			}
		}
	}
	/*******************************/

	disegna.bottone(BOTTINTRO, "altro\\bottuint.bmp");
	disegna.bottone(BOTTSPARI, "altro\\bottspar.bmp");
	disegna.bottone(BOTTMIRA, "altro\\bottmira.bmp");
	disegna.bottone(BOTTDANNI, "altro\\bottdann.bmp");
	disegna.bottone(OK, "altro\\bottok.bmp");
	disegna.bottone(EXIT, "altro\\bottexit.bmp");

	int puls=0;
	zone zona;		//zona cliccata
	int attivi[MAXBOTTONI] ={0,0,1,1,1,0,0,0,1};//indica i bottoni attivi (0 disattivo)
	int numero = -1;	//indica il numero di tasto premuto
	int danni = 0, appo;	//variabili per il conteggio dei danni totali
	int danniQ, danniR;	//resto e quoziente
	char stringa[DIMINFO];
	char stringappo[4];	//stringhe per la visualizzazione dei messaggi
	caselle stato = VUOTO;	//per controllo stato casella
	int sparato =0;		//flag, indica se si Š sparato o no
	int logX, logY;		//coordinate logiche
	int vittoria = 0;	//Š falso finche non vince
	int preVittoria = 0;
	char car;		//carattere per richiesta di uscita

	int contaA=0, contaC=0;	//per il conteggio degli spari Acqua, Colpito
	for (y=0;y < YGRIGLIA; y++){
		for (x = 0; x< XGRIGLIA; x++){	//scorre griglia
			if (! umano.getCasella(x, y, stato)){
				if (stato == SPRNAVE)	//se Š una casella colpita
					contaC++;
				if (stato == SPRVUOTO)	//se Š acqua
					contaA++;
			}
		}
	}

	disegna.info("Clicca sulla griglia per sparare.");

	mouse.cursEnab();
	do{
		mouse.getStatus(x,y,puls);
		if (puls == 1){			//se premuto il pulsante sinistro del mouse
			disegna.info("");	//cancella barra informazioni
			mouse.getPositionButt(x, y, 0);	//acquisisce posizione del click
			zona = disegna.whotIsThis(x, y);	//acquisisce che zona Š

			if (zona == PULSANTIERA){
				disegna.getBottone(x, y, numero);//numero del bottone
				if (numero != -1){		//se Š un bottone
					if (attivi[numero]){		//se attivo
						if (numero == OK){	//se pigiato ok
							if (preVittoria)
								vittoria = 1;
							else{
								mouse.cursDisab();
								compFase();	//passa alla fase computer
							}
						}

						if (numero == EXIT){	//se pigiato exit
							mouse.cursDisab();
							disegna.info("Vuoi veramente uscire? (Y/n)");
							do
								car = getch();
							while((car != 'y') && (car != 'n') && (car != '\r'));
							if ((car == 'y') || (car == '\r'))
								exit(0);
							else
								disegna.info("");
								mouse.cursEnab();
						}

						if (numero == BOTTDANNI){	//se pigiato bottone danni
							danni = 0;
							for (int i=0; i < NUMNAVI; i++){	//scorre le navi
								umano.danniNave(i, appo);
								danni += appo;		//somma i danni
							}
							danniQ = danni / NUMNAVI;		//media dei danni
							danniR = danni % NUMNAVI;
							strcpy(stringa, "Danni inflitti al nemico: ");
							itoa(danniQ, stringappo, 10);
							strcat(stringa, stringappo);
							itoa(danniR, stringappo,10);
							strcat(stringa, ",");
							strcat(stringa, stringappo);
							strcat(stringa, "%");			//crea stringa
							disegna.info(stringa);			//visualizza messaggio
						}

						if (numero == BOTTSPARI){			//se premuto pulsante statistiche colpi
							strcpy(stringa, "Colpi a segno: ");
							itoa(contaC, stringappo, 10);
							strcat(stringa, stringappo);
							strcat(stringa, "   Colpi a vuoto: ");
							itoa(contaA, stringappo, 10);
							strcat(stringa, stringappo);		//crea stringa
							disegna.info(stringa);			//visualizza messaggio
						}

						if (numero == BOTTMIRA){			//se premuto bottone mira
							if((contaC+contaA) != 0){		//se Š stato sparato qualche colpo
								appo = (float(contaC) / float(contaA+contaC)) *100.0;//percentuale spari andati a segno
								strcpy(stringa, "Colpi andati a segno: ");
								itoa(appo, stringappo, 10);
								strcat(stringa, stringappo);
								strcat(stringa, "% sul totale");
								disegna.info(stringa);
							}
							else					//se non sparato nessun colpo
								disegna.info("Nessun colpo sparato!");
						}
					}
					else
						numero = -1;	//se non attivo azzera il numero
				}
			}
			if (zona == GRIGLIA){
				if ((! sparato) || ((cheatDColpo) && (sparato < 2))){		//se non ha ancora effettuato il colpo (o il doppio colpo con il trucco)
					logX = x;
					logY = y;
					disegna.getCasella(logX, logY);		//posizioni logiche della griglia

					mouse.cursDisab();	//disabilita il cursore
					switch (umano.spara(logX, logY)){	//spara e controlla l'esito
						case ACQUA:
							disegna.casella(logX, logY, "spari\\sprvuoto.bmp", 0);	//disegna lo sparo
							disegna.info("Spiacente, ACQUA. Premere OK.");	//messaggio
							contaA++;	//aumenta contatore per bottone colpi
							sparato++;	//attiva flag
							attivi[OK] = 1;	//attiva bottone OK
							break;
						case COLPITO:
							disegna.casella(logX, logY, "spari\\sprnave.bmp", 0);	//disegna lo sparo
							disegna.info("Complimenti, COLPITO. Premere OK.");	//messaggio
							contaC++;
							sparato++;	//attiva flag
							attivi[OK] = 1;	//attiva bottone OK
							break;
						case AFFONDATO:
							disegna.casella(logX, logY, "spari\\sprnave.bmp", 0);	//disegna lo sparo
							disegna.info("Complimenti, COLPITO e AFFONDATO. Premere OK.");	//messaggio
							contaC++;
							sparato++;	//attiva flag
							attivi[OK] = 1;	//attiva bottone OK
							break;
						case ERRORE:
							disegna.info("Qu… hai gi… sparato idiota!");
							break;
					}
					mouse.cursEnab();	//riabilita il cursore

					danni = 0;
					for (int i=0; i < NUMNAVI; i++){	//scorre le navi
						umano.danniNave(i, appo);
						danni += appo;		//somma i danni
					}
					danni /= NUMNAVI;	//media danni
					if (danni == 100){	//se annientato tutte le barche
						preVittoria = 1;	//vittoria
						disegna.info("Complimenti hai vinto, premere OK");
					}
				}
				else		//se hai gi… sparato
					disegna.info("hai gi… sparato, premi OK");
			}
			delay(500);		//attende un po
		}
	}while(! vittoria);	//finchŠ non vince

	mouse.cursDisab();	//disabilita cursore

	cleardevice();	//pulisce schermo
	char frase[] = "Complimenti ammiraglio, abbiamo vinto.";	//frase da scrivere
	char carattere[2];	//carattere da visualizzare
	carattere[1] = '\0';	//per impostare a un solo carattere
	int max = strlen(frase)-1;

	disegna.immagine(450,5, "intro\\logo.bmp", 0, WHITE);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor(WHITE);
	for (i=0; i<= max; i++){	//scorre stringa
		disegna.immagine(5,5, "intro\\ste2.bmp", 0, -1);
		delay(5);

		disegna.immagine(5,5, "intro\\ste1.bmp", 0, -1);	//disegna immagini
		delay(5);

		carattere[0] = frase[i];	//legge il carattere
		outtextxy(10+(i*10), 200, carattere);	//e lo visualizza

		if (kbhit())	//se premuto un tasto
			break;	//salta la presentazione
	}

	getch();

	/*************cheat caprio*******************/
	if (cheatCaprio){
		cleardevice();	//pulisce schermo
		char frase2[] = "Sono il re del mondo!";	//frase da scrivere
		carattere[1] = '\0';	//per impostare a un solo carattere
		max = strlen(frase2)-1;

		for (i=0; i<= max; i++){	//scorre stringa
			disegna.immagine(5,5, "intro\\caprio2.bmp", 0, -1);
			delay(5);

			disegna.immagine(5,5, "intro\\caprio1.bmp", 0, -1);	//disegna immagini
			delay(5);

			carattere[0] = frase2[i];	//legge il carattere
			outtextxy(10+(i*10), 200, carattere);	//e lo visualizza

			if (kbhit())	//se premuto un tasto
				break;	//salta la presentazione
		}
		disegna.immagine(5,5, "intro\\caprio3.bmp", 0, -1);
		getch();
	}
	/**************fine************************/

	exit (0);

}

void compFase(){
	disegna.base();		//disegna base
	disegna.bottone(BOTTINTRO, "altro\\bottcint.bmp");

	orientamenti orient;	//per disegno navi
	int lungh;		//per lunghezza nave, inutile
	caselle casella;
	int x, y, i;		//coordinate e indice appoggi
	char* sprite;	//lo sprite della nave

	disegna.info("Attendere prego");

	for (y=0;y < YGRIGLIA; y++)
		for (x = 0; x< XGRIGLIA; x++)
			disegna.casella(x,y,"altro\\mare.bmp",0);//disegna le caselle con il mare

	for (i=0; i< NUMNAVI; i++){    	//disegna navi
		computer.posNave(i, x, y, orient, lungh);//acquisisce posizione e dati
		computer.spriteNave(i, sprite);		//sprite
		disegna.casella(x, y, sprite, orient);	//la disegna
	}

	for (y=0;y < YGRIGLIA; y++){			//disegna colpi
		for (x = 0; x< XGRIGLIA; x++){
			computer.getCasella(x, y, casella);
			if (casella == SPRVUOTO)	//se la casella Š uno sparo a vuoto
				disegna.casella(x,y,"spari\\sprvuoto.bmp", 0);
			if (casella == SPRNAVE)		//se Š uno sparo a nave
				disegna.casella(x,y,"spari\\sprnave.bmp", 0);
		}
	}

	delay(1000);	//attende
	computer.aiSpara(); 	//spara il computer

	for (y=0;y < YGRIGLIA; y++){			//disegna colpi aggiornato
		for (x = 0; x< XGRIGLIA; x++){
			computer.getCasella(x, y, casella);
			if (casella == SPRVUOTO)	//se la casella Š uno sparo a vuoto
				disegna.casella(x,y,"spari\\sprvuoto.bmp", 0);
			if (casella == SPRNAVE)		//se Š uno sparo a nave
				disegna.casella(x,y,"spari\\sprnave.bmp", 0);
		}
	}

	disegna.bottone(BOTTSPARI, "altro\\bottspar.bmp");
	disegna.bottone(BOTTMIRA, "altro\\bottmira.bmp");
	disegna.bottone(BOTTDANNI, "altro\\bottdann.bmp");
	disegna.bottone(OK, "altro\\bottok.bmp");
	disegna.bottone(EXIT, "altro\\bottexit.bmp");

	int puls=0;
	zone zona;		//zona cliccata
	int attivi[MAXBOTTONI] ={0,0,1,1,1,0,0,1,1};//indica i bottoni attivi (0 disattivo)
	int numero = -1;	//indica il numero di tasto premuto
	int danni = 0, appo;	//variabili per il conteggio dei danni totali
	int danniQ, danniR;	//resto e quoziente
	char stringa[DIMINFO];
	char stringappo[4];	//stringhe per la visualizzazione dei messaggi
	caselle stato = VUOTO;	//per controllo stato casella
	int preVittoria = 0;
	int vittoria = 0;	//Š falso finche non vince
	int logX, logY, numNave;//coordinate logiche e numero nave
	char car;		//carattere per richiesta di uscita

	int contaA=0, contaC=0;	//per il conteggio degli spari Acqua, Colpito
	for (y=0;y < YGRIGLIA; y++){
		for (x = 0; x< XGRIGLIA; x++){	//scorre griglia
			if (! computer.getCasella(x, y, stato)){
				if (stato == SPRNAVE)	//se Š una casella colpita
					contaC++;
				if (stato == SPRVUOTO)	//se Š acqua
					contaA++;
			}
		}
	}

	disegna.info("Premi OK per proseguire.");

	danni = 0;
	for (i=0; i < NUMNAVI; i++){	//scorre le navi
		computer.danniNave(i, appo);
		danni += appo;		//somma i danni
	}
	danni /= NUMNAVI;	//media danni
	if (danni == 100){	//se annientato tutte le barche
		preVittoria = 1;	//vittoria
		disegna.info("asta la vista, baby");
	}

	mouse.cursEnab();
	do{
		mouse.getStatus(x,y,puls);
		if (puls == 1){			//se premuto il pulsante sinistro del mouse
			disegna.info("");	//cancella barra informazioni
			mouse.getPositionButt(x, y, 0);	//acquisisce posizione del click
			zona = disegna.whotIsThis(x, y);	//acquisisce che zona Š

			if (zona == PULSANTIERA){
				disegna.getBottone(x, y, numero);//numero del bottone
				if (numero != -1){		//se Š un bottone
					if (attivi[numero]){		//se attivo
						if (numero == OK){	//se pigiato ok
							if (preVittoria)	//se ha vinto
								vittoria = 1;	//esce dal ciclo
							else{
								mouse.cursDisab();
								umanFase();	//passa alla fase umana
							}
						}

						if (numero == EXIT){	//se pigiato exit
							mouse.cursDisab();
							disegna.info("Vuoi veramente uscire? (Y/n)");
							do
								car = getch();
							while((car != 'y') && (car != 'n') && (car != '\r'));
							if ((car == 'y') || (car == '\r'))
								exit(0);
							else
								disegna.info("");
								mouse.cursEnab();
						}

						if (numero == BOTTDANNI){	//se pigiato bottone danni
							danni = 0;
							for (int i=0; i < NUMNAVI; i++){	//scorre le navi
								computer.danniNave(i, appo);
								danni += appo;		//somma i danni
							}
							danniQ = danni / NUMNAVI;		//media dei danni
							danniR = danni % NUMNAVI;
							strcpy(stringa, "Danni inflitti al nemico: ");
							itoa(danniQ, stringappo, 10);
							strcat(stringa, stringappo);
							itoa(danniR, stringappo,10);
							strcat(stringa, ",");
							strcat(stringa, stringappo);
							strcat(stringa, "%");			//crea stringa
							disegna.info(stringa);			//visualizza messaggio
						}

						if (numero == BOTTSPARI){			//se premuto pulsante statistiche colpi
							strcpy(stringa, "Colpi a segno: ");
							itoa(contaC, stringappo, 10);
							strcat(stringa, stringappo);
							strcat(stringa, "   Colpi a vuoto: ");
							itoa(contaA, stringappo, 10);
							strcat(stringa, stringappo);		//crea stringa
							disegna.info(stringa);			//visualizza messaggio
						}
						if (numero == BOTTMIRA){			//se premuto bottone mira
							if((contaC+contaA) != 0){		//se Š stato sparato qualche colpo
								appo = (float(contaC) / float(contaA+contaC)) *100.0;//percentuale spari andati a segno
								strcpy(stringa, "Colpi andati a segno: ");
								itoa(appo, stringappo, 10);
								strcat(stringa, stringappo);
								strcat(stringa, "% sul totale");
								disegna.info(stringa);
							}
							else					//se non sparato nessun colpo
								disegna.info("Nessun colpo sparato!");
						}
					}
					else
						numero = -1;	//se non attivo azzera il numero
				}
			}
			if (zona == GRIGLIA){			//se cliccato su griglia
				logX = x;
				logY = y;
				disegna.getCasella(logX, logY);
				if(!computer.numNave(logX, logY, numNave)){	//se c'e una nave
					computer.danniNave(numNave, appo);
					strcpy(stringa, "Danni nave: ");
					itoa(appo, stringappo, 10);
					strcat(stringa, stringappo);
					strcat(stringa, "%");
					disegna.info(stringa);
				}
			}


			delay(500);		//attende un po
		}
	}while(! vittoria);	//finchŠ non vince

	mouse.cursDisab(); 	//disattiva  mouse
        cleardevice();	//pulisce schermo
	char frase[] = "Ammiraglio, lei mi ha deluso profondamente, addio!";	//frase da scrivere
	char carattere[2];	//carattere da visualizzare
	carattere[1] = '\0';	//per impostare a un solo carattere
	int max = strlen(frase)-1;

	disegna.immagine(450,5, "intro\\logo.bmp", 0, WHITE);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor(RED);
	for (i=0; i<= max; i++){	//scorre stringa
		disegna.immagine(5,5, "intro\\ste2.bmp", 0, -1);
		delay(5);

		disegna.immagine(5,5, "intro\\ste1.bmp", 0, -1);	//disegna immagini
		delay(5);

		carattere[0] = frase[i];	//legge il carattere
		outtextxy(10+(i*10), 200, carattere);	//e lo visualizza

		if (kbhit())	//se premuto un tasto
			break;	//salta la presentazione
	}
	getch();

	exit(0);
}





