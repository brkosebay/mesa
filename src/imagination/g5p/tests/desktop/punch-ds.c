/* SPDX-License-Identifier: MIT
 * Generated-by: LLM. Exact binary-colour witnesses for discard/DS ordering.
 * --record submits over the offline fake; it never certifies rendered pixels.
 */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define W 64
#define H 64
static int record;
static GLuint shader(GLenum stage,const char *src) {
 GLuint s=glCreateShader(stage);glShaderSource(s,1,&src,NULL);glCompileShader(s);
 GLint ok=0;glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
 if(!ok){char log[2048];glGetShaderInfoLog(s,sizeof(log),NULL,log);fprintf(stderr,"shader: %s\n",log);exit(1);}return s;
}
static GLuint program(int kill) {
 const char *vs="#version 300 es\nlayout(location=0) in vec4 p; void main(){gl_Position=p;}";
 const char *f0="#version 300 es\nprecision highp float; uniform vec4 c; out vec4 o; void main(){o=c;}";
 const char *f1="#version 300 es\nprecision highp float; uniform vec4 c; out vec4 o; void main(){if(gl_FragCoord.x<32.)discard;o=c;}";
 const char *f2="#version 300 es\nprecision highp float; uniform vec4 c; out vec4 o; void main(){if(gl_FragCoord.x<16.)discard;if(gl_FragCoord.y<16.)discard;o=c;}";
 GLuint v=shader(GL_VERTEX_SHADER,vs), f=shader(GL_FRAGMENT_SHADER,kill==2?f2:kill?f1:f0),p=glCreateProgram();
 glAttachShader(p,v);glAttachShader(p,f);glLinkProgram(p);GLint ok=0;glGetProgramiv(p,GL_LINK_STATUS,&ok);
 if(!ok){char log[2048];glGetProgramInfoLog(p,sizeof(log),NULL,log);fprintf(stderr,"link: %s\n",log);exit(1);}
 glDeleteShader(v);glDeleteShader(f);return p;
}
static void draw(GLuint p,float z,int red) {
 const float v[]={-1,-1,z,1, 3,-1,z,1, -1,3,z,1};
 glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STREAM_DRAW);glUseProgram(p);
 glUniform4f(glGetUniformLocation(p,"c"),red?1:0,0,red?0:1,1);glDrawArrays(GL_TRIANGLES,0,3);
}
static int run(const char *name) {
 int two=strstr(name,"two")!=NULL,stencil=strstr(name,"stencil")!=NULL;
 int mask0=strstr(name,"mask0")!=NULL,blend=strstr(name,"blend")!=NULL;
 int load=strstr(name,"load")!=NULL,order=strstr(name,"order")!=NULL;
 int readonly=strstr(name,"readonly")!=NULL;
 GLuint p=program(0),k=program(two?2:1),tex,fb,rb,vb;
 glGenTextures(1,&tex);glBindTexture(GL_TEXTURE_2D,tex);glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA8,W,H);
 glGenFramebuffers(1,&fb);glBindFramebuffer(GL_FRAMEBUFFER,fb);glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex,0);
 glGenRenderbuffers(1,&rb);glBindRenderbuffer(GL_RENDERBUFFER,rb);glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,W,H);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rb);
 if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)return 1;
 glGenBuffers(1,&vb);glBindBuffer(GL_ARRAY_BUFFER,vb);glEnableVertexAttribArray(0);glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
 glViewport(0,0,W,H);glDisable(GL_DITHER);glClearColor(0,0,1,1);glClearDepthf(1);glClearStencil(0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
 glEnable(GL_DEPTH_TEST);glDepthFunc(GL_ALWAYS);if(!strstr(name,"first"))draw(p,0,0); /* blue at window depth .5 */
 if(stencil){glDisable(GL_DEPTH_TEST);glEnable(GL_STENCIL_TEST);glStencilFunc(GL_ALWAYS,1,255);glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);}
 else {glDepthFunc(GL_LESS);glDepthMask(readonly?GL_FALSE:GL_TRUE);}
 if(mask0)glColorMask(0,0,0,0);
 if(blend){glEnable(GL_BLEND);glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);}
 draw(k,-.5f,1); /* retained red at .25, killed blue remains at .5 */
 glDisable(GL_BLEND);glColorMask(1,1,1,1);
 if(load)glFinish();
 if(stencil){glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);glStencilFunc(mask0?GL_EQUAL:GL_NOTEQUAL,1,255);draw(p,-.2f,mask0?1:0);}
 else if(order){glDepthMask(GL_FALSE);draw(p,-.2f,0);} /* blue probe at .4: must pass only where kill kept .5 */
 uint8_t pixels[W*H*4];glReadPixels(0,0,W,H,GL_RGBA,GL_UNSIGNED_BYTE,pixels);GLenum e=glGetError();unsigned bad=0;
 for(int y=0;y<H;y++)for(int x=0;x<W;x++) {
  int kept=two?(x>=16&&y>=16):x>=32;
  int red=kept&&(!readonly||!order);
  const uint8_t want[]={red?255:0,0,red?0:255,255};
  if(memcmp(pixels+4*(y*W+x),want,4)&&bad++<4&&!record)
   printf("PUNCH-DIFF %s %d,%d got=%02x%02x%02x%02x expected=%02x%02x%02x%02x\n",name,x,y,pixels[4*(y*W+x)],pixels[4*(y*W+x)+1],pixels[4*(y*W+x)+2],pixels[4*(y*W+x)+3],want[0],want[1],want[2],want[3]);
 }
 printf("PUNCH-RUNG %s %s bad=%u pixels=%u gl=%#x\n",name,record?"RECORDED":(!bad&&!e)?"PASS":"FAIL",bad,W*H,e);
 glDeleteBuffers(1,&vb);glDeleteRenderbuffers(1,&rb);glDeleteFramebuffers(1,&fb);glDeleteTextures(1,&tex);glDeleteProgram(k);glDeleteProgram(p);
 return e||(!record&&bad);
}
int main(int argc,char **argv) {
 setvbuf(stdout,NULL,_IOLBF,0);int arg=1;if(argc>1&&!strcmp(argv[1],"--record")){record=1;arg++;}
 if(argc!=arg+1){fprintf(stderr,"usage: punch-ds [--record] RUNG\n");return 2;}
 const char *names[]={"depth-write","depth-two-first","depth-readonly","depth-order","depth-order-load","depth-two-order","depth-blend-order","stencil-colour","stencil-mask0-blend","stencil-mask0-blend-load","stencil-blend","stencil-two-mask0-blend"};
 int found=0;for(unsigned i=0;i<sizeof(names)/sizeof(names[0]);i++)found|=!strcmp(argv[arg],names[i]);if(!found)return 2;
 PFNEGLGETPLATFORMDISPLAYEXTPROC get=(void*)eglGetProcAddress("eglGetPlatformDisplayEXT");
 EGLDisplay d=get?get(EGL_PLATFORM_SURFACELESS_MESA,EGL_DEFAULT_DISPLAY,NULL):EGL_NO_DISPLAY;EGLint ma,mi;
 if(d==EGL_NO_DISPLAY||!eglInitialize(d,&ma,&mi)||!eglBindAPI(EGL_OPENGL_ES_API))return 1;
 const EGLint a[]={EGL_CONTEXT_MAJOR_VERSION,3,EGL_CONTEXT_MINOR_VERSION,0,EGL_NONE};EGLContext c=eglCreateContext(d,EGL_NO_CONFIG_KHR,EGL_NO_CONTEXT,a);
 if(c==EGL_NO_CONTEXT||!eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,c))return 1;
 printf("PUNCH-IDENTITY version=%s renderer=%s\n",glGetString(GL_VERSION),glGetString(GL_RENDERER));
 int rc=run(argv[arg]);eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(d,c);eglTerminate(d);return rc;
}
