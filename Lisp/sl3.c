/* A minimal Lisp interpreter
   Copyright 2004 Andru Luvisi

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License , or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, write to the Free Software
   Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define error(X) do { fprintf(stderr, "%s\n", X); exit(1); } while (0)

/*** List Structured Memory ***/
enum otype { INT, SYM, CONS, PROC, PRIMOP };
typedef struct obj {
  enum otype type;
  struct obj *p[1];
} obj;
typedef obj * (*primop)(obj *);
obj *all_symbols, *top_env, *nil, *tee, *quote, 
    *s_if, *s_lambda, *s_define, *s_setb;

#define cons(X, Y)            omake(CONS, 2, (X), (Y))
#define car(X)                ((X)->p[0])
#define cdr(X)                ((X)->p[1])
#define setcar(X,Y)           (((X)->p[0]) = (Y))
#define setcdr(X,Y)           (((X)->p[1]) = (Y))
#define mkint(X)              omake(INT, 1, (obj *)(X))
#define intval(X)             ((int)((X)->p[0]))
#define mksym(X)              omake(SYM, 1, (obj *)(X))
#define symname(X)            ((char *)((X)->p[0]))
#define mkprimop(X)           omake(PRIMOP, 1, (obj *)(X))
#define primopval(X)          ((primop)(X)->p[0])
#define mkproc(X,Y,Z)         omake(PROC, 3, (X), (Y), (Z))
#define procargs(X)           ((X)->p[0])
#define proccode(X)           ((X)->p[1])
#define procenv(X)            ((X)->p[2])
#define isnil(X)              ((X) == nil)

obj *omake(enum otype type, int count, ...) {
  obj *ret;
  va_list ap;
  int i;
  va_start(ap, count);
  ret = (obj *) malloc(sizeof(obj) + (count - 1)*sizeof(obj *));
  ret->type = type;
  for(i = 0; i < count; i++) ret->p[i] = va_arg(ap, obj *);
  va_end(ap);
  return ret;
}

obj *findsym(char *name) {
  obj *symlist;
  for(symlist = all_symbols; !isnil(symlist); symlist = cdr(symlist))
    if(!strcmp(name, symname(car(symlist))))
      return symlist;
  return nil;
}

obj *intern(char *name) {
  obj *op = findsym(name);
  if(!isnil(op)) return car(op);
  op = mksym(name);
  all_symbols = cons(op, all_symbols);
  return op;
}

/*** Environment ***/
#define extend(ENV, SYM, VAL) (cons(cons((SYM), (VAL)), (ENV)))

obj *multiple_extend(obj *env, obj *syms, obj *vals) {
  return isnil(syms) ? 
         env : 
         multiple_extend(extend(env, car(syms), car(vals)),
                         cdr(syms), cdr(vals));
}

obj *extend_top(obj *sym, obj *val) {
  setcdr(top_env, cons(cons(sym, val), cdr(top_env)));
  return val;
}

obj *assoc(obj *key, obj *alist) {
  if(isnil(alist)) return nil;
  if(car(car(alist)) == key) return car(alist);
  return assoc(key, cdr(alist));
}

/*** Input/Output ***/
FILE *ifp;
char *token_la;
int la_valid = 0;
#define MAXLEN 100
char buf[MAXLEN];
int bufused;

void add_to_buf(char ch) { if(bufused < MAXLEN - 1) buf[bufused++] = ch; }
char *buf2str()          { buf[bufused++] = '\0'; return strdup(buf); }
void setinput(FILE *fp)  { ifp = fp; }
void putback_token(char *token) { token_la = token; la_valid = 1; }

char *gettoken() {
  int ch;

  bufused = 0;
  if(la_valid) { la_valid = 0; return token_la; }
  do {
    if((ch = getc(ifp)) == EOF) exit(0);
  } while(isspace(ch));
  add_to_buf(ch);
  if(strchr("()\'", ch)) return buf2str();
  for(;;) {
    if((ch = getc(ifp)) == EOF) exit(0);
    if(strchr("()\'", ch) || isspace(ch)) {
      ungetc(ch, ifp);
      return buf2str();
    }
    add_to_buf(ch);
  }
}

obj *readlist();
obj *readobj() {
  char *token;

  token = gettoken();
  if(!strcmp(token, "(")) return readlist();
  if(!strcmp(token, "\'")) return cons(quote, cons(readobj(), nil));
  if(token[strspn(token, "0123456789")] == '\0') return mkint(atoi(token));
  return intern(token);
}

obj *readlist() {
  char *token = gettoken();
  obj *tmp;
  if(!strcmp(token, ")")) return nil;
  if(!strcmp(token, ".")) {
    tmp = readobj();
    if(strcmp(gettoken(), ")")) exit(1);
    return tmp;
  }
  putback_token(token);
  tmp = readobj(); /* Must force evaluation order */
  return cons(tmp, readlist());
}

