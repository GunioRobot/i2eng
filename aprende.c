/*
  Programa i2e
  Fichero aprende.c
  Autor Alfredo Casademunt
  Fecha 8-1-2000
*/

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <gtk/gtk.h>
#include "i2e.h"

static GtkWidget *dialogo = NULL;
static GtkWidget *idioma[2];

static void salir( GtkWidget *widget, gpointer data )
{
  parametros *datos;
  
  datos = (parametros *)data;

  gtk_widget_destroy( GTK_WIDGET( dialogo ));
  dialogo = NULL;

  gtk_widget_grab_default( datos->boton );

  gtk_entry_select_region( GTK_ENTRY( datos->entrada ), 0,
      strlen( gtk_entry_get_text( GTK_ENTRY( datos->entrada ))));
  gtk_widget_grab_focus( datos->entrada );

  return;
}

static gint salir_ev( GtkWidget *widget, GdkEventAny *ev, gpointer data )
{
  salir( widget, data );
  return TRUE;
}

static void addtoDict( GtkWidget *widget, gpointer data )
{
  parametros *datos;
  char *ingles;
  char *castellano;
  GString *patron;    /* expresión regular */
  regex_t buffer;     /* necesario para las expresiones regulares */
  int salida;         /* valor de retorno de algunas funciones */
  char numero[18];    /* buffer para entero */
  char *linea;
  int i;
  FILE *f;
  
  datos = (parametros *)data;

  ingles = strtrim( gtk_entry_get_text( GTK_ENTRY( idioma[1] )));
  castellano = strtrim( gtk_entry_get_text( GTK_ENTRY( idioma[0] )));
  
  if ((! strlen( ingles ))||(! strlen( castellano )))
  {
    mensaje( _("Error"), 0, 0, 1, _("Add text to both inputs.") );
    return;
  }
  
  patron = g_string_new( "^" );
  g_string_append( patron, ingles );
  g_string_append( patron, " : " );

  i = 0;
  while ( *(castellano+i) )
  {
    switch ( *(castellano+i) )
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
        g_string_append_c( patron, *(castellano+i) );
        break;
    }

    i++;
  }

  g_string_append( patron, "$" );
  salida = regcomp( &buffer, patron->str, REG_ICASE|REG_NOSUB|REG_NEWLINE );
  g_string_free( patron, TRUE );

  if ( salida )
  {
    sprintf( numero, "%i", salida );
    mensaje( _("Error"), 0, 0, 2, _("In function regcomp()=="), numero );
    return;
  }

  linea = g_malloc( datos->cpl );

  patron = g_string_new( ingles );
  g_string_append( patron, " : " );
  g_string_append( patron, castellano );
  g_string_append( patron, "\n" );

  for ( i=1 ; i>=0 ; i-- )
  {
    if (! datos->ficheros[i] )
      continue;

    f = fopen( datos->ficheros[i], "r" );
    if (! f)
    {
      mensaje( _("Error"), 0, 0, 2, _("I cannot open file "),
          datos->ficheros[i] );
      regfree( &buffer );
      g_string_free( patron, TRUE );
      g_free( linea );
      salir( widget, datos );
      return;
    }
    
    while ( fgets( linea, datos->cpl, f ))
    {
      salida = regexec( &buffer, linea, 0, 0, 0 );
      if (! salida )
      {
        mensaje( _("Warning"), 0, 0, 2, patron->str, _("Already in dictionary") );
        fclose( f );
        regfree( &buffer );
        g_string_free( patron, TRUE );
        g_free( linea );
        salir( widget, datos );
        return;
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
        g_string_free( patron, TRUE );
        g_free( linea );
        salir( widget, datos );
        return;
      }
    }

    fclose( f );
  }

  regfree( &buffer );
  g_free( linea );
  
  if (! datos->ficheros[1] )
  {
    datos->ficheros[1] = nomPersonal( PERSONAL_DICT );
  }
  
  f = fopen( datos->ficheros[1], "a" );
  if (! f)
  {
    mensaje( _("Error"), 0, 0, 2, _("I cannot open file "),
        datos->ficheros[1] );
    salir( widget, datos );
    return;
  }

  fputs( patron->str, f );
  fclose( f );

  if ( patron->len + 1 > datos->cpl )
  {
    datos->cpl = patron->len + 1;
  }  
  
  g_string_free( patron, TRUE );
  salir( widget, datos );
  
  return;
}

void aprende( GtkWidget *widget, gpointer data )
{
  GtkWidget *tabla;
  GtkWidget *marco;
  GtkWidget *boton;
  parametros *datos;
  
  if ( dialogo )
  {
    return;
  }
    
  datos = (parametros *)data;

  /* crea una ventana de dialogo */
  dialogo = (GtkWidget *)gtk_dialog_new();
  gtk_window_set_title( GTK_WINDOW( dialogo ), _("Learning"));
  gtk_window_position( GTK_WINDOW( dialogo ), GTK_WIN_POS_CENTER );
  gtk_container_border_width( GTK_CONTAINER( dialogo ), 10 );

  gtk_signal_connect( GTK_OBJECT( dialogo ), "delete_event",
      GTK_SIGNAL_FUNC( salir_ev ), (gpointer)datos );

  tabla = gtk_table_new( 2, 1, TRUE );
  gtk_table_set_row_spacings( GTK_TABLE( tabla ), 10 );
  gtk_table_set_col_spacings( GTK_TABLE( tabla ), 10 );
  gtk_box_pack_start( GTK_BOX( GTK_DIALOG( dialogo )->vbox),
      tabla, TRUE, TRUE, 10 );
  gtk_widget_show( tabla );

  marco = gtk_frame_new( _("English") );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), marco, 0, 1, 0, 1 );
  gtk_widget_show( marco );
  
  idioma[1] = gtk_entry_new();
  gtk_container_add( GTK_CONTAINER( marco ), idioma[1] );
  gtk_widget_show( idioma[1] );

  marco = gtk_frame_new( _("Spanish") );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), marco, 0, 1, 1, 2 );
  gtk_widget_show( marco );
  
  idioma[0] = gtk_entry_new();
  gtk_container_add( GTK_CONTAINER( marco ), idioma[0] );
  gtk_widget_show( idioma[0] );

  gtk_entry_set_text( GTK_ENTRY( idioma[datos->i2c] ),
      gtk_entry_get_text( GTK_ENTRY( datos->entrada )));
/*  gtk_widget_grab_focus( idioma[ datos->i2c ] );*/
  gtk_widget_grab_focus( marco );

  boton = gtk_button_new_with_label( _("Learn") );
  gtk_signal_connect( GTK_OBJECT( boton ), "clicked",
      GTK_SIGNAL_FUNC( addtoDict ), (gpointer)datos );
  gtk_box_pack_start( GTK_BOX( GTK_DIALOG( dialogo )->action_area),
      boton, TRUE, TRUE, 10 );
  gtk_widget_show( boton );

  boton = gtk_button_new_with_label( _("Cancel") );
  gtk_signal_connect( GTK_OBJECT( boton ), "clicked",
      GTK_SIGNAL_FUNC( salir ), (gpointer)datos );
  gtk_box_pack_start( GTK_BOX( GTK_DIALOG( dialogo )->action_area),
      boton, TRUE, TRUE, 2 );
  gtk_widget_show( boton );

  gtk_widget_show( dialogo );

  return;  
}
