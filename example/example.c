struct Point {
  int x;
  int y;
};

int add(int a, int b) {
	return a + b;
}

int main() {
	int a, b, c;
	float d;
	double e;
	char f;
	struct Point p;

	a = 2;
	b = a + 5;
	d = 3.14;
	e = 2.71828;
	f = 'A';

	if (a > b) {
		a = b;
	} else {
		b = a;
	}

	while (a < 10) {
		a = a + 1;
	}

	c = add(a, b);
	p.x = a;
	p.y = b;

	return 0;
}