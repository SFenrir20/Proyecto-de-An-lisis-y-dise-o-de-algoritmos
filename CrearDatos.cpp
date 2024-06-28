#include "RegistroCiudadanos.h"

int main() {
    int grado = 40;  // Ejemplo de grado para el árbol B
    RegistroCiudadanos registro(grado);
    
    int cantidadRegistros = 33000000;  // Número de registros a generar
    registro.generarRegistros(cantidadRegistros);

    return 0;
}