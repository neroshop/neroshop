/*
    Copyright (c) 2015-2021 sidtheprince

    This file is part of dokun-ui.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef DOKUN_UI_HPP
#define DOKUN_UI_HPP
#define DOKUN_UI_VERSION_MAJOR 1
#define DOKUN_UI_VERSION_MINOR 0
#define DOKUN_UI_VERSION_PATCH 0
#define DOKUN_UI_VERSION DOKUN_UI_VERSION_MAJOR "." DOKUN_UI_VERSION_MINOR "." DOKUN_UI_VERSION_PATCH
// header-only files.
#include "platform.hpp"
#include "types.hpp" 
#include "math.hpp"
#include "vector.hpp"
//#include "matrix_test.hpp"
//#include "quaternion.hpp" // not using right now
#include "string.hpp"
#include "logger.hpp"
#include "utility.hpp"
#include "system.hpp"
// engine.
#include "engine.hpp"
// container.
#include "window.hpp"
// graphics.
#include "texture.hpp"
#include "image.hpp"
// input.
#include "mouse.hpp"
#include "keyboard.hpp" //#include "touch.hpp" // deprecated
// audio.
//#include "sound.hpp"
//#include "music.hpp"
//#include "voice.hpp"
// font
#include "font.hpp"
// ui
#include "ui.hpp"
#include "box.hpp" // good // can also be used as a tooltip
#include "button.hpp" 
#include "label.hpp" // good
#include "edit.hpp" // ok - needs work on keyboard characters combinations
#include "progressbar.hpp" // good
#include "slider.hpp" // good
#include "toggle.hpp" // good
#include "scrollbar.hpp"
#include "list.hpp"
#include "spinner.hpp" // good
#include "menubar.hpp"
#include "grid.hpp"
// network.
//#include "server.hpp"
//#include "client.hpp"
// utilities - the most essential part of the engine
// entity and component
#include "entity.hpp"
#include "component.hpp"
// timer
#include "timer.hpp"
// shader (glsl)
#include "shader.hpp"
// resources (assets)
//#include "resource.hpp"
// data serialization (sqlite or libxml)
//#include "db.hpp"
// object manager - keeps track of objects
#include "factory.hpp" 
// script manager
#include "script.hpp"
// console
#include "console.hpp"
// video (cinema)
//#include "video.hpp"
// plugin (extension)
#include "plugin.hpp"
// file system
#include "file.hpp"
// event system
//#include "event.hpp"
// file format loader #include "loader.hpp" // deprecated
// renderer
#include "renderer.hpp"
// process
#include "process.hpp"
// browser
#include "browser.hpp"
#endif 
// about 60-70 headers; 50-60 sources                     missing (or not included here): dokun.h, build.h,  (dokun.h cannot include itself and build.h already includes dokun.h)
