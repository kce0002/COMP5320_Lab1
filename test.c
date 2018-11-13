#include <stdio.h>
#include <stdlib.h>
// test commit
int main() {
	u_int32_t a = 300;
	printf("\na10 = %d\n", a);
	printf("a16 = %x\n", a);
	char buf[32];
	printf("sizeof a: %d\n", sizeof(a));
	snprintf(buf, sizeof(a) + 1, "\n%02x\n", a);
	buf[4] = '\0';
	printf("STRING: \n%s\n", buf);
	return 0;
}
