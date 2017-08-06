import sys; sys.dont_write_bytecode = True

# CONSTANT VALUES
AA2AU = 1.88972612457


#
# COVALENT RADII
#
# based on "Atomic Radii of the Elements," M. Mantina, R. Valero, C. J. Cramer, and D. G. Truhlar,
# in CRC Handbook of Chemistry and Physics, 91st Edition (2010-2011),
# edited by W. M. Haynes (CRC Press, Boca Raton, FL, 2010), pages 9-49-9-50;
# corrected Nov. 17, 2010 for the 92nd edition.
#
r_in_AA = {
      1  :0.32,
      2  :0.37,
      3  :1.30,
      4  :0.99,
      5  :0.84,
      6  :0.75,
      7  :0.71,
      8  :0.64,
      9  :0.60,
      10 :0.62,
      11 :1.60,
      12 :1.40,
      13 :1.24,
      14 :1.14,
      15 :1.09,
      16 :1.04,
      17 :1.00,
      18 :1.01,
      19 :2.00,
      20 :1.74,
      21 :1.59,
      22 :1.48,
      23 :1.44,
      24 :1.30,
      25 :1.29,
      26 :1.24,
      27 :1.18,
      28 :1.17,
      29 :1.22,
      30 :1.20,
      31 :1.23,
      32 :1.20,
      33 :1.20,
      34 :1.18,
      35 :1.17,
      36 :1.16,
      37 :2.15,
      38 :1.90,
      39 :1.76,
      40 :1.64,
      41 :1.56,
      42 :1.46,
      43 :1.38,
      44 :1.36,
      45 :1.34,
      46 :1.30,
      47 :1.36,
      48 :1.40,
      49 :1.42,
      50 :1.40,
      51 :1.40,
      52 :1.37,
      53 :1.36,
      54 :1.36,
      55 :2.38,
      56 :2.06,
      57 :1.94,
      58 :1.84,
      59 :1.90,
      60 :1.88,
      61 :1.86,
      62 :1.85,
      63 :1.83,
      64 :1.82,
      65 :1.81,
      66 :1.80,
      67 :1.79,
      68 :1.77,
      69 :1.77,
      70 :1.78,
      71 :1.74,
      72 :1.64,
      73 :1.58,
      74 :1.50,
      75 :1.41,
      76 :1.36,
      77 :1.32,
      78 :1.30,
      79 :1.30,
      80 :1.32,
      81 :1.44,
      82 :1.45,
      83 :1.50,
      84 :1.42,
      85 :1.48,
      86 :1.46,
      87 :2.42,
      88 :2.11,
      89 :2.01,
      90 :1.90,
      91 :1.84,
      92 :1.83,
      93 :1.80,
      94 :1.80,
      95 :1.73,
      96 :1.68,
      97 :1.68,
      98 :1.68,
      99 :1.65,
      100:1.67,
      101:1.73,
      102:1.76,
      103:1.61,
      104:1.57,
      105:1.49,
      106:1.43,
      107:1.41,
      108:1.34,
      109:1.29,
      110:1.28,
      111:1.21,
      112:1.22,
      113:1.36,
      114:1.43,
      115:1.62,
      116:1.75,
      117:1.65,
      118:1.57,
}

class autoconverter_to_au():
    def __getitem__(self, key):
        return r_in_AA[key]*AA2AU
 
r_in_AU = autoconverter_to_au()

# test
#print(r_in_AA[8], r_in_AU[8])
