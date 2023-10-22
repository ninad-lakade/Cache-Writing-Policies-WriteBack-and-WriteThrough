#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_SIZE 16   // Number of cache lines
#define CACHE_WORD_SIZE 8 // Word size in cache (in bits)
#define MEMORY_SIZE 64  // Number of memory words
#define MEMORY_WORD_SIZE 8 // Word size in memory (in bits)
#define MAX_DATA_SIZE 100

// Data structure for cache line
typedef struct {
    char data[CACHE_WORD_SIZE + 1]; // +1 for null terminator
    int dirty; // 1 if dirty, 0 if clean
} CacheLine;

// Data structure for memory
typedef struct {
    char data[MEMORY_WORD_SIZE + 1]; // +1 for null terminator
} MemoryWord;

// Initialize cache and memory
CacheLine cache[CACHE_SIZE];
MemoryWord memory[MEMORY_SIZE];

// Initialize dirty bits
int dirty_bits[CACHE_SIZE] = {0};

// Initialize cache and memory
void initialize_cache_and_memory() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        strcpy(cache[i].data, "");
        dirty_bits[i] = 0;
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        strcpy(memory[i].data, "");
    }
}

// Read a word from cache or memory
char* read_word(int address) {
    int index = address % CACHE_SIZE;
    if (strcmp(cache[index].data, memory[address].data) != 0) {
        dirty_bits[index] = 1;
    }
    return cache[index].data;
}

// Write a word to cache and set the dirty bit
void write_word(int address, char* data) {
    int index = address % CACHE_SIZE;

    // Check if the dirty bit is set (cache line is dirty)
    if (dirty_bits[index] == 1) {
        // Write the old cache data back to the corresponding memory location
        strcpy(memory[address].data, cache[index].data);
        printf("Writing back cache line with tag %d to memory\n", address);
        dirty_bits[index] = 0; // Clear the dirty bit
    }

    // Update the cache with the new data
    strcpy(cache[index].data, data);
    dirty_bits[index] = 1; // Set the dirty bit since the cache has new data
    printf("Data written to cache and marked dirty for address %d: %s\n", address, data);
}

// Save cache contents to a text file
void save_cache_to_file() {
    FILE* cache_file = fopen("cache.txt", "w");
    if (cache_file == NULL) {
        perror("Error opening cache.txt");
        return;
    }
    for (int i = 0; i < CACHE_SIZE; i++) {
        fprintf(cache_file, "%s %d\n", cache[i].data, dirty_bits[i]);
    }
    fclose(cache_file);
    printf("Cache contents saved to cache.txt\n");
}

// Load cache contents from a text file
void load_cache_from_file() {
    FILE* cache_file = fopen("cache.txt", "r");
    if (cache_file == NULL) {
        perror("Error opening cache.txt");
        return;
    }
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (fscanf(cache_file, "%s %d", cache[i].data, &dirty_bits[i]) != 2) {
            fprintf(stderr, "Error reading cache file\n");
            fclose(cache_file);
            return;
        }
    }
    fclose(cache_file);
    printf("Cache contents loaded from cache.txt\n");
}

// Load memory contents from a text file
void load_memory_from_file() {
    FILE* memory_file = fopen("memory.txt", "r");
    if (memory_file == NULL) {
        perror("Error opening memory.txt");
        return;
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (fscanf(memory_file, "%s", memory[i].data) != 1) {
            fprintf(stderr, "Error reading memory file\n");
            fclose(memory_file);
            return;
        }
    }
    fclose(memory_file);
    printf("Memory contents loaded from memory.txt\n");
}

// Save memory contents to a text file
void save_memory_to_file() {
    FILE* memory_file = fopen("memory.txt", "w");
    if (memory_file == NULL) {
        perror("Error opening memory.txt");
        return;
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        fprintf(memory_file, "%s\n", memory[i].data);
    }
    fclose(memory_file);
    printf("Memory contents saved to memory.txt\n");
}

int main() {
    initialize_cache_and_memory(); // Initialize cache and memory

    load_cache_from_file(); // Load initial cache contents
    load_memory_from_file(); // Load initial memory contents

    int choice;
    int address;
    char data[MAX_DATA_SIZE];
    char* word_data;

    while (1) {
        printf("\nOptions:\n");
        printf("1. Read a word from cache/memory\n");
        printf("2. Write a word to cache/memory\n");
        printf("3. Save cache and memory contents to files\n");
        printf("4. Display cache contents\n");
        printf("5. Display memory contents\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                printf("Enter the cache address to read: ");
                if (scanf("%d", &address) != 1 || address < 0 || address >= MEMORY_SIZE) {
                    fprintf(stderr, "Invalid memory address.\n");
                    return 1;
                }
                word_data = read_word(address);
                printf("Read from memory address %d: %s\n", address, word_data);
                break;

            case 2:
                printf("Enter the address to write in cache: ");
                if (scanf("%d", &address) != 1 || address < 0 || address >= MEMORY_SIZE) {
                    fprintf(stderr, "Invalid memory address.\n");
                    return 1;
                }
                printf("Enter the data to write (up to 8 characters): ");
                if (scanf("%8s", data) != 1) {
                    fprintf(stderr, "Invalid data.\n");
                    return 1;
                }
                write_word(address, data);
                printf("Wrote to memory address %d: %s\n", address, data);
                break;

            case 3:
                save_cache_to_file();
                save_memory_to_file();
                break;

            case 4:
                printf("Cache Contents:\n");
                for (int i = 0; i < CACHE_SIZE; i++) {
                    printf("Line %d: Data: %s, Dirty Bit: %d\n", i, cache[i].data, dirty_bits[i]);
                }
                break;

            case 5:
                printf("Memory Contents:\n");
                for (int i = 0; i < MEMORY_SIZE; i++) {
                    printf("Word %d: Data: %s\n", i, memory[i].data);
                }
                break;

            case 6:
                save_cache_to_file(); // Save cache contents before exiting
                return 0;

            default:
                fprintf(stderr, "Invalid choice. Please select a valid option.\n");
        }
    }
}

