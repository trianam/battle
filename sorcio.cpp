#include"sorcio.h"

#define RESET 0x0		//valori del reg ax per le varie funzioni
#define CURSENAB 0x1
#define CURSDISAB 0x2
#define GETSTATUS 0x3
#define SETPOSITION 0x4
#define GETPOSITIONBUTT 0x5
#define GETPOSITIONBUTTREL 0x6
#define SETLIMITS_ORIZ 0x7
#define SETLIMITS_VERT 0x8
#define SETMICKEY 0x0F
#define AX reg.x.ax
#define BX reg.x.bx
#define DX reg.x.dx
#define CX reg.x.cx

Sorcio :: Sorcio(){
	cursor_flag = -1;	//valore iniziale del cursor flag
}

Sorcio :: ~Sorcio(){
}

void Sorcio :: action(int ax){
	AX = ax;		//assegna valore chiamata
	int86(0x33,&reg,&reg);	//interrupt del mouse
}

void Sorcio :: reset(){
	action(RESET);
	return;
}

void Sorcio :: cursEnab(){
	if(cursor_flag){	//se cursor flag = -1
		action(CURSENAB);
		cursor_flag++;	//cursor flag = 0
	}
	return;
}

void Sorcio :: cursDisab(){
	if(!cursor_flag){	//se cursor flag = 0
		action(CURSDISAB);
		cursor_flag--;	//cursor flag = -1
	}
	return;
}

void Sorcio :: getStatus(int& oriz, int& vert, int& butt){
	action(GETSTATUS);
	oriz = CX;	//assegna i valori di ritorno dello stato
	vert = DX;
	butt = BX;
	return;
}

void Sorcio :: setPosition(int oriz, int vert){
	CX = oriz;	//assegna posizione da settare
	DX = vert;
	action(SETPOSITION);
	return;
}

void Sorcio :: getPositionButt(int& oriz, int& vert, int butt){
	BX = butt;	//bottone da premere
	action(GETPOSITIONBUTT);
	oriz = CX;	//stato
	vert = DX;
	return;
}

void Sorcio :: getPositionButtRel(int& oriz, int& vert, int butt){
	BX = butt;	//bottone da rilasciare
	action(GETPOSITIONBUTTREL);
	oriz = CX;	//stato
	vert = DX;
	return;
}

void Sorcio :: setLimits(int west, int est, int nord, int sud){
	CX = west;	//assegna limiti orizzontali
	DX = est;
	action(SETLIMITS_ORIZ);
	CX = nord;	//assegna limiti verticali
	DX = sud;
	action(SETLIMITS_VERT);
	return;
}

void Sorcio :: setMickey(int mickeyOriz, int mickeyVert){
	CX = mickeyOriz;	//setta i rapporti
	DX = mickeyVert;
	action(SETMICKEY);
	return;
}

