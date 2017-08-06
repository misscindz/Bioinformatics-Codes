#! /usr/bin/env ruby

# Gruppe: Achoukhi, Breitschuh, Ching

def lcplen (useq, u_start, u_end,
            vseq, v_start, v_end)
  u_length, v_length = u_end - u_start, v_end - v_start
  if !u_length || !v_length
    return 0
  end
  counter = 0
  0.upto([u_length, v_length].min - 1) do
    if useq[u_start+counter] == vseq[v_start+counter]
      counter += 1
    else
      return counter
    end
  end
  return counter
end

def outsense_next_front (d, useq, ulen, vseq, vlen, front)
  (-1*d).upto(d) do |h|
    l = 0
    if front.has_key?([h-1,d-1])
      l = [l, front[[h-1,d-1]]].max
    end
    if front.has_key?([h+1,d-1])
      l = [l, front[[h+1,d-1]] + 1].max
    end
    if front.has_key?([h,d-1])
      l = [l, front[[h,d-1]] + 1].max
    end
    front[[h,d]] = l + lcplen(useq, l, ulen, vseq, h+l, vlen)
  end
end

def outsense_edist (useq, ulen, vseq, vlen)
  front = Hash.new
  front[[0,0]] = lcplen(useq, 0, useq.length, vseq, 0, vseq.length)
  if front.has_key?([vlen-ulen,0]) and front[[vlen-ulen,0]] == ulen
    return 0
  end
  1.upto([ulen,vlen].max) do |d|
    outsense_next_front(d,useq,ulen,vseq,vlen,front)
    if (-1*d) <= vlen-ulen && vlen-ulen <= d && front[[vlen-ulen,d]] == ulen
      return d
    end
  end
end

if ARGV[0] && ARGV[1]
  useq, vseq = ARGV[0], ARGV[1]
else
  $stderr.puts "Error: Please entry two strings to compare"
  $stderr.puts "Usage: #{$0} <string1> <string2>"
  exit(1)
end
ulen, vlen = useq.length, vseq.length
puts "edist = #{outsense_edist(useq,ulen,vseq,vlen)}"