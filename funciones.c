/*
  Programa i2e
  Fichero funciones.c
  Autor Alfredo Casademunt
  Fecha 8-1-2000
*/

#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>
#include "i2e.h"

/*
static const char *FORMATO = "^.\\+ : .\\+$";
*/

char *strtrim( char *str )
{
  int i;

  for ( i = 0 ; str[i] && !isalpha( str[i] ) ; i++ );

  str += i ;
  for ( i = strlen( str ) - 1 ; i > 0 && !isalpha( str[i] ) ; i-- );
  str[i+1] = '\0';

  return str;
}

char *nomPersonal( char *nom )
{
  char *home;
  char *nombre = NULL;
  
  home = getenv( "HOME" );
  if (! home )
  {
    return nombre;
  }
  
  nombre = g_malloc( strlen( home ) + strlen( nom ) + 1 );
  
  strcpy( nombre, home );
  strcat( nombre, nom );
  
  return nombre;
}

size_t maxLinea( char *fichero )
{
  FILE *f;
  int c;             /* para leer por caracteres el diccionario */ 
  size_t cpl = 0;    /* numero de caracteres por linea, lf incluido */
  size_t mcpl = 0;   /* maximo de cpl + 1*/
  
  f = fopen( fichero, "r" );
  if (! f )
  {
    printf( _("\nError opening file %s\n"), fichero);
    return 0;
  }
  
  do
  {
    c = fgetc( f );
    cpl++;
    
    if (( c == '\n' )||( c == EOF ))
    {
      if ( cpl > mcpl )
      {
        mcpl = cpl;
      }
      cpl = 0;      
    }
  } while (c != EOF );  

  mcpl++;
  fclose( f );
  
  return mcpl;
}

/*
int diccioOK( parametros *datos )
{
  FILE *f;
  char *linea;       *//* para leer por lineas el diccionario */
/*  regex_t buffer;    *//* necesario para las expresiones regulares */
/*  int salida;        *//* valor de retorno de algunas funciones */
/*  int i;

  linea = (char *)g_malloc( datos->cpl );

  salida = regcomp( &buffer, FORMATO, REG_ICASE|REG_NOSUB|REG_NEWLINE );
  if ( salida )
  {
    printf( _("\nError in regcomp( %i )\n"), salida );
    g_free( linea );
    return 0;
  }

  for ( i=1 ; i>=0 ; i-- )
  {
    if (! datos->ficheros[i] )
      continue;
      
    f = fopen( datos->ficheros[i], "r" );
    if (! f )
    {
      printf( _("\nError opening file %s\n"), datos->ficheros[i] );
      g_free( linea );
      regfree( &buffer );
      return 0;
    }

    while ( fgets( linea, datos->cpl, f ))
    {
      salida = regexec( &buffer, linea, 0, 0, 0 );
      if ( salida )
      {
        printf( _("\nError in regexec( %i )"), salida );
        printf( _("\nAt line \"%s\"\nOf file %s"), linea,
                datos->ficheros[i] );
        fclose( f );
        g_free( linea );
        regfree( &buffer );
        return 0;
      }
    }
    
    fclose( f );
  }
  
  g_free( linea );
  regfree( &buffer );

  return 1;
}
*/

void graba_rc( parametros *datos )
{
  char *nombre;
  FILE *f;

  nombre = nomPersonal( PERSONAL_RC );

  f = fopen( nombre, "w" );
  if (! f )
  {
    g_free( nombre );
    return;
  }

  fprintf( f, "%i %i", datos->i2c, datos->exacto );
  fclose( f );
  g_free( nombre );

  return;
}

int carga_rc( parametros *datos )
{
  int i2c;
  int exacto;
  char *nombre;
  FILE *f;

  nombre = nomPersonal( PERSONAL_RC );

  f = fopen( nombre, "r" );
  if (! f )
  {
    g_free( nombre );
    return 0;
  }

  fscanf( f, "%i %i", &i2c, &exacto );
  fclose( f );
  g_free( nombre );

  if (((i2c != 0)&&(i2c != 1))||((exacto != 0)&&(exacto != 1)))
  {
    return 0;
  }

  datos->i2c = i2c;
  datos->exacto = exacto;

  return 1;
}
