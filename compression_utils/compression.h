#ifndef COMPRESSION_H
#define COMPRESSION_H

// Define flags for compressed and uncompressed blocks
#define COMPRESSED_FLAG 0xFF
#define UNCOMPRESSED_FLAG 0xF0

// Define block size for block-based compression
#define BLOCK_SIZE 8

// Function declarations
int byte_compress(unsigned char* data_ptr, int data_size);
int byte_decompress(unsigned char* data_ptr, int compressed_size, unsigned char* decompressed_data);

#endif
