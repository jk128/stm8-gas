#include <stdio.h>
#include <stdlib.h>

void *xmalloc(size_t size) {
	void *ret = malloc(size);
	if(!ret) {
		fprintf(stderr, "Couldn't malloc\n");
		exit(-1);
	}
	return(ret);
}
