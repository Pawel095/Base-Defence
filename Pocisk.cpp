#include "Pocisk.h"

Pocisk::Pocisk()
{
    P_x = 0;
    P_y = 0;
    P_moveToX = 0;
    P_moveToY = 0;
}
void Pocisk::render(){
    if(isVisible==true){
        if(isPaused==false){
        renderRotatedTexture(P_tex,P_ren,P_x,P_y,P_angle,NULL);
        return void();
    }

    if ((int)P_x < P_moveToX){
        P_x+=dx;
    }else if ((int)P_x > P_moveToX){
		P_x-=dx;
    }
	if ((int)P_y < P_moveToY){
        P_y+=dy;
	}else if ((int)P_y>P_moveToY){
		P_y-=dy;
	}

    renderRotatedTexture(P_tex,P_ren,(int)P_x,(int)P_y,P_angle,NULL);
    }
}
void Pocisk::licz(){


    cout<<" "<<dx<<" "<<dy<<endl;
}
