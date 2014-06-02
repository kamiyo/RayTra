#include "Sampler.h"
#include "Hilbert.h"


Sampler::Sampler() {}

Sampler::Sampler(int dim_x, int dim_y, int type, int shape, int order, int algo, bool center)
: _x(dim_x), _y(dim_y), _type(type), _shape(shape), _order(order), _algo(algo), _center(center), _zero(false)
{
	Hilbert hilbert(_x, _y);
	points = hilbert.getPoints();
	xy = Vector2d((double) _x, (double) _y);
}

Sampler::Sampler(int dim_x, int dim_y) : _x(dim_x), _y(dim_y), _zero(true)
{
}

Sampler::~Sampler()
{
}

Sampler2d Sampler::genPoints() {
	Sampler2d out;
	out.resize(_x * _y);
	Vector2d adjust(0., 0.);
	if (_zero) {
		for (int q = 0; q < _y; q++) {
			for (int p = 0; p < _x; p++) {
				out[q * _x + p] = Vector2d(0., 0.);
			}
		}
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
							Vector2d temp(((double) p + RAN), ((double) q + RAN));
							temp -= adjust;
							temp.array() /= xy.array();
							out[q * _x + p] = temp;
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(((double) p + RAN), ((double) q + RAN));
							temp -= adjust;
							out[q * _x + p] = temp;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp(((double) p + RAN), ((double) q + RAN));
						temp.array() /= xy.array();
						to_unit_disk(temp[0], temp[1], temp);
						out[q * _x + p] = temp / 2.;
					}
				}
			}
		}
		else if (_order == HILBERT) {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(((double) p + RAN), ((double) q + RAN));
							temp -= adjust;
							temp.array() /= xy.array();
							out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp;
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(((double) p + RAN), ((double) q + RAN));
							temp -= adjust;
							out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp(((double) p + RAN), ((double) q + RAN));
						temp.array() /= xy.array();
						to_unit_disk(temp[0], temp[1], temp);
						out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp / 2.;
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
							Vector2d temp(((double) p + 0.5), ((double) q + 0.5));
							temp -= adjust;
							temp.array() /= xy.array();
							out[q * _x + p] = (temp);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(((double) p + 0.5), ((double) q + 0.5));
							temp -= adjust;
							out[q * _x + p] = temp;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp(((double) p + 0.5), ((double) q + 0.5));
						temp.array() /= xy.array();
						to_unit_disk(temp[0], temp[1], temp);
						out[q * _x + p] = temp / 2.;
					}
				}
			}
		}
		else if (_order == HILBERT)  {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(((double) p + 0.5), ((double) q + 0.5));
							temp -= adjust;
							temp.array() /= xy.array();
							out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp;
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(((double) p + 0.5), ((double) q + 0.5));
							temp -= adjust;
							out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp(((double) p + 0.5), ((double) q + 0.5));
						temp.array() /= xy.array();
						to_unit_disk(temp[0], temp[1], temp);
						out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp / 2.;
					}
				}
			}
		}
	}
	else if (_algo == NROOKS) {
		if (_order == LINEAR) {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL){
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(p + (q + RAN) / _y, q + (p + RAN) / _x);
							temp -= adjust;
							temp.array() /= xy.array();
							out[q * _x + p] = (temp);
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(p + (q + RAN) / _y, q + (p + RAN) / _x);
							temp -= adjust;
							out[q * _x + p] = (temp);
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp(p + (q + RAN) / _y, q + (p + RAN) / _x);
						temp.array() /= xy.array();
						to_unit_disk(temp[0], temp[1], temp);
						out[q * _x + p] = temp / 2.;
					}
				}
			}
		}
		else if (_order == HILBERT)  {
			if (_shape == SQUARE) {
				if (_type == FRACTIONAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(p + (q + RAN) / _y, q + (p + RAN) / _x);
							temp -= adjust;
							temp.array() /= xy.array();
							out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp;
						}
					}
				}
				else if (_type == INTEGRAL) {
					for (int q = 0; q < _y; q++) {
						for (int p = 0; p < _x; p++) {
							Vector2d temp(p + (q + RAN) / _y, q + (p + RAN) / _x);
							temp -= adjust;
							out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp;
						}
					}
				}
			}
			else if (_shape == CIRCLE) {
				for (int q = 0; q < _y; q++) {
					for (int p = 0; p < _x; p++) {
						Vector2d temp(p + (q + RAN) / _y, q + (p + RAN) / _x);
						temp.array() /= xy.array();
						to_unit_disk(temp[0], temp[1], temp);
						out[points[q * _x + p][1] * _x + points[q * _x + p][0]] = temp / 2.;
					}
				}
			}
		}
		//shuffle n-rooks
		for (int q = 0; q < _y; q++) {
			for (int p = 0; p < _x; p++) {
				int r = q + RAN * (_y - q);
			}
		}
	}
	return out;
}

void Sampler::shuffle(Sampler2d& samples, bool together) {
	if (together) {
		for (int i = (int) samples.size() - 1; i > 1; i--) {
			int random = genRand_int(0, i);
			std::swap(samples[i], samples[random]);
		}
	}
	else {
		for (int i = (int) samples.size() - 1; i > 1; i--) {
			int randomx = genRand_int(0, i);
			int randomy = genRand_int(0, i);
			std::swap(samples[i][0], samples[randomx][1]);
			std::swap(samples[i][1], samples[randomy][1]);
		}
	}
}

