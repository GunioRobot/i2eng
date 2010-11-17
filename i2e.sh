#!/bin/bash

# Copyright 2000 Alfredo Casademunt <darkd@wanadoo.es>
# Copyright 2010 Juan Francisco Cantero Hurtado <iam@juanfra.info>
#
# This file is part of i2eng.
#
# i2eng is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

DICCIONARIO="/usr/share/i2e/i2e.dict"
DICCIONARIO_LOCAL="$(pwd)/i2e.dict"

if [ ! -f "$DICCIONARIO" ] && [ ! -f "$DICCIONARIO_LOCAL" ]; then
{
  echo
  gettext -d i2e -sn "Dictionary"
  echo -n " $DICCIONARIO "
  gettext -d i2e -s "not found."
  echo
  exit 1
}
elif [ ! -f "$DICCIONARIO" ] && [ -f "$DICCIONARIO_LOCAL" ]; then
{
  DICCIONARIO="$DICCIONARIO_LOCAL"
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
