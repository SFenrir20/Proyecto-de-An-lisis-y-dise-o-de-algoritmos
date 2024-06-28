#ifndef REGISTROCIUDADANOS_H
#define REGISTROCIUDADANOS_H

#include "ArbolB.h"
#include <string>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random> 
#include <sstream> // Incluir para std::ostringstream y std::istringstream
#include <stdexcept> // Incluir para std::runtime_error
#include "Compressor.h"
#include <chrono>


class RegistroCiudadanos {
public:
    RegistroCiudadanos(int grado);
    ~RegistroCiudadanos();

    void insertar();
    void buscar(const std::string& dni);
    bool cargarDatosPorDni(const std::string& dni, Ciudadano& ciudadano);
    void eliminar(const std::string& dni);
    void guardarDatos();
    void cargarDatos();
    void cargarSoloDNIs();
    void generarRegistros(int cantidad);
    void imprimirDniAleatorios();

private:
    ArbolB arbol;
    std::unordered_set<std::string> dnis;
    std::string generarDniAleatorio();
    Ciudadano crearCiudadanoSimulado();
    const std::string archivo = "ciudadanos.dat";
};


RegistroCiudadanos::RegistroCiudadanos(int grado) : arbol(grado) {
    // No cargar datos automáticamente
}

RegistroCiudadanos::~RegistroCiudadanos() {
    guardarDatos();
}

std::string RegistroCiudadanos::generarDniAleatorio() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dis(10000000, 99999999);

    std::string dni;
    do {
        dni = std::to_string(dis(gen));
    } while (dnis.find(dni) != dnis.end());

    return dni;
}

Ciudadano RegistroCiudadanos::crearCiudadanoSimulado() {
    Ciudadano c;
    std::string dni = generarDniAleatorio();
    std::strcpy(c.dni, dni.c_str());
    std::strcpy(c.nombres_apellidos, "Nombre Apellido");
    std::strcpy(c.nacionalidad, "Peruano");
    std::strcpy(c.lugar_nacimiento, "Lugar de Nacimiento");
    std::strcpy(c.direccion.departamento, "Departamento");
    std::strcpy(c.direccion.provincia, "Provincia");
    std::strcpy(c.direccion.ciudad, "Ciudad");
    std::strcpy(c.direccion.distrito, "Distrito");
    std::strcpy(c.direccion.ubicacion, "Ubicación");
    std::strcpy(c.telefono, "987654321");
    std::strcpy(c.email, "correo@dominio.com");
    std::strcpy(c.estado_civil, "Soltero");
    return c;
}

void RegistroCiudadanos::insertar() {
    auto start = std::chrono::high_resolution_clock::now();

    Ciudadano c = crearCiudadanoSimulado();
    std::string dni(c.dni);
    if (dnis.find(dni) == dnis.end()) {
        std::ofstream datos(archivo, std::ios::binary | std::ios::app);
        std::streampos pos = datos.tellp();
        datos.write(reinterpret_cast<char*>(&c), sizeof(Ciudadano));
        datos.close();

        std::ofstream indices("indices.dat", std::ios::binary | std::ios::app);
        indices.write(dni.c_str(), sizeof(c.dni));
        indices.write(reinterpret_cast<char*>(&pos), sizeof(pos));
        indices.close();

        dnis.insert(dni);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo de inserción: " << duration.count() << " segundos." << std::endl;
}

void RegistroCiudadanos::buscar(const std::string& dni) {
    auto start = std::chrono::high_resolution_clock::now();

    if (dnis.find(dni) != dnis.end()) {
        Ciudadano c;
        if (cargarDatosPorDni(dni, c)) {
            std::cout << "DNI: " << c.dni << "\nNombre: " << c.nombres_apellidos
                      << "\nNacionalidad: " << c.nacionalidad << "\nLugar de Nacimiento: " << c.lugar_nacimiento
                      << "\nDirección: " << c.direccion.departamento << ", " << c.direccion.provincia << ", " 
                      << c.direccion.ciudad << ", " << c.direccion.distrito << ", " << c.direccion.ubicacion
                      << "\nTeléfono: " << c.telefono << "\nEmail: " << c.email << "\nEstado Civil: " << c.estado_civil << std::endl;
        } else {
            std::cout << "Error al cargar los datos del ciudadano con DNI " << dni << "." << std::endl;
        }
    } else {
        std::cout << "Ciudadano con DNI " << dni << " no encontrado en memoria." << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo de búsqueda: " << duration.count() << " segundos." << std::endl;
}

bool RegistroCiudadanos::cargarDatosPorDni(const std::string& dni, Ciudadano& ciudadano) {
    std::ifstream indices("indices.dat", std::ios::binary);
    if (indices.is_open()) {
        char dniBuffer[9] = {0};
        std::streampos pos;
        while (indices.read(dniBuffer, sizeof(dniBuffer))) {
            indices.read(reinterpret_cast<char*>(&pos), sizeof(pos));
            if (dni == dniBuffer) {
                std::ifstream datos(archivo, std::ios::binary);
                if (datos.is_open()) {
                    datos.seekg(pos);
                    datos.read(reinterpret_cast<char*>(&ciudadano), sizeof(Ciudadano));
                    datos.close();
                    return true;
                }
            }
        }
        indices.close();
    }
    return false;
}

void RegistroCiudadanos::eliminar(const std::string& dni) {
    auto start = std::chrono::high_resolution_clock::now();

    arbol.eliminar(dni.c_str());
    dnis.erase(dni);
    std::cout << "Ciudadano con DNI " << dni << " eliminado." << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo de eliminación: " << duration.count() << " segundos." << std::endl;
}

void RegistroCiudadanos::guardarDatos() {
    std::cout << "Guardando datos..." << std::endl;
    // Este método no se utiliza con el enfoque de archivo de índices
}

void RegistroCiudadanos::cargarDatos() {
    std::cout << "Cargando datos..." << std::endl;
    // Este método no se utiliza con el enfoque de archivo de índices
}

void RegistroCiudadanos::cargarSoloDNIs() {
    std::ifstream indices("indices.dat", std::ios::binary);
    if (indices.is_open()) {
        char dniBuffer[9] = {0};
        std::streampos pos;
        while (indices.read(dniBuffer, sizeof(dniBuffer))) {
            indices.read(reinterpret_cast<char*>(&pos), sizeof(pos));
            dnis.insert(dniBuffer);
        }
        indices.close();
    } else {
        std::cout << "No se encontró el archivo de índices. Empezando con datos vacíos." << std::endl;
    }
}

void RegistroCiudadanos::generarRegistros(int cantidad) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < cantidad; ++i) {
        insertar();
        if (i % 100000 == 0 && i != 0) {
            std::cout << i << " registros generados y guardados." << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total: " << duration.count() << " segundos." << std::endl;
}

void RegistroCiudadanos::imprimirDniAleatorios() {
    if (dnis.size() < 3) {
        std::cout << "No hay suficientes registros para mostrar 3 DNI." << std::endl;
        return;
    }
    
    std::vector<std::string> dniVector(dnis.begin(), dnis.end());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(dniVector.begin(), dniVector.end(), g);
    
    std::cout << "Tres DNI aleatorios:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << dniVector[i] << std::endl;
    }
}

#endif // REGISTROCIUDADANOS_H
