/* 
 * This sweeps an RGB LED through a rainbow of colors.
 */

// RGB conversion code adapted from David H https://stackoverflow.com/a/6930407/857012
typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} RGB;

static RGB hue2rgb(double hh);

RGB hue2rgb(double hh) {
	double p, q, ff;
	long i;
	RGB out;

	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = 0.0;
	q = 1.0 - ff;

	switch (i) {
	case 0:
		out.r = 1;
		out.g = ff;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = 1;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = 1;
		out.b = ff;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = 1;
		break;
	case 4:
		out.r = ff;
		out.g = p;
		out.b = 1;
		break;
	case 5:
	default:
		out.r = 1;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}
// End RGB conversion code.

int RedPin = 9;
int GreenPin = 10;
int BluePin = 11;
int hue = 0; // Marches up from 0 through 359.

void setup() {
	pinMode(RedPin, OUTPUT);
	pinMode(GreenPin, OUTPUT);
	pinMode(BluePin, OUTPUT);
}

void loop() {
	RGB rgb = hue2rgb(hue);
	analogWrite(RedPin, rgb.r * 100);
	analogWrite(GreenPin, rgb.g * 100);
	analogWrite(BluePin, rgb.b * 100);
	if (++hue >= 360)
		hue = 0;

	delay(15);
}