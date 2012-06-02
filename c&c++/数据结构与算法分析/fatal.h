/* @(#)fatal.h
 */
#include <stdio.h>
#include <stdlib.h>

#ifndef _FATAL_H
#define _FATAL_H

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

#endif /* _FATAL_H */

