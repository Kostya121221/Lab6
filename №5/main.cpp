#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

const double PI = 3.14159265358979323846;

struct Point {
    double x, y;
};

// Глобальные переменные для хранения параметров окна и фрактала
int windowWidth = 800;
int windowHeight = 600;
int recursionDepth = 3;
std::vector<Point> kochPoints;

// Рекурсивный расчет точек кривой Коха
void generateKochCurve(int n, Point p1, Point p2, std::vector<Point>& points) {
    if (n == 0) {
        points.push_back(p1);
        return;
    }

    Point s, v, e;

    s.x = p1.x + (p2.x - p1.x) / 3.0;
    s.y = p1.y + (p2.y - p1.y) / 3.0;

    e.x = p1.x + 2.0 * (p2.x - p1.x) / 3.0;
    e.y = p1.y + 2.0 * (p2.y - p1.y) / 3.0;

    double angle = PI / 3.0; // 60 градусов
    v.x = s.x + (e.x - s.x) * cos(angle) - (e.y - s.y) * sin(angle);
    v.y = s.y + (e.x - s.x) * sin(angle) + (e.y - s.y) * cos(angle);

    generateKochCurve(n - 1, p1, s, points);
    generateKochCurve(n - 1, s, v, points);
    generateKochCurve(n - 1, v, e, points);
    generateKochCurve(n - 1, e, p2, points);
}

// Функция инициализации параметров фрактала
void initFractal() {
    kochPoints.clear();
    
    // Задаем базовые точки с отступами от краев окна
    Point p1 = { 50.0, 150.0 };
    Point p2 = { static_cast<double>(windowWidth) - 50.0, 150.0 };

    generateKochCurve(recursionDepth, p1, p2, kochPoints);
    kochPoints.push_back(p2); // Добавляем финальную точку
}

// Основная функция отрисовки сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(0.0f, 1.0f, 0.0f);  

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);          

    glBegin(GL_LINE_STRIP);
    for (const auto& pt : kochPoints) {
        glVertex2d(pt.x, pt.y);
    }
    glEnd();

    glutSwapBuffers(); 
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initFractal();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    std::cout << "Enter recursion depth (recommended n = 1..6): ";
    if (!(std::cin >> recursionDepth) || recursionDepth < 0) {
        std::cout << "Invalid input!" << std::endl;
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Koch Curve - OpenGL/GLUT");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    initFractal();

    glutMainLoop();

    return 0;
}
