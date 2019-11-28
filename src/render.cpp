#include "../include/render.hpp"

#define luzes false

double var_tamanho = 0.1;
double var_escala = 0.1;

bool usa_textura = true;

double radianos(double graus) {
    return (3.141592653589793 * graus) / 180.0;
}


double norma(double vetor[3]) {
    return sqrt(vetor[0]*vetor[0] + vetor[1]*vetor[1] + vetor[2]*vetor[2]);
}


double* produto_cruzado(double a[3], double b[3]) {
    double* r = (double*) malloc(sizeof(double) * 3);

    r[0] = a[1]*b[2] - a[2]*b[1];
    r[1] = a[2]*b[0] - a[0]*b[2];
    r[2] = a[0]*b[1] - a[1]*b[0];

    return r;
}

Render::Render(int a) {

    this->pos[0] = 0;
    this->pos[1] = 0;
    this->pos[2] = 0;

    this->look_at[0] = 0;
    this->look_at[1] = 0;
    this->look_at[2] = -1;

    this->camera_up[0] = 0;
    this->camera_up[1] = 1;
    this->camera_up[2] = 0;

    this->fov = 60;

    this->pitch = 0.0;
    this->yaw = -90.0;

    this->primeiro_mouse = true;

    this->ultimo_mouse_x = 800 / 2;
    this->ultimo_mouse_y = 600 / 2;

    this->dt = 0;
    this->ut = 0;

    this->pipes = new Pipes(8, 45);


}


void Render::reshape() {

        glfwGetWindowSize(this->janela, &this->largura, &this->altura);
        //TODO: testar os valores
        glViewport(0, 0, this->largura, this->altura);

}


void Render::att_entrada() {


    double tempo_atual = glfwGetTime();
    this->dt = tempo_atual - this->ut;
    this->ut = tempo_atual;

    this->teclado_update();
    this->mouse_update();

}

void Render::mouse_update() {

    double xpos, ypos;
    glfwGetCursorPos(this->janela, &xpos, &ypos);

    if(this->primeiro_mouse) {
        this->primeiro_mouse = false;
        this->ultimo_mouse_x = xpos;
        this->ultimo_mouse_y = ypos;
    }



    double xoff = xpos - this->ultimo_mouse_x;
    double yoff = this->ultimo_mouse_y - ypos;

    this->ultimo_mouse_x = xpos;
    this->ultimo_mouse_y = ypos;


    // essa scl tem q ser proporcional ao fov
    int scl = 10;

    this->yaw += xoff / scl;
    this->pitch+= yoff / scl;

    this->pitch = this->pitch >  89 ?  89 : this->pitch;
    this->pitch = this->pitch < -89 ? -89 : this->pitch;

    double v_npos[3];

    v_npos[0] = cos(radianos(this->yaw)) * cos(radianos(this->pitch));
    v_npos[1] = sin(radianos(this->pitch));
    v_npos[2] = sin(radianos(this->yaw)) * cos(radianos(this->pitch));

    double val_norma = norma(v_npos);

    this->look_at[0] = v_npos[0] / val_norma;
    this->look_at[1] = v_npos[1] / val_norma;
    this->look_at[2] = v_npos[2] / val_norma;

}


