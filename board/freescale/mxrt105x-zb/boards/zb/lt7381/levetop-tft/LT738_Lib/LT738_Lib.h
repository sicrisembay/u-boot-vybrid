#ifndef _LT738_Lib_H
#define _LT738_Lib_H
#include "LT738.h"
#include "BSP_tftLT7381.h"

#define XI_4M    0
#define XI_8M    0
#define XI_10M   1
#define XI_12M   0

//��Ļ�ֱ���
#define LCD_XSIZE_TFT    320	
#define LCD_YSIZE_TFT    240

//��Ļ�������ο���Ļ�ֲ��޸�
//����
#define LCD_VBPD		 		 16
#define LCD_VFPD	 	 		 4
#define LCD_VSPW		     2
#define LCD_HBPD		     60
#define LCD_HFPD		     40
#define LCD_HSPW	   	   2

#define color256_black   0x00
#define color256_white   0xff
#define color256_red     0xe0
#define color256_green   0x1c
#define color256_blue    0x03
#define color256_yellow  color256_red|color256_green
#define color256_cyan    color256_green|color256_blue
#define color256_purple  color256_red|color256_blue

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

void LT738_Init(void);

/* Í¨¹ýÉèÖÃ¹¤×÷´°¿ÚµÄ·½·¨Ãèµã»­Í¼£¨¿í¶È±ØÐëÎª4µÄ±¶Êý£© */
void MPU_Memory_Write_Window_8bit(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);

/* Í¨¹ý¸Ä±ä×ø±êµÄ·½·¨Ãèµã»­Í¼ */
void MPU_Memory_Write_Pixel_8bit(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);

/* MCU¶ÁÈ¡SPI flashÃèµã»­Í¼ */
void Memory_Write_Flash(unsigned short x,unsigned short y,unsigned short w,unsigned short h,unsigned long addr);

/* Ð´Êý¾Ýµ½ÄÚ´æ */
void MPU8_8bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);//µ¥Æ¬»ú8Î»½Ó¿Ú£¬ÑÕÉ«Éî¶È8bpp
void MPU8_16bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);//µ¥Æ¬»ú8Î»½Ó¿Ú£¬ÑÕÉ«Éî¶È16bpp
void MPU8_24bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);//µ¥Æ¬»ú8Î»½Ó¿Ú£¬ÑÕÉ«Éî¶È24bpp
void MPU16_16bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned short *data);//µ¥Æ¬»ú16Î»½Ó¿Ú£¬ÑÕÉ«Éî¶È16bpp
void MPU16_24bpp_Mode1_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned short *data);//µ¥Æ¬»ú16Î»½Ó¿Ú£¬ÑÕÉ«Éî¶È24bpp
void MPU16_24bpp_Mode2_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned short *data);//µ¥Æ¬»ú16Î»½Ó¿Ú£¬ÑÕÉ«Éî¶È24bpp

/* Ӳ�����߶� */
void LT738_DrawLine(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long LineColor);
void LT738_DrawLine_Width(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long LineColor,unsigned short Width);
#endif
/* Ӳ����Բ */
void LT738_DrawCircle(unsigned short XCenter,unsigned short YCenter,unsigned short R,unsigned long CircleColor);
void LT738_DrawCircle_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short R,unsigned long ForegroundColor);
void LT738_DrawCircle_Width(unsigned short XCenter,unsigned short YCenter,unsigned short R,unsigned long CircleColor,unsigned long ForegroundColor,unsigned short Width);
#if 0
/* Ӳ������Բ */
void LT738_DrawEllipse(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long EllipseColor);
void LT738_DrawEllipse_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawEllipse_Width(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long EllipseColor,unsigned long ForegroundColor,unsigned short Width);   

/* Ӳ�������� */
void LT738_DrawSquare(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long SquareColor);
void LT738_DrawSquare_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long ForegroundColor);
void LT738_DrawSquare_Width(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long SquareColor,unsigned long ForegroundColor,unsigned short Width);

/* Ӳ����Բ�Ǿ��� */
void LT738_DrawCircleSquare(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X_R,unsigned short Y_R,unsigned long CircleSquareColor);
void LT738_DrawCircleSquare_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawCircleSquare_Width(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X_R,unsigned short Y_R,unsigned long CircleSquareColor,unsigned long ForegroundColor,unsigned short Width);

/* Ӳ���������� */
void LT738_DrawTriangle(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long TriangleColor);
void LT738_DrawTriangle_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long ForegroundColor);
void LT738_DrawTriangle_Frame(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long TriangleColor,unsigned long ForegroundColor);

/* Ӳ�����ı��� */
void LT738_DrawQuadrilateral(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned long ForegroundColor);
void LT738_DrawQuadrilateral_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned long ForegroundColor);
void LT738_DrawTriangle_Frame(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long TriangleColor ,unsigned long ForegroundColor);

/* Ӳ��������� */
void LT738_DrawPentagon(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned short X5,unsigned short Y5,unsigned long ForegroundColor);
void LT738_DrawPentagon_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned short X5,unsigned short Y5,unsigned long ForegroundColor);

/* Ӳ�������� */
void LT738_DrawLeftUpCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_DrawLeftDownCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_DrawRightUpCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_DrawRightDownCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_SelectDrawCurve(unsigned short XCenter ,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor,unsigned short  Dir);

/* Ӳ����1/4��Բ */
void LT738_DrawLeftUpCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawLeftDownCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawRightUpCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawRightDownCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_SelectDrawCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor,unsigned short  Dir);

/* Ӳ����Բ�� */
unsigned char LT738_DrawCylinder(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned short H,unsigned long CylinderColor,unsigned long ForegroundColor);

