//Cioranu Nickolass-Alexandru
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define LALPH 37
#define SUTA 100.0

void swapdbl(double *a, double *b)
{
	double aux = *a;
	*a = *b;
	*b = aux;
}

void swapint(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

typedef struct {
	int x, y;
	char t, o;
} nava;

int **aloc_mat(int n, int m)
{
	int **a = malloc((n + 1) * sizeof(int *));
	if (!a) {
		printf("Matricea nu a fost alocata.\n");
		exit(1);
	}
	for (int i = 1; i <= n; i++) {
		a[i] = calloc(m + 1, sizeof(int));
		if (!a[i]) {
			printf("Linia %d a matricei nu a fost alocata.\n", i);
			exit(1);
		}
	}
	return a;
}

nava *aloc_v1(int n)
{
	nava *v = malloc(n * sizeof(nava));
	if (!v) {
		printf("Vectorul nu a fost alocat");
		exit(1);
	}
	return v;
}

double *aloc_v2(int n)
{
	double *v = malloc((n + 1) * sizeof(double));
	if (!v) {
		printf("Vectorul nu a fost alocat");
		exit(1);
	}
	return v;
}

int *aloc_v3(int n)
{
	int *v = calloc(n + 1, sizeof(int));
	if (!v) {
		printf("Vectorul nu a fost alocat");
		exit(1);
	}
	return v;
}

char *aloc_str(int n)
{
	char *s;
	s = malloc(n * sizeof(char));
	if (!s) {
		printf("Stringul nu a fost alocat");
		exit(1);
	}
	return s;
}

void free_mat(int **a, int n)
{
	for (int i = 1; i <= n; i++) {
		free(a[i]);
	}
	free(a);
}

void nr_nave(int n, int m, int *k)
{
	int nrS = (n * m) / 70;
	int nrY = (n * m) / 55;
	int nrB = (n * m) / 40;
	int nrL = (n * m) / 30;
	int nrA = (n * m) / 20;
	*k = nrS + nrY + nrB + nrL + nrA;
}

//ship length
int l(char a)
{
	switch (a) {
	case 'S': return 5;
	case 'Y': return 4;
	case 'B': return 3;
	case 'L': return 2;
	case 'A': return 1;
	default: return 0;
	}
}

char *nume1(char a)
{
	switch (a) {
	case 'S': return "Shinano";
	case 'Y': return "Yamato";
	case 'B': return "Belfast";
	case 'L': return "Laffey";
	case 'A': return "Albacore";
	case 'H': return "orizontal";
	case 'V': return "vertical";
	default: return 0;
	}
}

char *nume2(int x)
{
	switch (x) {
	case 5: return "Shinano";
	case 4: return "Yamato";
	case 3: return "Belfast";
	case 2: return "Laffey";
	case 1: return "Albacore";
	default: return 0;
	}
}

//checks whether the ship can be placed on the board
int verif(nava *v, int **a, int n, int m)
{
	int ok = 1, t = (*v).t, o = (*v).o, x = (*v).x, y = (*v).y;
	if (x > n || x < 1 || y < 1 || y > m) {
		ok = 0;
	} else if (o == 'H') {
		if (y + l(t) - 1 > m) {
			ok = 0;
		} else
			//check whether another ship has already been placed
			for (int i = y; i <= y + l(t) - 1; i++) {
				if (a[x][i] != 0) {
					ok = 0;
				}
			}
	} else if (o == 'V') {
		if (x - l(t) + 1 < 1) {
			ok = 0;
		} else
			for (int i = x; i >= x - l(t) + 1; i--) {
				if (a[i][y] != 0) {
					ok = 0;
				}
			}
	}
	if (ok == 0) {
		printf("Nava %s nu poate fi amplasata %s la coordonatele "
			   "(%d, %d).\n", nume1(t), nume1(o), x, y);
		return 0;
	}
	return 1;
}

void amplasare(nava v, int **a)
{
	int i;
	if (v.o == 'H') {
		for (i = v.y; i <= v.y + l(v.t) - 1; i++) {
			a[v.x][i] = l(v.t);
		}
	} else if (v.o == 'V') {
		for (i = v.x; i >= v.x - l(v.t) + 1; i--) {
			a[i][v.y] = l(v.t);
		}
	}
}

void afisare(int **a, int n, int m)
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

//builds the number representing accuracy x from the input message
void constr_f(double *f, char *mes)
{
	*f = (mes[5] - '0') * 100 + (mes[6] - '0') * 10 + (mes[7] - '0');
	*f += (double)((mes[9] - '0') * 10 + (mes[10] - '0')) / 100;
	*f = (int)(*f * SUTA) / SUTA;
}

//the following four functions correspond to the four statistics requirements
int c2(double x, int t, int l)
{
	//x is a given accuracy, t is the total number of shots
	//and l is the number of hits
	int i = 0;
	double f = (double)l / t * 100;
	while (f < x) {
		i++;
		f = (double)(l + i) / t * 100;
	}
	return i;
}

int c4(double x, int t, int l)
{
	int i = l;
	double f = (double)l / t * 100;
	if (f >= x) {
		while (f >= x) {
			i--;
			f = (double)i / t * 100;
		}
		i++;
	}
	return l - i;
}

//function c1 builds a vector v so that, after sorting in descending order,
//the first element is the one that would change the total accuracy the most
//if that match's accuracy were 100, sorting both the accuracies and the number
//of ships to preserve their correspondence when replacing the accuracy with 100
int c1(double x, int *l, int *t, int n, double acc_t, int *nr_n, int s)
{
	int i;
	//a[i] is the accuracy of a match
	double *a = aloc_v2(n), *v = aloc_v2(n);
	for (i = 1; i <= n; i++) {
		a[i] = (double)l[i] / t[i] * SUTA;
		a[i] = (int)(a[i] * SUTA) / SUTA;
		v[i] = nr_n[i] * (SUTA - a[i]);
	}
	for (i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			if (v[i] < v[j]) {
				swapdbl(&v[i], &v[j]);
				swapdbl(&a[i], &a[j]);
				swapint(&nr_n[i], &nr_n[j]);
			}
		}
	}
	i = 0;
	if (acc_t < x) {
		while (acc_t < x) {
			acc_t += nr_n[i + 1] * (SUTA - a[i + 1]) / s;
			i++;
		}
	}
	free(v);
	free(a);
	return i;
}

