cmake_minimum_required(VERSION 3.0) # Modern CMake starts from version 3.0 onwards (3.7 comes with vulkan support)

project(dokun_ui)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
#############
# platform
if(WIN32) # tested on Windows 10 with MinGW (GNU 6.3.0)
    message("WIN32 device detected")
    set(WINDOWS TRUE)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # tested on Ubuntu 14.04(with GCC/G++4.8), 16.04(GCC/G++5.4.0), 19.04 (GCC 8.3.0), 20.04 (GCC 9.3.0 and Clang 10.0.0)
    message("Linux device detected")
    set(LINUX TRUE)
    option(DOKUN_CLANG "use clang" OFF) # clang (BSD License - your code can be used commercially and can be closed-source)
    option(DOKUN_GCC   "use gcc"   ON ) # gcc - shipped with Ubuntu by default (GPL License - your code must be free and open-source)
    set(CMAKE_C_COMPILER   /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()
#############
# compiler
if(LINUX) # if user chooses to compile with clang
if(DOKUN_CLANG) # cmake -DDOKUN_CLANG=ON -G"Unix Makefiles"
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # check if clang is set as the current compiler
    set(DOKUN_GCC OFF )  
    set(DOKUN_CLANG ON) # use clang (if not using gcc)
    set(CMAKE_C_COMPILER   /usr/bin/clang  )
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
else()
    message(WARNING "clang is not the default compiler!")
endif()
endif()
endif()
#############
# use C++ 11 (REQUIRED)
if(CMAKE_VERSION VERSION_LESS "3.1")
if(DOKUN_GCC) # if(CMAKE_COMPILER_IS_GNUCXX) # if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=gnu++11") # 98, 11, 14, 17, 20
endif()
if(DOKUN_CLANG)
    set(CMAKE_CXX_FLAGS "-std=c++11"  )
endif()
else()
    set(CMAKE_CXX_STANDARD 11) # supported in cmake version 3.1 and higher
endif() #
#############
# compiler defines
if(DOKUN_GCC) # if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND CMAKE_CXX_FLAGS "-no-pie") # no position independent executable (PIE)
endif()
if(DOKUN_CLANG)
    list(APPEND CMAKE_CXX_FLAGS "-w"     ) #-w=ignores all warnings
endif()
#############
# engine (dokun-ui)
set(dokun_ui_src  src/engine.cpp src/window.cpp src/texture.cpp src/image.cpp src/mouse.cpp src/keyboard.cpp #[[src/audio.cpp src/sound.cpp src/music.cpp src/voice.cpp]] src/font.cpp src/ui.cpp src/box.cpp src/button.cpp src/label.cpp src/edit.cpp src/progressbar.cpp src/slider.cpp src/toggle.cpp src/scrollbar.cpp src/list.cpp src/spinner.cpp src/menubar.cpp src/grid.cpp src/combo.cpp src/tab.cpp #[[src/server.cpp src/client.cpp]] src/entity.cpp src/component.cpp src/timer.cpp src/shader.cpp #[[src/resource.cpp]] #[[src/db.cpp]] src/factory.cpp src/script.cpp src/console.cpp #[[src/video.cpp]] src/plugin.cpp src/file.cpp #[[src/event.cpp]] src/renderer.cpp src/process.cpp src/browser.cpp src/logger.cpp)
#############
# scripting
set(lua51_src external/lua/lua-5.1.5/src/lua.c external/lua/lua-5.1.5/src/luac.c external/lua/lua-5.1.5/src/lapi.c external/lua/lua-5.1.5/src/lcode.c external/lua/lua-5.1.5/src/ldebug.c external/lua/lua-5.1.5/src/ldo.c external/lua/lua-5.1.5/src/ldump.c external/lua/lua-5.1.5/src/lfunc.c external/lua/lua-5.1.5/src/lgc.c external/lua/lua-5.1.5/src/llex.c external/lua/lua-5.1.5/src/lmem.c external/lua/lua-5.1.5/src/lobject.c external/lua/lua-5.1.5/src/lopcodes.c external/lua/lua-5.1.5/src/lparser.c external/lua/lua-5.1.5/src/lstate.c external/lua/lua-5.1.5/src/lstring.c external/lua/lua-5.1.5/src/ltable.c external/lua/lua-5.1.5/src/ltm.c external/lua/lua-5.1.5/src/lundump.c external/lua/lua-5.1.5/src/lvm.c external/lua/lua-5.1.5/src/lzio.c external/lua/lua-5.1.5/src/lauxlib.c external/lua/lua-5.1.5/src/lbaselib.c external/lua/lua-5.1.5/src/ldblib.c external/lua/lua-5.1.5/src/liolib.c external/lua/lua-5.1.5/src/lmathlib.c external/lua/lua-5.1.5/src/loslib.c external/lua/lua-5.1.5/src/lstrlib.c external/lua/lua-5.1.5/src/ltablib.c external/lua/lua-5.1.5/src/loadlib.c external/lua/lua-5.1.5/src/linit.c)
set(lua52_src external/lua/lua-5.2.4/src/lua.c external/lua/lua-5.2.4/src/luac.c external/lua/lua-5.2.4/src/lapi.c external/lua/lua-5.2.4/src/lcode.c external/lua/lua-5.2.4/src/lctype.c external/lua/lua-5.2.4/src/ldebug.c external/lua/lua-5.2.4/src/ldo.c external/lua/lua-5.2.4/src/ldump.c external/lua/lua-5.2.4/src/lfunc.c external/lua/lua-5.2.4/src/lgc.c external/lua/lua-5.2.4/src/llex.c external/lua/lua-5.2.4/src/lmem.c external/lua/lua-5.2.4/src/lobject.c external/lua/lua-5.2.4/src/lopcodes.c external/lua/lua-5.2.4/src/lparser.c external/lua/lua-5.2.4/src/lstate.c external/lua/lua-5.2.4/src/lstring.c external/lua/lua-5.2.4/src/ltable.c external/lua/lua-5.2.4/src/ltm.c external/lua/lua-5.2.4/src/lundump.c external/lua/lua-5.2.4/src/lvm.c external/lua/lua-5.2.4/src/lzio.c external/lua/lua-5.2.4/src/lauxlib.c external/lua/lua-5.2.4/src/lbaselib.c external/lua/lua-5.2.4/src/lbitlib.c external/lua/lua-5.2.4/src/lcorolib.c external/lua/lua-5.2.4/src/ldblib.c external/lua/lua-5.2.4/src/liolib.c external/lua/lua-5.2.4/src/lmathlib.c external/lua/lua-5.2.4/src/loslib.c external/lua/lua-5.2.4/src/lstrlib.c external/lua/lua-5.2.4/src/ltablib.c external/lua/lua-5.2.4/src/loadlib.c external/lua/lua-5.2.4/src/linit.c)                                           # 5.2 added lctype.c, lbitlib.c, and lcorolib.c
set(lua53_src external/lua/lua-5.3.6/src/lua.c external/lua/lua-5.3.6/src/luac.c external/lua/lua-5.3.6/src/lapi.c external/lua/lua-5.3.6/src/lcode.c external/lua/lua-5.3.6/src/lctype.c external/lua/lua-5.3.6/src/ldebug.c external/lua/lua-5.3.6/src/ldo.c external/lua/lua-5.3.6/src/ldump.c external/lua/lua-5.3.6/src/lfunc.c external/lua/lua-5.3.6/src/lgc.c external/lua/lua-5.3.6/src/llex.c external/lua/lua-5.3.6/src/lmem.c external/lua/lua-5.3.6/src/lobject.c external/lua/lua-5.3.6/src/lopcodes.c external/lua/lua-5.3.6/src/lparser.c external/lua/lua-5.3.6/src/lstate.c external/lua/lua-5.3.6/src/lstring.c external/lua/lua-5.3.6/src/ltable.c external/lua/lua-5.3.6/src/ltm.c external/lua/lua-5.3.6/src/lundump.c external/lua/lua-5.3.6/src/lvm.c external/lua/lua-5.3.6/src/lzio.c external/lua/lua-5.3.6/src/lauxlib.c external/lua/lua-5.3.6/src/lbaselib.c external/lua/lua-5.3.6/src/lbitlib.c external/lua/lua-5.3.6/src/lcorolib.c external/lua/lua-5.3.6/src/ldblib.c external/lua/lua-5.3.6/src/liolib.c external/lua/lua-5.3.6/src/lmathlib.c external/lua/lua-5.3.6/src/loslib.c external/lua/lua-5.3.6/src/lstrlib.c external/lua/lua-5.3.6/src/ltablib.c external/lua/lua-5.3.6/src/lutf8lib.c external/lua/lua-5.3.6/src/loadlib.c external/lua/lua-5.3.6/src/linit.c) # 5.3 added lutf8lib
set(lua54_src external/lua/lua-5.4.1/src/lua.c external/lua/lua-5.4.1/src/luac.c external/lua/lua-5.4.1/src/lapi.c external/lua/lua-5.4.1/src/lcode.c external/lua/lua-5.4.1/src/lctype.c external/lua/lua-5.4.1/src/ldebug.c external/lua/lua-5.4.1/src/ldo.c external/lua/lua-5.4.1/src/ldump.c external/lua/lua-5.4.1/src/lfunc.c external/lua/lua-5.4.1/src/lgc.c external/lua/lua-5.4.1/src/llex.c external/lua/lua-5.4.1/src/lmem.c external/lua/lua-5.4.1/src/lobject.c external/lua/lua-5.4.1/src/lopcodes.c external/lua/lua-5.4.1/src/lparser.c external/lua/lua-5.4.1/src/lstate.c external/lua/lua-5.4.1/src/lstring.c external/lua/lua-5.4.1/src/ltable.c external/lua/lua-5.4.1/src/ltm.c external/lua/lua-5.4.1/src/lundump.c external/lua/lua-5.4.1/src/lvm.c external/lua/lua-5.4.1/src/lzio.c external/lua/lua-5.4.1/src/lauxlib.c external/lua/lua-5.4.1/src/lbaselib.c external/lua/lua-5.4.1/src/lcorolib.c external/lua/lua-5.4.1/src/ldblib.c external/lua/lua-5.4.1/src/liolib.c external/lua/lua-5.4.1/src/lmathlib.c external/lua/lua-5.4.1/src/loadlib.c external/lua/lua-5.4.1/src/loslib.c external/lua/lua-5.4.1/src/lstrlib.c external/lua/lua-5.4.1/src/ltablib.c external/lua/lua-5.4.1/src/lutf8lib.c external/lua/lua-5.4.1/src/linit.c)                                          # lbitlib.c removed in 5.4
set(DOKUN_LUA51 OFF CACHE BOOL "Lua 5.1.5")
set(DOKUN_LUA52 OFF CACHE BOOL "Lua 5.2.4")
set(DOKUN_LUA53 OFF CACHE BOOL "Lua 5.3.6")
set(DOKUN_LUA54 OFF CACHE BOOL "Lua 5.4.1")
if(DOKUN_LUA51) # cmake -G"..Makefiles" -DDOKUN_LUA51=ON
    set(lua_src ${lua51_src})
	add_definitions(-DDOKUN_LUA51)
endif()
if(DOKUN_LUA52) # cmake -G"..Makefiles" -DDOKUN_LUA52=ON
    set(lua_src ${lua52_src})
endif()
if(DOKUN_LUA53) # cmake -G"..Makefiles" -DDOKUN_LUA53=ON
    set(lua_src ${lua53_src})
    add_definitions(-DLUA_LIBRARY) # required if using lua 5.3
endif()
if(DOKUN_LUA54) # cmake -G"..Makefiles" -DDOKUN_LUA54=ON
    set(lua_src ${lua54_src})
endif()
if(NOT lua_src)
    message(STATUS "Lua version not specified. Using Lua 5.4 by default")
	set(DOKUN_LUA54 ON)
	set(lua_src ${lua54_src})
endif()
if(LINUX)
add_definitions(-DLUA_USE_POSIX  )
endif()
#############
# GPU
if(WIN32)
    set(opengl_src opengl32.lib glu32.lib)
    set(vulkan_src vulkan-1.lib)
    set(Vulkan_LIBRARY C:/Windows/SysWOW64/vulkan-1.dll) # 64-bit Windows
    set(Vulkan_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/vulkan)
endif()
if(LINUX)
    set(opengl_src libGL.so libGLU.so)
    set(OpenGL_GL_PREFERENCE "LEGACY") # LEGACY(libGL.so) OR GLVND(libOpenGL.so and libGLX.so)
    set(vulkan_src libvulkan.so      )
endif()
#############
# window + screen
set(win32_src)
set(x11_src     libX11.a libxcb.a libXau.a libXdmcp.a)
set(wayland_src libwayland-client.so libwayland-server.so libwayland-cursor.so libwayland-egl.so libEGL.so)
set(cocoa_src)
set(glfw_src external/glfw/src/context.c external/glfw/src/init.c external/glfw/src/input.c external/glfw/src/monitor.c external/glfw/src/vulkan.c external/glfw/src/window.c) # ${common_SOURCES}) # glfw-3.3.5 (https://www.glfw.org/)
if(WINDOWS)
endif()
if(LINUX) # _GLFW_X11
list(APPEND glfw_src external/glfw/src/x11_init.c external/glfw/src/x11_monitor.c external/glfw/src/x11_window.c
                     external/glfw/src/xkb_unicode.c external/glfw/src/posix_time.c external/glfw/src/posix_thread.c external/glfw/src/glx_context.c
                     external/glfw/src/egl_context.c external/glfw/src/osmesa_context.c)
list(APPEND glfw_src external/glfw/src/linux_joystick.c) # for both _GLFW_X11 OR _GLFW_WAYLAND
endif()
# glfw depends on the following libs (on linux): libxrandr-dev libxinerama-dev libxi-dev # glfw requires libxrandr-dev for <X11/extensions/Xrandr.h>, libxinerama-dev for <X11/extensions/Xinerama.h>,libxi-dev for <X11/extensions/XInput2.h>
#############
# input
#############
# extension
set(glew_src  external/glew/src/glew.c) # glew-2.1.0 (this version adds support for OpenGL 4.6) (http://glew.sourceforge.net/)
set(posix_src libpthread.so libdl.so)
set(zlib_src  external/zlib/adler32.c external/zlib/crc32.c external/zlib/deflate.c external/zlib/inflate.c external/zlib/compress.c external/zlib/gzclose.c external/zlib/gzlib.c external/zlib/gzread.c external/zlib/gzwrite.c external/zlib/infback.c external/zlib/inffast.c external/zlib/inftrees.c external/zlib/trees.c external/zlib/uncompr.c external/zlib/zutil.c) # required for libpng
#############
# image
set(png_src  external/png/png.c external/png/pngerror.c external/png/pngget.c external/png/pngmem.c external/png/pngpread.c external/png/pngread.c external/png/pngrio.c external/png/pngrtran.c external/png/pngrutil.c external/png/pngset.c external/png/pngtrans external/png/pngwio.c external/png/pngwrite.c external/png/pngwtran.c external/png/pngwutil.c) # libpng-1.6.35 (http://www.libpng.org/pub/png/libpng.html)(https://libpng.sourceforge.io/)
set(jpeg_src external/jpeg/jaricom.c external/jpeg/jcapimin.c external/jpeg/jcapistd.c external/jpeg/jcarith.c external/jpeg/jccoefct.c external/jpeg/jccolor.c external/jpeg/jcdctmgr.c external/jpeg/jchuff.c external/jpeg/jcinit.c external/jpeg/jcmainct.c external/jpeg/jcmarker.c external/jpeg/jcmaster.c external/jpeg/jcomapi.c external/jpeg/jcparam.c external/jpeg/jcprepct.c external/jpeg/jcsample.c external/jpeg/jctrans.c external/jpeg/jdapimin.c external/jpeg/jdapistd.c external/jpeg/jdarith.c external/jpeg/jdatadst.c external/jpeg/jdatasrc.c external/jpeg/jdcoefct.c external/jpeg/jdcolor.c external/jpeg/jddctmgr.c external/jpeg/jdhuff.c external/jpeg/jdinput.c external/jpeg/jdmainct.c external/jpeg/jdmarker.c external/jpeg/jdmaster.c external/jpeg/jdmerge.c external/jpeg/jdpostct.c external/jpeg/jdsample.c external/jpeg/jdtrans.c external/jpeg/jerror.c external/jpeg/jfdctflt.c external/jpeg/jfdctfst.c external/jpeg/jfdctint.c external/jpeg/jidctflt.c external/jpeg/jidctfst.c external/jpeg/jidctint.c external/jpeg/jquant1.c external/jpeg/jquant2.c external/jpeg/jutils.c external/jpeg/jmemmgr.c) # jpeg-9b (http://ijg.org/) (consider https://libjpeg-turbo.org/ , a "2-6x times" faster fork of libjpeg)
set(gif_src  external/gif/lib/dgif_lib.c external/gif/lib/egif_lib.c external/gif/lib/gif_err.c external/gif/lib/gif_font.c external/gif/lib/gif_hash.c external/gif/lib/gifalloc.c external/gif/lib/openbsd-reallocarray.c external/gif/lib/quantize.c) # giflib-5.1.4 (http://giflib.sourceforge.net/)
set(tiff_src external/tiff/libtiff/tif_aux.c external/tiff/libtiff/tif_close.c external/tiff/libtiff/tif_codec.c external/tiff/libtiff/tif_color.c external/tiff/libtiff/tif_compress.c external/tiff/libtiff/tif_dir.c external/tiff/libtiff/tif_dirinfo.c external/tiff/libtiff/tif_dirread.c external/tiff/libtiff/tif_dirwrite.c external/tiff/libtiff/tif_dumpmode.c external/tiff/libtiff/tif_error.c external/tiff/libtiff/tif_extension.c external/tiff/libtiff/tif_fax3.c external/tiff/libtiff/tif_fax3sm.c external/tiff/libtiff/tif_flush.c external/tiff/libtiff/tif_getimage.c external/tiff/libtiff/tif_jbig.c external/tiff/libtiff/tif_jpeg_12.c external/tiff/libtiff/tif_jpeg.c external/tiff/libtiff/tif_luv.c external/tiff/libtiff/tif_lzma.c external/tiff/libtiff/tif_lzw.c external/tiff/libtiff/tif_next.c external/tiff/libtiff/tif_ojpeg.c external/tiff/libtiff/tif_open.c external/tiff/libtiff/tif_packbits.c external/tiff/libtiff/tif_pixarlog.c external/tiff/libtiff/tif_predict.c external/tiff/libtiff/tif_print.c external/tiff/libtiff/tif_read.c external/tiff/libtiff/tif_stream.cxx external/tiff/libtiff/tif_strip.c external/tiff/libtiff/tif_swab.c external/tiff/libtiff/tif_thunder.c external/tiff/libtiff/tif_tile.c external/tiff/libtiff/tif_version.c external/tiff/libtiff/tif_warning.c external/tiff/libtiff/tif_write.c external/tiff/libtiff/tif_zip.c) #tiff-4.0.7  tiffconf.h is generated with ./configure (http://simplesystems.org/libtiff/)
if(WINDOWS)
list(APPEND tiff_src external/tiff/libtiff/tif_win32.c)
list(APPEND jpeg_src external/jpeg/jmemansi.c         )
endif()
if(LINUX)
list(APPEND tiff_src external/tiff/libtiff/tif_unix.c )
list(APPEND jpeg_src external/jpeg/jmemansi.c         )
endif()
if(MACOSX)
list(APPEND jpeg_src external/jpeg/jmemmac.c          )
endif()
#############
# container - for vorbis and theora
set(ogg_src external/ogg/src/bitwise.c external/ogg/src/framing.c) # libogg-1.3.3  generate config_types.h using ./configure # (https://xiph.org/downloads/)
#############
# audio (codec)
set(vorbis_src external/vorbis/lib/analysis.c external/vorbis/lib/bitrate.c external/vorbis/lib/block.c external/vorbis/lib/codebook.c external/vorbis/lib/envelope.c external/vorbis/lib/floor0.c external/vorbis/lib/floor1.c external/vorbis/lib/info.c external/vorbis/lib/lookup.c external/vorbis/lib/lpc.c external/vorbis/lib/lsp.c external/vorbis/lib/mapping0.c external/vorbis/lib/mdct.c external/vorbis/lib/psy.c external/vorbis/lib/registry.c external/vorbis/lib/res0.c external/vorbis/lib/sharedbook.c external/vorbis/lib/smallft.c external/vorbis/lib/synthesis.c external/vorbis/lib/vorbisenc.c external/vorbis/lib/vorbisfile.c external/vorbis/lib/window.c) # vorbis-1.3.6 (https://xiph.org/downloads/)
set(celt_src   external/opus/celt/bands.c external/opus/celt/celt.c external/opus/celt/celt_decoder.c external/opus/celt/celt_encoder.c external/opus/celt/celt_lpc.c external/opus/celt/cwrs.c external/opus/celt/entcode.c external/opus/celt/entdec.c external/opus/celt/entenc.c external/opus/celt/kiss_fft.c external/opus/celt/laplace.c external/opus/celt/mathops.c external/opus/celt/mdct.c external/opus/celt/modes.c external/opus/celt/pitch.c external/opus/celt/quant_bands.c external/opus/celt/rate.c external/opus/celt/vq.c) # Xiph.Org's Celt
set(silk_src   external/opus/silk/A2NLSF.c external/opus/silk/ana_filt_bank_1.c external/opus/silk/biquad_alt.c external/opus/silk/bwexpander.c external/opus/silk/bwexpander_32.c external/opus/silk/check_control_input.c external/opus/silk/CNG.c external/opus/silk/code_signs.c external/opus/silk/control_audio_bandwidth.c external/opus/silk/control_codec.c external/opus/silk/control_SNR.c external/opus/silk/debug.c external/opus/silk/decoder_set_fs.c external/opus/silk/decode_core.c external/opus/silk/decode_frame.c external/opus/silk/decode_indices.c external/opus/silk/decode_parameters.c external/opus/silk/decode_pitch.c external/opus/silk/decode_pulses.c external/opus/silk/dec_API.c external/opus/silk/encode_indices.c external/opus/silk/encode_pulses.c external/opus/silk/enc_API.c external/opus/silk/gain_quant.c external/opus/silk/HP_variable_cutoff.c external/opus/silk/init_decoder.c external/opus/silk/init_encoder.c external/opus/silk/inner_prod_aligned.c external/opus/silk/interpolate.c external/opus/silk/lin2log.c external/opus/silk/log2lin.c external/opus/silk/LPC_analysis_filter.c external/opus/silk/LPC_inv_pred_gain.c external/opus/silk/LP_variable_cutoff.c external/opus/silk/NLSF2A.c external/opus/silk/NLSF_decode.c external/opus/silk/NLSF_del_dec_quant.c external/opus/silk/NLSF_encode.c external/opus/silk/NLSF_stabilize.c external/opus/silk/NLSF_unpack.c external/opus/silk/NLSF_VQ.c external/opus/silk/NLSF_VQ_weights_laroia.c external/opus/silk/NSQ.c external/opus/silk/NSQ_del_dec.c external/opus/silk/pitch_est_tables.c external/opus/silk/PLC.c external/opus/silk/process_NLSFs.c external/opus/silk/quant_LTP_gains.c external/opus/silk/resampler.c external/opus/silk/resampler_down2.c external/opus/silk/resampler_down2_3.c external/opus/silk/resampler_private_AR2.c external/opus/silk/resampler_private_down_FIR.c external/opus/silk/resampler_private_IIR_FIR.c external/opus/silk/resampler_private_up2_HQ.c external/opus/silk/resampler_rom.c external/opus/silk/shell_coder.c external/opus/silk/sigm_Q15.c external/opus/silk/sort.c external/opus/silk/stereo_decode_pred.c external/opus/silk/stereo_encode_pred.c external/opus/silk/stereo_find_predictor.c external/opus/silk/stereo_LR_to_MS.c external/opus/silk/stereo_MS_to_LR.c external/opus/silk/stereo_quant_pred.c external/opus/silk/sum_sqr_shift.c external/opus/silk/tables_gain.c external/opus/silk/tables_LTP.c external/opus/silk/tables_NLSF_CB_NB_MB.c external/opus/silk/tables_NLSF_CB_WB.c external/opus/silk/tables_other.c external/opus/silk/tables_pitch_lag.c external/opus/silk/tables_pulses_per_block.c external/opus/silk/table_LSF_cos.c external/opus/silk/VAD.c external/opus/silk/VQ_WMat_EC.c     external/opus/silk/float/apply_sine_window_FLP.c external/opus/silk/float/autocorrelation_FLP.c external/opus/silk/float/burg_modified_FLP.c external/opus/silk/float/bwexpander_FLP.c external/opus/silk/float/corrMatrix_FLP.c external/opus/silk/float/encode_frame_FLP.c external/opus/silk/float/energy_FLP.c external/opus/silk/float/find_LPC_FLP.c external/opus/silk/float/find_LTP_FLP.c external/opus/silk/float/find_pitch_lags_FLP.c external/opus/silk/float/find_pred_coefs_FLP.c external/opus/silk/float/inner_product_FLP.c external/opus/silk/float/k2a_FLP.c external/opus/silk/float/levinsondurbin_FLP.c external/opus/silk/float/LPC_analysis_filter_FLP.c external/opus/silk/float/LPC_inv_pred_gain_FLP.c external/opus/silk/float/LTP_analysis_filter_FLP.c external/opus/silk/float/LTP_scale_ctrl_FLP.c external/opus/silk/float/noise_shape_analysis_FLP.c external/opus/silk/float/pitch_analysis_core_FLP.c external/opus/silk/float/prefilter_FLP.c external/opus/silk/float/process_gains_FLP.c external/opus/silk/float/regularize_correlations_FLP.c external/opus/silk/float/residual_energy_FLP.c external/opus/silk/float/scale_copy_vector_FLP.c external/opus/silk/float/scale_vector_FLP.c external/opus/silk/float/schur_FLP.c external/opus/silk/float/solve_LS_FLP.c external/opus/silk/float/sort_FLP.c external/opus/silk/float/warped_autocorrelation_FLP.c external/opus/silk/float/wrappers_FLP.c) # Skype Ltd.'s Silk
set(opus_src   external/opus/src/analysis.c external/opus/src/mlp.c external/opus/src/mlp_data.c external/opus/src/opus.c external/opus/src/opus_decoder.c external/opus/src/opus_encoder.c external/opus/src/opus_multistream.c external/opus/src/opus_multistream_decoder.c external/opus/src/opus_multistream_encoder.c external/opus/src/repacketizer.c ${celt_src} ${silk_src})
add_definitions(-DUSE_ALLOCA)
add_definitions(-DOPUS_BUILD)
set(flac_src external/flac/src/libFLAC/cpu.c external/flac/src/libFLAC/crc.c external/flac/src/libFLAC/lpc.c external/flac/src/libFLAC/md5.c external/flac/src/libFLAC/fixed.c external/flac/src/libFLAC/float.c external/flac/src/libFLAC/format.c external/flac/src/libFLAC/memory.c external/flac/src/libFLAC/window.c external/flac/src/libFLAC/bitmath.c external/flac/src/libFLAC/bitreader.c external/flac/src/libFLAC/bitwriter.c external/flac/src/libFLAC/ogg_helper.c external/flac/src/libFLAC/ogg_mapping.c external/flac/src/libFLAC/lpc_intrin_sse.c external/flac/src/libFLAC/stream_decoder.c external/flac/src/libFLAC/stream_encoder.c external/flac/src/libFLAC/lpc_intrin_avx2.c external/flac/src/libFLAC/lpc_intrin_sse2.c external/flac/src/libFLAC/metadata_object.c external/flac/src/libFLAC/lpc_intrin_sse41.c external/flac/src/libFLAC/fixed_intrin_sse2.c external/flac/src/libFLAC/fixed_intrin_ssse3.c external/flac/src/libFLAC/metadata_iterators.c external/flac/src/libFLAC/ogg_decoder_aspect.c external/flac/src/libFLAC/ogg_encoder_aspect.c external/flac/src/libFLAC/stream_encoder_framing.c external/flac/src/libFLAC/windows_unicode_filenames.c external/flac/src/libFLAC/stream_encoder_intrin_avx2.c external/flac/src/libFLAC/stream_encoder_intrin_sse2.c external/flac/src/libFLAC/stream_encoder_intrin_ssse3.c) # flac-1.3.3       # flac - a command-line program for encoding and decoding files # libFLAC - for native Ogg Flac and Flac #libFlac++ - a c++ object wrapper # https://xiph.org/flac/license.html
if(NOT WIN32)
    list(REMOVE_ITEM flac_src "external/flac/src/libFLAC/windows_unicode_filenames.c")
endif()
add_definitions(-DFLAC__NO_DLL -DHAVE_LROUND -DFLAC__HAS_OGG)
#############
# video (codec)
set(theora_src external/theora/lib/analyze.c external/theora/lib/apiwrapper.c external/theora/lib/bitpack.c external/theora/lib/cpu.c external/theora/lib/decapiwrapper.c external/theora/lib/decinfo.c external/theora/lib/decode.c external/theora/lib/dequant.c external/theora/lib/encapiwrapper.c external/theora/lib/encfrag.c external/theora/lib/encinfo.c external/theora/lib/encode.c external/theora/lib/enquant.c external/theora/lib/fdct.c external/theora/lib/fragment.c external/theora/lib/huffdec.c external/theora/lib/huffenc.c external/theora/lib/idct.c external/theora/lib/info.c external/theora/lib/internal.c external/theora/lib/mathops.c external/theora/lib/mcenc.c external/theora/lib/quant.c external/theora/lib/rate.c external/theora/lib/state.c external/theora/lib/tokenize.c) # libtheora-1.1.1 (https://xiph.org/downloads/)
#############
# video (playback)
#############
# font
set(freetype_src external/freetype/src/autofit/autofit.c external/freetype/src/base/ftbase.c external/freetype/src/base/ftbbox.c external/freetype/src/base/ftbdf.c external/freetype/src/base/ftbitmap.c external/freetype/src/base/ftcid.c external/freetype/src/base/ftfstype.c external/freetype/src/base/ftgasp.c external/freetype/src/base/ftglyph.c external/freetype/src/base/ftgxval.c external/freetype/src/base/ftinit.c external/freetype/src/base/ftmm.c external/freetype/src/base/ftotval.c external/freetype/src/base/ftpatent.c external/freetype/src/base/ftpfr.c external/freetype/src/base/ftstroke.c external/freetype/src/base/ftsynth.c external/freetype/src/base/ftsystem.c external/freetype/src/base/fttype1.c external/freetype/src/base/ftwinfnt.c external/freetype/src/bdf/bdf.c external/freetype/src/bzip2/ftbzip2.c external/freetype/src/cache/ftcache.c external/freetype/src/cff/cff.c external/freetype/src/cid/type1cid.c external/freetype/src/gzip/ftgzip.c external/freetype/src/lzw/ftlzw.c external/freetype/src/pcf/pcf.c external/freetype/src/pfr/pfr.c external/freetype/src/psaux/psaux.c external/freetype/src/pshinter/pshinter.c external/freetype/src/psnames/psnames.c external/freetype/src/raster/raster.c external/freetype/src/sfnt/sfnt.c external/freetype/src/smooth/smooth.c external/freetype/src/truetype/truetype.c external/freetype/src/type1/type1.c external/freetype/src/type42/type42.c external/freetype/src/winfonts/winfnt.c #) #freetype-2.10.1 on 2019-07-01 => freetype-2.11.0 on 2022-01-28 (https://www.freetype.org/download.html) (https://download.savannah.gnu.org/releases/freetype/)
# 2.10.2
#external/freetype/src/sfnt/sfwoff2.c external/freetype/src/sfnt/woff2tags.c
# 2.11.0
external/freetype/src/dlg/dlg.c external/freetype/src/dlg/dlgwrap.c external/freetype/src/sdf/ftbsdf.c external/freetype/src/sdf/ftsdf.c external/freetype/src/sdf/ftsdfcommon.c #[[external/freetype/src/sdf/ftsdfrend.c]] external/freetype/src/sdf/sdf.c
)
if(WIN32)
    list(APPEND freetype_src external/freetype/builds/windows/ftdebug.c external/freetype/src/base/ftver.rc) #add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
endif()
if(LINUX)
    list(APPEND freetype_src external/freetype/src/base/ftdebug.c) # fixes the undefined reference to `FT_Trace_Disable' error in Ubuntu
endif()
add_definitions(-DFT2_BUILD_LIBRARY) # define 'DLL_EXPORT' for freetype2 sharedlib (.dll). Other defines: -DFT_WITH_ZLIB=ON, -DFT_WITH_BZIP2=ON, -DFT_WITH_PNG=ON
# added in 2.10.2: src/sfnt/sfwoff2.c, src/sfnt/woff2tags.c,     # removed in 2.10.3: include/freetype/internal/internal.h
# added in 2.11.0: builds/windows/ftsystem.c, src/dlg/dlg.c, src/dlg/dlgwrap.c, src/sdf/ftbsdf.c, src/sdf/ftsdf.c, src/sdf/ftsdfcommon.c, src/sdf/ftsdfrend.c, src/sdf/sdf.c, tests/issue-1063/main.c
#############
# audio (playback)
if(WIN32)
set(openal_src ${CMAKE_CURRENT_SOURCE_DIR}/src/OpenAL32.lib) # openal-soft (https://kcat.strangesoft.net/openal.html) (LGPL-licensed)
endif()
if(LINUX)
set(openal_src libopenal.so)
endif()
#############
# unicode
set(icu_src)
#############
if(LINUX)
   include_directories(/usr/include             )
   link_directories   (/usr/lib/x86_64-linux-gnu)
endif()
#############
# include_dir (.h and .hpp)
if(DOKUN_LUA51)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lua/lua-5.1.5/src)
endif()
if(DOKUN_LUA52)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lua/lua-5.2.4/src)
endif()
if(DOKUN_LUA53)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lua/lua-5.3.6/src)
endif()
if(DOKUN_LUA54)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lua/lua-5.4.1/src)
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies
   ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include 
   ${CMAKE_CURRENT_SOURCE_DIR}/external/png
   ${CMAKE_CURRENT_SOURCE_DIR}/external/zlib
   ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include                   # public headers - ft2build.h
   ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include/freetype          # public headers
   ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include/freetype/config   # public config headers
   ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include/freetype/internal # private headers - added 2019-07-29
   ${CMAKE_CURRENT_SOURCE_DIR}/external/ogg/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/vorbis/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/vorbis/lib
   ${CMAKE_CURRENT_SOURCE_DIR}/external/theora/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite
   #[[${CMAKE_CURRENT_SOURCE_DIR}/external/utf8/source]]
   ${CMAKE_CURRENT_SOURCE_DIR}/external/openal/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/openal-soft/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/jpeg
   ${CMAKE_CURRENT_SOURCE_DIR}/external/jpeg/jpeg-6b
   ${CMAKE_CURRENT_SOURCE_DIR}/external/tiff/libtiff
   ${CMAKE_CURRENT_SOURCE_DIR}/external/gif/lib
   #${CMAKE_CURRENT_SOURCE_DIR}/external/flac # config.h
   ${CMAKE_CURRENT_SOURCE_DIR}/external/flac/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/flac/src/libFLAC/include # for "private/cpu.h"
   #[[${CMAKE_CURRENT_SOURCE_DIR}/external/opus/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/opus/celt # Xiph.Org's Celt
   ${CMAKE_CURRENT_SOURCE_DIR}/external/opus/silk # Skype Limited's Silk
   ${CMAKE_CURRENT_SOURCE_DIR}/external/opus/silk/fixed
   ${CMAKE_CURRENT_SOURCE_DIR}/external/opus/silk/float
   ${CMAKE_CURRENT_SOURCE_DIR}/external/opus/silk/mips
   ${CMAKE_CURRENT_SOURCE_DIR}/external/opus/win32
   ${CMAKE_CURRENT_SOURCE_DIR}/external/theoraplay
   ${CMAKE_CURRENT_SOURCE_DIR}/external/theoraplayer/include]]
   ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
   #[[${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2/SDL2-2.0.4/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/portaudio/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/json/src]]
   ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
   #${CMAKE_CURRENT_SOURCE_DIR}/external/openssl/include
)
#############
# static_library_dir (.a and .lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src) # stored in 'src' since libraries are precompiled sources
#############
#############
# API definitions
add_definitions(-DDOKUN_OPENGL) # add_definitions(-DDOKUN_VULKAN)
add_definitions(-DDOKUN_OPENAL)
if(DOKUN_GLFW) # cmake -G"Unix Makefiles" -DDOKUN_GLFW=ON
    message("Using GLFW for window creation")
