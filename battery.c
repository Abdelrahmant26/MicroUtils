/*
 * Uses Icons from NerdFont
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
	FILE *fd;
	int val1, val2;
	char *str = malloc(100 * sizeof(char));
	size_t n = 99;
	char s1[100], s2[100];
	char emoji[11][5] = {"󰂎", "󰁺", "󰁻", "󰁼", "󰁽", "󰁾", "󰁿", "󰂀", "󰂁", "󰂂", "󰁹"};
	while(1){
		fd = fopen("/sys/class/power_supply/BAT0/capacity", "r");
		getline(&str, &n, fd);
		if(str[2] == '0'){
			str[3] = '\0';
			val1 = atoi(str);
		}else{
			str[2] = '\0';
			val1 = atoi(str);
		}
		printf("Bat:%d%%%s\n", val1, emoji[val1/10]);
		fclose(fd);
		sleep(5);
	}
	return 0;
}
