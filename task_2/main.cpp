#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

// Window size
int winW = 800, winH = 600;

// Circle parameters
int xc = 400, yc = 300;
int N;              // number of circles
int maxRadius;      // radius of outermost circle
int thickness;      // thickness of each circle

// Color gradient (user input)
float r1, g1, b1; // inner circle color
float r2, g2, b2; // outer circle color

// -------- Bresenham Circle Points ----------
void drawCirclePoints(int xc, int yc, int x, int y) {
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
}

// -------- Bresenham Circle Algorithm with Thickness ----------
void drawCircle(int xc, int yc, int radius, int thickness) {
    for (int r = radius; r < radius + thickness; r++) {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;

        glBegin(GL_POINTS);
        while (x <= y) {
            drawCirclePoints(xc, yc, x, y);
            if (d < 0)
                d += 4 * x + 6;
            else {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
        glEnd();
    }
}

// -------- Display Function ----------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < N; i++) {
        // Calculate color gradient
        float t = i / float(N - 1);
        float r = (1 - t) * r1 + t * r2;
        float g = (1 - t) * g1 + t * g2;
        float b = (1 - t) * b1 + t * b2;

        glColor3f(r, g, b);

        // Radius for this circle
        int radius = (i + 1) * (maxRadius / N);

        drawCircle(xc, yc, radius, thickness);
    }

    glFlush();
}

// -------- Initialization ----------
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(0, winW, 0, winH);
}

// -------- Main Function ----------
int main(int argc, char** argv) {
    cout << "Enter number of circles: ";
    cin >> N;
    cout << "Enter maximum radius: ";
    cin >> maxRadius;
    cout << "Enter thickness of circles: ";
    cin >> thickness;

    cout << "Enter inner circle color (r g b, 0-1): ";
    cin >> r1 >> g1 >> b1;
    cout << "Enter outer circle color (r g b, 0-1): ";
    cin >> r2 >> g2 >> b2;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Concentric Circles with Gradient and Thickness");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