//in c3, sorting is done in ascending order, and v is initialized more simply
//because the accuracies will be replaced with 0
int c3(double x, int *l, int *t, int n, double acc_t, int *nr_n, int s)
{
	int i;
	double *a = aloc_v2(n), *v = aloc_v2(n);
	for (i = 1; i <= n; i++) {
		a[i] = (double)l[i] / t[i] * SUTA;
		a[i] = (int)(a[i] * SUTA) / SUTA;
		v[i] = nr_n[i] * a[i];
	}
	for (i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			if (v[i] > v[j]) {
				swapdbl(&v[i], &v[j]);
				swapdbl(&a[i], &a[j]);
				swapint(&nr_n[i], &nr_n[j]);
			}
		}
	}
	i = 0;
	if (acc_t >= x) {
		while (acc_t >= x) {
			acc_t -= nr_n[i + 1] * a[i + 1] / s;
			i++;
		}
		i--;
	}
	free(v);
	free(a);
	return i;
}

int codif(char c)
{
	if (c == '.') {
		return 36;
	} else if (isupper(c)) {
		return c - 'A' + 10;
	}
	return c - '0';
}

char decodif(int c)
{
		if (c == 36) {
			return '.';
		} else if (c < 36 && c > 9) {
			return 'A' + c - 10;
		}
		return c + '0';
}

int mod(int n, int k)
{
	int a = n % k;
	if (a < 0) {
		a = a + k;
	}
	return a;
}

int inv(int d)
{
	int a = mod(d, LALPH);
	for (int i = 1; i < LALPH; i++) {
		if (mod((a * i), LALPH) == 1) {
			return i;
		}
	}
	return -1;
}

int det(int **a, int n)
{
	if (n == 1) {
		return a[1][1];
	}
	if (n == 2) {
		return a[1][1] * a[2][2] - a[1][2] * a[2][1];
	}
	//the n == 3 case is included to avoid many recursive calls when n > 3
	if (n == 3) {
		return a[1][1] * a[2][2] * a[3][3] + a[2][1] * a[3][2] * a[1][3] +
			   a[3][1] * a[1][2] * a[2][3] - a[3][1] * a[2][2] * a[1][3] -
			   a[1][2] * a[2][1] * a[3][3] - a[1][1] * a[3][2] * a[2][3];
	}
	int d = 0, **m, mi, mj, semn;
	for (int col = 1; col <= n; col++) {
		m = aloc_mat(n - 1, n - 1);
		mi = 1; mj = 1;
		for (int i = 2; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (j != col) {
					m[mi][mj++] = a[i][j];
					if (mj == n) {
						mj = 1;
						mi++;
					}
				}
			}
		}
		if (col % 2) {
			semn = 1;
		} else {
			semn = -1;
		}
		d += a[1][col] * semn * det(m, n - 1);
		free_mat(m, n);
	}
	return d;
}

