struct Vec3f
{
	float x;
	float y;
	float z;

	Vec3f() = default;
	
	Vec3f(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{}
	
	Vec3f(float f)
		: x(f), y(f), z(f)
	{}

};




