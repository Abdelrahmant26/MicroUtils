/*
 * Shows Current net speed for a given interface
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define BUF 1024
static char units[4][5] = {"B/s", "KB/s", "MB/s", "GB/s"};
typedef struct iface {
	long tx1, tx2, rx1, rx2;
} myif;
void consumeLine(FILE *f){
	int a;
	while((a = fgetc(f), a != '\n' && !feof(f) ));
}
void nospace(FILE *f){
	int a;
	while((a = fgetc(f), !isalnum(a) && !feof(f) ));
	ungetc(a, f);
}
void noDigit(FILE *f){
	int a;
	while((a=fgetc(f), !isdigit(a) && !feof(f) ));
	ungetc(a, f);
}
void getIfName(FILE *f, char *arr){
	int a;
	int i=0;
	while((a=fgetc(f), isalnum(a))){
		arr[i] = a;
		i++;
	}
	arr[i] = '\0';
	ungetc(a, f);
}
long getVal(FILE *f){
	long val = 0;
	int a;
	noDigit(f);
	while((a=fgetc(f), isdigit(a))){
		val *= 10;
		val += (a - '0');
	}
	ungetc(a, f);
	return val;
}

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Usage: %s ifname\n", argv[0]);
		return 0;
	}
	myif *if1 = malloc(sizeof(myif));
	long num, num2;
	char buf[BUF];
	FILE *net = fopen("/proc/self/net/dev", "r");
	fgets(buf, BUF, net);
	fgets(buf, BUF, net);
	int skip = 1, found=0;
	nospace(net);
	while(!feof(net)){
		skip++;
		getIfName(net, buf);
		if(!strcmp(buf, argv[1])){
			num = getVal(net);
			num2;
			for(int i=0; i<8;i++){
				num2 = getVal(net);
			}
			if1->tx1 = num;
			if1->rx1 = num2;
			found = 1;
			break;
		}
		consumeLine(net);
		nospace(net);
	}
	if(!found){
		printf("Interface %s not found\n", argv[1]);
		return 1;
	}
	sleep(2);
	while (1){
		rewind(net);
		for(int j=0;j<skip;j++){
			fgets(buf, BUF, net);
		}
		nospace(net);
		getIfName(net, buf);
		num = getVal(net);
		for(int i=0; i<8;i++){
			num2 = getVal(net);
		}
		if1->tx2 = num;
		if1->rx2 = num2;
		num = if1->tx2 - if1->tx1;
		num2 = if1->rx2 - if1->rx1;
		float n1 = (float)num/2;
		float n2 = (float)num2/2;
		int rIndex=0, tIndex=0;
		while(n1>1024){
			n1 /= 1024;
			tIndex++;
		}
		while(n2>1024){
			n2 /= 1024;
			rIndex++;
		}
	
		printf("%s - D:%.1f%s - U:%.1f%s\n", buf, n1, units[tIndex], n2, units[rIndex]);
		sleep(2);
		if1->tx1 = if1->tx2;
		if1->rx1 = if1->rx2;
	}

	return 0;
}


