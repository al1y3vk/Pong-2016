#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

enum Direction { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };

enum COLORS {
	BLACK = 0, BLUE = 1, GREEN = 2, AQUA = 3, RED = 4, PURPLE = 5, YELLOW = 6, WHITE = 7, GRAY = 8, LIGHT_BLUE = 9,
	LIGHT_GREEN = 10, LIGHT_AQUA = 11, LIGHT_RED = 12, LIGHT_PURPLE = 13, LIGHT_YELLOW = 14, BRIGHT_WHITE = 15
};

void my_set_cursor(int x, int y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {};
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(h, coord);
}

void my_set_color(int fg, int bg) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, fg | (bg << 4));
}


void Blink(int x1, int y1, int x2, int y2) {
	my_set_cursor(x1, y1);
	cout << " "; //ball
	my_set_cursor(x2, y2);
	cout << "O";
}

void Blink2(int x1, int y1, int x2, int y2) {
	my_set_cursor(x1, y1);
	cout << " ";
	my_set_cursor(x1, y1 + 1);
	cout << " ";
	my_set_cursor(x1, y1 + 2);
	cout << " ";
	my_set_cursor(x1, y1 + 3);
	cout << " ";

	my_set_cursor(x2, y2);
	cout << char(219);
	my_set_cursor(x2, y2 + 1);
	cout << char(219);
	my_set_cursor(x2, y2 + 2);
	cout << char(219);
	my_set_cursor(x2, y2 + 3);
	cout << char(219);
}

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

struct Ball {
	int x, y;
	int ox, oy;
	int oox, ooy;
	Direction dir;

	Ball(int px, int py) {
		ox = px; oox = px;
		oy = py; ooy = py;
		x = px; y = py;
		dir = STOP;
	}

	void Reset() {
		x = ox; y = oy;
		dir = STOP;
	}

	void ChangeDir(Direction d) {
		dir = d;
	}

	void RandomDir() {
		dir = (Direction)((rand() % 6) + 1);
	}

	inline int gx() { return x; }
	inline int gy() { return y; }
	inline Direction GetDir() { return dir; }

	void Move() {
		oox = x;
		ooy = y;
		switch (dir) {
		case LEFT:
			x--, y;
			break;
		case RIGHT:
			x++, y;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
};

struct Paddle {
	int x, y;
	int ox, oy;
	int oox, ooy;

	Paddle() {
		x = 0, y = 0;
	}

	Paddle(int px, int py) {
		ox = px; oox = px;
		oy = py; ooy = py;
		x = px; y = py;
	}

	inline void Reset() { x = ox; y = oy; }
	inline int gx() { return x; }
	inline int gy() { return y; }

	inline void MU() {
		oox = x;
		ooy = y;
		y--, x;
	}

	inline void MD() {
		oox = x;
		ooy = y;
		y++, x;
	}

};

struct GameManager {
	int x, y;
	int width, height;
	int score1, ascore, score2;
	int speed;
	char u1, d1, u2, d2;
	bool quit;

	Ball *ball;
	Paddle *p1;
	Paddle *p2;
	Paddle *ai;

	GameManager(int w, int h) {
		srand(time(0));
		quit = false;
		u1 = 'w', u2 = char(72);
		d1 = 's', d2 = char(80);
		score1 = 0, ascore = 0, score2 = 0;
		width = w; height = h;
		ball = new Ball(47, 14);
		p1 = new Paddle(16, h / 2 - 3);
		p2 = new Paddle(w - 2, h / 2 - 3);
		ai = new Paddle(w - 2, h / 2 - 3);
	}

	~GameManager() {
		delete ball, p1, ai, p2;
	}

	void ScoreUp(Paddle *p) {
		if (p == p1)
			score1++;
		else if (p == ai)
			ascore++;
		else if (p == p2)
			score2++;

		ball->Reset();
		p1->Reset();
		ai->Reset();
		p2->Reset();
	}

	void Draw() {
		my_set_cursor(15, 10);
		for (int i = 0; i < 67; ++i) {
			my_set_color(GREEN, BLACK);
			if (i == 0) {
				cout << char(201);
			}
			else if (i == 66) {
				cout << char(187);
			}
			else {
				cout << char(205);
			}
		}
		cout << endl;

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int bx = ball->gx();
				int by = ball->gy();

				int p1x = p1->gx();
				int aix = ai->gx();

				int p1y = p1->gy();
				int aiy = ai->gy();

				my_set_color(RED, BLACK);
				if (j == 15)
					cout << char(186);

				if (bx == j && by == i) {
					my_set_color(RED, BLACK);
					cout << "O"; //ball
				}

				else if (p1x == j && p1y == i)
					cout << char(219); //player1
				else if (p1x == j && p1y + 1 == i)
					cout << char(219); //player1
				else if (p1x == j && p1y + 2 == i)
					cout << char(219); //player1
				else if (p1x == j && p1y + 3 == i)
					cout << char(219); //player1

				else if (aix == j && aiy == i)
					cout << char(219); //ai
				else if (aix == j && aiy + 1 == i)
					cout << char(219); //ai
				else if (aix == j && aiy + 2 == i)
					cout << char(219); //ai
				else if (aix == j && aiy + 3 == i)
					cout << char(219); //ai

				else
					cout << " ";

				my_set_color(RED, BLACK);
				if (j == width - 1)
					cout << char(186);
			}
			cout << endl;
		}

		my_set_cursor(15, 41);
		for (int i = 0; i < 67; ++i) {
			my_set_color(GREEN, BLACK);
			if (i == 0) {
				cout << char(200);
			}
			else if (i == 66) {
				cout << char(188);
			}
			else {
				cout << char(205);
			}
		}
		cout << endl;

		my_set_cursor(15, 9);
		my_set_color(BLUE, BLACK);
		cout << "Player 1: " << score1 << endl;
		my_set_cursor(77, 9);
		my_set_color(BLUE, BLACK);
		cout << "Ai: " << ascore << endl;
	}

