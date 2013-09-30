#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string.h>
#include <math.h>
#include "SDL/SDL_image.h"

#define LIN 10
#define COL 10
#define JEWEL 4

    SDL_Surface *joias[4];
    SDL_Surface *fundo=NULL;
    SDL_Surface *screen = NULL;

typedef struct{
    SDL_Rect rect;
    int tipo;
}PECA;
     //imagens

//Procedimentos
void GENERATOR(PECA matriz[LIN][COL]);
void PRINT_ARRAY(PECA matriz[LIN][COL]);
void CHANGE_POSITION(PECA matriz[][COL], int lin1, int col1, int lin2, int col2);
int CHECK_ADJACENT(PECA matriz[][COL], int x, int y);
int verifica(PECA matriz[][COL]);
void troca(PECA matriz[][COL]);
void quebra(PECA matriz[][COL]);
void guardajoias();
int escolhejoia(PECA matriz[LIN][COL], int x , int y);
//int verificarclique(int , int );
void blitarjogo(PECA matriz[][COL]);
void itoa(int n, char *s);
void reverse ( char *s); 
void BlitaPontos ();

int pontuacao = 0;

	TTF_Font *font;

int main(int argc, char* argv[])
{

    PECA matriz[LIN][COL];
    int l1, c1, l2, c2, i, fecharjogo=1, verificador=0;
    int x0=50;
    int y0=150;
	SDL_Rect rectfundo={0,0,800,640};
    
	fundo = IMG_Load("fundo1.jpg");


	//retângulos
    /*SDL_Rect quad  = {0,0,640,480};
	SDL_Rect quadInt  = {0,0,640,480};*/

    //iniciar funções
    SDL_Init (SDL_INIT_EVERYTHING);

    //audio
    /*Mix_Chunk *music3 = NULL;
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
    music3=Mix_LoadMUS("fundo.mp3");*/

    //criar tela
   screen = SDL_SetVideoMode (800 , 640, 32, SDL_SWSURFACE );

   //Titulo da tela
	SDL_WM_SetCaption( "Digimon Jóias", NULL );

	//aplica a imagen na superfície
   /* Mix_PlayMusic( music3, 1 );*/
	
    GENERATOR(matriz);

    guardajoias(joias);

    //eventos
    SDL_Event event;

   //texto
	font = TTF_OpenFont( "lazy.ttf", 28 );

        do{
            SDL_BlitSurface(fundo, NULL, screen, &rectfundo );	
       
	    blitarjogo(matriz);
	    BlitaPontos ();
            
	SDL_Flip(screen);

            while( SDL_PollEvent( &event ) ){

                if( event.type == SDL_QUIT ){
                    fecharjogo=0;
                }

                if( event.type == SDL_MOUSEBUTTONDOWN ){
           		//se o botao esquerdo do mouse esta pressioando
                    if( (event.button.x > x0) &&  (event.button.x < x0+10*joias[0]->w)  &&   (event.button.y > y0) &&  (event.button.y < y0 + 10*joias[0]->h)){
                        if(verificador == 0){
                            l1= (event.button.x - x0) / joias[0]->w;
                            c1= (event.button.y - y0) / joias[0]->h;

                            verificador = 1;
							continue;
                        }
                    	if(verificador == 1){
                        	l2= (event.button.x - x0) / joias[0]->w;
                        	c2= (event.button.y - y0) / joias[0]->h;

                        	verificador = 0;
                    	}
                    }
                }

        }            
	if ( (l1<0 || (l1>LIN-1)) || (c1<0 || (c1>COL-1)) || (l2<0 || (l2>LIN-1)) || (c2<0 || (c2>COL-1)) )
            {

                //printf("TROCA DE POSICOES INVALIDAS!\n");
                continue;
            }


            if ( l1-l2>1 || c2-c1>1 )
            {
                //printf("TROCA SOMENTE ENTRE ELEMENTOS VIZINHOS!\n");
                continue;
            }

            CHANGE_POSITION(matriz, l1, c1, l2, c2);

            if(CHECK_ADJACENT(matriz, l1, c1))
	    {

		//printf("Achou sequencias!");

	    }

	    else if(CHECK_ADJACENT(matriz, l2, c2))
	    {

	        //printf("Achou sequencias!");
            }

	    if(verifica(matriz)!=0){
            pontuacao+=verifica(matriz);
	    }
        if(verifica(matriz)==0){
        	//printf("Nao achou sequencias!");
			CHANGE_POSITION(matriz, l1, c1, l2, c2);
            }
            for(i=0;i<=20;i++){
                troca(matriz);
                //quebra(matriz);
            }

       } while(/*!((l1 < 0 || c1 < 0) || (l2 < 0 ||c2 < 0))*/fecharjogo==1);

        return 0;
    }


