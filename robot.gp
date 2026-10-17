reset
set terminal windows size 900,700

# ============================================================
# 1) LECTURA DEL ARCHIVO DEL ROBOT
#    Rob_Spur.txt esta separado por comas (x, y, phi).
#    Lo leemos con separador coma y lo guardamos con "set table"
#    en rob.txt, que queda separado por espacios. Asi todo el
#    script usa el mismo separador y no hace falta ningun programa externo.
# ============================================================
set datafile separator comma
set table 'rob.txt'
plot 'Rob_Spur.txt' using 1:2:3 with table
unset table
set datafile separator whitespace

# ============================================================
# 2) ULTIMA POSE DEL ROBOT (x, y, phi)
#    stats cuenta las filas (STATS_records). Luego se usa "every"
#    para leer solo la ultima fila. eval + sprintf se usan porque
#    "every" necesita numeros, no expresiones.
# ============================================================
stats 'rob.txt' using 1 nooutput
N = int(STATS_records)
eval sprintf("stats 'rob.txt' every ::%d::%d using 1:2 nooutput", N-1, N-1)
rx_raw = STATS_mean_x
ry_raw = STATS_mean_y
eval sprintf("stats 'rob.txt' every ::%d::%d using 3 nooutput", N-1, N-1)
ph = STATS_mean                 # angulo phi en radianes

# ============================================================
# 3) PARAMETROS AJUSTABLES
# ============================================================
M  = 100      # escala del laberinto: unidades -> mm (15 unidades = 1500 mm)
R  = 10       # escala del robot: unidades del archivo -> mm
X0 = 0        # desplazamiento inicial del robot en x (mm)
Y0 = 0        # desplazamiento inicial del robot en y (mm)
L  = 70       # semilargo del robot (mm)   <- mas pequeño
W  = 50       # semiancho del robot (mm)   <- mas pequeño
WL = 18       # semilargo de cada rueda (mm)
C  = 12       # semilargo de la "x" del centro (mm)

# Posicion final del robot ya escalada a mm
rx = rx_raw*R + X0
ry = ry_raw*R + Y0

# ============================================================
# 4) TRANSFORMACION DE COORDENADAS
#    (u,v) son coordenadas locales del robot:
#      u = hacia delante, v = hacia la izquierda.
#    px, py las giran el angulo phi y las trasladan a la posicion
#    (rx, ry) del mundo.
# ============================================================
px(u,v) = rx + u*cos(ph) - v*sin(ph)
py(u,v) = ry + u*sin(ph) + v*cos(ph)

# ============================================================
# 5) APARIENCIA DEL GRAFICO
#    "size ratio -1" hace que 1 mm en x mida lo mismo que en y,
#    para que el laberinto no se deforme.
# ============================================================
set title "Kognitive Robotik - Labyrinth und Roboter"
set xlabel "X-Position (mm)"
set ylabel "Y-Position (mm)"
set size ratio -1
set xrange [0:1500]
set yrange [0:1500]
set grid
set key outside right top       # leyenda a la derecha, fuera del grafico

# ============================================================
# 6) DIBUJO DEL ROBOT
#    Cuerpo: poligono azul con las 4 esquinas rotadas segun phi.
#    Ruedas: dos segmentos rojos en los lados izquierdo y derecho.
#    "front" los dibuja por encima de las lineas del laberinto.
# ============================================================
set object 1 polygon from px(L,W),py(L,W) to px(-L,W),py(-L,W) \
    to px(-L,-W),py(-L,-W) to px(L,-W),py(L,-W) to px(L,W),py(L,W) \
    fs empty border rgb 'blue' lw 3 front
set arrow 1 from px(-WL,W),py(-WL,W)   to px(WL,W),py(WL,W)   nohead lc rgb 'red' lw 5 front
set arrow 2 from px(-WL,-W),py(-WL,-W) to px(WL,-W),py(WL,-W) nohead lc rgb 'red' lw 5 front
# Marca "x" en el centro del robot (dos diagonales, giran con phi)
set arrow 3 from px(-C,-C),py(-C,-C) to px(C,C),py(C,C)   nohead lc rgb 'black' lw 2 front
set arrow 4 from px(-C,C),py(-C,C)   to px(C,-C),py(C,-C) nohead lc rgb 'black' lw 2 front

# ============================================================
# 7) PLOT FINAL
#    - Paredes: cada fila de Labyrinth.txt es x1 y1 x2 y2.
#      "with vectors nohead" dibuja una linea desde (x1,y1) con
#      desplazamiento (x2-x1, y2-y1), todo multiplicado por M.
#    - Fahrspur: la trayectoria del robot como linea verde.
#    - Las dos ultimas curvas "1/0" no dibujan nada: solo crean
#      las entradas "Roboter" y "Raeder" en la leyenda.
# ============================================================
plot 'Labyrinth.txt' using ($1*M):($2*M):(($3-$1)*M):(($4-$2)*M) \
        with vectors nohead lw 3 lc rgb '#b39ddb' title 'Waende', \
             'rob.txt' using ($1*R+X0):($2*R+Y0) with lines lw 3 lc rgb '#76e04a' title 'Fahrspur', \
             1/0 with lines lw 3 lc rgb 'blue' title 'Roboter', \
             1/0 with lines lw 3 lc rgb 'red'  title 'Raeder'