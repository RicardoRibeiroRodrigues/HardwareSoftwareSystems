#include <stdio.h>

int ex5_solucao () {
	int a = 1;
	int count = 0;
	while (a > 0) {
		scanf("%d", &a);
		if (a > 0){
			count += a;
		}
	}
	return count;
}
