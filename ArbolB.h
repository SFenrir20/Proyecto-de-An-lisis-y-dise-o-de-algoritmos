#ifndef ARBOLB_H
#define ARBOLB_H

#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <queue>
#include "Ciudadano.h"
#include "Nodo.h"

class ArbolB {
private:
    Nodo* raiz;
    int grado;

public:
    ArbolB(int grado);
    ~ArbolB();

    void insertar(const Ciudadano& ciudadano);
    Ciudadano* buscar(const char* dni);
    void eliminar(const char* dni);

    template <typename Stream>
    void guardar(Stream& os, Nodo* nodo);

    template <typename Stream>
    void cargar(Stream& is, Nodo* nodo);

    Nodo* getRaiz() { return raiz; }
};

ArbolB::ArbolB(int grado) : raiz(nullptr), grado(grado) {}

ArbolB::~ArbolB() {
    // Liberar memoria de todos los nodos
    std::queue<Nodo*> nodos;
    if (raiz) nodos.push(raiz);
    while (!nodos.empty()) {
        Nodo* nodo = nodos.front();
        nodos.pop();
        for (auto hijo : nodo->hijos) {
            if (hijo) nodos.push(hijo);
        }
        delete nodo;
    }
}

void ArbolB::insertar(const Ciudadano& ciudadano) {
    if (!raiz) {
        raiz = new Nodo(grado, true);
        raiz->claves.push_back(ciudadano);
        std::cout << "Insertado en nueva raíz: " << ciudadano.dni << std::endl;
    } else {
        if (raiz->claves.size() == 2 * grado - 1) {
            Nodo* s = new Nodo(grado, false);
            s->hijos.push_back(raiz);
            s->dividirHijo(0, raiz);
            int i = 0;
            if (s->claves[0].dni < ciudadano.dni) i++;
            s->hijos[i]->insertarNoLleno(ciudadano);
            raiz = s;
            std::cout << "Dividida raíz e insertado: " << ciudadano.dni << std::endl;
        } else {
            raiz->insertarNoLleno(ciudadano);
           // std::cout << "Insertado en raíz existente: " << ciudadano.dni << std::endl;
        }
    }
}

Ciudadano* ArbolB::buscar(const char* dni) {
    return (raiz == nullptr) ? nullptr : raiz->buscar(dni);
}

void ArbolB::eliminar(const char* dni) {
    if (!raiz) return;
    raiz->eliminar(dni);
    if (raiz->claves.empty()) {
        Nodo* temp = raiz;
        if (raiz->hoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete temp;
    }
}

template <typename Stream>
void ArbolB::guardar(Stream& os, Nodo* nodo) {
    if (!nodo) return;
    nodo->guardar(os);
}

template <typename Stream>
void ArbolB::cargar(Stream& is, Nodo* nodo) {
    if (!nodo) {
        nodo = new Nodo(grado, true);
    }
    nodo->cargar(is);
    raiz = nodo;
}

// Instanciación de las plantillas
template void ArbolB::guardar(std::ofstream& os, Nodo* nodo);
template void ArbolB::guardar(std::ostringstream& os, Nodo* nodo);
template void ArbolB::cargar(std::ifstream& is, Nodo* nodo);
template void ArbolB::cargar(std::istringstream& is, Nodo* nodo);

#endif // ARBOLB_H
