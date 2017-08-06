/* Aufgabe 4.1, Gruppe 5: Achoukhi, Breitschuh, Ching, Moradi */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void match_count (char *u, char *v, unsigned int k,
                  unsigned int m, unsigned int n) {
  unsigned int i, matchcount = 0;
  int startpos, start_u, start_v;
  
  for(startpos = (int)(k-m); startpos <= (int)(n-k); ++startpos) {
    if(startpos < 0) {
      start_u = -1 * startpos;
      start_v = 0;
    } else {
      start_u = 0;
      start_v = startpos;
    }
    
    matchcount = 0;
    for(i = 0; i < k; ++i) {
      if(u[i+start_u] == v[i+start_v]) matchcount++;
    }
    printf("mc(%.*s,%.*s) = %d\n", k, u+start_u, k, v+start_v, matchcount);
    
    for(i = k; (i+start_u < m) && (i+start_v < n); ++i) {
      if(u[i+start_u-k] == v[i+start_v-k]) matchcount--;
      if(u[i+start_u]   == v[i+start_v])   matchcount++;
      printf("mc(%.*s,%.*s) = %d\n",
             k, u+start_u+(i-k+1), k, v+start_v+(i-k+1), matchcount);
    }
  }
}


int main (int argc, char *argv[]) {
  char *u, *v;
  unsigned int k, m, n;
  
  if(argc != 4) {
    fprintf(stderr, "USAGE: %s <string u> <string v> <window k>\n", argv[0]);
    return EXIT_FAILURE;
  }
  u = argv[1]; m = strlen(u);
  v = argv[2]; n = strlen(v);
  if (sscanf(argv[3], "%d", &k) != 1) {
    fprintf(stderr, "ERROR: k should be an integer <= the length of m or n\n");
    return EXIT_FAILURE;
  }
  if(k == 0) {
    fprintf(stderr, "ERROR: k should be > 0\n");
    return EXIT_FAILURE;
  } else if(k > m || k > n) {
    fprintf(stderr, "ERROR: k should be <= the length of m or n\n");
    return EXIT_FAILURE;
  }

  match_count(u, v, k, m, n);
  
  return EXIT_SUCCESS;
}

/* AS: 4 Punkte, damit klar ist was ihr vorhabt eure Logik in solchen Zeilen
 * wie 13-20 bitte kommentieren
 */
