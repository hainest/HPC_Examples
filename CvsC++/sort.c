#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int compare_int(const void *pa, const void *pb) {
	int a = *(int*)pa;
	int b = *(int*)pb;
	
	if(a < b) {
		return -1;
	}
	if(a > b) {
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "Usage: %s file N\n", argv[0]);
		return -1;
	}
	
	FILE *fin = fopen(argv[1],"r");
	
	if(!fin) {
		fprintf(stderr,"Unable to open file %s\n", argv[1]);
		return -1;
	}
	
	int n = atoi(argv[2]);
	int *x = malloc(n * sizeof(int));
	
	for(int i=0; i<n; ++i) {
		int r = fscanf(fin,"%d",&x[i]);
		if(r != 1) {
			fprintf(stderr,"Error reading line %d from file %s\n",i+1,argv[1]);
			goto quit;
		}
	}

	struct timeval start;
	gettimeofday(&start, NULL);

	qsort(x,n,sizeof(int),compare_int);

	struct timeval stop;
	gettimeofday(&stop, NULL);

	printf("%d,%ld\n",n,((stop.tv_sec - start.tv_sec)*1000000L + stop.tv_usec) - start.tv_usec);

quit:
	if(x)
		free(x);

	if(fin)
		fclose(fin);
	
	return 0;
}
