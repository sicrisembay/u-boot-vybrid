#include "LT738_Lib.h"

unsigned char CCLK;    // LT738���ں�ʱ��Ƶ��    
unsigned char MCLK;    // SDRAM��ʱ��Ƶ��
unsigned char SCLK;    // LCD��ɨ��ʱ��Ƶ��

//---------------------------------------------------------------------------------------------------------------------------------
void LT738_HW_Reset(void)
{
    /* Reset LCD Controller */
	BSP_TFT_LT7381_Reset(0);
	Delay_ms(500);
	BSP_TFT_LT7381_Reset(1);
	Delay_ms(500);
}

//���LT738ϵͳ
void System_Check_Temp(void)
{
	unsigned char i=0;
	unsigned char temp=0;
	unsigned char system_ok=0;
	do
	{
		if((LCD_StatusRead()&0x02)==0x00)    
		{
			Delay_ms(1);                  //��MCU �ٶ�̫�죬��Ҫ�rʹ��
			LCD_CmdWrite(0x01);
			Delay_ms(1);                  //��MCU �ٶ�̫�죬��Ҫ�rʹ��
			temp =LCD_DataRead();
			if((temp & 0x80)==0x80)       //���CCR�Ĵ���PLL�Ƿ�׼����
			{
				system_ok=1;
				i=0;
			}
			else
			{
				Delay_ms(1); //��MCU �ٶ�̫�죬��Ҫ�rʹ��
				LCD_CmdWrite(0x01);
				Delay_ms(1); //��MCU �ٶ�̫�죬��Ҫ�rʹ��
				LCD_DataWrite(0x80);
			}
		}
		else
		{
			system_ok=0;
			i++;
		}
		if(system_ok==0 && i==5)
		{
			LT738_HW_Reset(); //note1
			i=0;
		}
	}while(system_ok==0);
}

void LT738_PLL_Initial(void) 
{    
	unsigned int temp = 0;
	unsigned int temp1;
	
	unsigned short lpllOD_sclk, lpllOD_cclk, lpllOD_mclk;
	unsigned short lpllR_sclk, lpllR_cclk, lpllR_mclk;
	unsigned short lpllN_sclk, lpllN_cclk, lpllN_mclk;
	
	temp = (LCD_HBPD + LCD_HFPD + LCD_HSPW + LCD_XSIZE_TFT) * (LCD_VBPD + LCD_VFPD + LCD_VSPW+LCD_YSIZE_TFT) * 60;   
	
	temp1 = (temp%1000000)/100000;
	if(temp1>5)
		 temp = temp / 1000000 + 1;
	else temp = temp / 1000000;
	
	SCLK = temp;
	temp = temp * 3;
	MCLK = temp;
	CCLK = temp;
	
	if(CCLK > 100)	CCLK = 100;
	if(MCLK > 100)	MCLK = 100;
	if(SCLK > 65)		SCLK = 65;

	lpllOD_sclk = 2;
    lpllOD_cclk = 2;
    lpllOD_mclk = 2;
    lpllR_sclk  = 5;
    lpllR_cclk  = 5;
    lpllR_mclk  = 5;
    lpllN_mclk  = MCLK;
    lpllN_cclk  = CCLK;
    lpllN_sclk  = SCLK;

#if XI_4M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 2;
	lpllOD_mclk = 2;
	lpllR_sclk  = 2;
	lpllR_cclk  = 2;
	lpllR_mclk  = 2;
	lpllN_mclk  = MCLK;      
	lpllN_cclk  = CCLK;    
	lpllN_sclk  = 2*SCLK; 
	
#endif

#if XI_8M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 2;
	lpllOD_mclk = 2;
	lpllR_sclk  = 2;
	lpllR_cclk  = 4;
	lpllR_mclk  = 4;
	lpllN_mclk  = MCLK;      
	lpllN_cclk  = CCLK;    
	lpllN_sclk  = SCLK; 
	
#endif

#if XI_10M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 2;
	lpllOD_mclk = 2;
	lpllR_sclk  = 5;
	lpllR_cclk  = 5;
	lpllR_mclk  = 5;
	lpllN_mclk  = MCLK;      
	lpllN_cclk  = CCLK;    
	lpllN_sclk  = 2*SCLK; 
	
#endif

#if XI_12M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 2;
	lpllOD_mclk = 2;
	lpllR_sclk  = 3;
	lpllR_cclk  = 6;
	lpllR_mclk  = 6;
	lpllN_mclk  = MCLK;      
	lpllN_cclk  = CCLK;    
	lpllN_sclk  = SCLK; 
	
#endif
	
	LCD_CmdWrite(0x05);
	LCD_DataWrite((lpllOD_sclk<<6) | (lpllR_sclk<<1) | ((lpllN_sclk>>8)&0x1));
	LCD_CmdWrite(0x07);
	LCD_DataWrite((lpllOD_mclk<<6) | (lpllR_mclk<<1) | ((lpllN_mclk>>8)&0x1));
	LCD_CmdWrite(0x09);
	LCD_DataWrite((lpllOD_cclk<<6) | (lpllR_cclk<<1) | ((lpllN_cclk>>8)&0x1));

	LCD_CmdWrite(0x06);
	LCD_DataWrite(lpllN_sclk);
	LCD_CmdWrite(0x08);
	LCD_DataWrite(lpllN_mclk);
	LCD_CmdWrite(0x0a);
	LCD_DataWrite(lpllN_cclk);
      
	LCD_CmdWrite(0x00);
	Delay_us(1);
	LCD_DataWrite(0x80);

	Delay_ms(1);	//��PLLí
}

void LT738_SDRAM_initail(unsigned char mclk)
{
	unsigned short sdram_itv;

	LCD_RegisterWrite(0xe0,0x20);      
	LCD_RegisterWrite(0xe1,0x03);	//CAS:2=0x02�ACAS:3=0x03
	sdram_itv = (64000000 / 8192) / (1000/mclk) ;
	sdram_itv-=2;

	LCD_RegisterWrite(0xe2,sdram_itv);
	LCD_RegisterWrite(0xe3,sdram_itv >>8);
	LCD_RegisterWrite(0xe4,0x01);
	Check_SDRAM_Ready();
	Delay_ms(1);
}

void Set_LCD_Panel(void)
{
	//**[01h]**//   
	TFT_16bit();
	
	#if STM32_FSMC_8
	Host_Bus_8bit();    //��������8bit
	#else
	Host_Bus_16bit();	//��������16bit
	#endif
      
	//**[02h]**//
	RGB_16b_16bpp();
	MemWrite_Left_Right_Top_Down();	
	//MemWrite_Down_Top_Left_Right();
      
	//**[03h]**//
	Graphic_Mode();
	Memory_Select_SDRAM();
     
	PCLK_Falling();	       	//REG[12h]:ÏÂ½µÑØ 
	//PCLK_Rising();
	
	VSCAN_T_to_B();	        //REG[12h]:���ϵ���
	//VSCAN_B_to_T();				//���µ���
	
	PDATA_Set_RGB();        //REG[12h]:Select RGB output
	//PDATA_Set_RBG();
	//PDATA_Set_GRB();
	//PDATA_Set_GBR();
	//PDATA_Set_BRG();
	//PDATA_Set_BGR();

	HSYNC_Low_Active();     //REG[13h]:		  
	//HSYNC_High_Active();
	
	VSYNC_Low_Active();     //REG[13h]:			
	//VSYNC_High_Active();
	
	DE_High_Active();       //REG[13h]:	
	//DE_Low_Active();
 
	LCD_HorizontalWidth_VerticalHeight(LCD_XSIZE_TFT ,LCD_YSIZE_TFT);	
	LCD_Horizontal_Non_Display(LCD_HBPD);	                            
	LCD_HSYNC_Start_Position(LCD_HFPD);	                              
	LCD_HSYNC_Pulse_Width(LCD_HSPW);		                            	
	LCD_Vertical_Non_Display(LCD_VBPD);	                                
	LCD_VSYNC_Start_Position(LCD_VFPD);	                              
	LCD_VSYNC_Pulse_Width(LCD_VSPW);		                            	

	Memory_XY_Mode();	//Block mode (X-Y coordination addressing);��ģʽ
	Memory_16bpp_Mode();	
}

void LT738_initial(void)
{
	LT738_PLL_Initial();
	LT738_SDRAM_initail(MCLK);
	Set_LCD_Panel();
}

