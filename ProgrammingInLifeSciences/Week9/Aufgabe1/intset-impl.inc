#include "intset.hpp"
#include <cstdlib>
#include <cstdbool>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <typeinfo>

#define BITS_FOR_SIZE(SIZE)     ((SIZE) * CHAR_BIT)
#define ELEM2SECTION(LOGVAL,X)  ((X) >> (LOGVAL))
#define SECTIONMINELEM(S)       ((S) << IntSet::logsectionsize)

//Constructor
template <typename Basetype>
IntSet<Basetype>::IntSet(unsigned long maxelement, unsigned long nofelements) :
    maxelement(maxelement), nofelements(nofelements) {
      this->nextfree = 0;
      this->previouselem = ULONG_MAX;
      this->currentsectionnum = 0;
      this->logsectionsize = BITS_FOR_SIZE(sizeof(Basetype));
      this->numofsections = ELEM2SECTION(this->logsectionsize, maxelement) + 1;
      this->elements = new Basetype[nofelements];
      this->sectionstart = new unsigned long [numofsections+1];
		
      this->sectionstart[0] = 0;
      unsigned long idx;
      for (idx = 1; idx <= this->numofsections; idx++)
      {
        this->sectionstart[idx] = nofelements;
      }
    }

template <typename Basetype>
IntSet<Basetype>::~IntSet(){
    delete [] this->elements;
    delete [] this->sectionstart;
}

template <typename Basetype>
size_t IntSet<Basetype>::size(unsigned long maxelement,unsigned long nofelements)
{
  int logsectionsize = BITS_FOR_SIZE(sizeof(Basetype));
  return sizeof (Basetype) * nofelements +
         sizeof (unsigned long) * (ELEM2SECTION(logsectionsize,maxelement) + 1);
}

template <typename Basetype>
void IntSet<Basetype>::add(unsigned long elem)
{
  assert(this->nextfree < this->nofelements &&
         elem <= this->maxelement &&
         (this->previouselem == ULONG_MAX || this->previouselem < elem));
  while (elem >= SECTIONMINELEM(this->currentsectionnum+1))
  {
    assert(this->currentsectionnum < this->numofsections);
    this->sectionstart[this->currentsectionnum+1] = this->nextfree;
    this->currentsectionnum++;
  }
  assert(SECTIONMINELEM(this->currentsectionnum) <= elem &&
         elem < SECTIONMINELEM(this->currentsectionnum+1) &&
         ELEM2SECTION(this->logsectionsize,elem) ==
           this->currentsectionnum);
  this->elements[this->nextfree++] = (Basetype)elem;
  this->previouselem = elem;
}


template <typename T>
static bool binarysearch_is_member(const T *leftptr,
                                   const T *rightptr,
                                   T elem)
{
  const T *midptr;

  while (leftptr <= rightptr)
  {
    midptr = leftptr + (((unsigned long) (rightptr-leftptr)) >> 1);
    if (elem < *midptr)
    {
      rightptr = midptr-1;
    } else
    {
      if (elem > *midptr)
      {
        leftptr = midptr + 1;
      } else
      {
        return true;
      }
    }
  }
  return false;
}

template <typename Basetype>
bool IntSet<Basetype>::is_member(unsigned long elem)const
{
  if (elem <= this->maxelement)
  {
    const unsigned long sectionnum = ELEM2SECTION(this->logsectionsize,elem);

    if (this->sectionstart[sectionnum] < this->sectionstart[sectionnum+1])
    {
      return binarysearch_is_member<Basetype>(this->elements +
                                    this->sectionstart[sectionnum],
                                    this->elements +
                                    this->sectionstart[sectionnum+1] - 1,
                                    (Basetype) elem);
    }
  }
  return false;
}


template <typename Basetype>
unsigned long binarysearch_smallest_g(const Basetype *leftptr,
                                             const Basetype *rightptr,
                                             Basetype pos)
{
  const Basetype *midptr, *found = NULL, *leftorig = leftptr;

  assert(leftptr <= rightptr);
  if (pos < *leftptr)
  {
    return 0;
  }
  if (pos > *rightptr)
  {
    return 1UL + (unsigned long) (rightptr - leftptr);
  }
  assert(pos > *leftptr && pos < *rightptr);
  while (leftptr <= rightptr)
  {
    midptr = leftptr + ((unsigned long) (rightptr - leftptr) >> 1);
    if (pos < *midptr)
    {
      rightptr = midptr - 1;
    } else
    {
      found = midptr;
      if (pos > *midptr)
      {
        leftptr = midptr + 1;
      }
      else
        break;
    }
  }
  /* not allowed by exercise! */
  assert(*found != pos);
  assert(found != NULL && found >= leftorig);
  return 1UL + (unsigned long) (found - leftorig);
}

template <typename Basetype>
unsigned long IntSet<Basetype>::number_next_larger(unsigned long pos)const
{
  unsigned long sectionnum = ELEM2SECTION(this->logsectionsize,pos),
                result = this->sectionstart[sectionnum];

  assert(pos <= this->maxelement);

  if (pos > this->previouselem)
    return this->nofelements;

  if (this->sectionstart[sectionnum] < this->sectionstart[sectionnum+1])
  {
    result += binarysearch_smallest_g(
                      this->elements + this->sectionstart[sectionnum],
                      this->elements + this->sectionstart[sectionnum+1] - 1,
                      (Basetype) pos);
  }
  return result;
}

template <typename Basetype>
void IntSet<Basetype>::pretty_print(void)const
{
  unsigned long idx, sectionnum = 0;

  assert(this->nextfree > 0);
  for (idx = 0; idx < this->nextfree; idx++)
  {
    while (idx >= this->sectionstart[sectionnum+1])
    {
      sectionnum++;
    }
    printf("%lu%s",SECTIONMINELEM(sectionnum) + this->elements[idx],
                   idx < this->nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx < this->nextfree; idx++)
  {
    printf("%hu%s",this->elements[idx],
                   idx < this->nextfree - 1 ? "&" : "\\\\\n");
  }
  sectionnum = 0;
  for (idx = 0; idx < this->nextfree; idx++)
  {
    while (idx >= this->sectionstart[sectionnum+1])
    {
      sectionnum++;
    }
    printf("%lu%s",sectionnum,
                   idx < this->nextfree - 1 ? "&" : "\\\\\n");
  }
  for (idx = 0; idx <= this->numofsections; idx++)
  {
    printf("%lu%s",this->sectionstart[idx],
                   idx < this->numofsections ? "&" : "\\\\\n");
  }
}

