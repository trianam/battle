#include"interfac.h"

#define MAXBOTTONI 9
#define TRANSPARENT LIGHTCYAN

char* directory = "c:\\lavori\\battle\\sprites\\";

Interfac :: Interfac(){
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

}

Interfac :: ~Interfac(){
	closegraph();
}


void Interfac :: fskip(FILE *fp, int num_bytes){
	int i;
	for (i=0; i<num_bytes; i++)
		fgetc(fp);
}

void Interfac :: load_bmp(char *file,BITMAP &b){
  FILE *fp;
  long index;
  word num_colors;
  int x;

  /* open the file */
  if ((fp = fopen(file,"rb")) == NULL)
  {

    printf("Error opening file %s.\n",file);
    getch();
    exit(1);
  }

  /* check to see if it is a valid bitmap file */
  if (fgetc(fp)!='B' || fgetc(fp)!='M')
  {
    fclose(fp);
    printf("%s is not a bitmap file.\n",file);
    exit(1);
  }

  /* read in the width and height of the image, and the
     number of colors used; ignore the rest */
  fskip(fp,16);
  fread(&b.width, sizeof(word), 1, fp);
  fskip(fp,2);
  fread(&b.height,sizeof(word), 1, fp);
  fskip(fp,22);
  fread(&num_colors,sizeof(word), 1, fp);
  fskip(fp,6);

  /* assume we are working with an 8-bit file */
  if (num_colors==0) num_colors=256;


  /* try to allocate memory */
  if ((b.data = (byte *) malloc((word)(b.width*b.height))) == NULL)
  {
    fclose(fp);
    printf("Error allocating memory for file %s.\n",file);
    exit(1);
  }

  /* Ignore the palette information for now.
     See palette.c for code to read the palette info. */
  fskip(fp,num_colors*4);

  /* read the bitmap */
  for(index=(b.height-1)*b.width;index>=0;index-=b.width)
    for(x=0;x<b.width;x++)
      b.data[(word)index+x]=(byte)fgetc(fp);

  fclose(fp);
}

void Interfac :: draw_bmp_oriz(BITMAP bmp, int x, int y, int color){
	int j,i, offset = 0;
	for(j = 0; j < bmp.height; j++){		//scorre altezza
		for(i = 0; i< bmp.width; i++, offset++){	//scorre larghezza
			if (bmp.data[offset] != color)	//se il colore Š diverso dall'escluso
				putpixel(x+i, y+j, bmp.data[offset]);//scrive pixel
		}
	}
	return;
}

void Interfac :: draw_bmp_vert(BITMAP bmp, int x, int y, int color){
	int j,i, offset = 0;
	for(j = 0; j < bmp.height; j++){		//scorre altezza
		for(i = 0; i< bmp.width; i++, offset++){	//scorre larghezza
			if (bmp.data[offset] != color)	//se il colore Š diverso dall'escluso
				putpixel(x+j, y+i, bmp.data[offset]);//scrive pixel
		}
	}
	return;
}

void Interfac :: buttonPos(int pos, int& west, int& nord, int& est, int& sud){
	west = getmaxx()-143;
	nord = (20+(50*pos));
	est  = getmaxx()-24;
	sud  = (20+(50*(pos+1)));
	return;
}

void Interfac :: base(){
	float maxX=getmaxx(), maxY=getmaxy();

	//sfondo
	setfillstyle(SOLID_FILL,LIGHTGRAY);	//motivo e colore sfondo
	rectangle(0, 0, maxX, maxY);		//bordo sfondo
	floodfill(1,1,LIGHTGRAY);		//disegna sfondo

	//griglia
	setcolor(BLACK);			//setta colore linea barra
	setfillstyle(SOLID_FILL,LIGHTCYAN);		//setta motivo e colore sfondo barra
	bar3d(50,50,350,350,10,1);		//disegna barra
	setfillstyle(SOLID_FILL,DARKGRAY);	//setta motivo e colore bordi
	floodfill(351,50,BLACK);
	floodfill(350,49,BLACK);		//disegna bordi

	line(50,100,350,100);
	line(50,150,350,150);
	line(50,200,350,200);
	line(50,250,350,250);
	line(50,300,350,300);

	line(100,50,100,350);
	line(150,50,150,350);
	line(200,50,200,350);
	line(250,50,250,350);
	line(300,50,300,350);		//disegna righe divisioni caselle

	setcolor(DARKGRAY);			//colore testo
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(75,30,"A");
	outtextxy(125,30,"B");
	outtextxy(175,30,"C");
	outtextxy(225,30,"D");
	outtextxy(275,30,"E");
	outtextxy(325,30,"F");

	outtextxy(40,75,"1");
	outtextxy(40,125,"2");
	outtextxy(40,175,"3");
	outtextxy(40,225,"4");
	outtextxy(40,275,"5");
	outtextxy(40,325,"6");			//disegna lettere e numeri griglia


	//message box
	setcolor(RED);				//setta colore testo
	settextstyle(GOTHIC_FONT, HORIZ_DIR, 0);//setta stile testo
	outtextxy(10,maxY-90,"Message Box");	//disegna testo

	setfillstyle(SOLID_FILL, YELLOW);	//setta colore e motivo sfondo
	setcolor(BLACK);			//setta colore line barra
	setlinestyle(SOLID_LINE,1,2);		//setta stile linea barra
	bar3d(2, maxY-40, maxX-160, maxY-2, 10, 1);//disegna barra
	setfillstyle(SOLID_FILL,GREEN);		//setta colore e motivo riempimento bordi
	floodfill(50, maxY-43, BLACK);
	floodfill(maxX-155, maxY-20, BLACK);	//riempi bordi


	//pulsantiera
	setcolor(RED);				//setta colore testo
	settextstyle(GOTHIC_FONT, VERT_DIR, 0);	//setta stile testo
	outtextxy(maxX-190,1,"Options Box");	//disegna testo

	setfillstyle(SOLID_FILL, YELLOW);	//setta colore e motivo sfondo barra
	setcolor(BLACK);			//setta colore linea barra
	setlinestyle(SOLID_LINE,1,2);		//setta stile linea barra
	bar3d(maxX-145, 11, maxX-12, maxY-4, 10, 1);//disegna barra
	setfillstyle(SOLID_FILL,GREEN);		//setta colore e motivo riempimento bordi
	floodfill(maxX-130, 6, BLACK);
	floodfill(maxX-7, maxY-50, BLACK);	//riempi bordi

	//pulsanti
	int west, nord, est, sud;
	buttonPos(0, west, nord, est, sud);	//primo pulsante
	bar3d(west, nord, est, sud, 5, 1);
	for (int i=1; i<MAXBOTTONI; i++){
		buttonPos(i, west, nord, est, sud);
		bar3d(west, nord, est, sud, 5, 0);
		}

	return;
}