void LT738_Init(void)
{
	Delay_ms(100);                    //delay for LT738 power on
	LT738_HW_Reset();                 //LT738��λ
	System_Check_Temp();	            //��⸴λ�Ƿ�ɹ�
	Delay_ms(100);
	while(LCD_StatusRead()&0x02);	    //Initial_Display_test	and  set SW2 pin2 = 1
	LT738_initial();
}

#if 0

//--------------------------------------------------------------------------------------------------------------------------------------------

void MPU8_8bpp_Memory_Write
(
 unsigned short x           // x����
,unsigned short y           // y����
,unsigned short w           // ���
,unsigned short h           // �߶�
,const unsigned char *data  // �����׵�ַ
)
{														  
	unsigned short i,j;
	Graphic_Mode();
	Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);	
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{	   
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}	 

			
void MPU8_16bpp_Memory_Write
(
 unsigned short x           // x����
,unsigned short y           // y����
,unsigned short w           // ���
,unsigned short h           // �߶�
,const unsigned char *data  // �����׵�ַ
)
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}		 


void MPU8_24bpp_Memory_Write 
(
 unsigned short x           // x����
,unsigned short y           // y����
,unsigned short w           // ���
,unsigned short h           // �߶�
,const unsigned char *data  // �����׵�ַ
)

{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}



void MPU16_16bpp_Memory_Write 
(
 unsigned short x            // x����
,unsigned short y            // y����
,unsigned short w            // ���
,unsigned short h            // �߶�
,const unsigned short *data  // �����׵�ַ
)			
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite_Pixel(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}




void MPU16_24bpp_Mode1_Memory_Write 
(
 unsigned short x            // x����
,unsigned short y            // y����
,unsigned short w            // ���
,unsigned short h            // �߶�
,const unsigned short *data  // �����׵�ַ
)	
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w/2;j++)
		{
		 LCD_DataWrite_Pixel(*data);
		 Check_Mem_WR_FIFO_not_Full();
		 data++;
		 LCD_DataWrite_Pixel(*data);
		 Check_Mem_WR_FIFO_not_Full();
		 data++;
		 LCD_DataWrite_Pixel(*data);
		 Check_Mem_WR_FIFO_not_Full();
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}


void MPU16_24bpp_Mode2_Memory_Write
(
 unsigned short x            // x����
,unsigned short y            // y����
,unsigned short w            // ���
,unsigned short h            // �߶�
,const unsigned short *data  // �����׵�ַ
)	
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite_Pixel(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite_Pixel(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}





//--------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------- �߶� -----------------------------------------
void LT738_DrawLine
(
 unsigned short X1        // X1����
,unsigned short Y1        // Y1����
,unsigned short X2        // X2����
,unsigned short Y2        // Y2����
,unsigned long  LineColor // �߶���ɫ
)
{
	Foreground_color_65k(LineColor);
	Line_Start_XY(X1,Y1);
	Line_End_XY(X2,Y2);
	Start_Line();
	Check_2D_Busy();
}

void LT738_DrawLine_Width
(
 unsigned short X1        // X1����
,unsigned short Y1        // Y1����
,unsigned short X2        // X2����
,unsigned short Y2        // Y2����
,unsigned long  LineColor // �߶���ɫ
,unsigned short Width     // �߶ο��
)
{
	unsigned short  i = 0;
	signed  short x = 0, y = 0;
	double temp = 0;
	x = X2 - X1;
	y = Y2 - Y1;
	if(x == 0) temp = 2;
	else temp = -((double)y/(double)x);
	if(temp>=-1&&temp<=1)
		{
				while(Width--)
			{
				LT738_DrawLine(X1,Y1+i,X2,Y2+i,LineColor);
				i++;
			}	
		}
		
	else 
		{
				while(Width--)
			{
				LT738_DrawLine(X1+i,Y1,X2+i,Y2,LineColor);
				i++;
			}	
		}
}

#endif

//------------------------------------- Բ -----------------------------------------
void LT738_DrawCircle
(
 unsigned short XCenter           // Բ��Xλ��
,unsigned short YCenter           // Բ��Yλ��
,unsigned short R                 // �뾶
,unsigned long CircleColor        // ������ɫ
)
{
	Foreground_color_65k(CircleColor);
	Circle_Center_XY(XCenter,YCenter);
	Circle_Radius_R(R);
	Start_Circle_or_Ellipse();
	Check_2D_Busy(); 
}


void LT738_DrawCircle_Fill
(
 unsigned short XCenter           // Բ��Xλ��
,unsigned short YCenter           // Բ��Yλ��
,unsigned short R                 // �뾶
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Circle_Center_XY(XCenter,YCenter);
	Circle_Radius_R(R);
	Start_Circle_or_Ellipse_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawCircle_Width
(
 unsigned short XCenter          // Բ��Xλ��
,unsigned short YCenter          // Բ��Yλ��
,unsigned short R                // �뾶
,unsigned long CircleColor       // ������ɫ
,unsigned long ForegroundColor   // ������ɫ
,unsigned short Width            // �߿�
)
{
	LT738_DrawCircle_Fill(XCenter,YCenter,R+Width,CircleColor);
	LT738_DrawCircle_Fill(XCenter,YCenter,R,ForegroundColor);
}

#if 0
//------------------------------------- ��Բ -----------------------------------------
void LT738_DrawEllipse
(
 unsigned short XCenter          // ��Բ��Xλ��
,unsigned short YCenter          // ��Բ��Yλ��
,unsigned short X_R              // ��뾶
,unsigned short Y_R              // ���뾶
,unsigned long EllipseColor      // ������ɫ
)
{
	Foreground_color_65k(EllipseColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Circle_or_Ellipse();
	Check_2D_Busy(); 
}

void LT738_DrawEllipse_Fill
(
 unsigned short XCenter           // ��Բ��Xλ��
,unsigned short YCenter           // ��Բ��Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Circle_or_Ellipse_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawEllipse_Width
(
 unsigned short XCenter           // ��Բ��Xλ��
,unsigned short YCenter           // ��Բ��Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long EllipseColor       // ������ɫ
,unsigned long ForegroundColor    // ������ɫ
,unsigned short Width             // �߿�
)
{
	LT738_DrawEllipse_Fill(XCenter,YCenter,X_R+Width,Y_R+Width,EllipseColor);
	LT738_DrawEllipse_Fill(XCenter,YCenter,X_R,Y_R,ForegroundColor);
}



//------------------------------------- ���� -----------------------------------------
void LT738_DrawSquare
(
 unsigned short X1                // X1λ��
,unsigned short Y1                // Y1λ��
,unsigned short X2                // X2λ��
,unsigned short Y2                // Y2λ��
,unsigned long SquareColor        // ������ɫ
)
{
	Foreground_color_65k(SquareColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2);
	Start_Square();
	Check_2D_Busy(); 
}

#endif
void LT738_DrawSquare_Fill
(
 unsigned short X1                // X1λ��
,unsigned short Y1                // Y1λ��
,unsigned short X2                // X2λ��
,unsigned short Y2                // Y2λ��
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2);
	Start_Square_Fill();
	Check_2D_Busy();
}

#if 0
void LT738_DrawSquare_Width
(
 unsigned short X1                // X1λ��
,unsigned short Y1                // Y1λ��
,unsigned short X2                // X2λ��
,unsigned short Y2                // Y2λ��
,unsigned long SquareColor        // ������ɫ
,unsigned long ForegroundColor    // ������ɫ
,unsigned short Width             // �߿�
)
{
	LT738_DrawSquare_Fill(X1-Width,Y1-Width,X2+Width,Y2+Width,SquareColor);
	LT738_DrawSquare_Fill(X1,Y1,X2,Y2,ForegroundColor);
}


//------------------------------------- Բ�Ǿ��� -----------------------------------------
void LT738_DrawCircleSquare
(
 unsigned short X1                // X1λ��
,unsigned short Y1                // Y1λ��
,unsigned short X2                // X2λ��
,unsigned short Y2                // Y2λ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CircleSquareColor  // ������ɫ
)
{
	Foreground_color_65k(CircleSquareColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2); 
	Circle_Square_Radius_RxRy(X_R,Y_R);
	Start_Circle_Square();
	Check_2D_Busy();
}



void LT738_DrawCircleSquare_Fill
(
 unsigned short X1                // X1λ��
,unsigned short Y1                // Y1λ��
,unsigned short X2                // X2λ��
,unsigned short Y2                // Y2λ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long ForegroundColor  // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2); 
	Circle_Square_Radius_RxRy(X_R,Y_R);
	Start_Circle_Square_Fill();
	Check_2D_Busy(); 
}



void LT738_DrawCircleSquare_Width
(
 unsigned short X1                // X1λ��
,unsigned short Y1                // Y1λ��
,unsigned short X2                // X2λ��
,unsigned short Y2                // Y2λ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CircleSquareColor  // ������ɫ
,unsigned long ForegroundColor    // ������ɫ
,unsigned short Width             // ���
)
{
	LT738_DrawCircleSquare_Fill(X1-Width,Y1-Width,X2+Width,Y2+Width,X_R,Y_R,CircleSquareColor);
	LT738_DrawCircleSquare_Fill(X1,Y1,X2,Y2,X_R,Y_R,ForegroundColor);
}


//------------------------------------- ������ -----------------------------------------
void LT738_DrawTriangle
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned long TriangleColor    // ������ɫ
)
{
	Foreground_color_65k(TriangleColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Start_Triangle();
	Check_2D_Busy(); 
}



void LT738_DrawTriangle_Fill
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned long ForegroundColor  // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Start_Triangle_Fill();
	Check_2D_Busy();
}

void LT738_DrawTriangle_Frame
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned long TriangleColor    // ������ɫ
,unsigned long ForegroundColor  // ������ɫ
)
{
	LT738_DrawTriangle_Fill(X1,Y1,X2,Y2,X3,Y3,ForegroundColor);
	LT738_DrawTriangle(X1,Y1,X2,Y2,X3,Y3,TriangleColor);
}



//------------------------------------- ���� -----------------------------------------
void LT738_DrawLeftUpCurve
( 
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CurveColor         // ������ɫ
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Up_Curve();
	Check_2D_Busy(); 
}


void LT738_DrawLeftDownCurve
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CurveColor         // ������ɫ
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Down_Curve();
	Check_2D_Busy(); 
}


