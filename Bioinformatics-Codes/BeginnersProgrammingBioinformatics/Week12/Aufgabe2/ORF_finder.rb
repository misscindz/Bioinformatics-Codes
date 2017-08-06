#!/usr/bin/env ruby 
#PBI Blatt11 Aufgabe2
#Abgabe 11.1.16
#Gruppe: Wang, Achoukhi, Ching

$LOAD_PATH << '.'

require "transcription_table"
require "FastaIterator"

orfs = FastaIterator.new("seq-with-orfs.fna")

      orf_s=[]
      orfs.each do |header,sequence|
          
          rev_sequence = sequence.reverse
          rev_comp_sequence = rev_sequence.tr("ACGTacgt","TGCAtgca")
          orf_s.push(sequence.match(/atg.{3}*?(?:taa|tag|tga)/).to_s)
          orf_s.push(rev_comp_sequence.match(/atg.{3}*?(?:taa|tag|tga)/).to_s)

          longest_orf = orf_s.max_by(&:length)
          longest_orf.to_s.upcase! 

          if(longest_orf.length%3)==0
            pos = 0
            
            puts "#{header.chomp} with orf of length #{longest_orf.length}: translation "
            amino = TranscriptionTable.translate(longest_orf)
            
            while pos<amino.length do
            puts amino[pos..pos+69]
            pos += 70
            end
          end
          orf_s.clear
          
      end

          