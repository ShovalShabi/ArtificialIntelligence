#include "glut.h"
#include "MazeManager.h"
#include "GameManager.h"

MazeManager* mazeMng;
GameManager* gameMng;

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
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("Algorithm example Example");

    mazeMng = new MazeManager();
    gameMng = new GameManager();

    glutDisplayFunc(displayWrapper); // Use static function as a wrapper
    glutIdleFunc(idleWrapper);       // Use static function as a wrapper

    glutCreateMenu(menuWrapper);     // Use static function as a wrapper
    glutAddMenuEntry("run BFS", 1);
    glutAddMenuEntry("run DFS", 2);
    glutAddMenuEntry("run BiBFS", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    mazeMng->init(gameMng);

    glutMainLoop();
    delete gameMng;
    delete mazeMng;
    return 0;
}