void LT738_DrawRightUpCurve
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CurveColor         // ������ɫ
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Up_Curve();
	Check_2D_Busy(); 
}


void LT738_DrawRightDownCurve
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CurveColor         // ������ɫ
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Down_Curve();
	Check_2D_Busy(); 
}


void LT738_SelectDrawCurve
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CurveColor         // ������ɫ
,unsigned short  Dir              // ����
)
{
	switch(Dir)
	{
		case 0:LT738_DrawLeftDownCurve(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		case 1:LT738_DrawLeftUpCurve(XCenter,YCenter,X_R,Y_R,CurveColor);			break;
		case 2:LT738_DrawRightUpCurve(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		case 3:LT738_DrawRightDownCurve(XCenter,YCenter,X_R,Y_R,CurveColor);	break;
		default:																															break;
	}
}


//------------------------------------- 1/4ʵ����Բ -----------------------------------------
void LT738_DrawLeftUpCurve_Fill
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Up_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawLeftDownCurve_Fill
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Down_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawRightUpCurve_Fill
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Up_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawRightDownCurve_Fill
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long ForegroundColor    // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Down_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_SelectDrawCurve_Fill
(
 unsigned short XCenter           // ����Xλ��
,unsigned short YCenter           // ����Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned long CurveColor         // ������ɫ
,unsigned short  Dir              // ����
)
{
	switch(Dir)
	{
		case 0:LT738_DrawLeftDownCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		case 1:LT738_DrawLeftUpCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);			break;
		case 2:LT738_DrawRightUpCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);			break;
		case 3:LT738_DrawRightDownCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		default:																																		break;
	}
}



//------------------------------------- �ı��� -----------------------------------------

void LT738_DrawQuadrilateral
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned short X4              // X4λ��
,unsigned short Y4              // Y4λ��
,unsigned long ForegroundColor  // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0x8d);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}


void LT738_DrawQuadrilateral_Fill
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned short X4              // X4λ��
,unsigned short Y4              // Y4λ��
,unsigned long ForegroundColor  // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0xa7);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}



//------------------------------------- ����� -----------------------------------------

void LT738_DrawPentagon
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned short X4              // X4λ��
,unsigned short Y4              // Y4λ��
,unsigned short X5              // X5λ��
,unsigned short Y5              // Y5λ��
,unsigned long ForegroundColor  // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);
	Ellipse_Center_XY(X5,Y5);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0x8F);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}


void LT738_DrawPentagon_Fill
(
 unsigned short X1              // X1λ��
,unsigned short Y1              // Y1λ��
,unsigned short X2              // X2λ��
,unsigned short Y2              // Y2λ��
,unsigned short X3              // X3λ��
,unsigned short Y3              // Y3λ��
,unsigned short X4              // X4λ��
,unsigned short Y4              // Y4λ��
,unsigned short X5              // X5λ��
,unsigned short Y5              // Y5λ��
,unsigned long ForegroundColor  // ������ɫ
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);
	Ellipse_Center_XY(X5,Y5);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0xa9);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}


//------------------------------------- Բ�� -----------------------------------------
unsigned char LT738_DrawCylinder
(
 unsigned short XCenter           // ��Բ��Xλ��
,unsigned short YCenter           // ��Բ��Yλ��
,unsigned short X_R               // ��뾶
,unsigned short Y_R               // ���뾶
,unsigned short H                 // �߶�
,unsigned long CylinderColor      // ������ɫ
,unsigned long ForegroundColor    // ������ɫ
)
{
	if(YCenter < H)	return 1;
	
	//������Բ
	LT738_DrawEllipse_Fill(XCenter,YCenter,X_R,Y_R,ForegroundColor);
	LT738_DrawEllipse(XCenter,YCenter,X_R,Y_R,CylinderColor);
	
	//�м����
	LT738_DrawSquare_Fill(XCenter-X_R,YCenter-H,XCenter+X_R,YCenter,ForegroundColor);
	
	//������Բ
	LT738_DrawEllipse_Fill(XCenter,YCenter-H,X_R,Y_R,ForegroundColor);
	LT738_DrawEllipse(XCenter,YCenter-H,X_R,Y_R,CylinderColor);
	
	LT738_DrawLine(XCenter-X_R,YCenter,XCenter-X_R,YCenter-H,CylinderColor);
	LT738_DrawLine(XCenter+X_R,YCenter,XCenter+X_R,YCenter-H,CylinderColor);
	
	return 0;
}


//------------------------------------- ������ -----------------------------------------
void LT738_DrawQuadrangular
(
 unsigned short X1
,unsigned short Y1
,unsigned short X2
,unsigned short Y2
,unsigned short X3
,unsigned short Y3
,unsigned short X4
,unsigned short Y4
,unsigned short X5
,unsigned short Y5
,unsigned short X6
,unsigned short Y6
,unsigned long QuadrangularColor   // ������ɫ
,unsigned long ForegroundColor     // ������ɫ
)
{
	LT738_DrawSquare_Fill(X1,Y1,X5,Y5,ForegroundColor);
	LT738_DrawSquare(X1,Y1,X5,Y5,QuadrangularColor);
	
	LT738_DrawQuadrilateral_Fill(X1,Y1,X2,Y2,X3,Y3,X4,Y4,ForegroundColor);
	LT738_DrawQuadrilateral(X1,Y1,X2,Y2,X3,Y3,X4,Y4,QuadrangularColor);
	
	LT738_DrawQuadrilateral_Fill(X3,Y3,X4,Y4,X5,Y5,X6,Y6,ForegroundColor);
	LT738_DrawQuadrilateral(X3,Y3,X4,Y4,X5,Y5,X6,Y6,QuadrangularColor);
}


