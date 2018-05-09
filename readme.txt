This a handful of tools relating to researching the compression formats used by Sega, mainly in the Mega Drive Sonic games.

kos - main.c kosinski.c
 A compressor for the "Kosinski" format that aims to produce identical output to Sega's own compressor.

decomp - decomp.c
 A tool that parses a Kosinski file, and outputs each match to a text file. Used for comparing the output of the kos program with an official Kosinski file.
