/* SPDX-License-Identifier: MIT
 * Generated-by: LLM. Independent desktop GL3.2 pixel/synchronization rungs.
 * No version overrides, driver patches or fake-node pixel PASS mode.
 * Build: cc -std=gnu11 -O1 -Wall -Wextra -Werror gl32-rungs.c -o gl32-rungs -lEGL
 */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/glcorearb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oracles.h"

#define FUNCS(X) \
 X(PFNGLBINDBUFFERPROC,BindBuffer) X(PFNGLBUFFERDATAPROC,BufferData) X(PFNGLDELETEBUFFERSPROC,DeleteBuffers) \
 X(PFNGLGENBUFFERSPROC,GenBuffers) X(PFNGLENABLEVERTEXATTRIBARRAYPROC,EnableVertexAttribArray) X(PFNGLVERTEXATTRIBPOINTERPROC,VertexAttribPointer) \
 X(PFNGLBINDATTRIBLOCATIONPROC,BindAttribLocation) X(PFNGLCOLORMASKPROC,ColorMask) X(PFNGLLOGICOPPROC,LogicOp) \
 X(PFNGLTEXSUBIMAGE2DPROC,TexSubImage2D) X(PFNGLTEXPARAMETERFVPROC,TexParameterfv) X(PFNGLUNIFORM1FPROC,Uniform1f) X(PFNGLUNIFORM2FPROC,Uniform2f) \
 X(PFNGLACTIVETEXTUREPROC,ActiveTexture) X(PFNGLATTACHSHADERPROC,AttachShader) \
 X(PFNGLBINDFRAMEBUFFERPROC,BindFramebuffer) X(PFNGLBINDTEXTUREPROC,BindTexture) X(PFNGLBINDVERTEXARRAYPROC,BindVertexArray) \
 X(PFNGLCHECKFRAMEBUFFERSTATUSPROC,CheckFramebufferStatus) X(PFNGLCLEARPROC,Clear) X(PFNGLCLEARCOLORPROC,ClearColor) \
 X(PFNGLCOMPRESSEDTEXIMAGE2DPROC,CompressedTexImage2D) X(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC,CompressedTexSubImage2D) \
 X(PFNGLCOMPILESHADERPROC,CompileShader) X(PFNGLCREATEPROGRAMPROC,CreateProgram) X(PFNGLCREATESHADERPROC,CreateShader) \
 X(PFNGLDELETEFRAMEBUFFERSPROC,DeleteFramebuffers) X(PFNGLDELETEPROGRAMPROC,DeleteProgram) X(PFNGLDELETESHADERPROC,DeleteShader) \
 X(PFNGLDELETESYNCPROC,DeleteSync) X(PFNGLDELETETEXTURESPROC,DeleteTextures) X(PFNGLDELETEVERTEXARRAYSPROC,DeleteVertexArrays) \
 X(PFNGLDISABLEPROC,Disable) X(PFNGLDRAWARRAYSPROC,DrawArrays) X(PFNGLENABLEPROC,Enable) \
 X(PFNGLFENCESYNCPROC,FenceSync) X(PFNGLFLUSHPROC,Flush) X(PFNGLFRAMEBUFFERTEXTURE2DPROC,FramebufferTexture2D) \
 X(PFNGLGENFRAMEBUFFERSPROC,GenFramebuffers) X(PFNGLGENTEXTURESPROC,GenTextures) X(PFNGLGENVERTEXARRAYSPROC,GenVertexArrays) \
 X(PFNGLGETERRORPROC,GetError) X(PFNGLGETINTEGERVPROC,GetIntegerv) X(PFNGLGETPROGRAMINFOLOGPROC,GetProgramInfoLog) \
 X(PFNGLGETPROGRAMIVPROC,GetProgramiv) X(PFNGLGETSHADERINFOLOGPROC,GetShaderInfoLog) X(PFNGLGETSHADERIVPROC,GetShaderiv) \
 X(PFNGLGETSTRINGPROC,GetString) X(PFNGLGETSTRINGIPROC,GetStringi) X(PFNGLGETUNIFORMLOCATIONPROC,GetUniformLocation) X(PFNGLLINKPROGRAMPROC,LinkProgram) \
 X(PFNGLSHADERSOURCEPROC,ShaderSource) X(PFNGLREADPIXELSPROC,ReadPixels) X(PFNGLSAMPLEMASKIPROC,SampleMaski) X(PFNGLTEXIMAGE2DPROC,TexImage2D) \
 X(PFNGLTEXIMAGE2DMULTISAMPLEPROC,TexImage2DMultisample) X(PFNGLTEXPARAMETERIPROC,TexParameteri) \
 X(PFNGLUNIFORM1IPROC,Uniform1i) X(PFNGLUNIFORM3FPROC,Uniform3f) X(PFNGLUNIFORM4FPROC,Uniform4f) \
 X(PFNGLUSEPROGRAMPROC,UseProgram) X(PFNGLVIEWPORTPROC,Viewport) X(PFNGLWAITSYNCPROC,WaitSync) X(PFNGLCLIENTWAITSYNCPROC,ClientWaitSync)
#define DECL(t,n) static t g##n;
FUNCS(DECL)
#undef DECL
static PFNGLMINSAMPLESHADINGPROC gMinSampleShading;
static EGLDisplay display;
static EGLContext context;
static const char *unit;
static unsigned failures;
static GLuint vertex_buffers[2];
static unsigned vertex_buffer_count;
static int core_profile, record_only;
static char case_context[128]="setup";

