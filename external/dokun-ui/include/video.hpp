#ifndef VIDEO_HPP_DOKUN
#define VIDEO_HPP_DOKUN // new!

#include "logger.hpp"
#include "utility.hpp"
#include "string.hpp"
#include "texture.hpp"
#include "audio.hpp"

#ifdef __cplusplus // if c++
extern "C" {      // run as c code
#endif	
    #include <theora/theora.h> // video codec (theora)
    #include <theora/theoradec.h>
    #include <theora/theoraenc.h>
    //#include <png.h>     // libpng (png)	
#ifdef __cplusplus
}
#endif	

#ifdef __cplusplus // if c++
#include <iostream>
#include <vector>
#include <lua.hpp>

class Video // a cinema class; useful for video game cutscenes and animations
{
public:
	Video();                             static int new_(lua_State *L);
	Video(const std::string& file_name);
	~Video();                        
	bool load(std::string file_name, int vidfmt=0);    static int load(lua_State *L);
	//void draw();	                     static int draw(lua_State *L);
	void play();                         static int play(lua_State *L);
	void pause();                        static int pause(lua_State *L);
	void stop();                         static int stop(lua_State *L);
	// you can record the game or your camera
	static void record(int source);             static int record(lua_State *L); 
	static void screenshot(int window_width, int window_height, std::string file_name="output.png");    static int screenshot(lua_State *L);
	// getters
	int get_width();                     static int get_width (lua_State *L);
	int get_height();                    static int get_height(lua_State *L);
	double get_fps();                    static int get_fps(lua_State *L);
	int get_format();                    static int get_format(lua_State *L);
	void * get_data();                   static int get_data(lua_State *L);
	void * get_audio_data();
	static Texture * get_framebuffer_texture(int window_width, int window_height);
private:
	bool load_ogg(const std::string& file_name, int vidfmt); // loads a theora
	static void record_screen();  
	static void record_camera();
public:	
	// encoder and decoder
	void encode(const std::string& file_name);
	void decode(const std::string& file_name);
	void video_write();
    // exporter
	void png_to_theora(); 
	// properties
	std::vector<unsigned char*> data;
	void * audio_data();
	// video
    unsigned int playms; // playback time
    double fps;
    unsigned int width;
    unsigned int height;
    unsigned int format;
    unsigned char *pixels;
    std::string file;
	// video recording
	bool is_recording;
	//  audio
    unsigned int aplayms;  /* playback start time in milliseconds. */
    int channels;
    int freq;
    int frames;
    float *samples;  /* frames * channels float32 samples. */
    // texture
    Texture * texture;
    /* Ogg and codec state for demux/decode */
    ogg_sync_state   oy;
    ogg_page         og;
    ogg_stream_state vo;
    ogg_stream_state to;
    theora_info      ti;
    theora_comment   tc;
    theora_state     td;
    vorbis_info      vi;
    vorbis_dsp_state vd;
    vorbis_block     vb;
    vorbis_comment   vc;
    int              theora_p=0;
    int              vorbis_p=0;
    int              stateflag=0;
    /* single frame video buffering */
    int          videobuf_ready=0;
    ogg_int64_t  videobuf_granulepos=-1;
    double       videobuf_time=0;

