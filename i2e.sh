#!/bin/sh

# i2e.sh Versión 0.3

# Script creado por Alfredo Casademunt (darkd@wanadoo.es) al 8-1-2000
# bajo licencia GPL como alternativa en modo texto para i2e

DICCIONARIO="/usr/share/i2e/i2e.dict"

if [ ! -f "$DICCIONARIO" ]; then
{
  echo
  gettext -d i2e -sn "Dictionary"
  echo -n " $DICCIONARIO "
  gettext -d i2e -s "not found."
  echo
  exit 1
}
fi

if [ -z "$1" ]; then
{
  echo
  gettext -d i2e -s "i2e.sh [-re] [word]"
  echo
  gettext -d i2e -sn "By default, i2e.sh translates from English into Spanish with"
  gettext -d i2e -s " a \"raw\" search."
  gettext -d i2e -s "With -r option word is translated from Spanish into English."
  gettext -d i2e -s "With -e option an \"exact\" search will be performed."
  echo
  exit 0
}
fi

declare -i NUMERO

getopts ":re" OPCION

if [ -n "$OPTARG" ]; then
{
  echo
  gettext -d i2e -sn "Option"
  echo -n " \"-$OPTARG\" "
  gettext -d i2e -s "not valid."
  echo
  exit 1
}
fi

case $OPCION in
  "r")  
    getopts ":e" OPCION

    if [ -n "$OPTARG" ]; then
    {
      echo
      gettext -d i2e -sn "Option"
      echo -n " \"-$OPTARG\" "
      gettext -d i2e -s "not valid."
      echo
      exit 1
    }
    fi
    
    NUMERO=$OPTIND-1
    shift $NUMERO

    PATRON=`echo $* | sed -e 's/[aá]/[aá]/g; s/[eé]/[eé]/g; \
                      s/[ií]/[ií]/g; s/[oó]/[oó]/; s/[uúü]/[uúü]/g'`
    
    if [ "$OPCION" = "e" ]; then
    {
      echo
      grep -i ".* : $PATRON .*$" "$DICCIONARIO"
    }
    else
    {
      echo
      grep -i ".* : $PATRON" "$DICCIONARIO"
    }
    fi
    
    if [ $? -ne 0 ]; then
    {
      gettext -d i2e -sn "Not found in Spanish"
      echo " \"$*\"."
    }
    fi
  ;;
    
  "e")

    getopts ":r" OPCION

    if [ -n "$OPTARG" ]; then
    {
      echo
      gettext -d i2e -sn "Option"
      echo -n " \"-$OPTARG\" "
      gettext -d i2e -s "not valid."
      echo
      exit 1
    }
    fi
    
    NUMERO=$OPTIND-1
    shift $NUMERO
    
    if [ "$OPCION" = "r" ]; then
    {
      PATRON=`echo $* | sed -e 's/[aá]/[aá]/g; s/[eé]/[eé]/g; \
                        s/[ií]/[ií]/g; s/[oó]/[oó]/; s/[uúü]/[uúü]/g'`
      
      echo
      grep -i ".* : $PATRON .*$" "$DICCIONARIO"
      
      if [ $? -ne 0 ]; then
      {
        gettext -d i2e -sn "Not found in Spanish"
        echo " \"$*\"."
      }
      fi
    }
    else
    {
      echo
      grep -i "^$* .*: " "$DICCIONARIO"
      
      if [ $? -ne 0 ]; then
      {
        gettext -d i2e -sn "Not found in English"
        echo " \"$*\"."
      }
      fi
    }
    fi
  ;;
    
  "?")
    echo
    grep -i "^$*.* : " "$DICCIONARIO"
      
    if [ $? -ne 0 ]; then
    {
      gettext -d i2e -sn "Not found in English"
      echo " \"$*\"."
    }
    fi
  ;;

esac
echo

exit 0
