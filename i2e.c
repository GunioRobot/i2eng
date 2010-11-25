/*
  Programa i2e
  Fichero i2e.c
  Autor Alfredo Casademunt
  Fecha 8-1-2000
*/

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <unistd.h>
#include "i2e.h"

#define PACKAGE "i2e"
#define LOCALEDIR "/usr/share/locale"

static gint miraPortapapeles( gpointer data )
{
  static GdkAtom atomo = GDK_NONE;

  if ( atomo == GDK_NONE )
  {
    atomo = gdk_atom_intern( "STRING", FALSE );
  }

  gtk_selection_convert( data, GDK_SELECTION_PRIMARY, atomo,
      GDK_CURRENT_TIME );

  return TRUE;
}

static void leidoPortapapeles( GtkWidget *widget, GtkSelectionData *clip,
    gint time, gpointer data )
{
  char *str;
  char *ptr;
  parametros *datos = (parametros *)data;

  if (( clip->length < 0 )||( clip->type != GDK_SELECTION_TYPE_STRING ))
  {
    return;
  }

  str = (char *)g_malloc( strlen( clip->data ) + 1 );
  strcpy( str, (char *)clip->data );
  ptr = strtrim( str );

  if (( datos->cambiado )||
      ( strcmp( ptr, gtk_entry_get_text( GTK_ENTRY( datos->entrada )))))
  {
    datos->cambiado = FALSE;
    gtk_entry_set_text( GTK_ENTRY( datos->entrada ), ptr );
    traduce( datos->boton, datos );
    gdk_window_raise( widget->window );
            
  }
  g_free( str );

  return;
}

static void salir( GtkWidget *widget, gpointer data )
{
  parametros *datos = (parametros *)data;
  g_free( datos->ficheros[1] );
  graba_rc( datos );
  gtk_main_quit();

  return;
}

static gint salirEv( GtkWidget *widget, GdkEventAny *ev, gpointer data )
{
  salir( widget, data );
  return TRUE;
}

/* sigue el estado del togglebutton traducir */
static void cambiaTraducir( GtkWidget *widget, gpointer data )
{
  char *direccion[2] = { _(" Spanish -> English "), _(" English -> Spanish ") };

  parametros *datos = (parametros *)data;
  datos->i2c = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( widget ));
  datos->cambiado = TRUE;
  gtk_label_set_text( GTK_LABEL( GTK_TOGGLE_BUTTON( widget )->button.bin.child ),
      direccion[datos->i2c] );

  gtk_entry_select_region( GTK_ENTRY( datos->entrada ), 0,
      strlen( gtk_entry_get_text( GTK_ENTRY( datos->entrada ))));
  gtk_widget_grab_focus( datos->entrada );
  
  return;
}

/* sigue el estado del togglebutton busqueda */
static void cambiaBuscar( GtkWidget *widget, gpointer data )
{
  char *busqueda[2] = { _(" Raw search "), _(" Exact search ") };

  parametros *datos = (parametros *)data;
  datos->exacto = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( widget ));
  datos->cambiado = TRUE;
  gtk_label_set_text( GTK_LABEL( GTK_TOGGLE_BUTTON( widget )->button.bin.child ),
      busqueda[datos->exacto] );

  gtk_entry_select_region( GTK_ENTRY( datos->entrada ), 0,
      strlen( gtk_entry_get_text( GTK_ENTRY( datos->entrada ))));
  gtk_widget_grab_focus( datos->entrada );
  
  return;
}

/* estando enfocada la linea de entrada si:
   a) se pulsa la tecla Intro entonces se pulsa el boton de traducir
   b) se pulsan pagina arriba o abajo entonces se desplaza el texto traducido
*/
static gint pulsaTecla( GtkWidget *widget, GdkEventKey *ev, gpointer data )
{
  parametros *datos;
  gint retorno;

  datos = (parametros *)data;
  
  if ( ev->keyval == GDK_KP_Enter )
  {
    ev->keyval = GDK_Return;
  }

  if ( ev->keyval == GDK_Return )
  {
    gtk_signal_emit_by_name( GTK_OBJECT( datos->boton ), "clicked",
        &retorno );
    return TRUE;
  }

  if (( ev->keyval == GDK_Page_Up )||( ev->keyval == GDK_Page_Down ))
  {
    gtk_signal_emit_by_name( GTK_OBJECT( datos->salida ),
        "key_press_event", ev, &retorno );
    return TRUE;
  }
  
  return FALSE;
}

