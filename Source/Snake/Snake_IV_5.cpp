#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<windows.h>
#include<iostream>

#define MAPX (28)
#define MAPY (28)
using namespace std;

char TEST = 0; 

enum SIGN{GROUND, HEAD, UP, DOWN, RIGHT, LEFT, STAR, STAR2,
	AI_WORKING, AUTHOR, SPEED_UP, SLOW_DOWN, OPEN_LIGHT, NORMAL, EAT,
	CRASH, NO_REACTION, FREEZE};
void FindPath(char water[][MAPY], char map[][MAPY], int x, int y, int X, int Y, int ring);
void F_Count_Blocks(char water[][MAPY], char map[][MAPY], int x, int y, int ring);
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput,scrn);
}


void pause(void)
{
	if(getch() == 224)
	{
		getch();
	}
}
class object
{
	int a, b;
	
	char (*map)[MAPY];
	
	char (*Blocks)[3];
	int *Colors, *Light_Colors;
	int starx, stary;
	int open_light;
	int ring;
	
	public:
		object()
		{
			map = (char (*)[MAPY])malloc( sizeof( char[MAPY] ) * MAPX);
			for(a = 0; a < MAPX; a++)
			{
				for(b = 0; b < MAPY; b++)
				{
					map[a][b] = GROUND;
				}
			}
			map[MAPX-1][0] = RIGHT;
			map[MAPX-1][1] = RIGHT;
			map[MAPX-1][2] = HEAD;
			Blocks = (char (*)[3])malloc( sizeof( char [3] ) * 8);
			Colors = (int *)malloc( sizeof(int) * 8);
			Light_Colors = (int *)malloc( sizeof(int) * 8);
			strcpy(Blocks[0], "�@");	Colors[0] = 7;	Light_Colors[0] = 8;
			strcpy(Blocks[1], "��");	Colors[1] = 10;	Light_Colors[1] = 160;
			strcpy(Blocks[2], "��");	Colors[2] = 2;	Light_Colors[2] = 32;
			strcpy(Blocks[3], "��");	Colors[3] = 2;	Light_Colors[3] = 32;
			strcpy(Blocks[4], "��");	Colors[4] = 2;	Light_Colors[4] = 32;
			strcpy(Blocks[5], "��");	Colors[5] = 2;	Light_Colors[5] = 32;
			strcpy(Blocks[6], "��");	Colors[6] = 14;	Light_Colors[6] = 224;
			strcpy(Blocks[7], "��");	Colors[7] = 14;	Light_Colors[7] = 224;
			open_light = 0;
			ring = 0;
		}
		
		int monitor(int x, int y, char water[][MAPY], char mode)
		{
			if(mode == 1)
			{
				FindPath(water, map, starx, stary, -1, -1, ring);
			}
			if(open_light == 0)
			{
				gotoxy(2 * y + 2, x + 1);
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					Colors[ map[x][y] ] );
				
				cout<< Blocks[ map[x][y] ];
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					7 );
			}
			else
			{
				for(a = 0; a < MAPX; a++)
				{
					gotoxy(2, a + 1);
					for(b = 0; b < MAPY; b++)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							Light_Colors[ map[a][b] ] );
						printf("%2d", water[a][b]);
					}
				}
			}
			
			return 0;
		}
		int fix(int x, int y, int value, char water[][MAPY], char mode)
		{
			int temp;
			if(x >= 0 && x < MAPX && y >= 0 && y < MAPY )
			{
				temp = map[x][y];
				map[x][y] = value;
				monitor(x, y, water, mode);
			}
			else
			{
				temp = -1;
			}
			return temp;
		}
		void setstar(char water[][MAPY], char mode, FILE *fptr)
		{
			do
			{
				starx = rand()%MAPX;
				stary = rand()%MAPY;
			}while(map[starx][stary] != GROUND);
			
			if(TEST == 1)
			{ 
				fprintf(fptr,"%d %d\n",starx,stary); 
			} 
			
			fix(starx, stary, STAR, water, mode);
			return ;
		}
		
		friend class Snake;
		friend void welcome(class object * const MAP);
		friend int main(void);
		friend char Input(char mode, char water[][MAPY], class object MAP, class Snake *snake);
};
class Snake
{
	int headx, heady;
	int tailx, taily;
	int dirx, diry;
	char trace[4];
	int count;
	char change;
	char next_U;
	unsigned int length, score;
	char AI_all[MAPX][MAPY]; 
	public:
		Snake()
		{
			int a, b; 
			headx = MAPX - 1;
			heady = 2;
			tailx = MAPX - 1;
			taily = 0;
			dirx = 0;
			diry = 1;
			for(count = 0; count < 4; count++)
			{
				trace[count] = -1;
			}
			count = 0;
			length = 3;
			score = 0;
			change = 0;
			next_U = 4;
			for(a = 1; a < MAPX - 1; a++)
			{
				for(b = 0;( a != MAPX/2 )&&( a != MAPX/2 + 1 )&&( b < MAPY ); b++)
				{
					if(b % 2 == 0)
					{
						AI_all[a][b] = DOWN;
					}
					else
					{
						AI_all[a][b] = UP;
					}
				}
			}
			for(b = 1; b < MAPY - 1; b++)
			{
				if(b % 2 == 0)
				{
					AI_all[0][b] = DOWN;
					AI_all[MAPX/2][b] = LEFT;
					AI_all[MAPX/2 + 1][b] = DOWN;
					AI_all[MAPX - 1][b] = RIGHT;
				}
				else
				{
					AI_all[0][b] = LEFT;
					AI_all[MAPX/2][b] = UP;
					AI_all[MAPX/2 + 1][b] = RIGHT;
					AI_all[MAPX - 1][b] = UP;
				}
			}
			AI_all[0][0] = DOWN;
			AI_all[0][MAPY - 1] = LEFT;
			AI_all[MAPX/2][0] = DOWN;
			AI_all[MAPX/2 + 1][0] = DOWN;
			AI_all[MAPX - 1][0] = RIGHT;
			AI_all[MAPX/2][MAPY - 1] = UP;
			AI_all[MAPX/2 + 1][MAPY - 1] = UP;
			AI_all[MAPX - 1][MAPY - 1] = UP;
			/**
			gotoxy(0,0); 
			for(a=0;a<MAPX;a++) 
			{
				for(b=0;b<MAPY;b++)
				{
					if(AI_all[a][b] == UP)
					{
						cout<<"��";
					}
					else if(AI_all[a][b] == LEFT)
					{
						cout<<"��";
					}
					else if(AI_all[a][b] == RIGHT)
					{
						cout<<"��";
					}
					else if(AI_all[a][b] == DOWN)
					{
						cout<<"��";
					}
				}
				cout<<endl; 
			}
			getch();
			/**/
			
			/**
			gotoxy(2*(heady)+2,headx+1);
			cout<<"..";
			pause();/**/
		}
		void fixdir(char inputdir)
		{
			if(inputdir == -1)
			{
				cout<<"There is something that out of consideration.";
				getch();
			}
			else
			{
				if(inputdir == UP)
				{
					if(dirx == 0)
					{
						dirx = -1;
						diry = 0;
					}
				}
				else if(inputdir == LEFT)
				{
					if(diry == 0)
					{
						dirx = 0;
						diry = -1;
					}
				}
				else if(inputdir == RIGHT)
				{
					if(diry == 0)
					{
						dirx = 0;
						diry = 1;
					}
				}
				else if(inputdir == DOWN)
				{
					if(dirx == 0)
					{
						dirx = 1;
						diry = 0;
					}
				}
			}
			return ;
		}
		char move(class object MAP, char water[][MAPY], char mode)
		{
			int sit;
			int temp;
			if(dirx == -1)
			{
				temp = UP;
			}
			else if(diry == -1)
			{
				temp = LEFT;
			}
			else if(diry == 1)
			{
				temp = RIGHT;
			}
			else if(dirx == 1)
			{
				temp = DOWN;
			}
			trace[count] = temp;
			count = (count + 1) % 4;
			MAP.fix(headx, heady, temp, water, mode);
			headx += dirx;
			heady += diry;
			if(MAP.ring)
			{
				headx = (headx + MAPX)% MAPX;
				heady = (heady + MAPY)% MAPY;
			}
			sit = MAP.fix(headx, heady, HEAD, water, mode);
			if(sit == -1 || (sit != STAR && sit != STAR2 && sit != GROUND))
			{
				sit = CRASH;
				gotoxy(2 * heady + 2, headx + 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MAP.Colors[HEAD]);
				cout<<MAP.Blocks[HEAD];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				
			}
			else if(sit != STAR && sit!= STAR2)
			{
				sit = NORMAL;
				temp = MAP.map[tailx][taily];
				MAP.fix(tailx, taily, GROUND, water, mode);
				if(temp == UP)
				{
					tailx--;
				}
				else if(temp == LEFT)
				{
					taily--;
				}
				else if(temp == RIGHT)
				{
					taily++;
				}
				else if(temp == DOWN)
				{
					tailx++;
				}
				if(MAP.ring)
				{
					tailx = (tailx + MAPX)% MAPX;
					taily = (taily + MAPY)% MAPY;
				}
			}
			else
			{
				sit = EAT;
			}
			return sit;
		}
		char AI(char water[][MAPY], class object MAP);
		char AI_2(char water[][MAPY], class object MAP);
		
