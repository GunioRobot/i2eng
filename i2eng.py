#!/usr/bin/env python3

# Copyright 2011 Juan Francisco Cantero Hurtado <iam@juanfra.info>
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

import readline
from collections import defaultdict
import sys
import textwrap

diccionario_es = defaultdict(list)
diccionario_en = defaultdict(list)

def generar_diccionario():
    fichero_diccionario = open("i2e.dict", "r")

    for linea in fichero_diccionario:
        ingles, espaniol = linea.rstrip('\n').split(" : ")
        diccionario_es[espaniol].append(ingles)
        diccionario_en[ingles].append(espaniol)

    fichero_diccionario.close()

def numero_definiciones():
    print ("Definiciones en español: ", len(diccionario_es.keys()))
    print ("Definiciones en inglés: ", len(diccionario_en.keys()))
    print ("---")

def busqueda(palabra):
    if busqueda_precisa == True:
        if diccionario.get(palabra) != None:
            print ('=>', palabra)
            for resultado in (diccionario.get(palabra)):
                print ('-', resultado)
        else:
            print ("#", palabra, "no existe en el diccionario. Revisa si está bien escrita o prueba con la búsqueda aproximada.")

    else:
        for linea in diccionario:
            if palabra in linea:
                print ('=>', linea)
                for resultado in (diccionario[linea]):
                    print ('-', resultado)

def ayuda():
    texto_help = (
            """
            Comandos:
            /ayuda -> Este texto
            /exit -> Sale del programa
            /cambiar -> Cambia el diccionario por defecto
            /precision -> Cambia entre una búsqueda aproximada y otra más precisa
            /version -> Muestra la versión
            """
            )
    print (textwrap.dedent(texto_help))

def version():
    texto_version = "Versión de desarrollo. Puedes ver los autores y licencia en https://github.com/juanfra684/i2eng"
    print ("\n", texto_version)

diccionario = diccionario_es
def cambiar_diccionario():
    global diccionario
    if diccionario == diccionario_es:
        diccionario = diccionario_en
        print ("\n", "Diccionario: Inglés -> Español")
    elif diccionario == diccionario_en:
        diccionario = diccionario_es
        print ("\n", "Diccionario: Español -> Inglés")

busqueda_precisa = True
def precision():
    global busqueda_precisa
    if busqueda_precisa == False:
        busqueda_precisa = True
        print ("\n", "Búsqueda precisa")
    elif busqueda_precisa == True:
        busqueda_precisa = False
        print ("\n", "Búsqueda aproximada")

def ayuda_inicio():
    print ("Diccionario por defecto: Español -> Inglés")
    print ("Para buscar un término simplemente escríbelo.")
    print ("""Puedes cambiar el diccionario escribiendo "/cambiar", salir con "/exit" y obtener más ayuda con "/ayuda".""")
    print ("---")

def principal():
    while True:
        try:
            entrada = input(": ")
            entrada.lower()

            if "/exit" in entrada:
                sys.exit()
            elif "/cambiar" in entrada:
                cambiar_diccionario()
            elif "/version" in entrada:
                version()
            elif "/versión" in entrada:
                version()
            elif "/ayuda" in entrada:
                ayuda()
            elif "/help" in entrada:
                ayuda()
            elif "/precision" in entrada:
                precision()
            elif "/precisión" in entrada:
                precision()
            else:
                busqueda(entrada)

            print ("---")

        except KeyboardInterrupt:
            print ("See you!")
            sys.exit()


generar_diccionario()

numero_definiciones()

ayuda_inicio()

principal()
