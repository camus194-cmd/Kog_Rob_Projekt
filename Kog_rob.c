//sinf , cos

#include <stdio.h>

#include <math.h>

const float mm_per_sec_volt = 50; //konstante: mm/sec bei 1V spannung

float Robot_x = 100.0f; // 100mm = 10cm von Wand weg
float Robot_y = 100.0f;
float Robot_phi = 0.0f; // 0 Rad = schaut nach Rechts


void Drive(float Mot_L, float Mot_R, float Time) {
    Robot_x = Robot_x + (Mot_L + Mot_L)/2 * cos(Robot_phi);
    Robot_y = Robot_y + (Mot_L + Mot_L)/2 * sin(Robot_phi);
    Robot_phi = Robot_phi + (Mot_L - Mot_R)/2;
}

int main() {
    FILE *f = fopen("Rob_Spur.txt", "w");
    fprintf(f, "%.2f   %.2f    %.2f \n", Robot_x, Robot_y, Robot_phi);


    for (int k = 0; k < 10; k++) {
        Drive(2.0f, 4.0f, 1.0f);
        fprintf(f," %.2f    %.2f   %.2f\n", Robot_x, Robot_y, Robot_phi);
    }

    fclose(f);
    return 0;
}
