#include <math.h>
#include <stdio.h>

#define MAX_PAREDES 50
#define EPSILON     1e-6f
#define PI          3.14159265f

//Segmento de pared entre los puntos A(x1, y1) y B(x2, y2).
// TODO: explicar lo que es un struct
struct Pared {
    float x1;
    float y1;
    float x2;
    float y2;
};

struct Pared paredes[MAX_PAREDES];
int num_paredes = 0;

// Estado del robot (0 rad = mirando a la derecha).
// Este es el estado inicial y lo he cogido de tu código. Para esta semana es suficiente.
float robot_x   = 5.0f;
float robot_y   = 5.0f;
float robot_phi = 1.57f;

/**
 * Distancia desde el sensor hasta una pared concreta.
 * @param  px, py  Posicion del sensor.
 * @param  phi     Orientacion del sensor en radianes.
 * @param  pared   Pared a comprobar.
 * @return mu (distancia) o INFINITY si no hay choque: rayo paralelo,
 *         pared a la espalda del sensor o punto de corte fuera del segmento.
 */
float distancia_pared(float px, float py, float phi,
                             const struct Pared *pared)
{
    // este codigo es codigo avanzado de structs y punteros....
    const float c  = cosf(phi);
    const float s  = sinf(phi);
    const float dx = pared->x2 - pared->x1;   // componentes de B - A
    const float dy = pared->y2 - pared->y1;
    const float rx = pared->x1 - px;          // componentes de A - P
    const float ry = pared->y1 - py;

    const float det = dx * s - dy * c;
    // explicar MACROS y funcion fabsf
    if (fabsf(det) < EPSILON) {
        return INFINITY;
    }

    // sacado de tu pizzarra, resumen, etc
    const float mu     = (dx * ry - dy * rx) / det;
    const float lambda = (c * ry - s * rx) / det;

    if (mu <= 0.0f || lambda < 0.0f || lambda > 1.0f) {
        return INFINITY;
    }
    return mu;
}

/**
 * Simula el sensor: devuelve la distancia a la pared mas cercana.
 * @return Distancia minima o INFINITY si el sensor no apunta a ninguna pared.
 */
float percepcion(float px, float py, float phi)
{
    float minima = INFINITY;

    for (int i = 0; i < num_paredes; i++) {
        const float d = distancia_pared(px, py, phi, &paredes[i]);
        if (d < minima) {
            minima = d;
        }
    }
    return minima;
}

/**
 * Lee las paredes desde un fichero de texto (x1 y1 x2 y2 por linea).
 * @return Numero de paredes leidas o -1 si no se pudo abrir el fichero.
 */
int leer_paredes(const char *nombre_fichero)
{
    FILE *f = fopen(nombre_fichero, "r");
    if (f == NULL) {
        fprintf(stderr, "Error al abrir el fichero %s\n", nombre_fichero);
        return -1;
    }

    // misma idea... codigo complejo de structs...
    num_paredes = 0;
    while (num_paredes < MAX_PAREDES &&
           fscanf(f, "%f %f %f %f",
                  &paredes[num_paredes].x1, &paredes[num_paredes].y1,
                  &paredes[num_paredes].x2, &paredes[num_paredes].y2) == 4) {
        num_paredes++;
    }

    fclose(f);
    return num_paredes;
}

/**
 * Barre 360 grados con el sensor y guarda los puntos de choque.
 * Formato de cada linea: x_robot y_robot x_choque y_choque
 * @return 0 si todo fue bien, -1 si no se pudo abrir el fichero.
 */
//TODO modificar lo de los 360 grados pq igual al profe no le va.
static int guardar_barrido(const char *nombre_fichero)
{
    FILE *f = fopen(nombre_fichero, "w");
    if (f == NULL) {
        fprintf(stderr, "Error al abrir el fichero %s\n", nombre_fichero);
        return -1;
    }

    for (int grados = 0; grados < 360; grados++) {
        const float phi = (float)grados * PI / 180.0f;
        const float d   = percepcion(robot_x, robot_y, phi);

        if (isinf(d)) {
            continue;   // sin pared en esa direccion: no se dibuja
        }
        fprintf(f, "%.3f %.3f %.3f %.3f\n", robot_x, robot_y,
                robot_x + d * cosf(phi), robot_y + d * sinf(phi));
    }

    fclose(f);
    return 0;
}

int main(void)
{
    static const float angulos[] = {0.0f, 1.5708f, 3.1416f, 4.7124f};
    const int num_angulos = (int)(sizeof(angulos) / sizeof(angulos[0]));

    if (leer_paredes("Labyrinth.txt") < 0) {
        return 1;
    }
    printf("Paredes cargadas: %d\n", num_paredes);

    // Distancia en la direccion actual del robot
    printf("Robot (phi = %.2f rad) -> distancia = %.3f\n",
           robot_phi, percepcion(robot_x, robot_y, robot_phi));

    for (int i = 0; i < num_angulos; i++) {
        const float d = percepcion(robot_x, robot_y, angulos[i]);
        printf("phi = %.4f rad -> distancia = %.3f\n", angulos[i], d);
    }

    if (guardar_barrido("Sensor.txt") < 0) {
        return 1;
    }
    printf("Barrido guardado en Sensor.txt\n");
    return 0;
}