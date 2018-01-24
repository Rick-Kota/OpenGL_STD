/*
 * ball Robo Kick課題プログラム
 * Original Author 代田 康貴
 * Modified by 小澤航太朗
 */

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <cmath>

#define PI tan(1)*4
#define Pai 3.1415926
#define G 9.8
// ボールの初期位置の定義
#define Ball_Point -50

float t = 0, dt = 0.1;
int fg = 0;

typedef struct {
    GLfloat mass;
    GLfloat e;
    GLfloat r;
    GLfloat ang;
    GLfloat x;
    GLfloat y;
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
ball_cdt ball[10];

double mx, my;

void circle2D(double radius, int x, int y) {
    for (auto th1 = 0; th1 <= 360; th1 += 1) {
        auto th2 = th1 + 10.0;
        auto th1_rad = th1 / 180.0 * Pai;
        auto th2_rad = th2 / 180.0 * Pai;

        double x1 = radius * cos(th1_rad);
        double y1 = radius * sin(th1_rad);
        double x2 = radius * cos(th2_rad);
        double y2 = radius * sin(th2_rad);

        glBegin(GL_TRIANGLES);
        glVertex2f(x, y);
        glVertex2f(static_cast<GLfloat>(x1 + x), static_cast<GLfloat>(y1 + y));
        glVertex2f(static_cast<GLfloat>(x2 + x), static_cast<GLfloat>(y2 + y));
        glEnd();
    }


}

void Write_leg() {
    glBegin(GL_LINE_LOOP);
    glVertex3f(7, 0, 0);
    glVertex3f(7, leg1.length, 0);
    glVertex3f(5, leg1.length, 0);
    glVertex3f(0, leg1.length, 0);
    glVertex3f(0, leg1.length - 2, 0);
    glVertex3f(5, leg1.length - 2, 0);
    glVertex3f(5, 0, 0);
    glEnd();

}

void Wall() {
    glBegin(GL_LINE_STRIP);
    glVertex3f(-150, 0, 0);
    glVertex3f(90, 0, 0);
    glVertex3f(90, 210, 0);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);                       // OpenGLのバッファをクリア

    //行列スタックへのPushとPopに使う
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);                           // 円の色を指定
    glTranslatef(Ball_Point + ball[0].x, 4 + ball[0].y, 0.0);  // 円を描画した座標系の移動
    glRotatef(ball[0].ang, 0.0, 0.0, 1.0);              // 円を描画した座標系の回転
    circle2D(ball[1].r, 0, 0);                          // 円を描画
    glPopMatrix();

    glPushMatrix();                                     // 脚の描画
    glColor3f(0.0, 1.0, 0.0);                           // 足の色を指定
    glTranslatef(static_cast<GLfloat>(Ball_Point - leg1.length / sqrt(2) + 2),
                 static_cast<GLfloat>(leg1.length - (leg1.length - leg1.length / sqrt(2))), 0);
    glRotatef(leg1.ang, 0.0, 0.0, 1.0);
    Write_leg();                                        // 脚の描画
    glPopMatrix();

    glPushMatrix();                                     // 壁の描画
    glColor3f(1.0, 1.0, 1.0);                           // 壁の色を指定
    Wall();                                             // 壁の描画
    glPopMatrix();

    glutSwapBuffers();                                  // Swap(画面の更新)作業の実行
}

void simu() {
    // 結果時間 "t" を常にカウントアップ
    t = t + dt;
    // 足先位置の計算
    mx = Ball_Point - leg1.length / sqrt(2) - leg1.length * sin(leg1.ang * PI / 360) + 2;
    my = leg1.length / sqrt(2) + leg1.length * cos(leg1.ang * PI / 360);
    if (fg == 0) {
        // 脚の回転運動の定義
        leg1.ang = (leg1.ang + leg1.dang * dt) + (leg1.ddang * dt * dt / 2);
        // 脚の回転運動の減衰の計算 (Disable)
        leg1.dang = leg1.dang + leg1.ddang * dt;

        // 脚の先端がボールが存在する座標（規定値）に達した場合 = ボールとの当たり判定
        if (pow(mx - (Ball_Point), 2) + pow(my, 2) <= pow(ball[0].r, 2)) {
            // 足先長さと重量等からボールの初速度を計算
            ball[0].dx = leg1.length * 2 * PI * leg1.dang / (360 * ball[0].mass * sqrt(2));
            ball[0].dy = leg1.length * 2 * PI * leg1.dang / (360 * ball[0].mass * sqrt(2));
            printf("%f\n", ball[0].dx);
            // 接触フラグの立ち上げ
            fg = 1;
        }
    }

    // フラグが立ち上がった場合ボールが蹴られたとしボールを動かす
    if (fg == 1) {
        // 脚の動きを停止する
        leg1.dang = 0;
        leg1.ddang = 0;
        //ボールの動きを定義
        if (ball[0].y > -0.001) {   // ボール座標の中心が定数より大きい場合　= 床への当たり判定
            // ボールの放物線運動を定義
            ball[0].y += (ball[0].dy * dt) + (ball[0].ddy * dt * dt / 2);
            // ボールの上下運動における減衰の計算(Disable)
            ball[0].dy += ball[0].ddy * dt;

            // ボールが空中にある時回転させる
            if (ball[0].y > 0) ball[0].ang += 0.2;
            // ボールの現在角度が360度を超えた時0度に戻す
            if (ball[0].ang > 360.0) ball[0].ang = static_cast<GLfloat>(ball[0].ang - 360.0);
        } else { // ボールが完全に地面に接触した場合
            // "e" = 反発係数 反発させる
            ball[0].dy = -ball[0].e * ball[0].dy;
            // 閾値の上側に移動させる
            ball[0].y = 0;
            // ボールの動きが十分に小さい時動きを止める
            if (ball[0].dy <= 5) {
                ball[0].ddy = 0;
                ball[0].dy = 0;
                ball[0].y = 0;
                ball[0].ddx = 0;
                ball[0].dx = 0;
            }
        }

        // X方向の移動を定義
        ball[0].x = ball[0].x + ball[0].dx * dt + ball[0].ddx * dt * dt / 2;
        // X方向の移動における減衰の計算 (Disable)
        ball[0].dx = ball[0].dx + ball[0].ddx * dt;

        // X方向の壁に対する当たり判定
        if (ball[0].x > 140 - ball[0].r) {
            // 反発による速度の減衰を計算
            ball[0].dx = -ball[0].e * ball[0].dx;
            // 反発による加速度の減衰を計算
            ball[0].ddx = -ball[0].e * ball[0].ddx;
        }
    }
    glutPostRedisplay();
}

void init() {
    // 背景色の指定 & 初期化
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    // ボール構造体の初期化
    for (auto &i : ball) {
        i.x = 0;
        i.y = 0;
        i.dang = 0;
        i.dx = 0;
        i.dy = 0;
        i.ddx = 0;
        i.ddy = static_cast<GLfloat>(-G);
    }
    ball[0].mass = 5;
    ball[0].e = 0.8;
    ball[0].r = 4;
    ball[1].mass = 10;
    ball[1].e = 0.8;
    ball[1].r = 4;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
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
                glutIdleFunc(nullptr);
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
    glutCreateWindow("Hellow Robo Kick!!");         // Window の作成
    glutInitWindowSize(400, 400);                   // Window サイズの指定
    glutInitWindowPosition(100, 100);               // Window サイズの指定
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}

