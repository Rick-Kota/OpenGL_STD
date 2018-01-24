//
// Created by 小澤航太朗 on 2018/01/24.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>


// 頂点属性 　
struct Vertex {
// 位置
    GLfloat position[2];
};

static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

GLuint VertexArrayID;
glGenVertexArrays(1, &VertexArrayID);
glBindVertexArray(VertexArrayID);



int main() {
// GLFW を初期化する
    if (glfwInit() == GL_FALSE) {
// 初期化に失敗した
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }

//プログラム終了時の処理を登録する
    atexit(glfwTerminate);

// Windowを作成する
    GLFWwindow *const window(glfwCreateWindow(640, 480, "Hello!", nullptr, nullptr));
    if (window == nullptr) {
// Windowが作成できなかった
        std::cerr << "Can't create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }
// 作成したWindowを OpenGL の処理対象にする
    glfwMakeContextCurrent(window);

    // GLEW を初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        // GLEW の初期化に失敗した
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }


    // 垂直同期のタイミングを待つ
    glfwSwapInterval(1);

    // 背景色を指定する
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // これが頂点バッファを指し示すものとなります。
    GLuint vertexbuffer;

// バッファを1つ作り、vertexbufferに結果IDを入れます。
    glGenBuffers(1, &vertexbuffer);

// 次のコマンドは'vertexbuffer'バッファについてです。
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

// 頂点をOpenGLに渡します。
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT);
        //
        // 最初の属性バッファ：頂点
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
                3,                  // サイズ
                GL_FLOAT,           // タイプ
                GL_FALSE,           // 正規化？
                0,                  // ストライド
                (void*)0            // 配列バッファオフセット
        );

// 三角形を描きます！
        glDrawArrays(GL_TRIANGLES, 0, 3); // 頂点0から始まります。合計3つの頂点です。&rarr;1つの三角形です。

        glDisableVertexAttribArray(0);
        //
        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        glfwWaitEvents();
    }
}
