cmake_minimum_required(VERSION 3.21)

project(accurate_kosinski LANGUAGES C)

add_executable(compare
	"main_compare.c"
	"lib/kosinski_decompress.c"
	"lib/kosinski_decompress.h"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinski_compress
	"main_compress.c"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinski_compress_asm
	"main_compress_asm.c"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinski_decompress
	"main_decompress.c"
	"lib/kosinski_decompress.c"
	"lib/kosinski_decompress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinski_compress_d
	"main_compress.c"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinski_decompress_d
	"main_decompress.c"
	"lib/kosinski_decompress.c"
	"lib/kosinski_decompress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinskim_compress_d
	"main_moduled_compress.c"
	"lib/kosinski_moduled_compress.c"
	"lib/kosinski_moduled_compress.h"
	"lib/kosinski_decompress.c"
	"lib/kosinski_decompress.h"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinskim_decompress_d
	"main_moduled_decompress.c"
	"lib/kosinski_moduled_decompress.c"
	"lib/kosinski_moduled_decompress.h"
	"lib/kosinski_decompress.c"
	"lib/kosinski_decompress.h"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)
add_executable(kosinskim_compare
	"main_moduled_compare.c"
	"lib/kosinski_moduled_compress.c"
	"lib/kosinski_moduled_compress.h"
	"lib/kosinski_decompress.c"
	"lib/kosinski_decompress.h"
	"lib/kosinski_moduled_decompress.c"
	"lib/kosinski_moduled_decompress.h"
	"lib/kosinski_compress.c"
	"lib/kosinski_compress.h"
	"load_file_to_buffer.c"
	"load_file_to_buffer.h"
	"lib/memory_stream.c"
	"lib/memory_stream.h"
	"lib/clowncommon.h"
)

target_compile_definitions(kosinski_compress_d PRIVATE DEBUG)
target_compile_definitions(kosinski_decompress_d PRIVATE DEBUG)
target_compile_definitions(kosinskim_compress_d PRIVATE DEBUG)
target_compile_definitions(kosinskim_decompress_d PRIVATE DEBUG)


#########
# Tests #
#########

enable_testing()

