#pragma once

typedef float vec_t;
// 2D Vector
class Vector2
{
public:
	// Members
	float x, y;

	// Construction/destruction:
	Vector2(void);
	Vector2(vec_t X, vec_t Y);
	Vector2(vec_t* clr);

	Vector2(const Vector2& vOther)
	{
		x = vOther.x; y = vOther.y;
	}

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	// arithmetic operations
	Vector2& operator+=(const Vector2& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	Vector2& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}

	Vector2& operator*=(const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2& operator/=(const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	// this ought to be an opcode.
	Vector2& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector2& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}
	Vector2& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	vec_t Length() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const
	{
		return (x * x + y * y);
	}
	void Clamp();
	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance);
	}

	vec_t NormalizeInPlace();
	Vector2 Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector2 const& boxmin, Vector2 const& boxmax);

	// Get the distance from this vector to the other one.
	vec_t DistTo(const Vector2& vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const Vector2& vOther) const
	{
		Vector2 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const Vector2& a, const Vector2& b, float scalar);

	// Dot product.
	vec_t Dot(const Vector2& vOther) const;

	// assignment
	Vector2& operator=(const Vector2& vOther);

	// 2d
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	Vector2  ProjectOnto(const Vector2& onto);

	// copy constructors
	// Vector2(const Vector2 &vOther);

	// arithmetic operations
	Vector2 operator-(void) const;

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator/(const Vector2& v) const;
	Vector2 operator*(float fl) const;
	Vector2 operator/(float fl) const;

	// Cross product between two vectors.
	Vector2 Cross(const Vector2& vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector2 Min(const Vector2& vOther) const;
	Vector2 Max(const Vector2& vOther) const;
};