#!/usr/bin/env ruby
# Aufgabe 12.3
# Gruppe 5: Achoukhi, Breitschuh, Ching, Moradi

class PSSM

  def initialize (file)
    @matrix = Hash.new()
    
    begin
      data = File.new(file, "r")
    rescue
      STDERR.puts "Error: Could not open #{file}"
      abort "Usage: #{__FILE__} <s|l> <PSSM-data> <sequence> <threshold>"
    end
    lines = data.readlines
    data.close
    
    for i in 0...lines.length
      line = lines[i].split(' ')
      char = line[0]
      @m = line[1..-1].length
      for j in 0...line.length-1
        @matrix[[char,j]] = Integer(line[j+1])
      end
    end
  end
  
  def thd_array (th)
    @thd, sigma_d = Array.new(@m), Array.new(@m, 0)
    
    @matrix.each do |key, value|
      if sigma_d[key[1]] < value
         sigma_d[key[1]] = value
      end
    end
    (@m-2).downto(0) do |i|
      sigma_d[i] += sigma_d[i+1]
    end
    sigma_d[@m] = 0; sigma_d.delete_at 0
    for i in 0...@m
      @thd[i] = th - sigma_d[i]
    end
  end
  
  def simple (sequence, th)    
    for j in 0..sequence.length-@m
      score = 0
      for i in 0...@m
        score += @matrix[[sequence[i+j],i]]
      end
      if score >= th then
        puts "#{j}\t#{score}"
      end
    end
  end
  
  def lookahead (sequence, th)
    thd_array(th)

    for j in 0..sequence.length-@m
      score = 0
      for i in 0...@m
        score += @matrix[[sequence[i+j],i]]
        if score < @thd[i]
          break
        end
      end
      if score >= th then
        puts "#{j}\t#{score}"
      end
    end
  end

end

algorithm, file, sequence = String(ARGV[0]), String(ARGV[1]), String(ARGV[2])
begin
  th = Integer(ARGV[3])
rescue
  if ARGV.length != 4
    STDERR.puts "Error: Wrong number of arguments"
  else
    STDERR.puts "Error: The threshold has to be an integer"
  end
    abort "Usage: #{__FILE__} <s|l> <PSSM-data> <sequence> <threshold>"
end

pssm = PSSM.new(file)
if algorithm == "s"
  pssm.simple(sequence, th)
elsif algorithm == "l"
  pssm.lookahead(sequence, th)
else
  STDERR.puts "Error: Please choose th simple- <s> or lookahead-algorithm <l>"
  abort "Usage: #{__FILE__} <s|l> <PSSM-data> <sequence> <threshold>"
end