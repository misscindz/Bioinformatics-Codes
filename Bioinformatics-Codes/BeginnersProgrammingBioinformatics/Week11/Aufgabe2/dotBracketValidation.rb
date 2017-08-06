#!/usr/bin/ruby
#PBI Blatt11 Aufgabe2
#Abgabe 11.1.16
#Gruppe: Wang, Achoukhi, Ching

class Stack

  def initialize ()
    @data = Array.new()
  end

  def push(x)
    @data.push(x)
  end

  def pop()
    if self.empty?
      raise "stack is empty!"
    end
    return @data.pop()
  end

  def top()
    if self.empty?
      raise "stack is empty!"
    end
    return @data.top
  end

  def empty?
    return @data.empty?
  end

end


def validDotBracket(dotbracket)
   
  leftbrackets = Stack.new
  rightbracket = 1
  basepairs = []
    
  dotbracket.split("").each_with_index do |element, i|

    if element == '('
      leftbrackets.push(i+1)
         
    elsif element == ')'
      if leftbrackets.empty?
        raise 'Missing "(" bracket.'
      end
        rightbracket= i+1
        basepairs.push("#{leftbrackets.pop()}-#{rightbracket}")
    
    elsif element == '.'
      
    else
      raise 'Your input string includes invalid characters.'
    end
  
  end
  
  basepairs
    
end


def resultcontrol (newfile)

  my_result = "valid"
  begin
    file = File.new(newfile,"r")
  rescue => err
    STDERR.puts "Could not open data #{newfile}: #{err}"
    exit 1
  end
    
  lines = file.readlines
  lines.each do |line|
    line = line.split(" ")
    notation = line[0]
    my_result = "valid"
    given_result = line[1]
        
    puts "\n"+ notation

    begin 
      puts validDotBracket(notation)        
    rescue => err
      STDERR.puts err
      my_result = "invalid"
    end
        
    if my_result != given_result
      puts "Method result: " + my_result.upcase 
      puts "Test-file result: " + given_result.upcase
      puts "Results not matching. Method is not working correctly.\n\n"
            
    elsif my_result == given_result
      puts "Method result: " + my_result.upcase 
      puts "Test-file result: " + given_result.upcase
      puts "Results matching. Method is working correctly.\n\n"
    end
        
  end
  
  file.close
        
end


if ARGV.empty?
  raise "\nUsage: [filename] [testfile]\n"
  exit 1
end

newfile = ARGV[0]
resultcontrol(newfile)