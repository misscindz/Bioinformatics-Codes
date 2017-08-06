#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
from os import listdir
from os.path import isfile, join
from subprocess import Popen, PIPE
import numpy as np
import time


tests = [
            ["01_Water-Water", -4.77, -5.35, 8.76],
            ["02_Water-MeOH", -4.80, -5.83, 6.24],
            ["03_Water-MeNH2", -6.80, -7.89, -6.09],
            ["04_Water-Peptide", -6.63, -8.16, -4.11],
            ["05_MeOH-MeOH", -4.78, -6.16, -4.06],
            ["06_MeOH-MeNH2", -6.52, -8.50, -6.97],
            ["07_MeOH-Peptide", -6.40, -8.63, -5.16],
            ["08_MeOH-Water", -4.66, -5.46, -3.43],
            ["09_MeNH2-MeOH", -1.44, -3.12, -1.49],
            ["10_MeNH2-MeNH2", -2.29, -4.41, -1.73],
            ["11_MeNH2-Peptide", -2.14, -5.32, -1.67],
            ["12_MeNH2-Water", -6.66, -7.94, -5.70],
            ["13_Peptide-MeOH", -3.63, -6.02, -3.38],
            ["14_Peptide-MeNH2", -5.12, -8.02, -5.73],
            ["15_Peptide-Peptide", -5.35, -8.97, -4.79],
            ["16_Peptide-Water", -4.16, -5.34, -2.34],
            ["17_Uracil-Uracil_BP", -13.76, -17.23, -8.09],
            ["18_Water-Pyridine", -6.34, -7.71, -5.50],
            ["19_MeOH-Pyridine", -6.30, -8.37, -6.28],
            ["20_AcOH-AcOH", -16.19, -18.77, -10.94],
            ["21_AcNH2-AcNH2", -13.70, -16.44, -7.24],
            ["22_AcOH-Uracil", -16.07, -19.06, -9.37],
            ["23_AcNH2-Uracil", -15.94, -19.07, -8.38],
            ["24_Benzene-Benzene_pi-pi", 2.64, -3.92, -2.68],
            ["25_Pyridine-Pyridine_pi-pi", 1.96, -5.01, -3.95],
            ["26_Uracil-Uracil_pi-pi", -0.03, -10.31, -3.30],
            ["27_Benzene-Pyridine_pi-pi", 2.27, -4.55, -3.43],
            ["28_Benzene-Uracil_pi-pi", 2.18, -6.60, -4.48],
            ["29_Pyridine-Uracil_pi-pi", 1.12, -7.53, -4.23],
            ["30_Benzene-Ethene", 1.86, -1.90, -1.06],
            ["31_Uracil-Ethene", 1.04, -3.56, -2.33],
            ["32_Uracil-Ethyne", 0.26, -3.83, -1.70],
            ["33_Pyridine-Ethene", 1.61, -2.29, -1.64],
            ["34_Pentane-Pentane", 2.92, -3.73, -3.48],
            ["35_Neopentane-Pentane", 2.00, -2.56, -2.31],
            ["36_Neopentane-Neopentane", 1.46, -1.84, -1.56],
            ["37_Cyclopentane-Neopentane", 1.94, -2.47, -2.21],
            ["38_Cyclopentane-Cyclopentane", 2.17, -2.99, -2.76],
            ["39_Benzene-Cyclopentane", 2.02, -4.08, -3.48],
            ["40_Benzene-Neopentane", 1.59, -3.21, -2.63],
            ["41_Uracil-Pentane", 2.66, -5.16, -3.58],
            ["42_Uracil-Cyclopentane", 2.47, -4.39, -3.33],
            ["43_Uracil-Neopentane", 1.81, -3.78, -2.47],
            ["44_Ethene-Pentane", 1.59, -1.84, -1.40],
            ["45_Ethyne-Pentane", 1.24, -1.77, -0.99],
            ["46_Peptide-Pentane", 2.16, -4.25, -2.81],
            ["47_Benzene-Benzene_TS", 1.06, -3.14, -2.52],
            ["48_Pyridine-Pyridine_TS", 0.50, -3.71, -2.29],
            ["49_Benzene-Pyridine_TS", 0.67, -3.60, -2.61],
            ["50_Benzene-Ethyne_CH-pi", -0.36, -3.10, -1.83],
            ["51_Ethyne-Ethyne_TS", -0.75, -1.76, -0.36],
            ["52_Benzene-AcOH_OH-pi", -1.28, -4.90, -2.04],
            ["53_Benzene-AcNH2_NH-pi", -1.28, -4.52, -0.81],
            ["54_Benzene-Water_OH-pi", -1.52, -3.82, -1.55],
            ["55_Benzene-MeOH_OH-pi", -0.82, -4.63, -2.82],
            ["56_Benzene-MeNH2_NH-pi", 0.40, -3.55, -2.56],
            ["57_Benzene-Peptide_NH-pi", -0.26, -5.86, -3.72],
            ["58_Pyridine-Pyridine_CH-N", -1.65, -4.00, -1.02],
            ["59_Ethyne-Water_CH-O", -2.46, -3.04, -0.95],
            ["60_Ethyne-AcOH_OH-pi", -3.31, -5.23, -1.06],
            ["61_Pentane-AcOH", 1.88, -2.82, -1.69],
            ["62_Pentane-AcNH2", 1.48, -3.61, -1.81],
            ["63_Benzene-AcOH", 0.93, -4.03, -2.51],
            ["64_Peptide-Ethene", 0.44, -2.85, -1.10],
            ["65_Pyridine-Ethyne", -3.16, -4.56, -2.20],
            ["66_MeNH2-Pyridine", -0.52, -4.10, -2.08]
    ]


def runProgrammTestData(options, ref=3):

    program = "./emmi.py"
    path = "./data/testDataEmmi/"
    onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
    onlyfiles = [f for f in onlyfiles if f.endswith("emmi")]
    onlyfiles = sorted(onlyfiles)
    sumTime = 0

    for test in onlyfiles:
        start = time.time()
        testFile = "".join([path, test]).strip()
        number = int(test.split('_')[0])-1

        process = Popen([program, testFile]+list(options), stdout=PIPE)
        (output, err) = process.communicate()
        o = output.split()
        emi = float(o[0])
        exit_code = process.wait()

        eref = tests[number][ref]
        print "{};{};{}".format(test, emi, eref)
        end = time.time()
        sumTime += end - start

    print("Time", sumTime/len(onlyfiles))

if __name__ == "__main__":
    program = "./emmi.py"
    path = "./data/testDataEmmi/"

    testFile = "./data/??"

    options = np.array(["-nod", "-noe", "-nob", "-noh", "-nop"])

    for i in range(5):

        mask = np.ones(options.shape, dtype=bool)
        mask[i] = 0
        print options[mask], ";result;expected"
        runProgrammTestData(options[mask])

    # ohne loesungsmittel
    print "ohne loesungsmittel"
    runProgrammTestData(["-nob"], 2)
    print "ohne loesungsmittel und disp korektur"
    runProgrammTestData(["-nob", "-ndc"], 1)
    print "alles"
    runProgrammTestData([], 3)