	void Draw2() {
		my_set_cursor(15, 10);
		for (int i = 0; i < 67; ++i) {
			my_set_color(GREEN, BLACK);
			if (i == 0) {
				cout << char(201);
			}
			else if (i == 66) {
				cout << char(187);
			}
			else {
				cout << char(205);
			}
		}
		cout << endl;

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int bx = ball->gx();
				int by = ball->gy();

				int p1x = p1->gx();
				int p2x = p2->gx();

				int p1y = p1->gy();
				int p2y = p2->gy();

				my_set_color(RED, BLACK);
				if (j == 15)
					cout << char(186);
				if (bx == j && by == i) {
					my_set_color(RED, BLACK);
					cout << "O"; //ball
				}

				else if (p1x == j && p1y == i)
					cout << char(219); //player1
				else if (p1x == j && p1y + 1 == i)
					cout << char(219); //player1
				else if (p1x == j && p1y + 2 == i)
					cout << char(219); //player1
				else if (p1x == j && p1y + 3 == i)
					cout << char(219); //player1

				else if (p2x == j && p2y == i)
					cout << char(219); //player2
				else if (p2x == j && p2y + 1 == i)
					cout << char(219); //player2
				else if (p2x == j && p2y + 2 == i)
					cout << char(219); //player2
				else if (p2x == j && p2y + 3 == i)
					cout << char(219); //player2

				else
					cout << " ";

				my_set_color(RED, BLACK);
				if (j == width - 1)
					cout << char(186);

			}
			cout << endl;
		}

		my_set_cursor(15, 41);
		for (int i = 0; i < 67; ++i) {
			my_set_color(GREEN, BLACK);
			if (i == 0) {
				cout << char(200);
			}
			else if (i == 66) {
				cout << char(188);
			}
			else {
				cout << char(205);
			}
		}
		cout << endl;

		my_set_cursor(15, 9);
		my_set_color(BLUE, BLACK);
		cout << "Player 1: " << score1 << endl;

		my_set_cursor(71, 9);
		my_set_color(BLUE, BLACK);
		cout << "Player 2: " << score2 << endl;
	}

	void Input() {
		ball->Move();

		int bx = ball->gx();
		int by = ball->gy();

		int p1x = p1->gx();
		int p2x = p2->gx();
		int aix = ai->gx();

		int p1y = p1->gy();
		int p2y = p2->gy();
		int aiy = ai->gy();

		//ai
		if (by > aiy)
			by = aiy;

		else if (by < aiy)
			by = aiy;

		if (_kbhit()) {
			char current = _getch();
			if (current == u1 && p1y > 0)
					p1->MU();

			if (current == u2 && p2y > 0)
					p2->MU();

			if (current == d1 && p1y + 4 < height)
					p1->MD();

			if (current == d2 && p2y + 4 < height)
					p2->MD();

			if (ball->GetDir() == STOP)
				ball->RandomDir();

			if (current == 'q')
				quit = true;
		}
	}

	void Logic() {
		int bx = ball->gx();
		int by = ball->gy();

		int p1x = p1->gx();
		int aix = ai->gx();
		int p2x = p2->gx();

		int p1y = p1->gy();
		int aiy = ai->gy();
		int p2y = p2->gy();

		// cheap ai
		if (by > aiy)
			ai->MD();

		else if (by < aiy)
			ai->MU();

		//left paddle
		for (int i = 0; i < 4; ++i)
			if (bx == p1x + 1)
				if (by == p1y + i)
					ball->ChangeDir((Direction)((rand() % 3) + 4));

		//right paddle
		for (int i = 0; i < 4; ++i)
			if (bx == p2x - 1)
				if (by == p2y + i)
					ball->ChangeDir((Direction)((rand() % 3) + 1));

		//ai paddle
		for (int i = 0; i < 4; ++i)
			if (bx == aix - 1)
				if (by == aiy + i)
					ball->ChangeDir((Direction)((rand() % 3) + 1));

		//bottom wall
		if (by == height - 1)
			ball->ChangeDir(ball->GetDir() == DOWNRIGHT ? UPRIGHT : UPLEFT);

		//top wall
		if (by == 0)
			ball->ChangeDir(ball->GetDir() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

		//right wall
		if (bx == width - 1) { score1++; ScoreUp(p1); }
			

		//left wall
		if (bx == 16) { ascore++; ScoreUp(ai); }
			

		//left wall
		if (bx == 16) { score2++; ScoreUp(p2); }
			
	}

	void Single() {
		Draw();
		while (!quit) {
			Input();
			Logic();
			Blink(ball->oox + 1, ball->ooy + 11, ball->x + 1, ball->y + 11);
			Blink2(p1->oox + 1, p1->ooy + 11, p1->x + 1, p1->y + 11);
			Blink2(ai->oox + 1, ai->ooy + 11, ai->x + 1, ai->y + 11);
			Sleep(75);
		}
	}

	void Multi() {
		Draw2();
		while (!quit) {
			Input();
			Logic();
			Blink(ball->oox + 1, ball->ooy + 11, ball->x + 1, ball->y + 11);
			Blink2(p1->oox + 1, p1->ooy + 11, p1->x + 1, p1->y + 11);
			Blink2(p2->oox + 1, p2->ooy + 11, p2->x + 1, p2->y + 11);
			Sleep(75);
		}
	}
};

