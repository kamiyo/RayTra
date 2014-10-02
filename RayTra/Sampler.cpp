#include "Sampler.h"
#include "Hilbert.h"


Sampler::Sampler() {}

Sampler::Sampler(int dim_x, int dim_y, int type, int shape, int order, int algo, bool center)
: _x(dim_x), _y(dim_y), _type(type), _shape(shape), _order(order), _algo(algo), _center(center), _zero(false)
{
	if (_shape == CIRCLE) {
		int N = _x * _y;
		double temp = sqrt((double) N / M_PI);
		_y = (int)round(temp);
		_x = (int)round(temp * M_PI);
		if (_x * _y < N) {
			_y += 1;
		}
	}
	Hilbert hilbert(_x, _y);
	points = hilbert.getPoints();
	xy = Vector2d((double) _x, (double) _y);
	xy = xy.cwiseInverse();
	rooks.resize(_x, _y);
	for (int i = 0; i < _x; i++) {
		for (int j = 0; j < _y; j++) {
			rooks(i, j) = Vector2d((double) j / _y, (double) i / _x);
		}
	}
}

Sampler::Sampler(int dim_x, int dim_y) : _x(dim_x), _y(dim_y), _zero(true)
{
}

Sampler::~Sampler()
{
}

Sampler2d Sampler::genPoints() {
	Sampler2d out;
	out.resize(_x, _y);
	Vector2d adjust(0., 0.);
	if (_zero) {
		out.setConstant(Vector2d(0, 0));
		return out;
	}
	if (_center) {
		adjust << (double) _x / 2., (double) _y / 2.;
	}
	if (_algo == STRATIFIED) {
		if (_order == LINEAR) {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = (Vector2d(((double) p + RAN), ((double) q + RAN)) - adjust).cwiseProduct(xy);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = Vector2d(((double) p + RAN), ((double) q + RAN)) - adjust;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp = Vector2d(((double) p + RAN), ((double) q + RAN)).cwiseProduct(xy);
						DISK(temp);
						out(p, q) = temp / 2.;
					}
				}
			}
		}
		else if (_order == HILBERT) {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = (Vector2d(RAN, RAN) + points(p, q).cast<double>() - adjust).cwiseProduct(xy);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = Vector2d(RAN, RAN) + points(p, q).cast<double>() - adjust;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp = (Vector2d(RAN, RAN) + points(q * _x + p).cast<double>()).cwiseProduct(xy);
						DISK(temp);
						out(p, q) = temp / 2;
					}
				}
			}
		}
	}
	else if (_algo == CENTER) {
		if (_order == LINEAR) {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL){
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = (Vector2d(((double) p + 0.5), ((double) q + 0.5)) - adjust).cwiseProduct(xy);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = Vector2d(((double) p + 0.5), ((double) q + 0.5)) - adjust;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp = (Vector2d((double) p + 0.5, (double) q + 0.5)).cwiseProduct(xy);
						DISK(temp);
						out(p, q) = temp / 2.;
					}
				}
			}
		}
		else if (_order == HILBERT)  {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = (Vector2d(.5, .5) + points(q * _x + p).cast<double>() - adjust).cwiseProduct(xy);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = Vector2d(.5, .5) + points(q * _x + p).cast<double>() - adjust;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp = (Vector2d(.5, .5) + points(q * _x + p).cast<double>()).cwiseProduct(xy);
						DISK(temp);
						out(p, q) = temp / 2.;
					}
				}
			}
		}
	}
	else if (_algo == NROOKS || _algo == NROOKS_CORR) {
		(_algo == NROOKS)?shuffle(rooks, false):shuffle_correlated(rooks, _x, _y);
		if (_order == LINEAR) {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL){
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = (Vector2d(p, q) + rooks(p, q) - adjust).cwiseProduct(xy);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = Vector2d(p, q) + rooks(p, q) - adjust;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp = (Vector2d(p, q) + rooks(p, q)).cwiseProduct(xy);
						DISK(temp);
						out(p, q) = temp / 2.;
					}
				}
			}
		}
		else if (_order == HILBERT)  {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = (points(p, q).cast<double>() + rooks(p, q) - adjust).cwiseProduct(xy);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							out(p, q) = points(p, q).cast<double>() + rooks(p, q) - adjust;

						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp = (points(p, q).cast<double>() + rooks(p, q)).cwiseProduct(xy);
						DISK(temp);
						out(p, q) = temp / 2.;
					}
				}
			}
		}
	}
	return out;
}

void Sampler::shuffle(Sampler2d& samples, bool together) {
	if (together) {
		for (int i = (int) samples.size() - 1; i > 0; i--) {
			int random = genRand_int(0, i);
			std::swap(samples(i), samples(random));
		}
	}
	else {
		for (int i = (int) samples.size() - 1; i > 0; i--) {
			int randomx = genRand_int(0, i);
			int randomy = genRand_int(0, i);
			std::swap(samples(i)(0), samples(randomx)(0));
			std::swap(samples(i)(1), samples(randomy)(1));
		}
	}
}


void Sampler::shuffle_correlated(Sampler2d& samples, int m, int n) {
	for (int j = n - 1; j > 0; j--) {
		int random = genRand_int(0, j);
		for (int i = m - 1; i > 0; i--) {
			std::swap(samples(i, j)(0), samples(i, random)(0));
		}
	}
	for (int i = m - 1; i > 0; i--) {
		int random = genRand_int(0, i);
		for (int j = n - 1; j > 0; j--) {
			std::swap(samples(i, j)(1), samples(random, j)(1));
		}
	}

}
