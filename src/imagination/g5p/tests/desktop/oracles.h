/* SPDX-License-Identifier: MIT
 * Generated-by: LLM. CPU references; no GL or driver code. */
#ifndef G5P_GL32_ORACLES_H
#define G5P_GL32_ORACLES_H
#include <stdint.h>

/* Each nibble is the Boolean truth table for one GL logic op (CLEAR..SET).
 * Index its bit with (source_bit << 1) | destination_bit. */
static inline uint8_t logic_reference(unsigned op, uint8_t source, uint8_t dest)
{
 static const uint8_t table[16]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};
 unsigned result=0;
 for(unsigned bit=0;bit<8;bit++) {
  unsigned index=(((unsigned)source>>bit)&1)*2+(((unsigned)dest>>bit)&1);
  result|=((table[op]>>index)&1)<<bit;
 }
 return (uint8_t)result;
}

/* u2/v2 are exactly twice the normalized coordinates of a constant 2x2
 * texture. With these integer coordinates all linear taps weigh exactly1/4.
 * Return how many quarters of the result come from the border. */
static inline unsigned border_weight(unsigned linear, int u2, int v2)
{
 if(!linear)return u2<0||u2>=2||v2<0||v2>=2?4:0;
 unsigned outside=0;
 for(int y=v2-1;y<=v2;y++)for(int x=u2-1;x<=u2;x++)
  outside+=(unsigned)(x<0||x>=2||y<0||y>=2);
 return outside;
}
static inline uint8_t border_reference(unsigned weight, uint8_t border, uint8_t inside)
{
 return (uint8_t)((weight*border+(4-weight)*inside)/4);
}
/* Deliberately use integer-exact endpoint interpolants; the boundary phase uses
 * only endpoint selectors so signed -128 normalization is not mixed with rounding. */
static inline void rgtc_reference_table(int sign,int phase,int t[8]) {
 int a=phase==0?(sign?112:224):phase==1?(sign?-100:0):phase==2?(sign?112:224):(sign?-128:0);
 int b=phase==0?(sign?-112:0):phase==1?(sign?100:200):phase==2?(sign?-112:0):(sign?127:255);
 if(sign&&a==-128)a=-127;
 if(sign&&b==-128)b=-127;
 t[0]=a;t[1]=b;
 if(a>b)for(int k=1;k<=6;k++)t[k+1]=((7-k)*a+k*b)/7;
 else {for(int k=1;k<=4;k++)t[k+1]=((5-k)*a+k*b)/5;t[6]=sign?-127:0;t[7]=sign?127:255;}
}
#endif