//----------------------------------------------------------------------���-------------------------------------------------------------------
void LT738_MakeTable
(
	unsigned short X1,                  // ��ʼλ��X1
	unsigned short Y1,                  // ��ʼλ��X2
	unsigned short W,                   // ���
	unsigned short H,                   // �߶�
	unsigned short Line,                // ����
	unsigned short Row,                 // ����
	unsigned long  TableColor,          // �߿���ɫC1
	unsigned long  ItemColor,  					// ��Ŀ������ɫC2
	unsigned long  ForegroundColor,     // �ڲ����ڱ���ɫC3
	unsigned short width1,              // �ڿ���
	unsigned short width2,              // �����
	unsigned char  mode                 // 0����Ŀ������   1����Ŀ������ 
)
{
	unsigned short i = 0;
	unsigned short x2,y2;
	x2 = X1 + W * Row;
	y2 = Y1 + H * Line;
	
	LT738_DrawSquare_Width(X1,Y1,x2,y2,TableColor,ForegroundColor,width2);  
	
	if(mode == 0)	      LT738_DrawSquare_Fill(X1,Y1,X1+W,y2,ItemColor);  
	else if(mode == 1)	LT738_DrawSquare_Fill(X1,Y1,x2,Y1+H,ItemColor); 
	
	for(i = 0 ; i < Line ; i++)
	{
		LT738_DrawLine_Width(X1,Y1+i*H,x2,Y1+i*H,TableColor,width1);
	}
	
	for(i = 0 ; i < Row ; i++)
	{
		LT738_DrawLine_Width(X1+i*W,Y1,X1+i*W,y2,TableColor,width1);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void LT738_Color_Bar_ON(void)
{
	Color_Bar_ON();
}

void LT738_Color_Bar_OFF(void)
{
	Color_Bar_OFF();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
/* ѡ���ڲ������ֿ��ʼ�� */
void LT738_Select_Internal_Font_Init
(
 unsigned char Size         // ���������С  16��16*16     24:24*24    32:32*32
,unsigned char XxN          // ����Ŀ�ȷŴ�����1~4
,unsigned char YxN          // ����ĸ߶ȷŴ�����1~4
,unsigned char ChromaKey    // 0�����屳��ɫ͸��    1��������������ı���ɫ
,unsigned char Alignment    // 0�������岻����      1���������
)
{
	if(Size==16)	Font_Select_8x16_16x16();
	if(Size==24)	Font_Select_12x24_24x24();
	if(Size==32)	Font_Select_16x32_32x32();

	//(*)
	if(XxN==1)	Font_Width_X1();
	if(XxN==2)	Font_Width_X2();
	if(XxN==3)	Font_Width_X3();
	if(XxN==4)	Font_Width_X4();

	//(*)
	if(YxN==1)	Font_Height_X1();
	if(YxN==2)	Font_Height_X2();
	if(YxN==3)	Font_Height_X3();
	if(YxN==4)	Font_Height_X4();

	//(*)
	if(ChromaKey==0)	Font_Background_select_Color();	
	if(ChromaKey==1)	Font_Background_select_Transparency();	

	//(*)
	if(Alignment==0)	Disable_Font_Alignment();
	if(Alignment==1)	Enable_Font_Alignment();
}


/* ��ʾ�ڲ��������� */
void LT738_Print_Internal_Font_String
(
 unsigned short x               // ���忪ʼ��ʾ��xλ��
,unsigned short y               // ���忪ʼ��ʾ��yλ��
,unsigned long FontColor        // �������ɫ
,unsigned long BackGroundColor  // ����ı���ɫ��ע�⣺�����屳����ʼ����͸��ʱ�����ø�ֵ��Ч��
,char *c                        // ���ݻ�����׵�ַ
)
{
	Text_Mode();
	CGROM_Select_Internal_CGROM();
	Foreground_color_65k(FontColor);
	Background_color_65k(BackGroundColor);
	Goto_Text_XY(x,y);
	Show_String(c);
}

u8 FONTBUFF[1024] = {0};

/* ѡ���ⲿ�����ֿ��ʼ�� */
void LT738_Select_Outside_Font_Init
(
 unsigned char SCS           // ѡ����ҵ�SPI   : SCS��0       SCS��1
,unsigned char Clk           // SPIʱ�ӷ�Ƶ���� : SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long FlashAddr     // Դ��ַ(Flash)
,unsigned long MemoryAddr    // ��ʼ��ͼ���ַ(SDRAM)
,unsigned long DisplayAddr	 // ��ʾͼ���ַ
,unsigned long Num           // �ֿ����������С
,unsigned char Size          // ���������С  16��16*16     24:24*24    32:32*32
,unsigned char XxN           // ����Ŀ�ȷŴ�����1~4
,unsigned char YxN           // ����ĸ߶ȷŴ�����1~4
,unsigned char ChromaKey     // 1�����屳��ɫ͸��    0��������������ı���ɫ
,unsigned char Alignment     // 0�������岻����      1���������
)
{
	u16 i,/*j,*/k,m,temp;
	if(Size==16)	Font_Select_8x16_16x16();
	if(Size==24)	Font_Select_12x24_24x24();
	if(Size==32)	Font_Select_16x32_32x32();

	//(*)
	if(XxN==1)	Font_Width_X1();
	if(XxN==2)	Font_Width_X2();
	if(XxN==3)	Font_Width_X3();
	if(XxN==4)	Font_Width_X4();

	//(*)
	if(YxN==1)	Font_Height_X1();
	if(YxN==2)	Font_Height_X2();
	if(YxN==3)	Font_Height_X3();
	if(YxN==4)	Font_Height_X4();

	//(*)
	if(ChromaKey==0)	Font_Background_select_Color();	
	if(ChromaKey==1)	Font_Background_select_Transparency();	

	//(*)
	if(Alignment==0)	Disable_Font_Alignment();
	if(Alignment==1)	Enable_Font_Alignment();	
	
	i = Num/1024;
	Canvas_Image_Start_address(MemoryAddr);
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(k=0;k<i;k++)
	{
		W25X_Read_Data(FONTBUFF,FlashAddr+k*1024,1024);
		for(m = 0 ; m < 1024 ; m+=2)
		{
			if((m%32)==0)	Check_Mem_WR_FIFO_not_Full();
			temp = ((FONTBUFF[m+1]<<8)+FONTBUFF[m]);
			LCD_DataWrite_Pixel(temp);
		}
	}
	Check_Mem_WR_FIFO_Empty();
	CGRAM_Start_address(MemoryAddr);
	Canvas_Image_Start_address(DisplayAddr);
}

/* ��ʾ�ⲿ���ڲ��������� */
void LT738_Print_Outside_Font_String
(
 unsigned short x               // ���忪ʼ��ʾ��xλ��
,unsigned short y               // ���忪ʼ��ʾ��yλ��
,unsigned long FontColor        // �������ɫ
,unsigned long BackGroundColor  // ����ı���ɫ��ע�⣺�����屳����ʼ����͸��ʱ�����ø�ֵ��Ч��
,unsigned char *c               // ���ݻ�����׵�ַ
)
{
	unsigned short temp_H = 0;
	unsigned short temp_L = 0;
	unsigned short temp = 0;
	unsigned long i = 0;
	
	Text_Mode();
	Font_Select_UserDefine_Mode();
	Foreground_color_65k(FontColor);
	Background_color_65k(BackGroundColor);
	Goto_Text_XY(x,y);
	
	while(c[i] != '\0')
	{ 
		if(c[i] < 0xa1)
		{
			CGROM_Select_Internal_CGROM();   // �ڲ�CGROMΪ�ַ���Դ
			LCD_CmdWrite(0x04);
			LCD_DataWrite(c[i]);
			Check_Mem_WR_FIFO_not_Full();  
			i += 1;
		}
		else
		{
			Font_Select_UserDefine_Mode();   // �Զ����ֿ�
			LCD_CmdWrite(0x04);
			temp_H = ((c[i] - 0xa1) & 0x00ff) * 94;
			temp_L = c[i+1] - 0xa1;
			temp = temp_H + temp_L + 0x8000;
			LCD_DataWrite((temp>>8)&0xff);
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(temp&0xff);
			Check_Mem_WR_FIFO_not_Full();
			i += 2;		
		}
	}
	
  Check_2D_Busy();

  Graphic_Mode(); //back to graphic mode;ͼ��ģʽ
}

/*��ʾ48*48��72*72����*/
void LT738_BTE_Memory_Copy_ColorExpansion_8
(
 unsigned long S0_Addr             // SOͼ����ڴ���ʼ��ַ
,unsigned short XS0                // S0ͼ������Ϸ�Y����
,unsigned long Des_Addr            // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W              // Ŀ��ͼ��Ŀ��
,unsigned short XDes               // Ŀ��ͼ������Ϸ�X����(��ʾ���ڵ���ʼx����)
,unsigned short YDes               // Ŀ��ͼ������Ϸ�Y����(��ʾ���ڵ���ʼy����)
,unsigned short X_W                // ��ʾ���ڵĿ��
,unsigned short Y_H                // ��ʾ���ڵĳ���
,unsigned long Foreground_color
,unsigned long Background_color
)
{
	Foreground_color_256(Foreground_color);
	Background_color_256(Background_color);
	BTE_ROP_Code(7);
	
	BTE_S0_Color_8bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(Des_W);
	BTE_S0_Window_Start_XY(XS0,0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);
   
	BTE_Operation_Code(0x0e);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}

void LT738_BTE_Memory_Copy_ColorExpansion_Chroma_key_8
(
 unsigned long S0_Addr             // SOͼ����ڴ���ʼ��ַ
,unsigned short XS0                // S0ͼ������Ϸ�Y����
,unsigned long Des_Addr            // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W              // Ŀ��ͼ��Ŀ��
,unsigned short XDes               // Ŀ��ͼ������Ϸ�X����(��ʾ���ڵ���ʼx����)
,unsigned short YDes               // Ŀ��ͼ������Ϸ�Y����(��ʾ���ڵ���ʼy����)
,unsigned short X_W                // ��ʾ���ڵĿ��
,unsigned short Y_H                // ��ʾ���ڵĳ���
,unsigned long Foreground_color
)
{
	Foreground_color_256(Foreground_color);
	BTE_ROP_Code(7);
	
	BTE_S0_Color_8bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(Des_W);
	BTE_S0_Window_Start_XY(XS0,0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);
   
	BTE_Operation_Code(0x0f);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}

u8 FONTBUFF1[10] = {0};

void LT738_Print_OutsideFont_Giant
(
 unsigned char SCS           		// ѡ����ҵ�SPI   : SCS��0       SCS��1
,unsigned char Clk           		// SPIʱ�ӷ�Ƶ���� : SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long FlashAddr     		// �ֿ�Դ��ַ(Flash)
,unsigned long MemoryAddr    		// Ŀ�ĵ�ַ(SDRAM)
,unsigned long ShowAddr             // ��ʾ��ĵ�ַ
,unsigned short width               // ��ʾ��Ŀ��
,unsigned char Size          		// ���������С  48��48*48     72:72*72
,unsigned char ChromaKey     		// 0�����屳��ɫ͸��    1��������������ı���ɫ
,unsigned short x                   // ���忪ʼ��ʾ��xλ��
,unsigned short y                   // ���忪ʼ��ʾ��yλ��
,unsigned long FontColor            // �������ɫ
,unsigned long BackGroundColor      // ����ı���ɫ��ע�⣺�����屳����ʼ����͸��ʱ�����ø�ֵ��Ч��
,unsigned short w				// �����ϸ��0�����Ӵ�  1���Ӵ�1��  2���Ӵ�2��
,unsigned short s                   // �о�
,unsigned char *c                   // ���ݻ�����׵�ַ
)
{
	unsigned short temp_H = 0;
	unsigned short temp_L = 0;
	unsigned short temp = 0;
	unsigned long i = 0;
	unsigned short j = 0;
	unsigned short k = 0;
	unsigned short h = 0;
	unsigned short n = 0;
	unsigned short m = 0;
	unsigned short g = 0;
	unsigned short f = 0;
	unsigned short a = 0;
	unsigned short b = 0;
	
	h = x; k = y;
	Memory_8bpp_Mode();//ʹ��8λɫ�����洢ͼƬ
 	Canvas_Image_Start_address(MemoryAddr);
 	Canvas_image_width(width);
	while(c[i] != '\0')
	{
		temp_H = (c[i] - 0xa1) * 94;
		temp_L = c[i+1] - 0xa1;
		temp = temp_H + temp_L;
		for(a=0;a<Size;a++)//һ��дa��
		{
			W25X_Read_Data(FONTBUFF1,FlashAddr+temp*((Size*Size)/8)+a*(Size/8),Size/8);//��һ�е�����
			Goto_Pixel_XY(Size*j/8,a);//ָ��һ�е���ʼλ��
			LCD_CmdWrite(0x04);
			for(b = 0 ; b < Size/8 ; b++)
			{
				LCD_DataWrite(FONTBUFF1[b]);
			}
		}
		Check_Mem_WR_FIFO_Empty();
		i+=2;
		j++;
	}
	
	Memory_16bpp_Mode();
	Canvas_Image_Start_address(ShowAddr);
	Canvas_image_width(width);
	j = 0; i = 0;
	
	if(w>2)	w = 2;
	for(g=0;g<w+1;g++)
	{
		while(c[i] != '\0')
		{
			if((f == m)&&((x+Size*j+Size)>(width*(n+1)))) {m++;n++;y=y+Size-1+s;x = x+((width*n)-(x+Size*j))+g;f=n;}
			if(ChromaKey==1)
			{
				LT738_BTE_Memory_Copy_ColorExpansion_8(MemoryAddr,Size*j/8,
										   ShowAddr,width,x+Size*j,y,
										   Size,Size,FontColor,BackGroundColor
										   );
			}
			if(ChromaKey==0)
			{
				LT738_BTE_Memory_Copy_ColorExpansion_Chroma_key_8(MemoryAddr,Size*j/8,   
												  ShowAddr,width,x+Size*j,y,
													Size,Size,FontColor
													);
			}
			i+=2;
			j++;
		}
		ChromaKey=0;i=0;j=0;m=0;n=0;f=0;x=h+g+1;y=k+g+1;
	}
}

void LT738_Text_cursor_Init
(
 unsigned char On_Off_Blinking         // 0����ֹ�����˸   1��ʹ�ܹ����˸
,unsigned short Blinking_Time          // �������ֹ����˸ʱ��
,unsigned short X_W                    // ���ֹ��ˮƽ��С
,unsigned short Y_W                    // ���ֹ�괹ֱ��С
)
{
	if(On_Off_Blinking == 0)	Disable_Text_Cursor_Blinking();
	if(On_Off_Blinking == 1)	Enable_Text_Cursor_Blinking();

	Blinking_Time_Frames(Blinking_Time); 

	Text_Cursor_H_V(X_W,Y_W);

	Enable_Text_Cursor();
}


void LT738_Enable_Text_Cursor(void)
{
	Enable_Text_Cursor();
}


void LT738_Disable_Text_Cursor(void)
{
	Disable_Text_Cursor();
}


void LT738_Graphic_cursor_Init
(
 unsigned char Cursor_N                  // ѡ����   1:���1   2:���2   3:���3  4:���4
,unsigned char Color1                    // ��ɫ1
,unsigned char Color2                    // ��ɫ2
,unsigned short X_Pos                    // ��ʾ����X
,unsigned short Y_Pos                    // ��ʾ����Y
,unsigned char *Cursor_Buf               // ������ݵĻ����׵�ַ
)
{
	unsigned int i ;
	
	Memory_Select_Graphic_Cursor_RAM(); 
	Graphic_Mode();
	
	switch(Cursor_N)
	{
		case 1:	Select_Graphic_Cursor_1();	break;
		case 2:	Select_Graphic_Cursor_2();	break;
		case 3:	Select_Graphic_Cursor_3();	break;
		case 4:	Select_Graphic_Cursor_4();	break;
		default:break;
	}
	
	LCD_CmdWrite(0x04);
	for(i=0;i<256;i++)
	{					 
		LCD_DataWrite(Cursor_Buf[i]);
	}
	
	Memory_Select_SDRAM();//д����л�SDRAM
	Set_Graphic_Cursor_Color_1(Color1);
	Set_Graphic_Cursor_Color_2(Color2);
	Graphic_Cursor_XY(X_Pos,Y_Pos);

	Enable_Graphic_Cursor();
}


void LT738_Set_Graphic_cursor_Pos
(
 unsigned char Cursor_N                  // ѡ����   1:���1   2:���2   3:���3  4:���4
,unsigned short X_Pos                    // ��ʾ����X
,unsigned short Y_Pos                    // ��ʾ����Y
)
{
	Graphic_Cursor_XY(X_Pos,Y_Pos);
	switch(Cursor_N)
	{
		case 1:	Select_Graphic_Cursor_1();	break;
		case 2:	Select_Graphic_Cursor_2();	break;
		case 3:	Select_Graphic_Cursor_3();	break;
		case 4:	Select_Graphic_Cursor_4();	break;
		default:
		break;
	}
}


void LT738_Enable_Graphic_Cursor(void)
{
	Enable_Graphic_Cursor();
}


void LT738_Disable_Graphic_Cursor(void)
{
	Disable_Graphic_Cursor();
}


//-----------------------------------------------------------------------------------------------------------------------------

void LT738_PIP_Init
(
 unsigned char On_Off         // 0 : ��ֹ PIP    1 : ʹ�� PIP    2 : ����ԭ����״̬
,unsigned char Select_PIP     // 1 : ʹ�� PIP1   2 : ʹ�� PIP2
,unsigned long PAddr          // PIP�Ŀ�ʼ��ַ
,unsigned short XP            // PIP���ڵ�X����,���뱻4����
,unsigned short YP            // PIP���ڵ�Y����,���뱻4����
,unsigned long ImageWidth     // ��ͼ�Ŀ��
,unsigned short X_Dis         // ��ʾ���ڵ�X����
,unsigned short Y_Dis         // ��ʾ���ڵ�Y����
,unsigned short X_W           // ��ʾ���ڵĿ�ȣ����뱻4����
,unsigned short Y_H           // ��ʾ���ڵĳ��ȣ����뱻4����
)
{
	if(Select_PIP == 1 )  
	{
		Select_PIP1_Window_16bpp();
		Select_PIP1_Parameter();
	}
	if(Select_PIP == 2 )  
	{
		Select_PIP2_Window_16bpp();
		Select_PIP2_Parameter();
	}
	
	PIP_Display_Start_XY(X_Dis,Y_Dis);
	PIP_Image_Start_Address(PAddr);
	PIP_Image_Width(ImageWidth);
	PIP_Window_Image_Start_XY(XP,YP);
	PIP_Window_Width_Height(X_W,Y_H);
	

	if(On_Off == 0)
	{
		if(Select_PIP == 1 )  Disable_PIP1();	
		if(Select_PIP == 2 )  Disable_PIP2();
	}

	if(On_Off == 1)
	{
		if(Select_PIP == 1 )  Enable_PIP1();	
		if(Select_PIP == 2 )  Enable_PIP2();
	}
}


void LT738_Set_DisWindowPos
(
 unsigned char On_Off         // 0 : ��ֹ PIP, 1 : ʹ�� PIP, 2 : ����ԭ����״̬
,unsigned char Select_PIP     // 1 : ʹ�� PIP1 , 2 : ʹ�� PIP2
,unsigned short X_Dis         // ��ʾ���ڵ�X����
,unsigned short Y_Dis         // ��ʾ���ڵ�Y����
)
{
	if(Select_PIP == 1 )  Select_PIP1_Parameter();
	if(Select_PIP == 2 )  Select_PIP2_Parameter();
	
	if(On_Off == 0)
	{
		if(Select_PIP == 1 )  Disable_PIP1();	
		if(Select_PIP == 2 )  Disable_PIP2();
	}

	if(On_Off == 1)
	{
		if(Select_PIP == 1 )  Enable_PIP1();	
		if(Select_PIP == 2 )  Enable_PIP2();
	}
	
	PIP_Display_Start_XY(X_Dis,Y_Dis);
	
}




//-----------------------------------------------------------------------------------------------------------------------------



void BTE_Solid_Fill
(
 unsigned long Des_Addr           // ����Ŀ�ĵ�ַ 
,unsigned short Des_W             // Ŀ�ĵ�ַͼƬ���
,unsigned short XDes              // x���� 
,unsigned short YDes              // y���� 
,unsigned short color             // ������ɫ 
,unsigned short X_W               // ���ĳ��� 
,unsigned short Y_H               // ���Ŀ�� 
)            
{
	BTE_Destination_Color_16bpp();

	BTE_Destination_Memory_Start_Address(Des_Addr);

	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);
	BTE_Window_Size(X_W,Y_H);

	Foreground_color_65k(color);
	BTE_Operation_Code(0x0c);
	BTE_Enable();
	Check_BTE_Busy();     
}

/*  ��Ϲ�դ������BTE�ڴ渴�� */
void LT738_BTE_Memory_Copy
(
 unsigned long S0_Addr     // SOͼ����ڴ���ʼ��ַ
,unsigned short S0_W       // S0ͼ��Ŀ��
,unsigned short XS0        // S0ͼ������Ϸ�X����
,unsigned short YS0        // S0ͼ������Ϸ�Y����
,unsigned long S1_Addr     // S1ͼ����ڴ���ʼ��ַ
,unsigned short S1_W       // S1ͼ��Ŀ��
,unsigned short XS1        // S1ͼ������Ϸ�X����
,unsigned short YS1        // S1ͼ������Ϸ�Y����
,unsigned long Des_Addr    // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W      // Ŀ��ͼ��Ŀ��
,unsigned short XDes       // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes       // Ŀ��ͼ������Ϸ�Y����
,unsigned int ROP_Code     // ��դ����ģʽ
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned short X_W       // ����ڵĿ��
,unsigned short Y_H       // ����ڵĳ���
)
{
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_S1_Color_16bpp();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);	

	BTE_ROP_Code(ROP_Code);	
	BTE_Operation_Code(0x02); //BTE Operation: Memory copy (move) with ROP.
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}


/*  ��� Chroma Key ���ڴ渴�ƣ����� ROP�� */
void LT738_BTE_Memory_Copy_Chroma_key
(
 unsigned long S0_Addr             // SOͼ����ڴ���ʼ��ַ
,unsigned short S0_W               // S0ͼ��Ŀ��
,unsigned short XS0                // S0ͼ������Ϸ�X����
,unsigned short YS0                // S0ͼ������Ϸ�Y����
,unsigned long Des_Addr            // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W              // Ŀ��ͼ��Ŀ��
,unsigned short XDes               // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes               // Ŀ��ͼ������Ϸ�X����
,unsigned long Background_color    // ͸��ɫ
,unsigned short X_W                // ����ڵĿ��
,unsigned short Y_H                // ����ڵĳ���
)
{
	Background_color_65k(Background_color); 

	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);	

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Operation_Code(0x05);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}


