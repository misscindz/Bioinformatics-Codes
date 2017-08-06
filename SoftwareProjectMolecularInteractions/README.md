# molecular-interactions
Calculation of bond energy between molecules.

usage: emmi.py [-h] [-nod] [-noe] [-nob] [-noh] [-nop] [-dse] [-nCM5] [mf]

Calculates  Energy term for molecular interactions.

positional arguments:
  mf                    Paths to the xyz-molecule-file for which the Energy shall be calculated.
                        The file must be in the following format:
                         
                        Line 1:                 <number of atoms>
                        Line 2:                 Comment line
                        Line 3 and following:   <Molecule id of atom> <x> <y> <z>
                                                ...

optional arguments:
  -h, --help            show this help message and exit
  -nod, --noDispersion  dispersion energiy is not calculated.
  -noe, --noElectrostatics
                        electrostatic energiy is not calculated.
  -nob, --noBorn        born energiy is not calculated.
  -noh, --noHBondes     hBond energiy is not calculated.
  -nop, --noPolarisation
                        polarisation energiy is not calculated.
  -dse, --displaySingleEnergies
                        show the energie values out of which the all in all energy consits.
  -nCM5, --noCM5correction
                        no use of CM5 correction. (default: false).



