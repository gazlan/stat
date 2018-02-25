/* ******************************************************************** **
** @@ Stat
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Note   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include <math.h>
#include <float.h>

#include "..\shared\mmf.h"
#include "..\shared\file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#define  LOG2(x)           (log(x) / log(2))

#define ASCII_SIZE         (256)

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

extern DWORD   dwKeepError = 0;

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Proceed()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void  Proceed(const char* const pszFileName)
{
   MMF   MF;

   if (!MF.OpenReadOnly(pszFileName))
   {
      // Error !
      return;
   }

   BYTE*    pBuf   = MF.Buffer();
   DWORD    dwSize = MF.Size();

   DWORD    pFrq[ASCII_SIZE];

   memset(pFrq,0,sizeof(DWORD) * ASCII_SIZE);

   for (DWORD ii = 0; ii < dwSize; ++ii)
   {
      ++pFrq[pBuf[ii]];      
   }
   
   MF.Close();

   DWORD    dwCnt = 0;

   double   M = 0.0;
   double   D = 0.0;

   for (ii = 0; ii < ASCII_SIZE; ++ii)
   {
      if (pFrq[ii])
      {
         M += (double)pFrq[ii];

         ++dwCnt;
      }
   }

   M /= (double)dwCnt;

   double      pProbality[ASCII_SIZE];

   memset(pProbality,0,ASCII_SIZE * sizeof(double));

   for (ii = 0; ii < ASCII_SIZE; ++ii)
   {
      if (pFrq[ii])
      {
         D += ((double)pFrq[ii] - M) * ((double)pFrq[ii] - M);

         pProbality[ii] = pFrq[ii] / (double)dwSize;
      }
   }

   D /= (double)dwCnt;

   double   H0 = 0.0;

   for (ii = 0; ii < ASCII_SIZE; ++ii)
   {
      if (pProbality[ii] > FLT_EPSILON)
      {
         H0 += pProbality[ii] * LOG2(pProbality[ii]);
      }
   }

   H0 = -H0; // Invert

   printf("File size                   : %ld\n",dwSize);
   printf("Symbols count               : %d\n",dwCnt);
   printf("Zero | LF | CR | Tab | Space: %ld | %ld | %ld | %ld | %ld\n",pFrq['\x00'],pFrq['\x0A'],pFrq['\x0D'],pFrq['\x09'],pFrq['\x20']);
   printf("Biased sample variance      : %f\n",D);

   if (D > FLT_EPSILON)
   {
      printf("Biased standard deviation   : %f\n",sqrt(D));
      printf("Logarithmical variance      : %f, dB\n",10.0 * log10(D));
   }

   double      R = 1.0 - H0 / LOG2((double)dwSize);

   printf("Entropy                     : %G\n",(double)dwSize * H0);
   printf("Relative (bits/byte)        : %G\n",H0);
   printf("Redundancy                  : %G\n",R);
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void ShowHelp()
{
   const char  pszCopyright[] = "-*-  Stat  *  (c)  Gazlan,  2018  -*-";
   const char  pszDescript [] = "Calculate character statistics";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n\n",pszCopyright);
   printf("%s\n\n",pszDescript);
   printf("Usage: stat.com victim\n\n");
   printf("%s\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int main(int argc,char** argv)
{
   if (argc < 2)
   {
      ShowHelp();
      return 0;
   }

   if (argc == 2 && ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h"))))
   {
      ShowHelp();
      return 0;
   }

   Proceed(argv[1]);

   return 0;
}

/* ******************************************************************** **
**                That's All
** ******************************************************************** */