int **transpusa(int **a, int n)
{
	int **t = aloc_mat(n, n);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			t[i][j] = a[j][i];
		}
	}
	return t;
}

int **adjuncta(int **a, int n)
{
	int **tr = transpusa(a, n), semn, **adj = aloc_mat(n, n);
	int i = 1, j = 1;
	while (i <= n) {
		int **m = aloc_mat(n - 1, n - 1), mj = 1, mi = 1;
		for (int i1 = 1; i1 <= n; i1++) {
			for (int j1 = 1; j1 <= n; j1++) {
				if (i != i1 && j != j1) {
					m[mi][mj++] = tr[i1][j1];
					if (mj == n) {
						mj = 1;
						mi++;
					}
				}
			}
		}
		semn = 1;
		if ((i + j) % 2) {
			semn = -1;
		}
		adj[i][j] = semn * det(m, n - 1);
		free_mat(m, n - 1);
		j++;
		if (j > n) {
			i++;
			j = 1;
		}
	}
	free_mat(tr, n);
	return adj;
}

//the final matrix, which will be used in the product
int **mat_f(int **a, int n, int x)
{
	int **m = aloc_mat(n, n);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			m[i][j] = mod(x * a[i][j], LALPH);
		}
	}
	return m;
}

int *produs(int **a, int *v, int dim)
{
	int *m = aloc_v3(dim);
	for (int i = 1; i <= dim; i++) {
		for (int j = 1; j <= dim; j++) {
			m[i] += a[i][j] * v[j];
		}
		m[i] = mod(m[i], LALPH);
	}
	return m;
}

//repositions the dim-by-dim matrix a in an n-by-n matrix,
//selecting the first elements
int **repoz(int **a, int dim, int n)
{
	int **m = aloc_mat(n, n), mi = 1, mj = 1;
	for (int i = 1; i <= dim; i++) {
		for (int j = 1; j <= dim; j++) {
			m[mi][mj++] = a[i][j];
			if (mj == n + 1) {
				mj = 1;
				mi++;
				if (mi == n + 1)
					return m;
			}
		}
	}
	return m;
}

int **decriptare(char *c, char *mes, int dim)
{
	int i, j;
	int **cheie = aloc_mat(dim, dim);
	int dim_adj = dim;
	for (i = 1; i <= dim; i++) {
		for (j = 1; j <= dim; j++) {
			cheie[i][j] = codif(c[dim * (i - 1) + j - 1]);
		}
	}
	int d = det(cheie, dim), x = inv(d), **adj = adjuncta(cheie, dim);
	int **mf = mat_f(adj, dim, x);
	int *cod1, *cod2, l;
	//w was declared as size_t after running the checker
	size_t w = 0;
	while (w < strlen(mes)) {
		cod1 = aloc_v3(dim);
		l = strlen(mes + w);
		if (dim <= l) {
			for (j = 1; j <= dim; j++) {
				cod1[j] = codif(mes[j - 1 + w]);
			}
			cod2 = produs(mf, cod1, dim);
			for (j = 1; j <= dim; j++) {
				mes[j - 1 + w] = decodif(cod2[j]);
			}
			w = w + dim;
		} else {
			for (j = 1; j <= l; j++) {
				cod1[j] = codif(mes[j - 1 + w]);
			}
			int **copie = repoz(cheie, dim, l);
			d = det(copie, l);
			x = inv(d);
			free_mat(adj, dim_adj);
			free_mat(mf, dim_adj);
			adj = adjuncta(copie, l);
			mf = mat_f(adj, l, x);
			dim_adj = l;
			cod2 = produs(mf, cod1, l);
			free_mat(copie, l);
			for (j = 1; j <= l; j++) {
				mes[j - 1 + w] = decodif(cod2[j]);
			}
			w = strlen(mes);
		}
		free(cod1);
		free(cod2);
	}
	free_mat(adj, dim_adj);
	free_mat(mf, dim_adj);
	return cheie;
}

