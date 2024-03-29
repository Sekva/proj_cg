#ifndef PIPES_H
#define PIPES_H
#include <vector>
#define cubo_vec std::vector<std::vector<std::vector<int>>>

#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/glut.h>




enum DIR {
    CIMA = 1,
    BAIXO = 2,
    FRENTE = 3,
    TRAS = 4,
    DIREITA = 5,
    ESQUERDA = 6,
};


struct ponto {
    int x;
    int y;
    int z;
};


#include <vector>

class Pipes {



    private:
        cubo_vec mundo;
        cubo_vec memoria;
        std::vector<ponto> pos;
        unsigned long contador;
        char num_pipes;
        int dim;
        double tamanho_cubo;
        bool conectar;

        GLuint cubo_text;
        int cubo_x;
        int cubo_y;
        GLuint ligamento_text;
        int ligamento_x;
        int ligamento_y;

        unsigned char* cubo_image;
        unsigned char* ligamento_image;

        bool textura_carregada;



    public:
        Pipes(int num_pipes, int dim);

        void att();
        void render(double var_tamanho, bool usa_textura);
        void resetar();
        void set_cor(int in);


};




#endif
