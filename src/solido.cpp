#include "../include/solido.hpp"

struct vertice {

    Num x;
    Num y;
    Num z;

    std::vector<Face*> faces;
    uint16_t indice_solido;

    static vertice novo(Num x, Num y) { return vertice::novo(x, y, 0); }

    static vertice novo(Num x, Num y, Num z) {
        Vertice v;
        v.x = x; v.y = y; v.z = z;
        return v;
    }

    void add_face(Face* f) {
        this->faces.push_back(f); // add ponteiro da face no vetor
    }

    void set_indice(uint16_t i) { this->indice_solido = i; }

    void printar() {printf("%LF, %LF, %LF", this->x, this->y, this->z);} // imprime as coordenadas SEM QUEBRA DE LINHA

};


struct face {
    std::vector<Vertice*> vertices;
    uint16_t indice_solido;

    static face nova() { Face f; return f; }

    void add_vertice(Vertice* v) {
        this->vertices.push_back(v);
    }

    void set_indice(uint16_t i) { this->indice_solido = i; }

    void printar() {
        for(auto i : this->vertices) {i->printar(); printf("\n");}
        printf("\n");
    }

    void printar2() {
        std::cout << this->vertices.size() << " ";
        for(auto i : this->vertices) {
            std::cout << i->indice_solido << " ";
        }
        std::cout << "\n";
    }

};


struct face_vertice {
    std::vector<Face> faces;
    std::vector<Vertice> vertices;

    uint16_t indice_vertice = 0;
    uint16_t indice_face = 0;

    void add_vertice(Vertice v) {
        v.set_indice(this->indice_vertice);
        this->indice_vertice += 1;
        this->vertices.push_back(v);
    }

    void add_face(Face f) {
        f.set_indice(this->indice_face);
        this->indice_face += 1;
        this->faces.push_back(f);
    }

    Vertice* vertice_em (uint16_t i) { return &this->vertices[i]; }
    Face* face_em (uint16_t i) { return &this->faces[i]; }

};



