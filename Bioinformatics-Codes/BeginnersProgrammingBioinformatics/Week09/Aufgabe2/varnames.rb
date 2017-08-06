#!/usr/bin/ruby
# This small example script is meant to be corrected. Some of the variable
# identifiers do not follow the rules of Ruby and/or do not follow the Ruby
# naming conventions.
# Additionally add comments to the identifiers to classify them as either
# *name
# *global
# *function
# *classname
# *instance variable    
# *class variable      

class Humanbeing			# classname
  #counts the number of instances created
  @@hbcounter = 0			# class variable
  def initialize			# function 
    @@hbcounter += 1			# class variable
  end
  #getter function
  def name				# function 
    @firstname + " " + @secondname	# instance variable
  end
  #setter functions
  def firstname(name)          		# function 
    @firstname = name 			# instance variable
  end
  def secondname(name)			# function 
    @secondname = name			# instance variable
  end
end

person = Humanbeing.new			# class variable

person.firstname("Peter")		# instance variable
person.secondname("Petersen")  		# instance variable
puts person.name

exit 0			
