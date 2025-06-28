#pragma once
#include "notas.h"

const int melodiaMenu[][2] = {
  {MI5, 200}, {SOL5, 200}, {SI5, 200}, {SOL5, 200}, {MI5, 300},
  {RE5, 200}, {FA5, 200}, {LA5, 200}, {FA5, 200}, {RE5, 300},
  {MI5, 150}, {RE5, 150}, {DO5, 300}, {REST, 200}
};

const int numNotasMenu = sizeof(melodiaMenu) / sizeof(melodiaMenu[0]);