void LT738_BTE_Pattern_Fill
(
 unsigned char P_8x8_or_16x16      // 0 : use 8x8 Icon , 1 : use 16x16 Icon.
,unsigned long S0_Addr             // SOͼ����ڴ���ʼ��ַ
,unsigned short S0_W               // S0ͼ��Ŀ��
,unsigned short XS0                // S0ͼ������Ϸ�X����
,unsigned short YS0                // S0ͼ������Ϸ�Y����
,unsigned long Des_Addr            // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W              // Ŀ��ͼ��Ŀ��
, unsigned short XDes              // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes               // Ŀ��ͼ������Ϸ�X����
,unsigned int ROP_Code             // ��դ����ģʽ
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned short X_W                // ����ڵĿ��
,unsigned short Y_H                // ����ڵĳ���
)
{
	if(P_8x8_or_16x16 == 0)
	{
		Pattern_Format_8X8();
	}
	if(P_8x8_or_16x16 == 1)
	{														    
		Pattern_Format_16X16();
	}	
	
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);	

	BTE_ROP_Code(ROP_Code);	
	BTE_Operation_Code(0x06); //BTE Operation: Pattern Fill with ROP.
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}



void LT738_BTE_Pattern_Fill_With_Chroma_key
(
 unsigned char P_8x8_or_16x16      // 0 : use 8x8 Icon , 1 : use 16x16 Icon.
,unsigned long S0_Addr             // SOͼ����ڴ���ʼ��ַ
,unsigned short S0_W               // S0ͼ��Ŀ��
,unsigned short XS0                // S0ͼ������Ϸ�X����
,unsigned short YS0                // S0ͼ������Ϸ�Y����
,unsigned long Des_Addr            // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W              // Ŀ��ͼ��Ŀ��
,unsigned short XDes               // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes               // Ŀ��ͼ������Ϸ�Y����
,unsigned int ROP_Code             // ��դ����ģʽ
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned long Background_color   // ͸��ɫ
,unsigned short X_W               // ����ڵĿ��
,unsigned short Y_H               // ����ڵĳ���
)
{
	Background_color_65k(Background_color);
	
	if(P_8x8_or_16x16 == 0)
	{
		Pattern_Format_8X8();
	}
	if(P_8x8_or_16x16 == 1)
	{														    
		Pattern_Format_16X16();
	}	  

	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);	

	BTE_ROP_Code(ROP_Code);	
	BTE_Operation_Code(0x07); //BTE Operation: Pattern Fill with Chroma key.
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}



