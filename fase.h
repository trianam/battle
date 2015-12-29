#include"nave.cpp"

#define XGRIGLIA 6
#define YGRIGLIA 6
#define NUMNAVI 6

enum aif{RANDOM, SEARCH, PROSEG};
enum aiv{UP, DX, DN, SX};
/*enum aivX{XUP =0, XDX =1, XDN =-0, XSX =-1};
enum aivY{YUP =-1, YDX =0, YDN =1, YSX =-0};*/
enum colpi{ACQUA, COLPITO, AFFONDATO, ERRORE};
enum caselle{VUOTO, NAVE, SPRVUOTO, SPRNAVE, IMPOSS};//definisce i possibili stati della griglia

typedef struct{
	int x;     //colpo originale
	int y;
	aif fase;  //fase pensiero attuale
	aiv verso;	//verso di ricerca
	int proseg;	//proseguitore per la fase prosegui
}shoot;

class Fase{
	private:
		caselle enemyGrid[XGRIGLIA][YGRIGLIA];	//griglia nemica
		Nave enemyNavi[NUMNAVI];		//navi nemiche
		int cercaNave(int, int);			//Metodo che serve a sapere qual'Š la nave che colpisci
		shoot aiMem;
		void avanzaV(aiv&);//avanza il verso
		void calcolaSomm(aiv, int&, int&);//(verso, somm x, somm y) calcola i sommatori di coordinata
                void aggiornaImposs();	//aggiorna griglia colpi impossibili

	public:
		Fase();		//definisce il tipo
		~Fase();
		int addNave(int, int, orientamenti, int, int, char*);//(x, y, orient, lungh, posizione vett, sprite) aggiunge una nave al vettore
		int randomNavi();	//posiziona navi sulla griglia casualmente
		colpi spara(int, int);	//Metodo da chiamare quando si effettua un colpo sulla griglia(int, int coordinate logiche del punto cliccato)
		void aiSpara();//spara il computer
		int posNave(int, int&, int&, orientamenti&, int&);	//(numeronave, x, y, orientamento, lunghezza)restituisce la posizione della nave desiderata
		int numNave(int, int, int&);	//(x, y logiche, numero nave), data la posizione sulla griglia restituisce il numero
		int danniNave(int, int&);//(numnave, danni) restituisce i danni
		int spriteNave(int, char*&);//(numnave, sprite) restituisce lo sprite della nave
		int getCasella(int, int, caselle&);	//ritorna il valore della casella indicata
};