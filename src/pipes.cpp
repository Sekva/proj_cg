#include "../include/pipes.hpp"

#include <iostream>


#define dir_min CIMA
#define dir_max ESQUERDA


#define rand_att

#ifdef rand_att
    #define vel_att 10
#else
    #define vel_att 110
#endif

#define espera_att 250


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

    this->conectar = true; // conectar cubos

    this->tamanho_cubo = 0.1;

    this->pos.reserve(num_pipes);

    this->mundo.reserve(this->dim);
    this->memoria.reserve(this->dim);

    for(int i = 0; i < this->dim; i++) {
        this->mundo[i].reserve(this->dim);
        this->memoria[i].reserve(this->dim);
        for(int j = 0; j < this->dim; j++) {
            this->mundo[i][j].reserve(this->dim);
            this->memoria[i][j].reserve(this->dim);
        }
    }

    this->resetar();


}

void Pipes::resetar() {

    for(int i = 0; i < this->dim; i++) {
        for(int j = 0; j < this->dim; j++) {
            for(int k = 0; k < this->dim; k++) {
                this->mundo[i][j][k] = 0;
                this->memoria[i][j][k] = 0;
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
    this->contador++;
    if (this->contador < espera_att|| this->contador % vel_att != 0) { return; }


    //if(random(1, 10000)< 10) { this->resetar(); return; }



    //TODO: otimizar se pode andar, por bool?

#ifdef rand_att
    int i = random(0, this->num_pipes - 1);
#else
    for(int i = 0; i < this->num_pipes; i++) {
#endif
        double x = this->pos[i].x;
        double y = this->pos[i].y;
        double z = this->pos[i].z;


        // procurar uma direção
        for(int j = 0; j < dir_max; j++) {

            int dir = random(1, 6);

            double nx = x;
            double ny = y;
            double nz = z;

            switch(dir) {
                case 1: // CIMA
                    ny += 1;
                    goto dir_set;
                case 2: // BAIXO
                    ny -= 1;
                    goto dir_set;
                case 3: // FRENTE
                    nz -= 1;
                    goto dir_set;
                case 4: // TRAS
                    nz += 1;
                    goto dir_set;
                case 5: // DIREITA
                    nx += 1;
                    goto dir_set;
                case 6: // ESQUERDA
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

            if(this->mundo[nx][ny][nz] == 0) {
                this->mundo[nx][ny][nz] = i+1; // seta o numero do cano
                this->memoria[nx][ny][nz] = dir; // seta a direção tomada pra chegar aqui

                this->pos[i].x = nx;
                this->pos[i].y = ny;
                this->pos[i].z = nz;
                goto prox_cano;
            }
        }

    prox_cano:
#ifdef rand_att
        return;
#else
        continue;
        }
#endif

}


void Pipes::render() {

    for(int i = 0; i < this->dim; i++) {
        for(int j = 0; j < this->dim; j++) {
            for(int k = 0; k < this->dim; k++) {

                int num_cano = this->mundo[i][j][k];
                if(num_cano != 0) {

                    double dt = this->tamanho_cubo;

                    // FRONT
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i + dt, j + dt, k + dt);
                        glVertex3f( i - dt, j + dt, k + dt);
                        glVertex3f( i - dt, j - dt, k + dt);
                        glVertex3f( i + dt, j - dt, k + dt);
                    glEnd();

                    // BACK
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i + dt, j + dt, k - dt);
                        glVertex3f( i - dt, j + dt, k - dt);
                        glVertex3f( i - dt, j - dt, k - dt);
                        glVertex3f( i + dt, j - dt, k - dt);
                    glEnd();

                    // RIGHT
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i + dt, j + dt, k + dt);
                        glVertex3f( i + dt, j - dt, k + dt);
                        glVertex3f( i + dt, j - dt, k - dt);
                        glVertex3f( i + dt, j + dt, k - dt);
                    glEnd();

                    // LEFT
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i - dt, j + dt, k + dt);
                        glVertex3f( i - dt, j - dt, k + dt);
                        glVertex3f( i - dt, j - dt, k - dt);
                        glVertex3f( i - dt, j + dt, k - dt);
                    glEnd();

                    // TOP
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i - dt, j + dt, k + dt);
                        glVertex3f( i + dt, j + dt, k + dt);
                        glVertex3f( i + dt, j + dt, k - dt);
                        glVertex3f( i - dt, j + dt, k - dt);
                    glEnd();

                    // BOTTOM
                    glBegin(GL_POLYGON);
                        this->set_cor(num_cano);
                        glVertex3f( i - dt, j - dt, k + dt);
                        glVertex3f( i + dt, j - dt, k + dt);
                        glVertex3f( i + dt, j - dt, k - dt);
                        glVertex3f( i - dt, j - dt, k - dt);
                    glEnd();


                    //TODO: passar pra uma função a parte
                    if(this->conectar) {

                        int dir = this->memoria[i][j][k];
                        if (dir == 0) { continue; }

                        switch(dir) {
                            case 1: // CIMA

                                // FRENTE
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   - dt, k - dt);
                                    glVertex3f(i - dt, j-1 + dt, k - dt);
                                    glVertex3f(i + dt, j-1 + dt, k - dt);
                                    glVertex3f(i + dt, j   - dt, k - dt);
                                glEnd();

                                // ESQUERDA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   - dt, k - dt);
                                    glVertex3f(i - dt, j   - dt, k + dt);
                                    glVertex3f(i - dt, j-1 + dt, k + dt);
                                    glVertex3f(i - dt, j-1 + dt, k - dt);
                                glEnd();

                                // TRAS
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   - dt, k + dt);
                                    glVertex3f(i - dt, j-1 + dt, k + dt);
                                    glVertex3f(i + dt, j-1 + dt, k + dt);
                                    glVertex3f(i + dt, j   - dt, k + dt);
                                glEnd();

                                // DIREITA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i + dt, j   - dt, k - dt);
                                    glVertex3f(i + dt, j   - dt, k + dt);
                                    glVertex3f(i + dt, j-1 + dt, k + dt);
                                    glVertex3f(i + dt, j-1 + dt, k - dt);
                                glEnd();

                                break;
                            case 2: // BAIXO

                                // FRENTE
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k - dt);
                                    glVertex3f(i + dt, j   + dt, k - dt);
                                    glVertex3f(i + dt, j+1 - dt, k - dt);
                                    glVertex3f(i - dt, j+1 - dt, k - dt);
                                glEnd();

                                // ESQUERDA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k - dt);
                                    glVertex3f(i - dt, j+1 - dt, k - dt);
                                    glVertex3f(i - dt, j+1 - dt, k + dt);
                                    glVertex3f(i - dt, j   + dt, k + dt);
                                glEnd();

                                // TRAS
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k + dt);
                                    glVertex3f(i + dt, j   + dt, k + dt);
                                    glVertex3f(i + dt, j+1 - dt, k + dt);
                                    glVertex3f(i - dt, j+1 - dt, k + dt);
                                glEnd();

                                // DIREITA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i + dt, j   + dt, k - dt);
                                    glVertex3f(i + dt, j+1 - dt, k - dt);
                                    glVertex3f(i + dt, j+1 - dt, k + dt);
                                    glVertex3f(i + dt, j   + dt, k + dt);
                                glEnd();

                                break;
                            case 3: // FRENTE

                                // CIMA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k   + dt);
                                    glVertex3f(i - dt, j   + dt, k+1 - dt);
                                    glVertex3f(i + dt, j   + dt, k+1 - dt);
                                    glVertex3f(i + dt, j   + dt, k   + dt);
                                glEnd();

                                // ESQUERDA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k   + dt);
                                    glVertex3f(i - dt, j   + dt, k+1 - dt);
                                    glVertex3f(i - dt, j   - dt, k+1 - dt);
                                    glVertex3f(i - dt, j   - dt, k   + dt);
                                glEnd();

                                // BAIXO
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   - dt, k   + dt);
                                    glVertex3f(i - dt, j   - dt, k+1 - dt);
                                    glVertex3f(i + dt, j   - dt, k+1 - dt);
                                    glVertex3f(i + dt, j   - dt, k   + dt);
                                glEnd();

                                // DIREITA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i + dt, j   + dt, k   + dt);
                                    glVertex3f(i + dt, j   + dt, k+1 - dt);
                                    glVertex3f(i + dt, j   - dt, k+1 - dt);
                                    glVertex3f(i + dt, j   - dt, k   + dt);
                                glEnd();

                                break;
                            case 4: // TRAS

                                // CIMA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k   - dt);
                                    glVertex3f(i - dt, j   + dt, k-1 + dt);
                                    glVertex3f(i + dt, j   + dt, k-1 + dt);
                                    glVertex3f(i + dt, j   + dt, k   - dt);
                                glEnd();

                                // ESQUERDA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   + dt, k   - dt);
                                    glVertex3f(i - dt, j   + dt, k-1 + dt);
                                    glVertex3f(i - dt, j   - dt, k-1 + dt);
                                    glVertex3f(i - dt, j   - dt, k   - dt);
                                glEnd();

                                // BAIXO
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i - dt, j   - dt, k   - dt);
                                    glVertex3f(i - dt, j   - dt, k-1 + dt);
                                    glVertex3f(i + dt, j   - dt, k-1 + dt);
                                    glVertex3f(i + dt, j   - dt, k   - dt);
                                glEnd();

                                // DIREITA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i + dt, j   + dt, k   - dt);
                                    glVertex3f(i + dt, j   + dt, k-1 + dt);
                                    glVertex3f(i + dt, j   - dt, k-1 + dt);
                                    glVertex3f(i + dt, j   - dt, k   - dt);
                                glEnd();

                                break;
                            case 5: // DIREITA

                                // FRENTE
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   - dt, j + dt, k - dt);
                                    glVertex3f(i-1 + dt, j + dt, k - dt);
                                    glVertex3f(i-1 + dt, j - dt, k - dt);
                                    glVertex3f(i   - dt, j - dt, k - dt);
                                glEnd();

                                // CIMA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   - dt, j + dt, k - dt);
                                    glVertex3f(i-1 + dt, j + dt, k - dt);
                                    glVertex3f(i-1 + dt, j + dt, k + dt);
                                    glVertex3f(i   - dt, j + dt, k + dt);
                                glEnd();

                                // TRAS
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   - dt, j + dt, k + dt);
                                    glVertex3f(i-1 + dt, j + dt, k + dt);
                                    glVertex3f(i-1 + dt, j - dt, k + dt);
                                    glVertex3f(i   - dt, j - dt, k + dt);
                                glEnd();

                                // BAIXO
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   - dt, j - dt, k - dt);
                                    glVertex3f(i-1 + dt, j - dt, k - dt);
                                    glVertex3f(i-1 + dt, j - dt, k + dt);
                                    glVertex3f(i   - dt, j - dt, k + dt);
                                glEnd();

                                break;
                            case 6: // ESQUERDA

                                // FRENTE
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   + dt, j + dt, k - dt);
                                    glVertex3f(i+1 - dt, j + dt, k - dt);
                                    glVertex3f(i+1 - dt, j - dt, k - dt);
                                    glVertex3f(i   + dt, j - dt, k - dt);
                                glEnd();

                                // CIMA
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   + dt, j + dt, k - dt);
                                    glVertex3f(i+1 - dt, j + dt, k - dt);
                                    glVertex3f(i+1 - dt, j + dt, k + dt);
                                    glVertex3f(i   + dt, j + dt, k + dt);
                                glEnd();

                                // TRAS
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   + dt, j + dt, k + dt);
                                    glVertex3f(i+1 - dt, j + dt, k + dt);
                                    glVertex3f(i+1 - dt, j - dt, k + dt);
                                    glVertex3f(i   + dt, j - dt, k + dt);
                                glEnd();

                                // BAIXO
                                glBegin(GL_POLYGON);
                                    this->set_cor(num_cano);
                                    glVertex3f(i   + dt, j - dt, k - dt);
                                    glVertex3f(i+1 - dt, j - dt, k - dt);
                                    glVertex3f(i+1 - dt, j - dt, k + dt);
                                    glVertex3f(i   + dt, j - dt, k + dt);
                                glEnd();

                                break;
                        }


                    }



                }

            }
        }
    }

}


void Pipes::set_cor(int cor) {

    glColor3b(2, 2, 2);
    switch(cor) {
        case 1:
            glColor3ub(1, 168, 188);
            break;
        case 2:
            glColor3ub(61, 52, 169);
            break;
        case 3:
            glColor3ub(177, 72, 41);
            break;
        case 4:
            glColor3ub(16, 43, 126);
            break;
        case 5:
            glColor3ub(110, 31, 37);
            break;
        case 6:
            glColor3ub(140, 108, 23);
            break;
        case 7:
            glColor3ub(27, 99, 35);
            break;
        case 8:
            glColor3ub(118, 52, 100);
            break;
    }

}
