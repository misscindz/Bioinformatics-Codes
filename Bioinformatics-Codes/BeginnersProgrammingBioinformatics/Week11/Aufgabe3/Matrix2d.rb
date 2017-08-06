#!/usr/bin/env ruby
#PBI Blatt11 Aufgabe2
#Abgabe 11.1.16
#Gruppe: Wang, Achoukhi, Ching

class Matrix2D
    
    @rows = 0
    @cols = 0
    
    def initialize(m, n)
        @num = Array.new(m) { Array.new(n) }
        @rows = m
        @columns = n
    end
    
    def set_value (i,j,value)
        if i > @rows or j > @columns
            raise(ArgumentError, "the input index of rows or columns is out of range")
        end
        @num[i][j] = value
    end

    def get_value (i,j)
        if i > @rows or j > @columns
            raise(ArgumentError, "the input index of rows or columns is out of range")
        end
        return @num
    end

    def get_number_of_rows()
    
    return @num.length

    end

    def get_number_of_columns()
    
    return @num[0].length
    end

    def pretty_print()
    
    @num.each do |row|
        row.each.with_index(1) do |value,index|
            if index == @columns - 1
                puts row.each{|value| value}.join(", ")
                   end
    
                end
            end
            
         end
    end

#Test#
m = Matrix2D.new(3,4)
m.set_value(1,1,9)
m.get_value(2,2)
m.get_number_of_rows
m.get_number_of_columns
m.pretty_print()


