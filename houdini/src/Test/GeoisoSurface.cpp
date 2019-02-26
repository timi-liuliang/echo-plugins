#include "GeoisoSurface.h"
#include "GU/GU_Detail.h"

namespace Echo
{
	static float densityFunction(const UT_Vector3& P, void* data)
	{
		// return signed distance to unit sphere
		return 1.f - P.length();
	}

	void geoisoSurfaceTest()
	{
		GU_Detail		gdp;
		UT_BoundingBox	bounds;

		// evaluate the iso-surface inside this bounding box
		bounds.setBounds(-1, -1, -1, 1, 1, 1);

		// create an iso-surface
		gdp.polyIsoSurface(densityFunction, nullptr, bounds, 20, 20, 20);

		// save to sphere.bgeo
		gdp.save("D:/sphere.bgeo", nullptr);
	}
}