#pragma once
#include <cmath>

class CTriangle
{
public:
	struct SPoint3D
	{
		float fX = 0.0f;
		float fY = 0.0f;
		float fZ = 0.0f;
		float fU = 0.0f;
		float fV = 0.0f;
		float fTx = 0.0f;
        float fTy = 0.0f;
        float fTz = 0.0f;
        //float fB = 0.0f;
		void Normalize()
		{
			float length = sqrtf(fX*fX + fY*fY + fZ*fZ);
			if (length)
			{
				fX = fX / length;
				fY = fY / length;
				fZ = fZ / length;
			}
		}
		SPoint3D operator*(float fScalar)
		{
			SPoint3D Result;
			Result.fX = fX * fScalar;
			Result.fY = fY * fScalar;
			Result.fZ = fZ * fScalar;
			return Result;
		}
	};

	CTriangle();
	CTriangle(SPoint3D Point1, SPoint3D Point2, SPoint3D Point3);
	~CTriangle();

	SPoint3D* GetPoint1();
	SPoint3D* GetPoint2();
	SPoint3D* GetPoint3();

	void SetPoint1(SPoint3D Point);
	void SetPoint2(SPoint3D Point);
	void SetPoint3(SPoint3D Point);

private:
	SPoint3D m_Point1;
	SPoint3D m_Point2;
	SPoint3D m_Point3;
};

