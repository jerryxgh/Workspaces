/* Filename:  ~\lisp\lisp.c            Revision Date: July 23, 2007 */
/*****************************************************************************

                        LISP  INTERPRETER
                        -----------------

This progam is a GOVOL LISP interpreter.  This interpreter consists of
three major functions: SREAD, SEVAL, and SWRITE.  SREAD scans the
input string for input S-expressions (atoms and dotted pairs) and
returns a corresponding typed-pointer. The SEVAL function takes as
input a typed-pointer p to an input S-expression and evaluates it and
returns a typed pointer to its result.  SWRITE takes as input the
typed pointer returned from SEVAL and prints out the result.

LISP input lines beginning with a "/" are comment lines.  Indirect
input text is taken from a file Z to replace the directive of the form
"@Z".  SEVAL tracing can be turned on by using the directive "!trace",
and turned off with the directive "!notrace".

*****************************************************************************/

#define int16 int
#define int32 long
#define forward extern

#if defined(__GNUC__)
#  include "linuxenv.h"
#else
#  include "c:\csihead\turbcenv.h"
#endif
/* The above includes declare strlen(), strcpy(), strcmp(), calloc(),
   fflush(), fopen(), fclose(), fprintf(), sprintf(), fgetc(), labs(),
   floor(), and pow().  Also the type FILE is defined, and the longjump
   register-save structure template: jmp_buf is defined. This include will
   need to be constructed to conform to  any particular system. */

#if !defined(NULL)
#  define NULL 0L
#endif
#define EOF (-1)
#define EOS (0)

#define EQ ==
#define OR ||
#define AND &&
#define NOT !

#define n 1000
#define m 6000
/* n = size of Atom and Number tables, m = size of list-area. */

jmp_buf env;         /* struct to hold environment for longjump */
char *sout;          /* general output buffer pointer */

/* The atom table */
struct Atomtable {char name[16]; int32 L; int32 bl; int32 plist;} Atab[n];

/* The number table is used for storing floating point numbers.  The
   field nlink is used for linking number table nodes on the number
   table free space list. */

union Numbertable {double num; int16 nlink;} Ntab[n];

/* the number hash index table */
int16 nx[n];

/* the number table free space list head pointer */
int16 nf= -1;

/* the number table mark array nmark is used in garbage collection to
   mark words not to be returned to the free space list */
char nmark[n]; /* an array of 1-bit entries would suffice */

/* The list area */
struct Listarea {int32 car; int32 cdr;} *P;

/* the list area free space list head pointer */
int16 fp= -1;

/* the put-back variable */
int32 pb= 0;

/* The input string and related pointers */
char *g,*pg,*pge;

/* the input stream stack structure and head pointer */
struct Insave
   {struct Insave *link; char *pg, *pge; char g[202]; FILE *filep;};
struct Insave *topInsave;

/* the input prompt character */
char prompt;

/* seval depth count and trace switch */
int16 ct= 0, tracesw= 0;

/* Global ordinary atom typed-pointers */
int32 nilptr,tptr,currentin,eaL,quoteptr,sk,traceptr;

/* Number of free list-nodes */
int32 numf;

/* define global macros */

#define A(j)           P[j].car
#define B(j)           P[j].cdr

#define type(f)        (((f)>>28) & 0xf)
#define ptrv(f)        (0x0fffffff & (f))
#define sexp(t)        ((t) EQ 0 OR (t) EQ 8 OR (t) EQ 9)
#define fctform(t)     ((t)>9)
#define builtin(t)     ((t) EQ 10 OR (t) EQ 11)
#define userdefd(t)    ((t) EQ 12 OR (t) EQ 13)
#define dottedpair(t)  ((t) EQ 0)
#define fct(t)         ((t) EQ 10 OR (t) EQ 12 OR (t) EQ 14)
#define unnamedfsf(t)  ((t)>13)
#define namedfsf(t)    ((t)>9 AND (t)<14)
#define tp(t,j)        ((t) | (j))
#define ud(j)          (0x10000000 | (j))
#define se(j)          (0x00000000 | (j))
#define oa(j)          (0x80000000 | (j))
#define nu(j)          (0x90000000 | (j))
#define bf(j)          (0xa0000000 | (j))
#define bs(j)          (0xb0000000 | (j))
#define uf(j)          (0xc0000000 | (j))
#define us(j)          (0xd0000000 | (j))
#define tf(j)          (0xe0000000 | (j))
#define ts(j)          (0xf0000000 | (j))

/* variables used in file operations */
FILE *filep;
FILE *logfilep;

