#include <iostream>
#include "RegistroCiudadanos.h"

void mostrarMenu() {
    std::cout << "===========================" << std::endl;
    std::cout << "1. Buscar ciudadano por DNI" << std::endl;
    std::cout << "2. Insertar nuevo ciudadano" << std::endl;
    std::cout << "3. Eliminar ciudadano por DNI" << std::endl;
    std::cout << "4. Imprimir tres DNIs aleatorios" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Seleccione una opción: ";
}

int main() {
    int grado = 40;  // Ejemplo de grado para el árbol B
    RegistroCiudadanos registro(grado);
    
    registro.cargarSoloDNIs();

    int opcion;
    std::string dni;

    do {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "Ingrese el DNI a buscar: ";
                std::cin >> dni;
                registro.buscar(dni);
                break;
            case 2:
                registro.insertar();
                std::cout << "Nuevo ciudadano insertado y datos guardados." << std::endl;
                break;
            case 3:
                std::cout << "Ingrese el DNI a eliminar: ";
                std::cin >> dni;
                registro.eliminar(dni);
                std::cout << "Ciudadano eliminado y datos guardados." << std::endl;
                break;
            case 4:
                registro.imprimirDniAleatorios();
                break;
            case 5:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida. Intente nuevamente." << std::endl;
        }
    } while (opcion != 5);

    return 0;
}
