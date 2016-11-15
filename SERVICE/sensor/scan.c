#include "scan.h"
#include "stdlib.h"
#include "sensor.h"

#define IS_SCAN_OVER(_self_)  (_self_->cur.x > _self_->end.x || _self_->cur.y > _self_->end.y)

static void _Scan_Restart(Scan *thiz){
    thiz->cur = thiz->beg;
    thiz->over = 0;
}

static u16 _Scan_Point(Scan *thiz, Point point)
{   
    u16 res;
    Sensor_Select(point.x, point.y);
    res = 0x0fff - thiz->bundle_adc->read();
    //res = Sensor_Get_Code();
    //res = point.x;
	return res;
}

__inline void MOVE_2_NEXT(Scan *thiz)
{
    if(thiz->mode == SCAN_MODE_Y_FIRST) 
    {
        if(thiz->cur.y >= thiz->end.y)
        {
            thiz->cur.y = thiz->beg.y;
            thiz->cur.x++;
        }else
        {
			thiz->cur.y++;
        }
    }
    else if(thiz->mode == SCAN_MODE_X_FIRST)
    {
        if(thiz->cur.x >= thiz->end.x)
        {
			thiz->cur.x = thiz->beg.x;
			thiz->cur.y++;
        }else
        {
			thiz->cur.x++;
        } 
    }   
}

static u16 _Scan_Step(Scan *thiz)
{  
    u16 res          = 0;    
    Buffer *buffer = thiz->buffer;

    if(IS_SCAN_OVER((thiz))){
        thiz->over = 1;
        if(thiz->loop){
            _Scan_Restart(thiz);
            thiz->over = 0;
        }
        return _Scan_Point(thiz, thiz->end);  
    }
      
    res = _Scan_Point(thiz, thiz->cur);
    if(buffer){
        buffer->put(buffer, res >> 8);
        buffer->put(buffer, res);
    }
    MOVE_2_NEXT(thiz);
    
    return res;
}

static void _Scan_Start(Scan *thiz){
    thiz->bundle_idac->start();
}

static void _Scan_Stop(Scan *thiz){
    thiz->bundle_idac->stop();
}

Scan* Scan_Create(void){
    Scan *thiz      = (Scan*)malloc(sizeof(Scan));
    thiz->point     = _Scan_Point;
    thiz->step      = _Scan_Step;
    thiz->restart   = _Scan_Restart;
    thiz->start     = _Scan_Start;
    thiz->stop      = _Scan_Stop;
    return thiz;
}

void Scan_Destroy(Scan* thiz){
    free(thiz);
}
