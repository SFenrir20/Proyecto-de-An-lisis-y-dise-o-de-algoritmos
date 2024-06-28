#ifndef CIUDADANO_H
#define CIUDADANO_H

#include <cstring>

struct Direccion {
    char departamento[30];
    char provincia[30];
    char ciudad[30];
    char distrito[30];
    char ubicacion[50];
};

struct Ciudadano {
    char dni[9];
    char nombres_apellidos[50];
    char nacionalidad[20];
    char lugar_nacimiento[50];
    Direccion direccion;
    char telefono[15];
    char email[30];
    char estado_civil[15];
};

#endif // CIUDADANO_H
