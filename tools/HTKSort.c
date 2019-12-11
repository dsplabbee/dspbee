/*
$Id: HTKSort.c,v 1.1 2000/02/04 11:46:50 ftj Exp $

HTKSort.c

Generate sorted unique strings from input file.

This program has been written to avoid problems with Unix
sorting of SpeechDat(II) lexica.

Will match the HTK-assumption of a sorted lexicon.

Unchanged in Masper - MonoTrain

# Version 1.0  13.05.99  Borge Lindberg

$Log: HTKSort.c,v $
Revision 1.1  2000/02/04 11:46:50  ftj
Combined trntools and tsttools into a single tools directory

Revision 1.3  1999/12/10 11:45:22  ftj
Added history log

*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_STRING_LENGTH 1024

FILE *ifile;
FILE *ofile;

char globs1[MAX_STRING_LENGTH], globs2[MAX_STRING_LENGTH];

char **Strings = NULL;

/*****************************************************************************/
void exch(long i, long j)
/*****************************************************************************/
{ char *s1;

  s1 = Strings[i];
  Strings[i] = Strings[j];
  Strings[j] = s1;
}


/*****************************************************************************/
/* Substitutes strcmp: before comparing the two strings, it removes all      */
/*                     the "\" (back slashes)                                 */
int stringcompare (char *s1, char *s2)
/*****************************************************************************/
{ char *gsp1 = globs1, *gsp2 = globs2;

  while(*s1 != '\0') {
    if(*s1 == '\\') s1++;
    else
      *gsp1++ = *s1++;
  }
  *gsp1 = '\0';

  while(*s2 != '\0') {
    if(*s2 == '\\') s2++;
    else
      *gsp2++ = *s2++;
  }
  *gsp2 = '\0';
   
  return(strcmp(globs1,globs2));
}

/*****************************************************************************/
void split(long first, long last, long *lowret, long *highret)
/*****************************************************************************/
{
  long low,high;
  char *chp = Strings[(first+last)/2];

  low = first;
  high = last;

  do
  {
    while (stringcompare(chp,Strings[low]) > 0)
      low++;
    while (stringcompare(chp,Strings[high]) < 0)
      high--;
    if (low <= high)
    {
      exch(low,high);
      low++;
      high--;
    }
  } while (low <= high);

  *lowret = low;
  *highret = high;
}

/* Quick sort. */

/*****************************************************************************/
void quick(long first, long last)
/*****************************************************************************/
{
  long low,high;

  if (first < last)
  {
    split(first,last,&low,&high);
    if (first < high)
      quick(first,high);
    if (low < last)
      quick(low,last);
  }
}

/*****************************************************************************/
barf(fn)
/*****************************************************************************/
char *fn ;
{
  fprintf(stderr, "cannot open %s\n", fn) ;
  exit(1) ;
}

 
/*****************************************************************************/
int main(int argc, char *argv[], char *envp[])
/*****************************************************************************/
{ char ss[MAX_STRING_LENGTH];
  long NStrings = 0;
  long i, j, k;
    
  ifile = stdin ;
  ofile = stdout ;

  argv++ ;
  if (*argv) {
    if ((ifile = fopen(*argv, "rt")) == NULL) barf(*argv) ;
    argv++ ;
    if (*argv) {
      if ((ofile = fopen(*argv, "wt")) == NULL) barf(*argv) ;
      argv++ ;
      if (*argv) {
	fprintf(stderr, "usage: HTKSort [input [output]]\n") ;
	exit(1) ;/* Return with error */
      }
    }
  }

/*****************************************************************************/
/*   OK, now we are ready                                                    */
/*****************************************************************************/

  Strings = malloc (sizeof (char *));
  if (Strings == NULL) 
  { fprintf(stderr, "Memory allocation error in HTKSort");  exit(1); }

  while (fgets(ss, MAX_STRING_LENGTH, ifile))
  { int unique_string = 1;

    if (strlen (ss) >= (MAX_STRING_LENGTH-2)) {
      fprintf(stderr, "You are trying to sort a "
      			"file which have lines longer than %0d chars.\n"
	                "Goodbye.\n", 
      			MAX_STRING_LENGTH);  exit(1); }

    if (strlen(ss) > 0)
      ss[strlen(ss)-1] = 0;

    for (i=0; i<NStrings;i++)
     if ((unique_string = strcmp(ss, Strings[i])) == 0)
        break;

    if (unique_string != 0)
    { Strings = realloc(Strings, (NStrings+1) *  sizeof(char *));
      if (Strings == NULL) { 
        fprintf(stderr, "Memory allocation error in HTKSort");  exit(1); }

      Strings[NStrings] = malloc(strlen(ss) + 1);
      if (Strings == NULL) { 
        fprintf(stderr, "Memory allocation error in HTKSort");  exit(1); }

      strcpy(Strings[NStrings], ss);
      NStrings++;
    }
  }

  quick((long) 0,NStrings-1);
  
  for (i=0; i<NStrings;i++)
    fprintf(ofile, "%s\n", Strings[i]);

  return 0; /* Return with no errors */
}