static void fatal(const char *why) {
 fprintf(stderr,"GL32-RUNG RESULT FAIL %s %s case=%s gl=%#x egl=%#x\n",unit,why,case_context,gGetError?gGetError():0,eglGetError());exit(1);
}
static void load(void) {
#define LOAD(t,n) g##n=(t)eglGetProcAddress("gl" #n);
 FUNCS(LOAD)
#undef LOAD
 gMinSampleShading=(void *)eglGetProcAddress("glMinSampleShading");
 if(!gMinSampleShading)gMinSampleShading=(void *)eglGetProcAddress("glMinSampleShadingARB");
}
#define NEED(n) do {if(!g##n)fatal("missing gl" #n);}while(0)
static int extension(const char *name) {
 if(core_profile) {
  GLint count=0;gGetIntegerv(GL_NUM_EXTENSIONS,&count);
  for(GLint i=0;gGetStringi&&i<count;i++)if(!strcmp((const char *)gGetStringi(GL_EXTENSIONS,(GLuint)i),name))return 1;
  return 0;
 }
 const char *all=(const char *)gGetString(GL_EXTENSIONS);
 if(!all)return 0;
 size_t n=strlen(name);
 for(const char *p=all;(p=strstr(p,name));p+=n)
  if((p==all||p[-1]==' ')&&(p[n]==' '||!p[n]))return 1;
 return 0;
}
static void require_extension(const char *name) {
 int have=extension(name);printf("GL32-EXTENSION %s %s\n",name,have?"HAVE":"MISSING");
 if(!have)fatal(name);
}
static EGLContext create(EGLContext share) {
 const EGLint compat[]={EGL_CONTEXT_MAJOR_VERSION,2,EGL_CONTEXT_MINOR_VERSION,1,EGL_NONE};
 const EGLint core[]={EGL_CONTEXT_MAJOR_VERSION,3,EGL_CONTEXT_MINOR_VERSION,2,EGL_CONTEXT_OPENGL_PROFILE_MASK,EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,EGL_NONE};
 EGLContext c=eglCreateContext(display,EGL_NO_CONFIG_KHR,share,core_profile?core:compat);
 if(c==EGL_NO_CONTEXT)fatal(core_profile?"core3.2 context refused":"compat2.1 context refused");
 return c;
}
static void current(EGLContext c) {
 if(!eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,c))fatal("make current");
}
static GLuint shader(GLenum kind,const char *text) {
 GLuint s=gCreateShader(kind);gShaderSource(s,1,&text,NULL);gCompileShader(s);
 GLint ok;gGetShaderiv(s,GL_COMPILE_STATUS,&ok);if(!ok){char log[4096];gGetShaderInfoLog(s,sizeof(log),NULL,log);fprintf(stderr,"%s\n",log);fatal("shader compile");}return s;
}
static GLuint program(const char *fragment) {
 const char *vs="#version 120\nattribute vec2 position;void main(){gl_Position=vec4(position,0,1);}";
 GLuint v=shader(GL_VERTEX_SHADER,vs),f=shader(GL_FRAGMENT_SHADER,fragment),p=gCreateProgram();gAttachShader(p,v);gAttachShader(p,f);gBindAttribLocation(p,0,"position");gLinkProgram(p);
 GLint ok;gGetProgramiv(p,GL_LINK_STATUS,&ok);if(!ok){char log[4096];gGetProgramInfoLog(p,sizeof(log),NULL,log);fprintf(stderr,"%s\n",log);fatal("program link");}
 gDeleteShader(v);gDeleteShader(f);return p;
}
static GLuint rgba_texture(void) {
 GLuint t;gGenTextures(1,&t);gBindTexture(GL_TEXTURE_2D,t);gTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,4,4,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
 gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);return t;
}
static void attach(GLuint fbo,GLuint texture,GLenum target) {
 gBindFramebuffer(GL_FRAMEBUFFER,fbo);gFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,target,texture,0);
 if(gCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)fatal("FBO incomplete");
 gViewport(0,0,4,4);
}
static void state(GLuint *vao,GLuint *fbo) {
 require_extension("GL_ARB_framebuffer_object");require_extension("GL_ARB_vertex_array_object");
 NEED(GenVertexArrays);NEED(BindVertexArray);NEED(DeleteVertexArrays);NEED(DeleteBuffers);NEED(GenFramebuffers);NEED(BindFramebuffer);NEED(FramebufferTexture2D);NEED(CheckFramebufferStatus);
 NEED(GenBuffers);NEED(BindBuffer);NEED(BufferData);NEED(EnableVertexAttribArray);NEED(VertexAttribPointer);
 if(vertex_buffer_count>=2)fatal("vertex buffer bookkeeping");
 gGenVertexArrays(1,vao);gBindVertexArray(*vao);gGenFramebuffers(1,fbo);
 const float vertices[]={-1,-1,3,-1,-1,3};GLuint vbo;
 gGenBuffers(1,&vbo);vertex_buffers[vertex_buffer_count++]=vbo;gBindBuffer(GL_ARRAY_BUFFER,vbo);
 gBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);gVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);gEnableVertexAttribArray(0);
 gDisable(GL_DITHER);gDisable(GL_BLEND);gDisable(GL_DEPTH_TEST);gDisable(GL_STENCIL_TEST);gDisable(GL_SCISSOR_TEST);gDisable(GL_FRAMEBUFFER_SRGB);
}
static unsigned pixels(const char *name,const uint8_t want[4]) {
 uint8_t got[4*4*4];gReadPixels(0,0,4,4,GL_RGBA,GL_UNSIGNED_BYTE,got);if(gGetError()!=GL_NO_ERROR)fatal("readback GL error");
 if(record_only)return 0;
 for(unsigned i=0;i<16;i++)for(unsigned c=0;c<4;c++)if(got[4*i+c]!=want[c]){
  fprintf(stderr,"PIXEL FAIL unit=%s case=%s xy=%u,%u channel=%u expected=%u got=%u rgba=%u,%u,%u,%u\n",unit,name,i%4,i/4,c,want[c],got[4*i+c],got[4*i],got[4*i+1],got[4*i+2],got[4*i+3]);failures++;return 1;
 }
 return 0;
}
static void clear_zero(void) {gClearColor(0,0,0,0);gClear(GL_COLOR_BUFFER_BIT);}
static void draw(GLuint p) {gUseProgram(p);gDrawArrays(GL_TRIANGLES,0,3);if(gGetError()!=GL_NO_ERROR)fatal("draw GL error");}
static const uint8_t face[6][4]={{192,0,0,192},{0,192,0,192},{0,0,192,192},{192,192,0,192},{192,0,192,192},{0,192,192,192}};
static void upload_cube(GLuint cube,int corners) {
 gBindTexture(GL_TEXTURE_CUBE_MAP,cube);
 for(unsigned f=0;f<6;f++){
  uint8_t data[4*4*4];
  for(unsigned y=0;y<4;y++)for(unsigned x=0;x<4;x++){
   uint8_t *p=&data[4*(y*4+x)];if(!corners){memcpy(p,face[f],4);continue;}
   int a=x<2?-1:1,b=y<2?-1:1,dir[3];
   switch(f){case 0:dir[0]=1;dir[1]=-b;dir[2]=-a;break;case 1:dir[0]=-1;dir[1]=-b;dir[2]=a;break;
    case 2:dir[0]=a;dir[1]=1;dir[2]=b;break;case 3:dir[0]=a;dir[1]=-1;dir[2]=-b;break;
    case 4:dir[0]=a;dir[1]=-b;dir[2]=1;break;default:dir[0]=-a;dir[1]=-b;dir[2]=-1;break;}
   for(unsigned c=0;c<3;c++)p[c]=dir[c]>0?192:0;
   p[3]=192;
  }
  gTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+f,0,GL_RGBA8,4,4,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
 }
 gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_BASE_LEVEL,0);gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAX_LEVEL,0);
 gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
}
static void cube(void) {
 require_extension("GL_ARB_seamless_cube_map");
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture();attach(fbo,out,GL_TEXTURE_2D);GLuint cube;gGenTextures(1,&cube);gActiveTexture(GL_TEXTURE0);upload_cube(cube,0);
 GLuint p=program("#version 120\nuniform samplerCube source;uniform vec3 direction;void main(){gl_FragColor=textureCube(source,direction);}");gUseProgram(p);gUniform1i(gGetUniformLocation(p,"source"),0);GLint u=gGetUniformLocation(p,"direction");
 gEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);gBindTexture(GL_TEXTURE_CUBE_MAP,cube);gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 unsigned start=failures;
 for(unsigned f=0;f<6;f++){float d[3]={0};d[f/2]=f%2?-1:1;gUniform3f(u,d[0],d[1],d[2]);draw(p);pixels("face-center",face[f]);}
 printf("GL32-CASE %s cube-nearest %s %u/6\n",unit,failures==start?record_only?"RECORDED":"PASS":"FAIL",6-(failures-start));
 gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);gTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);start=failures;
 for(unsigned a=0;a<3;a++)for(unsigned b=a+1;b<3;b++)for(unsigned sa=0;sa<2;sa++)for(unsigned sb=0;sb<2;sb++){
  float d[3]={0};d[a]=sa?-1:1;d[b]=sb?-1:1;uint8_t want[4];for(unsigned c=0;c<4;c++)want[c]=(face[2*a+sa][c]+face[2*b+sb][c])/2;
  gUniform3f(u,d[0],d[1],d[2]);draw(p);pixels("face-edge",want);
 }
 printf("GL32-CASE %s cube-seamless-edges %s %u/12\n",unit,failures==start?record_only?"RECORDED":"PASS":"FAIL",12-(failures-start));
 /* Every incident face has the same corner texel. This is exact under all permitted
  * corner replacement rules, instead of requiring the spec's recommended average. */
 upload_cube(cube,1);start=failures;
 for(unsigned i=0;i<8;i++){float d[3];uint8_t want[4];for(unsigned c=0;c<3;c++){d[c]=(i&(1u<<c))?1:-1;want[c]=d[c]>0?192:0;}want[3]=192;gUniform3f(u,d[0],d[1],d[2]);draw(p);pixels("cube-corner",want);}
 printf("GL32-CASE %s cube-corners %s %u/8\n",unit,failures==start?record_only?"RECORDED":"PASS":"FAIL",8-(failures-start));
 gDeleteProgram(p);gDeleteTextures(1,&cube);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
