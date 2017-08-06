#!/usr/bin/ruby
# PBI Uebung 12
# Foerster Niehus Voges

class FastaIterator
  def initialize(filename)
    @filename = filename
    @headerlist = []
    @seqlist = []

    File.new(@filename, 'r').each do |line|
      if line.match(/^>/)
	@headerlist.push(line[1..-1])
	@seqlist.push("")
      else 
	@seqlist[-1] += line.chomp!
      end
    end
  end
  
  def each()
    0.upto(@headerlist.length - 1) do |x|
      yield @headerlist[x], @seqlist[x]
    end
  end
  
  
end
    
myfasta = FastaIterator.new("swiss.fna")
bins = []
myfasta.each do |x, y|
  puts x
  (0..y.length-1).step(70) do |i|
    puts y[i..i+69]
  end
  
  bin = ((y.length-1) / 100).floor
  bins[bin] = (bins[bin] || 0) + 1
end

bins.each_with_index do |bin, idx|
  puts "#{(idx*100) + 1}-#{(idx + 1)*100}: #{bin}" if bin != nil
end
  

  
=begin
1-100: 3
101-200: 3
201-300: 6
301-400: 1
401-500: 4
801-900: 1
901-1000: 1
1201-1300: 1
=end
