// Einbinden der benötigten Bibliotheken und Definition der globalen Konstanten (z.B. Pi und maximale Wandanzahl).
#include <math.h>
#include <stdio.h>

#define MAX_WALLS 50
#define EPSILON     1e-6f
#define PI          3.14159265f

// Definition der Datenstruktur für Wände sowie Initialisierung der globalen Variablen für den Roboterstatus und den Wandspeicher.
struct Wall {
    float x1;
    float y1;
    float x2;
    float y2;
};

struct Wall walls[MAX_WALLS];
int num_walls = 0;

float robot_x   = 1.5f;
float robot_y   = 4.0f;
float robot_phi = 1.57f;

// Berechnet mathematisch den exakten Schnittpunkt und die Distanz zwischen einem Sensorstrahl und einem einzelnen Wandsegment.
float distance_to_wall(float px, float py, float phi, const struct Wall *wall)
{
    const float c  = cosf(phi);
    const float s  = sinf(phi);
    const float dx = wall->x2 - wall->x1;
    const float dy = wall->y2 - wall->y1;
    const float rx = wall->x1 - px;
    const float ry = wall->y1 - py;

    const float det = dx * s - dy * c;
    
    if (fabsf(det) < EPSILON) {
        return INFINITY;
    }

    const float mu     = (dx * ry - dy * rx) / det;
    const float lambda = (c * ry - s * rx) / det;

    if (mu <= 0.0f || lambda < 0.0f || lambda > 1.0f) {
        return INFINITY;
    }

    return mu;
}

// Simuliert den Sensor, indem alle Wände geprüft werden und die kürzeste Distanz (nächste Wand) zurückgegeben wird.
float perception(float px, float py, float phi)
{
    float min_distance = INFINITY;

    for (int i = 0; i < num_walls; i++) {
        const float d = distance_to_wall(px, py, phi, &walls[i]);

        if (d < min_distance) {
            min_distance = d;
        }
    }

    return min_distance;
}

// Liest die Wandkoordinaten aus einer angegebenen Textdatei ein und speichert sie im globalen Array.
int read_walls(const char *filename)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    num_walls = 0;

    while (num_walls < MAX_WALLS &&
           fscanf(f, "%f %f %f %f",
                  &walls[num_walls].x1, &walls[num_walls].y1,
                  &walls[num_walls].x2, &walls[num_walls].y2) == 4) {
        num_walls++;
    }

    fclose(f);
    return num_walls;
}

// Führt eine Messung für den linken und rechten Sensor durch und speichert die ermittelten Trefferkoordinaten in einer Textdatei.
static int save_scan(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    float winkel_links = 120.0f; 
    const float phi_L = winkel_links * PI / 180.0f;
    const float dist_L = perception(robot_x, robot_y, phi_L);

    float winkel_rechts = 60.0f; 
    const float phi_R = winkel_rechts * PI / 180.0f;
    const float dist_R = perception(robot_x, robot_y, phi_R);

    if (!isinf(dist_L)) {
        fprintf(f, "%.3f %.3f %.3f %.3f\n", robot_x, robot_y,
                robot_x + dist_L * cosf(phi_L), robot_y + dist_L * sinf(phi_L));
    }
    
    if (!isinf(dist_R)) {
        fprintf(f, "%.3f %.3f %.3f %.3f\n", robot_x, robot_y,
                robot_x + dist_R * cosf(phi_R), robot_y + dist_R * sinf(phi_R));
    }

    fclose(f);
    return 0;
}

// Hauptprogramm: Lädt das Labyrinth, führt Testmessungen durch, speichert den finalen Scan und gibt die Ergebnisse in der Konsole aus.
int main(void)
{
    static const float angles[] = {0.0f, 1.5708f, 3.1416f, 4.7124f};
    const int num_angles = (int)(sizeof(angles) / sizeof(angles[0]));

    if (read_walls("Labyrinth.txt") < 0) {
        return 1;
    }
    printf("Walls loaded: %d\n", num_walls);

    printf("Robot (phi = %.2f rad) -> distance = %.3f\n",
           robot_phi, perception(robot_x, robot_y, robot_phi));

    for (int i = 0; i < num_angles; i++) {
        const float d = perception(robot_x, robot_y, angles[i]);
        printf("phi = %.4f rad -> distance = %.3f\n", angles[i], d);
    }

    if (save_scan("Sensor.txt") < 0) {
        return 1;
    }
    
    printf("Scan saved to Sensor.txt\n");

    return 0;
}