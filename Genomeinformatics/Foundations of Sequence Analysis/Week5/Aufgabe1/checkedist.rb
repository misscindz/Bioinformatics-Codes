#!/usr/bin/env ruby
# tests the tool to calculate the edit distances of short sequences

def usage(error)
  STDERR.puts "USAGE: #$0 <path> <testcases>"
  STDERR.puts "where <path> is the path to the tool to test"
  STDERR.puts "and <testcases> is the file with testcases in the format"
  STDERR.puts "u\tv\tE"
  exit error
end

if ARGV.length != 2
  usage(0)
end

unless File.exist?(ARGV[0]) and File.executable?(ARGV[0]) and
    File.exist?(ARGV[1])
  STDERR.puts "Somethings wrong with the files given as arguments"
  usage(1)
end

File.open(ARGV[1]) do |file|
  regex = /^(\w+)\t(\w+)\t(\d+)$/
  file.each_line do |line|
    if m = line.match(regex)
      e = nil
      begin
        e = Integer(m[3])
      rescue
        STDERR.puts "#{m[3]} can not be parsed as a number"
        usage(1)
      end
      result = `#{ARGV[0]} #{m[1]} #{m[2]}`
      
      if mres = result.match(regex)
        begin
          if e != Integer(mres[3])
            STDERR.puts "Edist(#{m[1]},#{m[2]}) should be #{e} got #{mres[3]}"
            exit 1
          end
        rescue
          STDERR.puts "#{mres[3]} can not be parsed as a number"
          exit 1
        end
      else
        STDERR.puts "could not parse:\n#{result}"
        exit 1
      end
    else
      STDERR.puts "could not parse:\n#{line}"\
        "something wrong with the testcases?"
      usage 1
    end
  end
end
