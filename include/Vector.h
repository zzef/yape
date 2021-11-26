#ifndef VECTOR_H
#define VECTOR_H

struct Edge;

class Vec {
	

	public:
		float y = 0;
		float x = 0;
		Vec();
		void print();
		Vec(Vec& v1, Vec& v2);
		Vec(Edge& e);
		float get_x() const;
		float get_y() const;
		void set_x(float x);
		void set_y(float y);

		Vec(float x, float y);
		Vec(float x, float y, float x1, float y1);
		float cross(Vec& v);
		Vec cross(float value);
		float dot(Vec& v);
		float mag();
		float _mag();
		Vec normalize();
		Vec rotate(float w);
		Vec ortho();
		Vec mid(Vec& v);
		Vec absolute();
		float scalar_proj(Vec& v);
		Vec operator+(const Vec& v);
		Vec operator-(const Vec& v);
		Vec operator*(float s);
		Vec operator/(float s);
					
};

struct Edge {
	Vec v1;
	Vec v2;
	Edge() {
		this->v1 = Vec(0,0);
		this->v2 = Vec(0,0);
	}
	Edge(Vec v1, Vec v2) {
		this->v1=v1;
		this->v2=v2;
	}
};

#endif
