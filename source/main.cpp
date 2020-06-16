#include <allegro.h>

//objetos
struct obj{int wx,wy,x,y,w,h,dir,bk;};
struct obj  p = {42, 0,14,23,42,44,5}, f[4],
         frut = {166, 0,370,352,50,40};

//funções
void sair();
void fant();
void mapa(int);
void control();
void restart();

//Variáveis Globais
int x      =   0;
int sai    =   0;
int som    =   0;
int mov    =   0;
int msg    =   0;
int tTxt   =   0;
int txtX   =   0;
int txtY   =   0;
int antX   =   0;
int antY   =   0;
int next   =   0;
int fase   =   0;
int vida   =   0;
int macFan = 100;
int vidas  =   3;
int morre  =   0; 
int inicio =   0;
int tFrut  =   0;
int tPower =   0; 
int width  =1048;
int height = 670;
int pontos =   0;
int hscore =1000;
int map[31][28] ;

SAMPLE *ponto , *pilula, *start, *sfase, *fruta, *macfan, *morto;
BITMAP *buffer, *itens, *fundo;
FONT   *f28_arial, *f18;

int main() {
	
    //Iniciação
	allegro_init();
    install_keyboard();
	set_color_depth(32);
	set_window_title("Meu Programa");
	set_close_button_callback(sair);
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);  
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
	
	//Variáveis Locais
	buffer = create_bitmap(width, height);
	fundo  = load_bitmap("../img/fundo.bmp", NULL);
	itens  = load_bitmap("../img/itens.bmp", NULL);
	f28_arial    = load_font("../img/f28.pcx", NULL, NULL);
	f18    = load_font("../img/f18.pcx", NULL, NULL);
	start  = load_sample("../sons/start.wav");
	sfase  = load_sample("../sons/fase.wav");
	morto  = load_sample("../sons/morto.wav");
	ponto  = load_sample("../sons/ponto.wav");
	fruta  = load_sample("../sons/fruta.wav");
	pilula = load_sample("../sons/pilula.wav");
	macfan = load_sample("../sons/macfan.wav");
	int i;
	for(i = 0; i <  4; i++){
		f[i].wx  =  0;
		f[i].wy  = 53 + i*41;
		f[i].x   = 12 + i;		
		f[i].y   = 14;
		f[i].w   = 40;
		f[i].h   = 40;
		f[i].dir =  1;		
	}
	mapa(1);	
	while (!(sai || key[KEY_ESC]))
	{	
		if(hscore < pontos) hscore = pontos;
		antX = p.x;
		antY = p.y;	
		draw_sprite(buffer, fundo, 5, 5);
		if(tTxt) textprintf_ex(buffer, f18, txtX, txtY, 0xffffff, -1, "%i", msg);//mensagem pontos	
		textprintf_right_ex(buffer, f28_arial, 980, 250, 0xffffff, -1,"%i", pontos);   //Pontos
		textprintf_right_ex(buffer, f28_arial, 980, 84, 0xffffff, -1, "%i", hscore);   // hscore
		if(tTxt) tTxt--;  //tempo para exibir a mensagem
		if(vida) control();
		mapa(0);
		//Exibindo frutas
		if(tFrut++ > 200) tFrut = 0; //tempo para exibir a fruta
		if(tFrut > 150){
			masked_blit(itens, buffer, frut.wx, frut.wy + fase*frut.h, frut.x, frut.y, frut.w, frut.h);
			//Pegando frutas
			if(p.y == 17 && (p.x == 13 || p.x == 14)){
				msg  = 100 + (fase*100);
				tTxt = 20;
				tFrut = 0;
				pontos+= msg;
				txtX  = frut.x;
				txtY  = frut.y + 5; 
				play_sample(fruta, 255, 128, 1000, 0);
			}
		}			
		
		if(vida || inicio) masked_blit(itens, buffer, p.wx + x, p.wy, p.x*28 - 8, p.y*21 - 7, p.w, p.h);
		for(i = 0; i < vida; i++) masked_blit(itens, buffer, 84, 0, 820 + i*45, 600, 42, 44);
		fant();
		draw_sprite(screen, buffer, 0, 0);
		rest(150);
		clear(buffer);
		restart();	
		//Sons	
		 	 if(som == 2){stop_sample(ponto); play_sample(ponto, 255, 128, 1000, 0);}
		else if(som == 3){play_sample(pilula, 255, 128, 1000, 0);}
		som = 0;	
	}	
	//Finalização
	destroy_font(f28_arial);
	destroy_font(f18);
	destroy_bitmap(itens);
	destroy_bitmap(fundo);
	destroy_bitmap(buffer);
	destroy_sample(start);
	destroy_sample(ponto);
	destroy_sample(macfan);
	destroy_sample(pilula);
	
	return 0;
}
END_OF_MAIN();