void writeobj(FILE *ofp, obj *op) {
  switch(op->type) {
    case INT:  fprintf(ofp, "%d", intval(op)); break;
    case CONS: 
		fprintf(ofp, "(");
		for(;;) {
			writeobj(ofp, car(op));
			if(isnil(cdr(op))) {
				fprintf(ofp, ")");
				break;
			}
			op = cdr(op);
			if(op->type != CONS) {
				fprintf(ofp, " . ");
				writeobj(ofp, op);
				fprintf(ofp, ")");
				break;
			}
			fprintf(ofp, " ");
		}
		break;
    case SYM:
		if(isnil(op)) fprintf(ofp, "()");
		else          fprintf(ofp, "%s", symname(op));
		break;
    case PRIMOP: fprintf(ofp, "#<PRIMOP>"); break;
    case PROC:   fprintf(ofp, "#<PROC>"); break;
    default: exit(1);
  }
}

/*** Evaluator (Eval/Apply) ***/
obj *evlis(obj *exps, obj *env);
obj *progn(obj *exps, obj *env);
obj *apply(obj *proc, obj *vals, obj *env);

obj *eval(obj *exp, obj *env) {
  obj *tmp;
  
  if(exp == nil) return nil;

  switch(exp->type) {
    case INT: return exp;
    case SYM:   tmp = assoc(exp, env);
                if(tmp == nil) error("Unbound symbol");
                return cdr(tmp);
    case CONS:  if(car(exp) == s_if) {
		  if(eval(car(cdr(exp)), env) != nil)
		    return eval(car(cdr(cdr(exp))), env);
		  else
		    return eval(car(cdr(cdr(cdr(exp)))), env);
		}
		if(car(exp) == s_lambda)
		  return mkproc(car(cdr(exp)), cdr(cdr(exp)), env);
		if(car(exp) == quote)
		  return car(cdr(exp));
		if(car(exp) == s_define)
		  return(extend_top(car(cdr(exp)),
		                    eval(car(cdr(cdr(exp))), env)));
		if(car(exp) == s_setb) {
		  obj *pair   = assoc(car(cdr(exp)), env);
		  obj *newval = eval(car(cdr(cdr(exp))), env);
		  setcdr(pair, newval);
		  return newval;
		}
		return apply(eval(car(exp), env), evlis(cdr(exp), env), env);
    case PRIMOP: return exp;
    case PROC:   return exp;
  }
  /* Not reached */
  return exp; 
}

obj *evlis(obj *exps, obj *env) {
  if(exps == nil) return nil;
  return cons(eval(car(exps), env), 
              evlis(cdr(exps), env));
}

obj *progn(obj *exps, obj *env) {
  if(exps == nil) return nil;
  for(;;) {
    if(cdr(exps) == nil)
      return eval(car(exps), env);
    eval(car(exps), env);
    exps = cdr(exps);
  }
}

obj *apply(obj *proc, obj *vals, obj *env) {
  if(proc->type == PRIMOP)
    return (*primopval(proc))(vals);
  if(proc->type == PROC) {
    /* For dynamic scope, use env instead of procenv(proc) */
    return progn(proccode(proc), 
                 multiple_extend(procenv(proc), procargs(proc), vals));
  }
  error("Bad argument to apply");
  /* Not reached */
  return nil; 
}

/*** Primitives ***/
obj *prim_sum(obj *args) {
  int sum;
  for(sum = 0; !isnil(args); sum += intval(car(args)), args = cdr(args));
  return mkint(sum);
}

obj *prim_sub(obj *args) {
  int sum;
  for(sum = intval(car(args)), args = cdr(args); 
      !isnil(args); 
      sum -= intval(car(args)), args = cdr(args));
  return mkint(sum);
}

obj *prim_prod(obj *args) {
  int prod;
  for(prod = 1; !isnil(args); prod *= intval(car(args)), args = cdr(args));
  return mkint(prod);
}

obj *prim_numeq(obj *args) {
  return intval(car(args)) == intval(car(cdr(args))) ? tee : nil;
}

obj *prim_cons(obj *args) { return cons(car(args), car(cdr(args))); }
obj *prim_car(obj *args)  { return car(car(args)); }
obj *prim_cdr(obj *args)  { return cdr(car(args)); }

/*** Initialization ***/
void init_sl3() {
  nil = mksym("nil");
  all_symbols = cons(nil, nil);
  top_env  = cons(cons(nil, nil), nil);
  tee      = intern("t");
  extend_top(tee, tee);
  quote    = intern("quote");
  s_if     = intern("if");
  s_lambda = intern("lambda");
  s_define = intern("define");
  s_setb   = intern("set!");
  extend_top(intern("+"), mkprimop(prim_sum));
  extend_top(intern("-"), mkprimop(prim_sub));
  extend_top(intern("*"), mkprimop(prim_prod));
  extend_top(intern("="), mkprimop(prim_numeq));
  extend_top(intern("cons"), mkprimop(prim_cons));
  extend_top(intern("car"),  mkprimop(prim_car));
  extend_top(intern("cdr"),  mkprimop(prim_cdr));
}

/*** Main Driver ***/
int main() {
  init_sl3();
  setinput(stdin);
  for(;;) {
    writeobj(stdout, eval(readobj(), top_env));
    printf("\n");
  }
  return 0;
}
