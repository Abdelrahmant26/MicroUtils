/*
 * Displays CPU current utilization over the past SLICE period
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#define SLICE 2
#define PATH "/sys/devices/system/cpu/cpufreq"

float avg(int total, char paths[][100]){
	/*
	 * calculate the average frequency by reading each policy's current 
	 * frequency and average them.
	 */
	FILE* fd;
	char buf[100];
	int freq = 0;
	for(int i=0; i<total; i++){
		fd = fopen(paths[i], "r");
		fgets(buf, 100, fd);
		freq += atol(buf);
		fclose(fd);
	}
	freq/=total;
	float frequency = (float)freq/1000000;
	return frequency;
}

int getPolicies(){
	/*
	 * Get the total number of directories under
	 * /sys/deices/system/cpu/cpufreq
	 */
	int total = 0;
	DIR* dir = opendir(PATH);
	struct dirent *entry;
	while((entry = readdir(dir))){
		if( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") ){
			continue;
		}
		total++;
	}
	closedir(dir);
	return total;
}

void assignPolicies(int total, char paths[][100]){
	/*
	 * After the numebr of policies has been known, Hardcode their paths
	 * to save the redundant directory traversal over and over
	 */
	DIR* dir = opendir(PATH);
	struct dirent *entry;
	while((entry = readdir(dir))){
		if( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") ){
			continue;
		}
		sprintf(paths[--total], PATH"/%s/scaling_cur_freq", entry->d_name);
	}
	closedir(dir);
}

int main(){
	// One time initialization for policies
	int total = getPolicies();
	char paths[total][100];
	assignPolicies(total, paths);
	
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
		printf("%.0f%% - %.1f\n", cputil*100, avg(total,paths));
		fclose(fd);
	}
	return 0;
}