void restart(){
	int i;
	if(morre || !vida) stop_sample(sfase);
	if(inicio){
		play_sample(start, 255, 128, 1000, 0);	
		vida = vidas;
		rest(5000);
		play_sample(sfase, 255, 128, 1000, 1);
		inicio = 0;
		pontos = 0;
		fase = 0;		
	}
	if(key[KEY_SPACE] && !vida){
		while(key[KEY_SPACE]) ; inicio = 1;
	}
	if(next == 568){
		fase++;
		stop_sample(sfase);
		stop_sample(pilula);	
		for(i = 0; i < 5; i++){
			draw_sprite(screen, fundo, 5, 5);
			rest(500);
			rectfill(screen, 0, 0, 780, 680, 0x00);
			rest(500);
		}
	}else if(morre && vida){
		vida--;
		stop_sample(sfase);
		rest(500);
		play_sample(morto, 255, 128, 1000, 0);
		for(i = 0; i < 7; i++){
			draw_sprite(buffer, fundo, 5, 5);	
			masked_blit(itens, buffer, p.wx + i*p.w, 261, p.x*28 - 8 - inicio*16, p.y*21 - 7, p.w, p.h);
			mapa(0);		
			draw_sprite(screen, buffer, 0, 0);			
			rest(200);
			clear(buffer);
		}
	}

	if(inicio || (next == 568) || morre){
		if(!morre) mapa(1);
		tPower = 0;
		tFrut  = 0;
		p.x = 14;
		p.y = 23;
		p.dir = 4;
		morre = 0;
		macFan = 100; 
		tTxt  = 0;
		for(i = 0; i < 4; i++){
			f[i].x = 12 + i;
			f[i].y = 14;
			f[i].bk = 0;
		}
		if(!inicio) play_sample(sfase, 255, 128, 1000, 1);
	}
}

