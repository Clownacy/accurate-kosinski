#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_file_to_buffer.h"
#include "kosinski_compress.h"
#include "kosinski_decompress.h"

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		FILE *in_file = fopen(argv[i], "rb");

		if (in_file)
		{
			FILE *out_file = fopen("out.unc", "wb");

			if (out_file)
			{
				KosinskiDecompress(in_file, out_file);

				fclose(in_file);
				fclose(out_file);

				unsigned char *file_buffer;
				long int file_size;

				if (LoadFileToBuffer("out.unc", &file_buffer, &file_size))
				{
					printf("File '%s' with size %lX loaded\n", argv[i], file_size);

					unsigned char *file_buffer1, *file_buffer2;
					long int file_size1, file_size2;

					file_size2 = KosinskiCompress(file_buffer, file_size, &file_buffer2);

					if (LoadFileToBuffer(argv[i], &file_buffer1, &file_size1))
					{
						if (file_size1 != file_size2)
							printf("File sizes don't match!\n");

						if (memcmp(file_buffer1, file_buffer2, (file_size1 > file_size2) ? file_size2 : file_size1))
							printf("The files don't match!\n\n");
						else
							printf("Yay the files match.\n\n");

						free(file_buffer1);
					}
					else
					{
						printf("Could not open '%s'\n", argv[i]);
					}

					free(file_buffer2);
					free(file_buffer);
				}
				else
				{
					printf("Could not open '%s'\n", "out.unc");
				}
			}
			else
			{
				printf("Could not open '%s'\n", "out.unc");
				fclose(in_file);
			}
		}
		else
		{
			printf("Could not open '%s'\n", argv[i]);
		}
	}

	getchar();

	remove("out.kos");
	remove("out.unc");;
}
