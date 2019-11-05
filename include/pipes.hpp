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
#include <glm/glm.hpp>
#include <GL/glu.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <GL/glut.h>




enum DIR {
    CIMA,
    BAIXO,
    FRENTE,
    TRAS,
    DIREITA,
    ESQUERDA,
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
        std::vector<ponto> pos;
        unsigned long contador;
        char num_pipes;
        int dim;
        double tamanho_cubo;

    public:
        Pipes(int num_pipes, int dim);

        void att();
        void render();
        void tentar_conectar(int x, int y, int z, int numero_cano);
        void set_cor(int in);


};




#endif