void fant(){
	int i;
	
	for(i = 0; i < 4; i++){
		
		//Fantasmas voltando ao normal
		if(f[i].x == 13 && (f[i].y == 13 || f[i].y == 14)) tPower ? (f[i].bk = 1) : (f[i].bk = 0);
		
		if(f[i].bk){
			     if(f[i].y  ==  5 && f[i].x > 9 && f[i].x < 18)                   {f[i].dir = 0;}//grande  T encima  
			else if(f[i].y  == 23 && f[i].x >   1  && f[i].x <  4)                {f[i].dir = 0;}  //7 abaixo
			else if(f[i].y  == 23 && f[i].x >  23  && f[i].x < 26)                {f[i].dir = 2;}  //F abaixo 
			else if(f[i].y  <  15 && f[i].y > 11 && (f[i].x == 9 || f[i].x == 18)){f[i].dir = 1;}//sobe fatasmas até a caixa 
			else if((f[i].y == 17)&& f[i].x > 9 && f[i].x < 18)                   {f[i].dir = 2;}   //embaixo da caixa frutas
			else if((f[i].y == 20 || f[i].y == 26) && f[i].x > 14 && f[i].x < 18) {f[i].dir = 2;}  //Ts centrais dir-> cima e baixo
			else if((f[i].y == 20 || f[i].y == 26) && f[i].x >  9 && f[i].x < 13) {f[i].dir = 0;}  //Ts centrais esq-> cima e baixo								
			else if(f[i].y  >  14 && map[f[i].y-1][f[i].x] != 1)                   f[i].dir = 1; // centraliza para cima				
			else if(f[i].y  <  13 &&(f[i].x != 9 || f[i].y == 5) && f[i].x != 18 && map[f[i].y+1][f[i].x] != 1) f[i].dir = 3; // centraliza para baixo
			else if(f[i].x  <  13 && map[f[i].y][f[i].x+1] != 1)                   f[i].dir = 2; // centraliza para direita	
			else if(f[i].x  >  13 && map[f[i].y][f[i].x-1] != 1)                   f[i].dir = 0; // centraliza para esquerda
			
		}else{
			//saindo da caixa
			if(f[i].x  >  10 && f[i].x  < 17  &&  f[i].y == 14) f[i].dir  = rand()%4;
			else if((f[i].y == 12 || f[i].y == 13) && (f[i].x == 13 || f[i].x == 14))  f[i].dir = 1;
			//Fantasmas travessando a tela
	  		else if(f[i].x <  1) f[i].x = 27;                  //saída pela esquerda
			else if(f[i].x > 26 && f[i].dir == 2) f[i].x =  1; //Saída pela direita	
			//Blinky, o fantasma vermelho, tem como função perseguir obstinadamente você durante todo o jogo
			else if(!tPower && i == 3 && f[3].x < p.x && f[3].dir != 0 && map[f[3].y][f[3].x+1] != 1) f[3].dir = 2;
			else if(!tPower && i == 3 && f[3].x > p.x && f[3].dir != 2 && map[f[3].y][f[3].x-1] != 1) f[3].dir = 0;
			else if(!tPower && i == 3 && f[3].y < p.y && f[3].dir != 1 && map[f[3].y+1][f[3].x] != 1) f[3].dir = 3;
			else if(!tPower && i == 3 && f[3].y > p.y && f[3].dir != 3 && map[f[3].y-1][f[3].x] != 1) f[3].dir = 1;
			// Pinky (o fantasma rosa), se posiciona em um ponto que é 32 pixels a frente do Pac-Man
			else if(i == 2 && f[2].x < p.x+16 && f[2].dir != 0 && map[f[2].y][f[2].x+1] != 1) f[2].dir = 2; 
			else if(i == 2 && f[2].x > p.x+16 && f[2].dir != 2 && map[f[2].y][f[2].x-1] != 1) f[2].dir = 0; 
			else if(i == 2 && f[2].y < p.y+16 && f[2].dir != 1 && map[f[2].y+1][f[2].x] != 1) f[2].dir = 3; 
			else if(i == 2 && f[2].y > p.y+16 && f[2].dir != 3 && map[f[2].y-1][f[2].x] != 1) f[2].dir = 1; 
			//fantasma azul, Inky, tenta posicionar-se em um local semelhante
			else if(i == 1 && f[1].x < p.x-8 && f[1].dir != 0 && map[f[1].y][f[1].x+1] != 1) f[1].dir = 2; 
			else if(i == 1 && f[1].x > p.x-8 && f[1].dir != 2 && map[f[1].y][f[1].x-1] != 1) f[1].dir = 0; 
			else if(i == 1 && f[1].y < p.y-8 && f[1].dir != 1 && map[f[1].y+1][f[1].x] != 1) f[1].dir = 3; 
			else if(i == 1 && f[1].y > p.y-8 && f[1].dir != 3 && map[f[1].y-1][f[1].x] != 1) f[1].dir = 1; 	
			else {//Clyde, o fantasma laranja, move-se completamente ao acaso.
				/// direita ///
				if(f[i].dir == 2 && map[f[i].y][f[i].x+1] == 1){
					if(map[f[i].y-1][f[i].x] != 1 && map[f[i].y+1][f[i].x] != 1)
						(rand()%2) ? (f[i].dir = 1) : (f[i].dir = 3);
					else (map[f[i].y-1][f[i].x] != 1) ? (f[i].dir = 1) : (f[i].dir = 3);
				}
				///// Esquerda  ////////////////
				if(f[i].dir == 0 && map[f[i].y][f[i].x-1] == 1){
					if(map[f[i].y-1][f[i].x] != 1 && map[f[i].y+1][f[i].x] != 1)
						(rand()%2) ? (f[i].dir = 1) : (f[i].dir = 3);
					else (map[f[i].y-1][f[i].x] != 1) ? (f[i].dir = 1) : (f[i].dir = 3);
				}
				///// Baixo ////////////////
				if(f[i].dir == 3 && map[f[i].y+1][f[i].x] == 1){
					if(map[f[i].y][f[i].x-1] != 1 && map[f[i].y][f[i].x+1] != 1)
						(rand()%2) ? (f[i].dir = 0) : (f[i].dir = 2);
					else (map[f[i].y][f[i].x-1] != 1) ? (f[i].dir = 0) : (f[i].dir = 2);
				}
			 	////////Cima///////////////
				if(f[i].dir == 1 && map[f[i].y-1][f[i].x] == 1){
					if(map[f[i].y][f[i].x-1] != 1 && map[f[i].y][f[i].x+1] != 1)
						(rand()%2) ? (f[i].dir = 0) : (f[i].dir = 2);
					else (map[f[i].y][f[i].x-1] != 1) ? (f[i].dir = 0) : (f[i].dir = 2);
				}
			}
		}
		
		
	         if(f[i].dir == 0 && map[f[i].y][f[i].x-1] != 1) f[i].x--; 	//esq
		else if(f[i].dir == 1 && map[f[i].y-1][f[i].x] != 1) f[i].y--;  //cima 
		else if(f[i].dir == 2 && map[f[i].y][f[i].x+1] != 1) f[i].x++;  //dir
		else if(f[i].dir == 3 && map[f[i].y+1][f[i].x] != 1) f[i].y++;  //baixo
		
		//Exibição dos fantasmas
		if(f[i].bk)     masked_blit(itens, buffer, f[i].dir*41, 217, f[i].x*28 - 8, f[i].y*21 - 7, f[i].w, f[i].h); //olhos
		else if(tPower) masked_blit(itens, buffer, 164, 176, f[i].x*28 - 8, f[i].y*21 - 7, f[i].w, f[i].h);	  //Azul
		else            masked_blit(itens, buffer, f[i].wx + f[i].dir*41, f[i].wy, f[i].x*28 - 8, f[i].y*21 - 7, f[i].w, f[i].h); // normal	
		
		//Colisão com fantasmas
		if(vida && !f[i].bk && (p.x == f[i].x && p.y == f[i].y || antX == f[i].x && antY == f[i].y)){
			if(tPower){
				f[i].bk = 1;
				macFan *= 2;
				tTxt = 30;
				pontos+= macFan;
				msg  = macFan;
				txtX = f[i].x*28-f[i].w/2;
				txtY = f[i].y*21;
				play_sample(macfan, 255, 128, 1000, 0);
				rest(500);
			}else morre = 1;		 	
	 	}
	}
}

