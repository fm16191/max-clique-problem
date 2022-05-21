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

#ifndef _GNU_SOURCE
   #define _GNU_SOURCE
#endif

#include "mcps.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **read_file(char *name, int *size)
{
   int **conn;
   char *filepath = NULL;
   filepath = malloc(sizeof(char) * (strlen(name) + 4));
   strcpy(filepath, "./");
   strcat(filepath, name);
   FILE *input;

   if ((input = fopen(filepath, "r")) == NULL)
      return printf("Error: Cannot open %s\n", filepath), NULL;

   char *line;
   size_t len = 0;
   ssize_t read;

   // Read Header
   // Format : ^c .*$
   do {
      read = getline(&line, &len, input);
   } while (read != -1 && line[0] == 'c');

   int nodes, edges;
   sscanf(line, "p edge %d %d", size, &edges);
   nodes = (int)*size;
   printf("Nodes : %d (%d edges)\n\n", nodes, edges);

   nodes++;
   *size = nodes;
   conn = malloc(sizeof(int *) * nodes);
   for (int i = 0; i < nodes; i++) {
      conn[i] = calloc(nodes, sizeof(int));
   }

   int vi, vj;
   while ((read = getline(&line, &len, input)) != -1) {
      if (line[0] == 'e') {
         sscanf(line, "e %d %d\n", &vi, &vj);
         conn[vi][vj] = 1;
         conn[vj][vi] = 1;
      }
   }
   return conn;
}

int main(int argc, char *argv[])
{
   if (argc != 2) {
      printf("Usage : %s [file.clq]\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("Reading file : %s\n", argv[1]);

   int **conn;
   int size;

   conn = read_file(argv[1], &size);
   if (!conn)
      return EXIT_FAILURE;

   Maxclique_t m;
   Maxclique_t_init(&m, conn, size);

   int *qmax = NULL;
   int qsize;

   clock_t start = clock();
   qmax = mcps(&m, &qsize);
   clock_t end = clock();

   printf("Maximum clique : \n");
   for (int i = 0; i < qsize; i++)
      printf("%d ", qmax[i]);
   printf("\n");
   printf("Size = %d\n", qsize);
   printf("Number of steps = %d\n", steps(&m));
   printf("Execution time = %f secs.\n",
          ((double)(end - start)) / CLOCKS_PER_SEC);

   free(qmax);
   free(conn);
   Maxclique_t_free(&m);

   return EXIT_SUCCESS;
}
