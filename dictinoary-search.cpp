#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <cstring>    // Added for strlen
#include <unistd.h>   // Added for usleep

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DICTIONARY_SIZE 10

int dictionary[DICTIONARY_SIZE];
int searchNumber = 0;
int currentIndex = -1;
int searchIndex = -1;
bool searchComplete = false;
bool isSearching = false;
char stepMessage[50];
char terminalMessage[50];

// Animation state variables
bool animationStarted = false;
int animationDelay = 3000000; // Delay between each step in microseconds
int slowDownThreshold = 3;    // Number of steps before slowing down animation

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(10, WINDOW_HEIGHT - 20);
    char message[50];
    sprintf(message, "Enter number to search: %d", searchNumber);
    int length = strlen(message);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
    }

    glPushMatrix();
    glTranslatef(100, 100, 0);

    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        if (i < currentIndex) {
            glColor3f(0.0, 1.0, 0.0); // Highlight the visited elements in green
        } else {
            glColor3f(1.0, 1.0, 1.0); // Default color
        }

        if (i == currentIndex) {
            glColor3f(1.0, 0.0, 0.0); // Highlight the current element being checked in red
        }

        if (i == searchIndex) {
            glColor3f(1.0, 1.0, 0.0); // Highlight the found number with yellow color
        }

        glRecti(i * 50, 0, (i + 1) * 50, 50);

        glColor3f(0.0, 0.0, 0.0);
        char number[5];
        sprintf(number, "%d", dictionary[i]);
        glRasterPos2i(i * 50 + 20, 20);
        length = strlen(number);
        for (int j = 0; j < length; j++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, number[j]);
        }
    }

    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(10, WINDOW_HEIGHT - 50);
    length = strlen(stepMessage);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, stepMessage[i]);
    }

    // Display terminal message
    glRasterPos2i(10, WINDOW_HEIGHT - 80);
    length = strlen(terminalMessage);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, terminalMessage[i]);
    }

    if (searchComplete) {
        glColor3f(1.0, 1.0, 0.0);
        glRasterPos2i(10, WINDOW_HEIGHT - 110);
        char resultMessage[50];
        if (searchIndex == -1) {
            sprintf(resultMessage, "Number %d not found!", searchNumber);
        } else {
            sprintf(resultMessage, "Number %d found at index %d", searchNumber, searchIndex);
        }
        length = strlen(resultMessage);
        for (int i = 0; i < length; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, resultMessage[i]);
        }
    }

    glutSwapBuffers();
}

void searchNumberInDictionary() {
    if (currentIndex < DICTIONARY_SIZE) {
        sprintf(stepMessage, "Checking index %d...", currentIndex);

        if (dictionary[currentIndex] == searchNumber) {
            searchIndex = currentIndex;
            searchComplete = true;
        }

        currentIndex++;
    } else {
        searchComplete = true;
    }

    glutPostRedisplay();

    if (!searchComplete) {
        if (currentIndex > DICTIONARY_SIZE - slowDownThreshold) {
            usleep(animationDelay * 2); // Slow down animation when number is about to be found
        } else {
            usleep(animationDelay);
        }
    } else {
        isSearching = false;
    }
}

void idle(void) {
    if (animationStarted && isSearching) {
        searchNumberInDictionary();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key >= '0' && key <= '9') {
        searchNumber = searchNumber * 10 + (key - '0');
        //printf("%d\n", searchNumber); // Print the number as it is being entered
    } else if (key == 13) { // ASCII value of 'Enter' key
        printf("Searching for number: %d\n", searchNumber);
        sprintf(terminalMessage, "Searching for number: %d", searchNumber);
        currentIndex = 0;
        searchIndex = -1;
        searchComplete = false;
        isSearching = true;
        animationStarted = true;
    }
}

int main(int argc, char **argv) {
    printf("Enter %d numbers for the dictionary:\n", DICTIONARY_SIZE);
    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        printf("Number %d: ", i + 1);
        scanf("%d", &dictionary[i]);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Dictionary Search");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutMainLoop();
    return 0;
}
