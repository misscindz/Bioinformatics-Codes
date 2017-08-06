#!/usr/bin/env ruby

dna = "ACGAATT ACTTTAGC"
rna = dna.gsub(/T/, "U")

print "\nHere is the starting DNA: "
print "#{dna}\n"

print "Here is the result: "
puts "#{rna}\n\n"
exit 0


							      
=begin	

PBI
Aufgabe 8.3
Gruppe: Achoukhi, Wang, Ching
Abgabe 7. Dezember 2015	
					   
Geaenderte Zeilen angezeigt mit sdiff:
======================================

accept10-1.rb 							      correct10-1.rb:

dna = 'ACGAATT\tACTTTAGC'				      |	dna = "ACGAATT ACTTTAGC"     ----> '\t' entfernt
rna = dna.gsub!(/T/, "U")				      |	rna = dna.gsub(/T/, "U")     ----> '!' entfernt
print "Here is the starting DNA:\t"			  |	print "\nHere is the starting DNA: " ----> newline zugefuegt, tabulator entfernt
print "#{dna}\n\n"					          |	print "#{dna}\n"   ---->newline entfernt
print "Here is the result:\n\n"				  |	print "Here is the result: " ----> 2 newlines entfernt
puts "#{rna}\n"						          |	puts "#{rna}\n\n" ---->newline hinzugefuegt


	      

=end	
