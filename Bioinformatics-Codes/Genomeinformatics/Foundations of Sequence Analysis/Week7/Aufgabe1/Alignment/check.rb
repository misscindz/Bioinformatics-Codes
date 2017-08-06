#!/usr/bin/env ruby

if ARGV.length != 2
  puts "Usage: #$0 testprogram testinput"
  exit 0
end

File.open(ARGV[1], 'r') do |file|
  file.each_line do |line|
    next if line.match /^#/
    m = line.match /^(\w+)\s+(\w+)\s+(\d+)$/
    puts `#{ARGV[0]} #{m[1]} #{m[2]}`
    if $? != 0
      exit 1
    end
  end
end