static void rgtc_block(uint8_t b[8],int sign,int phase,int reverse) {
 int t[8];rgtc_reference_table(sign,phase,t);b[0]=(uint8_t)((sign&&phase==3)?-128:t[0]);b[1]=(uint8_t)t[1];uint64_t bits=0;
 for(unsigned i=0;i<16;i++){unsigned n=phase==3?(i&1):(i&7);if(reverse)n=phase==3?1-n:7-n;bits|=(uint64_t)n<<(3*i);}for(unsigned i=0;i<6;i++)b[i+2]=(uint8_t)(bits>>(8*i));
}
static void rgtc(void) {
 require_extension("GL_ARB_texture_compression_rgtc");NEED(CompressedTexImage2D);NEED(CompressedTexSubImage2D);
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture();attach(fbo,out,GL_TEXTURE_2D);
 GLuint p=program("#version 120\nuniform sampler2D source;uniform float side;uniform int signed_mode;void main(){vec4 v=texture2D(source,fract(gl_FragCoord.xy/vec2(side)));vec2 q=signed_mode!=0?(floor(v.rg*127.0+0.5)+vec2(128.0))/255.0:v.rg;gl_FragColor=vec4(q,0,1);}");
 for(unsigned fmt=0;fmt<4;fmt++) {
  int sign=fmt&1,rg=fmt>=2;GLenum format=rg?(sign?GL_COMPRESSED_SIGNED_RG_RGTC2:GL_COMPRESSED_RG_RGTC2):(sign?GL_COMPRESSED_SIGNED_RED_RGTC1:GL_COMPRESSED_RED_RGTC1);
  GLuint tex;gGenTextures(1,&tex);gActiveTexture(GL_TEXTURE0);gBindTexture(GL_TEXTURE_2D,tex);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,1);
  unsigned start=failures;
  for(int phase=0;phase<4;phase++){
   snprintf(case_context,sizeof(case_context),"RGTC format=%#x phase=%d",format,phase);
   uint8_t block[16]={0};rgtc_block(block,sign,phase,0);if(rg)rgtc_block(block+8,sign,phase,1);int bytes=rg?16:8,level=phase==2,side=level?2:4;
   if(phase==1)gCompressedTexSubImage2D(GL_TEXTURE_2D,0,0,0,4,4,format,bytes,block);else gCompressedTexImage2D(GL_TEXTURE_2D,level,format,side,side,0,bytes,block);
   if(gGetError()!=GL_NO_ERROR)fatal("RGTC upload/update");
   gUseProgram(p);gUniform1i(gGetUniformLocation(p,"source"),0);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,level);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,level);gUniform1f(gGetUniformLocation(p,"side"),(float)side);gUniform1i(gGetUniformLocation(p,"signed_mode"),sign);draw(p);
   uint8_t got[64];gReadPixels(0,0,4,4,GL_RGBA,GL_UNSIGNED_BYTE,got);if(gGetError()!=GL_NO_ERROR)fatal("RGTC readback");int table[8];rgtc_reference_table(sign,phase,table);int bad=0;
   for(unsigned y=0;!record_only&&y<4;y++)for(unsigned x=0;x<4;x++){
    unsigned index=(y&(side-1))*4+(x&(side-1));unsigned selector=phase==3?(index&1):(index&7),other=phase==3?1-selector:7-selector;
    uint8_t want[4]={(uint8_t)(table[selector]+(sign?128:0)),(uint8_t)(rg?table[other]+(sign?128:0):(sign?128:0)),0,255};
    for(unsigned c=0;c<4;c++)if(got[4*(y*4+x)+c]!=want[c]){if(!bad)fprintf(stderr,"RGTC MISMATCH format=%#x phase=%d xy=%u,%u channel=%u want=%u got=%u\n",format,phase,x,y,c,want[c],got[4*(y*4+x)+c]);bad=1;}
   }
   failures+=(unsigned)bad;
  }
  printf("GL32-CASE %s RGTC-%s-%s %s %u/4\n",unit,rg?"RG":"R",sign?"SNORM":"UNORM",failures==start?record_only?"RECORDED":"PASS":"FAIL",4-(failures-start));gDeleteTextures(1,&tex);
 }
 gDeleteProgram(p);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