void criptare(int **cheie, char *str, int dim)
{
	int *cod1, *cod2, l, j;
	size_t w = 0;
	while (w < strlen(str)) {
		cod1 = aloc_v3(dim);
		l = strlen(str + w);
		if (dim <= l) {
			for (j = 1; j <= dim; j++) {
				cod1[j] = codif(str[j - 1 + w]);
			}
			cod2 = produs(cheie, cod1, dim);
			for (j = 1; j <= dim; j++) {
				str[j - 1 + w] = decodif(cod2[j]);
			}
			w = w + dim;
		} else {
			for (j = 1; j <= l; j++) {
				cod1[j] = codif(str[j - 1 + w]);
			}
			int **copie = repoz(cheie, dim, l);
			cod2 = produs(copie, cod1, l);
			free_mat(copie, l);
			for (j = 1; j <= l; j++) {
				str[j - 1 + w] = decodif(cod2[j]);
			}
			w = strlen(str);
		}
		free(cod1);
		free(cod2);
	}
}

char *creare_str(double x, int c)
{
	int z = (int)(x * 100), k = 1, kc = 0;
	int b = c, zero = 0;
	if (b == 0) {
		zero = 1;
	}
	while (b) {
		b /= 10;
		kc++;
		k *= 10;
	}
	char *s = malloc((8 + kc + 1) * sizeof(char));
	s[0] = (z / 10000) + '0';
	s[1] = (z / 1000 % 10) + '0';
	s[2] = (z / 100 % 10) + '0';
	s[3] = '.';
	s[4] = (z / 10 % 10) + '0';
	s[5] = (z % 10) + '0';
	s[6] = '.';
	if (zero) {
		s[7] = (c / k % 10) + '0';
		s[8] = '\0';
	} else {
		for (int i = 1; i <= kc; i++) {
			k /= 10;
			s[6 + i] = (c / k % 10) + '0';
		}
		s[6 + kc + 1] = '\0';
	}
	return s;
}

//function that handles the text output message
void mesaj(int rand, int **a, int *ki, int k, int x, int y, nava *v, int *li)
{
	//7 = previously attacked spaces
	//6 = unhit spaces belonging to a destroyed ship
	if (a[x][y] == 0 || a[x][y] == 6) {
		printf("Jucatorul %d a ratat o lovitura la coordonatele "
			   "(%d, %d).\n", rand, x, y);
	} else if (a[x][y] == 7) {
		printf("Coordonatele (%d, %d) au fost deja atacate de "
			   "jucatorul %d.\n", x, y, rand);
	} else {
		int ok = 0, i, j, l1;
		for (i = 0; i < k && ok == 0; i++) {
			if (x == v[i].x && y == v[i].y) {
				ok = 1;
				j = i;
				l1 = l(v[j].t);
			}
		}
		//if the player hits a ship at its bow or elsewhere
		if (ok == 0) {
			printf("Jucatorul %d a lovit o nava %s la coordonatele (%d, %d).\n",
				   rand, nume2(a[x][y]), x, y);
			(*li)++;
		} else {
			if (v[j].o == 'H') {
				for (i = v[j].y + 1; i <= v[j].y + l1 - 1; i++) {
					if (a[v[j].x][i] != 7) {
						a[v[j].x][i] = 6;
					}
				}
				printf("Jucatorul %d a distrus o nava %s plasata intre "
					   "coordonatele (%d, %d) si (%d, %d).\n", rand,
					   nume1(v[j].t), v[j].x, v[j].y, v[j].x, v[j].y + l1 - 1);
				(*ki)--;
				(*li)++;
			} else {
				for (i = v[j].x - 1; i >= v[j].x - l1 + 1; i--) {
					if (a[i][v[j].y] != 7) {
						a[i][v[j].y] = 6;
					}
				}
				printf("Jucatorul %d a distrus o nava %s plasata intre "
					   "coordonatele (%d, %d) si (%d, %d).\n", rand,
					   nume1(v[j].t), v[j].x, v[j].y, v[j].x - l1 + 1, v[j].y);
				(*ki)--;
				(*li)++;
			}
		}
	}
	a[x][y] = 7;
}

//the following two functions help reduce the number of lines in main
char *un_meci(char *mes, int *l1, int *t1, double f, int *l2, int *t2)
{
	int x = (mes[2] - '0') * 100 + (mes[3] - '0') * 10 + (mes[4] - '0');
	if (mes[0] == 'O') {
		double acc = (double)l1[x] / t1[x] * SUTA;
		if (strcmp(mes + 11, "MIN") == 0) {
			return creare_str(acc, c2(f, t1[x], l1[x]));
		} else {
			return creare_str(acc, c4(f, t1[x], l1[x]));
		}
	} else {
		double acc = (double)l2[x] / t2[x] * SUTA;
		if (strcmp(mes + 11, "MIN") == 0) {
			return creare_str(acc, c2(f, t2[x], l2[x]));
		} else {
			return creare_str(acc, c4(f, t2[x], l2[x]));
		}
	}
}