/* forward references */
forward int32 seval(int32 i);
forward void initlisp(void);
forward int32 sread(void);
forward void swrite(int32 i);
forward int32 newloc(int32 x, int32 y);
forward int32 numatom (double r);
forward int32 ordatom (char *s);
forward void gc(void);
forward void gcmark(int32 p);
forward char getgchar(void);
forward char lookgchar(void);
forward void fillg(void);
forward int32 e(void);
forward void error(char *s);
forward int16 fgetline(char *s, int16 lim, FILE *stream);
forward void ourprint(char *s);



/*==========================================================================*/
void spacerpt(int32 r)
/*---------------------------------------------------------------------------
  For debugging to see if we are leaking list-nodes.
  We are to protect r from garbage-collection.
  This function can be called from within the main loop.
----------------------------------------------------------------------------*/
{char s[60]; 
 int16 t;

 sprintf(s,"entering spacerpt: r=%x, numf=%d\n", r, numf); ourprint(s);

 t = type(r);
 if (namedfsf(t)) r = ptrv(Atab[ptrv(r)].L); /* dereference r */
 if (builtin(t)) r = nilptr; /*do not try to mark a builtin */
 gcmark(r);
 gc();

 sprintf(s,"leaving spacerpt: numf=%d\n", numf); ourprint(s);
}


/*==========================================================================*/
void main(void)
/*---------------------------------------------------------------------------
 Here is the main read/eval/print loop.
----------------------------------------------------------------------------*/
{int32 r;

 initlisp();

 setjmp(env);
 /* calling error() returns to here by longjmp() */

 for (;;) {ourprint("\n");
           prompt= '*';
           r=sread();
           r=seval(r);
           swrite(r);  /* swrite uses/frees no list-nodes. */
          }
}

/*==========================================================================*/
void error(char *msg)
/* char  *msg;   message to type out */
/*---------------------------------------------------------------------------
 Type-out the message msg and do longjmp() to top level
----------------------------------------------------------------------------*/
{int32 i,t;

 /* discard all input S-expression and argument list stacks */
 Atab[currentin].L= nilptr; Atab[eaL].L= nilptr; Atab[sk].L= nilptr;

 /* reset all atoms to their top-level values */
 for (i= 0; i<n; i++) if ((t= Atab[i].bl)!=nilptr)
    {while (B(t)!=nilptr) t= B(t); Atab[i].L= A(t); Atab[i].bl= nilptr;}

 ct= 0; ourprint("::"); ourprint(msg); ourprint("\n");
 longjmp(env,-1);
}

/*=========================================================================*/
void ourprint(char *s)
/* char *s; message to be typed out and logged */
/*--------------------------------------------------------------------------
 Print the string s to the terminal, and also in the logfile, lisp.log
 --------------------------------------------------------------------------*/
{printf("%s",s); fflush(stdout); fprintf(logfilep,"%s",s); fflush(logfilep);}

/*=========================================================================*/
void initlisp(void)
/*---------------------------------------------------------------------------
 This procedure installs all builtin functions and special forms into
 the atom table.  It also initializes the number table and list area.
 --------------------------------------------------------------------------*/
{int32 i;

 static char *BI[]=
    {"CAR","CDR","CONS","LAMBDA","SPECIAL","SETQ","ATOM","NUMBERP","QUOTE",
     "LIST","DO","COND","PLUS","TIMES","DIFFERENCE","QUOTIENT","POWER",
     "FLOOR","MINUS","LESSP","GREATERP","EVAL","EQ","AND","OR","SUM","PRODUCT",
     "PUTPLIST","GETPLIST","READ","PRINT","PRINTCR","MKATOM","BODY","RPLACA",
     "RPLACD","TSETQ", "NULL", "SET"
    };

 static char BItype[]=
    {10,10,10,11,11,11,10,10,11,10,
     10,11,10,10,10,10,10,10,10,10,
     10,10,10,11,11,10,10,10,10,10,
     10,10,10,10,10,10,11,10,11
    };

 /* number of builtin's in BI[] and BItype[] above */
 #define NBI 39

 /* allocate a global character array for messages */
 sout= (char *)calloc(80,sizeof(char));

 /* allocate the input string */
 g= (char *)calloc(202,sizeof(char));

 /* allocate the list area */
 P= (struct Listarea *)calloc(m,sizeof(struct Listarea));

/* initialize atom table names and the number table */
 for (i= 0; i<n; i++)
    {Atab[i].name[0]='\0'; nmark[i]=0; nx[i]= -1; Ntab[i].nlink=nf; nf=i;}

 /* install typed-case numbers for builtin functions and and special forms into
    the atom table */
 for (i= 0; i<NBI; i++)
    {Atab[ptrv(ordatom(BI[i]))].L= tp((((int32)BItype[i])<<28),(i+1));}

 nilptr= ordatom("NIL"); Atab[ptrv(nilptr)].L= nilptr;
 tptr= ordatom("T");     Atab[ptrv(tptr)].L= tptr;
 quoteptr= ordatom("QUOTE");

 /* Creating these lists in the atom-table ensures that we protect
    them during garbage-collection. Make CURRENTIN and EAL not upper-case
    to keep them private.*/
 currentin= ptrv(ordatom("CURRENTIN")); Atab[currentin].L= nilptr;
 eaL= ptrv(ordatom("EAL")); Atab[eaL].L= nilptr;
 sk= ptrv(ordatom("sreadlist")); Atab[sk].L= nilptr;

#define cilp Atab[currentin].L
#define eaLp Atab[eaL].L
#define skp Atab[sk].L

 /* initialize the bindlist (bl) and plist fields */
 for (i= 0; i<n; i++) Atab[i].bl= Atab[i].plist= nilptr;

 /* set up the list area free-space list */
 for (i= 1; i<m; i++) {B(i)= fp; fp= i;} numf = m-1;

 /* Prepare to read in predefined functions and special forms from the
    lispinit file: these are APPEND, REVERSE, EQUAL, APPLY, INTO,
    ONTO, NOT, NULL, ASSOC, NPROP, PUTPROP, GETPROP, and REMPROP */

 /* open the logfile */
 logfilep= fopen("lisp.log","w");
 ourprint(" ENTERING THE LISP INTERPRETER \n");

 /* establish the input buffer and the input stream stack */
 topInsave= NULL;
 strcpy(g,"@lispinit ");
 pg= g; pge= g+strlen(g);/* initialize start & end pointers to string g */
 filep= stdin;
}

