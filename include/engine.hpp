#pragma once
#define SDL_MAIN_HANDLED

#include "graphics/graphics_device.hpp"
#include "graphics/shader.hpp"
#include "graphics/deferred.hpp"
#include "graphics/mesh.hpp"
#include "graphics/texture.hpp"
#include "graphics/ui.hpp"

#include "system/log.hpp"
#include "system/clock.hpp"

#include "sound/sound_device.hpp"
#include "sound/sound2d.hpp"
#include "sound/sound3d.hpp"
#include "sound/soundlistener.hpp"

#include "script/script_device.hpp"
#include "script/reference.hpp"
#include "script/script.hpp"
#include "script/types/functions.hpp"
#include "script/types/scriptstdstring.h"

inline Graphics::CDevice& GetGraphics()
{
	return Graphics::CDevice::Instance();
}

inline Sound::CDevice& GetSound()
{
	return Sound::CDevice::Instance();
}

inline Script::CDevice& GetScript()
{
	return Script::CDevice::Instance();
}