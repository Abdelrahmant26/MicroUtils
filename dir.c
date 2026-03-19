#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#define PATH "/sys/devices/system/cpu/cpufreq"
int main(){
	char buf[100];
	long freq = 0;
	int total = 0;
	FILE *fd;
	DIR *dir = opendir(PATH);
	struct dirent *entry;

	while((entry = readdir(dir))){
		if( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
			continue;
		}
		total++;
	}

	char paths[total][100];
	rewinddir(dir);
	int tmp=total;
	while((entry = readdir(dir))){
		if( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
			continue;
		}
		sprintf(paths[--tmp],PATH"/%s""/scaling_cur_freq", entry->d_name);
	}


	while(1){
		freq = 0;
		for(int i=0; i<total; i++){
			fd = fopen(paths[i], "r");
			fgets(buf, 100, fd);
			freq += atol(buf);
			fclose(fd);
		}
		freq/=total;
		float frequency = (float)freq/1000000;
		printf("%.1f\n", frequency);
		sleep(2);
	}

	return 0;
}


