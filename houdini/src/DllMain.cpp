#include "DllMain.h"
#include "HoudiniModule.h"
#include "Test/GeoisoSurface.h"
#include "Test/HAPISession.h"
#include "Test/HDALoader.h"
#include "Test/IdToString.h"
#include <memory>
#include <vector>

using namespace Echo;

bool loadHoudiniEnginePlugin()
{
	// 1
	//geoisoSurfaceTest();

	HAPI_Result result = HAPI_RESULT_FAILURE;

	// 1.建立链接（会话)
	HAPI_Session session;
	startHAPISessionNamedPipe(session);

	// 2.初始化链接
	HAPI_CookOptions CookOptions;
	memset(&CookOptions, 0, sizeof(HAPI_CookOptions));
	CookOptions.curveRefineLOD = 8.0f;
	CookOptions.clearErrorsAndWarnings = false;
	CookOptions.maxVerticesPerPrimitive = 3;
	CookOptions.splitGeosByGroup = false;
	CookOptions.refineCurveToLinear = true;
	CookOptions.handleBoxPartTypes = false;
	CookOptions.handleSpherePartTypes = false;
	CookOptions.splitPointsByVertexAttributes = false;
	CookOptions.packedPrimInstancingMode = HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT;
	result = HAPI_Initialize(&session, &CookOptions, true, -1, "", "", "", "", "");

	// 3.加载hda文件
	HAPI_AssetLibraryId assetLibraryId = -1;
	result = HAPI_LoadAssetLibraryFromFile(&session, "C:/svn/Pipeline/HoudiniProjects/PCGTools/Vegetation/Biome_NFS.hda", true, &assetLibraryId);

	// 4.获取hda资源数量
	int assetCount = 0;
	result = HAPI_GetAvailableAssetCount(&session, assetLibraryId, &assetCount);

	// 5.获取hda中资源名称
	std::vector<HAPI_StringHandle> AssetNames;
	if (assetCount)
	{
		AssetNames.resize(assetCount);
		HAPI_GetAvailableAssets(&session, assetLibraryId, &AssetNames[0], assetCount);
	}

	int hapiAssetId = AssetNames[0];
	std::string idString = HAPI_IdToString( hapiAssetId, session);

	// 创建结点
	HAPI_NodeId rootNodeId;
	result = HAPI_CreateNode(&session, -1, idString.c_str(), nullptr, false, &rootNodeId);

	// 获取参数信息
	HAPI_ParmInfo params[100];
	result = HAPI_GetParameters(&session, rootNodeId, params, 0, 1);

	registerModules();

	return true;
}

bool unloadHoudiniEnginePlugin()
{
	return false;
}