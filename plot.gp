# Reiniciar configuraciones previas para evitar conflictos al recargar
reset

# Configuración visual
set title "Simulacion de Sensor y Laberinto"
set xlabel "X"
set ylabel "Y"
set size ratio -1       # Escala 1:1
set grid
set key outside right top

# Estilos
set style line 1 lc rgb "#FF8A80" lt 1 lw 0.6   # Rayos del sensor
set style line 2 lc rgb "#1A237E" lt 1 lw 2.5   # Paredes
set style line 3 lc rgb "#00C853" pt 7 ps 1.8   # Robot

# Posición del robot
ROBOT_X = 1.5
ROBOT_Y = 1.5

plot \
    'Sensor.txt' using 1:2:($3-$1):($4-$2) with vectors nohead ls 1 title "Rayos sensor", \
    'Sensor.txt' using 3:4 with points pt 7 ps 0.4 lc rgb "#D50000" title "Impactos", \
    'Labyrinth.txt' using 1:2:($3-$1):($4-$2) with vectors nohead ls 2 title "Paredes", \
    'Sensor.txt' every ::0::0 using 1:2 with points ls 3 title "Robot"