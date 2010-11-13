/*
    Programa i2e
    Fichero mensaje.c
    Autor Alfredo Casademunt
    Fecha 8-1-2000
*/

#include <string.h>
#include <stdarg.h>
#include <gtk/gtk.h>
#include "i2e.h"

static void salir( GtkWidget *widgwt, gpointer data )
{
  gtk_widget_destroy( data );
  return;
}

void mensaje( char *titulo, gint ancho, gint alto, gint mensajes, ... )
{
  char *texto;
  gint longitud = 0;
  gint i;
  va_list param;
  GtkWidget *dialogo;
  GtkWidget *marco;
  GtkWidget *grupo;
  GtkWidget *boton;
  GtkWidget *etiqueta;
  
  va_start( param, mensajes );
  for ( i=0 ; i<mensajes ; i++ )
    longitud += strlen( va_arg( param, char* ));
  va_end( param );
  longitud++;

  dialogo = gtk_dialog_new();
  gtk_window_set_title( GTK_WINDOW( dialogo ), titulo);
  gtk_window_position( GTK_WINDOW( dialogo ), GTK_WIN_POS_CENTER );
  gtk_container_border_width( GTK_CONTAINER( dialogo ), 5 );

  if (( ancho )&&( alto ))
    gtk_widget_set_usize( GTK_WIDGET( dialogo ), ancho, alto);
    
  marco = gtk_frame_new( NULL );
  gtk_box_pack_start( GTK_BOX( GTK_DIALOG( dialogo )->vbox), marco,
      TRUE, TRUE, 5 );
  gtk_widget_show( marco );
  
  grupo = gtk_vbox_new( FALSE, 1 );
  gtk_container_border_width( GTK_CONTAINER( grupo ), 5 );
  gtk_container_add( GTK_CONTAINER( marco ), grupo );
  gtk_widget_show( grupo );
  
  texto = (char *)g_malloc( longitud );
  *texto = '\0';

  va_start( param, mensajes );
  for ( i=0 ; i<mensajes ; i++ )
    strcat( texto, va_arg( param, char* ));
  va_end( param );

  etiqueta = gtk_label_new( texto );
  gtk_box_pack_start( GTK_BOX( grupo ), etiqueta, TRUE, TRUE, 0 );
  gtk_widget_show( etiqueta );

  g_free( texto );
   
  boton = gtk_button_new_with_label( _("Ok") );
  gtk_signal_connect( GTK_OBJECT( boton ), "clicked",
      GTK_SIGNAL_FUNC( salir ), dialogo );
  gtk_box_pack_start( GTK_BOX( GTK_DIALOG( dialogo )->action_area), boton,
      TRUE, TRUE, 0 );
  gtk_widget_grab_focus( boton );
  gtk_widget_show( boton );
  
  gtk_widget_show( dialogo );
  
  return;
}