/*==========================================================================*/
int32 sread(void)
/*----------------------------------------------------------------------------
  This procedure scans an input string g using a lexical token scanning
  routine, e(), where e() returns
                    1 if the token is '('
                    2 if the token is '''
                    3 if the token is '.'
                    4 if the token is ')'
  or a typed pointer d to an atom or number stored in row ptrv(d) in
  the atom or number tables. Due to the typecode (8 or 9) of d, d is a
  negative 32-bit integer.  The token found by e() is stripped from
  the front of g.
 
  SREAD constructs an S-expression and returns a typed pointer to it
  as its result.
  --------------------------------------------------------------------------*/
{int32 j,k,t,c;

 if ((c= e())<=0) return(c);

 if (c EQ 1) if ((k= e()) EQ 4) return(nilptr); else pb= k;
 /* to permit recursion, skp is a list of lists. */
 skp= newloc(nilptr,skp);
 A(skp)= j= k= newloc(nilptr,nilptr);

 /* we will return k, but we will fill node j first */
 if (c EQ 1)
    {scan: A(j)= sread();
     next: if ((c= e())<=2)
              {t= newloc(nilptr,nilptr); B(j)= t; j= t;
               if (c<=0) {A(j)= c; goto next;}
               pb= c; goto scan;
              }
     if (c!=4) {B(j)= sread(); if (e()!=4) error("syntax error");}
     skp= B(skp); return(k);
    }
 if (c EQ 2)
    {A(j)= quoteptr; B(j)= t= newloc(nilptr,nilptr); A(t)= sread();
     skp= B(skp); return(k);
    }
 error("bad syntax");
}

