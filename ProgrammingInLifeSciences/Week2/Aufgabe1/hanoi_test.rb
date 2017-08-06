#!/usr/bin/env ruby

class HanoiTower

  def initialize(n_dishes = 0)
    @dishes = []
    n_dishes.downto(1) {|dish| self.push(dish)}
  end

  def push(dish)
    if !@dishes.empty? and dish > @dishes.last
      raise "Staple broken: #{dish} > #{@dishes.last}"
    end
    @dishes.push(dish)
  end

  def pop
    if @dishes.empty?
      raise "Attempted to move from empty staple"
    end
    @dishes.pop
  end

  def empty?
    @dishes.empty?
  end

  def inspect
    @dishes.inspect
  end

end

class HanoiTowers

  NTowers = 3
  GoalTower = 1

  def initialize(n_dishes)
    @towers = []
    @towers << HanoiTower.new(n_dishes)
    (NTowers - 1).times {|i| @towers << HanoiTower.new}
  end

  def solved?
    @towers.each_with_index do |tower, i|
      return false if i != GoalTower and !tower.empty?
    end
    true
  end

  def move(from, to)
    @towers[to].push(@towers[from].pop)
  end

  def move_from_file(step)
    if step =~ /\((\d+),(\d+)\)/
      from = Integer($1)
      raise "Wrong tower number: #{from}" if from > NTowers or from < 1
      to = Integer($2)
      raise "Wrong tower number: #{to}" if to > NTowers or to < 1
      begin
        move(from-1,to-1)
      rescue => err
        STDERR.puts "Error during step #{step}\n#{err}"
        exit 1
      end
    else
      raise "Format error, step unrecognized: #{step.inspect}"
    end
  end

  def inspect
    out = []
    NTowers.times do |i|
      out << "Content of Tower #{i+1}}: #{@towers[i].inspect}"
    end
    out.join("\n")
  end

end

usage = <<-END
$0 <solution_file> <n_dishes>
END

if ARGV.size != 2
  STDERR.puts usage
  exit 1
end
solution_file = ARGV[0]
n_dishes = ARGV[1]

solution = File.read(solution_file)
n_dishes = Integer(n_dishes)
solution_steps = solution.split(" ")

problem = HanoiTowers.new(n_dishes)
solution_steps.each do |step|
  problem.move_from_file(step)
end
if problem.solved?
  puts "Problem solved correctly."
  exit 0
else
  STDERR.puts "Problem not solved correctly."
  STDERR.puts problem.inspect
  exit 1
end
