cmake_minimum_required(VERSION 3.21)

project(accurate_kosinski LANGUAGES C)

add_executable(compare "main_compare.c" "lib/kosinski_decompress.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinski_compress "main_compress.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinski_compress_asm "main_compress_asm.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinski_decompress "main_decompress.c" "lib/kosinski_decompress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinski_compress_d "main_compress.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinski_decompress_d "main_decompress.c" "lib/kosinski_decompress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinskim_compress_d "main_moduled_compress.c" "lib/kosinski_moduled_compress.c" "lib/kosinski_decompress.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinskim_decompress_d "main_moduled_decompress.c" "lib/kosinski_moduled_decompress.c" "lib/kosinski_decompress.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
add_executable(kosinskim_compare "main_moduled_compare.c" "lib/kosinski_moduled_compress.c" "lib/kosinski_decompress.c" "lib/kosinski_moduled_decompress.c" "lib/kosinski_compress.c" "load_file_to_buffer.c" "lib/memory_stream.c")
