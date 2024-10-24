#include "compression.h"
#include <stdio.h>
#include <string.h>

// Decompression for RLE compressed data
int rle_decompress(unsigned char* src, int src_size, unsigned char* dst) {
    int dst_index = 0;

    for (int i = 0; i < src_size; ) {
        unsigned char byte = src[i++];
        unsigned char count = src[i++];

        // Repeat the byte 'count' number of times in the destination buffer
        for (int j = 0; j < count; j++) {
            dst[dst_index++] = byte;
        }
    }

    return dst_index;  // Return the size of the decompressed data
}

// General decompression function for both block-based and full array compression
int byte_decompress(unsigned char* data_ptr, int compressed_size, unsigned char* decompressed_data) {
    int decompressed_index = 0;
    int i = 0;
    int contains_flag = 0;
    for (int k = 0; k < compressed_size; k++) {
        if (data_ptr[k] == COMPRESSED_FLAG || data_ptr[k] == UNCOMPRESSED_FLAG) {
            contains_flag = 1;
            break;
        }
    }

    if (!contains_flag) {
        printf("Data is already uncompressed, returning original size and keeping data as is.\n");
        memcpy(decompressed_data, data_ptr, compressed_size);
        return compressed_size;  // Return the original size if no compression flags are found
    }
    while (i < compressed_size) {
        unsigned char flag = data_ptr[i++];  // Read the magic number (COMPRESSED_FLAG or UNCOMPRESSED_FLAG)

        if (flag == COMPRESSED_FLAG) {
            // Decompress a compressed block or full array
            int start_index = i;
            while (i < compressed_size && data_ptr[i] != COMPRESSED_FLAG && data_ptr[i] != UNCOMPRESSED_FLAG) {
                i++;  // Move to next flag or the end of the array
            }
            int block_size = i - start_index;
            decompressed_index += rle_decompress(&data_ptr[start_index], block_size, &decompressed_data[decompressed_index]);
        } else if (flag == UNCOMPRESSED_FLAG) {
            // Copy the next block_size (BLOCK_SIZE) bytes for uncompressed data
            for (int j = 0; j < BLOCK_SIZE && i < compressed_size && data_ptr[i] != COMPRESSED_FLAG && data_ptr[i] != UNCOMPRESSED_FLAG; j++) {
                decompressed_data[decompressed_index++] = data_ptr[i++];
            }
        }
    }
    printf("Data was decompressed successfully\n");

    return decompressed_index;  // Return the size of the decompressed data
}