static void multisample(int frequency) {
 require_extension("GL_ARB_texture_multisample");NEED(TexImage2DMultisample);NEED(SampleMaski);
 if(frequency)require_extension("GL_ARB_sample_shading");
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture(),ms;gGenTextures(1,&ms);gBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ms);gTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_RGBA8,4,4,GL_TRUE);
 GLenum allocation_error=gGetError();
 if(allocation_error!=GL_NO_ERROR){fprintf(stderr,"GL32-MS allocation samples=4 GL=%#x\n",allocation_error);fatal("four-sample texture allocation");}
 GLuint drawp=program(frequency?"#version 120\n#extension GL_ARB_sample_shading:require\nvoid main(){gl_FragColor=vec4(gl_SampleID==0?1:0,gl_SampleID==1?1:0,gl_SampleID==2?1:0,gl_SampleID==3?1:0);}":"#version 120\nvoid main(){gl_FragColor=vec4(1,0,1,1);}");
 GLuint fetch=program("#version 120\n#extension GL_ARB_texture_multisample:require\nuniform sampler2DMS source;uniform int sample_index;void main(){gl_FragColor=texelFetch(source,ivec2(1,1),sample_index);}");
 unsigned rounds=frequency?1:36,checks=0;
 GLuint shader_mask=frequency?0:program("#version 120\n#extension GL_ARB_sample_shading:require\nuniform int output_mask;void main(){gl_FragColor=vec4(1,0,1,1);gl_SampleMask[0]=output_mask;}");
 if(frequency&&!gMinSampleShading)fatal("sample shading entry point absent");
 for(unsigned n=0;n<rounds;n++){
  unsigned static_mask=15,output_mask=15;int shader_output=!frequency&&n>=16;
  if(!frequency&&n<16)static_mask=n;
  else if(shader_output&&n<32)output_mask=n-16;
  else if(shader_output){static const unsigned a[]={5,10,3,0},b[]={10,5,5,15};static_mask=a[n-32];output_mask=b[n-32];}
  unsigned mask=static_mask&output_mask;
  snprintf(case_context,sizeof(case_context),"pipeline-mask=%x shader-mask=%x sample-frequency=%d",static_mask,output_mask,frequency);attach(fbo,ms,GL_TEXTURE_2D_MULTISAMPLE);gDisable(GL_SAMPLE_MASK);if(frequency)gDisable(GL_SAMPLE_SHADING);clear_zero();
  if(frequency){gEnable(GL_SAMPLE_SHADING);gMinSampleShading(1.0f);}else {gEnable(GL_SAMPLE_MASK);gSampleMaski(0,static_mask);}
  GLuint active=shader_output?shader_mask:drawp;
  if(shader_output){gUseProgram(active);gUniform1i(gGetUniformLocation(active,"output_mask"),(GLint)output_mask);}
  draw(active);gDisable(GL_SAMPLE_MASK);if(frequency)gDisable(GL_SAMPLE_SHADING);attach(fbo,out,GL_TEXTURE_2D);gActiveTexture(GL_TEXTURE0);gBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ms);gUseProgram(fetch);gUniform1i(gGetUniformLocation(fetch,"source"),0);
  for(unsigned sample=0;sample<4;sample++){
   uint8_t want[4]={0};if(frequency)want[sample]=255;else if(mask&(1u<<sample)){want[0]=want[2]=want[3]=255;}
   gUniform1i(gGetUniformLocation(fetch,"sample_index"),(GLint)sample);draw(fetch);char name[96];snprintf(name,sizeof(name),"pipeline-mask=%x shader-mask=%x sample=%u",static_mask,output_mask,sample);pixels(name,want);checks++;
  }
 }
 printf("GL32-CASE %s %s %s %u/%u\n",unit,frequency?"sample-frequency-fetch":"sample-mask-fetch",failures?"FAIL":record_only?"RECORDED":"PASS",checks-failures,checks);
 if(shader_mask)gDeleteProgram(shader_mask);
 gDeleteProgram(drawp);gDeleteProgram(fetch);gDeleteTextures(1,&ms);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
