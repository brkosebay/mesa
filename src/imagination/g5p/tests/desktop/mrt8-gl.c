/* SPDX-License-Identifier: MIT
 * Generated-by: LLM
 * Eight simultaneous colour outputs. Real pixel comparisons; no fake-node PASS mode.
 * Uses a compatibility2.1 context so integer-RT/GS qualification does not gate this independent rung.
 */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/glcorearb.h>
#include <stdio.h>
#include <string.h>

#define FUNCS(X) \
 X(PFNGLUNIFORM4FPROC,Uniform4f) X(PFNGLGETUNIFORMLOCATIONPROC,GetUniformLocation) X(PFNGLGETSTRINGPROC,GetString) X(PFNGLGETINTEGERVPROC,GetIntegerv) X(PFNGLGETERRORPROC,GetError) \
 X(PFNGLGENFRAMEBUFFERSPROC,GenFramebuffers) X(PFNGLBINDFRAMEBUFFERPROC,BindFramebuffer) \
 X(PFNGLDELETEFRAMEBUFFERSPROC,DeleteFramebuffers) X(PFNGLCHECKFRAMEBUFFERSTATUSPROC,CheckFramebufferStatus) \
 X(PFNGLGENTEXTURESPROC,GenTextures) X(PFNGLBINDTEXTUREPROC,BindTexture) X(PFNGLTEXIMAGE2DPROC,TexImage2D) \
 X(PFNGLDELETETEXTURESPROC,DeleteTextures) X(PFNGLFRAMEBUFFERTEXTURE2DPROC,FramebufferTexture2D) \
 X(PFNGLDRAWBUFFERSPROC,DrawBuffers) X(PFNGLREADBUFFERPROC,ReadBuffer) X(PFNGLREADPIXELSPROC,ReadPixels) \
 X(PFNGLCREATESHADERPROC,CreateShader) X(PFNGLSHADERSOURCEPROC,ShaderSource) X(PFNGLCOMPILESHADERPROC,CompileShader) \
 X(PFNGLGETSHADERIVPROC,GetShaderiv) X(PFNGLGETSHADERINFOLOGPROC,GetShaderInfoLog) X(PFNGLDELETESHADERPROC,DeleteShader) \
 X(PFNGLCREATEPROGRAMPROC,CreateProgram) X(PFNGLATTACHSHADERPROC,AttachShader) X(PFNGLBINDATTRIBLOCATIONPROC,BindAttribLocation) \
 X(PFNGLLINKPROGRAMPROC,LinkProgram) X(PFNGLGETPROGRAMIVPROC,GetProgramiv) X(PFNGLGETPROGRAMINFOLOGPROC,GetProgramInfoLog) \
 X(PFNGLUSEPROGRAMPROC,UseProgram) X(PFNGLDELETEPROGRAMPROC,DeleteProgram) \
 X(PFNGLGENBUFFERSPROC,GenBuffers) X(PFNGLBINDBUFFERPROC,BindBuffer) X(PFNGLBUFFERDATAPROC,BufferData) \
 X(PFNGLDELETEBUFFERSPROC,DeleteBuffers) X(PFNGLENABLEVERTEXATTRIBARRAYPROC,EnableVertexAttribArray) \
 X(PFNGLVERTEXATTRIBPOINTERPROC,VertexAttribPointer) X(PFNGLDRAWARRAYSPROC,DrawArrays) \
 X(PFNGLVIEWPORTPROC,Viewport) X(PFNGLENABLEPROC,Enable) X(PFNGLDISABLEPROC,Disable) \
 X(PFNGLSCISSORPROC,Scissor) X(PFNGLCLEARCOLORPROC,ClearColor) X(PFNGLCLEARPROC,Clear)
#define DECL(type,name) static type gl##name;
FUNCS(DECL)
#undef DECL

static GLuint compile(GLenum stage, const char *source)
{
 GLuint shader=glCreateShader(stage);glShaderSource(shader,1,&source,NULL);glCompileShader(shader);
 GLint ok=0;glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
 if(!ok){char log[4096]={0};glGetShaderInfoLog(shader,sizeof(log),NULL,log);fprintf(stderr,"MRT8 compile: %s\n",log);return 0;}
 return shader;
}

