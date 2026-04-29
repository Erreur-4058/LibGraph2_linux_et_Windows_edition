cmake_minimum_required(VERSION 3.10)
project(LibGraph2 VERSION 2.0 LANGUAGES CXX C)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir LIBGRAPH2_USE_SFML et LIBGRAPH2_EXPORTS
add_definitions(-DLIBGRAPH2_USE_SFML -DLIBGRAPH2_EXPORTS)

# Recherche de SFML
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Sources principales (après refactoring)
set(SOURCES
    LibGraph2Common.cpp
    LibGraph2impSFML.cpp
    
)

# Inclusion de tinyfiledialogs (à télécharger)
# set(SOURCES ${SOURCES} tinyfiledialogs.c)

# Création de la bibliothèque partagée
add_library(LibGraph2 SHARED ${SOURCES})

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

# Installation
install(TARGETS LibGraph2 DESTINATION lib)
install(FILES LibGraph2.h DESTINATION include)

# Programme de test
add_executable(test_libgraph2 test_libgraph2.cpp)
target_link_libraries(test_libgraph2 LibGraph2)
