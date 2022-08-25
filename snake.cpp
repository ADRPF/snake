/* Autor: Felipe Peres de Almeida
*  Data: 21/08/2022 -- 25/08/2022
*  Descrição: Tentativa de recriar o jogo snake no terminal CMD do windows.
*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

#define FPS 4


char *world[10][11];

//strings que representam "sprites" a serem desenhados
char snakeSprt[] = "[X]";
char voidSprt[] = "[ ]";
char scoreSprt[] = "[*]";
//pontos consumidos pelo jogador
short points;

typedef struct point {
	short x;
	short y;
}P;

typedef struct snake {
	char dir_x; //guarda os caracteres l - left e r - right é usado caracter nulo \0 como um valor neutro
	char dir_y; //guarda os caracteres u - up e d - down é usado caracter nulo \0 como um valor neutro
	P point;
	struct snake *nxt;
}SNAKE;

void fillWorld(SNAKE snake, P point) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			world[i][j] = voidSprt;
		}
		world[i][10] = (char*)"\n";
	}
	world[snake.point.y][snake.point.x] = snakeSprt;
	world[point.y][point.x] = scoreSprt;
}

void printWorld(P point) {
	world[point.y][point.x] = scoreSprt;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 11; j++) {
			printf("%s", world[i][j]);
		}
	}
}

void getInput(SNAKE* snake) {
	if (GetAsyncKeyState(VK_UP) && snake->dir_y != 'd') {
		snake->dir_y = 'u';
		snake->dir_x = '\0';
	}
	if (GetAsyncKeyState(VK_DOWN) && snake->dir_y != 'u') {
		snake->dir_y = 'd';
		snake->dir_x = '\0';
	}
	if (GetAsyncKeyState(VK_LEFT) && snake->dir_x != 'r') {
		snake->dir_x = 'l';
		snake->dir_y = '\0';
	}
	if (GetAsyncKeyState(VK_RIGHT) && snake->dir_x != 'l') {
		snake->dir_x = 'r';
		snake->dir_y = '\0';
	}
}

void grow(SNAKE *current) {
	if (current->nxt == NULL) {
		current->nxt = (SNAKE*)malloc(sizeof(SNAKE));
		current->nxt->nxt = NULL;
		current->nxt->point = current->point;
	}
	else
		grow(current->nxt);
}

void step(SNAKE *snake) {
	P tmp = snake->point;
	P tmp2;
	SNAKE *body;

	body = snake->nxt;
	//move o resto do corpo da cobra -- moves the body of the snake
	while (body != NULL) {
		tmp2 = body->point;
		body->point = tmp;
		tmp = tmp2;
		world[body->point.y][body->point.x] = snakeSprt;
		body = body->nxt;
	}
	//verifica a direção que a cobra está apontado
	if (snake->dir_x == 'r') {
		snake->point.x++;
	}
	if (snake->dir_x == 'l') {
		snake->point.x--;
	}
	if (snake->dir_y == 'u') {
		snake->point.y--;
	}
	if (snake->dir_y == 'd') {
		snake->point.y++;
	}

	if (!((snake->point.x == 10 || snake->point.x == -1) || (snake->point.y == 10 || snake->point.y == -1))) world[snake->point.y][snake->point.x] = snakeSprt;
	world[tmp.y][tmp.x] = voidSprt; //reseta o rastro da cobra para um espaço vázio
}


bool checkScore(SNAKE *snake, P *point) {
	if (snake->point.x == point->x && snake->point.y == point->y) {
		point->x = rand() % 10;
		point->y = rand() % 10;
		points++;
		world[point->y][point->x] = scoreSprt;

		grow(snake);
		return TRUE;
	}
	return FALSE;
}


bool checkColision(SNAKE *snake) {
	SNAKE *body = snake->nxt;
	while (body != NULL) {
		if (snake->point.x == body->point.x && snake->point.y == body->point.y) return TRUE;
		body = body->nxt;
	}
	return FALSE;
}

bool checkOutOfBounds(SNAKE snake) {
	if ((snake.point.x <= 9 && snake.point.x >= 0) && (snake.point.y <= 9 && snake.point.y >= 0)) return TRUE;
	return FALSE;
}


void freeMemory(SNAKE *snake) {
	SNAKE *destroy;
	while (snake != NULL) {
		destroy = snake;
		snake = snake->nxt;
		free(destroy);
	}
}


int main()
{
	//atores da cena
	SNAKE snake;
	P p;

	//inicializando variaveis
	srand(time(NULL));
	snake.dir_x = 'r';
	snake.point.x = 4;
	snake.point.y = 4;
	snake.nxt = NULL;
	p.x = rand() % 10;
	p.y = rand() % 10;

	fillWorld(snake, p);
	printWorld(p);

	//variaveis de sincornizaçao de tempo
	int ticks = 0;
	int time = 0;
	//gameplay loop
	while (checkOutOfBounds(snake)) {
		getInput(&snake);
		ticks++;
		if(ticks == 60 / FPS){
			step(&snake);
			if (checkColision(&snake)) { system("cls"); break; }
			checkScore(&snake, &p);
			system("cls");
			printWorld(p);
			printf("\ntime: %d\nscore: %d\n", time / FPS, points);
			printf("\npos_x: %d\npos_y: %d\n", snake.point.x, snake.point.y);
			time++;
			ticks = 0;
		}
		Sleep(16);
	}
	freeMemory(snake.nxt);
	system("cls");
	printf("Voce morreu\n\npontos consumidos: %d\n", points);

	return 0;
}