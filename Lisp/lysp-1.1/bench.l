;;; -*- scheme -*-

(define lambda	(fsubr (dlsym "lambdaFsubr")))
(define if	(fsubr (dlsym "ifFsubr")))
(define println	(subr  (dlsym "printlnSubr")))
(define +	(subr  (dlsym "addSubr")))
(define -	(subr  (dlsym "subtractSubr")))
(define <	(subr  (dlsym "lessSubr")))

(define nfibs
  (lambda (n)
    (if (< n 2)
	1
      (+ 1 (nfibs (- n 1)) (nfibs (- n 2))))))

(println "running...")
(println (nfibs 28))
