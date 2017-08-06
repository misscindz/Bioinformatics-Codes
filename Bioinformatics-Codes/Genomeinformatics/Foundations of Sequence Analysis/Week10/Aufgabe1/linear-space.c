/*  Aufgabe 10.1
    Gruppe 5: Achoukhi, Breitschuh, Ching, Moradi  */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

unsigned int *E, *R;


static unsigned int costs (const char x, const char y) {
  /* Verwendet wird die Levenshtein-Distanz (edit distance). */
  return x == y && x != 0 ? 0 : 1;
}

static unsigned int min3 (const unsigned int a, const unsigned int b,
                          const unsigned int c) {
  unsigned int tmp_min;
  tmp_min = (a < b) ? a : b;
  return (tmp_min < c) ? tmp_min : c;
}

static void firstEDtabRtabcolumn (const char *u, const unsigned int p) {
  unsigned int i;

  E[0] = 0; R[0] = 0;
  for(i = 1; i <= p; ++i) {
    E[i] = E[i-1] + costs(u[i-1], 0);
    R[i] = i;
  }
}

static void nextEDtabRtabcolumn (const char *u, const char b,
                                 const unsigned int j, const unsigned int j0,
                                 const unsigned int p) {
  unsigned int i, tmp_E, tmp_R, new_R = 0;
  unsigned int ins, del, rep, min;

  tmp_E = E[0]; tmp_R = R[0];
  E[0] = E[0] + costs(0, b);

  for(i = 1; i <= p; ++i) {
    del   = E[i-1] + costs(u[i-1], 0);
    ins   = E[i]   + costs(0, b);
    rep   = tmp_E  + costs(u[i-1], b);
    min = min3(del, ins, rep);
    tmp_E = E[i]; E[i] = min;

    if(j > j0) {
      if(min == ins) { new_R = R[i]; }
      else if(min == rep) { new_R = tmp_R; }
      else if(min == del) { new_R = R[i-1]; }
    } else { new_R = R[i]; }
    tmp_R = R[i]; R[i] = new_R;
  }
}

static void evaluateallcolumns (unsigned int j0,
                                const char *u, const char *v,
                                unsigned int p, unsigned int q) {
  unsigned int j;

  firstEDtabRtabcolumn(u, p);
  for(j = 1; j <= q; ++j) {
    nextEDtabRtabcolumn(u, v[j-1], j, j0, p);
  }
}

static unsigned int evaluatecrosspoints (unsigned int *C,
                                         const char *u, const char *v,
                                         unsigned int i, unsigned int j,
                                         unsigned int p, unsigned int q) {
  unsigned j0, i0;

  if (q >= 2) {
    j0 = q / 2;
    evaluateallcolumns(j0, u+i-1, v+j-1, p, q);
    i0 = R[p];
    C[j - 1 + j0] = i - 1 + i0;
    evaluatecrosspoints(C, u, v, i,      j,      i0,     j0    );
    evaluatecrosspoints(C, u, v, i + i0, j + j0, p - i0, q - j0);
  }
  return E[strlen(u)];
}

int main (int argc, char *argv[]) {
  unsigned int *C, m, n, edist, i;
  char *u, *v;

  if(argc!=3) {
    fprintf(stderr, "ERROR: Wrong number of arguments\n");
    fprintf(stderr, "Usage: %s <seq1> <seq2>\n", __FILE__);
    return EXIT_FAILURE;
  }
  u = argv[1]; v = argv[2];
  m = strlen(u); n = strlen(v);
  E = calloc((m + 1), sizeof(*E));
  R = calloc((m + 1), sizeof(*R));
  C = calloc((n + 1), sizeof(*C));
  C[0] = 0; C[n] = m;

  printf("sequence u (of length m=%d): %s\n", m, u);
  printf("sequence v (of length n=%d): %s\n", n, v);
  edist = evaluatecrosspoints(C, u, v, 1, 1, m, n);
  printf("distance=%u\ncrosspoints:\n", edist);
  for(i = 0; i <= n; ++i) {
    printf("%u\n", C[i]);
  }
  free(C); free(E); free(R);
  return EXIT_SUCCESS;
}