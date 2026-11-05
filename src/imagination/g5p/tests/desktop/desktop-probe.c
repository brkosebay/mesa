/* SPDX-License-Identifier: MIT
 * Generated-by: LLM. Advertisement/profile probe only; does not claim rendering support. */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <stdio.h>
#include <string.h>
typedef const unsigned char *(*string_fn)(unsigned);
typedef const unsigned char *(*stringi_fn)(unsigned,unsigned);
typedef void (*integer_fn)(unsigned,int *);
int main(void) {
 EGLDisplay d=eglGetDisplay(EGL_DEFAULT_DISPLAY);EGLint major,minor;
 if(!eglInitialize(d,&major,&minor)){fprintf(stderr,"EGL initialize error=%#x\n",eglGetError());return 1;}
 if(!eglBindAPI(EGL_OPENGL_API))return 1;
 string_fn str=(string_fn)eglGetProcAddress("glGetString");stringi_fn stri=(stringi_fn)eglGetProcAddress("glGetStringi");integer_fn integer=(integer_fn)eglGetProcAddress("glGetIntegerv");
 if(!str||!integer)return 1;
 const int versions[][2]={{4,6},{4,5},{4,4},{4,3},{4,2},{4,1},{4,0},{3,3},{3,2},{3,1},{3,0},{2,1}};
 for(unsigned profile=0;profile<2;profile++) {
  int reported=0;
  for(unsigned i=0;i<sizeof(versions)/sizeof(versions[0]);i++) {
   int ma=versions[i][0],mi=versions[i][1];if(!profile&&(ma<3||(ma==3&&mi<2)))continue;
   EGLint attrs[]={EGL_CONTEXT_MAJOR_VERSION,ma,EGL_CONTEXT_MINOR_VERSION,mi,EGL_CONTEXT_OPENGL_PROFILE_MASK,profile?EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT:EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,EGL_NONE};
   EGLContext c=eglCreateContext(d,NULL,EGL_NO_CONTEXT,attrs);
   if(c==EGL_NO_CONTEXT){printf("PROFILE-REQUEST %s %d.%d REFUSED egl=%#x\n",profile?"compat":"core",ma,mi,eglGetError());continue;}
   if(!eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,c)){fprintf(stderr,"make current error=%#x\n",eglGetError());return 1;}
   printf("PROFILE-REQUEST %s %d.%d CREATED GL_VERSION=%s\n",profile?"compat":"core",ma,mi,str(0x1f02));
   if(!reported) {
    printf("PROFILE-MAX %s GL_VERSION=%s renderer=%s GLSL=%s\n",profile?"compat":"core",str(0x1f02),str(0x1f01),str(0x8b8c));
    int count=0;integer(0x821d,&count);printf("EXTENSIONS %s",profile?"compat":"core");
    if(count>0&&stri)for(int j=0;j<count;j++)printf(" %s",stri(0x1f03,j));
    else {const unsigned char *e=str(0x1f03);if(e)printf(" %s",e);}puts("");
    const struct {const char *name;unsigned key;} limits[]={{"MAX_TEXTURE_SIZE",0x0d33},{"MAX_3D_TEXTURE_SIZE",0x8073},{"MAX_CUBE_MAP_TEXTURE_SIZE",0x851c},{"MAX_ARRAY_TEXTURE_LAYERS",0x88ff},{"MAX_DRAW_BUFFERS",0x8824},{"MAX_SAMPLES",0x8d57},{"MAX_VERTEX_UNIFORM_BLOCKS",0x8a2b},{"MAX_VERTEX_TEXTURE_IMAGE_UNITS",0x8b4c}};
    for(unsigned j=0;j<sizeof(limits)/sizeof(limits[0]);j++){int v=-1;integer(limits[j].key,&v);printf("LIMIT %s %s %d\n",profile?"compat":"core",limits[j].name,v);}reported=1;
   }
   eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(d,c);
  }
  if(!reported)printf("PROFILE-MAX %s NONE\n",profile?"compat":"core");
 }
 eglTerminate(d);return 0;
}