void Render::teclado_update() {

    if(glfwGetKey(this->janela, GLFW_KEY_T) == GLFW_PRESS) {
        usa_textura  = !usa_textura;
    }

    if(glfwGetKey(this->janela, GLFW_KEY_I) == GLFW_PRESS) {
        var_tamanho += 0.01;
    }
    if(glfwGetKey(this->janela, GLFW_KEY_U) == GLFW_PRESS) {
        if(var_tamanho > 0)
            var_tamanho -= 0.01;
    }

    if(glfwGetKey(this->janela, GLFW_KEY_K) == GLFW_PRESS) {
        var_escala += 0.01;
    }
    if(glfwGetKey(this->janela, GLFW_KEY_J) == GLFW_PRESS) {
        var_escala -= 0.01;
    }

    if(glfwGetKey(this->janela, GLFW_KEY_P) == GLFW_PRESS) {

        if(this->fov >= 1 && this->fov <= 60) {
            this->fov -= 0.1;
        }

        if(this->fov < 1) { this->fov = 1; }
        if(this->fov > 60) { this->fov = 60; }

    }

    if(glfwGetKey(this->janela, GLFW_KEY_O) == GLFW_PRESS) {
        if(this->fov >= 1 && this->fov <= 60) {
            this->fov += 0.1;
        }

        if(this->fov < 1) { this->fov = 1; }
        if(this->fov > 60) { this->fov = 60; }

    }

    double vel = 6.5 * this->dt;

    if(glfwGetKey(this->janela, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS) {
        vel = 0.25 * this->dt;
    }



    if(glfwGetKey(this->janela, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->janela, true);
    }

    if(glfwGetKey(this->janela, GLFW_KEY_W) == GLFW_PRESS) {
        this->pos[0] += vel * this->look_at[0];
        this->pos[1] += vel * this->look_at[1];
        this->pos[2] += vel * this->look_at[2];
    }

    if(glfwGetKey(this->janela, GLFW_KEY_S) == GLFW_PRESS) {
        this->pos[0] -= vel * this->look_at[0];
        this->pos[1] -= vel * this->look_at[1];
        this->pos[2] -= vel * this->look_at[2];
    }

    if(glfwGetKey(this->janela, GLFW_KEY_A) == GLFW_PRESS) {

        double rr[3];
        rr[0] = this->look_at[0];
        rr[1] = this->look_at[1];
        rr[2] = this->look_at[2];

        double* cruzado = produto_cruzado(rr, this->camera_up);
        double val_norma = norma(cruzado);

        this->pos[0] -= (cruzado[0] / val_norma) * vel;
        this->pos[1] -= (cruzado[1] / val_norma) * vel;
        this->pos[2] -= (cruzado[2] / val_norma) * vel;

    }

    if(glfwGetKey(this->janela, GLFW_KEY_D) == GLFW_PRESS) {

        double rr[3];
        rr[0] = this->look_at[0];
        rr[1] = this->look_at[1];
        rr[2] = this->look_at[2];

        double* cruzado = produto_cruzado(rr, this->camera_up);
        double val_norma = norma(cruzado);

        this->pos[0] += (cruzado[0] / val_norma) * vel;
        this->pos[1] += (cruzado[1] / val_norma) * vel;
        this->pos[2] += (cruzado[2] / val_norma) * vel;

    }

    if(glfwGetKey(this->janela, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->pos[1] += vel; //y
    }

    if(glfwGetKey(this->janela, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        this->pos[1] -= vel; //y
    }


}


void Render::clear() {

        glClearColor(0.0, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


void Render::display() {

    static float alpha = 0;


    while (!glfwWindowShouldClose(this->janela)) {
        this->att_entrada();

        this->reshape();

        this->clear();

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective(this->fov, this->largura / this->altura, 0.1, 200 );


        if(luzes) {
            //TODO: consertar luzes
            this->init_luzes();

        }


        gluLookAt(
                this->pos[0],
                this->pos[1],
                this->pos[2],

                this->pos[0] + this->look_at[0],
                this->pos[1] + this->look_at[1],
                this->pos[2] + this->look_at[2],

                this->camera_up[0],
                this->camera_up[1],
                this->camera_up[2]
                );

        glMatrixMode(GL_MODELVIEW_MATRIX);


        //DESENHA AQUI
        //==========================================

        //Triangulo rodando
        glPushMatrix();
        glRotatef(alpha, 0, 1, 0);
        alpha += 0.1;

        glBegin(GL_TRIANGLES);
            glColor3b(120, 120, 0);

            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(  0, 1, -10 );
            glVertex3f( -1, 0, -10 );
            glVertex3f(  1, 0, -10 );
        glEnd();

        glPopMatrix();

        // Triangulo parado
        glBegin(GL_TRIANGLES);
            glColor3b(120, 120, 0);

            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(  0, 1, -8 );
            glVertex3f( -1, 0, -8 );
            glVertex3f(  1, 0, -8 );
        glEnd();


        // PIPEEEEEEES
        glPushMatrix();
            //glTranslatef(0, 10, 0);
            glScalef(var_escala, var_escala, var_escala);
            this->pipes->att();
            this->pipes->render(var_tamanho, usa_textura);
        glPopMatrix();



        // Chão
        //------------------------------------------

        glPushMatrix();
        glBegin(GL_POLYGON);
            glColor3b(51, 51, 51);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(  50, -5, -50 );
            glVertex3f(  50, -5,  50 );
            glVertex3f( -50, -5,  50 );
            glVertex3f( -50, -5, -50 );
        glEnd();
        glPopMatrix();


        //==========================================
        //==========================================
        //==========================================


        glFlush();
        glfwSwapBuffers(this->janela);
        glfwPollEvents();
    }
    glfwTerminate();

}


void Render::init() {

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    this->janela = glfwCreateWindow(800, 600, "Alooo", nullptr, nullptr);

    if (this->janela == nullptr) {
        std::cout << "Falhou para abrir janela GLFW" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSetInputMode(this->janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(this->janela);

    glClearColor (0.0, 0.0, 0.0, 0.0);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if(luzes) {
        this->init_luzes();
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);


}


void Render::init_luzes() {

GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
 GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};    // "cor"
 GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
 GLfloat posicaoLuz[4]={0.0, 50.0, 0.0, 1.0};

 // Capacidade de brilho do material
 GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
 GLint especMaterial = 60;


 // Habilita o modelo de colorização de Gouraud
 glShadeModel(GL_SMOOTH);

 // Define a refletância do material 
 glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
 // Define a concentração do brilho
 glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

 // Ativa o uso da luz ambiente 
 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

 // Define os parâmetros da luz de número 0
 glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
 glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
 glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
 glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

 // Habilita a definição da cor do material a partir da cor corrente
 glEnable(GL_COLOR_MATERIAL);
 //Habilita o uso de iluminação
 glEnable(GL_LIGHTING);
 // Habilita a luz de número 0
 glEnable(GL_LIGHT0);



}


void Render::add_obj(Objeto* o) {

    this->objetos.push_back(o);

}


void Render::render() {

    this->init();
    this->display();

}
