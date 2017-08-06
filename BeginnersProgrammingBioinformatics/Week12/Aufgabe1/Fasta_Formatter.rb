#!/usr/bin/env ruby 
#PBI Blatt11 Aufgabe2
#Abgabe 11.1.16
#Gruppe: Wang, Achoukhi, Ching


class FastaIterator
    
    def initialize (filename)
      @file = File.new(filename, "r")
        rescue => err
            STDERR.puts "Could not open data #{newfile}: #{err}"
        exit 1
    end
    
    def each
      seq = ""
      header = ""
      @file.each_line do |line|
        if line.match (/^(>|\ s*$|\s*#)/)
          if not header.empty?
            yield header,seq
            seq = ""
          end
          header = line
        else
          seq += line
          seq.gsub!(/\s/,"")
        end
      end
      yield header,seq
    end

end

def print_formatted(head,sequence)
    pos = 0
    puts head
    while pos<sequence.length do
      puts sequence[pos..pos+69]
      pos += 70
    end
end
    
my_fasta = FastaIterator.new("swiss.fna")

bin=[]
bins=[]

my_fasta.each do |header, sequence|
    print_formatted(header,sequence)
    bin.push(((sequence.length.to_f-1.00)/100.00).ceil-1)
end

bin.uniq.each{|element| bins[element] = bin.count(element)}

bins.each_with_index do |element, index| 
  unless element.nil?
    puts "#{(100*index)+1}-#{(index + 1)*100}: #{element}"
  end
end