static unsigned run_count(unsigned count,int record_only)
{
 const char *vs="#version 120\nattribute vec2 position;void main(){gl_Position=vec4(position,0,1);}";
 char fs[2048]="#version 120\nuniform vec4 source;void main(){\n";size_t at=strlen(fs);
 for(unsigned t=0;t<count;t++)at+=(size_t)snprintf(fs+at,sizeof(fs)-at,"gl_FragData[%u]=source*vec4(%u.0,%u.0,%u.0,1.0);\n",t,t&1,(t>>1)&1,(t>>2)&1);
 snprintf(fs+at,sizeof(fs)-at,"}\n");
 GLuint vertex=compile(GL_VERTEX_SHADER,vs),fragment=compile(GL_FRAGMENT_SHADER,fs);if(!vertex||!fragment)return 1;
 GLuint program=glCreateProgram();glAttachShader(program,vertex);glAttachShader(program,fragment);glBindAttribLocation(program,0,"position");glLinkProgram(program);
 GLint linked=0;glGetProgramiv(program,GL_LINK_STATUS,&linked);if(!linked){char log[4096]={0};glGetProgramInfoLog(program,sizeof(log),NULL,log);fprintf(stderr,"MRT8 link %s\n",log);return 1;}
 glDeleteShader(vertex);glDeleteShader(fragment);
 GLuint fbo,textures[8],vbo;GLenum draw[8];glGenFramebuffers(1,&fbo);glBindFramebuffer(GL_FRAMEBUFFER,fbo);glGenTextures(count,textures);
 for(unsigned t=0;t<count;t++){
  glBindTexture(GL_TEXTURE_2D,textures[t]);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,16,16,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+t,GL_TEXTURE_2D,textures[t],0);draw[t]=GL_COLOR_ATTACHMENT0+t;
 }
 glDrawBuffers(count,draw);
 if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){fprintf(stderr,"MRT8 RESULT FAIL framebuffer GL=%#x\n",glGetError());return 1;}
 const GLfloat vertices[]={-1,-1,3,-1,-1,3};glGenBuffers(1,&vbo);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
 glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);glEnableVertexAttribArray(0);glUseProgram(program);glViewport(0,0,16,16);glDisable(GL_DITHER);
 /* Every attachment clear is black/transparent; the left half gets its distinct binary RGB plus alpha1. */
 glClearColor(0.125f,0.25f,0.5f,0.75f);glClear(GL_COLOR_BUFFER_BIT);
 unsigned failed=0;
 for(unsigned phase=0;phase<4;phase++){
 if(phase==3){glUniform4f(glGetUniformLocation(program,"source"),0.25f,0.5f,0.75f,1.0f);glDrawArrays(GL_TRIANGLES,0,3);}
 else if(phase){glUniform4f(glGetUniformLocation(program,"source"),1,1,1,1);glEnable(GL_SCISSOR_TEST);glScissor(phase==1?0:12,0,phase==1?8:4,16);glDrawArrays(GL_TRIANGLES,0,3);glDisable(GL_SCISSOR_TEST);}
 /* Readback ends rendering before the next phase; the second draw must preserve prior contents. */
 for(unsigned t=0;t<count;t++){
  unsigned char got[16*16*4];memset(got,0xa5,sizeof(got));glReadBuffer(GL_COLOR_ATTACHMENT0+t);glReadPixels(0,0,16,16,GL_RGBA,GL_UNSIGNED_BYTE,got);
  GLenum error=glGetError();unsigned bad=0;
  for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++)for(unsigned c=0;c<4;c++){
   int coloured=phase==3||(phase>=1&&x<8)||(phase==2&&x>=12);
   const unsigned char clear[4]={32,64,128,191},modulated[4]={64,128,191,255};
   unsigned char want=coloured?((c==3||((t>>c)&1))?(phase==3?modulated[c]:255):0):clear[c];
   if(!record_only&&got[(y*16+x)*4+c]!=want){if(!bad)fprintf(stderr,"MRT8 mismatch count=%u phase=%u target=%u xy=%u,%u channel=%u expected=%u got=%u\n",count,phase,t,x,y,c,want,got[(y*16+x)*4+c]);bad++;}
  }
  /* Full vectors make swapped/half-packed or missing high attachments visible. */
  for(unsigned probe=0;probe<3;probe++) {
   unsigned x=probe==0?0:probe==1?9:15;int coloured=phase==3||(phase>=1&&x<8)||(phase==2&&x>=12);
   const unsigned char clear[4]={32,64,128,191},modulated[4]={64,128,191,255};unsigned char want[4];
   for(unsigned c=0;c<4;c++)want[c]=coloured?((c==3||((t>>c)&1))?(phase==3?modulated[c]:255):0):clear[c];
   printf("MRT8-VECTOR count=%u phase=%u target=%u xy=%u,0 expected=%02x,%02x,%02x,%02x got=%02x,%02x,%02x,%02x judgement=%s\n",count,phase,t,x,want[0],want[1],want[2],want[3],got[4*x],got[4*x+1],got[4*x+2],got[4*x+3],record_only?"NONE":memcmp(want,got+4*x,4)?"FAIL":"PASS");
  }
  printf("RUNG MRT8-n%u-%s-target%u %s pixels=256 bad_channels=%u GL=%#x\n",count,phase==0?"clear":phase==1?"draw":phase==2?"preserve":"fractional",t,!bad&&!error?record_only?"RECORDED":"PASS":"FAIL",bad,error);failed+=!!bad||!!error;
 }
 }
 glDeleteBuffers(1,&vbo);glDeleteProgram(program);glDeleteTextures(count,textures);glDeleteFramebuffers(1,&fbo);
 return failed;
}