/*===========================================================================*/
int32 e(void)
/*----------------------------------------------------------------------------
 E is a lexical token scanning routine which has no input and returns
       1 if the token is '('
       2 if the token is '''
       3 if the token is '.'
       4 if the token is ')'
       or a negative typed-pointer to an entry in the atom table or the
       the number table.
-----------------------------------------------------------------------------*/
{double v,f,k,sign;
 int32 i,t,c;
 char nc[50], *np;
 struct Insave *tb;

#define OPENP '('
#define CLOSEP ')'
#define BLANK ' '
#define SINGLEQ '\''
#define DOT '.'
#define PLUS '+'
#define MINUS '-'
#define CHVAL(c) (c-'0')
#define DIGIT(c) ('0'<=(c) AND (c)<='9')
#define TOUPPER(c) ((c) + 'A'-'a')
#define ISLOWER(c) ((c)>='a' AND (c)<='z')

 if (pb!=0) {t= pb; pb= 0; return(t);}

start:
 while ((c= getgchar()) EQ BLANK);  /* remove blanks */

 if (c EQ OPENP)
    {while (lookgchar() EQ BLANK) getgchar(); /* remove blanks */
     if (lookgchar() EQ CLOSEP) {getgchar(); return(nilptr);} else return(1);
    }
 if (c EQ EOS)
    {if (topInsave EQ NULL) {fclose(logfilep); exit(0);}
     /* restore the previous input stream */
     fclose(filep);
     strcpy(g,topInsave->g); pg= topInsave->pg; pge= topInsave->pge;
     filep= topInsave->filep; topInsave= topInsave->link;
     if (prompt EQ '@') prompt= '>';
     goto start;
    }
 if (c EQ SINGLEQ) return(2);
 if (c EQ CLOSEP) return(4);
 if (c EQ DOT)
    {if (DIGIT(lookgchar())) {sign= 1.0; v= 0.0; goto fraction;} return(3);}
 if (NOT (DIGIT(c) OR ((c EQ PLUS OR c EQ MINUS) AND
     (DIGIT(lookgchar()) OR lookgchar() EQ DOT))))
    {np= nc; *np++= c;    /* put c in nc[0] */
     for (c= lookgchar();
	  c!=BLANK AND c!=DOT AND c!=OPENP AND c!=CLOSEP;
	  c= lookgchar())
	*(np++)= getgchar(); /* add a character */
     *np= EOS; /* nc is now a string */
     if (*nc EQ '@')
	{/* switch input streams */
	 /* save the current input stream */
	 tb= (struct Insave *)calloc(1,sizeof(struct Insave));
         tb->link= topInsave; topInsave= tb;
	 strcpy(tb->g,g); tb->pg= pg; tb->pge= pge; tb->filep= filep;

	 /* set up the new input stream */
         *g= EOS; pg= pge= g; prompt= '@';
	 filep= fopen(nc+1,"r"); /* skip over the @ */
         if (filep EQ NULL) error("Cannot open @file!");
         goto start;
	}
     /* convert the string nc to upper case */
     for (np= nc; *np!=EOS; np++)
	if (ISLOWER((int16)*np)) *np= (char)TOUPPER((int16)*np);
     return(ordatom(nc));
    }
 if (c EQ MINUS) {v= 0.0; sign= -1.0;} else {v= CHVAL(c); sign= 1.0;}
 while (DIGIT(lookgchar())) v= 10.0*v+CHVAL(getgchar());
 if (lookgchar() EQ DOT)
    {getgchar();
     if (DIGIT(lookgchar()))
        {fraction:
         k= 1.0; f= 0.0;
	 do {k=10.*k;f=10.*f+CHVAL(getgchar());} while (DIGIT(lookgchar()));
         v= v+f/k;
        }
    }
 return(numatom(sign*v));
}

/*===========================================================================*/
char getgchar(void)
/*----------------------------------------------------------------------------
 Fill the buffer string pg (=pointer to g) if needed, and then remove and 
 return the next character from the input.
-----------------------------------------------------------------------------*/
{fillg(); return(*pg++);}

/*===========================================================================*/
char lookgchar(void)
/*----------------------------------------------------------------------------
 Fill the buffer string pg (=g) if needed, and then return a copy of
 the next character in the input, but don't advance pg..
 * -----------------------------------------------------------------------------*/
{fillg(); return(*pg);}

/*===========================================================================*/
void fillg(void)
/*----------------------------------------------------------------------------
 Read a line into g[]. A line starting with a "/" is a comment line.
-----------------------------------------------------------------------------*/
{while (pg>=pge)
   {sprompt: if (filep EQ stdin) {sprintf(sout,"%c",prompt); ourprint(sout);}
    if (fgetline(g,200,filep)<0) return;
    if (filep EQ stdin) {fprintf(logfilep,"%s\n",g); fflush(logfilep);}
    if (*g EQ '/') goto sprompt;
    pg= g; pge= g+strlen(g); *pge++= ' '; *pge= '\0'; prompt= '>';
   }
}

/*===========================================================================*/
int16 fgetline(char *s, int16 lim, FILE *stream)
/*----------------------------------------------------------------------------
 fgetline() gets a line (CRLF or LF delimited) from stream and puts it into s (up
 to lim chars).  The function returns the length of this string.  If there
 are no characters but just EOF, it returns -1 (EOF) as the length.  There
 is no deblanking except to drop CR's and LF's ('\n') and map TABs to blanks.
-----------------------------------------------------------------------------*/
{int16 c,i;
#define TAB 9
 for (i=0; i<lim AND (c=fgetc(stream))!=EOF AND c!='\n'; ++i)
    {if (c EQ TAB) c= BLANK; s[i]= c;}
 s[i]= '\0';
 if (c EQ EOF AND i EQ 0) return(-1); else return(i);
}

