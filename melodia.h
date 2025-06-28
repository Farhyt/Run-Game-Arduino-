#pragma once
#include "notas.h"

const int melodia[][2] = {
  {MI5, 150}, {SOL5, 150}, {MI5, 150}, {SOL5, 150},
  {RE5, 200}, {REST, 100}, {RE5, 200}, {REST, 100},
  {DO5, 300}, {MI5, 150}, {SOL4, 200}, {REST, 150},
  {RE5, 150}, {LA4, 200}, {FA5, 200}, {REST, 100},
  {SOL5, 200}, {DO5, 150}, {MI5, 300}
};

const int numNotas = sizeof(melodia) / sizeof(melodia[0]);