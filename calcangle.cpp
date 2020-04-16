#include "includes.h"

vec3 angles::Subtract(vec3 src, vec3 dst)
{
	vec3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

float angles::Magnitude(vec3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float angles::Distance(vec3 src, vec3 dst)
{
	vec3 diff = Subtract(src, dst);
	return Magnitude(diff);
}

vec3 angles::CalcAngle(vec3 src, vec3 dst)
{
	vec3 angles;

	vec3 delta = dst - src;
	float hyp = src.Distance(dst);
	angles.x = -asin(delta.z / hyp) * (180.0f / PI);
	angles.y = atan2(delta.y, delta.x) * (180.0f / PI);
	angles.z = 0.0f;

	return angles;
}

vec3 angles::Clamp(vec3 angle)
{
	if (angle.x > 89.0f && angle.x <= 180.0f)
		angle.x = 89.0f;
	if (angle.x > 180.0f)
		angle.x = angle.x - 360.0f;
	if (angle.x < -89.0f)
		angle.x = -89.0f;
	if (angle.y > 180.0f)
		angle.y = angle.y - 360.0f;
	if (angle.y < -180.0f)
		angle.y = angle.y + 360.0f;
	return angle;
}

vec3 angles::Norm(vec3 angle)
{
	while (angle.y < -180) {
		angle.y += 360;
	}
	while (angle.y > 190) {
		angle.y -= 360;
	}
	while (angle.x > 89) {
		angle.x = 89;
	}
	while (angle.x < -89) {
		angle.x = -89;
	}
	return angle;
}