/*===========================================================================*/
int32 numatom(double r)
/*----------------------------------------------------------------------------
 The number r is looked-up in the number table and stored there as a lazy
 number atom if it is not already present.  The typed-pointer to this number
 atom is returned.
 ----------------------------------------------------------------------------*/
{int32 j;

#define hashnum(r) ((*(1+(int32 *)(&r)) & 0x7fffffff) % n)

 j= hashnum(r);

 while (nx[j]!=-1)
    if (Ntab[nx[j]].num EQ r) {j= nx[j]; goto ret;} else if (++j EQ n) j= 0;

 if (nf<0) {gc(); if (nf<0) error("The number table is full");}
 nx[j]= nf; j= nf; nf= Ntab[nf].nlink; Ntab[j].num= r;
ret: return(nu(j));
}

/*===========================================================================*/
int32 ordatom (char *s)
/*----------------------------------------------------------------------------
 The ordinary atom whose name is given as the argument string s is looked-up
 in the atom table and stored there as an atom with the value undefined if it
 is not already present.  The typed-pointer to this ordinary atom is then
 returned.
 ----------------------------------------------------------------------------*/
{int32 j,c;

#define hashname(s) (abs((s[0]<<16)+(s[(j=strlen(s))-1]<<8)+j) % n)

 j= hashname(s); c= 0;

// DEBUG(printf("ordatom: `%s' hashes to %d. k=%d, n=%d\n",s,j,k,n););
 while (Atab[j].name[0]!=EOS)
    {if (strcmp(Atab[j].name,s) EQ 0) goto ret;
     else if (++j >= n) {j= 0; if (++c>1) error("atom table is full");}
    }

 strcpy(Atab[j].name,s); Atab[j].L= ud(j);
ret: return(oa(j));
}

/*===========================================================================*/
void swrite(int32 j)
/*----------------------------------------------------------------------------
 The S-expression pointed to by j is typed out.
 ----------------------------------------------------------------------------*/
{int32 i;
 int16 listsw;

 i= ptrv(j);
 switch (type(j))
    {case 0: /* check for a list */
        j= i;
        while (type(B(j)) EQ 0) j= B(j);
        listsw= (B(j) EQ nilptr);
        ourprint("(");
        while (listsw)
	   {swrite(A(i)); if ((i= B(i)) EQ nilptr) goto close; ourprint(" ");}
        swrite(A(i)); ourprint(" . "); swrite(B(i));
close:  ourprint(")");
        break;

     case  8: ourprint(Atab[i].name); break;
     case  9: sprintf(sout,"%-g",Ntab[i].num); ourprint(sout); break;
     case 10: sprintf(sout,"{builtin function: %s}",Atab[i].name);
	      ourprint(sout); break;
     case 11: sprintf(sout,"{builtin special form: %s}",Atab[i].name);
	      ourprint(sout); break;
     case 12: sprintf(sout,"{user defined function: %s}",Atab[i].name);
	      ourprint(sout); break;
     case 13: sprintf(sout,"{user defined special form: %s}",Atab[i].name);
	      ourprint(sout); break;
     case 14: ourprint("{unnamed function}"); break;
     case 15: ourprint("{unnamed special form}"); break;
    }
}

/*===========================================================================*/
void traceprint(int32 v, int16 osw)
/* int32 v; the object to be printed
 * int16 osw; 1 for seval() output, 0 for seval() input
 */
/*----------------------------------------------------------------------------
 This function prints out the input and the result for each successive
 invocation of seval() when tracing is requested.
----------------------------------------------------------------------------*/
{if (tracesw>0)
    {if (osw EQ 1) sprintf(sout,"%d result:",ct--);
     else sprintf(sout,"%d seval:",++ct);
     ourprint(sout); swrite(v); ourprint("\n");
    }
}

