# Copyright 2022 fm1619

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


CC=gcc
CFLAGS=-g3 -Wall -Wextra -I include/
OFLAGS=-Ofast -march=native -mtune=native
# OFLAGS=-Ofast -march=native -mtune=native -funroll-loops -finline-functions -ftree-loop-vectorize -ftree-vectorize
LDFLAGS=
TARGET=mcps
DEPS=build
SRC=src

all: $(TARGET)

$(DEPS)/%.o: $(SRC)/%.c
	@mkdir -p $(DEPS)
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@

mcps: $(DEPS)/main.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LDFLAGS)

clean: 
	rm -f $(TARGET) $(DEPS) *.o

example: $(TARGET)
	./$(TARGET) samples/C125.9.clq

verif: build
	./$(TARGET) test.clq | head -n 29 > tmp.txt
	cat tmp.txt
	echo "=================="
	diff --color=auto tmp.txt verif.txt

memcheck:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(TARGET) samples/C125.9.clq