#include "../include/video.hpp"

Video::Video()
{}
///////////
Video::Video(const std::string& file_name)
{
	if(!load(file_name))
		Logger("Could not load video!");
}
///////////
Video::~Video()
{}
///////////
bool Video::load(std::string file_name, int vidfmt)
{
	return load_ogg(file_name, vidfmt);
}
///////////
int Video::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->load(lua_tostring(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
///////////
void Video::play()
{}
int Video::play(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->play();
	}
	return 0;
}
///////////
void Video::pause()
{}
int Video::pause(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->pause();
	}
	return 0;
}
///////////
void Video::stop()
{}
int Video::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->stop();
	}
	return 0;
}
///////////
void Video::record(int source)
{
	switch(source)
	{
		case 0:
		    record_screen();
			break;
		case 1:
		    record_camera();
			break;
		default:
            record_screen();
            break;
	}
}
int Video::record(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->record(lua_tonumber(L, 2));
	}
	return 0;
}
///////////
void Video::screenshot(int window_width, int window_height, std::string file_name)
{
    unsigned char * data = new unsigned char[window_width * window_height * 4]; // 4 channels per pixel (RGBA)
    if(data == nullptr) {
		Logger("Could not capture window");
		return;
	}
#ifdef DOKUN_OPENGL
    glReadPixels(static_cast<GLint>(0), static_cast<GLint>(0),
		static_cast<GLsizei>(window_width), static_cast<GLsizei>(window_height),
		GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLvoid *>(data));
#endif
    FILE * file = fopen(file_name.c_str(), "wb");
    if(file == nullptr)
	{
		Logger("Could not capture window");
		delete data;
		return;
	}
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr); // write_struct
    if (!png_ptr) {
		Logger("Could not capture window");
		delete data;
		fclose(file);
		return;
	}
    png_infop info_ptr = png_create_info_struct(png_ptr); // info_struct
    if (!info_ptr) {
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, nullptr);
        delete data;
        fclose(file);
        return;
	}
    if (setjmp(png_jmpbuf(png_ptr)))  // png error stuff
	{
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        delete data;
        fclose(file);
		return;
    }
	// init png writing
	png_init_io(png_ptr, file);
    // write header - store color, depth, etc.
	int bit_depth  = 8;
	int color_type = 6;
    png_set_IHDR(png_ptr, info_ptr, window_width, window_height,
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	// write all image info
	png_write_info(png_ptr, info_ptr);
	//--------------------------
	// Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	// row pointer - create output data
	png_bytep * data_p = new png_bytep[window_height];
	if (!data_p)
	{
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(file);
	    return;
    }
	for(int i=0; i < window_height; ++i)
	{
		data_p[window_height - 1 - i] = (png_byte*)data + i * rowbytes;
	}
	//---------------------------
	png_write_image(png_ptr, (png_bytepp)data_p); // write byte data
	png_write_end(png_ptr, nullptr); // stop writing
	// clean up time!
    png_destroy_write_struct(&png_ptr, &info_ptr);	// destroy both write and info struct //if (png_ptr && info_ptr) {}
    delete [] data;// delete pixel data
	fclose(file); // close file
	Logger("Screenshot taken");
}
int Video::screenshot(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->screenshot(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
///////////
///////////
Texture * Video::get_framebuffer_texture(int window_width, int window_height)
{
    unsigned char * data = new unsigned char[window_width * window_height * 4]; // 4 channels per pixel (RGBA)
    if(data == nullptr) {Logger("Could not capture window");return nullptr;}
#ifdef DOKUN_OPENGL
    glReadPixels(static_cast<GLint>(0), static_cast<GLint>(0), static_cast<GLsizei>(window_width), static_cast<GLsizei>(window_height), GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLvoid *>(data));
#endif
    Texture * texture = new Texture(data, window_width, window_height, 1, 4); // store framebuffer in texture// save texture so can delete later
    return texture;
}
///////////
///////////
int Video::get_width()
{
	return width;
}
int Video::get_width (lua_State *L)
{
    return 1;
}
///////////
int Video::get_height()
{
	return height;
}
int Video::get_height(lua_State *L)
{
    return 1;
}
///////////
void * Video::get_data()
{
	return &data[0];
}
void * Video::get_audio_data()
{
    return nullptr;
}
///////////
double Video::get_fps()
{
	return fps;
}
int Video::get_fps(lua_State *L)
{
    return 1;
}
///////////
int Video::get_format()
{
	return format;
}
int Video::get_format(lua_State *L)
{
    return 1;
}
///////////
///////////
bool Video::load_ogg(const std::string& file_name, int vidfmt)
{
/*	
    //TheoraVideoManager *mgr=new TheoraVideoManager();
	const THEORAPLAY_VideoFrame  * video = 0;
    const THEORAPLAY_AudioPacket * audio = 0;
	THEORAPLAY_Decoder * decoder = THEORAPLAY_startDecodeFile(file_name.c_str(),
        20, // maxframes
        static_cast<THEORAPLAY_VideoFormat>(vidfmt));
	while(THEORAPLAY_isDecoding(decoder)) {
		video = THEORAPLAY_getVideo(decoder);
		if(video != nullptr)
		{
			Logger("Retrieved video frame " + std::to_string(video->playms) + "ms"); // 120 frames
			Logger(std::to_string(video->fps) + "= fps\n"    +  // sample.ogg is 30 frames per second
			    std::to_string(video->width ) + "= width\n"  +  // 320
				std::to_string(video->height) + "= height\n" +  // 240
				std::to_string(video->format) + "= format\n" +  // 0
			    String(pixels).str() + "\n"
				);
			// get width, height, frames, etc.    3966ms(last)
			playms = video->playms;
			fps    = video->fps;
			width  = video->width ;
			height = video->height;
			format = video->format;
			pixels = video->pixels;
			data.push_back(video->pixels); // 120 = size
			std::cout << data.size() << "= size of data\n";
			// free the video
			THEORAPLAY_freeVideo(video);
		} // if
        audio = THEORAPLAY_getAudio(decoder);
        if(audio != nullptr)
        {
			Logger("Retrieved " + std::to_string(audio->frames) + " frames of audio " + std::to_string(audio->playms) + "ms");
            THEORAPLAY_freeAudio(audio);
        } // if
		if (!video && !audio)
            dokun::sleep(10);
	} // while

	if (THEORAPLAY_decodingError(decoder))
	{
		Logger("Error decoding theora!");
		return false;
	}
	THEORAPLAY_stopDecode(decoder);
*/	
///////////////////////////////////////////

	return true;
}
///////////
void Video::record_screen()
{}
void Video::record_camera()
{}
///////////
///////////
///////////
void Video::encode(const std::string& file_name)
{}
///////////
// decode stuff
/*
// Helper; just grab some more compressed bitstream and sync it for
//   page extraction
int buffer_data(ogg_sync_state *oy){
  char *buffer=ogg_sync_buffer(oy,4096);
  int bytes=fread(buffer,1,4096,file);
  ogg_sync_wrote(oy,bytes);
  return(bytes);
}

// helper: push a page into the appropriate stream
// this can be done blindly; a stream won't accept a page
//                that doesn't belong to it
static int queue_page(ogg_page *page){
  if(theora_p)ogg_stream_pagein(&to,page);
  if(vorbis_p)ogg_stream_pagein(&vo,page);
  return 0;
}
*/
///////////
void Video::video_write(void){ // https://www.theora.org/doc/libtheora-1.1/structyuv__buffer.html
  /* taken from player_sample.c test file for theora alpha */
  int i;
  yuv_buffer yuv;
  int crop_offset;
  theora_decode_YUVout(&td,&yuv);
  // let's draw the data (*yuv[3]) on a SDL screen (*screen) /
  // deal with border stride /
  // reverse u and v for SDL /
  // and crop input properly, respecting the encoded frame rect /
  /*crop_offset=ti.offset_x+yuv.y_stride*ti.offset_y;
  for(i=0;i<yuv_overlay->h;i++)
    memcpy(yuv_overlay->pixels[0]+yuv_overlay->pitches[0]*i,
	   yuv.y+crop_offset+yuv.y_stride*i,
	   yuv_overlay->w);
  crop_offset=(ti.offset_x/2)+(yuv.uv_stride)*(ti.offset_y/2);
  for(i=0;i<yuv_overlay->h/2;i++){
    memcpy(yuv_overlay->pixels[1]+yuv_overlay->pitches[1]*i,
	   yuv.v+crop_offset+yuv.uv_stride*i,
	   yuv_overlay->w/2);
    memcpy(yuv_overlay->pixels[2]+yuv_overlay->pitches[2]*i,
	   yuv.u+crop_offset+yuv.uv_stride*i,
	   yuv_overlay->w/2);
  }*/
  // Show, baby, show!
  //SDL_DisplayYUVOverlay(yuv_overlay, &rect);*/
}
///////////
void Video::decode(const std::string& file_name)
{
    FILE * file = fopen(file_name.c_str(), "rb");
    if(!file) {Logger("Could not open file: " + file_name, "error"); return;}
    // start up Ogg stream synchronization layer
    ogg_sync_init(&oy);
    // init supporting Vorbis structures needed in header parsing
    vorbis_info_init(&vi);
    vorbis_comment_init(&vc);
    // init supporting Theora structures needed in header parsing
    theora_comment_init(&tc);
    theora_info_init(&ti);
    //void parseHeaders(){
  // extracted from player_sample.c test file for theora alpha
  ogg_packet op;
  // Parse the headers
  // Only interested in Vorbis/Theora streams
  while(!stateflag){
    //int buffer_data(ogg_sync_state *oy){
      char *buffer=(char *)ogg_sync_buffer(&oy,4096);
      int bytes=fread(buffer,1,4096,file);
      (ogg_sync_state *)ogg_sync_wrote(&oy,bytes); // ???
      //return(bytes);
      int ret=(int)bytes;//buffer_data(&oy); // replacement  
    //}  
    if(ret==0)break;
    while(ogg_sync_pageout(&oy,&og)>0){
      ogg_stream_state test;

      // is this a mandated initial header? If not, stop parsing
      if(!ogg_page_bos(&og)){
	// don't leak the page; get it into the appropriate stream
	//static int queue_page(ogg_page *page){
        if(theora_p)ogg_stream_pagein(&to,&og); // page changed to og
        if(vorbis_p)ogg_stream_pagein(&vo,&og); // page changed og
        //return 0;
    //}
	//queue_page(&og);
	stateflag=1;
	break;
      }

      ogg_stream_init(&test,ogg_page_serialno(&og));
      ogg_stream_pagein(&test,&og);
      ogg_stream_packetout(&test,&op);

      // identify the codec: try theora
      if(!theora_p && theora_decode_header(&ti,&tc,&op)>=0){
	// it is theora
	memcpy(&to,&test,sizeof(test));
	theora_p=1;
      }else if(!vorbis_p && vorbis_synthesis_headerin(&vi,&vc,&op)>=0){
	// it is vorbis /
	memcpy(&vo,&test,sizeof(test));
	vorbis_p=1;
      }else{
	// whatever it is, we don't care about it /
	ogg_stream_clear(&test);
      }
    }
  }

  // we've now identified all the bitstreams. parse the secondary header packets. /
  while((theora_p && theora_p<3) || (vorbis_p && vorbis_p<3)){
    int ret;

    // look for further theora headers /
    while(theora_p && (theora_p<3) && (ret=ogg_stream_packetout(&to,&op))){
      if(ret<0){
      	printf("Error parsing Theora stream headers; corrupt stream?\n");
      	exit(1);
      }
      if(theora_decode_header(&ti,&tc,&op)){
        printf("Error parsing Theora stream headers; corrupt stream?\n");
        exit(1);
      }
      theora_p++;
      if(theora_p==3)break;
    }

    // look for more vorbis header packets /
    while(vorbis_p && (vorbis_p<3) && (ret=ogg_stream_packetout(&vo,&op))){
      if(ret<0){
	printf("Error parsing Vorbis stream headers; corrupt stream?\n");
	exit(1);
      }
      if(vorbis_synthesis_headerin(&vi,&vc,&op)){
	printf("Error parsing Vorbis stream headers; corrupt stream?\n");
	exit(1);
      }
      vorbis_p++;
      if(vorbis_p==3)break;
    }

    // The header pages/packets will arrive before anything else we
    //   care about, or the stream is not obeying spec /

    if(ogg_sync_pageout(&oy,&og)>0){
	//static int queue_page(ogg_page *page){
        if(theora_p)ogg_stream_pagein(&to,&og);
        if(vorbis_p)ogg_stream_pagein(&vo,&og);
        //return 0;
    //}
	    //queue_page(&og); // demux into the appropriate stream /
    }else{
    //int buffer_data(ogg_sync_state *oy){
      char *buffer=(char *)ogg_sync_buffer(&oy,4096);
      int bytes=fread(buffer,1,4096,file);
      (ogg_sync_state *)ogg_sync_wrote(&oy,bytes); // ???
      //return(bytes);    
      int ret=(int)bytes;//buffer_data(&oy);
      if(ret==0){
	fprintf(stderr,"End of file while searching for codec headers.\n");
	exit(1);
      }
    }
  }
//}
    // initialize decoders
    theora_decode_init(&td, &ti);
    printf("Ogg logical stream %x is Theora %dx%d %.02f fps video\n"
           "  Frame content is %dx%d with offset (%d,%d).\n",
	   to.serialno,ti.width,ti.height, (double)ti.fps_numerator/ti.fps_denominator,
	   ti.frame_width, ti.frame_height, ti.offset_x, ti.offset_y);
	//report_colorspace(theora_info *ti);
	{
        switch(ti.colorspace){
          case OC_CS_UNSPECIFIED:
            // nothing to report
            break;;
          case OC_CS_ITU_REC_470M:
            fprintf(stderr,"  encoder specified ITU Rec 470M color.\n");
            break;;
          case OC_CS_ITU_REC_470BG:
            fprintf(stderr,"  encoder specified ITU Rec 470BG color.\n");
            break;;
          default:
            fprintf(stderr,"warning: encoder specified unknown colorspace (%d).\n",
            ti.colorspace);
            break;;
        }
    }
    //dump_comments(theora_comment *tc);
    {
        int i, len;
        char *value;

        printf("Encoded by %s\n",tc.vendor);
        if(tc.comments){
          printf("theora comment header:\n");
          for(i=0;i<tc.comments;i++){
            if(tc.user_comments[i]){
              len=tc.comment_lengths[i];
      	      value=(char*)malloc(len+1);
      	      memcpy(value,tc.user_comments[i],len);
      	      value[len]='\0';
      	      printf("\t%s\n", value);
      	      free(value);
            }
          }
        }
        //return(0);    
    }
	// open video 
    if(theora_p){//open_video();
  //  static void open_video(void){
  // taken from player_sample.c test file for theora alpha

  //if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
  //  printf("Unable to initialize SDL: %s\n", SDL_GetError());
  //  exit(1);
  //}
  //screen = SDL_SetVideoMode(ti.frame_width, ti.frame_height, 0, SDL_SWSURFACE);
  //if ( screen == NULL ) {
  //  printf("Unable to set %dx%d video mode: %s\n",
  //         ti.frame_width,ti.frame_height,SDL_GetError());
  //  exit(1);
  //}
  // ----
  this->texture = new Texture(nullptr, ti.frame_width, ti.frame_height); // empty texture
  // convert RGB pixels to YUV12 pixels
  // set texture properties
  texture->set_internal_format(GL_LUMINANCE);
  texture->set_format(GL_LUMINANCE);
  texture->set_filter(GL_LINEAR, GL_LINEAR);
  texture->set_wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
 /* 
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
		glGenTextures(1, static_cast<GLuint *>(&buffer)); // generate a buffer
		glBindTexture(target, static_cast<GLuint>(buffer)); // bind buffer
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
        glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
        glTexImage2D(target, level, static_cast<GLint>(internal_format), static_cast<GLsizei>(get_width()), static_cast<GLsizei>(get_height()), 0, static_cast<GLenum>(get_format()), GL_UNSIGNED_BYTE, static_cast<GLvoid *>(get_data())); // pass texture width, height, and data to OpenGL
        glGenerateMipmap(target); // generate mipmaps              glGenerateTextureMipmap(static_cast<GLuint>(buffer)); // <- this is only in OpenGL 4.5
		glBindTexture(target, 0); // unbind buffer  */
  // ----
  
  //yuv_overlay = SDL_CreateYUVOverlay(ti.frame_width, ti.frame_height,
  //		     SDL_YV12_OVERLAY,
  //			     screen);
  //if ( yuv_overlay == NULL ) {
  //  Logger("Couldn't create yuv_overlay: ", "error");
  //  exit(1);
  //}
  //rect.x = 0;
  //rect.y = 0;
  //rect.w = ti.frame_width;
  //rect.h = ti.frame_height;
  
  //SDL_DisplayYUVOverlay(yuv_overlay, &rect);
  }
  	/*
	/////////////////////
	// our main loop
int i,j;
  ogg_packet op;
  SDL_Event event;
  int hasdatatobuffer = 1;
  int playbackdone = 0;
  double now, delay, last_frame_time = 0;

  int frameNum=0;
  int skipNum=0;	
  
  while(!playbackdone){
    // break out on SDL quit event
    if(window.is_open())
    {
      
    }
    while(theora_p && !videobuf_ready){
      // get one video packet...
      if(ogg_stream_packetout(&to,&op)>0){

        theora_decode_packetin(&td,&op);

	  videobuf_granulepos=td.granulepos;
	  videobuf_time=theora_granule_time(&td,videobuf_granulepos);
	  // update the frame counter
	  frameNum++;

	  // check if this frame time has not passed yet.
	  //   If the frame is late we need to decode additonal
	  //   ones and keep looping, since theora at this stage
	  //   needs to decode all frames
	  now=get_time();
	  delay=videobuf_time-now;
	  if(delay>=0.0){
		// got a good frame, not late, ready to break out
		videobuf_ready=1;
	  }else if(now-last_frame_time>=1.0){
		// display at least one frame per second, regardless
		videobuf_ready=1;
	  }else{
		fprintf(stderr, "dropping frame %d (%.3fs behind)\n",
			frameNum, -delay);
	   }
      }else{
	// need more data
	break;
      }
    }

    if(!hasdatatobuffer && !videobuf_ready && !audiobuf_ready){
      isPlaying = 0;
      playbackdone = 1;
    }

    // if we're set for the next frame, sleep
    if((!theora_p || videobuf_ready) &&
       (!vorbis_p || audiobuf_ready)){
        int ticks = 1.0e3*(videobuf_time-get_time());
	if(ticks>0)
          SDL_Delay(ticks);
    }

    if(videobuf_ready){
      // time to write our cached frame
      video_write();
      videobuf_ready=0;
      last_frame_time=get_time();

      // if audio has not started (first frame) then start it
      //if ((!isPlaying)&&(vorbis_p)){
      //  start_audio();
      //  isPlaying = 1;
      }
    }
  }  
	*/
    // Clean up
    if(theora_p){
    ogg_stream_clear(&to);
    theora_clear(&td);
    theora_comment_clear(&tc);
    theora_info_clear(&ti);
    } // end of clean_theora
    ogg_sync_clear(&oy);   
	fclose(file);   
}
///////////
///////////
void Video::png_to_theora()
{}
///////////
///////////
int Video::new_(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
	lua_settop(L, 0); // clear stack

	lua_createtable(L, 0, 0); // create table
	lua_getglobal(L, "Video");
	lua_setmetatable(L, 1); // set metatable
	// set userdata
    Video ** video = static_cast<Video **>(lua_newuserdata(L, sizeof(Video *)));
    if(!file_name.empty())
	{
		*video = new Video(file_name);
	}
    if(file_name.empty())
	{
		*video = new Video();
	}
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	{
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