void LT738_BTE_MCU_Write_MCU_16bit
(
 unsigned long S1_Addr              // S1ͼ����ڴ���ʼ��ַ
,unsigned short S1_W                // S1ͼ��Ŀ��
,unsigned short XS1                 // S1ͼ������Ϸ�X����
,unsigned short YS1                 // S1ͼ������Ϸ�Y����
,unsigned long Des_Addr             // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W               // Ŀ��ͼ��Ŀ��
,unsigned short XDes                // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes                // Ŀ��ͼ������Ϸ�Y����
,unsigned int ROP_Code              // ��դ����ģʽ 
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned short X_W                 // ����ڵĿ��
,unsigned short Y_H                 // ����ڵĳ���
,const unsigned short *data         // S0�������׵�ַ
)
{
	unsigned short i,j;

	BTE_S1_Color_16bpp();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_ROP_Code(ROP_Code);
	BTE_Operation_Code(0x00);		//BTE Operation: MPU Write with ROP.
	BTE_Enable();
	
	BTE_S0_Color_16bpp();
	LCD_CmdWrite(0x04);				 		//Memory Data Read/Write Port
	
	for(i=0;i< Y_H;i++)
	{
		for(j=0;j< (X_W);j++)
		{
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel((*data));
			data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}



void LT738_BTE_MCU_Write_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr                 // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W                   // Ŀ��ͼ��Ŀ��
,unsigned short XDes                    // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes                    // Ŀ��ͼ������Ϸ�Y����
,unsigned long Background_color         // ͸��ɫ
,unsigned short X_W                     // ����ڵĿ��
,unsigned short Y_H                     // ����ڵĳ���
,const unsigned short *data             // S0�������յ�ַ
)
{
	unsigned int i,j;
	
	Background_color_65k(Background_color);
	
	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x04);		//BTE Operation: MPU Write with chroma keying (w/o ROP)
	BTE_Enable();

	BTE_S0_Color_16bpp();
	LCD_CmdWrite(0x04);			//Memory Data Read/Write Port
	
	for(i=0;i< Y_H;i++)
	{	
		for(j=0;j< (X_W);j++)
	  {
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel((*data));
			data++;
	  }
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}


/* �����չɫ�ʵ� MPU ���� */
void LT738_BTE_MCU_Write_ColorExpansion_MCU_16bit
(
 unsigned long Des_Addr               // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W                 // Ŀ��ͼ��Ŀ��
,unsigned short XDes                  // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes                  // Ŀ��ͼ������Ϸ�Y����
,unsigned short X_W                   // ����ڵĿ��
,unsigned short Y_H                   // ����ڵĳ���
,unsigned long Foreground_color       // ǰ��ɫ
/*Foreground_color : The source (1bit map picture) map data 1 translate to Foreground color by color expansion*/
,unsigned long Background_color       // ����ɫ
/*Background_color : The source (1bit map picture) map data 0 translate to Background color by color expansion*/
,const unsigned short *data           // ���ݻ����׵�ַ
)
{
	unsigned short i,j;
	
	RGB_16b_16bpp();
	Foreground_color_65k(Foreground_color);
	Background_color_65k(Background_color);
	BTE_ROP_Code(15);
	
	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x8);		//BTE Operation: MPU Write with Color Expansion (w/o ROP)
	BTE_Enable();
	
	LCD_CmdWrite(0x04);				 		//Memory Data Read/Write Port  
	for(i=0;i< Y_H;i++)
	{	
		for(j=0;j< X_W/16;j++)
		{
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel(*data);  
			data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}

/* �����չɫ���� Chroma key �� MPU ���� */
void LT738_BTE_MCU_Write_ColorExpansion_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr            // Ŀ��ͼ����ڴ���ʼ��ַ
,unsigned short Des_W              // Ŀ��ͼ��Ŀ��
,unsigned short XDes               // Ŀ��ͼ������Ϸ�X����
,unsigned short YDes               // Ŀ��ͼ������Ϸ�Y����
,unsigned short X_W                // ����ڵĿ��
,unsigned short Y_H                // ����ڵĳ���
,unsigned long Foreground_color    // ǰ��ɫ
/*Foreground_color : The source (1bit map picture) map data 1 translate to Foreground color by color expansion*/
,const unsigned short *data        // ���ݻ����׵�ַ
)
{
	unsigned short i,j;

	RGB_16b_16bpp();
	Foreground_color_65k(Foreground_color);
	BTE_ROP_Code(15);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);


	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x9);		//BTE Operation: MPU Write with Color Expansion and chroma keying (w/o ROP)
	BTE_Enable();

	LCD_CmdWrite(0x04);				 		//Memory Data Read/Write Port  
	for(i=0;i< Y_H;i++)
	{	
		for(j=0;j< X_W/16;j++)
		{
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel(*data);  
			data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}

/* ���͸���ȵ��ڴ渴�� */
void BTE_Alpha_Blending
(
 unsigned long S0_Addr         // SOͼ����ڴ���ʼ��ַ
 ,unsigned short S0_W          // S0ͼ��Ŀ��
 ,unsigned short XS0           // S0ͼ������Ϸ�X����
 ,unsigned short YS0           // S0ͼ������Ϸ�Y����
 ,unsigned long S1_Addr        // S1ͼ����ڴ���ʼ��ַ
 ,unsigned short S1_W          // S1ͼ��Ŀ��
 ,unsigned short XS1           // S1ͼ������Ϸ�X����
 ,unsigned short YS1           // S1ͼ������Ϸ�Y����
 ,unsigned long Des_Addr       // Ŀ��ͼ����ڴ���ʼ��ַ
 ,unsigned short Des_W         // Ŀ��ͼ��Ŀ��
 ,unsigned short XDes          // Ŀ��ͼ������Ϸ�X����
 ,unsigned short YDes          // Ŀ��ͼ������Ϸ�X����
 ,unsigned short X_W           // ����ڵĿ��
 ,unsigned short Y_H           // ����ڵĳ���
 ,unsigned char alpha          // ͸���ȵȼ���32�ȼ���
)
{	
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_S1_Color_16bpp();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x0A);		//BTE Operation: Memory write with opacity (w/o ROP)
	BTE_Alpha_Blending_Effect(alpha);
	BTE_Enable();
	Check_BTE_Busy();
}

