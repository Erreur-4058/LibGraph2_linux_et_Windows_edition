cmake_minimum_required(VERSION 3.14)
project(LibGraph2 VERSION 2.0 LANGUAGES CXX C)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir LIBGRAPH2_USE_SFML et LIBGRAPH2_EXPORTS
add_compile_definitions(LIBGRAPH2_USE_SFML)


# ========================================================
# NOUVEAU : Téléchargement automatique de SFML 2.6.2 
# (Bypass la version 3.0 de MSYS2 via un fichier ZIP)
# ========================================================
include(FetchContent)
FetchContent_Declare(
  SFML
  URL https://github.com/SFML/SFML/archive/refs/tags/2.6.2.zip
)
# On désactive la compilation des exemples/docs de SFML pour gagner du temps
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)
# ========================================================

# Sources principales
set(SOURCES
    LibGraph2Common.cpp
    LibGraph2impSFML.cpp
)

# Création de la bibliothèque partagée
add_library(LibGraph2 SHARED ${SOURCES})
target_compile_definitions(LibGraph2 PRIVATE LIBGRAPH2_EXPORTS)

# Liaison avec SFML
target_link_libraries(LibGraph2 
    sfml-system 
    sfml-window 
    sfml-graphics
)

# Répertoires d'inclusion
target_include_directories(LibGraph2 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Installation (optionnelle)
install(TARGETS LibGraph2 DESTINATION lib)
install(FILES LibGraph2.h DESTINATION include)

# Programme de test
add_executable(test_libgraph2 WIN32 test_libgraph2.cpp)
target_link_libraries(test_libgraph2 LibGraph2)