/*==========================================================================*/
int32 seval(int32 p)
/*---------------------------------------------------------------------------
 Evaluate the S-expression pointed to by the typed-pointer p; construct the
 result value as necessary; return a typed-pointer to the result.
 ---------------------------------------------------------------------------*/
{int32 ty,t,v,j,f,fa,na;
/* I think t can be static. also fa and j?  -test later. */

 int32 *endeaL;
 static double s;

#define U1 A(p)
#define U2 A(B(p))
#define E1 A(p)
#define E2 A(B(p))

#define Return(v) {traceprint(v,1); return(v);}

 traceprint(p,0);

 if(type(p)!=0)
   {/* p does not point to a non-atomic S-expression.
     *
     * If p is a type-8 typed pointer to an ordinary atom whose value is a
     * builtin or user-defined function or special form, then a typed-pointer
     * to that atom-table entry with typecode 10, 11, 12, or 13, depending upon
     * the value of the atom, is returned.  Note that this permits us to know
     * the names of functions and special forms.
     *
     * if p is a type-8 typed pointer to an ordinary atom whose value is not a
     * builtin or user defined function or special form, and thus has the type-
     * code 8, 9, 14, or 15, then a typed-pointer corresponding to the value of
     * this atom is returned.
     *
     * if p is a non-type-8 typed-pointer to a number atom or to a function or
     * special form (named or unnamed), then the same pointer p is returned.
     */

    if ((t= type(p))!=8) Return(p); j= ptrv(p);

   /* The association list is implemented with shallow binding in the atom-
      table, so the current values of all atoms are found in the atom table. */

    if (Atab[j].name[0] EQ '!')
       {tracesw= (strcmp(Atab[j].name,"!TRACE") EQ 0)?1:0; longjmp(env,-1);}

    if ((t= type(Atab[j].L)) EQ 1)
       {sprintf(sout,"%s is undefined\n",Atab[j].name); error(sout);}

    if (namedfsf(t)) Return(tp(t<<28,j));
    Return(Atab[j].L);
   } /* end of if (type(p)!=0) */

 /* Save the list consisting of the current function and the supplied
    arguments as the top value of the currentin list to protect it
    from garbage collection. The currentin list is a list of lists. */

 cilp= newloc(p,cilp);

 /* compute the function or special form to be applied */
 tracesw-- ; f= seval(A(p)); tracesw++; ty= type(f);
 if (NOT fctform(ty)) error(" invalid function or special form");
 f= ptrv(f); if (NOT unnamedfsf(ty)) f= ptrv(Atab[f].L);

 /* now let go of the supplied input function */
 A(cilp)= p= B(p);

 /* If f is a function (not a special form), build a new list of its
    evaluated arguments and add it to the eaL list (the eaL list is a
    list of lists.)  Then let go of the list of supplied arguments,
    replacing it with the new list of evaluated arguments */
 if (fct(ty))
    {/* compute the actual arguments */
     eaLp= newloc(nilptr,eaLp);
     /* evaluate the actual arguments and build a list by tail-cons-ing! */
     endeaL= &A(eaLp);
     while (p!=nilptr)
        {*endeaL= newloc(seval(A(p)),nilptr); endeaL= &B(*endeaL); p= B(p);}
     /* Set p to be the first node in the evaluated arguments list. */
     p= A(eaLp);

     /* Throw away the current supplied arguments list by popping the 
        currentin list */
     cilp= B(cilp);
    }

 /* At this point p points to the first node of the actual argument
    list.  if p EQ nilptr, we have a function or special form with no
    arguments */
 if (NOT builtin(ty))
    {/* f is a non-builtin function or non-builtin special form.  do
        shallow binding of the arguments and evaluate the body of f by
        calling seval */
     fa= A(f); /* fa points to the first node of the formal argument list */
     na= 0;    /* na counts the number of arguments */
     /* run through the arguments and place them as the top values of
	the formal argument atoms in the atom-table.  Push the old
	value of each formal argument on its binding list. */
     if (type(fa) EQ 8 AND fa != nilptr)
        {/* This will bind the entire input actual arglist as the
            single actual arg.  Sometimes, it is wrong - we should
            dereference the named fsf's in the p list, first. */

         t=ptrv(fa); 
         Atab[t].bl=newloc(Atab[t].L,Atab[t].bl); 
         Atab[t].L=p;
         goto apply;
        }
     else
	while (p!=nilptr AND dottedpair(type(fa)))
           {t= ptrv(A(fa)); fa= B(fa);
            Atab[t].bl= newloc(Atab[t].L,Atab[t].bl);
            v= A(p); if (namedfsf(type(v))) v= Atab[ptrv(v)].L;
            Atab[t].L= v; ++na; p= B(p);
           }

     if (p!=nilptr) error("too many actuals");
     /* The following code would forbid some useful trickery. 
        if (fa!=nilptr) error("too many formals"); */

     /* now apply the non-builtin special form or function */
 apply: v= seval(B(f));

     /* now unbind the actual arguments */
     fa= A(f);
     if (type(fa) EQ 8 AND fa != nilptr)
        {t= ptrv(fa); Atab[t].L= A(Atab[t].bl); Atab[t].bl= B(Atab[t].bl);}
     else
        while (na-- > 0)
           {t= ptrv(A(fa)); fa= B(fa);
            Atab[t].L= A(Atab[t].bl); Atab[t].bl= B(Atab[t].bl);
           }
    } /* end non-builtins */
 else
    {/* at this point we have a builtin function or special form.  f
        is the pointer value of the atom in the atom table for the
        called function or special form and p is the pointer to the
        argument list.*/

     v= nilptr;
     switch (f) /* begin builtins */
     {case 1: /* CAR */
         if (NOT dottedpair(type(E1))) error("illegal CAR argument");
         v= A(E1); break;
      case 2: /* CDR */
         if (NOT dottedpair(type(E1))) error("illegal CDR argument");
         v= B(E1); break;
      case 3: /* CONS */
         if (sexp(type(E1)) AND sexp(type(E2))) v= newloc(E1,E2);
         else error("Illegal CONS arguments");
         break;

         /* for LAMBDA and SPECIAL, we could check that U1 is either an
            ordinary atom or a list of ordinary atoms */
      case 4:/* LAMBDA */ v= tf(newloc(U1,U2)); break;
      case 5:/* SPECIAL */ v= ts(newloc(U1,U2)); break;
      case 6:/* SETQ */
         f= U1; if (type(f)!=8) error("illegal assignment");
assign:  v= ptrv(f); endeaL= &Atab[v].L;
doit:    t= seval(U2);
         switch (type(t))
            {case 0: /* dotted pair */
             case 8: /* ordinary atom */
             case 9: /* number atom */
		*endeaL= t; break;
             case 10: /* builtin function */
             case 11: /* builtin special form */
             case 12: /* user-defined function */
             case 13: /* user-defined special form */
		*endeaL= Atab[ptrv(t)].L; break;
             case 14: /* unnamed function */
		*endeaL= uf(ptrv(t)); break;
             case 15: /* unamed special form */
	        *endeaL= us(ptrv(t)); break;
            } /* end of type(t) switch cases */

         tracesw--; v= seval(f); tracesw++; break;

      case 7: /* ATOM */
         if ((type(E1)) EQ 8 OR (type(E1)) EQ 9) v= tptr; break;

      case 8: /* NUMBERP */
         if (type(E1) EQ 9) v= tptr; break;

      case 9: /* QUOTE */ v= U1; break;
      case 10: /* LIST */ v= p; break;
      case 11: /* DO */ while (p!=nilptr) {v= A(p); p= B(p);} break;

      case 12: /* COND */
         while (p!=nilptr)
            {f = A(p);
             if (seval(A(f))!=nilptr) {v=seval(A(B(f))); break;} else p=B(p);
            }
         break;

      case 13: /* PLUS */
         v= numatom(Ntab[ptrv(E1)].num+Ntab[ptrv(E2)].num); break;

      case 14: /* TIMES */
         v= numatom(Ntab[ptrv(E1)].num*Ntab[ptrv(E2)].num); break;

      case 15: /* DIFFERENCE */
         v= numatom(Ntab[ptrv(E1)].num-Ntab[ptrv(E2)].num); break;

      case 16: /* QUOTIENT */
         v= numatom(Ntab[ptrv(E1)].num/Ntab[ptrv(E2)].num); break;

      case 17: /* POWER */
         v= numatom(pow(Ntab[ptrv(E1)].num,Ntab[ptrv(E2)].num));
         break;

      case 18: /* FLOOR */ v= numatom(floor(Ntab[ptrv(E1)].num)); break;
      case 19: /* MINUS */ v= numatom(-Ntab[ptrv(E1)].num); break;
      case 20: /* LESSP */
         if(Ntab[ptrv(E1)].num<Ntab[ptrv(E2)].num) v= tptr; break;

      case 21: /* GREATERP */
         if (Ntab[ptrv(E1)].num>Ntab[ptrv(E2)].num); v= tptr; break;

      case 22: /* EVAL */ v= seval(E1); break;
      case 23: /* EQ */ v= (E1 EQ E2) ? tptr : nilptr; break;

      case 24: /* AND */
         while (p!=nilptr AND seval(A(p))!=nilptr) p= B(p);
         if (p EQ nilptr) v= tptr;  /* else v remains nilptr */
         break;

      case 25: /* OR */
         while (p!=nilptr AND seval(A(p)) EQ nilptr) p= B(p);
         if (p!=nilptr) v= tptr;  /* else v remains nilptr */
         break;

      case 26: /* SUM */
         for (s= 0.0; p!=nilptr; s= s+Ntab[ptrv(A(p))].num, p= B(p));
         v= numatom(s); break;

      case 27: /* PRODUCT */
         for (s= 1.0; p!=nilptr; s= s*Ntab[ptrv(A(p))].num, p= B(p));
         v= numatom(s); break;

      case 28: /* PUTPLIST */ v= E1; Atab[ptrv(v)].plist= E2; break;
      case 29: /* GETPLIST */ v= Atab[ptrv(E1)].plist; break;
      case 30: /* READ */ ourprint("\n!"); prompt= EOS; v= sread(); break;
      case 31: /* PRINT */
         if (p EQ nilptr) ourprint(" ");
         else while (p!=nilptr) {swrite(A(p)); ourprint(" "); p= B(p);}
         break;

      case 32: /* PRINTCR */
         if (p EQ nilptr) ourprint("\n");
         else while (p!=nilptr) {swrite(A(p)); ourprint("\n"); p= B(p);}
         break;

      case 33: /* MKATOM */
         strcpy(sout,Atab[ptrv(E1)].name); strcat(sout,Atab[ptrv(E2)].name);
         v= ordatom(sout); break;

      case 34: /* BODY */
         if (unnamedfsf(type(E1))) v= ptrv(E1);
         else if (userdefd(type(E1))) v= ptrv(Atab[ptrv(E1)].L);
	 else error("illegal BODY argument");
	 break;

      case 35: /* RPLACA */
	 v= E1;
	 if (NOT dottedpair(type(v))) error("illegal RPLACA argument");
	 A(v)= E2; break;

      case 36: /* RPLACD */
         v= E1;
         if (NOT dottedpair(type(v))) error("illegal RPLACD argument");
         B(v)= E2; break;

      case 37: /* TSETQ */
	/* Set the top-level value of U1 to seval(U2).*/
	 if (Atab[f= ptrv(U1)].bl EQ nilptr) goto assign;
	 v= Atab[f].bl; while (B(v)!=nilptr) v= B(v);
	 endeaL= &A(v); goto doit;

      case 38: /* NULL */
         if (E1 EQ nilptr) v= tptr; break;

      case 39:  /* SET */
	 f= seval(U1); goto assign;

      default: error("dryrot: bad builtin case number");
     } /* end of switch cases */

    } /* end builtins */

 /* pop the eaL list or pop the currentin list, whichever is active */
 if (fct(ty)) eaLp= B(eaLp); else cilp= B(cilp);

 Return(v);
}

