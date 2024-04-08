#include "glut.h"
#include "MazeMngr.h"
#include "GameMngr.h"

MazeMngr* mazeMng;
GameMngr* gameMng;

void displayWrapper() {
	mazeMng->display();
}

void idleWrapper() {
	mazeMng->idle(gameMng);
}

void menuWrapper(int choice) {
	mazeMng->menu(choice, gameMng);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(400, 50);
	glutCreateWindow("Packman Final Project");

	mazeMng = new MazeMngr();
	gameMng = new GameMngr(NUM_OF_COINS);

	glutDisplayFunc(displayWrapper); // Use static function as a wrapper
	glutIdleFunc(idleWrapper);       // Use static function as a wrapper

	glutCreateMenu(menuWrapper);     // Use static function as a wrapper
	glutAddMenuEntry("Run Packman Game", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	mazeMng->init(gameMng);

	glutMainLoop();
	delete gameMng;
	delete mazeMng;
	return 0;
}