#ifndef STRCUT_H
#define STRCUT_H
/*****************************************************************/
//Copyright (C), 2018-2019, CZHer, Ltd.                        
//[文 件 名]   :  struct.h                            
//[作    者]   :  ZypherChan   
//[邮    箱]   :  chen_zehang@126.com 
//[创建日期]   :  2018-12-17 22:19:17                 
//[描    述]   :  定义公共枚举值和宏
/******************************************************************/ 

#define BUTTON_HEIGHT 25
#define BUTTON_WIDTH  25

#define LABEL_HEIGHT 20
#define LABEL_WIDTH 60

#define ITEM_LENGTH 13

#define MARGIN 10

//当前绘制状态
enum DrawStatus
{
	WAITDRAW = 0,
	DRAWINIG,
	DRAWEND,
};

//鼠标当前位置
enum CursorLocation
{
	TOPLEFT = 0,
	TOP,
	TOPRIGHT,
	LEFT,
	MIDDLE,
	RIGHT,
	BUTTOMLEFT,
	BUTTOM,
	BUTTOMRIGHT,
	INVALID = -1,
};

//添加标记状态
enum DrawEditFlag
{
	DRAWLINE = 0,
	DRAWRECT,
	DRAWCIRCLE,
	DRAWTEXT,
	DRAWDRAG,
};


#endif