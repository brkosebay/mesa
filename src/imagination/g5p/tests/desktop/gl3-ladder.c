/* SPDX-License-Identifier: MIT
 * Generated-by: LLM
 * Desktop GL acceptance ladder. Real pixels only; never a fake-node PASS.
 * Build: cc -O1 -Wall -Wextra gl3-ladder.c -o gl3-ladder -lEGL -lGL
 */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/glcorearb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* GL dispatch comes from this EGL implementation, with no libGL dependency. */
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
static PFNGLBEGINQUERYPROC glBeginQuery;
static PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
static PFNGLBINDTEXTUREPROC glBindTexture;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
static PFNGLCLEARPROC glClear;
static PFNGLCLEARBUFFERIVPROC glClearBufferiv;
static PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
static PFNGLCLEARCOLORPROC glClearColor;
static PFNGLCOLORMASKPROC glColorMask;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLCREATESHADERPROC glCreateShader;
static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLDELETEQUERIESPROC glDeleteQueries;
static PFNGLDELETESHADERPROC glDeleteShader;
static PFNGLDELETETEXTURESPROC glDeleteTextures;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
static PFNGLDISABLEPROC glDisable;
static PFNGLDRAWARRAYSPROC glDrawArrays;
static PFNGLENABLEPROC glEnable;
static PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
static PFNGLENDQUERYPROC glEndQuery;
static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
static PFNGLGENQUERIESPROC glGenQueries;
static PFNGLGENTEXTURESPROC glGenTextures;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
static PFNGLGETERRORPROC glGetError;
static PFNGLGETPROGRAMIVPROC glGetProgramiv;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
static PFNGLGETSHADERIVPROC glGetShaderiv;
static PFNGLGETSTRINGPROC glGetString;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLREADPIXELSPROC glReadPixels;
static PFNGLSCISSORPROC glScissor;
static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLTEXIMAGE2DPROC glTexImage2D;
static PFNGLUNIFORM4IVPROC glUniform4iv;
static PFNGLUNIFORM4UIVPROC glUniform4uiv;
static PFNGLUSEPROGRAMPROC glUseProgram;
static PFNGLVIEWPORTPROC glViewport;
static PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
static PFNGLBINDBUFFERPROC glBindBuffer;
static PFNGLBUFFERDATAPROC glBufferData;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
static PFNGLGENBUFFERSPROC glGenBuffers;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
static void load_gl(void) {
   glBindAttribLocation = (void*)eglGetProcAddress("glBindAttribLocation"); if (!glBindAttribLocation) exit(1);
   glBindBuffer = (void*)eglGetProcAddress("glBindBuffer"); if (!glBindBuffer) exit(1);
   glBufferData = (void*)eglGetProcAddress("glBufferData"); if (!glBufferData) exit(1);
   glDeleteBuffers = (void*)eglGetProcAddress("glDeleteBuffers"); if (!glDeleteBuffers) exit(1);
   glEnableVertexAttribArray = (void*)eglGetProcAddress("glEnableVertexAttribArray"); if (!glEnableVertexAttribArray) exit(1);
   glGenBuffers = (void*)eglGetProcAddress("glGenBuffers"); if (!glGenBuffers) exit(1);
   glVertexAttribPointer = (void*)eglGetProcAddress("glVertexAttribPointer"); if (!glVertexAttribPointer) exit(1);
   glAttachShader = (void *)eglGetProcAddress("glAttachShader"); if (!glAttachShader) { fputs("missing glAttachShader\n",stderr); exit(1); }
   glBeginConditionalRender = (void *)eglGetProcAddress("glBeginConditionalRender"); if (!glBeginConditionalRender) { fputs("missing glBeginConditionalRender\n",stderr); exit(1); }
   glBeginQuery = (void *)eglGetProcAddress("glBeginQuery"); if (!glBeginQuery) { fputs("missing glBeginQuery\n",stderr); exit(1); }
   glBindFragDataLocation = (void *)eglGetProcAddress("glBindFragDataLocation"); if (!glBindFragDataLocation) { fputs("missing glBindFragDataLocation\n",stderr); exit(1); }
   glBindFramebuffer = (void *)eglGetProcAddress("glBindFramebuffer"); if (!glBindFramebuffer) { fputs("missing glBindFramebuffer\n",stderr); exit(1); }
   glBindTexture = (void *)eglGetProcAddress("glBindTexture"); if (!glBindTexture) { fputs("missing glBindTexture\n",stderr); exit(1); }
   glBindVertexArray = (void *)eglGetProcAddress("glBindVertexArray"); if (!glBindVertexArray) { fputs("missing glBindVertexArray\n",stderr); exit(1); }
   glCheckFramebufferStatus = (void *)eglGetProcAddress("glCheckFramebufferStatus"); if (!glCheckFramebufferStatus) { fputs("missing glCheckFramebufferStatus\n",stderr); exit(1); }
   glClear = (void *)eglGetProcAddress("glClear"); if (!glClear) { fputs("missing glClear\n",stderr); exit(1); }
   glClearBufferiv = (void *)eglGetProcAddress("glClearBufferiv"); if (!glClearBufferiv) { fputs("missing glClearBufferiv\n",stderr); exit(1); }
   glClearBufferuiv = (void *)eglGetProcAddress("glClearBufferuiv"); if (!glClearBufferuiv) { fputs("missing glClearBufferuiv\n",stderr); exit(1); }
   glClearColor = (void *)eglGetProcAddress("glClearColor"); if (!glClearColor) { fputs("missing glClearColor\n",stderr); exit(1); }
   glColorMask = (void *)eglGetProcAddress("glColorMask"); if (!glColorMask) { fputs("missing glColorMask\n",stderr); exit(1); }
   glCompileShader = (void *)eglGetProcAddress("glCompileShader"); if (!glCompileShader) { fputs("missing glCompileShader\n",stderr); exit(1); }
   glCreateProgram = (void *)eglGetProcAddress("glCreateProgram"); if (!glCreateProgram) { fputs("missing glCreateProgram\n",stderr); exit(1); }
   glCreateShader = (void *)eglGetProcAddress("glCreateShader"); if (!glCreateShader) { fputs("missing glCreateShader\n",stderr); exit(1); }
   glDeleteFramebuffers = (void *)eglGetProcAddress("glDeleteFramebuffers"); if (!glDeleteFramebuffers) { fputs("missing glDeleteFramebuffers\n",stderr); exit(1); }
   glDeleteProgram = (void *)eglGetProcAddress("glDeleteProgram"); if (!glDeleteProgram) { fputs("missing glDeleteProgram\n",stderr); exit(1); }
   glDeleteQueries = (void *)eglGetProcAddress("glDeleteQueries"); if (!glDeleteQueries) { fputs("missing glDeleteQueries\n",stderr); exit(1); }
   glDeleteShader = (void *)eglGetProcAddress("glDeleteShader"); if (!glDeleteShader) { fputs("missing glDeleteShader\n",stderr); exit(1); }
   glDeleteTextures = (void *)eglGetProcAddress("glDeleteTextures"); if (!glDeleteTextures) { fputs("missing glDeleteTextures\n",stderr); exit(1); }
   glDeleteVertexArrays = (void *)eglGetProcAddress("glDeleteVertexArrays"); if (!glDeleteVertexArrays) { fputs("missing glDeleteVertexArrays\n",stderr); exit(1); }
   glDisable = (void *)eglGetProcAddress("glDisable"); if (!glDisable) { fputs("missing glDisable\n",stderr); exit(1); }
   glDrawArrays = (void *)eglGetProcAddress("glDrawArrays"); if (!glDrawArrays) { fputs("missing glDrawArrays\n",stderr); exit(1); }
   glEnable = (void *)eglGetProcAddress("glEnable"); if (!glEnable) { fputs("missing glEnable\n",stderr); exit(1); }
   glEndConditionalRender = (void *)eglGetProcAddress("glEndConditionalRender"); if (!glEndConditionalRender) { fputs("missing glEndConditionalRender\n",stderr); exit(1); }
   glEndQuery = (void *)eglGetProcAddress("glEndQuery"); if (!glEndQuery) { fputs("missing glEndQuery\n",stderr); exit(1); }
   glFramebufferTexture2D = (void *)eglGetProcAddress("glFramebufferTexture2D"); if (!glFramebufferTexture2D) { fputs("missing glFramebufferTexture2D\n",stderr); exit(1); }
   glGenFramebuffers = (void *)eglGetProcAddress("glGenFramebuffers"); if (!glGenFramebuffers) { fputs("missing glGenFramebuffers\n",stderr); exit(1); }
   glGenQueries = (void *)eglGetProcAddress("glGenQueries"); if (!glGenQueries) { fputs("missing glGenQueries\n",stderr); exit(1); }
   glGenTextures = (void *)eglGetProcAddress("glGenTextures"); if (!glGenTextures) { fputs("missing glGenTextures\n",stderr); exit(1); }
   glGenVertexArrays = (void *)eglGetProcAddress("glGenVertexArrays"); if (!glGenVertexArrays) { fputs("missing glGenVertexArrays\n",stderr); exit(1); }
   glGetError = (void *)eglGetProcAddress("glGetError"); if (!glGetError) { fputs("missing glGetError\n",stderr); exit(1); }
   glGetProgramiv = (void *)eglGetProcAddress("glGetProgramiv"); if (!glGetProgramiv) { fputs("missing glGetProgramiv\n",stderr); exit(1); }
   glGetShaderInfoLog = (void *)eglGetProcAddress("glGetShaderInfoLog"); if (!glGetShaderInfoLog) { fputs("missing glGetShaderInfoLog\n",stderr); exit(1); }
   glGetShaderiv = (void *)eglGetProcAddress("glGetShaderiv"); if (!glGetShaderiv) { fputs("missing glGetShaderiv\n",stderr); exit(1); }
   glGetString = (void *)eglGetProcAddress("glGetString"); if (!glGetString) { fputs("missing glGetString\n",stderr); exit(1); }
   glGetUniformLocation = (void *)eglGetProcAddress("glGetUniformLocation"); if (!glGetUniformLocation) { fputs("missing glGetUniformLocation\n",stderr); exit(1); }
   glLinkProgram = (void *)eglGetProcAddress("glLinkProgram"); if (!glLinkProgram) { fputs("missing glLinkProgram\n",stderr); exit(1); }
   glReadPixels = (void *)eglGetProcAddress("glReadPixels"); if (!glReadPixels) { fputs("missing glReadPixels\n",stderr); exit(1); }
   glScissor = (void *)eglGetProcAddress("glScissor"); if (!glScissor) { fputs("missing glScissor\n",stderr); exit(1); }
   glShaderSource = (void *)eglGetProcAddress("glShaderSource"); if (!glShaderSource) { fputs("missing glShaderSource\n",stderr); exit(1); }
   glTexImage2D = (void *)eglGetProcAddress("glTexImage2D"); if (!glTexImage2D) { fputs("missing glTexImage2D\n",stderr); exit(1); }
   glUniform4iv = (void *)eglGetProcAddress("glUniform4iv"); if (!glUniform4iv) { fputs("missing glUniform4iv\n",stderr); exit(1); }
   glUniform4uiv = (void *)eglGetProcAddress("glUniform4uiv"); if (!glUniform4uiv) { fputs("missing glUniform4uiv\n",stderr); exit(1); }
   glUseProgram = (void *)eglGetProcAddress("glUseProgram"); if (!glUseProgram) { fputs("missing glUseProgram\n",stderr); exit(1); }
   glViewport = (void *)eglGetProcAddress("glViewport"); if (!glViewport) { fputs("missing glViewport\n",stderr); exit(1); }
}

