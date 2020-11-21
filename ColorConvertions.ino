//
// Code snippet taken as is from 
// https://gist.github.com/postspectacular/2a4a8db092011c6743a7#file-hsv2rgb-ino-L29

// HSV->RGB conversion based on GLSL version
// expects hsv channels defined in 0.0 .. 1.0 interval
float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float* hsv2rgb(float h, float s, float b, float* rgb) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}

float* rgb2hsv(float r, float g, float b, float* hsv) {
  float s = step(b, g);
  float px = mix(b, g, s);
  float py = mix(g, b, s);
  float pz = mix(-1.0, 0.0, s);
  float pw = mix(0.6666666, -0.3333333, s);
  s = step(px, r);
  float qx = mix(px, r, s);
  float qz = mix(pw, pz, s);
  float qw = mix(r, px, s);
  float d = qx - min(qw, py);
  hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
  hsv[1] = d / (qx + 1e-10);
  hsv[2] = qx;
  return hsv;
}

// Discovered from this tweet thread https://twitter.com/koujaku/status/1326361797100662785
// Modified from this gist: https://gist.github.com/maluoi/1c4e1ebd34c4837313dff32992a10183

// Reference here: http://www.mrao.cam.ac.uk/~dag/CUBEHELIX/
float* cubehelix_hsv2rgb(float h, float s, float l, float* rgb) {
	const float tau = 6.28318f;
	l = min(1,l);
	float angle = tau * (h+(1/3.f));
	float amp   = s * l * (1.f - l);

	float a_cos = cos(angle);
	float a_sin = sin(angle);
	float r = l + amp * (-0.14861f * a_cos + 1.78277f * a_sin);
	float g = l + amp * (-0.29227f * a_cos - 0.90649f * a_sin);
	float b = l + amp * ( 1.97294f * a_cos);
	rgb[0] = max(0,min(1, r));
	rgb[1] = max(0,min(1, g));
	rgb[2] = max(0,min(1, b));
  return rgb;
}