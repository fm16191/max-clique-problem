/*
    Copyright 2022 fm1619

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MCP_H
#define MCP_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Color_s {
   int *i;
   int sz;
} Color_t;

typedef struct Vertex_s {
   int i, d;
} Vertex_t;

typedef struct Vertices_s {
   Vertex_t *v;
   int sz;
} Vertices_t;

typedef struct Maxclique_s {
   int **e;
   int pk, level;
   int sz;
   float Tlimit;
   Vertices_t V;
   Color_t *C, QMAX, Q;
} Maxclique_t;

void Color_t_init(Color_t *c, int sz);
void Color_t_copy(Color_t *a, Color_t *b);

void Vertices_t_init(Vertices_t *v, int size);
int Vertices_t_degree_sort(const void *a, const void *b);
void Vertices_t_init_colors(Vertices_t *V);

void Maxclique_t_init(Maxclique_t *m, int **conn, const int sz);
void Maxclique_t_free(Maxclique_t *m);
int steps(Maxclique_t *m);
int connection(Maxclique_t *m, const int i, const int j);
int cut1(Maxclique_t *m, const int pi, const Color_t *A);
void cut2(Maxclique_t *m, const Vertices_t *A, Vertices_t *B);

void Vertices_t_init_degrees(Maxclique_t *m, Vertices_t *v);
void color_sort(Maxclique_t *m, Vertices_t *R);

void expand(Maxclique_t *m, Vertices_t *R);
int *mcps(Maxclique_t *m, int *sz);

#endif
