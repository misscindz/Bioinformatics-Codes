#!/usr/bin/env ruby

dna = 'ACGAATT\tACTTTAGC'
rna = dna.gsub!(/T/, "U")

print "Here is the starting DNA:\t"
print "#{dna}\n\n"

print "Here is the result:\n\n"
puts "#{rna}\n"
exit 0

=begin

PBI
Aufgabe 8.3
Gruppe: Achoukhi, Wang, Ching
Abgabe 7. Dezember 2015
					   
Geaenderte Zeilen angezeigt mit sdiff:
======================================

error10-1.rb 							      accept10-1.rb

rna = dna.gsub!(/T/, /U/)				      |	rna = dna.gsub!(/T/, "U")  ----> "U" statt /U/
print "Here "is the starting DNA:\t"		  |	print "Here is the starting DNA:\t" ----> "-Zeichen entfernt
puts "#{rna)\n"						          |	puts "#{rna}\n" ----> geschweifte Klammer statt runder Klammer

     
=end
