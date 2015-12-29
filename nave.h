#include<string.h>
#define DIMSPRITE 30

enum bool {FALSE = 0, TRUE = 1};//definizione di un booleano
enum orientamenti {ORIZ=0, VERT};	//possibili orientamento

class Nave{
	private:
		int danni;	//percentuale danni
		int xPos;
		int yPos;	//posizione su griglia (estremo sinistro o superiore, dipende dall'orientamento)
		orientamenti orientamento;//verticale o orizzontale
		int lunghezza;	//numero caselle lunghezza
		int colpite;	//numero caselle colpite
		char sprite[DIMSPRITE];//percorso dello sprite
	public:
		Nave();
		~Nave();
		void inizializza(int, int, orientamenti, int, char*);//(posizione x, y, orientamento, lunghezza, sprite)
		int getPosBarra();	//ritorna la posizione sulla barra
		int getDanni();		//ritorna percentuale danni
		int getXPos();
		int getYPos();		//ritorna posizione
		orientamenti getOrientamento();//ritorna orientamento
		int getLunghezza();	//ritorna lunghezza
		char* getSprite();	//ritorna lo sprite
		void colpisci();	//aggiorna lo stato e i danni quando colpita

};

