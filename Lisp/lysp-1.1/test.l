;;; -*- scheme -*-

;;; initial environment contains: t dlsym fsubr subr define

(define nil ())

(define flambda	(fsubr (dlsym "flambdaFsubr")))
(define lambda	(fsubr (dlsym "lambdaFsubr")))
(define let	(fsubr (dlsym "letFsubr")))
(define if	(fsubr (dlsym "ifFsubr")))
(define while	(fsubr (dlsym "whileFsubr")))
(define setq 	(fsubr (dlsym "setqFsubr")))

(define cons	(subr (dlsym "consSubr")))
(define rplaca	(subr (dlsym "rplacaSubr")))
(define rplacd	(subr (dlsym "rplacdSubr")))
(define car	(subr (dlsym "carSubr")))
(define cdr	(subr (dlsym "cdrSubr")))
(define eval	(subr (dlsym "evalSubr")))
(define apply	(subr (dlsym "applySubr")))
(define map	(subr (dlsym "mapSubr")))
(define assq	(subr (dlsym "assqSubr")))
(define println	(subr (dlsym "printlnSubr")))
(define +	(subr (dlsym "addSubr")))
(define -	(subr (dlsym "subtractSubr")))
(define *	(subr (dlsym "multiplySubr")))
(define /	(subr (dlsym "divideSubr")))
(define %	(subr (dlsym "modulusSubr")))
(define <	(subr (dlsym "lessSubr")))
(define <=	(subr (dlsym "lessEqualSubr")))
(define ==	(subr (dlsym "equalSubr")))
(define !=	(subr (dlsym "notEqualSubr")))
(define >=	(subr (dlsym "greaterEqualSubr")))
(define >	(subr (dlsym "greaterSubr")))
(define println	(subr (dlsym "printlnSubr")))

(define caar	(lambda (x) (car (car x))))
(define cadr	(lambda (x) (car (cdr x))))
(define cdar	(lambda (x) (cdr (car x))))
(define cddr	(lambda (x) (cdr (cdr x))))
(define cadar	(lambda (x) (car (cdr (car x)))))
(define caddar	(lambda (x) (car (cdr (cdr (car x))))))

(define quote	(flambda (form) (car form)))
(define list	(lambda args args))

(define global-environment
  (let ((globals ((flambda (args env) env))))
    (lambda () globals)))

(define double (lambda (x) (+ x x)))
(double 21)

(define make-counter (lambda (n) (lambda () (setq n (+ n 1)))))
(define counter (make-counter 40))
(counter)
(counter)
(counter)
(counter)
(counter)

'(+ 3 4)

(eval '(+ 3 4))
(apply + 3 4)

(let ((x 0))
  (while (< x 10)
    (println (setq x (+ 1 x)))))

(setq *syntax-table*
      (cons (cons 'for
		  (flambda (form)
		    (list 'let (list (list (caar form) (cadar form)))
			  (list 'while (list '<= (caar form) (caddar form))
				(cadr form)
				(list 'setq (caar form) (list '+ 1 (caar form)))))))
	    *syntax-table*))

(for (i 0 10) (println i))

(define nfibs
  (lambda (n)
    (if (< n 2)
	1
	(+ 1 (nfibs (- n 1)) (nfibs (- n 2))))))

(nfibs 20)

(eval '(+ a b) (cons '(a . 3) (cons '(b . 4) (global-environment))))

(define dict '((foo . 1)(bar . 2)(baz . 3)))

(assq 'bar dict)

(define assqval (lambda (key alist) (cdr (assq key alist))))

(assqval 'foo dict)
(assqval 'bar dict)
(assqval 'baz dict)
(assqval 'qux dict)

(define puts (dlsym "puts"))

(puts "Hello, world")

(define malloc		(dlsym "malloc"))
(define free		(dlsym "free"))
(define sprintf		(dlsym "sprintf"))

(let ((memory (malloc 1024)))
  (sprintf memory "Hello %s world %d" "primitive" 42)
  (puts memory) 
  (free memory))

(define set-int		(subr (dlsym "intPokeSubr")))
(define get-char	(subr (dlsym "charPeekSubr")))

(let ((memory (malloc 1024)))
  (set-int memory 0 0x01020304)
  (set-int memory 1 0x05060708)
  (sprintf memory "%02x %02x %02x %02x %02x %02x %02x %02x"
	   (get-char memory 0) (get-char memory 1) (get-char memory 2) (get-char memory 3)
	   (get-char memory 4) (get-char memory 5) (get-char memory 6) (get-char memory 7))
  (puts memory)
  (free memory))

(define set-char	(subr (dlsym "charPokeSubr")))
(define ptr->string	(subr (dlsym "primToStringSubr")))

(define make-string
  (lambda ()
    (let ((memory (malloc 8)))
      (set-char memory 0 65)
      (set-char memory 1 66)
      (set-char memory 2 67)
      (set-char memory 3 0)
      (puts memory)
      (let ((s (ptr->string memory)))
	(free memory)
	s))))

(define exit (dlsym "exit"))

(exit 0)
