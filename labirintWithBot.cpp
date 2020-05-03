#include "stdafx.h"
#include <iostream>
#include <stack>
#include <ctime>
#include "glut.h"

///////////////////////////////////////////////////////////
int blockSize = 4;						//размер в визуализации лабиринта
const int Height = 100					//высота
* 2 + 1;
const int Width = 100						//ширина
* 2 + 1;
///////////////////////////////////////////////////////////

int Maze[Height][Width];						//массив дл€ хранени€ готового лабиринта

int VisitCell[Height / 2 + 2][Width / 2 + 2];	//массив дл€ хранени€ посейщЄнных €чеек

int MazeBot[Height][Width];

int StackPreviousElement = 1;					//элемент, следующий за верхним в стеке, позици€ по горизонтали
int RandomDirection = 0;						//переменна€ рандома
int StackFirstElement = 0;						//переменна€, временно хран€ща€ верхний элемент стека

int AppCell = 0;								//€чейка выше
int RightCell = 0;								//€чейка правее
int DownCell = 0;								//€чейка ниже
int LeftCell = 0;								//€чейка левее

int StartPositionX;
int StartPositionY;

int FinishPositionX;
int FinishPositionY;

int MoveCounter = 0;

using namespace std;

void InitAround(stack <int> &Stack1) {
	AppCell = VisitCell[Stack1.top() - 1][StackPreviousElement];
	RightCell = VisitCell[Stack1.top()][StackPreviousElement + 1];
	DownCell = VisitCell[Stack1.top() + 1][StackPreviousElement];
	LeftCell = VisitCell[Stack1.top()][StackPreviousElement - 1];
}
void InitAroundBot(stack <int> &Stack1) {
	AppCell = Maze[Stack1.top() - 1][StackPreviousElement];
	RightCell = Maze[Stack1.top()][StackPreviousElement + 1];
	DownCell = Maze[Stack1.top() + 1][StackPreviousElement];
	LeftCell = Maze[Stack1.top()][StackPreviousElement - 1];
}
void VisualAroundBot(stack <int> &Stack1) {
	MazeBot[Stack1.top() - 1][StackPreviousElement] = AppCell;
	MazeBot[Stack1.top()][StackPreviousElement + 1] = RightCell;
	MazeBot[Stack1.top() + 1][StackPreviousElement] = DownCell;
	MazeBot[Stack1.top()][StackPreviousElement - 1] = LeftCell;
}
void InitPreviousElementStack(stack <int> &Stack1) {
	StackFirstElement = Stack1.top();
	Stack1.pop();
	StackPreviousElement = Stack1.top();
	Stack1.push(StackFirstElement);
}
void KillWall(stack <int> &Stack1, int x, int y) {
	Maze[(Stack1.top() - 1) * 2 + 1 + x][(StackPreviousElement - 1) * 2 + 1 + y] = 0;
	VisitCell[Stack1.top() + x][StackPreviousElement + y] = 1;
	StackFirstElement = Stack1.top();
	Stack1.push(StackPreviousElement + y);
	StackPreviousElement = Stack1.top();
	Stack1.push(StackFirstElement + x);
}

void GenerLowArrs() {
	for (int i = 0; i < Height; i++) {
		for (int p = 0; p < Width; p++) {
			Maze[i][p] = 1;
		}
	}

	for (int i = 1; i < Height; i += 2) {
		for (int p = 1; p < Width; p += 2) {
			Maze[i][p] = 0;
		}
	}

	for (int i = 0; i < Height / 2 + 2; i++) {
		for (int p = 0; p < Width / 2 + 2; p++) {
			VisitCell[i][p] = 1;
		}
	}

	for (int i = 1; i < Height / 2 + 1; i++) {
		for (int p = 1; p < Width / 2 + 1; p++) {
			VisitCell[i][p] = 0;
		}
	}
	for (int i = 0; i < Height; i++) {
		for (int p = 0; p < Width; p++) {
			MazeBot[i][p] = 4;
		}
	}
}
void GenerMaze(stack <int> &Stack2) {
	Stack2.push(1);
	Stack2.push(1);
	VisitCell[Stack2.top()][StackPreviousElement] = 1;

	while (!Stack2.empty()) {
		InitAround(Stack2);
		RandomDirection = rand() % 4 + 1;

		if ((AppCell == 1) & (RightCell == 1) & (DownCell == 1) & (LeftCell == 1)) {
			Stack2.pop();
			Stack2.pop();
			if (Stack2.empty() == 1) { break; }
			else {
				InitPreviousElementStack(Stack2);
				RandomDirection = 5;
			}
		}

		switch (RandomDirection) {
		case 1:
			if (AppCell == 0) { KillWall(Stack2, -1, 0); }
			else { RandomDirection = rand() % 4 + 1; }
			break;

		case 2:
			if (RightCell == 0) { KillWall(Stack2, 0, 1); }
			else { RandomDirection = rand() % 4 + 1; }
			break;

		case 3:
			if (DownCell == 0) { KillWall(Stack2, 1, 0); }
			else { RandomDirection = rand() % 4 + 1; }
			break;

		case 4:
			if (LeftCell == 0) { KillWall(Stack2, 0, -1); }
			else { RandomDirection = rand() % 4 + 1; }
			break;
		default: break;
		}
	}
}

