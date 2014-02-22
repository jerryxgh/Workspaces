                 LYSP: 50 Years of Symbolic Processing

Last weekend I was thinking about Ashwin Ram's "Short Ballad Dedicated to
the Growth of Programs" [1] and the result is LYSP: a tiny, lightweight Lisp
interpreter closely following the tradition of the earliest implementations.
It is dedicated to the inventor and first implementers of that language,
which is 50 years old this year.  If you partially evaluate LYSP in Latin it
means "50 Years of Symbolic Processing".

IBM once made a dialect of Lisp called Lysp and published a paper about it.
As far as I know, it died shortly thereafter and so I have unilaterally
swept and reallocated the name.  (If you know differently, please tell me.)

Being a weekend hack there are probably numerous bugs.  If you find one,
send a description (or perferably a prescription) to the author (Ian
Piumarta) at firstName (at) lastName (dot) com.  If you use it for something
interesting, I'd love to hear about that too.

It started off really small (about 150 LOC) but I got carried away.  I hope
you will forgive the bloat.  Compiled on x86 with gcc -Os it's about 17 KB
for the interpreter and 4 KB for the garbage collector (if you configure it
to use the cheesy, built-in, home-made, precise one -- rather than the
classy, incremental, conservative Boehm-Demers-Weiser one in /usr/lib).
That makes for about 21 KB of self-contained Lisp interpreter, including a
rudimentary (but useful) FFI and macros.  If you do something interesting
with it in the embedded space, I'd love to hear about that too.

And yes: '(cdr (assq key a-list))' does exactly what you want it to in LYSP.

[1] http://www.apl.jhu.edu/~hall/lisp/Scheme-Ballad.text
