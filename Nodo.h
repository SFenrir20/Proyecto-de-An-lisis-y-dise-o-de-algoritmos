#ifndef NODO_H
#define NODO_H

#include <vector>
#include <iostream>
#include "Ciudadano.h"

class Nodo {
public:
    std::vector<Ciudadano> claves;
    std::vector<Nodo*> hijos;
    bool hoja;
    int grado;

    Nodo(int grado, bool hoja);

    void insertarNoLleno(const Ciudadano& ciudadano);
    void dividirHijo(int i, Nodo* y);
    Ciudadano* buscar(const char* dni);
    void eliminar(const char* dni);
    void eliminarNoHoja(int idx);
    Ciudadano obtenerPredecesor(int idx);
    Ciudadano obtenerSucesor(int idx);
    void llenar(int idx);
    void prestarDeAnterior(int idx);
    void prestarDeSiguiente(int idx);
    void unir(int idx);
    int findKey(const char* dni);

    template <typename Stream>
    void guardar(Stream& os);

    template <typename Stream>
    void cargar(Stream& is);
};


Nodo::Nodo(int grado, bool hoja) : grado(grado), hoja(hoja) {}

void Nodo::insertarNoLleno(const Ciudadano& ciudadano) {
    int i = claves.size() - 1;
    if (hoja) {
        claves.push_back(ciudadano);
        while (i >= 0 && claves[i].dni > ciudadano.dni) {
            claves[i + 1] = claves[i];
            i--;
        }
        claves[i + 1] = ciudadano;
    } else {
        while (i >= 0 && claves[i].dni > ciudadano.dni) {
            i--;
        }
        if (hijos[i + 1]->claves.size() == 2 * grado - 1) {
            dividirHijo(i + 1, hijos[i + 1]);
            if (claves[i + 1].dni < ciudadano.dni) {
                i++;
            }
        }
        hijos[i + 1]->insertarNoLleno(ciudadano);
    }
}

void Nodo::dividirHijo(int i, Nodo* y) {
    Nodo* z = new Nodo(y->grado, y->hoja);
    for (int j = 0; j < grado - 1; j++) {
        z->claves.push_back(y->claves[j + grado]);
    }
    if (!y->hoja) {
        for (int j = 0; j < grado; j++) {
            z->hijos.push_back(y->hijos[j + grado]);
        }
    }
    y->claves.resize(grado - 1);
    hijos.insert(hijos.begin() + i + 1, z);
    claves.insert(claves.begin() + i, y->claves[grado - 1]);
    y->claves.resize(grado - 1);
}

Ciudadano* Nodo::buscar(const char* dni) {
    int i = 0;
    while (i < claves.size() && std::strcmp(dni, claves[i].dni) > 0) {
        i++;
    }
    if (i < claves.size() && std::strcmp(dni, claves[i].dni) == 0) {
        return &claves[i];
    }
    if (hoja) {
        return nullptr;
    }
    return hijos[i]->buscar(dni);
}

void Nodo::eliminar(const char* dni) {
    int idx = findKey(dni);
    if (idx < claves.size() && std::strcmp(claves[idx].dni, dni) == 0) {
        if (hoja) {
            claves.erase(claves.begin() + idx);
        } else {
            eliminarNoHoja(idx);
        }
    } else {
        if (hoja) return;
        bool flag = ((idx == claves.size()) ? true : false);
        if (hijos[idx]->claves.size() < grado) {
            llenar(idx);
        }
        if (flag && idx > claves.size()) {
            hijos[idx - 1]->eliminar(dni);
        } else {
            hijos[idx]->eliminar(dni);
        }
    }
}

void Nodo::eliminarNoHoja(int idx) {
    Ciudadano k = claves[idx];
    if (hijos[idx]->claves.size() >= grado) {
        Ciudadano pred = obtenerPredecesor(idx);
        claves[idx] = pred;
        hijos[idx]->eliminar(pred.dni);
    } else if (hijos[idx + 1]->claves.size() >= grado) {
        Ciudadano succ = obtenerSucesor(idx);
        claves[idx] = succ;
        hijos[idx + 1]->eliminar(succ.dni);
    } else {
        unir(idx);
        hijos[idx]->eliminar(k.dni);
    }
}

Ciudadano Nodo::obtenerPredecesor(int idx) {
    Nodo* cur = hijos[idx];
    while (!cur->hoja) {
        cur = cur->hijos[cur->claves.size()];
    }
    return cur->claves[cur->claves.size() - 1];
}

Ciudadano Nodo::obtenerSucesor(int idx) {
    Nodo* cur = hijos[idx + 1];
    while (!cur->hoja) {
        cur = cur->hijos[0];
    }
    return cur->claves[0];
}

void Nodo::llenar(int idx) {
    if (idx != 0 && hijos[idx - 1]->claves.size() >= grado) {
        prestarDeAnterior(idx);
    } else if (idx != claves.size() && hijos[idx + 1]->claves.size() >= grado) {
        prestarDeSiguiente(idx);
    } else {
        if (idx != claves.size()) {
            unir(idx);
        } else {
            unir(idx - 1);
        }
    }
}

