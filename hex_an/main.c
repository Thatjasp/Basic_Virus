int main() {
	__asm__("mov $0x401040, %rax");
	__asm__("jmpq *%rax");
}