    int          audiobuf_ready=0;
    ogg_int64_t  audiobuf_granulepos=0; /* time position of last sample */    
    
}; // -lopenvx -lvxu
#endif
#endif
/*
std::cout << "libtheora version: " << th_version_string () << std::endl; // prints: Xiph.Org libtheora 1.1 20090822 (Thusnelda)
std::cout << "libtheora version: " << th_version_number () << std::endl; // prints: 197121

// print bitstream version:
unsigned version = th_version_number();

    fprintf(stdout, "Bitstream: %d.%d.%d (0x%06X)\n",
        (version >> 16) & 0xff, (version >> 8) & 0xff, (version) & 0xff,
        version);
===========================================================================        
// print version string
 const char *version = th_version_string();

    if (version == NULL) {
      fprintf(stderr, "Error querying libtheora version string.\n");
      return -1;
    }

    fprintf(stdout, "Version: %s\n", version);        
=========================================================================== 
Generate a dummy encoder context for use in th_encode_ctl queries (encoding = writing):
th_enc_ctx *dummy_encode_ctx(void)
{
    th_enc_ctx *ctx;
    th_info info;

    // set the minimal video parameters
    th_info_init(&info);
    info.frame_width=320;
    info.frame_height=240;
    info.fps_numerator=1;
    info.fps_denominator=1;

    // allocate and initialize a context object
    ctx = th_encode_alloc(&info);
    if (ctx == NULL) {
        fprintf(stderr, "Error allocating encoder context.\n");
    }

    // clear the info struct
    th_info_clear(&info);

    return ctx;
}
=========================================================================== 
// Query the current and maximum values for the 'speed level' setting.
//   This can be used to ask the encoder to trade off encoding quality
//   vs. performance cost, for example to adapt to realtime constraints.
int check_speed_level(th_enc_ctx *ctx, int *current, int *max)
{
    int ret;

    // query the current speed level
    ret = th_encode_ctl(ctx, TH_ENCCTL_GET_SPLEVEL, current, sizeof(int));
    if (ret) {
        fprintf(stderr, "Error %d getting current speed level.\n", ret);
        return ret;
    }
    // query the maximum speed level, which varies by encoder version
    ret = th_encode_ctl(ctx, TH_ENCCTL_GET_SPLEVEL_MAX, max, sizeof(int));
    if (ret) {
        fprintf(stderr, "Error %d getting max speed level.\n", ret);
        return ret;
    }

    return 0;
}
=========================================================================== 
// Print the current and maximum speed level settings
int print_speed_level(th_enc_ctx *ctx)
{
    int current = -1;
    int max = -1;
    int ret;

    ret = check_speed_level(ctx, &current, &max);
    if (ret == 0) {
        fprintf(stdout, "Default speed level: %d\n", current);
        fprintf(stdout, "Maximum speed level: %d\n", max);
    }

    return ret;
}
===========================================================================
int main(int argc, char **argv) {
    th_enc_ctx *ctx;

    // print versioning
    print_version_string();
    print_version();

    // allocate a generic context for queries that require it
    ctx = dummy_encode_ctx();
    if (ctx != NULL) {
        // dump the speed level setting
        print_speed_level(ctx);
        // clean up
        th_encode_free(ctx);
    }

    return 0;
}
 
=========================================================================== 

Functions for encoding
You must link to libtheoraenc and libtheoradec if you use any of the functions in this section.

The functions are listed in the order they are used in a typical encode. The basic steps are:

Fill in a th_info structure with details on the format of the video you wish to encode.
Allocate a th_enc_ctx handle with th_encode_alloc().
Perform any additional encoder configuration required with th_encode_ctl().
Repeatedly call th_encode_flushheader() to retrieve all the header packets.
For each uncompressed frame:
Submit the uncompressed frame via th_encode_ycbcr_in()
Repeatedly call th_encode_packetout() to retrieve any video data packets that are ready.
Call th_encode_free() to release all encoder memory.


th_enc_ctx * 	th_encode_alloc (const th_info *_info)
 	Allocates an encoder instance.
int 	th_encode_ctl (th_enc_ctx *_enc, int _req, void *_buf, size_t _buf_sz)
 	Encoder control function.
int 	th_encode_flushheader (th_enc_ctx *_enc, th_comment *_comments, ogg_packet *_op)
 	Outputs the next header packet.
int 	th_encode_ycbcr_in (th_enc_ctx *_enc, th_ycbcr_buffer _ycbcr)
 	Submits an uncompressed frame to the encoder.
int 	th_encode_packetout (th_enc_ctx *_enc, int _last, ogg_packet *_op)
 	Retrieves encoded video data packets.
void 	th_encode_free (th_enc_ctx *_enc)
 	Frees an allocated encoder instance.
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
=========================================================================== 
        
===========================================================================         
=========================================================================== 
=========================================================================== 
*/