void Nodo::prestarDeAnterior(int idx) {
    Nodo* hijo = hijos[idx];
    Nodo* hermano = hijos[idx - 1];
    for (int i = hijo->claves.size() - 1; i >= 0; --i) {
        hijo->claves[i + 1] = hijo->claves[i];
    }
    if (!hijo->hoja) {
        for (int i = hijo->hijos.size() - 1; i >= 0; --i) {
            hijo->hijos[i + 1] = hijo->hijos[i];
        }
    }
    hijo->claves[0] = claves[idx - 1];
    if (!hijo->hoja) {
        hijo->hijos[0] = hermano->hijos[hermano->claves.size()];
    }
    claves[idx - 1] = hermano->claves[hermano->claves.size() - 1];
    hijo->claves.resize(hijo->claves.size() + 1);
    hermano->claves.resize(hermano->claves.size() - 1);
}

void Nodo::prestarDeSiguiente(int idx) {
    Nodo* hijo = hijos[idx];
    Nodo* hermano = hijos[idx + 1];
    hijo->claves[hijo->claves.size()] = claves[idx];
    if (!hijo->hoja) {
        hijo->hijos[hijo->claves.size() + 1] = hermano->hijos[0];
    }
    claves[idx] = hermano->claves[0];
    for (int i = 1; i < hermano->claves.size(); ++i) {
        hermano->claves[i - 1] = hermano->claves[i];
    }
    if (!hermano->hoja) {
        for (int i = 1; i <= hermano->claves.size(); ++i) {
            hermano->hijos[i - 1] = hermano->hijos[i];
        }
    }
    hijo->claves.resize(hijo->claves.size() + 1);
    hermano->claves.resize(hermano->claves.size() - 1);
}

void Nodo::unir(int idx) {
    Nodo* hijo = hijos[idx];
    Nodo* hermano = hijos[idx + 1];
    hijo->claves[grado - 1] = claves[idx];
    for (int i = 0; i < hermano->claves.size(); ++i) {
        hijo->claves[i + grado] = hermano->claves[i];
    }
    if (!hijo->hoja) {
        for (int i = 0; i <= hermano->claves.size(); ++i) {
            hijo->hijos[i + grado] = hermano->hijos[i];
        }
    }
    for (int i = idx + 1; i < claves.size(); ++i) {
        claves[i - 1] = claves[i];
    }
    for (int i = idx + 2; i <= claves.size(); ++i) {
        hijos[i - 1] = hijos[i];
    }
    hijo->claves.resize(hijo->claves.size() + hermano->claves.size() + 1);
    claves.resize(claves.size() - 1);
    hijos.resize(hijos.size() - 1);
    delete hermano;
}

int Nodo::findKey(const char* dni) {
    int idx = 0;
    while (idx < claves.size() && std::strcmp(claves[idx].dni, dni) < 0) {
        ++idx;
    }
    return idx;
}

template <typename Stream>
void Nodo::guardar(Stream& os) {
    os.write(reinterpret_cast<char*>(&hoja), sizeof(hoja));
    int numClaves = claves.size();
    os.write(reinterpret_cast<char*>(&numClaves), sizeof(numClaves));

    for (const auto& clave : claves) {
        os.write(reinterpret_cast<const char*>(&clave), sizeof(Ciudadano));
    }

    int numHijos = hijos.size();
    os.write(reinterpret_cast<char*>(&numHijos), sizeof(numHijos));
    
    for (const auto& hijo : hijos) {
        bool hijoEsNulo = (hijo == nullptr);
        os.write(reinterpret_cast<char*>(&hijoEsNulo), sizeof(hijoEsNulo));
        if (!hijoEsNulo) {
            hijo->guardar(os);
        }
    }
}

template <typename Stream>
void Nodo::cargar(Stream& is) {
    is.read(reinterpret_cast<char*>(&hoja), sizeof(hoja));
    int numClaves;
    is.read(reinterpret_cast<char*>(&numClaves), sizeof(numClaves));

    claves.resize(numClaves);
    for (int i = 0; i < numClaves; ++i) {
        is.read(reinterpret_cast<char*>(&claves[i]), sizeof(Ciudadano));
    }

    int numHijos;
    is.read(reinterpret_cast<char*>(&numHijos), sizeof(numHijos));
    
    hijos.resize(numHijos);
    for (int i = 0; i < numHijos; ++i) {
        bool hijoEsNulo;
        is.read(reinterpret_cast<char*>(&hijoEsNulo), sizeof(hijoEsNulo));
        if (!hijoEsNulo) {
            hijos[i] = new Nodo(grado, true);
            hijos[i]->cargar(is);
        } else {
            hijos[i] = nullptr;
        }
    }
}

// Instanciación de las plantillas
template void Nodo::guardar(std::ofstream& os);
template void Nodo::guardar(std::ostringstream& os);
template void Nodo::cargar(std::ifstream& is);
template void Nodo::cargar(std::istringstream& is);
#endif // NODO_H
