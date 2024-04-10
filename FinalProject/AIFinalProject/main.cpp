/**
 * @file main.cpp
 * @brief Main file for running the Pacman game using GLUT.
 */

#include "glut.h"
#include "MazeMngr.h"
#include "GameMngr.h"

MazeMngr* mazeMng; ///< Pointer to the Maze Manager instance.
GameMngr* gameMng; ///< Pointer to the Game Manager instance.

/**
 * @brief Wrapper function for the display method of MazeMngr class.
 */
void displayWrapper() {
    mazeMng->display();
}

/**
 * @brief Wrapper function for the idle method of MazeMngr class, passing gameMng as a parameter.
 */
void idleWrapper() {
    mazeMng->idle(gameMng);
}

/**
 * @brief Wrapper function for the menu method of MazeMngr class.
 * @param choice The menu choice.
 */
void menuWrapper(int choice) {
    mazeMng->menu(choice, gameMng);
}

/**
 * @brief Main function for initializing and running the Pacman game.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Integer indicating the exit status.
 */
int main(int argc, char* argv[]) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(400, 50);
    glutCreateWindow("Packman Final Project");

    // Create instances of MazeMngr and GameMngr
    mazeMng = new MazeMngr();
    gameMng = new GameMngr(NUM_OF_COINS);

    // Register display, idle, and menu functions
    glutDisplayFunc(displayWrapper); // Use static function as a wrapper
    glutIdleFunc(idleWrapper);       // Use static function as a wrapper

    glutCreateMenu(menuWrapper);     // Use static function as a wrapper
    glutAddMenuEntry("Run Packman Game", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Initialize MazeMngr
    mazeMng->init(gameMng);

    // Enter the GLUT event processing loop
    glutMainLoop();

    // Cleanup: delete instances of MazeMngr and GameMngr
    delete gameMng;
    delete mazeMng;

    return 0;
}
