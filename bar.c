#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
char line[200], tmp[100];
char emoji[11][5] = {"󰂎", "󰁺", "󰁻", "󰁼", "󰁽", "󰁾", "󰁿", "󰂀", "󰂁", "󰂂", "󰁹"};
//wchar_t battery[] = {0xf008e, 0xf007a, 0xf007b, 0xf007c, 0xf007d, 0xf007f, 0xf0080, 0xf0081, 0xf0082, 0xf0079};
//read cpu user nice sys idle io irq sirq steal none < /proc/stat
//long user, nice, sys, idle, io, irq, sirq; 
int main(){
	FILE *fd;
	char *str = malloc(100 * sizeof(char));
	size_t n = 99;
	//printf("%s", str);
	char s[100];
	char ss[100];
	long val, val2;
	setlocale(LC_ALL, "en_US.UTF-8");
	
	FILE *cpu;
	long long cpuTotal, cpuTotal2; 
	long jeff[8], jef2[8], delta[8];
	float cputil = 0;
	char *cpustr = malloc(100 * sizeof(char));
	size_t cpun = 99;
	cpu = fopen("/proc/stat", "r");
	getline(&cpustr, &cpun, cpu);
	sscanf(cpustr, "%s %ld %ld %ld %ld %ld %ld %ld %ld", s, jeff, jeff+1, jeff+2, jeff+3, jeff+4, jeff+5, jeff+6, jeff+7);
	printf("%s\n", cpustr);
	printf("%ld %ld %ld %ld %ld %ld %ld %ld\n", jeff[0], jeff[1], jeff[2], jeff[3], jeff[4], jeff[5], jeff[6], jeff[7]);
	fclose(cpu);

	while(1){
		//Mem
		fd = fopen("/proc/meminfo", "r");
		getline(&str, &n, fd);
		sscanf(str, "%s %s", s, ss);
		val = atoi(ss);
		getline(&str, &n, fd);
		getline(&str, &n, fd);
		sscanf(str, "%s %s", s, ss);
		//printf("%s %s\n", s, ss);
		val2 = atoi(ss);
		//printf("%s %s\n", s, ss);
		//printf("%d %d\n", val, val2);
		float perc = ( ( (float)val - val2 ) * 100 )/val;
		sprintf(tmp,"%.0f ", perc);
		strcat(line, tmp);
		fclose(fd);

		// Battery
		fd = fopen("/sys/class/power_supply/BAT0/capacity", "r");
		getline(&str, &n, fd);
		sscanf(str, "%s", s);
		val = atoi(s);
		//wprintf(L"Bat=%d%% %lc\n", val, battery[val/10]);//"\udb80");//\udc7a");
		sprintf(tmp, "Bat: %d%%%s ", val, emoji[val/10]);
		strcat(line, tmp);
		fclose(fd);

		// Time
		time_t t = time(NULL);
		struct tm *tm_info = localtime(&t);
		sprintf(tmp, "%02d-%02d-%02d ", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
		strcat(line, tmp);
		printf("%s\n", line);
		//printf("%s", "󰂀");

		cpu = fopen("/proc/stat", "r");
		getline(&cpustr, &cpun, cpu);
		sscanf(cpustr, "%s %ld %ld %ld %ld %ld %ld %ld %ld", s, jef2, jef2+1, jef2+2, jef2+3, jef2+4, jef2+5, jef2+6, jef2+7);
		printf("%s\n", cpustr);
		printf("%ld %ld %ld %ld %ld %ld %ld %ld\n", jef2[0], jef2[1], jef2[2], jef2[3], jef2[4], jef2[5], jef2[6], jef2[7]);
		fclose(cpu);
		for(int i=0;i<7;i++){
			delta[i] = jef2[i] - jeff[i];
		}
		cputil = 0;
		for(int i=0;i<7;i++){
			if(i != 3){
				cputil += delta[i];
			}
		}
		cputil = cputil/delta[3];
		printf("%f\n", cputil);
		sleep(5);
	}

	return 0;
}
