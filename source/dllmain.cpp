#include "stdafx.h"

struct DataDirObject
{
	DataDirObject(const uint32_t _1, const uint32_t _2, string16 _3, string16 _4)
		: enabled(_1)
		, packid(_2)
		, datadir(std::move(_3))
		, productkey(std::move(_4))
	{
	}
	uint32_t enabled;
	uint32_t packid;
	string16 datadir;
	string16 productkey;
};

//all the addresses are currently the non disk version only

static_detour(SetDataDirs_detour, void())
{
	void detoured()
	{

		original_function();

		auto data_dirs = reinterpret_cast<vector<DataDirObject> *>(Address(0x015fea00));
		auto data_dirs_size = reinterpret_cast<int32_t *>(Address(0x0152b348));

		data_dirs->emplace_back(0x1, 0x0, u"C:\\ProgramData\\Spore ModAPI Launcher Kit\\DataSMA\\", u"");
		++(*data_dirs_size);
	}
};

void AttachDetours()
{
	SetDataDirs_detour::attach(Address(0x687bd0));
}

// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

