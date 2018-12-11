#include <stdlib.h> // card > pixar.ppm
#include <stdio.h>
#include <math.h>
#include <thread>
#include <unistd.h>
#include <fstream>      // std::fstream
#include <utility>


#define R return
#define O operator
typedef float F;
typedef int I;


class Thread_local {
    public:
        int thread_id;
        int w;
        int h;
        int s;
        char *array;
};

//DIRECTION
float d_x = -25;
float d_y = 5;
float d_z = 28;

//VECTOR
float v_x = -3;
float v_y = 4;
float v_z = 0;



struct V
{
    F x,y,z;
    V(F v=0)
    {
        x=y=z=v;
    }
    V(F a,F b,F c=0)
    {
        x=a;y=b;z=c;
    }
    V O+(V r)
    {
        R V(x+r.x,y+r.y,z+r.z);
    }
    V O*(V r)
    {
        R V(x*r.x,y*r.y,z*r.z);
    }
    F O%(V r)
    {
        R x*r.x+y*r.y+z*r.z;
    }
    V O!()
    {
        R*this*(1/sqrtf(*this%*this));
    }
};

F L(F l,F r)
{
    R l<r?l:r;
}

F U()
{
    R(F)rand()/RAND_MAX;
}

F B(V p,V l,V h)
{
    l=p+l*-1;
    h=h+p*-1;
    R-L(L(L(l.x,h.x),L(l.y,h.y)),L(l.z,h.z));
}

F S(V p,I&m)    /*This function handles the letters that make up PIXAR*/
{
    F d=1e9; // 1e9
    V f=p;      //handles word size
    f.z=0;
    char l[]="505_5W9W5_9_COC_AOEOA_E_IOQ_I_QOUOY_Y_]OWW[WaOa_aWeWa_e_cWiO"; /*"505_5W9W5_9_COC_AOEOA_E_IOQ_I_QOUOY_Y_]OWW[WaOa_aWeWa_e_cWiO"*/

    //add in "hire me" in smaller letters in front of pixar logo with 'name and email'

 //   f.x += 8;
//    f.y += 5;
    //f.z += .25;
    //letter rendering start  
    for(I i=0;i<60;i+=4)
    {
        V b=V(l[i]-79,l[i+1]-79)*.5,e=V(l[i+2]-79,l[i+3]-79)*.5+b*-1,o=f+(b+e*L(-L((b+f*-1)%e/(e%e),0),1))*-1;
        d=L(d,o%o);
    }
    d=sqrtf(d);V a[]={V(-11,6),V(11,6)};    //handles vert placement of curved shit (no depth)
    for(I i=2;i--;)
    {
        V o=f+a[i]*-1;
        d=L(d,o.x>0?fabsf(sqrtf(o%o)-2):(o.y+=o.y>0?-2:2,sqrtf(o%o)));
    }
    //letter rendering end
    
    //wall rendering
    d=powf(powf(d,8)+powf(p.z,8),.125)-.5;
    m=1;    //  back_wall    D  RIGHT Wall    ROOF              FLOOR        #of slat and spacing        light source?   slat width
    F r=L(-L(B(p,V(-30,-.5,-30),V(30,18,30)),B(p,V(-25,17,-25),V(25,20,25))),B(V(fmodf(fabsf(p.x),8),p.y,p.z),V(1.5,18.5,-25),V(6.5,20,30)));/*<- LIGHT (this deals with the light details ) 30,18,30*/
    if(r<d)
        d=r,m=2;
    F s=19.9-p.y;
    if(s<d)
        d=s,m=3;
    //end wall render
    R d;
}

I M(V o,V d,V&h,V&n)
{
    I m,s=0;
    F t=0,c;
    for(;t<100;t+=c)       //depth of path travel                                                                                                                   /*Depth of ray travel from camera and checks if any objects are intercepted*/
        if((c=S(h=o+d*t,m))<.01||++s>99)
            R n=!V(S(h+V(.01,0),s)-c,S(h+V(0,.01),s)-c,S(h+V(0,0,.01),s)-c),m;
    R 0;
}

V T(V o,V d)
{
    V h,n,r,t=1,l(!V(.6,.6,1));
    for(I b=3;b--;)
    {
        I m=M(o,d,h,n);
        if(!m)
            break;
        if(m==1)
        {
            d=d+n*(n%d*-2);
            o=h+d*.1;
            t=t*.2;
        }
        if(m==2)
        {
            F i=n%l,p=6.283185*U(),c=U(),s=sqrtf(1-c),g=n.z<0?-1:1,u=-1/(g+n.z),v=n.x*n.y*u;
            d=V(v,g+n.y*n.y*u,-n.y)*(cosf(p)*s)+V(1+g*n.x*n.x*u,g*v,-g*n.x)*(sinf(p)*s)+n*sqrtf(c);
            o=h+d*.1;
            t=t*.2;
            if(i>0&&M(h+n*.1,l,h,n)==3)
                r=r+t*V(500,400,100)*i;
        }
        if(m==3)
        {
            r=r+t*V(50,80,100);        /* changing end picture color maybe hue ?*/
            break;
        }
    }
    R r;
}


