#include <ucontext.h>
#include <stdio.h>

int func(void *arg) {
	puts("this is func");
}

void coroutine_test() {
	char stack[1024 * 128];
	ucontext_t child, main;
	
	getcontext(&child);

	child.uc_stack.ss_sp = stack;
	child.uc_stack.ss_size = sizeof(stack);
	child.uc_stack.ss_flags = 0;

	child.uc_link = &main;

	makecontext(&child, (void (*)(void)) func, 0);

	swapcontext(&main, &child);
	puts("this is coroutine_test");
}

int main() {
	coroutine_test();
	return 0;
}
