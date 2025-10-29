#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm> // for std::min, std::max
using namespace std;

int winW = 800, winH = 600;
int x1_, y1_, x2_, y2_;
bool thickMode = false;
int lineWidth = 3;

// --------- Bresenham Line Algorithm (All Slopes) ----------
void drawBresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
    glEnd();
}

// --------- Draw Circle for Thickness (8-way fill) ----------
void drawCirclePoints(int cx, int cy, int r) {
    for (int dx = -r; dx <= r; dx++) {
        for (int dy = -r; dy <= r; dy++) {
            if (dx * dx + dy * dy <= r * r)
                glVertex2i(cx + dx, cy + dy);
        }
    }
}

// --------- Draw Thick Line ----------
void drawThickLine(int x1, int y1, int x2, int y2, int W) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int r = W / 2;

    glBegin(GL_POINTS);
    while (true) {
        drawCirclePoints(x1, y1, r);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
    glEnd();
}

// --------- Display Function ----------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);
    if (thickMode)
        drawThickLine(x1_, y1_, x2_, y2_, lineWidth);
    else
        drawBresenhamLine(x1_, y1_, x2_, y2_);

    glFlush();
}

// --------- Keyboard Controls ----------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            lineWidth = std::min(lineWidth + 1, 50);
            thickMode = true; // auto-enable thick mode
            cout << "Line thickness: " << lineWidth << endl;
            glutPostRedisplay();
            break;

        case '-':
            lineWidth = std::max(lineWidth - 1, 1);
            thickMode = true; // auto-enable thick mode
            cout << "Line thickness: " << lineWidth << endl;
            glutPostRedisplay();
            break;

        case 't': case 'T':
            thickMode = !thickMode;
            cout << "Thick mode: " << (thickMode ? "ON" : "OFF") << endl;
            glutPostRedisplay();
            break;

        case 'q': case 'Q':
        case 27: // ESC
            exit(0);
            break;
    }
}

// --------- Initialization ----------
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    gluOrtho2D(0, winW, 0, winH);
}

// --------- Main Function ----------
int main(int argc, char** argv) {
    cout << "Enter first point (x1 y1): ";
    cin >> x1_ >> y1_;
    cout << "Enter second point (x2 y2): ";
    cin >> x2_ >> y2_;
    cout << "Enter initial line thickness: ";
    cin >> lineWidth;
    if(lineWidth > 1) thickMode = true; // automatically enable thick mode if thickness >1

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Bresenham Line Drawing (Thick + Keyboard Control)");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    cout << "\n[Controls]\n";
    cout << "  +  Increase line thickness (auto-enable thick mode)\n";
    cout << "  -  Decrease line thickness (auto-enable thick mode)\n";
    cout << "  t  Toggle thick mode ON/OFF\n";
    cout << "  q / ESC  Quit program\n";

    glutMainLoop();
    return 0;
}