static int verify=1;
static void fail(const char *s) { fprintf(stderr, "FAIL %s GL=%#x\n", s, glGetError()); exit(1); }
static GLuint shader(GLenum type, const char *s) {
   GLuint sh=glCreateShader(type); glShaderSource(sh,1,&s,NULL); glCompileShader(sh);
   GLint ok; glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);
   if (!ok) { char log[4096]; glGetShaderInfoLog(sh,sizeof(log),NULL,log); fprintf(stderr,"%s\n",log); fail("compile"); }
   return sh;
}
static GLuint program(const char *fs, int clip) {
   const char *vs=clip ?
      "#version 130\nin vec2 position;void main(){vec2 p=position;gl_Position=vec4(p,0,1);gl_ClipDistance[0]=p.x;}" :
      "#version 130\nin vec2 position;void main(){vec2 p=position;gl_Position=vec4(p,0,1);}";
   GLuint v=shader(GL_VERTEX_SHADER,vs), f=shader(GL_FRAGMENT_SHADER,fs), p=glCreateProgram();
   glAttachShader(p,v); glAttachShader(p,f); glBindFragDataLocation(p,0,"color"); glBindAttribLocation(p,0,"position"); glLinkProgram(p);
   GLint ok; glGetProgramiv(p,GL_LINK_STATUS,&ok); if(!ok) fail("link");
   glDeleteShader(v); glDeleteShader(f); return p;
}
static GLuint target(GLenum format) {
   GLuint t; glGenTextures(1,&t); glBindTexture(GL_TEXTURE_2D,t);
   GLenum type=format==GL_RGBA32UI ? GL_UNSIGNED_INT : format==GL_RGBA32I ? GL_INT : GL_UNSIGNED_BYTE;
   glTexImage2D(GL_TEXTURE_2D,0,format,16,16,0,format==GL_RGBA8 ? GL_RGBA : GL_RGBA_INTEGER,type,NULL);
   glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,t,0);
   if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) fail("integer/colour FBO unsupported");
   return t;
}
static void integer_case(int sign) {
   GLuint t=target(sign ? GL_RGBA32I : GL_RGBA32UI);
   GLuint p=program(sign ? "#version 130\nuniform ivec4 value;out ivec4 color;void main(){color=value;}" :
      "#version 130\nuniform uvec4 value;out uvec4 color;void main(){color=value;}",0);
   const uint32_t values[4]={0x80000001,0x7fffffff,0x12345678,0xffffffff};
   const uint32_t clear[4]={0,17,0x10001,0x76543210}; uint32_t got[16*16*4];
   if(sign) glClearBufferiv(GL_COLOR,0,(const GLint*)clear); else glClearBufferuiv(GL_COLOR,0,clear);
   /* Force the raw clear to retire; the draw must use the typed LOAD background. */
   glReadPixels(0,0,16,16,GL_RGBA_INTEGER,sign ? GL_INT : GL_UNSIGNED_INT,got);
   if(glGetError()!=GL_NO_ERROR) fail("integer clear readback");
   if(verify) for(int i=0;i<256;i++) for(int c=0;c<4;c++)
      if(got[i*4+c]!=clear[c]) {
         fprintf(stderr,"INTEGER-MISMATCH phase=clear type=%s pixel=%d,%d channel=%d expected=%08x got=%08x\n",
                 sign ? "sint32" : "uint32",i%16,i/16,c,clear[c],got[i*4+c]);
         for(int q=0;q<4;q++)fprintf(stderr,"INTEGER-CHANNEL channel=%d expected=%08x got=%08x\n",q,clear[q],got[i*4+q]);
         fail("raw integer clear");
      }
   glUseProgram(p); GLint u=glGetUniformLocation(p,"value");
   if(sign) glUniform4iv(u,1,(const GLint*)values); else glUniform4uiv(u,1,values);
   /* Partial draw must preserve untouched pixels and masked components. */
   glEnable(GL_SCISSOR_TEST); glScissor(8,0,8,16); glColorMask(GL_TRUE,GL_FALSE,GL_TRUE,GL_FALSE);
   glDrawArrays(GL_TRIANGLES,0,3); glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); glDisable(GL_SCISSOR_TEST);
   glReadPixels(0,0,16,16,GL_RGBA_INTEGER,sign ? GL_INT : GL_UNSIGNED_INT,got);
   if(glGetError()!=GL_NO_ERROR) fail("integer draw/readback");
   for(int y=0;y<16;y++) for(int x=0;x<16;x++) for(int c=0;c<4;c++) {
      uint32_t want=(x>=8 && !(c&1)) ? values[c] : clear[c];
      if(verify && got[(y*16+x)*4+c]!=want) {
         fprintf(stderr,"INTEGER-MISMATCH phase=draw-mask-load type=%s pixel=%d,%d channel=%d expected=%08x got=%08x\n",
                 sign ? "sint32" : "uint32",x,y,c,want,got[(y*16+x)*4+c]);
         for(int q=0;q<4;q++)fprintf(stderr,"INTEGER-CHANNEL channel=%d expected=%08x got=%08x\n",q,
                     (x>=8 && !(q&1)) ? values[q] : clear[q],got[(y*16+x)*4+q]);
         fail(sign ? "signed bits/mask/LOAD" : "unsigned bits/mask/LOAD");
      }
   }
   glDeleteProgram(p); glDeleteTextures(1,&t); printf("RUNG %s %s\n",sign ? "sint32" : "uint32",verify ? "PASS pixels" : "RECORDED");
}
static void pixels(int half) {
   unsigned char got[16*16*4]; glReadPixels(0,0,16,16,GL_RGBA,GL_UNSIGNED_BYTE,got);
   if(glGetError()!=GL_NO_ERROR) fail("colour readback");
   for(int y=0;y<16;y++) for(int x=0;x<16;x++) for(int c=0;c<4;c++) {
      int want=c==3 ? 255 : c==0 && (!half || x>=8) ? 255 : 0;
      if(verify && got[(y*16+x)*4+c]!=want) fail("clip/conditional pixels");
   }
}
static void clip_conditional(void) {
   GLuint t=target(GL_RGBA8);
   const char *fs="#version 130\nout vec4 color;void main(){color=vec4(1,0,0,1);}";
   GLuint p=program(fs,1); glUseProgram(p); glClearColor(0,0,0,1); glClear(GL_COLOR_BUFFER_BIT);
   glEnable(GL_CLIP_DISTANCE0); glDrawArrays(GL_TRIANGLES,0,3); glDisable(GL_CLIP_DISTANCE0); pixels(1);
   puts("RUNG clip-distance PASS"); glDeleteProgram(p); p=program(fs,0); glUseProgram(p);
   GLuint q; glGenQueries(1,&q);
   for(int visible=0;visible<2;visible++) {
      glClear(GL_COLOR_BUFFER_BIT); glEnable(GL_SCISSOR_TEST); glScissor(0,0,visible ? 16 : 0,16);
      glBeginQuery(GL_SAMPLES_PASSED,q); glDrawArrays(GL_TRIANGLES,0,3); glEndQuery(GL_SAMPLES_PASSED);
      glDisable(GL_SCISSOR_TEST); glClear(GL_COLOR_BUFFER_BIT);
      glBeginConditionalRender(q,GL_QUERY_WAIT); glDrawArrays(GL_TRIANGLES,0,3); glEndConditionalRender();
      unsigned char got[16*16*4]; glReadPixels(0,0,16,16,GL_RGBA,GL_UNSIGNED_BYTE,got);
      for(int i=0;i<256;i++) for(int c=0;c<4;c++)
         if(verify && got[i*4+c]!=(c==3 || (c==0 && visible) ? 255 : 0)) fail("conditional zero/nonzero");
      if(glGetError()!=GL_NO_ERROR) fail("conditional GL error");
   }
   puts("RUNG conditional PASS"); glDeleteQueries(1,&q); glDeleteProgram(p); glDeleteTextures(1,&t);
}
int main(int argc,char **argv) {
   int integers_only=0, uint_only=0, sint_only=0;
   for(int i=3;i<argc;i++) {
      if(!strcmp(argv[i],"--no-verify")) verify=0;
      else if(!strcmp(argv[i],"--integers-only")) integers_only=1;
      else if(!strcmp(argv[i],"--uint32-only")) { integers_only=1;uint_only=1; }
      else if(!strcmp(argv[i],"--sint32-only")) { integers_only=1;sint_only=1; }
      else return 2;
   }
   int minor=argc>1 ? atoi(argv[1]) : 1; if(minor!=0 && minor!=1) return 2;
   PFNEGLGETPLATFORMDISPLAYEXTPROC get=(void*)eglGetProcAddress("eglGetPlatformDisplayEXT");
   EGLDisplay d=get ? get(EGL_PLATFORM_SURFACELESS_MESA,EGL_DEFAULT_DISPLAY,NULL) : EGL_NO_DISPLAY;
   if(d==EGL_NO_DISPLAY || !eglInitialize(d,NULL,NULL) || !eglBindAPI(EGL_OPENGL_API)) return 1;
   int compat=argc>2 && !strcmp(argv[2],"compat");
   EGLint a[]={EGL_CONTEXT_MAJOR_VERSION,3,EGL_CONTEXT_MINOR_VERSION,minor,
      EGL_CONTEXT_OPENGL_PROFILE_MASK,compat ? EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT : EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,EGL_NONE};
   EGLContext ctx=eglCreateContext(d,EGL_NO_CONFIG_KHR,EGL_NO_CONTEXT,a);
   if(ctx==EGL_NO_CONTEXT) { printf("DGL BLOCKED GL3.%d context EGL=%#x\n",minor,eglGetError()); eglTerminate(d); return 77; }
   if(!eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,ctx)) return 1;
   load_gl();
   printf("GL_VERSION %s\nGL_RENDERER %s\n",glGetString(GL_VERSION),glGetString(GL_RENDERER));
   GLuint vao,fbo; glGenVertexArrays(1,&vao); glBindVertexArray(vao); glGenFramebuffers(1,&fbo); glBindFramebuffer(GL_FRAMEBUFFER,fbo);
   GLuint vertices; const float xy[]={-1,-1,3,-1,-1,3};
   glGenBuffers(1,&vertices); glBindBuffer(GL_ARRAY_BUFFER,vertices); glBufferData(GL_ARRAY_BUFFER,sizeof(xy),xy,GL_STATIC_DRAW);
   glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL); glEnableVertexAttribArray(0);
   glViewport(0,0,16,16); glDisable(GL_DITHER); if(!sint_only)integer_case(0); if(!uint_only)integer_case(1); if(!integers_only) clip_conditional();
   glDeleteBuffers(1,&vertices);
   glDeleteFramebuffers(1,&fbo); glDeleteVertexArrays(1,&vao);
   eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT); eglDestroyContext(d,ctx); eglTerminate(d);
   printf("DGL GL3.%d RESULT %s %s\n",minor,verify ? "PASS" : "RECORDED",integers_only ? "integers" : "all-rungs"); return 0;
}
