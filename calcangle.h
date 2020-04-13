#pragma once
#pragma once
#include "includes.h"
#define PI 3.1415927f

namespace angles
{
	vec3 Subtract(vec3 src, vec3 dst);
	float Magnitude(vec3 vec);
	float Distance(vec3 src, vec3 dst);
	vec3 CalcAngle(vec3 src, vec3 dst);
	vec3 Clamp(vec3 angle);
	vec3 Norm(vec3 angle);
}
