#!/usr/bin/python3

#dateinamen einlesen, wenn empty, dann default
#wenn temperatur

import math

#default filenames
filename = 'simulation_data.txt'
filename_out =  'simulation_analysis.out'

file = open(filename, 'r')
file_out = open(filename_out,'w')

temperature = []
pressure = []
intmol_energy = []
mol_reach = []

i=0
j=0
k=0
l=0
mean = 0
std_dev = 0
minimum=0
maximum= 0

for line in file:
    if 'temperature' in line:
        line.split()
        temperature[i] = line[1]
        i = i+1
    if 'pressure' in line:
        line.split()
        pressure[j] = line[1]
        i = j+1
    if 'intermolecular' in line:
        line.split()
        intmol_energy[k] = line[1]
        k = k+1
        
for elem in temperature:
    mean = (1/len(temperature))*elem + mean
    std_dev = (1/len(temperature))*pow(elem-mean)+std_dev
    std_dev = math.sqrt(std_dev)
    minimum = min(temperature)
    maximum = max(temperature)
    
file_out.write("=== ERGEBNISSE ===")
file_out.write("\n- Temperatur: " + mean + "+/-" + std_dev )
file_out.write("\nWertebereich: " + minimum + "-" + maximum)

for elem in pressure:
    mean = (1/len(pressure))*elem + mean
    std_dev = (1/len(pressure))*pow(elem-mean)+std_dev
    std_dev = math.sqrt(std_dev)
    minimum = min(pressure)
    maximum = max(pressure)
    
file_out.write("\n- intermol: " + mean + "+/-" + std_dev )
file_out.write("\nWertebereich: " + minimum + "-" + maximum)

# - Temperatur: <Mittelwert> +/- <Standardabweichung>
# Wertebereich: <Min> - <Max>
# - Druck: <Mittelwert> +/- <Standardabweichung>
# Wertebereich: <Min> - <Max>
# - intermolekulare Energie: <Mittelwert> +/- <Standardabweichung>
# Wertebereich: <Min> - <Max>
# - Molek"ule ausser Reichweite: <Mittelwert> +/- <Standardabweichung>
# Wertebereich: <Min> - <Max>    
    
    
    
    
    
        
        
    