/*========================================================================*/
int32 newloc(int32 x, int32 y)
/*--------------------------------------------------------------------------
 Allocates and loads the fields of a new location in the list area, with
 a()= X, b()= Y. The index of the new location is returned.
 -------------------------------------------------------------------------*/
{int32 j;

 if (fp<0) {gcmark(x); gcmark(y); gc(); if (fp<0) error("out of space");}
 j= fp; fp= B(j); A(j)= x; B(j)= y; numf--; return(j);
}

/*========================================================================*/
void gc(void)
/*--------------------------------------------------------------------------
 Garbage collector for number table and listarea
--------------------------------------------------------------------------*/
{int32 i,t;

#define marked(p)    ((A(p) & 0x08000000)!=0)
#define marknode(p)  (A(p) |= 0x08000000)
#define unmark(p)    (A(p) &= 0xf7ffffff)

 for (i= 0; i<n; i++)
    {gcmark(Atab[i].L); gcmark(Atab[i].bl); gcmark(Atab[i].plist);}

 for (i= 0; i<n; i++) nx[i]= -1;

 for (nf= -1,i= 0; i<n; i++)
    if (nmark[i] EQ 0) {Ntab[i].nlink= nf; nf= i;}
    else  /* restore num[i] */
       {t= hashnum(Ntab[i].num);
        while (nx[t]!=-1) if ((++t) EQ n) t= 0;
        nx[t]= i; nmark[i]= 0;
       }

 /* build the new list-node free-space list */
 fp= -1; numf= 0;
 for (i=1; i<m; i++) 
     if (NOT marked(i)) {B(i)= fp; fp= i; numf++;} else unmark(i);
}

/*========================================================================*/
void gcmark(int32 p)
/*--------------------------------------------------------------------------
 Mark the S-expression given by the typed-pointer p.
--------------------------------------------------------------------------*/
{static int32 s,t;

 static char c[120];

#define marknum(t,p)   if ((t) EQ 9) nmark[ptrv(p)]= 1
#define listp(t)       ((t) EQ 0 OR (t)>11)

start:
 t= type(p);
 if (listp(t))
    {p=ptrv(p); if (marked(p)) return; t=A(p); marknode(p);
     if (NOT listp(type(t))) {marknum(type(t),t); p=B(p); goto start;}
     s=B(p); 
     if (NOT listp(type(s))) {marknum(type(s),s); p=t; goto start;}
     gcmark(t); 
     p=B(p); goto start; /* Equivalent to the recursive call: gcmark(B(p)) */
    }
   else marknum(t,p);
}

