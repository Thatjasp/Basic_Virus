#include <elf.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
struct code_cave {
	uint64_t cave_location;
	uint64_t cave_size;
};
char mov[] = {0x48,0xc7,0xc0,0x40,0x10,0x40,0x00};
char jmp[] = {0xff,0xe0};
void get_code_cave(int fd, struct code_cave *cc) {
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
				cave_start =  i + prev_offset;
			}
			else if ( cur_hex == 0) {
				cave_length++;
			}

			else if ( cur_hex != 0 ) {
				if (cave_length > max_cave_size) {
					max_cave_size = cave_length;
					max_cave_location = cave_start+1;
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

	cc->cave_size = max_cave_size;
	cc->cave_location = max_cave_location;

//	return max_cave_location;
}

int main(int argc, char **argv) {
	Elf64_Ehdr ehdr;
	Elf64_Phdr phdr;
	Elf64_Shdr shdr;
	int fd = open("hello", O_RDWR);
	read(fd, &ehdr, sizeof(ehdr));
	struct code_cave cc;
	get_code_cave(fd, &cc);
	printf("Entrypoint: 0x%x\n", ehdr.e_entry);
//	ehdr.e_entry = 0x401040;
	ehdr.e_entry = 0x400000 + cc.cave_location;
	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(ehdr));

	lseek(fd, 0, SEEK_SET);
	read(fd, &ehdr, sizeof(ehdr));
	printf("Entrypoint After: 0x%x\n", ehdr.e_entry);

	int finit_offset = ehdr.e_shoff + (15 * ehdr.e_shentsize);

	lseek(fd, finit_offset, SEEK_SET);
	int ret = read(fd, &shdr, sizeof(shdr));

//	//for (int i = 0; i < 15; ++i) {
//	//	read(fd, &shdr, sizeof(shdr));
//	//}
	printf("Section Header size: %d\n", shdr.sh_size);
	printf("Code Cave size: %d\n", cc.cave_size);

	shdr.sh_size = 0xd;

	lseek(fd, finit_offset, SEEK_SET);
	write(fd, &shdr, sizeof(shdr));

	lseek(fd, finit_offset, SEEK_SET);
	ret = read(fd, &shdr, sizeof(shdr));
	printf("New Section Header size: %d\n", shdr.sh_size);

	// Write Instructions

	lseek(fd, cc.cave_location, SEEK_SET);
	write(fd, mov, 7);
	write(fd, jmp, 2);

	close(fd);
	
}
