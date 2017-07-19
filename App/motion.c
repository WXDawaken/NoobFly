#include "motion.h"
float g[3]={0.0f,0.0f,1.0};
void Motion_Init(float *q,float *e)
{
   euler2q(e[0],e[1],e[2],q);
}
void Motion_Detect(float *a,float *m,float *w,float *q,float *e)
{
	 float r_w[3],vg[3],h[3],b[3],ex[3],w_t[3];
	 r_w[0]=w[0]/180*PI;
	 r_w[1]=w[1]/180*PI;
	 r_w[2]=w[2]/180*PI;
   normalize(a,3);
	 normalize(m,3);
	 N2B(q,g,vg);
	 B2N(q,m,h);
	 get_b(h,b);
	 N2B(q,b,w_t);
	 get_e(ex,a,vg,m,w_t);
	 w_adjust(r_w,ex);
	 update_q(q,r_w,DT);
	 normalize(q,4);
	 q2euler(q,e);
}