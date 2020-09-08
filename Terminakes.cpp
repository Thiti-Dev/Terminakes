//# WRITTEN BY THITI MAHAWANNAKIT
//# VERSION 1.00

#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

// Static variable
const int width = 20, height = 20;
//---------------

// Enumeration declaration
enum headingDirection {
	STOP,
	UP,
	DOWN,
	LEFT,
	RIGHT
};
enum class typeOfWallCreation {
	TOP,
	BOTTOM
};
//------------------------

// Entity
struct Snake {
	double xPos, yPos;
	int tailX[100], tailY[100], numberOfTails=0;
	headingDirection hDir;
	Snake() {
		xPos = width / 2;
		yPos = height / 2;
		hDir = STOP;
	}
	void READER_Movement()
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'w':
				hDir = UP;
				break;
			case 'a':
				hDir = LEFT;
				break;
			case 's':
				hDir = DOWN;
				break;
			case 'd':
				hDir = RIGHT;
				break;
			}
		}
	}
};
struct Food {
	int xPos, yPos;
	void RandomPosition() {
		xPos = rand() % width; // random x position of the food
		yPos = rand() % height;// random y position of the food
	}
};
//--------------------------

// functions

static void ClearTerminalScreen() {
	system("cls");
}
// ---------------------

struct GAME_UTILS {
	int score = 0;
	bool isGameOver;
	Snake ENTITY_snake;
	Food ENTITY_food;
	GAME_UTILS() {
		isGameOver = false;
		debug_enabled = false;
	}
	void Setup() {
		isGameOver = false;
		ENTITY_snake.hDir = STOP;
		// Setting position of the snake to be centered of the generated box
		ENTITY_snake.xPos = width / 2;
		ENTITY_snake.yPos = height / 2;
		ENTITY_food.RandomPosition();
		score = 0;
		//------------------------------------------------------------------
	}
	void Draw() {
		ClearTerminalScreen();
		WALL_Draw(typeOfWallCreation::TOP);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (j == 0) cout << "#"; // printing the left wall
				if (i == ENTITY_snake.yPos && j == ENTITY_snake.xPos) cout << "O"; //printing the snake itself
				else if (i == ENTITY_food.yPos && j == ENTITY_food.xPos) cout << "+"; // printing the food itself
				else {
					bool isPrinted = false;
					for (int k = 0; k < ENTITY_snake.numberOfTails; k++)
					{
						if (ENTITY_snake.tailX[k] == j && ENTITY_snake.tailY[k] == i)
						{
							cout << "o";
							isPrinted = true;
						}
					}
					if (!isPrinted)
						cout << " "; // showing empty space if nothings met the condition
				}
				if (j == width - 1) cout << "#"; // printing the right wall
			}
			cout << endl;
		}

		WALL_Draw(typeOfWallCreation::BOTTOM);
		cout << "Score: " << score << endl; // showing the score at the bottom of the box
	}
	void HANDLER_Movement() {
		ENTITY_snake.READER_Movement();
	}
	void TICK_Event() {
		int prevX = ENTITY_snake.tailX[0];
		int prevY = ENTITY_snake.tailY[0];
		int prev2X, prev2Y;
		ENTITY_snake.tailX[0] = ENTITY_snake.xPos;
		ENTITY_snake.tailY[0] = ENTITY_snake.yPos;
		for (int i = 1; i < ENTITY_snake.numberOfTails; i++)
		{
			prev2X = ENTITY_snake.tailX[i];
			prev2Y = ENTITY_snake.tailY[i];
			ENTITY_snake.tailX[i] = prevX;
			ENTITY_snake.tailY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}
		switch (ENTITY_snake.hDir)
		{
			case LEFT:
				ENTITY_snake.xPos--;
				break;
			case RIGHT:
				ENTITY_snake.xPos++;
				break;
			case UP:
				ENTITY_snake.yPos--;
				break;
			case DOWN:
				ENTITY_snake.yPos++;
				break;
			default:
				break;
		}
		// Portal edge
		if (ENTITY_snake.xPos >= width) ENTITY_snake.xPos = 0; else if (ENTITY_snake.xPos < 0) ENTITY_snake.xPos = width - 1;
		if (ENTITY_snake.yPos >= height) ENTITY_snake.yPos = 0; else if (ENTITY_snake.yPos < 0) ENTITY_snake.yPos = height - 1;
		//-----------
		// Check if hitting it owns tails
		for (int i = 0; i < ENTITY_snake.numberOfTails; i++)
			if (ENTITY_snake.tailX[i] == ENTITY_snake.xPos && ENTITY_snake.tailY[i] == ENTITY_snake.yPos)
				isGameOver = true;
		// --------------------------------
		if (debug_enabled) {
			cout << "DEBUG MODE: ON [INSPECT X,Y AXIS OF THE ENTITYS]" << endl;
			cout << "snake_x: " << ENTITY_snake.xPos << ", snake_y: " << ENTITY_snake.yPos << endl;
			cout << "food_x: " << ENTITY_food.xPos << ", food_y: " << ENTITY_food.yPos << endl;
		}
		if (ENTITY_snake.xPos == ENTITY_food.xPos && ENTITY_snake.yPos == ENTITY_food.yPos)
		{
			score += 10;
			ENTITY_food.RandomPosition();
			ENTITY_snake.numberOfTails++;
		}
	}
	void SetDebugMode(bool isToggle) {
		debug_enabled = isToggle;
	}
private:
	bool debug_enabled;
	// In order to draw a special thing on separate top/bottom wall [Could be implement later if needed =>  but stay the same for now]
	void WALL_Draw(typeOfWallCreation type) {
		switch (type)
		{
			case typeOfWallCreation::TOP:
				for (int i = 0; i < width + 2; i++)
					cout << "#";
					cout << endl;
				break;
			case typeOfWallCreation::BOTTOM:
				for (int i = 0; i < width + 2; i++)
					cout << "#";
					cout << endl;
				break;
			default:
				break;
			}
	}
};

int main() {
	//Creating game controller
	GAME_UTILS controller;
	controller.SetDebugMode(true);
	//------------------------
	controller.Setup();
	while (!controller.isGameOver) {
		controller.Draw();
		controller.HANDLER_Movement();
		controller.TICK_Event();
	}
	return 0;
}