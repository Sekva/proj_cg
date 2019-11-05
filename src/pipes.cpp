#include "../include/pipes.hpp"

#include <iostream>


#define dir_min CIMA
#define dir_max ESQUERDA


int random(int min, int max) {
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_int_distribution<int> unif(min, max);
    return unif(rng);
}


Pipes::Pipes(int num_pipes, int dim) {


    this->contador = 0;
    this->num_pipes = num_pipes;
    this->dim = dim;

    this->tamanho_cubo = 0.4;

    this->pos.reserve(num_pipes);

    this->mundo.reserve(this->dim);

    for(int i = 0; i < this->dim; i++) {
        this->mundo[i].reserve(this->dim);
        for(int j = 0; j < this->dim; j++) {
            this->mundo[i][j].reserve(this->dim);
        }
    }


    for(int i = 0; i < this->dim; i++) {
        for(int j = 0; j < this->dim; j++) {
            for(int k = 0; k < this->dim; k++) {
                this->mundo[i][j][k] = 0;
            }
        }
    }



    for(int i = 1; i <= this->num_pipes; i++) {

        // Procurando um espaço vago
        bool vago = false;
        while(!vago) {

            //Gerando posições aleatorias
            this->pos[i-1].x = random(0, this->dim-1);
            this->pos[i-1].y = random(0, this->dim-1);
            this->pos[i-1].z = random(0, this->dim-1);

            if(this->mundo[this->pos[i-1].x][this->pos[i-1].y][this->pos[i-1].z] == 0) {
                vago = true;
            }

        }
        //Setando o primeiro cubo
        this->mundo[this->pos[i-1].x][this->pos[i-1].y][this->pos[i-1].z] = i;
    }

}


void Pipes::att() {
    if (this->contador > 0) { return; }
    this->contador++;
    this->contador %= 100;


    //TODO: otimizar, por bool?
    for(int i = 0; i < this->num_pipes; i++) {
        double x = this->pos[i].x;
        double y = this->pos[i].y;
        double z = this->pos[i].z;


        // procurar uma direção
        for(int j = 0; j < dir_max; j++) {
            int dir = random(dir_min, dir_max);

            double nx = x;
            double ny = y;
            double nz = z;

            switch((DIR) dir) {
                case CIMA:
                    ny += 1;
                    goto dir_set;
                case BAIXO:
                    ny -= 1;
                    goto dir_set;
                case FRENTE:
                    nz += 1;
                    goto dir_set;
                case TRAS:
                    nz -= 1;
                    goto dir_set;
                case DIREITA:
                    nx += 1;
                    goto dir_set;
                case ESQUERDA:
                    nx -= 1;
                    goto dir_set;
                default:
                    goto prox_cano;
            }

            dir_set:
            if (nx < 0)          {goto prox_cano;}
            if (nx >= this->dim) {goto prox_cano;}
            if (ny < 0)          {goto prox_cano;}
            if (ny >= this->dim) {goto prox_cano;}
            if (nz < 0)          {goto prox_cano;}
            if (nz >= this->dim) {goto prox_cano;}


            std::cout << "nx " << nx << "\n";
            std::cout << "ny " << ny << "\n";
            std::cout << "nz " << nz << "\n";
            std::cout << "\n";

            if(this->mundo[nx][ny][nz] == 0) {
                this->mundo[nx][ny][nz] = i+1; // seta o numero do cano
                this->pos[i].x = nx;
                this->pos[i].y = ny;
                this->pos[i].z = nz;
                goto prox_cano;
            }
        }

    prox_cano:
        continue;

    }

}


void Pipes::render() {

    for(int i = 0; i < this->dim; i++) {
        for(int j = 0; j < this->dim; j++) {
            for(int k = 0; k < this->dim; k++) {

                int num_cano = this->mundo[i][k][j];
                if(num_cano != 0) {

                    double dx = this->tamanho_cubo;


                    // FRONT
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);

                        glVertex3f( i + dx, j + dx, k + dx);
                        glVertex3f( i - dx, j + dx, k + dx);
                        glVertex3f( i - dx, j - dx, k + dx);
                        glVertex3f( i + dx, j - dx, k + dx);

                    glEnd();

                    // BACK
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i + dx, j + dx, k - dx);
                        glVertex3f( i - dx, j + dx, k - dx);
                        glVertex3f( i - dx, j - dx, k - dx);
                        glVertex3f( i + dx, j - dx, k - dx);
                    glEnd();

                    // RIGHT
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i + dx, j + dx, k + dx);
                        glVertex3f( i + dx, j - dx, k + dx);
                        glVertex3f( i + dx, j - dx, k - dx);
                        glVertex3f( i + dx, j + dx, k - dx);
                    glEnd();

                    // LEFT
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i - dx, j + dx, k + dx);
                        glVertex3f( i - dx, j - dx, k + dx);
                        glVertex3f( i - dx, j - dx, k - dx);
                        glVertex3f( i - dx, j + dx, k - dx);
                    glEnd();

                    // TOP
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i - dx, j + dx, k + dx);
                        glVertex3f( i + dx, j + dx, k + dx);
                        glVertex3f( i + dx, j + dx, k - dx);
                        glVertex3f( i - dx, j + dx, k - dx);
                    glEnd();

                    // BOTTOM
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i - dx, j - dx, k + dx);
                        glVertex3f( i + dx, j - dx, k + dx);
                        glVertex3f( i + dx, j - dx, k - dx);
                        glVertex3f( i - dx, j - dx, k - dx);
                    glEnd();


                    this->tentar_conectar(i, j, k, num_cano);


                }

            }
        }
    }

}



void Pipes::tentar_conectar(int x, int y, int z, int numero_cano) {


    //TODO: vai desenhar duas vezes por agr


    if(this->mundo[x][y][z+1] == numero_cano) {

        glBegin(GL_POLYGON);
            this->set_cor(numero_cano);
        glEnd();

        glBegin(GL_POLYGON);
            this->set_cor(numero_cano);
        glEnd();

        glBegin(GL_POLYGON);
            this->set_cor(numero_cano);
        glEnd();

        glBegin(GL_POLYGON);
            this->set_cor(numero_cano);
        glEnd();
    }

    if(this->mundo[x][y][z-1] == numero_cano) {}

    if(this->mundo[x][y+1][z] == numero_cano) {}

    if(this->mundo[x][y-1][z] == numero_cano) {}

    if(this->mundo[x+1][y][z] == numero_cano) {}

    if(this->mundo[x-1][y][z] == numero_cano) {}





}



void Pipes::set_cor(int cor) {


    glColor3b(2, 2, 2);
    switch(cor) {
        case 1:
            glColor3b(1/2, 168/2, 188/2);
            break;
        case 2:
            glColor3b(61/2, 52/2, 169/2);
            break;
        case 3:
            glColor3b(177/2, 72/2, 41/2);
            break;
    }



}