//figure out how to use unsigned short
I old_main(Thread_local& data)
{
    char *array = (char *)data.array;
    int w = data.w;
    int h = data.h;
    int s = data.s;
    int number;
   int count = (w * data.thread_id * 3);
    dprintf(2, "Memory place: %d\tID: %d\n", count, data.thread_id);
                            //e is cam locationa ////g is direction of the cmaera //l! is the rotation
    /*I w=400, h=240,s=16;*/V e(d_x, d_y, d_z),g=!(V(v_x, v_y, v_z)+e*-1),l=!V(g.z,0,-g.x)*(1./w),u(g.y*l.z-g.z*l.y,g.z*l.x-g.x*l.z,g.x*l.y-g.y*l.x);
    int y= (h - data.thread_id);
    while( y > 0)
    {
        dprintf(2,"%d\tcount:%d\t ID:%d\n", y, count, data.thread_id);
        for(I x=w;x--;)
        {
            V c;
            for(I p=s;p--;)
                c=c+T(e,!(g+l*(x-w/2+U())+u*(y-h/2+U())));
            c=c*(1./s)+14./241;
            V o=c+1;
            c=V(c.x/o.x,c.y/o.y,c.z/o.z)*255;
            
            number = (int)c.x;
            array[count++] = (unsigned char)number;
            number = (int)c.y;
            array[count++] = (unsigned char)number;
            number = (int)c.z;
            array[count++] = (unsigned char)number;
            //printf("%c%c%c",(int)c.x,(int)c.y,(int)c.z);
        }
        count += ((w * 3) * 3);
        y -= 4;
    }
    dprintf(2, "Memory end: %d\tID:%d\n", count, data.thread_id);
}

int     threader(std::ofstream &file)
{
    int w=720, h=480, s=1424;
    char *array;
    char header[] = "P6 720 480 255 ";

    array = (char*)malloc((sizeof(char) * 3)* (w * h));

    Thread_local first;
    first.w = w;
    first.h = h;
    first.s = s;
    first.array = array;//[((150 * w) * 3)];
    first.thread_id = 0;
    //first.set_values(w, h, s, 0, 99, array);

    Thread_local second;
    second.w = w;
    second.h = h;
    second.s = s;
    second.array = array;//[((100 * w) * 3)];
    second.thread_id = 1;
    //second.set_values(w, h, s, 100, 199, array);
    
    Thread_local third;
    third.w = w;
    third.h = h;
    third.s = s;
    third.array = array;                //[((50 * w) * 3)];
    third.thread_id = 2;
    
    //third.set_values(w, h, s, 200, 299, array);

    
    Thread_local fourth;
    fourth.w = w;
    fourth.h = h;
    fourth.s = s;
    fourth.array = array;
    fourth.thread_id = 3;
   // fourth.set_values(w, h, s, 300, 400, array);
    std::thread t1(old_main, std::ref(first));
    std::thread t2(old_main, std::ref(second));
    std::thread t3(old_main, std::ref(third));
    std::thread t4(old_main, std::ref(fourth));
    
    t4.join();
    t3.join();
    t2.join();
    t1.join();

    file.write (header, strlen(header));
  //  write(1, &header, strlen(header));
    int i = 0; //= ((w * h) * 3);
    while (i <= h)
    {
        file.write (&array[i * (w * 3)], (w * 3));
        i++;
    }
}

/*
//DIRECTION
float d_x = -25;
float d_y = 5;
float d_z = 28;

//VECTOR
float v_x = -3;
float v_y = 4;
float v_z = 0;
*/

int main(void)
{
   int frame_count = 1500;
    std::string pre("picture_");
    std::string post(".ppm");
    std::string file_name;
    std::ofstream file;


    int i = 1;
    d_z = -1;
    while (i <= frame_count)
    {
        
        file_name = (std::to_string(1000+i) + post);
        file.open(file_name);

        threader(file);
        if (d_z <28)
            d_z += .3;
        else if (d_x < 5)
            d_x += .3;
        else if (d_y > 0)
            d_y -= .3;
        else
            break;
        file.close();
        i++;
    }
    file.close();
    return(0);
}
//  g++ moving_version.cpp -lpthread -std=c++11 -O3