seq = list("gagctgaataa")

komp = {"a":"t", "t":"a", "g":"c", "c":"g"} # key: a, t, g, c; hash table der komplementaeren Basen 

for i in range(len(seq)/2): #halbe sequenz wird jeweils betrachtet
    x = seq[i] #wert zwischenspeichern
    seq[i] = komp[seq[len(seq)-1-i]] #i-ten wert durch komplement von ende minus i-ten wert ersaetzen
    seq[len(seq)-1-i] = komp[x] #komplement des gespeicherten wertes an ende minus i-ter position einfuegen
if len(seq)%2 != 0: #falls ungerade sequenzlaenge
    seq[(len(seq)/2)] = komp[seq[(len(seq)/2)]] #komplement des mittleren buchstaben einsetzen
    
print "".join(seq) #Ausgabe des Komplements als Wort