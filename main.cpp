/*
 * B1 Robo Kick課題プログラム
 * Original Author 代田 康貴
 */

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <cmath>

#define PI tan(1)*4
#define G 9.8

float t = 0, dt = 0.1;
int fg = 0;

typedef struct {
    GLfloat mass;
    GLfloat e;
    GLfloat r;
    GLfloat ang;
    double x;
    double y;
    GLfloat dang;
    double dx;
    double dy;
    GLfloat ddx;
    GLfloat ddy;
} ball_cdt;

typedef struct {
    GLfloat ang;
    GLfloat dang;
    GLfloat ddang;
    GLfloat length;
} leg_cdt;

leg_cdt leg1 = {90, 80, 0, 60};    //脚の初期角度,角速度,?,脚の長さ
ball_cdt b1[10];

double mx, my;

void display() {
    int i;
    float theta = 0;

    glClear(GL_COLOR_BUFFER_BIT);

    //行列スタックへのPushとPopに使う
    glPushMatrix();

    glColor3f(1.0, 1.0, 0.0);

    glTranslatef(static_cast<GLfloat>(-50 + b1[0].x), static_cast<GLfloat>(b1[0].y), 0.0);

    glRotatef(b1[0].ang, 0.0, 0.0, 1.0);

    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(0, 0, 0);

    for (i = 0; i < 55; i++) {
        theta += PI / 60;
        glVertex3f(b1[0].r * cos(theta), b1[0].r * sin(theta), 0);
    }

    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(static_cast<GLfloat>(-50 - leg1.length / sqrt(2) + 2),
                 static_cast<GLfloat>(leg1.length - (leg1.length - leg1.length / sqrt(2))), 0);
    glRotatef(leg1.ang, 0.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(7, 0, 0);
    glVertex3f(7, leg1.length, 0);
    glVertex3f(5, leg1.length, 0);
    glVertex3f(0, leg1.length, 0);
    glVertex3f(0, leg1.length - 2, 0);
    glVertex3f(5, leg1.length - 2, 0);
    glVertex3f(5, 0, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();

    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-150, 0, 0);
    glVertex3f(90, 0, 0);
    glVertex3f(90, 210, 0);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void simu() {
    t = t + dt;
    mx = -50 - leg1.length / sqrt(2) - leg1.length * sin(leg1.ang * PI / 360) + 2;
    my = leg1.length / sqrt(2) + leg1.length * cos(leg1.ang * PI / 360);
    if (fg == 0) {
        leg1.ang = leg1.ang + leg1.dang * dt + leg1.ddang * dt * dt / 2;
        leg1.dang = leg1.dang + leg1.ddang * dt;

        if (pow(mx - (-50), 2) + pow(my, 2) <= pow(b1[0].r, 2)) {
            b1[0].dx = leg1.length * 2 * PI * leg1.dang / (360 * b1[0].mass * sqrt(2));
            b1[0].dy = leg1.length * 2 * PI * leg1.dang / (360 * b1[0].mass * sqrt(2));
            printf("%f\n", b1[0].dx);
            fg = 1;
        }
    }

    if (fg == 1) {

        leg1.dang = 0;
        leg1.ddang = 0;

        //ball1
        if (b1[0].y > -0.001) {
            b1[0].y = b1[0].y + b1[0].dy * dt + b1[0].ddy * dt * dt / 2;
            b1[0].dy = b1[0].dy + b1[0].ddy * dt;

            if (b1[0].y > 0) b1[0].ang += 0.2;
            if (b1[0].ang > 360.0) b1[0].ang = static_cast<GLfloat>(b1[0].ang - 360.0);
        } else {
            b1[0].dy = -b1[0].e * b1[0].dy;

            if (b1[0].dy <= 5) {
                b1[0].ddy = 0;
                b1[0].dy = 0;
                b1[0].y = 0;
                b1[0].ddx = 0;
                b1[0].dx = 0;
            }
            b1[0].y = 0;
        }

        b1[0].x = b1[0].x + b1[0].dx * dt + b1[0].ddx * dt * dt / 2;
        b1[0].dx = b1[0].dx + b1[0].ddx * dt;

        if (b1[0].x > 140 - b1[0].r) {
            b1[0].dx = -b1[0].e * b1[0].dx;
            b1[0].ddx = -b1[0].e * b1[0].ddx;
        }
    }
    glutPostRedisplay();
}

void init() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    for (auto &i : b1) {
        i.x = 0;
        i.y = 0;
        i.dang = 0;
        i.dx = 0;
        i.dy = 0;
        i.ddx = 0;
        i.ddy = static_cast<GLfloat>(-G);
    }

    b1[0].mass = 5;
    b1[0].e = 0.8;
    b1[0].r = 4;
    b1[1].mass = 10;
    b1[1].e = 0.8;
    b1[1].r = 4;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
    glOrtho(-100.0, 100.0, -10.0, 210.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(simu);
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                glutIdleFunc(NULL);
            }
            break;
        default:
            break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '\x1b') exit(0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}

