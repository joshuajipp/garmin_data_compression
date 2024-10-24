#include "compression.h"
#include <stdio.h>
#include <string.h>

// Helper function for RLE compression over a block
int rle_compress(unsigned char* src, int src_size, unsigned char* dst) {
    int dst_index = 0;

    for (int i = 0; i < src_size; ) {
        unsigned char current_byte = src[i];
        int count = 1;

        // Count how many times the current byte repeats
        while (i + count < src_size && src[i + count] == current_byte && count < 127) {
            count++;
        }

        // Store the byte and its repetition count
        dst[dst_index++] = current_byte;
        dst[dst_index++] = count;

        // Move the index forward by the count
        i += count;
    }

    return dst_index;  // Return the size of the compressed data
}

// Full array compression with RLE
int compress_full_array(unsigned char* data_ptr, int data_size, unsigned char* compressed_data) {
    int compressed_index = 1;  // Leave space for the magic number at index 0

    // Perform RLE compression on the entire array
    compressed_index += rle_compress(data_ptr, data_size, &compressed_data[compressed_index]);

    // Add the magic number for compressed data
    compressed_data[0] = COMPRESSED_FLAG;
    return compressed_index;  // Return size of compressed data
}

// Block-based compression using RLE on each block
int compress_blocks(unsigned char* data_ptr, int data_size, unsigned char* compressed_data, int block_size) {
    int compressed_index = 0;

    // Loop over each block
    for (int i = 0; i < data_size; i += block_size) {
        int remaining_size = data_size - i;
        int current_block_size = remaining_size < block_size ? remaining_size : block_size;

        // Attempt to compress the current block
        unsigned char block_compressed[block_size * 2];
        int block_compressed_size = rle_compress(&data_ptr[i], current_block_size, block_compressed);

        // Compare compressed block size with original block size
        if (block_compressed_size < current_block_size) {
            // Add a compressed block flag
            compressed_data[compressed_index++] = COMPRESSED_FLAG;
            // Copy compressed block into final buffer
            for (int k = 0; k < block_compressed_size; k++) {
                compressed_data[compressed_index++] = block_compressed[k];
            }
        } else {
            // Add an uncompressed block flag
            compressed_data[compressed_index++] = UNCOMPRESSED_FLAG;
            // Copy original block into final buffer
            for (int k = 0; k < current_block_size; k++) {
                compressed_data[compressed_index++] = data_ptr[i + k];
            }
        }
    }

    return compressed_index;  // Return size of compressed data
}

// Main compression function to decide the best method
int byte_compress(unsigned char* data_ptr, int data_size) {
    if (data_size == 0) return 0;

    // Buffer to hold the fully compressed data
    unsigned char full_compressed[data_size * 2];  // Max possible size
    int full_compressed_size = compress_full_array(data_ptr, data_size, full_compressed);

    // Buffer to hold block compressed data
    unsigned char block_compressed[data_size * 2];  // Max possible size
    int block_compressed_size = compress_blocks(data_ptr, data_size, block_compressed, BLOCK_SIZE);

    // Compare the compressed sizes
    if (full_compressed_size < data_size && full_compressed_size <= block_compressed_size) {
        // Full array compression gives the smallest size, copy it to original buffer
        memcpy(data_ptr, full_compressed, full_compressed_size);
        printf("Data was compressed using full array RLE compression.\n");
        return full_compressed_size;
    } else if (block_compressed_size < data_size) {
        // Block-based compression gives the smallest size, copy it to original buffer
        memcpy(data_ptr, block_compressed, block_compressed_size);
        printf("Data was compressed using block-based RLE compression.\n");
        return block_compressed_size;
    } else {
        // Neither compression reduced the size, return the original data
        printf("Data was not compressed as it would increase the size.\n");
        return data_size;
    }
}
