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
#include <stdbool.h>
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

// Color_t
void Color_t_init(Color_t *c, int sz)
{
   c->i = calloc(sz, sizeof(int));
   c->sz = 0;
}

void Color_t_copy(Color_t *a, Color_t *b)
{
   for (int j = 0; j < b->sz; j++)
      a->i[j] = b->i[j];
   a->sz = b->sz;
}

// Vertices_t
void Vertices_t_init(Vertices_t *v, int size)
{
   v->sz = 0;
   v->v = calloc(size, sizeof(Vertex_t));
}

static int Vertices_t_degree_sort(const void *a, const void *b)
{
   Vertex_t *va = (Vertex_t *)a;
   Vertex_t *vb = (Vertex_t *)b;
   return (int)((int)va->d < (int)vb->d);
}

void Vertices_t_init_colors(Vertices_t *V)
{
   const int max_degree = V->v[0].d;
   for (int i = 0; i < max_degree; i++)
      V->v[i].d = i + 1;
   for (int i = max_degree; i < V->sz; i++)
      V->v[i].d = max_degree + 1;
}

// Maxclique_t
void Maxclique_t_init(Maxclique_t *m, int **conn, const int sz)
{
   m->pk = 0;
   m->level = 1;
   m->sz = sz;
   m->Tlimit = 0.025;

   assert(conn != 0 && sz > 0);
   Vertices_t_init(&m->V, sz);
   Color_t_init(&m->Q, sz);
   Color_t_init(&m->QMAX, sz);
   for (int i = 0; i < sz; i++)
      m->V.v[m->V.sz++].i = i;
   m->e = conn;

   m->C = calloc(sz + 1, sizeof(Color_t));
   for (int i = 0; i < sz + 1; i++)
      Color_t_init(&m->C[i], sz + 1);
}

void Maxclique_t_free(Maxclique_t *m)
{
   free(m->C);
   free(m->V.v);
   free(m->Q.i);
   free(m->QMAX.i);
}

int steps(Maxclique_t *m)
{
   return m->pk;
}

bool connection(Maxclique_t *m, const int i, const int j)
{
   return m->e[i][j];
}

bool cut1(Maxclique_t *m, const int pi, const Color_t *A)
{
   for (int i = 0; i < A->sz; i++)
      if (connection(m, pi, A->i[i]))
         return true;
   return false;
}

void cut2(Maxclique_t *m, const Vertices_t *A, Vertices_t *B)
{
   for (int i = 0; i < A->sz - 1; i++) {
      if (connection(m, A->v[A->sz - 1].i, A->v[i].i))
         B->v[B->sz++].i = A->v[i].i;
   }
}

void Vertices_t_init_degrees(Maxclique_t *m, Vertices_t *v)
{
   for (int i = 0; i < v->sz; i++) {
      int d = 0;
      for (int j = 0; j < v->sz; j++)
         if (connection(m, v->v[i].i, v->v[j].i))
            d++;
      v->v[i].d = d;
   }
}

void color_sort(Maxclique_t *m, Vertices_t *R)
{
   int j = 0;
   int maxno = 1;
   int min_k = m->QMAX.sz - m->Q.sz + 1;
   m->C[1].sz = 0;
   m->C[2].sz = 0;
   int k = 1;
   for (int i = 0; i < R->sz; i++) {
      int pi = R->v[i].i;
      k = 1;
      while (cut1(m, pi, &m->C[k]))
         k++;
      if (k > maxno) {
         maxno = k;
         m->C[maxno + 1].sz = 0;
      }
      m->C[k].i[m->C[k].sz++] = pi;
      if (k < min_k) {
         R->v[j++].i = pi;
      }
   }
   if (j > 0)
      R->v[j - 1].d = 0;
   if (min_k <= 0)
      min_k = 1;
   for (k = min_k; k <= maxno; k++)
      for (int i = 0; i < m->C[k].sz; i++) {
         R->v[j].i = m->C[k].i[i];
         R->v[j++].d = k;
      }
}

void expand(Maxclique_t *m, Vertices_t *R)
{
   while (R->sz) {
      if (m->Q.sz + R->v[R->sz - 1].d > m->QMAX.sz) {
         m->Q.i[m->Q.sz++] = R->v[R->sz - 1].i;
         Vertices_t Rp;
         Vertices_t_init(&Rp, R->sz);
         cut2(m, R, &Rp);

         if (Rp.sz) {
            color_sort(m, &Rp);
            m->pk++;
            expand(m, &Rp);
         }
         else if (m->Q.sz > m->QMAX.sz) {
            printf("step = \033[36m%d\033[0m \tcurrent max. clique size = %d\n",
                   m->pk, m->Q.sz);
            Color_t_copy(&m->QMAX, &m->Q);
         }
         free(Rp.v);
         m->Q.sz--;
      }
      else
         return;
      R->sz--;
   }
}

int *mcps(Maxclique_t *m, int *sz)
{
   Vertices_t_init_degrees(m, &m->V);
   qsort(m->V.v, m->V.sz, sizeof(Vertex_t *), Vertices_t_degree_sort);
   Vertices_t_init_colors(&m->V);
   expand(m, &m->V);

   int *maxclique;
   maxclique = calloc(m->QMAX.sz, sizeof(int));
   for (int i = 0; i < m->QMAX.sz; i++) {
      maxclique[i] = m->QMAX.i[i];
   }

   *sz = m->QMAX.sz;
   return maxclique;
}

#endif
