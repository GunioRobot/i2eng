/*
  Programa i2e
  Fichero traduce.c
  Autor Alfredo Casademunt
  Fecha 8-1-2000
*/

#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2e.h"

/* palabra origen en español o en ingles */
static const char *INICIO[] = { "^.* : ", "^" };

/* busqueda exacta, español o ingles */
static const char *FINAL[] = { "[[:space:]].*$", "[[:space:].*:]" };

void traduce( GtkWidget *widget, gpointer data )
{
  parametros *datos;
  gchar *texto;       /* texto para traducir */
  GString *patron;    /* expresión regular */
  regex_t buffer;     /* necesario para las expresiones regulares */
  int salida;         /* valor de retorno de algunas funciones */
  char numero[18];    /* buffer para entero */
  char *linea;        /* para leer el ficheros por lineas */
  char *tr;	      /* para la salida de la traduccion */
  char *ptr;	      /* puntero al caracter dos puntos */
  int encontrado = 0; /* muestra mensaje si no hay traduccion */
  FILE *f;
  int i;
  char *LENGUAJE[] = { _("English"), _("Spanish") };

  datos = (parametros *)data;

  texto = strtrim( gtk_entry_get_text( GTK_ENTRY( datos->entrada )));
  gtk_entry_set_text( GTK_ENTRY( datos->entrada ), texto );

  gtk_text_backward_delete( GTK_TEXT( datos->salida ),
      gtk_text_get_length( GTK_TEXT( datos->salida )));

  if (! strlen( texto ))
  {
    gtk_widget_grab_focus( datos->entrada );
    return;
  }

  patron = g_string_new( INICIO[datos->i2c] );
  
  if ( datos->i2c )
  {
    g_string_append( patron, texto );
  }
  else
  {
    i = 0;
    while ( *(texto+i) )
    {
      switch ( *(texto+i) )
      {
        case 'a':
        case 'á':
          g_string_append( patron, "[aá]" );
          break;

        case 'e':
        case 'é':
          g_string_append( patron, "[eé]" );
          break;

        case 'i':
        case 'í':
          g_string_append( patron, "[ií]" );
          break;

        case 'o':
        case 'ó':
          g_string_append( patron, "[oó]" );
          break;

        case 'u':
        case 'ú':
        case 'ü':
          g_string_append( patron, "[uúü]" );
          break;

        default:
          g_string_append_c( patron, *(texto+i) );
          break;
      }
      
      i++;
    }
  }

  if ( datos->exacto )
  {
    g_string_append( patron, FINAL[datos->i2c] );
  }

  salida = regcomp( &buffer, patron->str, REG_ICASE|REG_NOSUB|REG_NEWLINE );
  g_string_free( patron, TRUE );
  
  if ( salida )
  {
    sprintf( numero, "%i", salida );
    mensaje( _("Error"), 0, 0, 2, _("In function regcomp()=="), numero );
    return;
  }

  linea = (char *)g_malloc( datos->cpl );
  tr = (char *)g_malloc( datos->cpl );
  
  for ( i=1 ; i>=0 ; i-- )
  {
    if (! datos->ficheros[i] )
      continue;

    f = fopen( datos->ficheros[i], "r" );
    if (! f )
    {
      mensaje( _("Error"), 0, 0, 2, _("I cannot open file "), datos->ficheros[i] );
      regfree( &buffer );
      g_free( linea );
      g_free( tr );
      return;
    }

    while ( fgets( linea, datos->cpl, f ))
    {
      salida = regexec( &buffer, linea, 0, 0, 0 );    
      if (! salida )
      {
        if ( datos->i2c )
        {
          strcpy( tr, linea );
        }
        else
        {
          ptr = strchr( linea, ':' );
          strcpy( tr, ptr+2 );
          tr[strlen( tr )-1] = '\0';
          strcat( tr, " : " );
          ptr--;
          *ptr = '\0';
          strcat( tr, linea );
          *ptr = ' ';
          strcat( tr, "\n" );
        }
        
        gtk_text_insert( GTK_TEXT( datos->salida ), NULL, NULL, NULL,
                         tr, strlen( tr ));
        if (! encontrado )
          encontrado = 1;
      }

      if ( salida > REG_NOMATCH )
      {
        sprintf( numero, "%i", salida );
        mensaje( _("Error"), 0, 0, 6,
            _("In function regexec()=="),
            numero,
            _("\nAt line: "),
            linea,
            _("Of file "),
            datos->ficheros[i] );
        fclose( f );
        regfree( &buffer );
        g_free( linea );
        g_free( tr );
        
        return;
      }
    }
    
    fclose( f );
  }
  
  if ( encontrado )
  {
    gtk_text_backward_delete( GTK_TEXT( datos->salida ), 1 );
  }
  else
  {
    patron = g_string_new( _("Not found in ") );
    g_string_append( patron, LENGUAJE[! datos->i2c] );
    g_string_append( patron, "\n\"" );
    g_string_append( patron, texto );
    g_string_append( patron, "\"" );

    gtk_text_insert( GTK_TEXT( datos->salida ), NULL, NULL, NULL,
        patron->str, patron->len );

    g_string_free( patron, TRUE );
  }

  gtk_adjustment_set_value( GTK_TEXT( datos->salida )->vadj, 0.0 );

  regfree( &buffer );
  g_free( linea );
  g_free( tr );

  gtk_entry_select_region( GTK_ENTRY( datos->entrada ), 0, strlen( texto ));
  gtk_widget_grab_focus( datos->entrada );

  return;
}
