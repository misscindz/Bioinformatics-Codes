//Gruppe: Ching, Wang, Achoukhi
//PFN Blatt 7, Aufgabe 2
//Abgabe 8.6.2016

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

void enumkmers(char *word, char *permutation, int n, int i, int k){
	int j;
	if (i == k){
		printf("%s\n",permutation);
		return;
	}
	else{
		for(j=0;j<n;j++){
				permutation[i] = word[j];
				enumkmers(word, permutation, n, i+1, k);
		}
	}
}


int main(int argc, char *argv[]){
	int i=0,n=0,k=0;
	char *word=NULL, *permutation=NULL;
	word = malloc(50*sizeof(char));
	assert(word!=NULL);
	
	if((argc<3) || (sscanf(argv[1], "%s",word)!= 1 
                        || sscanf(argv[2], "%d",&k)!= 1)){
        fprintf(stderr,"Usage: %s <string> <number>", argv[0]);                        
    }
	
	permutation = malloc((k+1)*sizeof(char));
	
	n = strlen(word);
	i = strlen(permutation);
	
	enumkmers(word, permutation, n, i ,k);
	
	free(word);
	free(permutation);
	return EXIT_SUCCESS;
}
