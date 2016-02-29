cmake_minimum_required(VERSION 3.3)
project(PETITEAU_LACHTER)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Tests/affect-err.l
    Tests/affect.l
    Tests/boucle.l
    Tests/boucle_error.l
    Tests/essai1.l
    Tests/essai2.l
    Tests/essai3.l
    Tests/essai4.l
    Tests/expression.l
    Tests/fichierTest.l
    Tests/max.l
    Tests/tri.l
    affiche_arbre_abstrait.c
    affiche_arbre_abstrait.h
    analyseur_lexical.c
    analyseur_lexical.h
    analyseur_syntaxique.c
    analyseur_syntaxique.h
    premiers.c
    premiers.h
    suivants.c
    suivants.h
    symboles.h
    syntabs.c
    syntabs.h
    test_yylex.c
    util.c
    util.h)

add_executable(PETITEAU_LACHTER ${SOURCE_FILES})