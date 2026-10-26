reset
set terminal windows size 2000,1500

# ============================================================
# 1) LECTURA DE DATOS (antes de limitar los ejes)
# ============================================================
set datafile separator comma

# Contar registros totales
stats 'Rob_Spur.txt' nooutput
N = int(STATS_records)

# Leer coordenadas (x, y) de la última fila
eval sprintf("stats 'Rob_Spur.txt' every ::%d::%d using 1:2 nooutput", N-1, N-1)
rx_raw = STATS_mean_x
ry_raw = STATS_mean_y

# Leer orientación phi de la última fila
eval sprintf("stats 'Rob_Spur.txt' every ::%d::%d using 3 nooutput", N-1, N-1)
ph = STATS_mean

# ============================================================
# 2) PARÁMETROS Y ESCALA
# ============================================================
M  = 100      # Escala del laberinto (unidades -> mm)
R  = 10       # Escala del robot
X0 = 0
Y0 = 0
L  = 50       # Semilargo chasis (mm)
W  = 50       # Semiancho chasis (mm)
WL = 10       # Semilargo rueda (mm)
C  = 12       # Semilargo cruz central (mm)

rx = rx_raw*R + X0
ry = ry_raw*R + Y0

# ============================================================
# 3) TRANSFORMACIÓN DE COORDENADAS LOCALES A GLOBALES
# ============================================================
px(u,v) = rx + u*cos(ph) - v*sin(ph)
py(u,v) = ry + u*sin(ph) + v*cos(ph)

# ============================================================
# 4) CONFIGURACIÓN VISUAL DEL LIENZO
# ============================================================
set title "Kognitive Robotik - Labyrinth und Roboter"
set xlabel "X-Position (mm)"
set ylabel "Y-Position (mm)"
set size ratio -1
set xrange [0:1500]
set yrange [0:1500]
set grid
set key outside right top

# ============================================================
# 5) DIBUJO DEL ROBOT
# ============================================================
set object 1 polygon from px(L,W),py(L,W) to px(-L,W),py(-L,W) \
    to px(-L,-W),py(-L,-W) to px(L,-W),py(L,-W) to px(L,W),py(L,W) \
    fs empty border rgb 'blue' lw 3 front

set arrow 1 from px(-WL,W),py(-WL,W)   to px(WL,W),py(WL,W)   nohead lc rgb 'red'   lw 5 front
set arrow 2 from px(-WL,-W),py(-WL,-W) to px(WL,-W),py(WL,-W) nohead lc rgb 'red'   lw 5 front
set arrow 3 from px(-C,-C),py(-C,-C)   to px(C,C),py(C,C)     nohead lc rgb 'black' lw 2 front
set arrow 4 from px(-C,C),py(-C,C)     to px(C,-C),py(C,-C)   nohead lc rgb 'black' lw 2 front

# ============================================================
# 6) TRAZADO FINAL (LABERINTO + TRAYECTORIA)
# ============================================================

# 1. Leemos primero Rob_Spur.txt (con comas) y lo guardamos
set datafile separator comma
plot 'Sensor.txt' using ($1*R+X0):($2*R+Y0) with lines lw 3 lc rgb '#76e04a' title 'Fahrspur', \
     1/0 with lines lw 3 lc rgb 'blue' title 'Roboter', \
     1/0 with lines lw 3 lc rgb 'red'  title 'Raeder'

# 2. Cambiamos a espacios para superponer las paredes del laberinto
unset datafile separator
replot 'Labyrinth.txt' using ($1*M):($2*M):(($3-$1)*M):(($4-$2)*M) \
        with vectors nohead lw 3 lc rgb '#b39ddb' title 'Waende'