		friend int main(void);
		friend char Input(char mode, char water[][MAPY], class object MAP, class Snake *snake);
};
void welcome(class object * const MAP)
{
	int a,b;
	char str[2][9] = {"��ɫʳ�","��ɶ}��"};
	SetConsoleTitle("Snake_IV_5");
	gotoxy(0,0);
	for(a = 0; a < MAPX + 12; a++)
	{
		cout<<"��";
	}
	for(a = 0; a < MAPX; a++)
	{
		cout<<"��";
		for(b = 0; b < MAPY; b++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				(*MAP).Colors[ (*MAP).map[a][b] ] );
			
			cout<< (*MAP).Blocks[ (*MAP).map[a][b] ];
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				(*MAP).Colors[ (*MAP).map[a][b] ] );
		}
		cout<<"���@�@�@�@�@�@�@�@�@��";
	}
	for(a = 0; a < MAPX + 12; a++)
	{
		cout<<"��";
	}
	gotoxy(2 * MAPY + 4, 2);
	cout<<"�@�g�Y�D";
  	gotoxy(2 * MAPY + 4, 3);
  	cout<<"�@�@ Snake_IV_5";
  	gotoxy(2 * MAPY + 4, 5);
  	cout<<"�@�@   By pi314 ";
  	gotoxy(2 * MAPY + 4, 7);
  	cout<<"������������������";
  	gotoxy(2 * MAPY + 4, 9);
  	cout<<"�@�@�C���{�p";
  	gotoxy(2 * MAPY + 4, 11);
  	cout<<"�@����:   3";
  	gotoxy(2 * MAPY + 4, 12);
  	cout<<"�@�t��: 1/20";
  	gotoxy(2*MAPY+4,13);
  	cout<<"�@����:   0";
  	gotoxy(2*MAPY+4,15);
  	cout<<"������������������";
  	gotoxy(2*MAPY+4,17);
  	cout<<"�@�@�C���ާ@";
  	gotoxy(2*MAPY+4,19);
  	cout<<"�@��������";
  	gotoxy(2*MAPY+4,20);
  	cout<<"�@�@����D��";
  	gotoxy(2*MAPY+4,21);
  	cout<<"�@�@�@��i��V";
  	gotoxy(2*MAPY+4,23);
  	cout<<"�@+ - �[�t��t";
  	gotoxy(2*MAPY+4,25);
  	cout<<"�@�����@�@�@����";
  	gotoxy(2*MAPY+4,26);
  	cout<<"���� ���q�Ҧ� ����";
  	gotoxy(2*MAPY+4,27);
  	cout<<"�@�����@�@�@����";
  	
	gotoxy(16, 11);
	cout<<"�п�ܹC���Ҧ�(��,��,Enter):";
	gotoxy(16, 13);
	cout<<"�� ��ɫʳ� ��";
	gotoxy(16, 15);
	cout<<"�@ ��ɶ}�� ";
	do
	{
		a = (char)getch();
		if(a == -32 || a == 0)
		{
			b = (char)getch();
		}
		if(a == -32 && b == 72)
		{
			if((*MAP).ring == 1)
			{
				gotoxy(16, 13);
				cout<<"�� ��ɫʳ� ��";
				gotoxy(16, 15);
				cout<<"�@ ��ɶ}�� �@";
			}
			(*MAP).ring = 0;
		}
		else if(a == -32 && b == 80)
		{
			if((*MAP).ring == 0)
			{
				gotoxy(16, 13);
				cout<<"�@ ��ɫʳ� �@";
				gotoxy(16, 15);
				cout<<"�� ��ɶ}�� ��";
			}
			(*MAP).ring = 1;
		}
		else if(a == 27)
		{
			if(TEST == 0)
			{
				SetConsoleTitle("TEST = 1");
				Sleep(500);
				TEST = 1;
			}
			else
			{
				SetConsoleTitle("TEST = 0");
				Sleep(500);
				TEST = 0;
			}
			SetConsoleTitle("Snake_IV_5");
		}
	}while(a != 13);
	gotoxy(16, 11);
	cout<<"�w�w�w�w�w�w�w�w�w�w�w�w�w�w";
	gotoxy(16, 13);
	cout<<"�w�w�w�w�w�w�w";
	gotoxy(16, 15);
	cout<<"�w�w�w�w�w�w�w";
	Sleep(30);
	gotoxy(16, 11);
	cout<<"�@�@�@�@�@�@�@�@�@�@�@�@�@�@";
	gotoxy(16, 13);
	cout<<"�@�@�@�@�@�@�@";
	gotoxy(16, 15);
	cout<<"�@�@�@�@�@�@�@";
		gotoxy(16, 11);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(16, 11);
		cout<<"�@�@�@�@";
		gotoxy(14, 9);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(14, 9);
		cout<<"�@�@�@�@";
		gotoxy(12, 7);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(12, 7);
		cout<<"�@�@�@�@";
		gotoxy(10, 5);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(10, 5);
		cout<<"�@�@�@�@";
		gotoxy(8, 3);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(8, 3);
		cout<<"�@�@�@�@";
		Sleep(30);
	if((*MAP).ring == 1)
	{
		SetConsoleTitle("Snake_IV_5 �w ��ɶ}��");
	}
	else
	{
		SetConsoleTitle("Snake_IV_5 �w ��ɫʳ�");
	}
	return ;
}
char Input(char mode, char water[][MAPY], class object MAP, class Snake *snake)
{
	char ch[2] = {-1, -1};
	int a, b;
	
	if(kbhit())
	{
		ch[0] = getch();
		if(ch[0] == -32 || ch[0] == 0)
		{
			ch[1] = getch();
		}
	}
	if(ch[0] == 27)
	{
		return AI_WORKING;
	}
	else if(ch[0] == '`')
	{
		return AUTHOR;
	}
	else if(ch[0] == '+' || (ch[0] == -32 && ch[1] == 73))
	{
		return SPEED_UP;
	}
	else if(ch[0] == '-' || (ch[0] == -32 && ch[1] == 81))
	{
		return SLOW_DOWN;
	}
	else if(ch[0] == '\\' || (ch[0] == 0 && ch[1] == 59))
	{
		return OPEN_LIGHT;
	}
	else if(ch[0] == 'p' || ch[0] == 'P' || ch[0] == 32) 
	{
		return FREEZE; 
	} 
	if(mode == 0)
	{
		if(ch[1] == 72 || ch[0] == 'w' || ch[0] == 'W')
		{
			return UP;
		}
		else if(ch[1] == 75 || ch[0] == 'a' || ch[0] == 'A')
		{
			return LEFT;
		}
		else if(ch[1] == 77 || ch[0] == 'd' || ch[0] == 'D')
		{
			return RIGHT;
		}
		else if(ch[1] == 80 || ch[0] == 's' || ch[0] == 'S')
		{
			return DOWN;
		}
		else
		{
			return NO_REACTION;
		}
	}
	else if(mode == 1)
	{
		if(MAP.Colors[6] == 12)
		{
			return (*snake).AI_2(water, MAP);
		}
		else
		{
			return (*snake).AI(water, MAP);
		} 
	} 
	cout<<"Input -1";
	return -1;
}
char Snake::AI(char water[][MAPY], class object MAP)
{
	char temp;
	int a, b;
	int max;
	char Count_Blocks[MAPX][MAPY];
	int count_blocks[4];/*�W���k�U*/
	
	FindPath(water, MAP.map, MAP.starx, MAP.stary, -1, -1, MAP.ring);
	/*�Ʈ�l*/ 
	for(a = 0; a < 4; a++)
	{
		count_blocks[a] = 0;
	}
	F_Count_Blocks(Count_Blocks, MAP.map, headx, heady, MAP.ring);
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(Count_Blocks[a][b] % 2 == 0)
			{
				count_blocks[0]++;
			}
			if(Count_Blocks[a][b] % 3 == 0)
			{
				count_blocks[1]++;
			}
			if(Count_Blocks[a][b] % 5 == 0)
			{
				count_blocks[2]++;
			}
			if(Count_Blocks[a][b] % 7 == 0)
			{
				count_blocks[3]++;
			}
		}
	}
	max = 0;
	for(a = 1; a < 4; a++) 
	{
		if(count_blocks[a] == count_blocks[max])
		{
			if(a == 1 &&( heady - 1 >= 0 || MAP.ring))
			{
				if(water[headx][(heady - 1+MAPY)%MAPY] < water[headx][heady] && water[headx][(heady - 1+MAPY)%MAPY] > 0)
					max = 1;
			}
			else if(a == 2 && (heady + 1 < MAPY || MAP.ring))
			{
				if(water[headx][(heady + 1)%MAPY] < water[headx][heady] && water[headx][(heady + 1)%MAPY] > 0)
					max = 2;
			}
			else if(a == 3 && (headx + 1 < MAPX || MAP.ring))
			{
				if(water[(headx + 1)%MAPX][heady] < water[headx][heady] && water[(headx + 1)%MAPX][heady] > 0)
					max = 3;
			}
		}
		else if(count_blocks[a] > count_blocks[max]) 
		{
			max = a; 
		} 
	} 
	/*�Ʈ�l����*/
		if(MAP.ring == 0)
		{
			if(headx + dirx < 0 || headx + dirx >= MAPX || 
				heady + diry < 0 || heady + diry >= MAPY ||
				water[headx + dirx][heady + diry] == -1)
			{
				a = 0;
			}
			else
			{
				a = 1;
			}
		}
		else
		{
			if(water[(headx + dirx + MAPX)%MAPX][(heady + diry + MAPY)%MAPY] == -1)
			{
				a = 0;
			}
			else
			{
				a = 1;
			}
		}
		if(!a)/*����*/
		{
			if(max == 0)
			{
				return UP;
			}
			else if(max == 1)
			{
				return LEFT;
			}
			else if(max == 2)
			{
				return RIGHT;
			}
			else if(max == 3)
			{
				return DOWN;
			}
		}
		
		temp = -1;
		if(water[headx][heady] != 0)/*�����i�Y��P�P*/
		{
			change = 0;
			next_U = 4;
			if(MAP.ring == 0 && water[headx + dirx][heady + diry] < water[headx][heady] && water[headx + dirx][heady + diry] > 0)
			{
				temp = NO_REACTION;
			}
			else if(MAP.ring == 1 && water[(headx + dirx+MAPX)%MAPX][(heady + diry+MAPY)%MAPY] < water[headx][heady] && water[headx + dirx][heady + diry] > 0)
			{
				temp = NO_REACTION;
			}
			else if(temp == -1)/*�~�򨫷|�����P�P*/
			{
				if(!MAP.ring)
				{
					if(headx - 1 >= 0)
					{
						if((water[headx - 1][heady] < water[headx][heady]) && 
							(water[headx - 1][heady] > 0) )
						{
							temp = UP;
						}
					}
					if((heady - 1 >= 0) && (temp == -1))
					{
						if((water[headx][heady - 1] < water[headx][heady]) && 
							(water[headx][heady - 1] > 0) )
						{
							temp = LEFT;
						}
					}
					if((heady + 1 < MAPY) && (temp == -1) )
					{
						if((water[headx][heady + 1] < water[headx][heady]) && 
							(water[headx][heady + 1] > 0) )
						{
							temp = RIGHT;
						}
					}
					if((headx + 1 < MAPX) && (temp == -1) )
					{
						if((water[headx + 1][heady] < water[headx][heady]) && 
						(water[headx + 1][heady] > 0) )
						{
							temp = DOWN;
						}
					}
				}
				else
				{
					if((water[(headx - 1+MAPX)%MAPX][heady] < water[headx][heady]) && 
						(water[(headx - 1+MAPX)%MAPX][heady] > 0) )
					{
						temp = UP;
					}
					if((water[headx][(heady - 1+MAPY)%MAPY] < water[headx][heady]) && 
						(water[headx][(heady - 1+MAPY)%MAPY] > 0) && (temp == -1) )
					{
						temp = LEFT;
					}
					if((water[headx][(heady + 1)%MAPY] < water[headx][heady]) && 
						(water[headx][(heady + 1)%MAPY] > 0) && (temp == -1) )
					{
						temp = RIGHT;
					}
					if((water[(headx + 1)%MAPX][heady] < water[headx][heady]) && 
					(water[(headx + 1)%MAPX][heady] > 0) && (temp == -1) )
					{
						temp = DOWN;
					}
				}
				/**
				cout<<"turn -1.";
				return -1;/**/ 
			}
		}
		else/*�S���i�Y��P�P*/
		{
			if(dirx != 0)
			{
				a = b = 0;
				if(trace[((int)count + 0) % 4] == RIGHT || trace[((int)count + 0) % 4] == LEFT)
					a = 1;
				if(trace[((int)count + 2) % 4] == RIGHT || trace[((int)count + 2) % 4] == LEFT)
					b = 1;
				
				if(a * b)
				{
					/**
					gotoxy(0,0);
					for(a=3;a>=0;a--)
					{
						cout<<(int)trace[((int)count + a + 4) % 4]<<' ';
					}
					cout<<(int)change;
					getch();/**/
					temp = -1;
					if(MAP.ring == 0)
					{
						if(heady - 1 >= 0 && MAP.map[headx][heady - 1] == GROUND && change < next_U)
						{
							change++;
							temp = LEFT;
						}
						else if(heady + 1 < MAPY && MAP.map[headx][heady + 1] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = RIGHT;
						}
						else if(temp == -1) 
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
					else
					{
						if(MAP.map[headx][(heady - 1+MAPY)%MAPY] == GROUND && change < next_U)
						{
							change++;
							temp = LEFT;
						}
						else if(MAP.map[headx][(heady + 1)%MAPY] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = RIGHT;
						}
						else if(temp == -1) 
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
				}
				else
				{
					temp = NO_REACTION;
				}
				
			}
			else if(diry != 0)
			{
				a = b = 0;
				if(trace[((int)count + 0) % 4] == UP || trace[((int)count + 0) % 4] == DOWN)
					a = 1;
				if(trace[((int)count + 2) % 4] == UP || trace[((int)count + 2) % 4] == DOWN)
					b = 1;
				
				if(a * b)
				{
					/**
					gotoxy(0,0);
					for(a=3;a>=0;a--)
					{
						cout<<(int)trace[((int)count + a + 4) % 4]<<' ';
					}
					cout<<(int)change;
					getch();/**/
					if(MAP.ring == 0)
					{
						if(headx - 1 >= 0 && MAP.map[headx - 1][heady] == GROUND && change < next_U)
						{
							change++;
							temp = UP;
						}
						else if(headx + 1 < MAPX && MAP.map[headx + 1][heady] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = DOWN;
						}
						else if(temp == -1)
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
					else
					{
						if(MAP.map[(headx - 1+MAPX)%MAPX][heady] == GROUND && change < next_U)
						{
							change++;
							temp = UP;
						}
						else if(MAP.map[(headx + 1)%MAPX][heady] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = DOWN;
						}
						else if(temp == -1)
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
				}
				else
				{
					temp = NO_REACTION;
				}
				
			}
		}
	if(temp == NO_REACTION)
	{
		if(dirx == -1)
		{
			temp = UP;
		}
		else if(diry == -1)
		{
			temp = LEFT;
		}
		else if(diry == 1)
		{
			temp = RIGHT;
		}
		else if(dirx == 1)
		{
			temp = DOWN;
		}
	}
	if(temp == UP) 
	{
		temp = 0; 
	}
	else if(temp == LEFT)
	{
		temp = 1;
	}
	else if(temp == RIGHT)
	{
		temp = 2;
	}
	else if(temp == DOWN)
	{
		temp = 3;
	}
	if( (count_blocks[max] - count_blocks[temp]) > (length / 2))
	{
		temp = max;
		/**
		cout<<'['<<(int)temp<<']';
		getch();/**/
	} 
	
	if(temp == 0)
	{
		return UP;
	}
	else if(temp == 1)
	{
		return LEFT;
	}
	else if(temp == 2)
	{
		return RIGHT;
	}
	else if(temp == 3)
	{
		return DOWN;
	}
}
char Snake::AI_2(char water[][MAPY], class object MAP)
{
	char temp;
	int a, b;
	int max;
	char Count_Blocks[MAPX][MAPY];
	int count_blocks[4];/*�W���k�U*/
	
	
	FindPath(water, MAP.map, MAP.starx, MAP.stary, -1, -1, MAP.ring);
	/*�Ʈ�l*/ 
	for(a = 0; a < 4; a++)
	{
		count_blocks[a] = 0;
	}
	F_Count_Blocks(Count_Blocks, MAP.map, headx, heady, MAP.ring);
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(Count_Blocks[a][b] % 2 == 0)
			{
				count_blocks[0]++;
			}
			if(Count_Blocks[a][b] % 3 == 0)
			{
				count_blocks[1]++;
			}
			if(Count_Blocks[a][b] % 5 == 0)
			{
				count_blocks[2]++;
			}
			if(Count_Blocks[a][b] % 7 == 0)
			{
				count_blocks[3]++;
			}
		}
	}
	max = 0;
	for(a = 1; a < 4; a++) 
	{
		if(count_blocks[a] == count_blocks[max])
		{
			if(a == 1)
			{
				if(heady - 1 >= 0||MAP.ring)
				{
					if(water[headx][(heady - 1+MAPY)%MAPY] < water[headx][heady])
						max = 1;
				}
			}
			else if(a == 2)
			{
				if(heady + 1 < MAPY || MAP.ring)
				{
					if(water[headx][(heady + 1)%MAPY] < water[headx][heady])
						max = 2;
				}
			}
			else if(a == 3)
			{
				if(headx + 1 < MAPX || MAP.ring)
				{
					if(water[(headx + 1)%MAPX][heady] < water[headx][heady])
						max = 3;
				}
			}
		}
		else if(count_blocks[a] > count_blocks[max]) 
		{
			max = a; 
		} 
	} 
	/*�Ʈ�l����*/
	if(headx + dirx >= 0 && headx + dirx < MAPX && 
		heady + diry >= 0 && heady + diry < MAPY && 
		water[headx + dirx][heady + diry] == -1)/*����*/ 
	{
		if(max == 0)
		{
			return UP;
		}
		else if(max == 1)
		{
			return LEFT;
		}
		else if(max == 2)
		{
			return RIGHT;
		}
		else if(max == 3)
		{
			return DOWN;
		}
	}
	
	temp = AI_all[headx][heady];
	/**
	cout<<(int)temp;
	getch(); /**/ 
	if(temp == NO_REACTION)
	{
		if(dirx == -1)
		{
			temp = UP;
		}
		else if(diry == -1)
		{
			temp = LEFT;
		}
		else if(diry == 1)
		{
			temp = RIGHT;
		}
		else if(dirx == 1)
		{
			temp = DOWN;
		}
	}
	if(temp == UP) 
	{
		temp = 0; 
	}
	else if(temp == LEFT)
	{
		temp = 1;
	}
	else if(temp == RIGHT)
	{
		temp = 2;
	}
	else if(temp == DOWN)
	{
		temp = 3;
	}
	if( (count_blocks[max] - count_blocks[temp]) > (length / 2))
	{
		temp = max;
	} 
	
	if(temp == 0)
	{
		return UP;
	}
	else if(temp == 1)
	{
		return LEFT;
	}
	else if(temp == 2)
	{
		return RIGHT;
	}
	else if(temp == 3)
	{
		return DOWN;
	}
}
void FindPath(char water[][MAPY], char map[][MAPY], int x, int y, int X, int Y, int ring)
{
	int a, b;
	int stx[2][MAPX * MAPY], sty[2][MAPX * MAPY], top, top2, distance;
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(map[a][b] == GROUND || map[a][b] == STAR || map[a][b] == STAR2 || 
				map[a][b] == HEAD)
			{
				water[a][b] = 0;
			}
			else
			{
				water[a][b] = -1;
			}
		}
	}
	if(X != -1 && Y != -1)
	{
		water[X][Y] = -1;
	}
	top = 1;
	stx[1][0] = x;
	sty[1][0] = y;
	if(water[x][y] == -1)
		return ;
	water[x][y] = 1;
	distance = 2;
	top2 = 0;
	while(top)
	{
		for(a = 0; a < top; a++)
		{
			if(!ring)
			{
				if(stx[(distance + 1) % 2][a] - 1 >= 0)
				{
					if( water[stx[(distance + 1) % 2][a] - 1][sty[(distance + 1) % 2][a]] == 0)
					{
						water[stx[(distance + 1) % 2][a] - 1][sty[(distance + 1) % 2][a]] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a] - 1;
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
						top2++;
					}
				}
				if(sty[(distance + 1) % 2][a] - 1 >= 0)
				{
					if( water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] - 1] == 0)
					{
						water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] - 1] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a] - 1;
						top2++;
					}
				}
				if(sty[(distance + 1) % 2][a] + 1 < MAPY)
				{
					if( water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] + 1] == 0)
					{
						water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] + 1] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a] + 1;
						top2++;
					}
				}
				if(stx[(distance + 1) % 2][a] + 1 < MAPX)
				{
					if( water[stx[(distance + 1) % 2][a] + 1][sty[(distance + 1) % 2][a]] == 0)
					{
						water[stx[(distance + 1) % 2][a] + 1][sty[(distance + 1) % 2][a]] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a] + 1;
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
						top2++;
					}
				}
			}
			else
			{
				if( water[(stx[(distance + 1) % 2][a]-1+MAPX)%MAPX][sty[(distance + 1) % 2][a]] == 0)
				{
					water[(stx[(distance + 1) % 2][a]-1+MAPX)%MAPX][sty[(distance + 1) % 2][a]] = distance;
					stx[distance % 2][top2] = (stx[(distance + 1) % 2][a]-1+MAPX)%MAPX;
					sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
					top2++;
				}
				if( water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]-1+MAPY)%MAPY] == 0)
				{
					water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]-1+MAPY)%MAPY] = distance;
					stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
					sty[distance % 2][top2] = (sty[(distance + 1) % 2][a]-1+MAPY)%MAPY;
					top2++;
				}
				if( water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]+1)%MAPY] == 0)
				{
					water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]+1)%MAPY] = distance;
					stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
					sty[distance % 2][top2] = (sty[(distance + 1) % 2][a]+1)%MAPY;
					top2++;
				}
				if( water[(stx[(distance + 1) % 2][a]+1)%MAPX][sty[(distance + 1) % 2][a]] == 0)
				{
					water[(stx[(distance + 1) % 2][a]+1)%MAPX][sty[(distance + 1) % 2][a]] = distance;
					stx[distance % 2][top2] = (stx[(distance + 1) % 2][a]+1)%MAPX;
					sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
					top2++;
				}
			}
		}
		top = top2;
		top2 = 0;
		distance++;
	}
	return ;
}

