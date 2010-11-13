/*
  Programa i2e
  Fichero i2e.h
  Autor Alfredo Casademunt
  Fecha 8-1-2000
*/

#include <sys/types.h>
#include <gtk/gtk.h>

#include <libintl.h>
#define _(Cadena) gettext(Cadena)

#define DICCIONARIO "/usr/share/i2e/i2e.dict"
#define PERSONAL_DICT "/.i2e.dict"
#define PERSONAL_RC "/.i2erc"

typedef struct
{
  GtkWidget *entrada;
  GtkWidget *salida;
  GtkWidget *boton;
  char *ficheros[2];
  size_t cpl;
  int i2c;
  int exacto;
  int cambiado;
} parametros;

/* Reserva menoria con g_malloc() y devuelve una cadena
   con el nombre : "$HOME/nom"
*/   
char *nomPersonal( char *nom );

/* Quita los caracteres no alfabeticos al principio y al final
   de la cadenachar *strtrim( char *str )
*/
char *strtrim( char *str );

/* Devuelve el numero maximo de caracteres en una linea
   incluido el caracter de nueva linea y mas uno.
   O devuelve 0 si ha habido algun error.
*/
size_t maxLinea( char *fichero );

/* Comprueba el formato de los diccionarios y devuelve 1
   si no hay error o 0 en caso contrario
*/
/*int diccioOK( parametros *datos );*/

/* para mostrar mensajes */
void mensaje( char *titulo, gint ancho, gint alto, gint mensajes, ... );

/* busca y muestra una traducción */
void traduce( GtkWidget *widget, gpointer data );

/* añade palabras en el diccionario personal */
void aprende( GtkWidget *widget, gpointer data );

int carga_rc( parametros *datos );
void graba_rc( parametros *datos );