int main(int argc,char **argv)
{
 setvbuf(stdout,NULL,_IOLBF,0);
 int record_only=argc==2&&!strcmp(argv[1],"--record-only");
 if(argc!=1&&!record_only)return 2;
 PFNEGLGETPLATFORMDISPLAYEXTPROC platform=(void*)eglGetProcAddress("eglGetPlatformDisplayEXT");
 EGLDisplay display=platform?platform(EGL_PLATFORM_SURFACELESS_MESA,NULL,NULL):EGL_NO_DISPLAY;
 if(display==EGL_NO_DISPLAY||!eglInitialize(display,NULL,NULL)||!eglBindAPI(EGL_OPENGL_API))return 1;
 const EGLint attrs[]={EGL_CONTEXT_MAJOR_VERSION,2,EGL_CONTEXT_MINOR_VERSION,1,EGL_NONE};
 EGLContext ctx=eglCreateContext(display,EGL_NO_CONFIG_KHR,EGL_NO_CONTEXT,attrs);
 if(ctx==EGL_NO_CONTEXT||!eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,ctx)){
  fprintf(stderr,"MRT8 RESULT FAIL context EGL=%#x\n",eglGetError());return 1;}
#define LOAD(type,name) gl##name=(type)eglGetProcAddress("gl" #name);if(!gl##name){fprintf(stderr,"missing gl%s\n",#name);return 1;}
 FUNCS(LOAD)
#undef LOAD
 printf("MRT8 GL_VERSION=%s renderer=%s\n",glGetString(GL_VERSION),glGetString(GL_RENDERER));
 GLint buffers=0,attachments=0;glGetIntegerv(GL_MAX_DRAW_BUFFERS,&buffers);glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,&attachments);
 printf("MRT8 limits draw_buffers=%d colour_attachments=%d\n",buffers,attachments);
 if(buffers<8||attachments<8){puts("MRT8 RESULT MISSING eight simultaneous targets are not advertised");return 77;}
 unsigned failed=0;
 for(unsigned count=5;count<=8;count++)failed+=run_count(count,record_only);
 eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(display,ctx);eglTerminate(display);
 printf("MRT8 RESULT %s rungs=%u/104 pixels=26624\n",failed?"FAIL":record_only?"RECORDED":"PASS",104-failed);return !!failed;
}
