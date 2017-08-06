#include <iostream>
#include <string>
#include <fstream>

/* Aufruf: g++ -o my_first_program my_first_program.cpp */
/* ./my_first_program some_dud_ligands.smi output_file.smi */

using namespace std;


// answers of questions
int answer()
{
  std::cout << "\t1.Um zu kompilieren benoetigt man ein Compiler,z.B.\n"
  << "\teignet sich der g++ Compiler. Fuer den Aufruf gibt man \n"
  << "\tden Befehl: g++ -o 'sourcefile' 'outputfile' ein." <<endl;
  std::cout << "\t2.Warning option sind diagnostische Nachrichten,\n"
  <<"\tdie bei Risiken oder moegliche Fehler ausgegeben werden.\n"
  <<"\tBeispiel: -Werror: Wandelt Alle Warnungen in Error um.\n"
  <<"\tOptimaziation Option sind Funktionen, die Beispielsweise benutzt\n"
  <<"\twerden um die Laufzeit zu verbessern oder den Speicherplatz\n"
  <<"\tzu verringern.Bsp: -ftree-loop-optimize: führt loop-Optimierung\n"
  <<"\tin Bäumen aus."<<endl;
  std::cout << "\t3. Mit der Compiler Option -Wall können alle Fhelermeldungen\n"
  << "\tangezeigt werden" <<endl;
  return 0;
}

// Work with some_dud_ligands.smi
 
// Number of molecules in input_file
int nof_molecules(string input_file)
{
  ifstream inFile(input_file.c_str());
    if(inFile.is_open())
    {
      string line;
      int count = 0;
      while(getline(inFile,line))
      {
        count++;
      }
      //inFile.close();
      cout << "\tThe Number of molecules in " << input_file << " is " <<count<<std::endl;
    }
    else
    {
      cerr << "File can not open.\n "<< std::endl;
    }
}

// Numer of atoms in input_file     
int find_space_idx(string line)
{
  
  for (int idx = 0; idx < line.length(); idx++)
  {
    if(isspace(line[idx]))
    {
      return idx;
    }
  }
  return -1;
}      
    
int count_atoms(string line)
{
  int count = 0, i;
  for (i =0; i< line.length(); i++)
  {
    if(isalpha(line[i]))
    {
      count++;
    }
  }
  return count;
}  

int nof_atoms(string input_file)
{
  ifstream inFile(input_file.c_str());
  if(inFile.is_open())
    {
    string line;
    int count = 0, space_idx = 0;
    while(getline(inFile,line))
    { 
      space_idx = find_space_idx(line);
      if(space_idx != -1)
      { 
        count = count + count_atoms(line.substr(0,(space_idx-1)));
      }      
    }
    inFile.close();
    cout << "\tThe Number of atoms in " << input_file << " is " <<count<<std::endl;
  }
  else
  {
    cerr << "File can not open.\n "<< std::endl;  
  } 
}

// write smiles string in output_file
int create_smiles(string input_file, string output_file)  
{
  ifstream inFile (input_file.c_str());
  ofstream outFile (output_file.c_str());
  if(inFile.is_open() && outFile.is_open())
  {
    string line;
    int space_idx = 0;
    while(getline(inFile,line))
    {
      space_idx = find_space_idx(line);
      outFile << line.substr(0,space_idx-1) << std::endl;
    }
    inFile.close();
    outFile.close();
    return 0;
  }
  else
  {
    cerr << "File can not open\n"<< std::endl;
    return 1;
  }
}  
   
int main(int argc, char *argv[]) 
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " <input_file>" " <output_file>" << std::endl;
    return 1;  
  }
  else
  {
    string input_file = argv[1];
    string output_file = argv[2];
    
    answer();
    nof_molecules(input_file);
    nof_atoms(input_file);
    cout<< "\t5 elements in smiles_file:\n"
    <<"\tNitrogen(N), Oxygen(O), Fluor(F), Sulfur(S), Carbonyl(C)" << std::endl;
    create_smiles(input_file,output_file);     
  }  
  return 1;
}
      
