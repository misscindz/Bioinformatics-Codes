#!/usr/bin/ruby
# Ching, Wang, Achoukhi
# Aufgabe 9.4
# backwards


if ARGV.length != 1
  STDERR.puts "Usage: #{$0} dateiname"
  exit 1
end
  # Dateiname einlesen, in dem der String steht
  dateiname = ARGV[0]

  # oeffnen der Datei oder Fehlermeldung, falls Datei nicht geoeffnet werden kann oder nicht existiert   
  if File.exist?(dateiname)
    begin
      datei = File.new(dateiname, "r")
    rescue => err
      STDERR.puts "Kann den File #{dateiname} nicht oeffnen: #{err}"
      exit 1
    end
  else
    STDERR.puts "File #{dateiname} existiert nicht!"
    exit 1
  end

  # string lesen, welcher in der Datei steht und in string speichern
  string = datei.read

  # '/n' entfernen
  string.chomp!

  # Datei schließen
  datei.close
 
  # Ausgabe der Strings 
  puts "\nHier der eingelesene String:\n"
  puts "#{string}\n\n"
  stringrev = string.reverse
  puts "Hier der eingelesene String verkehrt herum:\n"
  puts "#{stringrev}\n\n"
  
