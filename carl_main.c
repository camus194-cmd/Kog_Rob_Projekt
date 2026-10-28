// Einbinden der Mathematik-Bibliothek (fuer cosf, sinf, fabsf, INFINITY, isinf)
#include <math.h>
// Einbinden der Standard-Eingabe/Ausgabe-Bibliothek (fuer printf, fprintf, fopen, fclose, fscanf)
#include <stdio.h>

// Definition globaler Konstanten
#define MAX_WALLS 50        // Maximale Anzahl an Waenden, die im Speicher abgelegt werden koennen
#define EPSILON     1e-6f   // Sehr kleine Zahl (0.000001), um Divisionen durch 0 bei parallelen Linien zu verhindern
#define PI          3.14159265f // Kreiszahl Pi zur Umrechnung von Grad in Bogenmass (Radian)

// Struktur-Definition "Wall": Stellt ein Wandsegment von Punkt A(x1, y1) nach Punkt B(x2, y2) dar
struct Wall {
    float x1; // X-Koordinate des Anfangspunkts (Punkt A)
    float y1; // Y-Koordinate des Anfangspunkts (Punkt A)
    float x2; // X-Koordinate des Endpunkts (Punkt B)
    float y2; // Y-Koordinate des Endpunkts (Punkt B)
};

// Globales Array, in dem bis zu 50 Waende gespeichert werden koennen
struct Wall walls[MAX_WALLS];
// Zaehler fuer die aktuell im Speicher geladenen Waende
int num_walls = 0;

// Zustand des Roboters im Raum (Startposition und Blickrichtung)
float robot_x   = 1.5f;  // X-Position des Roboters (z. B. 5.0 = 500mm)
float robot_y   = 4.0f;  // Y-Position des Roboters (z. B. 5.0 = 500mm)
float robot_phi = 1.57f; // Blickwinkel des Roboters in Radian (1.57 rad ca. 90 Grad = nach oben)

/**
 * Berechnet die Entfernung von einem Sensorpunkt (px, py) in Blickrichtung (phi) zu einer konkreten Wand.
 */
float distance_to_wall(float px, float py, float phi,
                       const struct Wall *wall)
{
    // Berechnet den Richtungsvektor des Sensorstrahls (Kosinus für X, Sinus für Y)
    const float c  = cosf(phi); // X-Anteil der Blickrichtung
    const float s  = sinf(phi); // Y-Anteil der Blickrichtung

    // Richtungsvektor der Wand (Differenz zwischen Endpunkt B und Startpunkt A)
    const float dx = wall->x2 - wall->x1;   // Wand-Laenge in X-Richtung
    const float dy = wall->y2 - wall->y1;   // Wand-Laenge in Y-Richtung

    // Vektor vom Sensorpunkt P zum Wandstartpunkt A
    const float rx = wall->x1 - px;         // Abstand Sensor zu Wand-A in X
    const float ry = wall->y1 - py;         // Abstand Sensor zu Wand-A in Y

    // Berechnet die Determinante, um zu prüfen, ob Strahl und Wand parallel verlaufen
    const float det = dx * s - dy * c;
    
    // Wenn die Determinante nahezu Null ist (kleiner als EPSILON), verlaufen Strahl und Wand parallel
    if (fabsf(det) < EPSILON) {
        return INFINITY; // Kein Schnittpunkt moeglich -> Rückgabe von Unendlich
    }

    // Berechnet den Schnittpunkt-Parameter mu (die Entfernung vom Sensor zum Trefferpunkt)
    const float mu     = (dx * ry - dy * rx) / det;
    // Berechnet den Parameter lambda (zeigt an, wo auf dem Wandsegment der Schnittpunkt liegt: 0.0 bis 1.0)
    const float lambda = (c * ry - s * rx) / det;

    // Ueberprueft, ob ein gueltiger Treffer vorliegt:
    // 1. mu > 0.0f: Die Wand liegt VOR dem Sensor (nicht im Ruecken des Roboters)
    // 2. lambda zwischen 0.0 und 1.0: Der Treffer liegt wirklich AUF dem Wandsegment
    if (mu <= 0.0f || lambda < 0.0f || lambda > 1.0f) {
        return INFINITY; // Wand wird verfehlt -> Rückgabe von Unendlich
    }

    // Gibt die berechnete Distanz zur Wand zurück
    return mu;
}

/**
 * Simuliert den Sensor: Testet alle geladenen Wände und gibt die Distanz zur nächsten Wand zurück.
 */
float perception(float px, float py, float phi)
{
    // Setzt die aktuell kleinste Distanz vorab auf Unendlich
    float min_distance = INFINITY;

    // Schleife geht nacheinander alle geladenen Waende durch
    for (int i = 0; i < num_walls; i++) {
        // Berechnet die Distanz zur aktuellen Wand i (Uebergabe der Speicheradresse mit &)
        const float d = distance_to_wall(px, py, phi, &walls[i]);

        // Wenn die gemessene Distanz kleiner ist als das bisherige Minimum
        if (d < min_distance) {
            min_distance = d; // Speichere die neue kleinste Distanz
        }
    }

    // Gibt die kleinste gefundene Wand-Distanz zurueck (oder INFINITY, wenn keine Wand getroffen wurde)
    return min_distance;
}

