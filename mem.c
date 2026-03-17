/*
 * Shows memory usage
 */
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
int main(){
	FILE *fd;
	char c[100], *str = c;
	size_t n = 99;
	char s1[100], s2[100];
	int val1, val2;
	while(1){
		fd = fopen("/proc/meminfo", "r");
		getline(&str, &n, fd);
		sscanf(str, "%s %s %s", s1, s1, s2);
		getline(&str, &n, fd);
		getline(&str, &n, fd);
		sscanf(str, "%s %s", s2, s2);
		val1 = atoi(s1);
		val2 = atoi(s2);
		printf("%.0f\n", ( val1 - val2 )*100 / (float)val1 );
		fclose(fd);
		sleep(5);
	}
	return 0;
}