void file(GameManager *g, char *name) {
	if (g == nullptr) {
		return;
	}

	FILE *f = NULL;
	fopen_s(&f, name, "wb");

	if (f != NULL) {
		int size = 20;
		fwrite(&size, sizeof(int), 1, f);
	}
}

void main() {
	ShowConsoleCursor(false);

	srand(time(0));
	GameManager c(80, 30);

	system("mode con cols=100 lines=50");

	int choice;
	bool resume = true;

	while (resume != false) {
		my_set_cursor(39, 19);
		my_set_color(RED, BLACK);
		cout << " >> Single Player << " << endl;

		my_set_cursor(41, 20);
		my_set_color(BLUE, BLACK);
		cout << " >> Two Player << " << endl;

		my_set_cursor(44, 21);
		my_set_color(RED, BLACK);
		cout << " >> Help << " << endl;

		my_set_cursor(44, 22);
		my_set_color(BLUE, BLACK);
		cout << " >> Exit << " << endl;

		my_set_cursor(29, 23);
		my_set_color(RED, BLACK);
		cout << " >> Enter your choice and press return << " << endl;

		my_set_cursor(48, 24);
		cin >> choice;

		system("cls");

		switch (choice) {
		case 1:
			my_set_cursor(43, 9);
			my_set_color(GREEN, BLACK);
			cout << "Game start!" << endl;
			c.Single();
			break;
		case 2:
			my_set_cursor(43, 9);
			my_set_color(GREEN, BLACK);
			cout << "Game start!" << endl;
			c.Multi();
			break;
		case 3:
			cout << "1st Player controls the left paddle up and down width: W, S" << endl;
			cout << "2nd Player controls the right paddle up and down width: the arrows" << endl;
			cout << "Enjoy!!" << endl;
			cout << "Choose Again: " << endl;
			cin >> choice;
			break;
		case 4:
			cout << "See You!" << endl;
			resume = false;
			break;
		default:
			cout << "Not a Valid Choice" << endl;
			cout << "Choose again:" << endl;
			cin >> choice;
			break;
		}
	}
}