void control(){
	if(key[KEY_LEFT] && map[p.y][p.x-1] != 1) p.dir = 0;
	if(key[KEY_UP]   && map[p.y-1][p.x] != 1) p.dir = 1;
	if(key[KEY_RIGHT]&& map[p.y][p.x+1] != 1) p.dir = 2;
	if(key[KEY_DOWN] && map[p.y+1][p.x] != 1) p.dir = 3;	
	
	if(p.dir == 0 && map[p.y][p.x-1] != 1){p.x--; p.wx = 0;}
	if(p.dir == 1 && map[p.y-1][p.x] != 1) p.y--;
	if(p.dir == 2 && map[p.y][p.x+1] != 1){p.x++; p.wx = 41;}
	if(p.dir == 3 && map[p.y+1][p.x] != 1) p.y++;
	
	//Abrindo e fechando a boca 
	if(mov > 4) { x = 0; mov = 0;}
	if(mov++ > 1) x = 42;
		
	//Atravessando a tela
	if(p.x > 26 &&  p.dir) p.x =  0;
	if(p.x <  1 && !p.dir) p.x = 27;
	
	//Recolhe objetos
	if(map[p.y][p.x] == 2){pontos+= 10; som = 2; map[p.y][p.x] = 0;}
	//pegando pilulas
	if(map[p.y][p.x] == 3){  
		som = 3; 
		pontos+= 50;
		tPower = 50;
		map[p.y][p.x] = 0;			
	}
	if(tPower) tPower--;
	if(tPower == 1) stop_sample(pilula);
	
}

void mapa(int mod){
	int i, j;

	char mp[31][28] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
					   {1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
					   {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
					   {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
					   {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
					   {1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,4,4,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,0,0,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {0,0,0,0,0,0,2,2,2,2,1,0,0,0,0,0,0,1,2,2,2,2,0,0,0,0,0,0},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
					   {1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
					   {1,2,2,2,1,1,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,1,1,2,2,2,1},
					   {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
					   {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
					   {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
					   {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};	
	next = 0;				   					   
	for(i = 0; i < 31; i++){
		for(j = 0; j < 28; j++){
			if(mod)	map[i][j] = mp[i][j];
			if(map[i][j] == 2) masked_blit(itens, buffer,   10, 10, j*28 + 10, i*21 + 16,  6,  6);//Pontos	
			if(map[i][j] == 3 && mov < 3) masked_blit(itens, buffer, 127,  0, j*28 +  3, i*21 + 10, 18, 18);//Pilula
			if(map[i][j]) next++;	
		}
	}					
	
}

void sair(){sai= 1;}
END_OF_FUNCTION(sair);