void Interfac :: matrixCopy(int dest[DIMCASELLA][DIMCASELLA], int sorg[DIMCASELLA][DIMCASELLA]){
	int i, j;
	for (i = 0; i <= DIMCASELLA; i++){
		for (j = 0; j <= DIMCASELLA; j++){
			dest[i][j] = sorg[i][j];
		}
	}
	return;
}

int Interfac :: gridEff(int coord){
	return (50+(coord*(DIMCASELLA-1)));
}

void Interfac :: casella(int x, int y, char* sprite, int ruotato){
	BITMAP bmp;
	char dir[DIMTOTALSPRITE];
	strcpy(dir, directory);	//directory base degli sprites
	strcat(dir, sprite);	//directory dello sprite
	load_bmp(dir, bmp);	//carica il bitmap
	if (ruotato)		//se v… ruotato
		draw_bmp_vert(bmp, gridEff(x), gridEff(y), TRANSPARENT);//disegna verticale
	else//altrimenti
		draw_bmp_oriz(bmp, gridEff(x), gridEff(y), TRANSPARENT);//disegna normale
	free(bmp.data);		//libera la memoria
	return;
}

void Interfac :: bottone(int num, char* sprite){
	int west, nord, inutil;
	BITMAP bmp;
	char dir[DIMTOTALSPRITE];
	strcpy(dir, directory);	//directory base degli sprites
	strcat(dir, sprite);	//directory dello sprite

	buttonPos(num, west,nord,inutil,inutil);//coordinate fisiche bott
	load_bmp(dir, bmp);			//carica bitmap
	draw_bmp_oriz(bmp, west, nord, TRANSPARENT);	//disegna

	free(bmp.data);
	return;
}

void Interfac :: bottonSwitchOn(int pos){
	int est, nord, inutil;
	buttonPos(pos, inutil, nord, est, inutil);

	setfillstyle(SOLID_FILL, RED);
	floodfill(est+1, nord, BLACK);
	return;
}

void Interfac :: bottonSwitchOff(int pos){
	int est, nord, inutil;
	buttonPos(pos, inutil, nord, est, inutil);

	setfillstyle(SOLID_FILL, YELLOW);
	floodfill(est+1, nord, BLACK);
	return;
}

zone Interfac :: whotIsThis(int x, int y){
	int maxX = getmaxx();
	int maxY = getmaxy();

	if ( ((x >= 50)&&(x <= 350)) && ((y >= 50)&&(y <= 350)) )
		return GRIGLIA;
	else if( ((x >= maxX-145)&&(x <= maxX-12)) && ((y >= 11)&&(y <= maxY-4)) )
		return PULSANTIERA;
	else
		return NULLA;
}

int Interfac :: getCasella(int& x, int& y){
	if (whotIsThis(x, y) != GRIGLIA)
		return 1;//ERRORE valori fuori griglia
	if(((x-50)%50||(y-50)%50) == 0)
		return 2;//Errore, ha cliccato sui bordi
	x=(x-50)/50;
	y=(y-50)/50;
	return 0;
}

int Interfac :: getBottone(int x, int y, int& num){
	int west, nord, est, sud;
	for (int i =0; i <= MAXBOTTONI ; i++){
		buttonPos(i, west, nord, est, sud);
		if (((x > west)&&(x < est)) && ((y > nord)&&( y < sud))){
			num = i;
			return 0;
		}
	}
	return 1;
}

int Interfac :: info(char* stringa){
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
	setcolor(YELLOW);
	outtextxy(5, getmaxy()-35, lastInfo);	//cancella vecchia stringa

	if (strlen(stringa) > DIMINFO)
		return 1;
	setcolor(RED);
	outtextxy(5, getmaxy()-35, stringa);	//nuova stringa
	strcpy(lastInfo, stringa);
	return 0;
}


void Interfac :: immagine(int x, int y, char* sprite, int ruotato, int color){
	BITMAP bmp;
	char dir[DIMTOTALSPRITE];
	strcpy(dir, directory);	//directory base degli sprites
	strcat(dir, sprite);	//directory dello sprite
	load_bmp(dir, bmp);	//carica il bitmap
	if (ruotato)		//se v… ruotato
		draw_bmp_vert(bmp, x, y, color);//disegna verticale
	else//altrimenti
		draw_bmp_oriz(bmp, x, y, color);//disegna normale
	free(bmp.data);		//libera la memoria
	return;
}







