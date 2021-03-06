/*
	EN/PL	#####################################################################################
			|To create this maze i've used an "Depth-first search" algorithm.					|
			|Do stworzenia labiryntu użyłem algorytmu "Depth-first search"						|
			|Link: https://en.wikipedia.org/wiki/Maze_generation_algorithm#Depth-first_search	|
			#####################################################################################
	V1 NOTE
		What's new:
			* bieda mode (poor mode) - faster drawing
			* cool border

			printf("\n  %c", 218); w prawo
			printf("%c", 196); poziom
			printf("%c  ", 191); w lewo
			printf("%c", 179); w dol
			DOWN
			printf("%c", 192);prawo
			printf("%c", 217);lewo
*/

#define _WIN32_WINNT 0x0501 //do zmiany rozmiaru
#include <iostream> 
#include <windows.h> //do zmiany kolorkow
#include <conio.h> //do _getch()
#include <string> 
#include <cstdlib>
#include <time.h> // do losowych
using namespace std;


struct Cell
{
	bool visited,closed;
	char mark;

}; 
bool bieda = true;
void textColor(int x)
{
	if (!bieda)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
	}
}
void gotoxy(int x, int y)
{
	HANDLE hCon;
	COORD dwPos;

	dwPos.X = x;
	dwPos.Y = y;

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, dwPos);
}
void clearScreen() //czyszczenie ekranu
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void hidecursor() //fix for bliking
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int width, height;
Cell map[60][60]; //creates whole map
int drawX, drawY;
int startX, startY;
int playerX, playerY;
int endX, endY;
int drawDirection;
int spawnDirection=0;
int button;
string confirm;
int start_time;
int now_time;

bool checkRight(int x, int y)
{
	if (x + 1 <= width && // jesli nie dotyka prawej krwedzi
		((map[x + 1][y].mark != 'x') && (map[x + 1][y].mark != 's')) && //jesli nastepne pole to gracz lub start 
		((map[x + 2][y].mark == 'd') || (map[x + 1][y].mark == 'd' )) && //jesli nastepna komorka jest dostepna
		((map[x + 1][y - 1].mark != 'x') && (map[x + 1][y + 1].mark != 'x')) && //jasli nastepna komorka nie sasiaduje z korytarzem
		((map[x + 2][y].visited == false ) || (map[x + 1][y].visited == false)) //jasli nie zostala juz odwiedzona
		) //sprawdza czy mozna isc w prawo
	{
		return true;
	}
	else
	{
		return false;
	}
 }
