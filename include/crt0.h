// ensure that _start is in its own section so that it can be relocated freely
void _start(void) __attribute__((section(".text.start")));
