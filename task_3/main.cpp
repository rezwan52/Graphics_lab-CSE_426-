#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

// Window size
int winW = 800, winH = 600;

// Clipping window
float xmin, ymin, xmax, ymax;

// Lines
struct Line {
    float x0, y0, x1, y1;
};
vector<Line> lines;

// -------- Liang-Barsky Algorithm ----------
bool liangBarsky(float x0, float y0, float x1, float y1,
                 float xmin, float ymin, float xmax, float ymax,
                 float &xc0, float &yc0, float &xc1, float &yc1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {x0 - xmin, xmax - x0, y0 - ymin, ymax - y0};
    float u1 = 0.0f, u2 = 1.0f;

    for(int i=0; i<4; i++){
        if(p[i]==0){
            if(q[i]<0) return false; // Parallel and outside
        } else {
            float r = q[i]/p[i];
            if(p[i]<0) u1 = max(u1, r);
            else       u2 = min(u2, r);
        }
    }
    if(u1 > u2) return false;

    xc0 = x0 + u1*dx;
    yc0 = y0 + u1*dy;
    xc1 = x0 + u2*dx;
    yc1 = y0 + u2*dy;
    return true;
}

// -------- Draw Function ----------
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clipping window
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
    glEnd();

    // Draw original lines in gray
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for(auto &l : lines){
        glVertex2f(l.x0, l.y0);
        glVertex2f(l.x1, l.y1);
    }
    glEnd();

    // Draw clipped lines in red
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(auto &l : lines){
        float xc0, yc0, xc1, yc1;
        if(liangBarsky(l.x0, l.y0, l.x1, l.y1, xmin, ymin, xmax, ymax, xc0, yc0, xc1, yc1)){
            glVertex2f(xc0, yc0);
            glVertex2f(xc1, yc1);
        }
    }
    glEnd();

    glFlush();
}

// -------- Initialization ----------
void init(){
    glClearColor(1,1,1,1);
    gluOrtho2D(0, winW, 0, winH);
}

// -------- Main Function ----------
int main(int argc, char** argv){
    cout << "Enter clipping window (xmin ymin xmax ymax): ";
    cin >> xmin >> ymin >> xmax >> ymax;

    int n;
    cout << "Enter number of lines: ";
    cin >> n;

    for(int i=0;i<n;i++){
        Line l;
        cout << "Enter line " << i+1 << " (x0 y0 x1 y1): ";
        cin >> l.x0 >> l.y0 >> l.x1 >> l.y1;
        lines.push_back(l);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Liang-Barsky Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
