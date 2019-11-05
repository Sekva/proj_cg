#ifndef RENDER_HPP
#define RENDER_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL/glu.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <GL/glut.h>



#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>

#include "../include/solido.hpp"
#define STB_IMAGE_IMPLEMENTATION
//#include "../include/stb_image.h"

#include "../include/pipes.hpp"



class Render {


    private:
        GLFWwindow* janela;
        std::vector<Objeto*> objetos;
        double pos[3];
        double camera_up[3];
        double look_at[3];
        int largura;
        int altura;

        Pipes* pipes;




        double pitch;
        double yaw;

        double fov;

        bool primeiro_mouse;

        double ultimo_mouse_x;
        double ultimo_mouse_y;
        double dt;
        double ut;

    private:
        void init();
        void reshape();
        void display();
        void mouse_update();
        void teclado_update();
        void att_entrada();
        void clear();

        void init_luzes();



    public:
        Render(int a);

        void add_obj(Objeto* o);
        void render();

};


#endif
