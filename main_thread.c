#include <stdio.h>
#include <math.h>

const float mm_per_sec_volt = 50; //konstante: mm/sec bei 1V spannung

float Robot_x = 15.0f; // 100mm = 10cm von Wand weg
float Robot_y = 15.0f;
float Robot_phi = 1.57f; // 0 Rad = mira derecha nach Rechts
float d_x; // Distanz von roboter_x zu wand_x
float d_y; // Distanz von robotor_y zu wand_y
//FILE* f;

/*void ReadWalls() {
    float x1 , y1 , x2 , y2 ;
    FILE* f = fopen("laberinto.txt","r"); // Todo: Laberynth datei lesen


    printf("fopen liefert : %d\n",f);
    while(_eof(f) == 0){ //todo: check if end of the file reached
    fscanf(f,"%f %f %f %f , x1 , y1 , x2 , y2);
    float Dist = 2.3f; // Todo : call distance calculation
    printf(" %f %f %f %f --> %f\n" , x1 , y1 , x2 , y2);
    }
    fclose(f);

 */
struct Wand{
    float x1,y1,x2,y2;
};

struct Wand nwand[50];


void Drive(float Mot_L, float Mot_R, float Time) {
    Robot_x = Robot_x + ((Mot_L + Mot_R) / 2) * cosf(Robot_phi);
    Robot_y = Robot_y + ((Mot_L + Mot_R) / 2) * sinf(Robot_phi);
    Robot_phi = Robot_phi + (Mot_L - Mot_R) / 2;
}
//
void Distanz(float x_wand, float y_wand , float x_robot , float y_robot ) {

    d_x = (x_wand - x_robot)/cosf(Robot_phi);
    d_y = (y_wand - y_robot)/sinf(Robot_phi);
}

int main() {
    FILE *f=fopen("Labyrinth.txt","r");
    if (f == 0 ) {
        printf("Fehler bei Datei offnen\n");
        return 1;
    }


    /*FILE *f = fopen("Rob_Spur.txt", "w");
    fprintf(f, "%6.1f, %6.1f , %6.1f\n", Robot_x, Robot_y, Robot_phi);


    while (d_x > 0.0f) {
        for (int k = 0; k < 15.0f; k++) {
            Drive(5.0f, 5.0f, 15.0f);//para que es time ?
            fprintf(f, "%6.1f, %6.1f, %6.1f\n", Robot_x, Robot_y, Robot_phi);
        }

    }




    fclose(f);*/
    return 0;
};
