#pragma once
#include "Math/Vector.h"

class FStaticHelpers
{
public:
	static double GetAngleBetweenVectors(const FVector& A, const FVector& B)
	{
		const double Dot = FVector::DotProduct(A.GetSafeNormal(), B.GetSafeNormal());
		const double Angle = FMath::RadiansToDegrees(FMath::Acos(Dot));

		return Angle;
	};
};