void guardajoias(){
    joias[0]=IMG_Load("1.png");
    joias[1]=IMG_Load("2.png");
    joias[2]=IMG_Load("3.png");
    joias[3]=IMG_Load("4.png");
}

int escolhejoia(PECA matriz[LIN][COL], int x, int y){

    if(matriz[x][y].tipo==1){
        return 1;
    }
    if(matriz[x][y].tipo==2){
        return 2;
    }
    if(matriz[x][y].tipo==3){
        return 3;
    }
    if(matriz[x][y].tipo==4){
        return 4;
    }
}
void blitarjogo(PECA matriz[][COL]){

  int x, y, joiaescolhida;
  int x0=50;
  int y0=150;

    for(x=0; x<LIN; x++){
        for(y=0; y<COL; y++){
            joiaescolhida=escolhejoia(matriz, x, y);
            matriz[x][y].rect.y = y0+y*40;
            matriz[x][y].rect.x = x0+x*40;
            SDL_BlitSurface( joias[joiaescolhida-1], NULL, screen, &matriz[x][y].rect );
        }
    }

}

    int CHECK_ADJACENT(PECA matriz[][COL], int x, int y){
        // retorna 1 se em um determinado ponto existe sequencias, 0 caso contrário.

        if(y + 2 <= LIN){
            if((matriz[x][y].tipo == matriz[x][y + 1].tipo) && (matriz[x][y].tipo == matriz[x][y + 2].tipo)){
                // Achou sequencias
                return 1;//retornará a posição de y para a troca.
            }
        }
        if(y - 2 >= 0){
            if((matriz[x][y].tipo == matriz[x][y - 1].tipo) && (matriz[x][y].tipo == matriz[x][y - 2].tipo)){
                // Achou sequencias
                return 1;// retornará a posição de y para troca.
            }
        }

        if(x + 2 <= COL){
            if((matriz[x][y].tipo == matriz[x + 1][y].tipo) && (matriz[x][y].tipo == matriz[x + 2][y].tipo)){
                // Achou sequencias
                return 1;//retornando o valor com parametro para quebra de linha correta.
            }
        }

        if(x - 2 >= 0){
            if((matriz[x][y].tipo == matriz[x - 1][y].tipo) && (matriz[x][y].tipo == matriz[x - 2][y].tipo)){
                // Achou sequencias
                return 1;//retornando o valor com parametro para quebra de linha correta.
            }
        }

        if((x - 1 >= 0) && (x + 1 <= COL)){
            if((matriz[x][y].tipo == matriz[x - 1][y].tipo) && (matriz[x][y].tipo == matriz[x + 1][y].tipo)){
                // Achou sequencias
                return 1;
            }
        }

        if((y - 1 >= 0) && (y + 1 <= LIN)){
            if((matriz[x][y].tipo == matriz[x][y - 1].tipo) && (matriz[x][y].tipo == matriz[x][y + 1].tipo)){
                // Achou sequencias
                return 1;
            }
        }

        // nao achou sequencias
        return 0;
    }

    void GENERATOR(PECA matriz[LIN][COL])
    {
        int i, j;

        srand(time(NULL));

        for(i=0; i<LIN; i++)
        {   for(j=0; j<COL; j++)
            {
                matriz[i][j].tipo = (rand()%JEWEL)+1;
            }
        }

        for(i=0; i<LIN; i++)
        {   for(j=0; j<COL; j++)
            {
               while(CHECK_ADJACENT(matriz, i, j))
               {
                      matriz[i][j].tipo = (rand()%JEWEL)+1;
                     }
                 }
            }
    }

/*void PRINT_ARRAY(PECA matriz[LIN][COL])
{
  int i, j;

     //DECORATE();

     for(i=0; i<LIN; i++)
     {   for(j=0; j<COL; j++)
         {
              printf("%2d ", matriz.tipo[i][j]);
         }
              printf("\n");
     }

}*/

