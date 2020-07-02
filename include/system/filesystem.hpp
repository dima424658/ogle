//
//  clock.hpp
//
//  Created on: 12 Sep. 2019
//      Author: Dmitry Pankov
//

#pragma once

#include <filesystem>

namespace System
{
	enum class eResourceType
	{
		eImage, eVideo, eMesh
	};

	struct sResource
	{
		std::filesystem::directory_entry entry;
		eResourceType type;
	};

	class CResourceManager
	{
	public:

	private:

	};
};