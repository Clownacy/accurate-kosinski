cmake_minimum_required(VERSION 3.21)

project(accurate-kosinski LANGUAGES C)


#############
# Libraries #
#############

add_library(accurate-kosinski-clowncommon INTERFACE) # Namespaced to prevent collision with other projects that may create their own clowncommon target.
target_include_directories(accurate-kosinski-clowncommon INTERFACE "lib/clowncommon")

add_library(memory-stream STATIC
	"memory-stream.c"
	"memory-stream.h"
)
set_target_properties(memory-stream PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(memory-stream PUBLIC accurate-kosinski-clowncommon)

add_library(load-file-to-buffer STATIC
	"load-file-to-buffer.c"
	"load-file-to-buffer.h"
)
set_target_properties(load-file-to-buffer PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(load-file-to-buffer PUBLIC accurate-kosinski-clowncommon)

add_library(kosinski-compressor STATIC
	"lib/kosinski-compress.c"
	"lib/kosinski-compress.h"
)
set_target_properties(kosinski-compressor PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-compressor PUBLIC accurate-kosinski-clowncommon)

add_library(kosinski-decompressor STATIC
	"lib/kosinski-decompress.c"
	"lib/kosinski-decompress.h"
)
set_target_properties(kosinski-decompressor PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-decompressor PUBLIC accurate-kosinski-clowncommon)

add_library(kosinskim-compressor STATIC
	"lib/kosinski-moduled-compress.c"
	"lib/kosinski-moduled-compress.h"
)
set_target_properties(kosinskim-compressor PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-compressor PUBLIC kosinski-compressor)

add_library(kosinskim-decompressor STATIC
	"lib/kosinski-moduled-decompress.c"
	"lib/kosinski-moduled-decompress.h"
)
set_target_properties(kosinskim-decompressor PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-decompressor PUBLIC kosinski-decompressor)


#########
# Tools #
#########

# Compressors
add_executable(kosinski-compress
	"main-compress.c"
)
set_target_properties(kosinski-compress PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-compress PRIVATE kosinski-compressor)

add_executable(kosinski-compress-d
	"main-compress.c"
)
set_target_properties(kosinski-compress-d PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-compress-d PRIVATE kosinski-compressor)
target_compile_definitions(kosinski-compress-d PRIVATE DEBUG)

add_executable(kosinski-compress-asm
	"main-compress-asm.c"
)
set_target_properties(kosinski-compress-asm PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-compress-asm PRIVATE memory-stream kosinski-compressor)

add_executable(kosinskim-compress
	"main-moduled-compress.c"
)
set_target_properties(kosinskim-compress PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-compress PRIVATE kosinskim-compressor)

add_executable(kosinskim-compress-d
	"main-moduled-compress.c"
)
set_target_properties(kosinskim-compress-d PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-compress-d PRIVATE kosinskim-compressor)
target_compile_definitions(kosinskim-compress-d PRIVATE DEBUG)

# Decompressors
add_executable(kosinski-decompress
	"main-decompress.c"
)
set_target_properties(kosinski-decompress PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-decompress PRIVATE kosinski-decompressor)

add_executable(kosinski-decompress-d
	"main-decompress.c"
)
set_target_properties(kosinski-decompress-d PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-decompress-d PRIVATE kosinski-decompressor)
target_compile_definitions(kosinski-decompress-d PRIVATE DEBUG)

add_executable(kosinskim-decompress
	"main-moduled-decompress.c"
)
set_target_properties(kosinskim-decompress PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-decompress PRIVATE kosinskim-decompressor)

add_executable(kosinskim-decompress-d
	"main-moduled-decompress.c"
)
set_target_properties(kosinskim-decompress-d PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-decompress-d PRIVATE kosinskim-decompressor)
target_compile_definitions(kosinskim-decompress-d PRIVATE DEBUG)

# Comparers
add_executable(kosinski-compare
	"main-compare.c"
)
set_target_properties(kosinski-compare PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinski-compare PRIVATE load-file-to-buffer memory-stream kosinski-compressor kosinski-decompressor)


add_executable(kosinskim-compare
	"main-moduled-compare.c"
)
set_target_properties(kosinskim-compare PROPERTIES
	C_STANDARD 90
	C_EXTENSIONS OFF
)
target_link_libraries(kosinskim-compare PRIVATE load-file-to-buffer memory-stream kosinskim-compressor kosinskim-decompressor)


#########
# Tests #
#########

enable_testing()

# Kosinski files
#add_test(NAME test_kosinski_Mega_CD_BIOS_SUB_CPU_BIOS_Europe COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Mega CD BIOS/SUB-CPU BIOS (Europe).bin")
#add_test(NAME test_kosinski_Mega_CD_BIOS_SUB_CPU_BIOS_Japan COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Mega CD BIOS/SUB-CPU BIOS (Japan).bin")
#add_test(NAME test_kosinski_Mega_CD_BIOS_SUB_CPU_BIOS_USA COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Mega CD BIOS/SUB-CPU BIOS (USA).bin")
# The above three tests fail because the files appear to have been compressed with a different compressor to the others.
add_test(NAME test_kosinski_Sonic_1_artkos_Flowers_at_Ending COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/artkos/Flowers at Ending.bin")
add_test(NAME test_kosinski_Sonic_1_map256_GHZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/GHZ.bin")
add_test(NAME test_kosinski_Sonic_1_map256_LZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/LZ.bin")
add_test(NAME test_kosinski_Sonic_1_map256_MZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/MZ.bin")
add_test(NAME test_kosinski_Sonic_1_map256_MZ_JP1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/MZ (JP1).bin")
add_test(NAME test_kosinski_Sonic_1_map256_SBZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/SBZ.bin")
add_test(NAME test_kosinski_Sonic_1_map256_SBZ_JP1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/SBZ (JP1).bin")
add_test(NAME test_kosinski_Sonic_1_map256_SLZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/SLZ.bin")
add_test(NAME test_kosinski_Sonic_1_map256_SYZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/map256/SYZ.bin")
add_test(NAME test_kosinski_Sonic_1_sound_z80 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 1/sound/z80.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_ARZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/ARZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_CNZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/CNZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_CPZ_DEZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/CPZ_DEZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_EHZ_HTZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/EHZ_HTZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_HTZ_Supp COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/HTZ_Supp.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_MCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/MCZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_MTZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/MTZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_OOZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/OOZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_SpecStag COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/SpecStag.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_WFZ_SCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/WFZ_SCZ.bin")
add_test(NAME test_kosinski_Sonic_2_art_kosinski_WFZ_Supp COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/art/kosinski/WFZ_Supp.bin")
add_test(NAME test_kosinski_Sonic_2_collision_ARZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/ARZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_ARZ_secondary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/ARZ secondary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_CNZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/CNZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_CNZ_secondary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/CNZ secondary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_CPZ_and_DEZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/CPZ and DEZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_CPZ_and_DEZ_secondary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/CPZ and DEZ secondary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_EHZ_and_HTZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/EHZ and HTZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_EHZ_and_HTZ_secondary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/EHZ and HTZ secondary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_MCZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/MCZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_MTZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/MTZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_OOZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/OOZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_WFZ_and_SCZ_primary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/WFZ and SCZ primary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_collision_WFZ_and_SCZ_secondary_16x16_collision_index COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/collision/WFZ and SCZ secondary 16x16 collision index.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_ARZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/ARZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_ARZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/ARZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_CNZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/CNZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_CNZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/CNZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_CPZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/CPZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_CPZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/CPZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_DEZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/DEZ.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_EHZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/EHZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_EHZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/EHZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_HTZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/HTZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_HTZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/HTZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_MCZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/MCZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_MCZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/MCZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_MTZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/MTZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_MTZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/MTZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_MTZ_3 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/MTZ_3.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_OOZ_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/OOZ_1.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_OOZ_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/OOZ_2.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_SCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/SCZ.bin")
add_test(NAME test_kosinski_Sonic_2_level_layout_WFZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/level/layout/WFZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_ARZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/ARZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_CNZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/CNZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_CPZ_DEZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/CPZ_DEZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_EHZ_HTZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/EHZ_HTZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_MCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/MCZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_MTZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/MTZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_OOZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/OOZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_128x128_WFZ_SCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/128x128/WFZ_SCZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_ARZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/ARZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_CNZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/CNZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_CPZ_DEZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/CPZ_DEZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_EHZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/EHZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_HTZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/HTZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_MCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/MCZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_MTZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/MTZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_OOZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/OOZ.bin")
add_test(NAME test_kosinski_Sonic_2_mappings_16x16_WFZ_SCZ COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/mappings/16x16/WFZ_SCZ.bin")
add_test(NAME test_kosinski_Sonic_2_misc_Special_stage_object_location_lists_Kosinski_compression COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/misc/Special stage object location lists (Kosinski compression).bin")
add_test(NAME test_kosinski_Sonic_2_misc_Special_stage_object_perspective_data_Kosinski_compression COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 2/misc/Special stage object perspective data (Kosinski compression).bin")
add_test(NAME test_kosinski_Sonic_3K_General_Blue_Sphere_Kosinski_Art_Characters COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Blue Sphere/Kosinski Art/Characters.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Blue_Sphere_Kosinski_Art_Character_Sprites COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Blue Sphere/Kosinski Art/Character Sprites.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Blue_Sphere_Kosinski_Art_Normal_Text COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Blue Sphere/Kosinski Art/Normal Text.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Competition_Menu_Kosinski_Art_Level_Select COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Competition Menu/Kosinski Art/Level Select.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Competition_Menu_Kosinski_Art_Mode_Select COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Competition Menu/Kosinski Art/Mode Select.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Competition_Menu_Kosinski_Art_Player_Sprites COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Competition Menu/Kosinski Art/Player Sprites.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Competition_Menu_Kosinski_Art_Results COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Competition Menu/Kosinski Art/Results.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Ending_Kosinski_Art_Robotnik_Standing_Smug COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Ending/Kosinski Art/Robotnik Standing Smug.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Ending_Kosinski_Art_Sonic_Knuckles_Pose_Banner COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Ending/Kosinski Art/Sonic Knuckles Pose Banner.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Save_Menu_Kosinski_Art_Menu_BG COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Save Menu/Kosinski Art/Menu BG.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Save_Menu_Kosinski_Art_Misc COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Save Menu/Kosinski Art/Misc.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Save_Menu_Kosinski_Art_Portraits COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Save Menu/Kosinski Art/Portraits.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Save_Menu_Kosinski_Art_SK_Extra COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Save Menu/Kosinski Art/SK Extra.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Save_Menu_Kosinski_Art_SK_Zone_Art COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Save Menu/Kosinski Art/SK Zone Art.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Save_Menu_Kosinski_Art_Zone_Art COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Save Menu/Kosinski Art/Zone Art.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Kosinski_Art_S3_SSResults_General COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Kosinski Art/S3 SSResults General.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Kosinski_Art_S3_SSResults_Tails_Knuckles_Icons COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Kosinski Art/S3 SSResults Tails Knuckles Icons.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Kosinski_Art_SSResults_General COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Kosinski Art/SSResults General.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Kosinski_Art_SSResults_Tails_Knuckles_Icons COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Kosinski Art/SSResults Tails Knuckles Icons.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Layout_Perspective_Maps COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Layout/Perspective Maps.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Layout_SK_Set_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Layout/SK Set 1.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Special_Stage_Layout_SK_Set_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Special Stage/Layout/SK Set 2.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic 1.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_8 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic 8.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_9 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic 9.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_A COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic A.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_B COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic B.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_C COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic C.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_S3_Sonic_D COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/S3 Sonic D.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_ANDKnuckles_Subtitle COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK ANDKnuckles Subtitle.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_Big_SEGA COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK Big SEGA.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_Death_Egg COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK Death Egg.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_Mountain_Sprite COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK Mountain Sprite.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_Screen_Background COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK Screen Background.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_Sonic_and_Knuckles_Hands COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK Sonic and Knuckles Hands.bin")
add_test(NAME test_kosinski_Sonic_3K_General_Title_Kosinski_Art_SK_Sonic_Falling COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/General/Title/Kosinski Art/SK Sonic Falling.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Blocks_Act_1_Main_Level COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Blocks/Act 1 Main Level.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Blocks_Act_1_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Blocks/Act 1 Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Blocks_Act_2_BomberShip COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Blocks/Act 2 BomberShip.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Blocks_Act_2_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Blocks/Act 2 Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Chunks_Act_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Chunks/Act 1.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_AIZ_Chunks_Act_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/AIZ/Chunks/Act 2.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ALZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ALZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ALZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ALZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_BPZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/BPZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_BPZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/BPZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_CGZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/CGZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_CGZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/CGZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_CNZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/CNZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_CNZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/CNZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DDZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DDZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DDZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DDZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DEZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DEZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DEZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DEZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DEZ_Blocks_Act_3 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DEZ/Blocks/Act 3.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DEZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DEZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DEZ_Chunks_Act_3 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DEZ/Chunks/Act 3.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DEZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DEZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DPZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DPZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_DPZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/DPZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_EMZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/EMZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_EMZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/EMZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_FBZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/FBZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_FBZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/FBZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Gumball_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Gumball/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Gumball_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Gumball/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HCZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HCZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HCZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HCZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HCZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HCZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HCZ_Chunks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HCZ/Chunks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HCZ_Chunks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HCZ/Chunks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HCZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HCZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HPZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HPZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HPZ_Blocks_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HPZ/Blocks/Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HPZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HPZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HPZ_Chunks_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HPZ/Chunks/Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_HPZ_Kosinski_Art_Master_Emerald COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/HPZ/Kosinski Art/Master Emerald.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ICZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ICZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ICZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ICZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ICZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ICZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ICZ_Chunks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ICZ/Chunks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ICZ_Chunks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ICZ/Chunks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_ICZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/ICZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Blocks_Act_2_Death_Egg COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Blocks/Act 2 Death Egg.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Chunks_Act_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Chunks/Act 1.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Chunks_Act_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Chunks/Act 2.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Chunks_Act_2_Death_Egg COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Chunks/Act 2 Death Egg.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LBZ_Chunks_Unknown_Unused COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LBZ/Chunks/Unknown Unused.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Blocks_Act_3_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Blocks/Act 3 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Chunks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Chunks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Chunks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Chunks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Chunks_Act_3_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Chunks/Act 3 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_LRZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/LRZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MGZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MGZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MGZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MGZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MGZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MGZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MGZ_Chunks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MGZ/Chunks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MGZ_Chunks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MGZ/Chunks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MGZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MGZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MHZ_Blocks_Custom COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MHZ/Blocks/Custom.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MHZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MHZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MHZ_Blocks_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MHZ/Blocks/Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MHZ_Chunks_Custom COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MHZ/Chunks/Custom.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MHZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MHZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_MHZ_Chunks_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/MHZ/Chunks/Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Pachinko_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Pachinko/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Pachinko_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Pachinko/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Pachinko_Kosinski_Art_Animated_BG_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Pachinko/Kosinski Art/Animated BG 1.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Pachinko_Kosinski_Art_Animated_BG_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Pachinko/Kosinski Art/Animated BG 2.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Slots_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Slots/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_Slots_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/Slots/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Blocks_Act_1_Custom COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Blocks/Act 1 Custom.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Blocks_Act_2_Custom COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Blocks/Act 2 Custom.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Blocks_Act_2_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Blocks/Act 2 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Blocks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Blocks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Chunks_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Chunks/Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Chunks_Unused2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Chunks/Unused2.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SOZ_Chunks_Unused COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SOZ/Chunks/Unused.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Blocks_Act_1_Custom COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Blocks/Act 1 Custom.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Blocks_Act_1_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Blocks/Act 1 Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Blocks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Blocks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Blocks_Act_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Blocks/Act 2.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Chunks_Act_1_Custom COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Chunks/Act 1 Custom.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Chunks_Act_1_Primary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Chunks/Act 1 Primary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Chunks_Act_1_Secondary COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Chunks/Act 1 Secondary.bin")
add_test(NAME test_kosinski_Sonic_3K_Levels_SSZ_Chunks_Act_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/Levels/SSZ/Chunks/Act 2.bin")
add_test(NAME test_kosinski_Sonic_3K_sound_driver_part_1 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/sound driver part 1.bin")
add_test(NAME test_kosinski_Sonic_3K_sound_driver_part_2 COMMAND kosinski-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/kosinski/Sonic 3 & Knuckles/sound driver part 2.bin")

# Moduled Kosinski files
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Egg_Robo COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Egg Robo.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Island_Lift_Animated_Tiles COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Island Lift Animated Tiles.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Normal_Sonic_Ending_Pose COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Normal Sonic Ending Pose.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Pelican_and_Dolphin COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Pelican and Dolphin.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Sonic_3_Pose_Banner COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Sonic 3 Pose Banner.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Sonic_3_Sonic_Ending_Pose COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Sonic 3 Sonic Ending Pose.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Sonic_3_Super_Sonic_Ending_Pose COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Sonic 3 Super Sonic Ending Pose.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Sonic_Plane COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Sonic Plane.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Sonic_Plane_Ending COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Sonic Plane Ending.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Super_Sonic_Ending_Pose COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Super Sonic Ending Pose.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Ending_KosinskiM_Art_Tails_Ending_Pose COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Ending/KosinskiM Art/Tails Ending Pose.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Special_Stage_KosinskiM_Art_Special_Stage_Chaos_Emerald COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Special Stage/KosinskiM Art/Special Stage Chaos Emerald.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Special_Stage_KosinskiM_Art_Special_Stage_Super_Emerald COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Special Stage/KosinskiM Art/Special Stage Super Emerald.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Special_Stage_KosinskiM_Art_SSResults_HYPER COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Special Stage/KosinskiM Art/SSResults HYPER.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Special_Stage_KosinskiM_Art_SSResults_HYPER_K COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Special Stage/KosinskiM Art/SSResults HYPER (K).bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Special_Stage_KosinskiM_Art_SSResults_SUPER COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Special Stage/KosinskiM Art/SSResults SUPER.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Special_Stage_KosinskiM_Art_SSResults_SUPER_K COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Special Stage/KosinskiM Art/SSResults SUPER (K).bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Batbot_Batbot COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Batbot/Batbot.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Blaster_Blaster COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Blaster/Blaster.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Blastoid_Blastoid COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Blastoid/Blastoid.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Caterkiller_Jr_Caterkiller_Jr COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Caterkiller Jr/Caterkiller Jr.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Chainspike_Chainspike COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Chainspike/Chainspike.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Clamer_Clamer_Shot COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Clamer/Clamer Shot.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Cluckoid_Cluckoid_Arrow COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Cluckoid/Cluckoid Arrow.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Corkey_Corkey COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Corkey/Corkey.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Dragonfly_Dragonfly COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Dragonfly/Dragonfly.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Egg_Robo_Badnik_Egg_Robo_Badnik COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Egg Robo Badnik/Egg Robo Badnik.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Fireworm_Fireworm_Segments COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Fireworm/Fireworm Segments.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Iwamodoki_Iwamodoki COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Iwamodoki/Iwamodoki.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Jawz_Jawz COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Jawz/Jawz.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Knuckles_Cutscene_Knuckles_Ending_Cutscene COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Knuckles/Cutscene/Knuckles Ending Cutscene.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Knuckles_Cutscene_Knuckles_Intro_Bomb COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Knuckles/Cutscene/Knuckles Intro Bomb.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Knuckles_Cutscene_Knuckles_MHZ_Switch COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Knuckles/Cutscene/Knuckles MHZ Switch.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Knuckles_Cutscene_Knuckles_Peering COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Knuckles/Cutscene/Knuckles Peering.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Madmole_Madmole COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Madmole/Madmole.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Mantis_Mantis COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Mantis/Mantis.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Master_Emerald_Master_Emerald COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Master Emerald/Master Emerald.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Mecha_Sonic_Mecha_Sonic_Extra_Art COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Mecha Sonic/Mecha Sonic Extra Art.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Mecha_Sonic_Mecha_Sonic_Head COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Mecha Sonic/Mecha Sonic Head.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Mega_Chopper_Mega_Chopper COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Mega Chopper/Mega Chopper.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Monkey_Dude_Monkey_Dude COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Monkey Dude/Monkey Dude.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Mushmeanie_Mushmeanie COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Mushmeanie/Mushmeanie.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Orbinaut_Orbinaut COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Orbinaut/Orbinaut.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Pointdexter_Pointdexter COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Pointdexter/Pointdexter.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Results_Results_General COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Results/Results General.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Results_Results_KNUCKLES COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Results/Results KNUCKLES.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Results_Results_MILES COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Results/Results MILES.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Results_Results_SONIC COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Results/Results SONIC.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Results_Results_SONIC_S3 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Results/Results SONIC S3.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Results_Results_TAILS COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Results/Results TAILS.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Ribot_Ribot COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Ribot/Ribot.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Robotnik_Robotnik_Ship_Crane COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Robotnik/Robotnik Ship Crane.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Rockn_Rockn COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Rockn/Rock'n.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Sandworm_Sandworm COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Sandworm/Sandworm.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Skorp_Skorp COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Skorp/Skorp.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Snail_Blaster_Snail_Blaster COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Snail Blaster/Snail Blaster.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Sonic_Art_Hyper_Sonic_Stars COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Sonic/Art/Hyper Sonic Stars.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Sparkle_Sparkle COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Sparkle/Sparkle.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Spikebonker_Spikebonker COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Spikebonker/Spikebonker.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Spiker_Spiker COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Spiker/Spiker.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_SS_Entry_Badnik_Explosion COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/SS Entry/Badnik Explosion.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Star_Pointer_Star_Pointer COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Star Pointer/Star Pointer.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Starpost_Starpost_Stars_1 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Starpost/Starpost Stars 1.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Starpost_Starpost_Stars_2 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Starpost/Starpost Stars 2.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Starpost_Starpost_Stars_3 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Starpost/Starpost Stars 3.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Tails_Art_Super_Tails_birds COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Tails/Art/Super Tails birds.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Technosqueek_Technosqueek COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Technosqueek/Technosqueek.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Teleporter_Teleporter COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Teleporter/Teleporter.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Title_Card_Bonus_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Title Card/Bonus Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Title_Card_Title_Card_Number_1 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Title Card/Title Card Number 1.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Title_Card_Title_Card_Number_2 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Title Card/Title Card Number 2.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Title_Card_Title_Card_Red_ACT COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Title Card/Title Card Red ACT.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Title_Card_Title_Card_S3K_Zone COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Title Card/Title Card S3K Zone.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Title_Card_Title_Card_SK_Zone COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Title Card/Title Card SK Zone.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Toxomister_Toxomister COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Toxomister/Toxomister.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Tulipon_Tulipon COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Tulipon/Tulipon.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Sprites_Turbo_Spiker_Turbo_Spiker COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Sprites/Turbo Spiker/Turbo Spiker.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Title_KosinskiM_Art_SK_Banner COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Title/KosinskiM Art/SK Banner.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Title_KosinskiM_Art_SK_Menu COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Title/KosinskiM Art/SK Menu.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Title_KosinskiM_Art_SK_Sonic_Knuckles COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Title/KosinskiM Art/SK Sonic Knuckles.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_General_Title_KosinskiM_Art_SK_Sonic_Land_Frames COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/General/Title/KosinskiM Art/SK Sonic Land Frames.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_KosinskiM_Art_Intro_Emeralds COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/KosinskiM Art/Intro Emeralds.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_KosinskiM_Art_Intro_Plane COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/KosinskiM Art/Intro Plane.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_1_Fire_Overlay COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 1 Fire Overlay.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_1_Main_Level COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 1 Main Level.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_1_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 1 Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_2_Bombership_2 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 2 Bombership 2.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_2_Bombership COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 2 Bombership.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_2_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 2 Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_AIZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/AIZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ALZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ALZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ALZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ALZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_BPZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/BPZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_BPZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/BPZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_CGZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/CGZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_CGZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/CGZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_CNZ_KosinskiM_Art_Balloon COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/CNZ/KosinskiM Art/Balloon.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_CNZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/CNZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_CNZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/CNZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DDZ_KosinskiM_Art_Misc_Art COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DDZ/KosinskiM Art/Misc Art.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DDZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DDZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DDZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DDZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_KosinskiM_Art_Final_Boss_Debris COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/KosinskiM Art/Final Boss Debris.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_KosinskiM_Art_Final_Boss_Misc_Art COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/KosinskiM Art/Final Boss Misc Art.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_KosinskiM_Art_Miniboss_Misc_Art COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/KosinskiM Art/Miniboss Misc Art.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_Tiles_Final_Boss_Act COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/Tiles/Final Boss Act.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DEZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DEZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DPZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DPZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_DPZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/DPZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_EMZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/EMZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_EMZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/EMZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Before_Boss_Pillar COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Before Boss Pillar.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Button COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Button.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Cloud COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Cloud.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Exit_Door COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Exit Door.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Exit_Hall COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Exit Hall.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Miniboss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Miniboss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_FBZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/FBZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_Gumball_Tiles_Gumball COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/Gumball/Tiles/Gumball.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_KosinskiM_Art_Geyser_Horizontal COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/KosinskiM Art/Geyser Horizontal.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_KosinskiM_Art_Geyser_Vertical COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/KosinskiM Art/Geyser Vertical.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_KosinskiM_Art_Large_Fan COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/KosinskiM Art/Large Fan.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HCZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HCZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HPZ_KosinskiM_Art_Knuckles_Dizzy_Stars COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HPZ/KosinskiM Art/Knuckles Dizzy Stars.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HPZ_KosinskiM_Art_Special_Stage_Emeralds COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HPZ/KosinskiM Art/Special Stage Emeralds.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HPZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HPZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HPZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HPZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_HPZ_Tiles_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/HPZ/Tiles/Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ICZ_KosinskiM_Art_Snowdust COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ICZ/KosinskiM Art/Snowdust.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ICZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ICZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ICZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ICZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ICZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ICZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_ICZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/ICZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_KosinskiM_Art_Act_2_Death_Egg_Small COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/KosinskiM Art/Act 2 Death Egg Small.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_KosinskiM_Art_Final_Boss_2 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/KosinskiM Art/Final Boss 2.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_KosinskiM_Art_Miniboss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/KosinskiM Art/Miniboss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_KosinskiM_Art_Miniboss_Box COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/KosinskiM Art/Miniboss Box.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_Tiles_Act_2_Death_Egg_2 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/Tiles/Act 2 Death Egg 2.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_Tiles_Act_2_Death_Egg COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/Tiles/Act 2 Death Egg.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LBZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LBZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Autoscroll COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Autoscroll.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_BG_Death_Egg COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/BG Death Egg.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Death_Egg_Flash COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Death Egg Flash.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Knuckles_Boulder COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Knuckles Boulder.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Miniboss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Miniboss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Platform_Debris COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Platform Debris.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Rock_Crusher COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Rock Crusher.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_Tiles_Boss_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/Tiles/Boss Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_LRZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/LRZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_KosinskiM_Art_End_Boss_Debris COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/KosinskiM Art/End Boss Debris.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_KosinskiM_Art_Miniboss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/KosinskiM Art/Miniboss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MGZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MGZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_End_Boss_Pillar COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/End Boss Pillar.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_End_Boss_Spikes COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/End Boss Spikes.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_Miniboss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/Miniboss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_Miniboss_Log COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/Miniboss Log.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_Ship_Propeller COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/Ship Propeller.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_Tiles_Custom COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/Tiles/Custom.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_MHZ_Tiles_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/MHZ/Tiles/Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_Pachinko_Tiles_Pachinko COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/Pachinko/Tiles/Pachinko.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_Slots_Tiles_Slots COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/Slots/Tiles/Slots.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_KosinskiM_Art_End_Boss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/KosinskiM Art/End Boss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_KosinskiM_Art_Miniboss COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/KosinskiM Art/Miniboss.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_KosinskiM_Art_Sand_Particles COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/KosinskiM Art/Sand Particles.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_Tiles_Act_1_Custom COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/Tiles/Act 1 Custom.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_Tiles_Act_2_Custom COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/Tiles/Act 2 Custom.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_Tiles_Act_2_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/Tiles/Act 2 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SOZ_Tiles_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SOZ/Tiles/Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_KosinskiM_Art_Death_Egg_Small COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/KosinskiM Art/Death Egg Small.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_KosinskiM_Art_GHZ_Ball_and_Chain COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/KosinskiM Art/GHZ Ball and Chain.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_KosinskiM_Art_MTZ_Orbs COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/KosinskiM Art/MTZ Orbs.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_KosinskiM_Art_Spiral_Ramp COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/KosinskiM Art/Spiral Ramp.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_KosinskiM_Art_Teleporter_Beam COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/KosinskiM Art/Teleporter Beam.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_KosinskiM_Art_Title_Card COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/KosinskiM Art/Title Card.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_Tiles_Act_1_Custom COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/Tiles/Act 1 Custom.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_Tiles_Act_1_Primary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/Tiles/Act 1 Primary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_Tiles_Act_1_Secondary COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/Tiles/Act 1 Secondary.bin")
add_test(NAME test_moduled_kosinski_Sonic_3K_Levels_SSZ_Tiles_Act_2 COMMAND kosinskim-compare "${CMAKE_CURRENT_SOURCE_DIR}/test/moduled kosinski/Sonic 3 & Knuckles/Levels/SSZ/Tiles/Act 2.bin")
