#include <elf.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int get_code_cave(int fd) {
	char buf[1024];
	int max_cave_location = 0;
	int max_cave_size = 0;

	int cave_start = 0;

	int cave_length = 0;

	char prev_hex = 0x1;
	char cur_hex = 0;


	int prev_offset = 0;
	lseek(fd, 0, SEEK_SET);
	int bytes_read = read(fd, buf, sizeof(buf));
	while (bytes_read > 0) {

		for (int i = 0; i < bytes_read; ++i) {
			cur_hex = buf[i];
			if ( prev_hex != 0  && cur_hex == 0) {
				printf("New Cave 0x%x\n", prev_offset + i);
				cave_start =  i + prev_offset;
			}
			else if ( cur_hex == 0) {
				cave_length++;
			}

			else if ( cur_hex != 0 ) {
				if (cave_length > max_cave_size) {
					max_cave_size = cave_length;
					max_cave_location = cave_start;
				}
				cave_length = 0;
			}
			prev_hex = cur_hex;
		}
		prev_offset += bytes_read;
		bytes_read = read(fd, buf, sizeof(buf));
	}

	printf("Largest Cave size: %d\n", max_cave_size);
	printf("Largest Cave location: 0x%x\n", max_cave_location);
	return max_cave_location;
}

int main(int argc, char **argv) {
	Elf64_Ehdr ehdr;
	Elf64_Phdr phdr;
	Elf64_Shdr shdr;
	int fd = open("hello", O_RDWR);

	read(fd, &ehdr, sizeof(ehdr));

	get_code_cave(fd);
	//printf("Entrypoint: 0%x\n", ehdr.e_entry);
	//ehdr.e_entry = 0x1111111;
	//lseek(fd, 0, SEEK_SET);
	//write(fd, &ehdr, sizeof(ehdr));
	int finit_offset = ehdr.e_shoff + (15 * ehdr.e_shentsize);
	lseek(fd, finit_offset, SEEK_SET);
	int ret = read(fd, &shdr, sizeof(shdr));

	//for (int i = 0; i < 15; ++i) {
	//	read(fd, &shdr, sizeof(shdr));
	//}
	
	printf("Section Header size: 0x%x\n", shdr.sh_size);

	shdr.sh_size = 0xd + 40;

	close(fd);
	
}
