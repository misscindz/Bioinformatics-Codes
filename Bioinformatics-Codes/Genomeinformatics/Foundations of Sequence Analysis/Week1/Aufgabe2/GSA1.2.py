seq = "gagtccgatggcgttggat"
k = 5 #Wortlaenge
wordcount = 0 #Anzahl ausgegebener Woerter
word = [] #array

def getnext(seq): #funktiondefinition
    for i in seq:
        yield i #creation eines generators damit next() benutzt werden kann

z = getnext(seq) #initiation des generators

while True: #loop
	while len(word) < k: #array word auf laenge k fuellen
		x = z.next() #naechste sequenzeinheit (base) abrufen
		word.append(x) 
	print "".join(word[wordcount:] + word[:wordcount])
	#das wort kann ausgegeben werden wenn im array am index wordcount angefangen wird, 
	#und einmal komplett durchgelaufen wird (so dass alle elemente einmal ausgegeben wurden)
	try:
		word[wordcount] = z.next() #das naechte element wird an der korrekten stelle eingefuegt
	except StopIteration: #keine weiteren werte in next()
		break #loop beenden
	wordcount += 1 #es wurde ein zusaetzliches wort ausgegeben
	if wordcount > k-1: #wordcount soll nicht laenger als k sein, so dass das print statement gilt
		wordcount = 0