char *tot_jocul(char *mes, int z, double acc_t1, double acc_t2, double f,
				int *k, int *l1, int *l2, int *t1, int *t2)
{
	int s = 0;
	for (int i = 1; i <= z; i++) {
		s += k[i];
	}
	if (mes[0] == 'O') {
		if (strcmp(mes + 11, "MIN") == 0) {
			return creare_str(acc_t1, c1(f, l1, t1, z, acc_t1, k, s));
		} else {
			return creare_str(acc_t1, c3(f, l1, t1, z, acc_t1, k, s));
		}
	} else {
		if (strcmp(mes + 11, "MIN") == 0) {
			return creare_str(acc_t2, c1(f, l2, t2, z, acc_t2, k, s));
		} else {
			return creare_str(acc_t2, c3(f, l2, t2, z, acc_t2, k, s));
		}
	}
}

int main(void)
{
	int j, n, m, ax, ay, **m1, **m2, i, *k, z = 0;
	int *t1, *t2, *l1, *l2, s = 0, **cheie, cript = 0, dim;
	scanf("%d", &j);
	double acc_t1 = 0, acc_t2 = 0, f;
	t1 = aloc_v3(j); l1 = aloc_v3(j); t2 = aloc_v3(j);
	l2 = aloc_v3(j); k = aloc_v3(j);
	while (j) {
		z++;	i = 0;
		scanf("%d %d", &n, &m);
		m1 = aloc_mat(n, m); m2 = aloc_mat(n, m); nr_nave(n, m, &k[z]);
		nava *nv1 = aloc_v1(k[z]), *nv2 = aloc_v1(k[z]);
		while (i < k[z]) {
			int ok = 0;
			while (ok == 0) {
				scanf(" %c %c", &nv1[i].t, &nv1[i].o);
				scanf("%d %d", &nv1[i].x, &nv1[i].y);
				ok = verif(&nv1[i], m1, n, m);
			}
			amplasare(nv1[i], m1);	ok = 0;
			while (ok == 0) {
				scanf(" %c %c", &nv2[i].t, &nv2[i].o);
				scanf("%d %d", &nv2[i].x, &nv2[i].y);
				ok = verif(&nv2[i], m2, n, m);
			}
			amplasare(nv2[i], m2);
			i++;
		}
		afisare(m1, n, m); printf("\n"); afisare(m2, n, m);
		int ok = 0, rand = 1, k1 = k[z], k2 = k[z];
		while (ok == 0) {
			scanf("%d %d", &ax, &ay);
			if (rand == 1) {
				mesaj(rand, m2, &k2, k[z], ax, ay, nv2, &l1[z]);
				if (k2 == 0) {
					ok = 1;
					printf("Jucatorul %d a castigat.\n", rand);
				}
				t1[z]++;
			} else {
				mesaj(rand, m1, &k1, k[z], ax, ay, nv1, &l2[z]);
				if (k1 == 0) {
					ok = 1;
					printf("Jucatorul %d a castigat.\n", rand);
				}
				t2[z]++;
			}
			rand = rand % 2 + 1;
		}
		//total accuracies
		acc_t1 += k[z] * ((double)l1[z] / t1[z] * SUTA);
		acc_t2 += k[z] * ((double)l2[z] / t2[z] * SUTA);
		s = s + k[z];	j--;
		free(nv1); free(nv2); free_mat(m1, n); free_mat(m2, n);
	}
	char *c, *mes, *str;
	acc_t1 = (int)(acc_t1 / s * SUTA) / SUTA; mes = aloc_str(15);
	acc_t2 = (int)(acc_t2 / s * SUTA) / SUTA; c = aloc_str(SUTA);
	scanf(" %s", c);
	if (strcmp(c, "Q")) {
		scanf(" %s", mes);
		while (strcmp(mes, "Q")) {
			if (strcmp(c, "-")) {
				dim = sqrt(strlen(c)); cript = 1;
				cheie = decriptare(c, mes, dim);
			}
			constr_f(&f, mes);
			if (mes[1] == 'U')
				str = un_meci(mes, l1, t1, f, l2, t2);
			else
				str = tot_jocul(mes, z, acc_t1, acc_t2, f, k, l1, l2, t1, t2);
			if (cript == 1)
				criptare(cheie, str, dim);
			printf("%s\n", str); free(str); scanf(" %s", mes);
			if (cript == 1)
				free_mat(cheie, dim);
		}
	}
	free(t1); free(l1); free(l2); free(t2); free(k); free(mes); free(c);
	return 0;
}
