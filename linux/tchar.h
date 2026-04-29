#pragma once
#ifndef _WIN32
// Fichier de compatibilité pour porter le code LibGraph2 vers Linux

// Mapping de _tWinMain vers WinMain avec injection automatique de main() sur
// Linux Cela permet de garder le code utilisateur intact.
#define _tWinMain(...)                                                         \
  WinMain(__VA_ARGS__);                                                        \
  int main(int argc, char **argv) { return WinMain(0, 0, (char *)"", 0); }     \
  int WinMain(__VA_ARGS__)

#endif
