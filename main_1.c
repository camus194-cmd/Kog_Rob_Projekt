// como interactua M_L y M_R y como funciona la funcion drive ?

#include <stdio.h>
#include <math.h>

const float mm_per_sec_volt = 50; //konstante: mm/sec bei 1V spannung

float Robot_x = 15.0f; // 100mm = 10cm von Wand weg
float Robot_y = 15.0f;
float Robot_phi = 1.57f; // 0 Rad = mira derecha nach Rechts

void Drive(float Mot_L, float Mot_R, float Time) {
    Robot_x = Robot_x + ((Mot_L + Mot_R) / 2) * cos(Robot_phi);
    Robot_y = Robot_y + ((Mot_L + Mot_R) / 2) * sin(Robot_phi);
    Robot_phi = Robot_phi + (Mot_L - Mot_R) / 2;
}

int main() {
    FILE *f = fopen("Rob_Spur.txt", "w");
    fprintf(f, "%6.1f, %6.1f , %6.1f\n", Robot_x, Robot_y, Robot_phi);

    for (int k = 0; k < 18.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 0;
    for (int k = 0; k < 11.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 4.7; // radianes
    for (int k = 0; k < 7.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 3.14 ; // radianes
    for (int k = 0; k < 4.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 4.7 ; // radianes
    for (int k = 0; k < 5.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 0 ; // radianes
    for (int k = 0; k < 10.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 1.57 ; // radianes
    for (int k = 0; k < 6.8f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 0 ; // radianes
    for (int k = 0; k < 7.6f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    Robot_phi = 1.57 ; // radianes
    for (int k = 0; k < 11.0f; k++) {
        Drive(5.0f, 5.0f, 15.0f); //para que es time ?
        fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
    }

    fclose(f);
    return 0;
};