bool checkLeft(int x, int y)
{
	if (x - 1 >= 1 && 
		((map[x - 1][y].mark != 'x' ) && (map[x - 1][y].mark != 's')) && 
		((map[x - 2][y].mark == 'd') || (map[x - 1][y].mark == 'd')) &&
		((map[x - 1][y - 1].mark != 'x') && (map[x - 1][y + 1].mark != 'x')) &&
		((map[x - 2][y].visited == false) || (map[x - 1][y].visited == false))
		) //sprawdza czy mozna isc w lewo
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool checkUp(int x, int y)
{
	if ((y - 1 >= 1 )&& 
		((map[x][y - 1].mark != 'x') && (map[x][y - 1].mark != 's')) && 
		((map[x][y - 2].mark == 'd') || (map[x][y - 1].mark == 'd')) &&
		((map[x + 1][y - 1].mark != 'x') && (map[x - 1][y - 1].mark != 'x')) &&
		((map[x][y - 2].visited == false) || (map[x][y - 1].visited == false))
		) //sprawdza czy mozna isc w gore
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool checkDown(int x, int y)
{
	if ((y + 1 <= width) && 
		((map[x][y + 1].mark != 'x') && (map[x][y + 1].mark != 's')) &&
		((map[x][y + 2].mark == 'd')|| (map[x][y + 1].mark =='d')) &&
		((map[x + 1][y + 1].mark != 'x') && (map[x - 1][y + 1].mark != 'x')) &&
		((map[x][y + 2].visited == false) || (map[x][y + 1].visited == false)) 
		) //sprawdza czy mozna isc w dol
	{
		return true;
	}
	else
	{
		return false;
	}
}
void fix()
{

	if (map[drawX + 1][drawY].mark == 'e' && map[drawX + 1][drawY].closed != true)
	{
		drawX++;
		map[drawX][drawY].mark = 'x';
	}else if (map[drawX - 1][drawY].mark == 'e' && map[drawX - 1][drawY].closed != true)
	{
		drawX--;
		map[drawX][drawY].mark = 'x';
	}
	else if (map[drawX][drawY - 1].mark == 'e' && map[drawX][drawY - 1].closed != true)
	{
		drawY--;
		map[drawX][drawY].mark = 'x';
	}
	else if (map[drawX][drawY + 1].mark == 'e' && map[drawX][drawY + 1].closed != true)
	{
		drawY++;
		map[drawX][drawY].mark = 'x';
	}

}
void line(int up_or_down)
{   
	for (int i = 0; i <= width+2; i++)
	{
		if (up_or_down==1)
		{ 
			if (i == 0) printf("%c", 218); 
			else if (i == width + 2)printf("%c", 191);
			else
			{
				printf("%c", 196); // prosta linia
			}
		}
		else
		{
			if (i == 0) printf("%c", 192);
			else if (i == width + 2)printf("%c", 217);
			else
			{
				printf("%c", 196);
			}
		}
		
	}
}
void menu()
{
	cout << "Witaj" << endl;
	cout << "Podaj szerokosc od 20 do 60 Liczba musi byc parzysta!!(zalecane 50)" << endl;
	cin >> width;
	if (!(width % 2 == 0) || width < 20 || width > 60)
	{
		cout << "Podano zla szerokosc, ustawiam ja na 40" << endl;
		width = 40;
	}
	cout << "Podaj wysokosc od 10 do 30 Liczba musi byc parzysta!! (zalecane 24)" << endl;
	cin >> height;
	if (!(height % 2 == 0) || height < 10 || height > 30)
	{
		cout << "Podano zla wysokosc, ustawiam ja na 24" << endl;
		height=24;
	}
	Sleep(2000);

}

int main()
{
	srand(time(NULL)); //do pelnej losowosci
	SetConsoleTitle(TEXT("Maze")); //zmiana nazwy
	
	system("mode con:cols=80 lines=25");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	menu();
	system("mode con:cols=80 lines=35");

	COORD a;
	a.X = 0;
	a.Y = 0;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = height+5;  //wiersze
	rect.Right = width+9; //kolumny


	SetConsoleScreenBufferSize(handle, a);
	SetConsoleWindowInfo(handle, 1, &rect);

	start_time = NULL;
	do //rysowanie pozycji Y startu
	{
		startY = rand() % height;
	} while (startY % 2 != 1);
	
	do //rysowanie pozycji X startu
	{
		if (startY == 2 || startY == width-1)
		{
			startX = rand() % width;
			drawX = startX + 1;
		}
		else
		{
			if (rand() % 10 >= 5)
			{
				startX = 1;
				drawX = 2;
			}
			else
			{
				startX = width-1;
				drawX = startX-1;
			}
		}
	} while (startX % 2 != 1);
	
	drawY = startY;
	playerX = drawX;
	playerY = drawY;
	system("cls");
	hidecursor();

	bool work = true; //zmienna sterujaca petla
	bool done = false; //czy trzeba dalej rysowac labirynt

	map[drawX][drawY].mark = 'x'; //pozycja kontrolera budowy 
	map[drawX][drawY].mark = 's'; //pozycja startu
	map[drawX][drawY].visited = true;
	

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			if (i % 2 == 1 && j % 2 == 1)
			{
				map[j][i].mark = 'd';
			}
		}
	}
	map[startX][startY].mark = 's';
	while (work) //rysowanie
	{
		gotoxy(0, 0);
		if (bieda) cout << "rysowanie,prosze czekac";
	if (!bieda)
	{
		line(1);//upper line

		cout << endl;
		for (int i = 0; i <= height; i++) //left and right walls 
		{
			printf("%c", 179);
			for (int j = 0; j <= width; j++)
			{
				if (map[j][i].mark == 'x') //darwing pencil's positon
				{
					textColor(255);
					cout << " ";
					textColor(7);
				}
				else if (map[j][i].mark == 'd') //darwing patch's positon
				{
					textColor(136);
					cout << " ";
					textColor(7);
				}
				else if (map[j][i].mark == 'e') //darwing patch's positon
				{
					cout << " ";
				}
				else if (map[j][i].mark == 'p') //darwing players positon
				{
					textColor(238);
					cout << "P";
					textColor(7);
				}
				else if (map[j][i].mark == 's') //darwing start's positon
				{
					textColor(10);
					cout << "S";
					textColor(7);
				}
				else if (map[j][i].mark == 'y') //darwing finish positon
				{
					textColor(12);
					cout << "E";
					textColor(7);
				}
				else
				{
					textColor(136);
					cout << "#";
					textColor(7);
				}

			}
			printf("%c \n", 179);
		}
		line(0);//lower line
	}
		if (!done) //jesli labirynt został narysowany
		{
			int canMove[4]; // w celu wylosowania 

			if (checkRight(drawX, drawY))
			{
				canMove[0] = 1;
			}
			else
			{
				canMove[0] = 0;
			}
			if (checkLeft(drawX, drawY))
			{
				canMove[1] = 2;
			}
			else
			{
				canMove[1] = 0;
			}
			if (checkUp(drawX, drawY))
			{
				canMove[2] = 3;
			}
			else
			{
				canMove[2] = 0;
			}
			if (checkDown(drawX, drawY))
			{
				canMove[3] = 4;
			}
			else
			{
				canMove[3] = 0;
			}

			do
			{
				int x;
				x = rand() % 4;  //losuje, aż do wylosowania poprawnego kierunku
				drawDirection = canMove[x];
				if (canMove[0] + canMove[1] + canMove[2] + canMove[3] == 0) //jesli utknie
				{
					map[drawX][drawY].mark = 'e';
					map[drawX][drawY].closed = true;
					fix();
					break;
				}
			} while (drawDirection == 0);

			switch (drawDirection)
			{
			case 1:
			{
				map[drawX][drawY].visited = true;
				map[drawX][drawY].mark = 'e';
				drawX++;
				map[drawX][drawY].mark = 'x';
				break;
			}
			case 2:
			{
				map[drawX][drawY].visited = true;
				map[drawX][drawY].mark = 'e';
				drawX--;
				map[drawX][drawY].mark = 'x';
				break;
			}
			case 3:
			{
				map[drawX][drawY].visited = true;
				map[drawX][drawY].mark = 'e';
				drawY--;
				map[drawX][drawY].mark = 'x';
				break;
			}
			case 4:
			{
				map[drawX][drawY].visited = true;
				map[drawX][drawY].mark = 'e';
				drawY++;
				map[drawX][drawY].mark = 'x';
				break;
			}
			default:
				break;
			}
		}

		if ((drawX + 1 == startX || drawX - 1 == startX) && (drawY == startY)) //jesli plansza zostanie narysowana
		{
			int canSpawn[4];

			cout<< endl << "Rysowanie zakonczone ";
			done = true;
			bieda = false;
			
			
			//sprawdza czy miejsce jest dostepne

			if (map[2][1].mark=='e')  //prawy gorny
				{
					canSpawn[0] = 1;
				}
			else
			{
				canSpawn[0] = 0;
			}
			if (map[width-1][1].mark=='e') //lewy gorny
				{					
					canSpawn[1] = 2;
				}
			else
			{
				canSpawn[1] = 0;
			}
			if (map[2][height-1].mark=='e') //parwy dolny
				{					
					canSpawn[2] = 3;
				}
			else
			{
				canSpawn[2] = 0;
			}
			if (map[width-1][height-1].mark=='e') //lewy donly
				{				
					canSpawn[3] = 4;
				}
			else
			{
				canSpawn[3] = 0;
			}
			if (spawnDirection == 0)
			{
				do
				{
					if (startX == 1) //aby losowało koniec po drógiej stronie mapy
					{
						canSpawn[0] = 0;
						canSpawn[2] = 0;
					}
					else if (startX == width - 1)
					{
						canSpawn[1] = 0;
						canSpawn[3] = 0;
					}
					int x;
					x = rand() % 4;  //losuje, aż do wylosowania dostepnego miejsca
					spawnDirection = canSpawn[x];
				

				} while (spawnDirection == 0);
			}

			switch (spawnDirection)
			{
			case 1:
			{
				map[1][1].mark = 'y';
				break;
			}
			case 2:
			{
				map[width - 1][1].mark = 'y';
				break;
			}
			case 3:
			{
				map[1][height - 1].mark = 'y';
				break;
			}
			case 4:
			{
				map[width - 1][height - 1].mark = 'y';
				break;
			}
			default:
				break;
			}
			if (start_time == NULL)
			{
				start_time = time(0);
			}
		}
		if (done)
		{	
			map[playerX][playerY].mark = 'p';
			if (_kbhit())
			{
				
				button = _getch();
				switch (button)
				{
				case 97: //moving left 
				{
					if (map[playerX-1][playerY].mark == 'e')
					{
						map[playerX][playerY].mark = 'e';
						playerX--;
						
					}
					if (map[playerX - 1][playerY].mark == 'y')
					{
						work = false;
					}
				}
				break;
				case 100: //moving right
				{
					if (map[playerX + 1][playerY].mark == 'e')
					{
						map[playerX][playerY].mark = 'e';
						
						playerX++;
						
					}
					if (map[playerX + 1][playerY].mark == 'y')
					{
						work = false;
					}
				}
				break;
				case 119: // moving up
				{
					if (map[playerX][playerY-1].mark == 'e')
					{
						map[playerX][playerY].mark = 'e';

						playerY--;

					}
					if (map[playerX][playerY-1].mark == 'y')
					{
						work = false;
					}
					
				}
				break;
				case 115: // moving down
				{
					if (map[playerX][playerY + 1].mark == 'e')
					{
						map[playerX][playerY].mark = 'e';

						playerY++;

					}
					if (map[playerX][playerY + 1].mark == 'y')
					{
						work = false;
					}
				}
				break;
				}
			}
		}
		if (done) 
		{ 
		Sleep(1000 / 30); //1 sec / ilość fps gdzy plansza została narysowana
		now_time = time(0) - start_time;
		cout <<"Czas:"<< now_time << endl;
		} 
		
		//_getch(); //w celach diagnostycznych 

	}
	textColor(10);
	cout <<endl<< "Wygranko" << endl;
	textColor(7);
	cout << "chcesz zagrac ponownie? y/n" << endl;
	cin >> confirm; //work in progress
	if (confirm == "y")
	{
		system("cls");
		spawnDirection = 0;
		SetConsoleScreenBufferSize(handle, a);
		SetConsoleWindowInfo(handle, 1, &rect);

		for (int i = 0; i <= height; i++) 
		{
		
			for (int j = 0; j <= width; j++)
			{
				map[j][i].mark = 'n';
				map[j][i].closed = false;
				map[j][i].visited = false;
				bieda = true;
	
			}
		}
		system("mode con:cols=80 lines=25");
		main();
	}
	system("pause");
	return 0;
}