void BTE_PNG
(
 unsigned long S0_Addr         // µ×Í¼µÄÄÚ´æÆðÊ¼µØÖ·
 ,unsigned short S0_W          // µ×Í¼µÄ¿í¶È
 ,unsigned short XS0           // µ×Í¼µÄ×óÉÏ·½X×ø±ê
 ,unsigned short YS0           // µ×Í¼µÄ×óÉÏ·½Y×ø±ê
 ,unsigned long S1_Addr        // pngÍ¼ÏñµÄÄÚ´æÆðÊ¼µØÖ·
 ,unsigned short S1_W          // pngÍ¼ÏñµÄ¿í¶È
 ,unsigned short XS1           // pngÍ¼ÏñµÄ×óÉÏ·½X×ø±ê
 ,unsigned short YS1           // pngÍ¼ÏñµÄ×óÉÏ·½Y×ø±ê
 ,unsigned long Des_Addr       // ÏÔÊ¾µÄÄÚ´æÆðÊ¼µØÖ·
 ,unsigned short Des_W         // ÏÔÊ¾µÄ¿í¶È
 ,unsigned short XDes          // ÏÔÊ¾µÄ×óÉÏ·½X×ø±ê
 ,unsigned short YDes          // ÏÔÊ¾µÄ×óÉÏ·½X×ø±ê
 ,unsigned short X_W           // ÏÔÊ¾µÄ¿í¶È
 ,unsigned short Y_H           // ÏÔÊ¾µÄ³¤¶È
)
{	
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_S1_Color_16bit_Alpha();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x0A);		//BTE Operation: Memory write with opacity (w/o ROP)
	BTE_Enable();
	Check_BTE_Busy();
}

//----------------------------------------------------------------------------------------------------------------------------------

void LT738_PWM0_Init
(
 unsigned char on_off                       // 0����ֹPWM0    1��ʹ��PWM0
,unsigned char Clock_Divided                // PWMʱ�ӷ�Ƶ  ȡֵ��Χ 0~3(1,1/2,1/4,1/8)
,unsigned char Prescalar                    // ʱ�ӷ�Ƶ     ȡֵ��Χ 1~256
,unsigned short Count_Buffer                // ����PWM���������
,unsigned short Compare_Buffer              // ����ռ�ձ�
)
{
	Select_PWM0();
	Set_PWM_Prescaler_1_to_256(Prescalar);

	if(Clock_Divided ==0)	Select_PWM0_Clock_Divided_By_1();
	if(Clock_Divided ==1)	Select_PWM0_Clock_Divided_By_2();
	if(Clock_Divided ==2)	Select_PWM0_Clock_Divided_By_4();
	if(Clock_Divided ==3) Select_PWM0_Clock_Divided_By_8();

	Set_Timer0_Count_Buffer(Count_Buffer);  
	Set_Timer0_Compare_Buffer(Compare_Buffer);	

	if (on_off == 1)	Start_PWM0(); 
	if (on_off == 0)	Stop_PWM0();
}


