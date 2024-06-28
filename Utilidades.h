#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "Ciudadano.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>

std::string generarDniAleatorio();
Ciudadano crearCiudadanoSimulado();

std::string generarDniAleatorio() {
    std::string dni = std::to_string(rand() % 90000000 + 10000000);
    return dni;
}

Ciudadano crearCiudadanoSimulado() {
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

#endif // UTILIDADES_H