/**
 * Liest die Wandkoordinaten aus einer Textdatei ein (Format pro Zeile: x1 y1 x2 y2).
 */
int read_walls(const char *filename)
{
    // Oeffnet die Datei im Lese-Modus ("r" steht fuer read)
    FILE *f = fopen(filename, "r");

    // Prüft, ob beim Oeffnen der Datei ein Fehler aufgetreten ist (z. B. Datei existiert nicht)
    if (f == NULL) {
        // Gibt eine Fehlermeldung auf der Standard-Fehlerausgabe (stderr) aus
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1; // bricht ab und gibt den Fehlercode -1 zurueck
    }

    // Setzt den Wandzaehler vor dem Einlesen auf 0 zurück
    num_walls = 0;

    // Liest zeilenweise 4 Fließkommazahlen (x1, y1, x2, y2) in das Array ein,
    // solange die maximale Wandanzahl nicht erreicht ist und fscanf erfolgreich 4 Werte liest
    while (num_walls < MAX_WALLS &&
           fscanf(f, "%f %f %f %f",
                  &walls[num_walls].x1, &walls[num_walls].y1,
                  &walls[num_walls].x2, &walls[num_walls].y2) == 4) {
        num_walls++; // Erhoeht den Zaehler um 1 fuer jede erfolgreich gelesene Wand
    }

    // Schließt die Datei sauber wieder
    fclose(f);

    // Gibt die Anzahl der erfolgreich eingelesenen Waende zurueck
    return num_walls;
}

/**
 * Tastet die Umgebung schrittweise in Grad ab und speichert die Trefferpunkte in einer Textdatei.
 */
static int save_scan(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    // --- Definition deiner ZWEI Sensoren ---
    // Der Roboter guckt nach vorne (90 Grad = 1.57 rad). 
    // Die Sensoren sind schräg nach außen montiert.
    
    // 1. Linker Sensor (z.B. +30 Grad nach links von der Mitte)
    float winkel_links = 120.0f; 
    const float phi_L = winkel_links * PI / 180.0f;
    const float dist_L = perception(robot_x, robot_y, phi_L);

    // 2. Rechter Sensor (z.B. -30 Grad nach rechts von der Mitte)
    float winkel_rechts = 60.0f; 
    const float phi_R = winkel_rechts * PI / 180.0f;
    const float dist_R = perception(robot_x, robot_y, phi_R);

    // --- In Datei speichern (nur wenn Wand getroffen) ---
    // Linker Strahl
    if (!isinf(dist_L)) {
        fprintf(f, "%.3f %.3f %.3f %.3f\n", robot_x, robot_y,
                robot_x + dist_L * cosf(phi_L), robot_y + dist_L * sinf(phi_L));
    }
    
    // Rechter Strahl
    if (!isinf(dist_R)) {
        fprintf(f, "%.3f %.3f %.3f %.3f\n", robot_x, robot_y,
                robot_x + dist_R * cosf(phi_R), robot_y + dist_R * sinf(phi_R));
    }

    fclose(f);
    return 0;
}

// Hauptfunktion: Hier startet die Ausfuehrung des C-Programms
int main(void)
{
    // Array mit 4 Test-Winkeln in Radian (entspricht 0°, 90°, 180°, 270°)
    static const float angles[] = {0.0f, 1.5708f, 3.1416f, 4.7124f};
    
    // Berechnet die Anzahl der Elementen im Array (Gesamtgroesse geteilt durch Groesse eines Elements)
    const int num_angles = (int)(sizeof(angles) / sizeof(angles[0]));

    // Versucht, die Waende aus "Labyrinth.txt" zu laden. Bei Fehler (< 0) wird abgebrochen.
    if (read_walls("Labyrinth.txt") < 0) {
        return 1; // Programm beenden mit Fehlercode 1
    }
    // Gibt die Anzahl der geladenen Wände im Terminal aus
    printf("Walls loaded: %d\n", num_walls);

    // Misst und druckt die Distanz in der aktuellen Blickrichtung des Roboters
    printf("Robot (phi = %.2f rad) -> distance = %.3f\n",
           robot_phi, perception(robot_x, robot_y, robot_phi));

    // Schleife geht alle 4 Test-Winkel aus dem Array durch
    for (int i = 0; i < num_angles; i++) {
        // Misst die Distanz fuer den aktuellen Winkel i
        const float d = perception(robot_x, robot_y, angles[i]);
        // Druckt den Winkel und die gemessene Distanz in der Konsole aus
        printf("phi = %.4f rad -> distance = %.3f\n", angles[i], d);
    }

    // Fuehrt den Scan durch und speichert das Ergebnis in "Sensor.txt". Bei Fehler wird abgebrochen.
    if (save_scan("Sensor.txt") < 0) {
        return 1; // Programm beenden mit Fehlercode 1
    }
    
    // Gibt eine Bestätigungsmeldung aus
    printf("Scan saved to Sensor.txt\n");

    // Programm erfolgreich beenden (Rückgabewert 0 an das Betriebssystem)
    return 0;
}