int main( int argc, char *argv[] )
{
  GtkWidget *ventana;
  GtkWidget *tabla;
  GtkWidget *chequeo;
  GtkWidget *subtabla;
  GtkWidget *barra;
  GtkWidget *boton;
  parametros datos;
  char *direccion[2];
  char *busqueda[2];
  size_t cpl;    /* caracteres por linea */

  setlocale( LC_ALL, "es_ES.ISO-8859-1" );
/*  setlocale( LC_MESSAGES, "" );*/
  bindtextdomain( PACKAGE, LOCALEDIR );
  textdomain( PACKAGE );

  direccion[0] = _(" Spanish -> English ");
  direccion[1] = _(" English -> Spanish ");
  busqueda[0] = _(" Raw search ");
  busqueda[1] = _(" Exact search ");

  /* inicializa datos */
  if (! carga_rc( &datos ))
  {
    datos.i2c = 1;
    datos.exacto = 0;
  }

  datos.ficheros[0] = DICCIONARIO;
  
  datos.cpl = maxLinea( datos.ficheros[0] );
  if (! datos.cpl )
  {
    return 1;
  }
  
  datos.ficheros[1] = nomPersonal( PERSONAL_DICT );
  if ( access( datos.ficheros[1], R_OK ))
  {
    g_free( datos.ficheros[1] );
    datos.ficheros[1] = NULL;
  }
  else
  {
    cpl = maxLinea( datos.ficheros[1] );
    if (! cpl )
    {
      g_free( datos.ficheros[1] );
      return 1;
    }
    
    if ( cpl > datos.cpl )
    {
      datos.cpl = cpl;
    }
  }

  datos.cambiado = FALSE;

  /*if (! diccioOK( &datos ))
  {
    g_free( datos.ficheros[1] );
    return 0;
  }*/

/*  gtk_set_locale();*/
  gtk_init( &argc, &argv );

  /* crea la ventana principal */
  ventana = gtk_window_new( GTK_WINDOW_TOPLEVEL );
 /* gtk_window_set_modal( GTK_WINDOW( ventana ), TRUE );*/
  gtk_window_set_title( GTK_WINDOW(ventana), "i2e" );
  gtk_container_border_width( GTK_CONTAINER( ventana ), 5 );

  /* los eventos "delete_event" y "destroy" terminan el programa */
  gtk_signal_connect( GTK_OBJECT( ventana ), "delete_event",
      GTK_SIGNAL_FUNC( salirEv ), (gpointer)&datos );
  gtk_signal_connect( GTK_OBJECT( ventana ), "destroy",
      GTK_SIGNAL_FUNC( salir ), (gpointer)&datos );

  /* leido del portapapeles */
  gtk_signal_connect( GTK_OBJECT( ventana ), "selection_received",
      GTK_SIGNAL_FUNC( leidoPortapapeles ), (gpointer)&datos );


  /* tabla para toda la ventana */
  tabla = gtk_table_new( 6, 6, FALSE );
  gtk_table_set_row_spacings( GTK_TABLE( tabla ), 5 );
  gtk_table_set_col_spacings( GTK_TABLE( tabla ), 5 );
  gtk_container_add( GTK_CONTAINER( ventana ), tabla );
  gtk_widget_show( tabla );  

  chequeo = gtk_toggle_button_new_with_label( direccion[datos.i2c] );
  gtk_toggle_button_set_state( GTK_TOGGLE_BUTTON( chequeo ), datos.i2c );
  gtk_signal_connect( GTK_OBJECT( chequeo ), "toggled",
      GTK_SIGNAL_FUNC( cambiaTraducir ), (gpointer)&datos );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), chequeo, 0, 3, 0, 1 );
  gtk_widget_show( chequeo );
                                               
  chequeo = gtk_toggle_button_new_with_label( busqueda[datos.exacto] );
  gtk_toggle_button_set_state( GTK_TOGGLE_BUTTON( chequeo ), datos.exacto );
  gtk_signal_connect( GTK_OBJECT( chequeo ), "toggled",
      GTK_SIGNAL_FUNC( cambiaBuscar ), (gpointer)&datos );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), chequeo, 3, 6, 0, 1 );
  gtk_widget_show( chequeo );

  /* (sub)tabla para el texto de la traducción y las barras de scroll */
  subtabla = gtk_table_new( 2, 2, FALSE );
  gtk_table_set_row_spacings( GTK_TABLE( subtabla ), 0 );
  gtk_table_set_col_spacings( GTK_TABLE( subtabla ), 0 );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), subtabla, 0, 6, 1, 4 );
  gtk_widget_show( subtabla );  

  /* widget para el texto de la traduccion en la subtabla */
  datos.salida = gtk_text_new( NULL, NULL );
  gtk_text_set_editable( GTK_TEXT( datos.salida ), FALSE );
  gtk_text_set_word_wrap( GTK_TEXT( datos.salida ), FALSE );
  gtk_table_attach_defaults( GTK_TABLE( subtabla ), datos.salida, 0, 1, 0, 1);
  gtk_widget_show( datos.salida );

  /* scroll horizontal en la subtabla */
  barra = gtk_hscrollbar_new( GTK_TEXT( datos.salida )->hadj );
  GTK_WIDGET_UNSET_FLAGS( GTK_HSCROLLBAR( barra ), GTK_CAN_FOCUS );
  gtk_table_attach( GTK_TABLE( subtabla ), barra, 0, 1, 1, 2,
      GTK_EXPAND | GTK_FILL, GTK_FILL, 0, 0 );
  gtk_widget_show( barra ); 
                                                
  /* scroll vertical en la subtabla */
  barra = gtk_vscrollbar_new( GTK_TEXT( datos.salida )->vadj );
  GTK_WIDGET_UNSET_FLAGS( GTK_VSCROLLBAR( barra ), GTK_CAN_FOCUS );
  gtk_table_attach( GTK_TABLE( subtabla ), barra, 1, 2, 0, 1,
      GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0 );
  gtk_widget_show( barra );

  /* crea la linea de entrada de texto */
  datos.entrada = gtk_entry_new();
  gtk_table_attach_defaults( GTK_TABLE( tabla ), datos.entrada, 0, 6, 4, 5 );
  GTK_WIDGET_SET_FLAGS( datos.entrada, GTK_CAN_FOCUS );
  gtk_widget_grab_focus( datos.entrada );
  gtk_widget_show( datos.entrada );

  datos.boton = gtk_button_new_with_label( _("Translate"));
  gtk_signal_connect( GTK_OBJECT( datos.boton ), "clicked",
      GTK_SIGNAL_FUNC( traduce ), (gpointer)&datos );
  gtk_signal_connect( GTK_OBJECT( datos.entrada ), "key_press_event",
      GTK_SIGNAL_FUNC( pulsaTecla ), (gpointer)&datos );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), datos.boton, 0, 2, 5, 6 );
  GTK_WIDGET_SET_FLAGS( datos.boton, GTK_CAN_DEFAULT );
  gtk_widget_grab_default( datos.boton );
  gtk_widget_show( datos.boton );
  
  boton = gtk_button_new_with_label( _("Learn"));
  gtk_signal_connect( GTK_OBJECT( boton ), "clicked",
      GTK_SIGNAL_FUNC( aprende ), (gpointer)&datos );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), boton, 2, 4, 5, 6 );
  GTK_WIDGET_SET_FLAGS( boton, GTK_CAN_DEFAULT );
  gtk_widget_show( boton );

  boton = gtk_button_new_with_label( _("Exit"));
  gtk_signal_connect( GTK_OBJECT( boton ), "clicked",
      GTK_SIGNAL_FUNC( salir ), (gpointer)&datos );
  gtk_table_attach_defaults( GTK_TABLE( tabla ), boton, 4, 6, 5, 6 );
  GTK_WIDGET_SET_FLAGS( boton, GTK_CAN_DEFAULT );
  gtk_widget_show( boton );

  gtk_widget_show( ventana );

  gtk_timeout_add( 500, (GtkFunction)miraPortapapeles, ventana );

  gtk_main();
  
  return 0;
}
