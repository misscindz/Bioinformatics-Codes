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

    begin    
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
            seq.chomp!
          end
        end
        yield header,seq
      end
    end 
end

    