add_test(NAME test_sonic1_artkos_flowers COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/artkos/Flowers at Ending.bin")
add_test(NAME test_sonic1_map256_ghz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/GHZ.bin")
add_test(NAME test_sonic1_map256_lz      COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/LZ.bin")
add_test(NAME test_sonic1_map256_mz      COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/MZ.bin")
add_test(NAME test_sonic1_map256_mz_jp1  COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/MZ (JP1).bin")
add_test(NAME test_sonic1_map256_sbz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/SBZ.bin")
add_test(NAME test_sonic1_map256_sbz_jp1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/SBZ (JP1).bin")
add_test(NAME test_sonic1_map256_slz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/SLZ.bin")
add_test(NAME test_sonic1_map256_syz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/map256/SYZ.bin")
add_test(NAME test_sonic1_sound_z80      COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 1/sound/z80.bin")

add_test(NAME test_sonic2_art_kosinski_arz       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/ARZ.bin")
add_test(NAME test_sonic2_art_kosinski_cnz       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/CNZ.bin")
add_test(NAME test_sonic2_art_kosinski_cpz_dez   COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/CPZ_DEZ.bin")
add_test(NAME test_sonic2_art_kosinski_ehz_htz   COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/EHZ_HTZ.bin")
add_test(NAME test_sonic2_art_kosinski_htz_supp  COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/HTZ_Supp.bin")
add_test(NAME test_sonic2_art_kosinski_mcz       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/MCZ.bin")
add_test(NAME test_sonic2_art_kosinski_mtz       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/MTZ.bin")
add_test(NAME test_sonic2_art_kosinski_ooz       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/OOZ.bin")
add_test(NAME test_sonic2_art_kosinski_spec_stag COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/SpecStag.bin")
add_test(NAME test_sonic2_art_kosinski_wfz_scz   COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/WFZ_SCZ.bin")
add_test(NAME test_sonic2_art_kosinski_wfz_supp  COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/art/kosinski/WFZ_Supp.bin")

add_test(NAME test_sonic2_collision_arz_primary       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/ARZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_arz_secondary     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/ARZ secondary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_cnz_primary       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/CNZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_cnz_secondary     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/CNZ secondary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_cpz_dez_primary   COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/CPZ and DEZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_cpz_dez_secondary COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/CPZ and DEZ secondary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_ehz_htz_primary   COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/EHZ and HTZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_ehz_htz_secondary COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/EHZ and HTZ secondary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_mcz_primary       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/MCZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_mtz_primary       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/MTZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_ooz_primary       COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/OOZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_wfz_scz_primary   COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/WFZ and SCZ primary 16x16 collision index.bin")
add_test(NAME test_sonic2_collision_wfz_scz_secondary COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/collision/WFZ and SCZ secondary 16x16 collision index.bin")

add_test(NAME test_sonic2_level_layout_arz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/ARZ_1.bin")
add_test(NAME test_sonic2_level_layout_arz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/ARZ_2.bin")
add_test(NAME test_sonic2_level_layout_cnz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/CNZ_1.bin")
add_test(NAME test_sonic2_level_layout_cnz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/CNZ_2.bin")
add_test(NAME test_sonic2_level_layout_cpz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/CPZ_1.bin")
add_test(NAME test_sonic2_level_layout_cpz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/CPZ_2.bin")
add_test(NAME test_sonic2_level_layout_dez  COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/DEZ.bin")
add_test(NAME test_sonic2_level_layout_ehz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/EHZ_1.bin")
add_test(NAME test_sonic2_level_layout_ehz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/EHZ_2.bin")
add_test(NAME test_sonic2_level_layout_htz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/HTZ_1.bin")
add_test(NAME test_sonic2_level_layout_htz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/HTZ_2.bin")
add_test(NAME test_sonic2_level_layout_mcz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/MCZ_1.bin")
add_test(NAME test_sonic2_level_layout_mcz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/MCZ_2.bin")
add_test(NAME test_sonic2_level_layout_mtz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/MTZ_1.bin")
add_test(NAME test_sonic2_level_layout_mtz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/MTZ_2.bin")
add_test(NAME test_sonic2_level_layout_mtz3 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/MTZ_3.bin")
add_test(NAME test_sonic2_level_layout_ooz1 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/OOZ_1.bin")
add_test(NAME test_sonic2_level_layout_ooz2 COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/OOZ_2.bin")
add_test(NAME test_sonic2_level_layout_scz  COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/SCZ.bin")
add_test(NAME test_sonic2_level_layout_wfz  COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/level/layout/WFZ.bin")

add_test(NAME test_sonic2_mappings_16x16_arz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/ARZ.bin")
add_test(NAME test_sonic2_mappings_16x16_cnz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/CNZ.bin")
add_test(NAME test_sonic2_mappings_16x16_cpz_dez COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/CPZ_DEZ.bin")
add_test(NAME test_sonic2_mappings_16x16_ehz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/EHZ.bin")
add_test(NAME test_sonic2_mappings_16x16_htz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/HTZ.bin")
add_test(NAME test_sonic2_mappings_16x16_mcz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/MCZ.bin")
add_test(NAME test_sonic2_mappings_16x16_mtz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/MTZ.bin")
add_test(NAME test_sonic2_mappings_16x16_ooz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/OOZ.bin")
add_test(NAME test_sonic2_mappings_16x16_wfz_scz COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/16x16/WFZ_SCZ.bin")

add_test(NAME test_sonic2_mappings_128x128_arz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/ARZ.bin")
add_test(NAME test_sonic2_mappings_128x128_cnz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/CNZ.bin")
add_test(NAME test_sonic2_mappings_128x128_cpz_dez COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/CPZ_DEZ.bin")
add_test(NAME test_sonic2_mappings_128x128_ehz_htz COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/EHZ_HTZ.bin")
add_test(NAME test_sonic2_mappings_128x128_mcz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/MCZ.bin")
add_test(NAME test_sonic2_mappings_128x128_mtz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/MTZ.bin")
add_test(NAME test_sonic2_mappings_128x128_ooz     COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/OOZ.bin")
add_test(NAME test_sonic2_mappings_128x128_wfz_scz COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/mappings/128x128/WFZ_SCZ.bin")

add_test(NAME test_sonic2_misc_special_stage_location    COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/misc/Special stage object location lists (Kosinski compression).bin")
add_test(NAME test_sonic2_misc_special_stage_perspective COMMAND compare "${CMAKE_CURRENT_SOURCE_DIR}/test/Sonic 2/misc/Special stage object perspective data (Kosinski compression).bin")
