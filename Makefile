#Cioranu Nickolass-Alexandru

# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# Define targets, e.g., ninel, codeinvim, vectsecv, nomogram.
TARGETS=battleships

# Manually define all targets.
build: $(TARGETS)

battleships: battleships.c
	$(CC) $(CFLAGS) battleships.c -o battleships -lm


# Pack the solution into a zip file.
pack:
	zip -FSr Battleships.zip README Makefile battleships.c

# Clean the solution.
clean:
	rm -f $(TARGETS)

.PHONY: pack clean