/* Native MS reads: edge coverage distinguishes every configured A9 sample.
 * Standard x positions are3/8,7/8,1/8,5/8. Vertical boundaries avoid any
 * API viewport-y convention; Zink and the kernel use that standard pattern. */
static void ms_fetch(void) {
 require_extension("GL_ARB_texture_multisample");NEED(TexImage2DMultisample);
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture(),ms;
 gGenTextures(1,&ms);gBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ms);gTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4,GL_RGBA8,4,4,GL_TRUE);
 GLenum allocation_error=gGetError();
 if(allocation_error!=GL_NO_ERROR){fprintf(stderr,"GL32-MS allocation samples=4 GL=%#x\n",allocation_error);fatal("four-sample texture allocation");}
 GLuint fill=program("#version 120\nuniform vec4 value;void main(){gl_FragColor=value;}");
 GLuint fetch=program("#version 120\n#extension GL_ARB_texture_multisample:require\nuniform sampler2DMS source;uniform int sample_index;void main(){gl_FragColor=texelFetch(source,ivec2(1,1),sample_index);}");
 static const float fullscreen[]={-1,-1,3,-1,-1,3};
 static const unsigned x16[]={6,14,2,10};unsigned checks=0;
 for(unsigned phase=0;phase<5;phase++) {
  attach(fbo,ms,GL_TEXTURE_2D_MULTISAMPLE);clear_zero();
  if(phase) {
   float edge=phase==1?3.0f:(1.0f+0.25f*(phase-1))*0.5f-1.0f;
   float vertices[]={-4,-4,edge,-4,edge,4};
   gBindBuffer(GL_ARRAY_BUFFER,vertex_buffers[vertex_buffer_count-1]);
   gBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
   gUseProgram(fill);gUniform4f(gGetUniformLocation(fill,"value"),0.25f,0.5f,0.75f,1.0f);draw(fill);
  }
  attach(fbo,out,GL_TEXTURE_2D);gBindBuffer(GL_ARRAY_BUFFER,vertex_buffers[vertex_buffer_count-1]);
  gBufferData(GL_ARRAY_BUFFER,sizeof(fullscreen),fullscreen,GL_STATIC_DRAW);
  gActiveTexture(GL_TEXTURE0);gBindTexture(GL_TEXTURE_2D_MULTISAMPLE,ms);
  gUseProgram(fetch);gUniform1i(gGetUniformLocation(fetch,"source"),0);
  for(unsigned sample=0;sample<4;sample++) {
   unsigned inside=phase==1||(phase>=2&&x16[sample]<4*(phase-1));
   const uint8_t colour[]={64,128,191,255};uint8_t want[4]={0};if(inside)memcpy(want,colour,4);
   snprintf(case_context,sizeof(case_context),"ms-fetch phase=%u sample=%u standard_x=%u/16 covered=%u",phase,sample,x16[sample],inside);
   gUniform1i(gGetUniformLocation(fetch,"sample_index"),(GLint)sample);draw(fetch);pixels(case_context,want);checks++;
  }
 }
 printf("GL32-CASE %s four-sample-edge-fetch %s %u/20\n",unit,failures?"FAIL":record_only?"RECORDED":"PASS",checks-failures);
 gDeleteProgram(fill);gDeleteProgram(fetch);gDeleteTextures(1,&ms);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
/* A core3.2 string must not turn an unsupported GS into a silent draw.
 * Frontend rejection is expected; a later attempted draw is accepted by the
 * runner only when the G5P pipeline emits its named refusal. */
