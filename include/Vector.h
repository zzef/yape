#ifndef VECTOR_H
#define VECTOR_H

class Vec {
	
	private:
		float x;
		float y;

	public:
		Vec();
		void print();
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

#endif
