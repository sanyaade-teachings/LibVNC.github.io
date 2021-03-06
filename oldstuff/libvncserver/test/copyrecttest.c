#include <rfb/rfb.h>
#include <math.h>

void initBackground(rfbScreenInfoPtr server)
{
  unsigned int i,j;

  for(i=0;i<server->width;i++)
    for(j=0;j<server->height;j++) {
      server->frameBuffer[(j*server->width+i)*4+0]=i&0xff;
      server->frameBuffer[(j*server->width+i)*4+1]=j&0xff;
      server->frameBuffer[(j*server->width+i)*4+2]=(i*j)&0xff;
    }
}

int main(int argc,char** argv)
{             
  int width=400,height=300,w=20,x,y;
  double r,phi;
  
  rfbScreenInfoPtr server=rfbGetScreen(&argc,argv,width,height,8,3,4);
  server->frameBuffer=(char*)malloc(width*height*4);
  initBackground(server);
  server->deferUpdateTime=0;
  rfbInitServer(server);

  r=0;
  while(1) {
    if(r<=0) {
      initBackground(server);
      rfbMarkRectAsModified(server,0,0,width,height);
      r=0.43;
      phi=0;
    } else {
      r-=0.0001;
      phi+=0.02;
      if(phi>2*M_PI)
	phi-=2*M_PI;
    }
    x=width*(0.5+cos(phi)*r);
    y=height*(0.5+sin(phi)*r);
    if(x>=0 && y>=0 && x+w<=width && y+w<=height) {
      unsigned int dx=width*0.5*(1-cos(phi)*r)-x,
      		dy=height*0.5*(1-sin(phi)*r)-y;
      rfbDoCopyRect(server,x,y,x+w,y+w,-dx,-dy);
    }
    rfbProcessEvents(server,50000);
  }
  return(0);
}