void OneStepBot(stack <int> &Stack1, int x, int y) {
	Maze[Stack1.top() + x][StackPreviousElement + y] = 2;
	MazeBot[Stack1.top() + x][StackPreviousElement + y] = 2;
	Stack1.push(StackPreviousElement + y);
	Stack1.push(StackFirstElement + x);
}

void OneMoveBot(stack <int> &Stack1) {
	InitPreviousElementStack(Stack1);
	InitAroundBot(Stack1);

	if (!((Stack1.top() == FinishPositionX) & (StackPreviousElement == FinishPositionY))) {
		VisualAroundBot(Stack1);
	}

	RandomDirection = rand() % 4 + 1;

	if ((AppCell != 0) & (RightCell != 0) & (DownCell != 0) & (LeftCell != 0)) {
		Maze[Stack1.top()][StackPreviousElement] = 3;
		Stack1.pop();
		Stack1.pop();
		RandomDirection = 5;
	}

	switch (RandomDirection) {
	case 1:
		if (AppCell == 0) { OneStepBot(Stack1, -1, 0); }
		else { RandomDirection = rand() % 4 + 1; }
		break;

	case 2:
		if (RightCell == 0) { OneStepBot(Stack1, 0, 1); }
		else { RandomDirection = rand() % 4 + 1; }
		break;

	case 3:
		if (DownCell == 0) { OneStepBot(Stack1, 1, 0); }
		else { RandomDirection = rand() % 4 + 1; }
		break;

	case 4:
		if (LeftCell == 0) { OneStepBot(Stack1, 0, -1); }
		else { RandomDirection = rand() % 4 + 1; }
		break;

	default: break;
	}
}

void quad(int x, int y, float red, float green, float blue) {
	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex2i(0 + x * blockSize, 0 + y * blockSize);
	glColor3f(red, green, blue);
	glVertex2i(0 + x * blockSize, blockSize + y * blockSize);
	glColor3f(red, green, blue);
	glVertex2i(blockSize + x * blockSize, blockSize + y * blockSize);
	glColor3f(red, green, blue);
	glVertex2i(blockSize + x * blockSize, 0 + y * blockSize);
	glEnd();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

void DisplayMaze(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	for (int i = 0; i < Height; i++) {
		for (int p = 0; p < Width; p++) {
			int Cell = Maze[i][p];
			switch (Cell) {
			case 1: quad(p, i, 0.0, 0.0, 0.0); break;
			case 5: quad(p, i, 0.0, 0.0, 1.0); break;
			case 6: quad(p, i, 1.0, 1.0, 0.0); break;
			}
		}
	}

	glFlush();
	glutSwapBuffers();
}

void DisplayMazeBot(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	for (int i = 0; i < Height; i++) {
		for (int p = 0; p < Width; p++) {
			int Cell = MazeBot[i][p];
			switch (Cell) {
			case 1: quad(p, i, 0.0, 0.0, 0.0); break;
			case 2: quad(p, i, 0.0, 1.0, 0.0); break;
			case 3: quad(p, i, 1.0, 0.0, 0.0); break;
			case 4: quad(p, i, 1.0, 0.5, 0.0); break;
			case 5: quad(p, i, 0.0, 0.0, 1.0); break;
			case 6: quad(p, i, 1.0, 1.0, 0.0); break;
			}
		}
	}
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	srand(time(0));

	StartPositionX = (rand() % (Height / 2)) * 2 + 1;
	StartPositionY = (rand() % (Width / 2)) * 2 + 1;
	FinishPositionX = (rand() % (Height / 2)) * 2 + 1;
	FinishPositionY = (rand() % (Width / 2)) * 2 + 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(blockSize * Width, blockSize * Height);

	stack <int> Stack;

	glutCreateWindow("лабиринт");

	GenerLowArrs();
	GenerMaze(Stack);

	glutReshapeFunc(reshape);
	glutDisplayFunc(DisplayMaze);

	glutCreateWindow("лабиринт с прохождением");

	glutReshapeFunc(reshape);

	Stack.push(StartPositionY);
	Stack.push(StartPositionX);
	Maze[StartPositionX][StartPositionY] = 2;

	while (!((Stack.top() == FinishPositionX) & (StackPreviousElement == FinishPositionY))) {
		OneMoveBot(Stack);
		InitPreviousElementStack(Stack);
	}

	Maze[StartPositionX][StartPositionY] = 5;
	Maze[FinishPositionX][FinishPositionY] = 6;
	MazeBot[StartPositionX][StartPositionY] = 5;
	MazeBot[FinishPositionX][FinishPositionY] = 6;
//	cout << MoveCounter;
	glutDisplayFunc(DisplayMazeBot);
	glutMainLoop();
}