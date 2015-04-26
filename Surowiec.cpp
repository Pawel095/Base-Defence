#include "Surowiec.h"

Surowiec::Surowiec(){

}
void Surowiec::render(){
    if(isVisible){
        renderTexture(tex,ren,dim.x,dim.y);
    }
}
