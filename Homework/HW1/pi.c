#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n, i;
	printf("n = ");
	scanf("%d", &n);

	double pi = 0.;
	double multiplier_term = 16;

	for (i=0; i<=n; i++) {
		double term_1 = 4.0/(8*i+1);
		double term_2 = 2.0/(8*i+4);
		double term_3 = 1.0/(8*i+5);
		double term_4 = 1.0/(8*i+6);
		multiplier_term /= 16.0;
		pi += multiplier_term*(term_1-term_2-term_3-term_4);
	}

	printf("PI = %.10f\n", pi);
	return 0;
}
