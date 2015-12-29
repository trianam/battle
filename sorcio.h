#include<dos.h>

class Sorcio {
	private:
		union REGS reg;
                int cursor_flag;
		void action(int);
	public:
		Sorcio();
		~Sorcio();
		void reset();//posiziona il mouse al centro (non visibile)
		void cursEnab();//rende visibile il cursore
		void cursDisab();//rende invisibile il cursore
		void getStatus(int&, int&, int&);//(posizione orizzontale, verticale, stato tasti) restituisce stato mouse
		void setPosition(int, int);//(orizzontale, vericale) sposta il cursore nella posizione indicata
		void getPositionButt(int&, int&, int);//(orizzontale, vertivale, tasto da premere) restituisce posizione quando Š premuto il tasto indicato
		void getPositionButtRel(int&, int&, int);//lo stesso ma quando il tasto Š rilasciato
		void setLimits(int, int, int, int);//(west, est, nord, sud) setta i limiti di spostamento del cursore
		void setMickey(int, int);//(mickey orizzontale, mickey verticale) setta il rapporto di movimento, mickey su 8 pixel
};