#!/usr/bin/env ruby

require "set"

def parse_match_lists(filename)
  firstmatch = true
  currentseq = nil
  currentnum = nil
  pos_set = Set.new()
  matchlist = Array.new()
  File.open(filename,"r").each_line do |line|
    m = line.match(/^#\t(\d+)\t(\S+)$/)
    if m
      if firstmatch
        firstmatch = false
      else
        matchlist.push([currentnum, currentseq, pos_set.dup])
        pos_set.clear()
      end
      currentnum = m[1].to_i
      currentseq = m[2]
    else
      m = line.match(/^(\d+)$/)
      if m
        pos_set.add(m[1].to_i)
      end
    end
  end
  if not firstmatch
    matchlist.push([currentnum, currentseq, pos_set.dup])
    pos_set.clear()
  end
  return matchlist
end

if ARGV.length != 2
  STDERR.puts "#{$0} <matchfile_1> <matchfile_2>"
  exit 1
end

matchlist0 = parse_match_lists(ARGV[0])
matchlist1 = parse_match_lists(ARGV[1])

if matchlist0.length != matchlist1.length
  STDERR.puts "#{$0}: matchlist0.length = #{matchlist0.length} != " +
              "#{matchlist1.length} = matchlist1.length"
  exit 1
end

matchlist_length = 0
0.upto(matchlist0.length-1) do |idx|
  m0_seqnum, m0_seq, m0_pos_set = matchlist0[idx] 
  m1_seqnum, m1_seq, m1_pos_set = matchlist1[idx] 
  # puts "#{m0_seqnum} #{m0_seq} #{m0_pos_set.to_a}"
  matchlist_length += m0_pos_set.size
  if m0_seqnum != m1_seqnum or m0_seq != m1_seq or m0_pos_set != m1_pos_set
    STDERR.puts " m0=(#{m0_seqnum},#{m0_seq},#{m0_pos_set}) != " +
                "(#{m1_seqnum},#{m1_seq},#{m1_pos_set}) = m1"
    exit 1
  end
end
puts "#{matchlist0.length} matchlists of total length #{matchlist_length} compared"