void LT738_PWM0_Duty(unsigned short Compare_Buffer)
{
	Set_Timer0_Compare_Buffer(Compare_Buffer);
}

#endif

void LT738_PWM1_Init
(
 unsigned char on_off                       // 0����ֹPWM0    1��ʹ��PWM0
,unsigned char Clock_Divided                // PWMʱ�ӷ�Ƶ  ȡֵ��Χ 0~3(1,1/2,1/4,1/8)
,unsigned char Prescalar                    // ʱ�ӷ�Ƶ     ȡֵ��Χ 1~256
,unsigned short Count_Buffer                // ����PWM���������
,unsigned short Compare_Buffer              // ����ռ�ձ�
)
{
	Select_PWM1();
	Set_PWM_Prescaler_1_to_256(Prescalar);
 
	if(Clock_Divided ==0)	Select_PWM1_Clock_Divided_By_1();
	if(Clock_Divided ==1)	Select_PWM1_Clock_Divided_By_2();
	if(Clock_Divided ==2)	Select_PWM1_Clock_Divided_By_4();
	if(Clock_Divided ==3)	Select_PWM1_Clock_Divided_By_8();

	Set_Timer1_Count_Buffer(Count_Buffer); 
	Set_Timer1_Compare_Buffer(Compare_Buffer); 

	if (on_off == 1)	Start_PWM1(); 
	if (on_off == 0)	Stop_PWM1();
}

#if 0
void LT738_PWM1_Duty(unsigned short Compare_Buffer)
{
	Set_Timer1_Compare_Buffer(Compare_Buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------

// LT738�������ģʽ
void LT738_Standby(void)
{
	Power_Saving_Standby_Mode();
	Check_Power_is_Saving();
}
// �Ӵ���ģʽ�л���
void LT738_Wkup_Standby(void)
{
	Power_Normal_Mode();
	Check_Power_is_Normal();
}


// LT738������ͣģʽ
void LT738_Suspend(void)
{
	LT738_SDRAM_initail(10);
	Power_Saving_Suspend_Mode();
	Check_Power_is_Saving();
}
// ����ͣģʽ�л���
void LT738_Wkup_Suspend(void)
{
	Power_Normal_Mode();
	Check_Power_is_Normal();
	LT738_SDRAM_initail(MCLK);
}


// LT738��������ģʽ
void LT738_SleepMode(void)
{
	Power_Saving_Sleep_Mode();
	Check_Power_is_Saving();
}
// ������ģʽ�л���
void LT738_Wkup_Sleep(void)
{
	Power_Normal_Mode();
	Check_Power_is_Normal();
}

//-------------------------------------------------------------------------------------------------------------------------------
void MPU_Memory_Write_Window_8bit//ͨ�����ù������ڵķ�����㻭ͼ
(
 unsigned short x            // x����
,unsigned short y            // y����
,unsigned short w            // ��� ע�⣺����Ϊ4�ı���
,unsigned short h            // �߶�
,const unsigned char *data   // ����(8λ)
)
{
    static bool isMainWindow = false;
    unsigned long imageAddress = 0U;
    if (x == 0 && y == 0
            && w == LCD_WIDTH && h == LCD_HEIGHT)
    {
        if (!isMainWindow)
        {
            imageAddress = LCD_XSIZE_TFT * LCD_YSIZE_TFT * 16;

        }

        /* Write image data to display image buffer */
        Canvas_Image_Start_address(imageAddress);
        Canvas_image_width(w);
        Active_Window_XY(x, y);
        Active_Window_WH(w, h);
        Graphic_Mode();
        Goto_Pixel_XY(x,y);
        Memory_Select_SDRAM();
        LCD_CmdWrite(0x04);
        LCD_DataWrite_Line(data, h * w * 2);
        Check_Mem_WR_FIFO_Empty();

        /* Select main window image */
        Main_Image_Start_Address(imageAddress);
        Main_Image_Width(LCD_XSIZE_TFT);
        Main_Window_Start_XY(0,0);
        isMainWindow = !isMainWindow;
    }
    else
    {
        Graphic_Mode();
        Active_Window_XY(x,y);
        Active_Window_WH(w,h);
        Goto_Pixel_XY(x,y);
        LCD_CmdWrite(0x04);

        /* Send image data to the SPI */
        LCD_DataWrite_Line(data, h *w * 2);

        Check_Mem_WR_FIFO_Empty();
        Active_Window_XY(0,0);
        Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);
    }
}

void MPU_Memory_Write_Pixel_8bit//ͨ���ı�����ķ�����㻭ͼ
(
 unsigned short x            // x����
,unsigned short y            // y����
,unsigned short w            // ���
,unsigned short h            // �߶�
,const unsigned char *data   // ����(8λ)
)
{
	unsigned short i,j,temp;
	Graphic_Mode();
	for(j=0;j< h;j++)
	{
		for(i=0;i< w;i++)
		{
			 Goto_Pixel_XY(x+i,y+j);//�趨����
			 LCD_CmdWrite(0x04);//дSDRAM�Ĵ���
			 temp = *data;
			 data++;
			 temp+=(*data<<8);
			 LCD_DataWrite_Pixel(temp);//д16λ����
			 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}

#if 0 /* Roti App doesn't need this function */
void MPU_Memory_Write_8bit_NOSS  //Í¨¹ýÐ´ÈëÃ¿Ò»ÐÐµÄ·½Ê½»­Í¼
(
 unsigned short x				// x×ø±ê
,unsigned short y				// y×ø±ê
,unsigned short w				// ¿í¶È 
,unsigned short h				// ¸ß¶È
,const unsigned char *data		// Êý¾Ý(8Î»)
)			
{
	unsigned short i,j,temp;
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	SS_RESET;
	SPI2_ReadWriteByte(0x80);
		for(i=0;i< h;i++)
	{
		for(j=0;j< w;j++)
		{
			 temp = *data;
			 data++;
			 temp+=(*data<<8);
			 LCD_DataWrite_Pixel_SPI_NOSS(temp);
			 data++;
		}
	}
	SS_SET;
}
#endif

u8 BUFF[LCD_XSIZE_TFT*2] = {0};

//MCU��ȡSPI flash��㻭ͼ
//x,y:����  w,h:��Ⱥ͸߶�  addr:flash��ַ
void Memory_Write_Flash(u16 x,u16 y,u16 w,u16 h,u32 addr)
{
	u16 i,j,color;
	Graphic_Mode();
	for(j = 0 ; j < h ; j++)
	{
		W25X_Read_Data(BUFF,addr,w*2);//MCU��ȡͼƬһ�е�����
		Goto_Pixel_XY(x,y+j);
		LCD_CmdWrite(0x04);
		for(i = 0 ; i < w ; i++)//д��һ������
		{
			color = BUFF[i*2+1];
			color = ((color<<8)&0xff00)+BUFF[i*2];
			if((i%32)==0)	Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel(color);
		}
		addr+=w*2;//ָ����һ��
	}
	Check_Mem_WR_FIFO_Empty();
}

#if 0 /* Roti App doesn't need this function */
void Memory_Write_Flash_SPI(u16 x,u16 y,u16 w,u16 h,u32 addr)
{
	u16 i,j,color;
	Graphic_Mode();
	for(j = 0 ; j < h ; j++)
	{
		W25X_Read_Data(BUFF,addr,w*2);//MCU Read the data on one line of the picture
		Goto_Pixel_XY(x,y+j);
		LCD_CmdWrite(0x04);
		SS_RESET;
		SPI2_ReadWriteByte(0x80);
		for(i = 0 ; i < w ; i++)//Write a row of data
		{
			color = BUFF[i*2+1];
			color = ((color<<8)&0xff00)+BUFF[i*2];
			if((i%32)==0)	Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel_SPI_NOSS(color);
		}
		addr+=w*2;//Point to next line
		SS_SET;
	}
	Check_Mem_WR_FIFO_Empty();
}
#endif


#endif
//-------------------------------------------------------------------------------------------------------------------------------



