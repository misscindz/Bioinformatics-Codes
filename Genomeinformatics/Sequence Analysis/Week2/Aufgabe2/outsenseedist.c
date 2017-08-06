// Übungblatt 1 Aufgabe 2
// Cindy Ching, Hendrik Breitschuh, Asmaa Achoukhi 
// 11.04.17
// Aufruf: ./outsenseedist.x -w <q>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))


// berechnet den längsten gemeinsamen Pärfix beider Substrings u und v. 
static unsigned long lcplen(const unsigned char *s,const unsigned char *send,
                            const unsigned char *t,const unsigned char *tend)
{
    unsigned long lcplen = 0;
    while(s <= send && t <= tend && *s == *t)
    {
        lcplen += 1;
        s++;
        t++;
    }
    return lcplen;
}

// berechnet aus der Vorgängergeneration d-1 die aktuellen Frontwerte der Generation d.
static void outsense_next_front(unsigned long *destfront,
                                const unsigned long *sourcefront,
                                unsigned long d,
                                const unsigned char *useq,
                                unsigned long ulen,
                                const unsigned char *vseq,
                                unsigned long vlen)
{
    unsigned long l,h;
        
    for (h = 0; h < 2*d+1; h++) // 2*d+1 Frontwerte für jedes d.
    {
        l = 0;
        if (h > 1)
        {
            l = MAX(l, sourcefront[h-2]);  
        }
        if (h < 2*d-1)
        {
            l = MAX(l, sourcefront[h]+1); 
        }
        if (h < 2*d && h > 0)
        {
            l = MAX(l, sourcefront[h-1]+1);  
        }
        destfront[h] = l + lcplen(useq+l, useq + ulen-1, vseq+l+h-d , vseq + vlen-1);
    }
}

// berechnet die EditDistanz von den beiden Sequenzen useq und vseq.
// stellt den Speicherplatz fuer die jeweiligen Generationen bereit.
unsigned long outsense_edist(void *info,
                             const unsigned char *useq,
                             unsigned long ulen,
                             const unsigned char *vseq,
                             unsigned long vlen)
{
    unsigned long d = 0;
    unsigned long *first_front = NULL;
    unsigned long *next_front = NULL;
 
    // front(0,0) berechnen.
    first_front = calloc(1,sizeof(*first_front));
    assert(first_front);
    first_front[0] = lcplen(useq, useq + ulen-1, vseq, vseq + vlen -1);
    if (first_front[0] == ulen && ulen == vlen)
    {
        free(first_front);
        return 0;
    }
    // Alle anderen Generationen von Frontwerten berechnen.
    for (d = 1 ; d <= MAX(ulen,vlen); d++)
    {
        next_front = calloc(2*d+1,sizeof(*next_front));
        assert(next_front);
        outsense_next_front(next_front,first_front,d,useq,ulen,vseq,vlen);
        // Abbruchkriterium
        if (abs(vlen-ulen) <= d && next_front[vlen-ulen + d] == ulen)
        {
          free(next_front);
          free(first_front);
          return d;             // EditDistanz bestimmt.
        }
        free(first_front);
        first_front = next_front;     
    }
    return(EXIT_FAILURE);
}
