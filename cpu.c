/*
 * Displays CPU current utilization over the past SLICE period
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define SLICE 2

int main(){
	FILE *fd;
	int val1, val2;
	char tmp[10], c[100];
	size_t n = 99;
	char *str = c;
	long long cpuTotal, cpuTotal2;
	long jef1[8], jef2[8], delta[8];
	float cputil;
	fd = fopen("/proc/stat", "r");
	getline(&str, &n, fd);
	sscanf(str, "%s %ld %ld %ld %ld %ld %ld %ld %ld", tmp, jef1, jef1+1, jef1+2, jef1+3, jef1+4, jef1+5, jef1+6, jef1+7);
	fclose(fd);
	while(1){
		sleep(SLICE);
		fd = fopen("/proc/stat", "r");
		getline(&str, &n, fd);
		sscanf(str, "%s %ld %ld %ld %ld %ld %ld %ld %ld", tmp, jef2, jef2+1, jef2+2, jef2+3, jef2+4, jef2+5, jef2+6, jef2+7);
		cpuTotal = 0;
		for(int i=0;i<8;i++){
			delta[i] = jef2[i] - jef1[i];
			jef1[i] = jef2[i];
		}
		cputil = 0;
		for(int i=0;i<8;i++){
			cputil += delta[i];
		}
		cputil = (cputil - delta[3]) / cputil;
		printf("%.0f%%\n", cputil*100);
		fclose(fd);
	}
	return 0;
}