void CHANGE_POSITION(PECA  matriz[][COL], int lin1, int col1, int lin2, int col2)
{
   int troca;

    troca = matriz[lin1][col1].tipo;
    matriz[lin1][col1].tipo = matriz[lin2][col2].tipo;
    matriz[lin2][col2].tipo = troca;

}

int verifica(PECA matriz[][COL]){

    int x, y, pontuacao=0;

    for(x=0; x<10; x++){
        for(y=0; y+2<10; y++){
            if((matriz[x][y].tipo==matriz[x][y+1].tipo)&&(matriz[x][y].tipo==matriz[x][y+2].tipo)){
                pontuacao+=100;
            }

        }
    }
    for(y=0; y<10; y++){
        for(x=0; x+2<10; x++){
            if((matriz[x][y].tipo==matriz[x+1][y].tipo)&&(matriz[x][y].tipo==matriz[x+2][y].tipo)){
                pontuacao+=100;
            }

        }
    }
    return pontuacao;

}

void troca(PECA matriz[][COL]){

    int matcp[LIN][COL];
    int x, y, i;

    for(x=0; x<10; x++){
        for(y=0; y<10; y++){
            matcp[x][y]=0;
        }
    }
    for(x=0; x<10; x++){
        for(y=0; y+2<10; y++){
            if((matriz[x][y].tipo==matriz[x][y+1].tipo)&&(matriz[x][y].tipo==matriz[x][y+2].tipo)){
                matcp[x][y]=1;
                matcp[x][y+1]=1;
                matcp[x][y+2]=1;
                y++;
                y++;
            }

        }
    }
    for(y=0; y<10; y++){
        for(x=0; x+2<10; x++){
            if((matriz[x][y].tipo==matriz[x+1][y].tipo)&&(matriz[x][y].tipo==matriz[x+2][y].tipo)){
                matcp[x][y]=1;
                matcp[x+1][y]=1;
                matcp[x+2][y]=1;
                x++;
                x++;
            }

        }
    }

    srand(time(NULL));

    for(y=0; y<10; y++){
        for(x=0; x<10; x++){
            if(matcp[x][y]==1){
                i=x;
                while(i-1>=0){
                    matriz[i][y].tipo=matriz[i-1][y].tipo;
                    i--;
                }
                matriz[i][y].tipo=(rand()%JEWEL)+1;
            }

        }
    }
}

/*void quebra(PECA matriz[][COL]){

    int x, y, i=0;

    srand(time(NULL));

    for(y=0; y<10; y++){
        for(x=0; x<10; x++){
            if(matriz[x][y].tipo==8){
                i=x;
                while(i-1>=0){
                    matriz[i][y].tipo=matriz[i-1][y].tipo;
                    i--;
                }
                matriz[i][y].tipo=(rand()%JEWEL)+1;
            }

        }
    }
}*/

void itoa(int n, char s[]) 
{ 
	int i, sign;
		
		if ((sign = n) < 0) 
			n = -n;
			i = 0; 
		
		do { 
			s[i++] = n % 10 + '0';
		   } while ((n /= 10) > 0);


		if (sign < 0) 

			s[i] = '-';

				s[i] = '\0'; 

			reverse(s); 
} 

void reverse (char s[]) 
{ 

	int i, j; char c;
	
		for (i = 0, j = strlen(s)-1; i<j; i++, j--) { 
			
			c = s[i]; 
			s[i] = s[j]; 
			s[j] = c; 
		
		} 
}



void BlitaPontos () 
{ 
	char pont[10]; 

	itoa(pontuacao,pont);
	
	SDL_Rect textLocation = { 0, 600, 0, 0 }; 
	SDL_Color foregroundColor = { 255, 255, 255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid (font, "Score:", foregroundColor);
	SDL_BlitSurface (textSurface, NULL, screen, &textLocation);
	SDL_Color PontColor = { 255, 240, 50 };	
	SDL_Rect PontLocation = { 0, 600, 0, 0 }; 	
	textSurface = TTF_RenderText_Solid (font, pont, PontColor); 
	SDL_BlitSurface (textSurface, NULL, screen, &PontLocation);
	SDL_FreeSurface (textSurface); 
}