static void gs_refusal(void) {
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture();attach(fbo,out,GL_TEXTURE_2D);
 GLuint p=gCreateProgram();GLuint v=shader(GL_VERTEX_SHADER,"#version 150\nin vec2 position;void main(){gl_Position=vec4(position,0,1);}");
 GLuint f=shader(GL_FRAGMENT_SHADER,"#version 150\nout vec4 colour;void main(){colour=vec4(0,1,0,1);}");
 gAttachShader(p,v);gAttachShader(p,f);gBindAttribLocation(p,0,"position");gLinkProgram(p);
 GLint ok=0;gGetProgramiv(p,GL_LINK_STATUS,&ok);if(!ok)fatal("GS refusal VS/FS control link failed");
 GLuint gs=gCreateShader(GL_GEOMETRY_SHADER);GLenum error=gGetError();
 if(!gs||error){printf("GL32-GS REFUSAL frontend geometry shader creation GL=%#x\n",error);goto done;}
 const char *source="#version 150\nlayout(points) in;layout(triangle_strip,max_vertices=3) out;void main(){for(int i=0;i<3;i++){gl_Position=gl_in[0].gl_Position+vec4(i==1?0.5:-0.5,i==2?0.5:-0.5,0,0);EmitVertex();}EndPrimitive();}";
 gShaderSource(gs,1,&source,NULL);gCompileShader(gs);gGetShaderiv(gs,GL_COMPILE_STATUS,&ok);
 if(!ok){char log[4096]={0};gGetShaderInfoLog(gs,sizeof(log),NULL,log);printf("GL32-GS REFUSAL frontend geometry shader compilation: %s\n",log);gDeleteShader(gs);goto done;}
 gAttachShader(p,gs);gLinkProgram(p);gGetProgramiv(p,GL_LINK_STATUS,&ok);
 if(!ok){char log[4096]={0};gGetProgramInfoLog(p,sizeof(log),NULL,log);printf("GL32-GS REFUSAL frontend geometry shader link: %s\n",log);gDeleteShader(gs);goto done;}
 gUseProgram(p);gDrawArrays(GL_POINTS,0,1);gFlush();error=gGetError();
 printf("GL32-GS DRAW-ATTEMPT geometry pipeline expected to refuse GL=%#x\n",error);
 /* A zero GL error is insufficient: external gate requires the actual named
  * G5P stderr refusal, and never accepts an apparently successful GS draw. */
 failures++;gDeleteShader(gs);
 done:gDeleteShader(v);gDeleteShader(f);gDeleteProgram(p);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
static void logic_ops(void) {
 NEED(LogicOp);NEED(ColorMask);NEED(TexSubImage2D);
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture();attach(fbo,out,GL_TEXTURE_2D);
 GLuint p=program("#version 120\nuniform vec4 value;void main(){gl_FragColor=value;}");
 /* Binary and fractional byte-exact sources exercise UNORM conversion.
  * Alternating destination bits exercise all four input truth-table combinations
  * at every bit position; RGB/alpha write masks also preserve the LOAD. */
 static const uint8_t sources[8][4]={{0,0,0,0},{255,255,255,255},{0,255,0,255},{255,0,255,0},
  {1,127,128,254},{17,85,170,239},{32,64,191,223},{254,128,127,1}};
 static const uint8_t dests[4][4]={{0x55,0xaa,0x33,0xcc},{0xaa,0x55,0xcc,0x33},{0,255,0,255},{255,0,255,0}};
 static const char *names[16]={"CLEAR","AND","AND_REVERSE","COPY","AND_INVERTED","NOOP","XOR","OR","NOR","EQUIV","INVERT","OR_REVERSE","COPY_INVERTED","OR_INVERTED","NAND","SET"};
 uint8_t data[64];unsigned before=failures;
 for(unsigned d=0;d<4;d++) {
  for(unsigned i=0;i<16;i++)memcpy(data+4*i,dests[d],4);
  gBindTexture(GL_TEXTURE_2D,out);gTexSubImage2D(GL_TEXTURE_2D,0,0,0,4,4,GL_RGBA,GL_UNSIGNED_BYTE,data);
  pixels("logic destination upload",dests[d]);
 }
 printf("GL32-CASE %s destination-precondition %s %u/4\n",unit,failures==before?record_only?"RECORDED":"PASS":"FAIL",4-(failures-before));
 for(unsigned op=0;op<16;op++) {
  before=failures;
  for(unsigned source=0;source<8;source++)for(unsigned d=0;d<4;d++)for(unsigned masked=0;masked<2;masked++) {
   for(unsigned i=0;i<16;i++)memcpy(data+4*i,dests[d],4);
   gBindTexture(GL_TEXTURE_2D,out);gTexSubImage2D(GL_TEXTURE_2D,0,0,0,4,4,GL_RGBA,GL_UNSIGNED_BYTE,data);
   gColorMask(GL_TRUE,!masked,GL_TRUE,!masked);gEnable(GL_COLOR_LOGIC_OP);gLogicOp(GL_CLEAR+op);
   snprintf(case_context,sizeof(case_context),"op=%s source=%u dest=%u mask=%x",names[op],source,d,masked?5:15);
   gUseProgram(p);gUniform4f(gGetUniformLocation(p,"value"),sources[source][0]/255.0f,sources[source][1]/255.0f,sources[source][2]/255.0f,sources[source][3]/255.0f);draw(p);
   uint8_t want[4];for(unsigned c=0;c<4;c++)want[c]=masked&&(c==1||c==3)?dests[d][c]:logic_reference(op,sources[source][c],dests[d][c]);
   pixels(case_context,want);
   gDisable(GL_COLOR_LOGIC_OP);gColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  }
  printf("GL32-CASE %s logic-%s %s %u/64\n",unit,names[op],failures==before?record_only?"RECORDED":"PASS":"FAIL",64-(failures-before));
 }
 gDeleteProgram(p);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
static void borders(void) {
 NEED(TexParameterfv);NEED(Uniform2f);
 GLuint vao,fbo;state(&vao,&fbo);GLuint out=rgba_texture();attach(fbo,out,GL_TEXTURE_2D);
 const uint8_t inside[4]={0,192,64,128};uint8_t data[16];for(unsigned i=0;i<4;i++)memcpy(data+4*i,inside,4);
 GLuint texture;gGenTextures(1,&texture);gActiveTexture(GL_TEXTURE0);gBindTexture(GL_TEXTURE_2D,texture);
 gTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,2,2,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
 gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
 gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
 GLuint p=program("#version 120\nuniform sampler2D source;uniform vec2 coord;void main(){gl_FragColor=texture2D(source,coord);}");
 gUseProgram(p);gUniform1i(gGetUniformLocation(p,"source"),0);GLint coord=gGetUniformLocation(p,"coord");
 static const uint8_t colors[2][4]={{192,64,128,192},{64,192,0,64}};
 static const int coords[13][2]={{1,1},{-2,1},{4,1},{1,-2},{1,4},{0,1},{2,1},{1,0},{1,2},{0,0},{0,2},{2,0},{2,2}};
 for(unsigned color=0;color<2;color++)for(unsigned linear=0;linear<2;linear++) {
  unsigned before=failures;GLfloat rgba[4];for(unsigned c=0;c<4;c++)rgba[c]=colors[color][c]/255.0f;
  gTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,rgba);
  gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,linear?GL_LINEAR:GL_NEAREST);gTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,linear?GL_LINEAR:GL_NEAREST);
  for(unsigned i=0;i<13;i++) {
   int u2=coords[i][0],v2=coords[i][1];unsigned weight=border_weight(linear,u2,v2);uint8_t want[4];
   for(unsigned c=0;c<4;c++)want[c]=border_reference(weight,colors[color][c],inside[c]);
   snprintf(case_context,sizeof(case_context),"border=%u %s uv=(%g,%g) weight=%u/4",color,linear?"linear":"nearest",u2*0.5,v2*0.5,weight);
   gUniform2f(coord,u2*0.5f,v2*0.5f);draw(p);pixels(case_context,want);
  }
  printf("GL32-CASE %s arbitrary-border-%u-%s %s %u/13\n",unit,color,linear?"linear":"nearest",failures==before?record_only?"RECORDED":"PASS":"FAIL",13-(failures-before));
 }
 gDeleteProgram(p);gDeleteTextures(1,&texture);gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo);gDeleteVertexArrays(1,&vao);
}
/* Limits from GL3.2 core tables 6.34, 6.37, 6.39, 6.41--6.46.
 * Query PASS checks the advertisement only, never execution at the limit. */
