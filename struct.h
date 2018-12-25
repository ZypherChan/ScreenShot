#ifndef STRCUT_H
#define STRCUT_H
/*****************************************************************/
//Copyright (C), 2018-2019, CZHer, Ltd.                        
//[�� �� ��]   :  struct.h                            
//[��    ��]   :  ZypherChan   
//[��    ��]   :  chen_zehang@126.com 
//[��������]   :  2018-12-17 22:19:17                 
//[��    ��]   :  ���幫��ö��ֵ�ͺ�
/******************************************************************/ 

#define BUTTON_HEIGHT 25
#define BUTTON_WIDTH  25

#define LABEL_HEIGHT 20
#define LABEL_WIDTH 60

#define ITEM_LENGTH 13

#define MARGIN 10

//��ǰ����״̬
enum DrawStatus
{
	WAITDRAW = 0,
	DRAWINIG,
	DRAWEND,
};

//��굱ǰλ��
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

//��ӱ��״̬
enum DrawEditFlag
{
	DRAWLINE = 0,
	DRAWRECT,
	DRAWCIRCLE,
	DRAWTEXT,
	DRAWDRAG,
};


#endif