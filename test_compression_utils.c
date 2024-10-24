#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "compression_utils/compression.h"

// Function declarations
int is_compressed(unsigned char* data_ptr, int data_size);
int byte_compress(unsigned char* data_ptr, int data_size);
int byte_decompress(unsigned char* data_ptr, int compressed_size, unsigned char* decompressed_data);

// Helper function to print the array in a readable format
void print_array(const unsigned char* array, int size, const char* description) {
    printf("%s: [", description);
    for (int i = 0; i < size; i++) {
        printf("0x%02X", array[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Main function to test different cases
int main() {
    // Example data for testing
    unsigned char data_case_1[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 
                                    0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 }; // Original data
    unsigned char data_case_2[] = { 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x01, 0x02, 
                                    0x03, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // Random windows + repeated windows
    unsigned char data_case_3[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };  // Data that might not compress well

    unsigned char decompressed_data[100]; // Buffer for decompression

    unsigned char* test_cases[] = { data_case_1, data_case_2, data_case_3};
    int test_case_sizes[] = { sizeof(data_case_1), sizeof(data_case_2), sizeof(data_case_3) };
    int num_test_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    // Test each case for compression and decompression
    for (int i = 0; i < num_test_cases; i++) {
        unsigned char* data = test_cases[i];
        int data_size = test_case_sizes[i];

        printf("\n===== Test Case %d =====\n", i + 1);

        // Print original data
        print_array(data, data_size, "Original Data");

        // Compress the data
        int compressed_size = byte_compress(data, data_size);

        // Print compressed data and size
        print_array(data, compressed_size, "Compressed Data");
        printf("Compressed Size: %d bytes\n", compressed_size);
        // Calculate and print the compression factor
        if (compressed_size < data_size) {
            double compression_factor = (double)data_size / compressed_size;
            printf("Compression Factor: %.2f (Original size / Compressed size)\n", compression_factor);
        } else {
            printf("Compression Factor: 1.00 (No effective compression)\n");
        }
        printf("\n");

        // Decompress the data
        int decompressed_size = byte_decompress(data, compressed_size, decompressed_data);

        // Print decompressed data and size
        print_array(decompressed_data, decompressed_size, "Decompressed Data");
        printf("Decompressed Size: %d bytes\n", decompressed_size);

    }

    return 0;
}
