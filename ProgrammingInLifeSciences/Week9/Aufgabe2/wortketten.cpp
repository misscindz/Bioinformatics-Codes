//PFN Blatt 9
//Aufgabe 2
//Gruppe: Wang, Achoukhi, Ching
//Abgabe: 22.6.2016

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <queue>
using namespace std;
typedef vector<string> Wordlist;
typedef map<string, Wordlist> Adjacencemap;

void bfs(string start, string to, map<string ,vector<string> > previousWord){
    map<string,int> visited;
    map<string,bool> printed;
    string top;
    queue<string> q;
    q.push(start);
    visited[start] =1;
    while(q.empty() == false){
        string top = q.front();
        q.pop();
        if(top == to){
             cout<<"found chain of "<<visited[top]<<" words for words: "
             << start<<" and "<<to<<endl;
             printed.insert(pair<string, bool>(top, true));
        }
        else{
        int total = previousWord[top].size();
            for(int i = 0; i<total; ++i){
                /*count gives how many times one element appears*/
                if(!visited.count(previousWord[top][i])){
                    visited[previousWord[top][i]] = visited[top]+1;
                    q.push(previousWord[top][i]);
                }

            }

        }

        
    }
    
    if(printed[start] == false && printed[to] == false){
        cout<<"no chain found for words: "<<start<<" and "<<to<<endl;

    }

}

map<string, Wordlist> adj_diff(Wordlist wordlist){
    unsigned int i = 0, j = 0, k = 0,count_mismatch = 0;
    Adjacencemap adjacencemap;
    for(i = 0; i < wordlist.size();++i){
        for(j =0; j < wordlist.size();++j){
            if(wordlist.at(i).size() == wordlist.at(j).size()){
                count_mismatch = 0;
                    for(k = 0; k < wordlist[i].size();++k){
                        if(wordlist[i][k] != wordlist[j][k]){
                            count_mismatch++;
                
                        }
                    }
                    if(count_mismatch ==1){
                        adjacencemap[wordlist[i]].push_back(wordlist[j]);
                    }
            }
        
       }
    }
    return adjacencemap;
}

int main(){
    ifstream in_stream;
    ifstream wordpair;
    Wordlist wordlist;
    map<string, Wordlist> mymap;
    in_stream.open("5BuchstabenWoerter.txt");
    wordpair.open("Wortpaare.txt");
    string templine;
    string start, to;
    string line;
    
    if(!in_stream){
        cout<<"cannot open input file,\n";
        return 1;
    }
    
    while(in_stream){
        getline(in_stream,line);
        wordlist.push_back(line);
        
    }
    mymap = adj_diff(wordlist);

  
  if(!wordpair){
        cout<<"cannot open input file,\n";
        return 1;
    }
    
    while(getline(wordpair,templine,'\n')){
        vector<string> tempstr;
        istringstream ss(templine);
        ss>>start>>to;
        bfs(start, to, mymap);
    }
    in_stream.close();
    wordpair.close();
    return 0;
}