if(LINUX)
    add_definitions(-D_GLFW_X11)
if(DOKUN_OPENGL)
    add_definitions(-D_GLFW_GLX)
    add_definitions(-D_GLFW_USE_OPENGL)
endif()
endif()
    #list(REMOVE_ITEM dokun_ui_src src/window.cpp)
    list(APPEND dokun_ui_src ${glfw_src} #[[src/window_glfw.cpp]])
endif()
add_definitions(-DDOKUN_DEBUG ) # dokun debug mode. Use -DDOKUN_DEBUG0 for more intense debugging#add_definitions(-DDOKUN_WAYLAND)
#############
# output folder - '.a' and '.lib' will go in folder "src"
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}    )
#############
# build dokun-ui    # add_executable(dokun_ui           ${dokun_ui_src} ${lua_src} ${png_src} ${gif_src} ${jpeg_src} ${tiff_src} ${ogg_src} ${vorbis_src} ${flac_src} ${theora_src} ${freetype_src} ${zlib_src} ${glew_src} ${dokun_res}              )
add_library   (libdokun-ui STATIC ${dokun_ui_src} ${lua_src} ${png_src} ${gif_src} ${jpeg_src} ${tiff_src} ${ogg_src} ${vorbis_src} ${flac_src} ${theora_src} ${freetype_src} ${zlib_src} ${glew_src}                           )
#############
# set properties
#set_target_properties(dokun_ui    PROPERTIES COMPILE_DEFINITIONS "DOKUN_BUILD_CONSOLE")
set_target_properties(libdokun-ui PROPERTIES COMPILE_DEFINITIONS "DOKUN_BUILD_LIBRARY")
set_target_properties(libdokun-ui PROPERTIES PREFIX ""                                ) # remove extra 'lib' prefix
#############
#############
#[[ # libraries don't need to be linked to anything
# on a Windows platform (tested with MinGW, g++ 5.3.0)
if(WIN32)
	find_package(OPENGL)
	if(OPENGL_FOUND) 
	    target_link_libraries(dokun-ui  ${opengl_src})
	endif()
	#find_package(Vulkan) # optional
	if(Vulkan_FOUND)
	    target_link_libraries(dokun-ui  ${vulkan_src})
	endif()
	target_link_libraries(dokun-ui  ${winsocket_src})
	target_link_libraries(dokun-ui  ${openal_src}   )
endif(WIN32)]]
#############
#[[ # on a Linux platform (tested on Ubuntu with g++-4.8)
if(LINUX)
    find_package(X11)
    if(X11_FOUND)
	    target_link_libraries(dokun-ui  ${x11_src})
    endif()
	find_package(OpenGL)
    if(OPENGL_FOUND)
	    target_link_libraries(dokun-ui  ${opengl_src})
    endif()
	#find_package(Vulkan) # optional # will not link to Vulkan unless 'find_package' is used and Vulkan is found
    if(Vulkan_FOUND)
	    target_link_libraries(dokun-ui  ${vulkan_src})
    endif()
    find_package(OpenAL REQUIRED)
	if(OPENAL_FOUND)
		target_link_libraries(dokun-ui  ${openal_src})
	endif()
	target_link_libraries(dokun-ui  ${posix_src})
endif(LINUX)]]
#############
#[[ 
if(MACOSX)
endif (MACOSX)
#############
if(ANDROID)
endif(ANDROID)
]]
#############
