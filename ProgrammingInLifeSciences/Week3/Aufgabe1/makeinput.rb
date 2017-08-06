#!/usr/bin/env ruby
f = File.new("input", "w")
a=[]
1000.times do
  x=rand(1000)
  if a.empty?
    a.push(x)
  else
    a.push(a.last + x)
  end
end
f.puts a.join(" "); f.close