/* Ӳ���������� */
void LT738_DrawQuadrangular(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned short X5,unsigned short Y5,unsigned short X6,unsigned short Y6,unsigned long QuadrangularColor,unsigned long ForegroundColor);

/* ��� */
void LT738_MakeTable(unsigned short X1,unsigned short Y1,unsigned short W,unsigned short H,unsigned short Line,unsigned short Row,unsigned long  TableColor,unsigned long  ItemColor,unsigned long  ForegroundColor,unsigned short width1,unsigned short width2,unsigned char  mode);

/* ʹ���ڽ��ֿ� */
void LT738_Select_Internal_Font_Init(unsigned char Size,unsigned char XxN,unsigned char YxN,unsigned char ChromaKey,unsigned char Alignment);
void LT738_Print_Internal_Font_String(unsigned short x,unsigned short y,unsigned long FontColor,unsigned long BackGroundColor ,char *c);

/* ʹ���⽨�ֿ� */
void LT738_Select_Outside_Font_Init(unsigned char SCS,unsigned char Clk,unsigned long FlashAddr,unsigned long MemoryAddr,unsigned long DisplayAddr,unsigned long Num,unsigned char Size,unsigned char XxN,unsigned char YxN,unsigned char ChromaKey,unsigned char Alignment);
void LT738_Print_Outside_Font_String(unsigned short x,unsigned short y,unsigned long FontColor,unsigned long BackGroundColor,unsigned char *c);
void LT738_Print_OutsideFont_Giant(unsigned char SCS,unsigned char Clk,unsigned long FlashAddr,unsigned long MemoryAddr,unsigned long ShowAddr,unsigned short width,unsigned char Size,unsigned char ChromaKey,unsigned short x,unsigned short y,unsigned long FontColor,unsigned long BackGroundColor,unsigned short w,unsigned short s,unsigned char *c);

/* ���ֹ�� */
void LT738_Text_cursor_Init(unsigned char On_Off_Blinking,unsigned short Blinking_Time,unsigned short X_W,unsigned short Y_W);
void LT738_Enable_Text_Cursor(void);
void LT738_Disable_Text_Cursor(void);

/*  ͼ���� */
void LT738_Graphic_cursor_Init(unsigned char Cursor_N,unsigned char Color1,unsigned char Color2,unsigned short X_Pos,unsigned short Y_Pos,unsigned char *Cursor_Buf);
void LT738_Set_Graphic_cursor_Pos(unsigned char Cursor_N,unsigned short X_Pos,unsigned short Y_Pos);
void LT738_Enable_Graphic_Cursor(void);
void LT738_Disable_Graphic_Cursor(void);

/* ���鴫�����棨BitBLT�� */
void BTE_Solid_Fill(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short color,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Memory_Copy(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long S1_Addr,unsigned short S1_W,unsigned short XS1,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned int ROP_Code,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Memory_Copy_Chroma_key(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned long Background_color,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Pattern_Fill(unsigned char P_8x8_or_16x16,unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long Des_Addr,unsigned short Des_W, unsigned short XDes,unsigned short YDes,unsigned int ROP_Code ,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Pattern_Fill_With_Chroma_key(unsigned char P_8x8_or_16x16,unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned int ROP_Code,unsigned long Background_color,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_MCU_Write_MCU_16bit(unsigned long S1_Addr,unsigned short S1_W,unsigned short XS,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned int ROP_Code,unsigned short X_W,unsigned short Y_H ,const unsigned short *data);
void LT738_BTE_MCU_Write_Chroma_key_MCU_16bit(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned long Background_color,unsigned short X_W,unsigned short Y_H,const unsigned short *data);
void LT738_BTE_MCU_Write_ColorExpansion_MCU_16bit(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H,unsigned long Foreground_color ,unsigned long Background_color ,const unsigned short *data);
void LT738_BTE_MCU_Write_ColorExpansion_Chroma_key_MCU_16bit(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H,unsigned long Foreground_color,const unsigned short *data);
void BTE_Alpha_Blending(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long S1_Addr,unsigned short S1_W,unsigned short XS1,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H,unsigned char alpha);
void BTE_PNG(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long S1_Addr,unsigned short S1_W,unsigned short XS1,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H);							

/*  PIP */
void LT738_PIP_Init(unsigned char On_Off,unsigned char Select_PIP,unsigned long PAddr,unsigned short XP,unsigned short YP,unsigned long ImageWidth,unsigned short X_Dis,unsigned short Y_Dis,unsigned short X_W,unsigned short Y_H);
void LT738_Set_DisWindowPos(unsigned char On_Off,unsigned char Select_PIP,unsigned short X_Dis,unsigned short Y_Dis);

/*  PWM */
void LT738_PWM0_Init(unsigned char on_off,unsigned char Clock_Divided,unsigned char Prescalar,unsigned short Count_Buffer,unsigned short Compare_Buffer);
void LT738_PWM1_Init(unsigned char on_off,unsigned char Clock_Divided,unsigned char Prescalar,unsigned short Count_Buffer,unsigned short Compare_Buffer);
void LT738_PWM0_Duty(unsigned short Compare_Buffer);
void LT738_PWM1_Duty(unsigned short Compare_Buffer);

/*  Standby Mode */
void LT738_Standby(void);
void LT738_Wkup_Standby(void);

/* Suspend Mode */
void LT738_Suspend(void);
void LT738_Wkup_Suspend(void);

/* Sleep Mode */
void LT738_SleepMode(void);
void LT738_Wkup_Sleep(void);

#endif
