#! usr/bin/env ruby
#  Aufgabe 8.2, Gruppe 5: Achoukhi, Breitschuh, Ching, Moradi

def costs (x, y)
  # Verwendet wird die Levenshtein-Distanz (edit distance).
  return x == y && x != 0 ? 0 : 1
end

def firstEDtabRcolumn (e, r, u)
  e[0], r[0] = 0, nil
  for i in 1..u.length
    e[i]= e[i-1] + costs(u[i-1], 0)
  end
end

def nextEDtabRcolumn(e, r, u, b, j, j0)
  tmp_e, tmp_r = e[0], r[0]
  e[0], r[0] = e[0] + costs(0, b), nil
  
  for i in 1..u.length
    del = e[i-1] + costs(u[i-1], 0)
    ins = e[i]   + costs(0, b)
    rep = tmp_e  + costs(u[i-1], b)
    min = [del, ins, rep].min
    tmp_e, e[i] = e[i], min
    
    if    j  < j0 then new_r = nil
    elsif j == j0 then new_r = i
      elsif min == ins then new_r = r[i]
      elsif min == rep then new_r = tmp_r
      elsif min == del then new_r = r[i-1]
    end
    tmp_r, r[i] = r[i], new_r
  end
end

def evaluateallcolumns (e, r, u, v)
  j0 = v.length/2
  firstEDtabRcolumn(e, r, u)
  for j in 1..v.length
    nextEDtabRcolumn(e, r, u, v[j-1], j, j0)
  end
  puts "edist: #{e[u.length]}"
  puts "r = #{r}"
end

u, v = String(ARGV[0]), String(ARGV[1])
if u == "" || v == ""
  STDERR.puts "Error: Input has to include two strings"
  abort "Usage: #{__FILE__} <String u> <String v>"
end
e = Array.new(u.length+1)
r = Array.new(u.length+1)
evaluateallcolumns(e, r, u, v)
