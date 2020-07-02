#pragma once
#define SDL_MAIN_HANDLED

#ifndef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include "graphics/device.hpp"
#include "graphics/shader.hpp"
#include "graphics/deferred.hpp"
#include "graphics/mesh.hpp"
#include "graphics/texture.hpp"
#include "graphics/ui.hpp"
#include "graphics/ui/editor.hpp"

#include "system/log.hpp"
#include "system/timer.hpp"
#include "system/input.hpp"

#include "sound/device.hpp"
#include "sound/sound2d.hpp"
#include "sound/sound3d.hpp"
#include "sound/soundlistener.hpp"

#include "script/device.hpp"
#include "script/reference.hpp"
#include "script/script.hpp"
#include "script/types/functions.hpp"
#include "script/types/scriptstdstring.h"
#include "script/types/glm_as.hpp"

#include "scene/object.hpp"
#include "scene/scene.hpp"
#include "scene/camera.hpp"