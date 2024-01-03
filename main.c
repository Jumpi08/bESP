#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <raylib.h>
#include <varray.h>

Varray(uint8_t) read_entire_file(const char* file_path)
{
	Varray(uint8_t) buffer = NULL;
	FILE* file;
	file = fopen(file_path,"rb");
	if(file == NULL) return NULL;
	fseek(file,0,SEEK_END);
	uint64_t file_size = ftell(file);
	fseek(file,0,SEEK_SET);

	varray_reserve(buffer,file_size+1);
	fread(buffer,file_size,1,file);
	varray_length_set(buffer,file_size+1);
	fclose(file);

	buffer[file_size] = 0; //add null terminated byte
	return buffer;
}

int main(void)
{
	const char* file_path = "./main.c";
	Varray(uint8_t) file_buffer = read_entire_file(file_path);
	if(file_buffer == NULL)
	{
		fprintf(stderr,"Could not read file %s",file_path);
	}
	printf("%s\n",file_buffer);
	printf("Size of file buffer:%lu\n", varray_length(file_buffer));

	const int screen_width = 0xFF*4;
	const int screen_height = 0xFF*4;

	InitWindow(screen_width,screen_height,"bESP");
	SetTargetFPS(30);

	while(!WindowShouldClose())
	{
		if(IsFileDropped())
		{
			FilePathList dropped_files = LoadDroppedFiles();
			varray_destroy(file_buffer);
			file_buffer = read_entire_file(dropped_files.paths[0]);

		}
		BeginDrawing();
			ClearBackground(BLACK);
			for(int i = 0; i < varray_length(file_buffer)-1; ++i)
			{
				Color color = (Color){255,0,0,50};
				uint64_t pixel_x = (file_buffer[i])*4;
				uint64_t pixel_y = (file_buffer[i+1])*4;
				DrawRectangle(pixel_x, pixel_y, 4, 4, color);
			}
		EndDrawing();
	}

	varray_destroy(file_buffer);

	return 0;
}