static void limits(void) {
 const struct {GLenum pname;const char *name;GLint minimum;int upper;} rows[]={
#define LIMIT(n,v) {GL_##n,#n,v,0}
  LIMIT(MAX_DRAW_BUFFERS,8),LIMIT(MAX_COLOR_ATTACHMENTS,8),LIMIT(MAX_SAMPLES,4),
  LIMIT(MAX_CLIP_DISTANCES,8),LIMIT(SUBPIXEL_BITS,4),LIMIT(MAX_3D_TEXTURE_SIZE,256),
  LIMIT(MAX_TEXTURE_SIZE,1024),LIMIT(MAX_ARRAY_TEXTURE_LAYERS,256),LIMIT(MAX_CUBE_MAP_TEXTURE_SIZE,1024),
  LIMIT(MAX_RENDERBUFFER_SIZE,1024),LIMIT(MAX_TEXTURE_BUFFER_SIZE,65536),LIMIT(MAX_RECTANGLE_TEXTURE_SIZE,1024),
  LIMIT(MAX_VERTEX_ATTRIBS,16),LIMIT(MAX_VERTEX_UNIFORM_COMPONENTS,1024),LIMIT(MAX_VERTEX_UNIFORM_BLOCKS,12),
  LIMIT(MAX_VERTEX_OUTPUT_COMPONENTS,64),LIMIT(MAX_VERTEX_TEXTURE_IMAGE_UNITS,16),
  LIMIT(MAX_GEOMETRY_UNIFORM_BLOCKS,12),LIMIT(MAX_GEOMETRY_INPUT_COMPONENTS,64),LIMIT(MAX_GEOMETRY_OUTPUT_COMPONENTS,128),
  LIMIT(MAX_GEOMETRY_OUTPUT_VERTICES,256),LIMIT(MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS,1024),LIMIT(MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,16),
  LIMIT(MAX_FRAGMENT_UNIFORM_COMPONENTS,1024),LIMIT(MAX_FRAGMENT_UNIFORM_BLOCKS,12),LIMIT(MAX_FRAGMENT_INPUT_COMPONENTS,128),
  LIMIT(MAX_TEXTURE_IMAGE_UNITS,16),LIMIT(MAX_PROGRAM_TEXEL_OFFSET,7),{GL_MIN_PROGRAM_TEXEL_OFFSET,"MIN_PROGRAM_TEXEL_OFFSET",-8,1},
  LIMIT(MAX_UNIFORM_BUFFER_BINDINGS,36),LIMIT(MAX_UNIFORM_BLOCK_SIZE,16384),LIMIT(MAX_COMBINED_UNIFORM_BLOCKS,36),
  LIMIT(MAX_VARYING_COMPONENTS,60),LIMIT(MAX_COMBINED_TEXTURE_IMAGE_UNITS,48),
  LIMIT(MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,64),LIMIT(MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,4),LIMIT(MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS,4),
  LIMIT(MAX_SAMPLE_MASK_WORDS,1),LIMIT(MAX_COLOR_TEXTURE_SAMPLES,1),LIMIT(MAX_DEPTH_TEXTURE_SAMPLES,1),LIMIT(MAX_INTEGER_SAMPLES,1),
#undef LIMIT
 };
 unsigned count=sizeof(rows)/sizeof(rows[0]);
 for(unsigned i=0;i<count;i++){
  GLint got=0;gGetIntegerv(rows[i].pname,&got);GLenum err=gGetError();int pass=!err&&(rows[i].upper?got<=rows[i].minimum:got>=rows[i].minimum);
  printf("GL32-LIMIT %s %s got=%d required%s%d gl=%#x\n",pass?"PASS":"FAIL",rows[i].name,got,rows[i].upper?"<=":">=",rows[i].minimum,err);failures+=!pass;
 }
 printf("GL32-CASE %s advertised-minima %s %u/%u\n",unit,failures?"FAIL":record_only?"RECORDED":"PASS",count-failures,count);
}
static void fences(void) {
 require_extension("GL_ARB_sync");NEED(FenceSync);NEED(Flush);NEED(WaitSync);NEED(ClientWaitSync);NEED(DeleteSync);
 GLuint vao1,fbo1;state(&vao1,&fbo1);GLuint shared=rgba_texture();attach(fbo1,shared,GL_TEXTURE_2D);
 GLuint writer=program("#version 120\nuniform vec4 value;void main(){gl_FragColor=value;}");
 GLuint reader=program("#version 120\nuniform sampler2D source;void main(){gl_FragColor=texture2D(source,vec2(0.375));}");
 EGLContext second=create(context);current(second);GLuint vao2,fbo2;state(&vao2,&fbo2);GLuint out=rgba_texture();attach(fbo2,out,GL_TEXTURE_2D);
 for(unsigned i=0;i<8;i++){
  snprintf(case_context,sizeof(case_context),"cross-context-fence pattern=%u",i);
  current(context);gBindVertexArray(vao1);attach(fbo1,shared,GL_TEXTURE_2D);gUseProgram(writer);gUniform4f(gGetUniformLocation(writer,"value"),i&1?1:0,i&2?1:0,i&4?1:0,1);draw(writer);
  GLsync a=gFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE,0);if(!a)fatal("writer fence");gFlush();
  current(second);gBindVertexArray(vao2);attach(fbo2,out,GL_TEXTURE_2D);gWaitSync(a,0,GL_TIMEOUT_IGNORED);gActiveTexture(GL_TEXTURE0);gBindTexture(GL_TEXTURE_2D,0);gBindTexture(GL_TEXTURE_2D,shared);gUseProgram(reader);gUniform1i(gGetUniformLocation(reader,"source"),0);draw(reader);
  GLsync b=gFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE,0);if(!b)fatal("reader fence");gFlush();GLenum wait=gClientWaitSync(b,GL_SYNC_FLUSH_COMMANDS_BIT,2000000000ull);if(wait!=GL_ALREADY_SIGNALED&&wait!=GL_CONDITION_SATISFIED)fatal("reader client wait timeout/error");
  uint8_t want[4]={i&1?255:0,i&2?255:0,i&4?255:0,255};pixels("shared-context fence",want);
  wait=gClientWaitSync(a,0,0);if(wait!=GL_ALREADY_SIGNALED&&wait!=GL_CONDITION_SATISFIED)fatal("dependency fence not complete after consumer");gDeleteSync(a);gDeleteSync(b);
 }
 printf("GL32-CASE %s cross-context-server-and-client-wait %s %u/8\n",unit,failures?"FAIL":record_only?"RECORDED":"PASS",8-failures);
 gDeleteTextures(1,&out);gDeleteFramebuffers(1,&fbo2);gDeleteVertexArrays(1,&vao2);current(context);eglDestroyContext(display,second);gDeleteProgram(writer);gDeleteProgram(reader);gDeleteTextures(1,&shared);gDeleteFramebuffers(1,&fbo1);gDeleteVertexArrays(1,&vao1);
}
int main(int argc,char **argv) {
 if(argc==3&&!strcmp(argv[2],"--record-only"))record_only=1;
 if(argc!=2&&!record_only){fprintf(stderr,"usage: %s cube|gs-refusal|ms-fetch|sample-mask|sample-shading|sync|rgtc|logic-op|border|limits\n",argv[0]);return 2;}unit=argv[1];core_profile=!strcmp(unit,"limits")||!strcmp(unit,"gs-refusal");
 if(strcmp(unit,"cube")&&strcmp(unit,"sample-mask")&&strcmp(unit,"sample-shading")&&strcmp(unit,"ms-fetch")&&strcmp(unit,"gs-refusal")&&strcmp(unit,"sync")&&strcmp(unit,"rgtc")&&strcmp(unit,"limits")&&strcmp(unit,"logic-op")&&strcmp(unit,"border"))return 2;
 const char *overrides[]={"MESA_GL_VERSION_OVERRIDE","MESA_GLES_VERSION_OVERRIDE","MESA_GLSL_VERSION_OVERRIDE","MESA_EXTENSION_OVERRIDE","PVR_G5P_FORMATS","LIBGL_ALWAYS_SOFTWARE","GALLIUM_DRIVER","LD_PRELOAD"};
 for(unsigned i=0;i<sizeof(overrides)/sizeof(overrides[0]);i++)if(getenv(overrides[i])&&getenv(overrides[i])[0]&&!(record_only&&!strcmp(overrides[i],"LD_PRELOAD")))fatal(overrides[i]);
 PFNEGLGETPLATFORMDISPLAYEXTPROC platform=(void*)eglGetProcAddress("eglGetPlatformDisplayEXT");display=platform?platform(EGL_PLATFORM_SURFACELESS_MESA,NULL,NULL):EGL_NO_DISPLAY;
 if(display==EGL_NO_DISPLAY||!eglInitialize(display,NULL,NULL)||!eglBindAPI(EGL_OPENGL_API))fatal("EGL init/bind desktop GL");
 context=create(EGL_NO_CONTEXT);current(context);load();
 NEED(GetError);NEED(GetString);NEED(GetIntegerv);
 if(!core_profile) {
  NEED(CreateShader);NEED(ShaderSource);NEED(CompileShader);NEED(GetShaderiv);NEED(GetShaderInfoLog);
  NEED(CreateProgram);NEED(AttachShader);NEED(BindAttribLocation);NEED(LinkProgram);NEED(GetProgramiv);NEED(GetProgramInfoLog);
 }
 if(core_profile) {
  GLint major=0,minor=0,mask=0;gGetIntegerv(GL_MAJOR_VERSION,&major);gGetIntegerv(GL_MINOR_VERSION,&minor);gGetIntegerv(GL_CONTEXT_PROFILE_MASK,&mask);
  if(gGetError()!=GL_NO_ERROR||major<3||(major==3&&minor<2)||!(mask&GL_CONTEXT_CORE_PROFILE_BIT))fatal("actual core3.2 profile missing");
 }
 printf("GL32-MODE %s\n",record_only?"RECORDED-no-pixel-judgement":"hardware-pixels");
 printf("GL32-CONTEXT requested=%s overrides=none\n",core_profile?"core3.2":"compat2.1");
 printf("GL32-SCOPE %s\n",core_profile?"strict-core-profile-and-advertised-limits":!strcmp(unit,"sample-shading")?"later-GL4.0-ARB_sample_shading":"independent-feature-on-default-compat");
 printf("GL32-IDENTITY GL_VERSION=%s renderer=%s GLSL=%s\n",gGetString(GL_VERSION),gGetString(GL_RENDERER),gGetString(GL_SHADING_LANGUAGE_VERSION));
 if(!strcmp(unit,"gs-refusal"))gs_refusal();else if(!strcmp(unit,"ms-fetch"))ms_fetch();else if(!strcmp(unit,"logic-op"))logic_ops();else if(!strcmp(unit,"border"))borders();else if(!strcmp(unit,"limits"))limits();else if(!strcmp(unit,"cube"))cube();else if(!strcmp(unit,"rgtc"))rgtc();else if(!strcmp(unit,"sync"))fences();else multisample(!strcmp(unit,"sample-shading"));
 if(vertex_buffer_count)gDeleteBuffers((GLsizei)vertex_buffer_count,vertex_buffers);
 current(EGL_NO_CONTEXT);eglDestroyContext(display,context);eglTerminate(display);printf("GL32-RUNG RESULT %s %s mismatches=%u\n",failures?"FAIL":record_only?"RECORDED":"PASS",unit,failures);return failures?1:0;
}
