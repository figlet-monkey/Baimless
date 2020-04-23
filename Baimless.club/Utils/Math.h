#pragma once
#include "..\SDK\VMatrix.h"

#define PI 3.14159265358979323846f

class Math {
public:

	void VectorAngles(const Vector &vecForward, Vector &vecAngles);
	auto get_rotated_position(Vector start, const float rotation, const float distance)->Vector;

	void VectorAngles(const Vector& forward, Vector& up, Vector& angles);

	void VectorTransform(const Vector in1, const matrix3x4_t in2, Vector &out);

	Vector CalcAngle(const Vector& vecSource, const Vector& vecDestination);

	vec_t VectorNormalize(Vector& v);

	void AngleVectors(const Vector &angles, Vector *forward);

	void NormalizeAngles(Vector& angles);

	float NormalizeYaw(float yaw);

	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

	void AngToVec(const Vector &angles, Vector &forward);

	Vector ProjectPoint(Vector Origin, double Yaw, double Distance);

	void RandomSeed(int seed);

	float RandomFloat(float min, float max);

	bool Clamp(Vector &angles);

	void ClampAngles(Vector &angles);

	float GRD_TO_BOG(float GRD);
	void AngleVectors(const Vector& vAngles, Vector& vForward);
	float Distance(Vector2D point1, Vector2D point2);
	template< class T, class Y >
	FORCEINLINE T clamp(T const& val, Y const& minVal, Y const& maxVal)
	{
		if (val < minVal)
			return minVal;
		else if (val > maxVal)
			return maxVal;
		else
			return val;
	}
}; 
extern Math g_Math;