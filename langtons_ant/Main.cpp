#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#define MAX 500
#define S 16.f
using namespace std;
using namespace sf;

int WIDTH, HEIGHT, SPEED; // marimea ferestrei + fps
int nAnts, c_box[MAX][MAX]; // nr de furnici, vectorul de casete (pt afisare)
int t[MAX][MAX]; // teren folosit in acest frame
enum dir { UP = 0, RIGHT, DOWN, LEFT }; // pt directiile de deplasare ale furnicilor
struct ant
{
	int i, j;
	int d;
} c_ants[MAX]; // structura de furnici (pt coordonatele fiecarei furnici si directia lor)
RectangleShape box(Vector2f(S, S)); // caseta

void setup()
{
	cout << "W: "; cin >> WIDTH;
	cout << "H: "; cin >> HEIGHT;
	cout << "S: "; cin >> SPEED;
	cout << "N: "; cin >> nAnts;
}

void initGrid()
{
	for (int i = 1; i <= HEIGHT; i++)
		for (int j = 1; j <= WIDTH; j++) c_box[i][j] = 0; // reinitializarea casetelor pt afisare
	int aux = (nAnts + 1), i, j, _len = 0;
	srand(time(NULL));
	while (aux--)
	{
		i = rand() % HEIGHT + 1; j = rand() % WIDTH + 1; // coordonatele furnicilor; aleatoriu
		c_ants[_len++].i = i; c_ants[_len].j = j; c_ants[_len].d = UP;
	}
}

void drawBoxes(RenderWindow &window)
{
	for(int i=1;i<=HEIGHT;i++)
		for(int j=1;j<=WIDTH;j++)
			if (c_box[i][j] == 1)
			{
				box.setFillColor(Color::Black);
				box.setPosition((j - 1)*S, (i - 1)*S);
				window.draw(box);
			}
}

bool inside(int i, int j)
{
	return (i > 0 && i <= HEIGHT && j > 0 && j <= WIDTH); // inauntrul matricei
}

void updateAnts()
{
	int i_ant, j_ant;
	for (int i = 1; i <= nAnts; i++)
	{
		// actualizam caseta (devine alba sau colorata)
		i_ant = c_ants[i].i; j_ant = c_ants[i].j; // coordonatele furnicii actuale
		if (inside(i_ant, j_ant))
		{
			if (c_box[i_ant][j_ant] == 0) t[i_ant][j_ant] = 1; // daca caseta pe care se afla furnica acum e alba -> va deveni colorata
			else t[i_ant][j_ant] = 0; // altfel devine alba

			switch (c_ants[i].d) // actualizam pozitia furnicii in functie de directie
			{
			case UP:
				if (c_box[i_ant][j_ant]) c_ants[i].d = LEFT; // caseta pe care a ajuns este colorata -> furnica face o intoarcere spre stanga
				else c_ants[i].d = RIGHT; // altfel -> spre dreapta
				if(c_ants[i].d==RIGHT) c_ants[i].j++;
				else c_ants[i].j--;
				break;
			case RIGHT:
				if (c_box[i_ant][j_ant]) c_ants[i].d = UP; // > intors la 90grade in stanga devine ^
				else c_ants[i].d = DOWN; // > intors la 90grade in dreapta devine v
				if(c_ants[i].d==DOWN) c_ants[i].i++;
				else c_ants[i].i--;
				break;
			case DOWN:
				if (c_box[i_ant][j_ant]) c_ants[i].d = RIGHT; // v intors la 90grade in stanga devine >
				else c_ants[i].d = LEFT; // v intors la 90grade in dreapta devine <
				if(c_ants[i].d==RIGHT) c_ants[i].j++;
				else c_ants[i].j--;
				break;
			case LEFT:
				if (c_box[i_ant][j_ant]) c_ants[i].d = DOWN; // < intors la 90grade in stanga devine v
				else c_ants[i].d = UP; // < intors la 90grade in dreapta devine ^
				if(c_ants[i].d==DOWN) c_ants[i].i++;
				else c_ants[i].i--;
				break;
			}
		}
	}
}

void newTerrain()
{
    // actualizeaza teren
	for(int i=1;i<=HEIGHT;i++)
        for(int j=1;j<=WIDTH;j++) c_box[i][j]=t[i][j];
}

void drawAnts(RenderWindow &window)
{
	int i_ant, j_ant;
	for(int i=1;i<=nAnts;i++)
	{
		i_ant = c_ants[i].i; j_ant = c_ants[i].j;
		if (inside(i_ant, j_ant))
		{
			box.setPosition((j_ant - 1)*S, (i_ant - 1)*S);
			box.setFillColor(Color::Red);
			window.draw(box);
		}
	}
}

int main()
{
	// initialiizari
	setup();
	initGrid();
	box.setFillColor(Color::Black);

	// fereastra
	RenderWindow window(VideoMode(S*WIDTH, S*HEIGHT), "Langton's Ant");
	window.setFramerateLimit(SPEED);
	while(window.isOpen())
	{
		// evenimente
		Event e;
		while (window.pollEvent(e))
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::R) initGrid(); // pt restart

		// logica
		window.clear(Color::White);
		if(Keyboard::isKeyPressed(Keyboard::Space)) 
		{
			newTerrain();
			updateAnts();
		}

		// afisare
		drawBoxes(window);
		drawAnts(window);

		window.display();
	}
	return 0;
}
