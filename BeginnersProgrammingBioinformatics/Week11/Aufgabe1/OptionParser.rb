#!/usr/bin/env ruby 
#PBI Blatt11 Aufgabe2
#Abgabe 11.1.16
#Gruppe: Wang, Achoukhi, Ching

require 'optparse'
require 'ostruct'

  if ARGV.empty?
    raise "Missing filename."
  elsif not ["--help","-h"].include? ARGV.last
    filename = ARGV.last
    if not File.exist?(filename)
      raise "File not existing"
    end
  end

class OptparseExample

  def self.parse(args)
    options = OpenStruct.new
    options.selecttop = []
    options.echo = false
    options.search = ""
    
    selecttop_switch = false
    search_switch = false
    

    opt_parser = OptionParser.new do |opts|
      opts.banner = "Usage: #{$0} [options] inputfile"

      opts.separator ""
      opts.separator "List of possible options:"
      
      #selecttop
      identifiers = ["LOCUS", "DEFINITION", "ACCESSION", "VERSION", "KEYWORDS", "SOURCE", "REFERENCE", "FEATURES", "ORIGIN"]
      opts.on("--selecttop x,y,z", Array, "Specify Genbank-Toplevel-Identifier") do |selecttop|
         if selecttop_switch == true
           raise "\nOption can only be used once.\n"
         else
            selecttop.each do |element|
                if not identifiers.include? element.upcase
                    raise "\nMissing identifier or invalid identifier #{element}. Valid Identifiers: e.g. 'LOCUS', 'DEFINITION', 'ORIGIN'. \n "
                end
            end
           options.selecttop = selecttop
           selecttop_switch = true
         end
      end

      # echo
      opts.on("-e", "--echo", "Use echo mode") do
         if options.echo == true
           raise "\nOption can only be used once.\n"
         else
           if search_switch == true
              raise "\n\n'--echo' and '--search' cannot be used at the same time\n\n"
              ARGV << '-h'
           else
              options.echo = true
           end
         end
      end
      
     # regexp
      opts.on("-s","--search <regexp>", "Search mode using specified <regexp>") do |regexp|
        if search_switch == true
          raise "\nOption can only be used once.\n"
        
        elseif options.echo == true
          raise '"--echo" and "--search" cannot be used at the same time'
          ARGV << '-h'
        else
          options.search = regexp
          search_switch = true
        end
      end

      opts.on_tail("-h", "--help", "Show help this help message") do
        puts opts
        exit
      end
    end
    
    opt_parser.parse!(args)
    options
  end
 

end  # class OptparseExample

options = OptparseExample.parse(ARGV)