void F_Count_Blocks(char water[][MAPY], char map[][MAPY], int x, int y, int ring)
{
	int a, b;
	int stx[2][MAPX * MAPY], sty[2][MAPX * MAPY], top, top2, distance;
	int attribute[2][MAPX * MAPY];
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(map[a][b] == GROUND || map[a][b] == STAR || map[a][b] == STAR2 || 
				map[a][b] == HEAD)
			{
				water[a][b] = 1;
			}
			else
			{
				water[a][b] = 0;
			}
		}
	}
	top = 0;
	/**
	stx[1][0] = x;
	sty[1][0] = y;/**/
	if(!ring)
	{
		if(x - 1 >= 0)
		{
			if(water[x - 1][y] != 0)
			{
				water[x - 1][y] *= 2;
				stx[0][top] = x - 1;
				sty[0][top] = y;
				attribute[0][top] = 2;
				top++;
			}
		}
		if(y - 1 >= 0)
		{
			if(water[x][y - 1] != 0)
			{
				water[x][y - 1] *= 3;
				stx[0][top] = x;
				sty[0][top] = y - 1;
				attribute[0][top] = 3;
				top++;
			}
		}
		if(y + 1 < MAPY)
		{
			if(water[x][y + 1] != 0)
			{
				water[x][y + 1] *= 5;
				stx[0][top] = x;
				sty[0][top] = y + 1;
				attribute[0][top] = 5;
				top++;
			}
		}
		if(x + 1 < MAPX)
		{
			if(water[x + 1][y] != 0)
			{
				water[x + 1][y] *= 7;
				stx[0][top] = x + 1;
				sty[0][top] = y;
				attribute[0][top] = 7;
				top++;
			}
		}
	}
	else
	{
		if(water[(x - 1 +MAPX)%MAPX][y] != 0)
		{
			water[(x - 1 +MAPX)%MAPX][y] *= 2;
			stx[0][top] = (x - 1 +MAPX)%MAPX;
			sty[0][top] = y;
			attribute[0][top] = 2;
			top++;
		}
		if(water[x][(y - 1+MAPY)%MAPY] != 0)
		{
			water[x][(y - 1+MAPY)%MAPY] *= 3;
			stx[0][top] = x;
			sty[0][top] = (y - 1+MAPY)%MAPY;
			attribute[0][top] = 3;
			top++;
		}
		if(water[x][(y + 1)%MAPY] != 0)
		{
			water[x][(y + 1)%MAPY] *= 5;
			stx[0][top] = x;
			sty[0][top] = (y + 1)%MAPY;
			attribute[0][top] = 5;
			top++;
		}
		if(water[(x + 1)%MAPX][y] != 0)
		{
			water[(x + 1)%MAPX][y] *= 7;
			stx[0][top] = (x + 1)%MAPX;
			sty[0][top] = y;
			attribute[0][top] = 7;
			top++;
		}
	}
	water[x][y] = 0;
	top2 = 0;
	distance = 0;
	while(top)
	{
		/** 
		gotoxy(0,0);
		printf("%d\n",distance); 
		for(a=0;a<top;a++) 
			printf("%d %d %d\n",stx[distance][a], sty[distance][a], attribute[distance][a]);
		getch(); /**/ 
		for(a = 0; a < top; a++)
		{
			if(!ring)
			{
				if(stx[distance][a] - 1 >= 0)
				{
					if( water[stx[distance][a] - 1][sty[distance][a]] % attribute[distance][a] != 0)
					{
						water[stx[distance][a] - 1][sty[distance][a]] *= attribute[distance][a];
						stx[(distance + 1) % 2][top2] = stx[distance][a] - 1;
						sty[(distance + 1) % 2][top2] = sty[distance][a];
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
				if(sty[distance][a] - 1 >= 0)
				{
					if( water[stx[distance][a]][sty[distance][a] - 1] % attribute[distance][a] != 0)
					{
						water[stx[distance][a]][sty[distance][a] - 1] *= attribute[distance][a];
						stx[(distance + 1) % 2][top2] = stx[distance][a];
						sty[(distance + 1) % 2][top2] = sty[distance][a] - 1;
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
				if(sty[distance][a] + 1 < MAPY)
				{
					if( water[stx[distance][a]][sty[distance][a] + 1] % attribute[distance][a] != 0)
					{
						water[stx[distance][a]][sty[distance][a] + 1] *= attribute[distance][a];
						stx[(distance + 1) % 2][top2] = stx[distance][a];
						sty[(distance + 1) % 2][top2] = sty[distance][a] + 1;
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
				if(stx[distance][a] + 1 < MAPX)
				{
					if( water[stx[distance][a] + 1][sty[distance][a]] % attribute[distance][a] != 0)
					{
						water[stx[distance][a] + 1][sty[distance][a]] *= attribute[distance][a];
						stx[(distance + 1) % 2][top2] = stx[distance][a] + 1;
						sty[(distance + 1) % 2][top2] = sty[distance][a];
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
			}
			else
			{
				if( water[(stx[distance][a] - 1+MAPX)%MAPX][sty[distance][a]] % attribute[distance][a] != 0)
				{
					water[(stx[distance][a] - 1+MAPX)%MAPX][sty[distance][a]] *= attribute[distance][a];
					stx[(distance + 1) % 2][top2] = (stx[distance][a] - 1+MAPX)%MAPX;
					sty[(distance + 1) % 2][top2] = sty[distance][a];
					attribute[(distance + 1) % 2][top2] = attribute[distance][a];
					top2++;
				}
				if( water[stx[distance][a]][(sty[distance][a] - 1+MAPY)%MAPY] % attribute[distance][a] != 0)
				{
					water[stx[distance][a]][(sty[distance][a] - 1+MAPY)%MAPY] *= attribute[distance][a];
					stx[(distance + 1) % 2][top2] = stx[distance][a];
					sty[(distance + 1) % 2][top2] = (sty[distance][a] - 1+MAPY)%MAPY;
					attribute[(distance + 1) % 2][top2] = attribute[distance][a];
					top2++;
				}
				if( water[stx[distance][a]][(sty[distance][a] + 1)%MAPY] % attribute[distance][a] != 0)
				{
					water[stx[distance][a]][(sty[distance][a] + 1)%MAPY] *= attribute[distance][a];
					stx[(distance + 1) % 2][top2] = stx[distance][a];
					sty[(distance + 1) % 2][top2] = (sty[distance][a] + 1)%MAPY;
					attribute[(distance + 1) % 2][top2] = attribute[distance][a];
					top2++;
				}
				if( water[(stx[distance][a] + 1)%MAPX][sty[distance][a]] % attribute[distance][a] != 0)
				{
					water[(stx[distance][a] + 1)%MAPX][sty[distance][a]] *= attribute[distance][a];
					stx[(distance + 1) % 2][top2] = (stx[distance][a] + 1)%MAPX;
					sty[(distance + 1) % 2][top2] = sty[distance][a];
					attribute[(distance + 1) % 2][top2] = attribute[distance][a];
					top2++;
				}
			}
		}
		top = top2;
		top2 = 0;
		distance = (distance + 1) % 2;
	}
	return ;
}
int main(void)
{
	srand(time(NULL));
	FILE *fptr;
	class object MAP;
	class Snake snake;
	char mode;
	char sit;
	char water[MAPX][MAPY] = {0};
	int speed_list[] = {250,167,125,100,83,71,63,56,50,45,42,38,36,33,31,29,28,26,25,20,10};
	int bling_counter;
	int a, b;
	int author_display;
	int speed,lowest_speed;
	int move_permition; 
	
	
	
	welcome(&MAP);
	
	if(TEST) 
	{ 
		fptr=fopen("star.txt","a"); 
		fprintf(fptr,"========\n");
	} 
	
	MAP.setstar(water, mode, fptr);
	
	mode = 0;
	bling_counter = 0;
	author_display = 0;
	speed = 0;
	lowest_speed = 0;
	move_permition = 1; 
	while(1)
	{
		Sleep( speed_list[speed] );
		bling_counter += speed_list[speed];
		if(bling_counter>=300)
		{
			bling_counter = 0;
			if(MAP.map[MAP.starx][MAP.stary] == STAR)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR2, water, mode);
			}
			else if(MAP.map[MAP.starx][MAP.stary] == STAR2)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR, water, mode);
			}
		}
		if(kbhit() || mode)
		{
			sit = Input(mode, water, MAP, &snake);
			if(sit == AI_WORKING)
			{
				mode = (mode + 1) % 2;
				gotoxy(2 * MAPY + 8, 26);
				cout<<"�w�w�w�w�w";
				Sleep(50);
				gotoxy(2 * MAPY + 8, 26);
				if(mode == 0) 
				{
					cout<<" ���q�Ҧ� ";
				}
				else if(mode == 1)
				{
					cout<<" �۰ʴM�| ";
				}
			}
			else if(sit == AUTHOR)
			{
				author_display = (author_display + 1) % 2;
				gotoxy(2 * MAPY + 4, 5);
				cout<<"�@�@   �w�w�w�w ";
				Sleep(50);
				gotoxy(2 * MAPY + 4, 5);
				if(author_display == 0)
				{
					cout<<"�@�@   By pi314 ";
				}
				else
				{
					cout<<"�@�@   By ������";
				}
			}
			else if(sit == OPEN_LIGHT)
			{
				MAP.open_light = (MAP.open_light + 1) % 2;
				if(MAP.open_light == 0)
				{
					for(a = 0; a < MAPX; a++)
					{
						for(b = 0; b < MAPY; b++)
						{
							MAP.monitor(a, b, water, mode);
						}
					}
				}
			}
			else if(sit == SPEED_UP)
			{
				speed = speed < 19 ? speed + 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"�w";
				Sleep(30);
				printf("\b\b%2d", speed + 1);
				if(mode == 1)
				{
					sit = Input(mode, water, MAP, &snake);
				}
			}
			else if(sit == SLOW_DOWN)
			{
				speed = speed > lowest_speed ? speed - 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"�w";
				Sleep(30);
				printf("\b\b%2d", speed + 1);
				if(mode == 1)
				{
					sit = Input(mode, water, MAP, &snake);
				}
			}
			else if(sit == FREEZE) 
			{
				move_permition = (move_permition + 1) % 2;
			} 
			snake.fixdir(sit);
		}
		if(move_permition)
		{ 
				sit = snake.move(MAP, water, mode); 
				FindPath(water, MAP.map, MAP.starx, MAP.stary, -1, -1, MAP.ring);
			if(sit == CRASH)
			{
				break;
			}
			else if(sit == EAT)
			{
				snake.length++;
				gotoxy(2 * MAPY + 12, 11);
				cout.width(3);
				cout<<snake.length;
				
				snake.score += (speed + 1) * 2 + (snake.length)/2;
  				gotoxy(2 * MAPY + 12, 13);
				cout.width(3);
  				cout<<snake.score;
  				if((snake.length - 3) % 7 == 0 && snake.length > 3)
  				{
					if(lowest_speed < 20 && TEST == 0)
					{ 
						lowest_speed++;
						if(speed < lowest_speed) 
						{
							speed = lowest_speed;
							gotoxy(2 * MAPY + 11, 12);
							cout<<"�w";
							Sleep(30);
							printf("\b\b%2d", speed + 1);
						}
						if(lowest_speed == 20)
						{
							MAP.Colors[6] = MAP.Colors[7] = 12;
						}
					} 
				}
  				
  				if(snake.length < MAPX * MAPY)
  				{
					MAP.setstar(water, mode, fptr);
				}
			}
		}
	}
	if(TEST) 
	{ 
		fclose(fptr) ; 
	} 
	gotoxy(15,13);
	cout<<"�C�������A�Ы��ť��䵲���C���C";
	do
	{
		Sleep( speed_list[speed] );
		bling_counter += speed_list[speed];
		if(bling_counter>=300)
		{
			bling_counter = 0;
			if(MAP.map[MAP.starx][MAP.stary] == STAR)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR2, water, mode);
			}
			else if(MAP.map[MAP.starx][MAP.stary] == STAR2)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR, water, mode);
			}
		}
		if(kbhit())
		{
			a = getch();
			if(a == 224 || a == 0)
			{
				b = getch();
			}
			if(a == '`')
			{
				author_display = (author_display + 1) % 2;
				gotoxy(2 * MAPY + 4, 5);
				cout<<"�@�@   �w�w�w�w ";
				Sleep(50);
				gotoxy(2 * MAPY + 4, 5);
				if(author_display == 0)
				{
					cout<<"�@�@   By pi314 ";
				}
				else
				{
					cout<<"�@�@   By ������";
				}
			}
			else if(a == 27)
			{
				mode = (mode + 1) % 2;
				gotoxy(2 * MAPY + 8, 26);
				cout<<"�w�w�w�w�w";
				Sleep(50);
				gotoxy(2 * MAPY + 8, 26);
				if(mode == 1)
				{
					cout<<" �۰ʴM�| ";
				}
				else
				{
					cout<<" ���q�Ҧ� ";
				}
			}
			else if(a == '+' || (a == 224 && b == 73))
			{
				speed = speed < 19 ? speed + 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"�w";
				Sleep(50);
				printf("\b\b%2d", speed + 1);
			}
			else if(a == '-' || (a == 224 && b == 81))
			{
				speed = speed > lowest_speed ? speed - 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"�w";
				Sleep(50);
				printf("\b\b%2d", speed + 1);
			}
		}
	}while(a != ' ');
	gotoxy(0, 30);
	cout<<endl;
	cout<<"�z�w�w11/21 �w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x Snake_III_0.exe  �g�Y�D 3.0  �x\n";
	cout<<"�|�s�w�w�w�w�w�w�w�w�w�w�w�w�w�w�r�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"  �xhttp://www.cs.nctu.edu.tw/~cychih/Snake_III_0.exe �x\n";
	cout<<"  �|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�S�O�P��: �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x2010/11/22 ex860��X���U��i�Ϥ�V�|�������`��bug.  �x\n";
	cout<<"�x�� �קאּSnake_III_1.exe                            �x\n";
	cout<<"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�S�O�P�¡G�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x�b�t��W�ĥ�CMturtle�b2008�~���@�~���l�{�D��. �x\n";
	cout<<"�u - - - - - - - - - - - - - - - - - - - - - - -�t\n";
	cout<<"�x11/26�[�j�w���i�J���Ѫ��P�_��O�C             �x\n";
	cout<<"�x11/29�L�T�[�j���X���Ѫ���O�C                 �x\n";
	cout<<"�x11/30�[�W�p���t�ΡC                           �x\n";
	cout<<"�x�� �קאּSnake_III_2.exe                      �x\n";
	cout<<"�|�s�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�r�w�w�{\n";
	cout<<"�@�xhttp://www.cs.nctu.edu.tw/~cychih/Snake_III_2.exe �x\n";
	cout<<"�@�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�z�w�w2011/01/15�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x�q01/14�U�ȶ}�l��class���g�A���b�]�ɪ��a���������C�x\n";
	cout<<"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n";
	cout<<"�z�w�w2011/01/17�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x����AI�t��k���֤ߡC�[�W�����Ҧ��ɪ��p�S�ġC�x\n";
	cout<<"�x�� Snake_IV_1.exe                           �x\n";
	cout<<"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�z�w�w2011/01/19�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x�[�W��h�p�S��(���ê���21�q�t��)�C�x\n";
	cout<<"�x�� Snake_IV_2.exe�@�@�@�@ �@�@�@�@�x\n";
	cout<<"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�z�w�w2011/01/21�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x�j��AI�G�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�x\n"; 
	cout<<"�x�@�@���J�����A�h�������s�N���s���A�H�F����r�j�઺�ĪG�x\n"; 
	cout<<"�x    ���Y���s�|���h���V�j��C(�U�@����V���K����)�@�@�@�x\n";
	cout<<"�x�� Snake_IV_3.exe �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�x\n";
	cout<<"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�z�w�w2011/01/24�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x�[�W�৹��¶����a�Ϫ�AI�A";
	printf("%c%c%c%c",-90,-88,-91,92); 
	cout<<"�o�ʪ��ܯ����a��";
	cout<<(char)-69<<(char)92;
	cout<<"���C�x\n";
	cout<<"�x�� Snake_IV_4.exe �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�x\n";
	cout<<"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"�z2011/01/26�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n";
	cout<<"�x�[�W����ɶ}��"<<(char)-91<<(char)92<<(char)-81<<(char)-32;
	cout<<"���A�]�ק�AI�A���s"<<(char)-91<<(char)92<<(char)-81<<(char)-32<<"�C�x\n";
	cout<<"�x�@�ק�u�@��x�A�ױ��w�g���Ī����}�C�@�@�@�x\n";
	cout<<"�|�s�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�r�w�w�w�{\n";
	cout<<"�@�xhttp://www.cs.nctu.edu.tw/~cychih/Snake_IV_5.exe�x\n";
	cout<<"�@�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n";
	cout<<"\n\n\n";
	cout<<"�Ы����N�䵲���C�� >>\n\n";
	pause();
	return 0;
}
