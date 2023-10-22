#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random 8-bit binary number
char getRandomBinary() {
    return (char)(rand() % 2 + '0'); // Generates '0' or '1'
}

int main() {
    // Create and open memory.txt for writing
    FILE *memoryFile = fopen("memory.txt", "w");
    if (memoryFile == NULL) {
        perror("Error opening memory.txt");
        return 1;
    }

    // Create and open cache.txt for writing
    FILE *cacheFile = fopen("cache.txt", "w");
    if (cacheFile == NULL) {
        perror("Error opening cache.txt");
        fclose(memoryFile);  // Close the memory file before exiting
        return 1;
    }

    // Initialize random number generator
    srand(time(NULL));

    // Create and append random binary data to memory.txt
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 8; j++) {
            char bit = getRandomBinary();
            fputc(bit, memoryFile);
        }
        fputc('\n', memoryFile); // Add a newline after each 8-bit binary number
    }

    // Create and append random binary data to cache.txt
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            char bit = getRandomBinary();
            fputc(bit, cacheFile);
        }
        fputc('\n', cacheFile); // Add a newline after each 8-bit binary number
    }

    // Close both files
    fclose(memoryFile);
    fclose(cacheFile);

    printf("Random binary data has been written to memory.txt and cache.txt.\n");

    return 0;
}

