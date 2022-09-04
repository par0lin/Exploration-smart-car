#include "oled.h"
#include "iic.h"
#include "oledfont.h"
#include "QRget.h"

const u8  OLED_init_cmd[25]=
{
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//�ر���ʾ
       0xD5,//����ʱ�ӷ�Ƶ����,��Ƶ��
       0x80,  //[3:0],��Ƶ����;[7:4],��Ƶ��

       0xA8,//��������·��
       0X3F,//Ĭ��0X3F(1/64)
       0xD3,//������ʾƫ��
       0X00,//Ĭ��Ϊ0
       0x40,//������ʾ��ʼ�� [5:0],����.                              
       0x8D,//��ɱ�����
       0x14,//bit2������/�ر�
       0x20,//�����ڴ��ַģʽ
       0x02,//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
       0xA1,//���ض�������,bit0:0,0->0;1,0->127;
       0xC8,//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
       0xDA,//����COMӲ����������
       0x12,//[5:4]����            
       0x81,//�Աȶ�����
       0xEF,//1~255;Ĭ��0X7F (��������,Խ��Խ��)
       0xD9,//����Ԥ�������
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//����VCOMH ��ѹ����
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
       0xA6,//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ        
       0xAF,//������ʾ     
};
u8 OLED_GRAM[128][8];
void OLED_send_cmd(u8 o_command)
{
    
    IIC_Start();
		IIC_Send_Byte(0x78); 
		IIC_Wait_Ack();
    IIC_Send_Byte(0x00); 
		IIC_Wait_Ack();
    IIC_Send_Byte(o_command);
		IIC_Wait_Ack();
    IIC_Stop();
}
void OLED_send_data(u8 o_data)
{ 
    IIC_Start();
		IIC_Send_Byte(0x78); 
		IIC_Wait_Ack();
    IIC_Send_Byte(0x40);	
		IIC_Wait_Ack();
    IIC_Send_Byte(o_data);
		IIC_Wait_Ack();
    IIC_Stop();
}
void Column_set(u8 column)
{
    OLED_send_cmd(0x10|(column>>4));    //�����е�ַ��λ
    OLED_send_cmd(0x00|(column&0x0f));   //�����е�ַ��λ  
}
void Page_set(u8 page)
{
    OLED_send_cmd(0xb0+page);
}
void OLED_Init(void)
{
	IIC_Init();
	u8 i;
	for(i=0;i<25;i++)
	{
		OLED_send_cmd(OLED_init_cmd[i]);
	}
	OLED_Clear();
}
void OLED_Refresh_Gram(void)
{
	u8 page,column;
	for(page=0;page<(64/8);page++)        //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(OLED_GRAM[column][page]);
		}
	}
}
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;  
		OLED_Refresh_Gram();//������ʾ
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}
//	for(x=0;x<128;x++)
//		for(y=0;y<8;y++)
//	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	//OLED_Refresh_Gram();
}
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,s32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;
	if(num<0)
	{
		OLED_ShowChar(x,y,'-',size,1);
		num=-num;
		x+=size/2;
	}
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
	//OLED_Refresh_Gram();
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
	while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
	{       
			if(x>(128-(size/2))){x=0;y+=size;}
			if(y>(64-size)){y=x=0;OLED_Clear();}
			OLED_ShowChar(x,y,*p,size,1);	 
			x+=size/2;
			p++;
	}  
	//OLED_Refresh_Gram();
}	
//��ʾ1��������
//x,y :�������	 
//z_len :����λ��
//f_len :С��λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ	 		  
void OLED_ShowFloat(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size)
{         	
	u8 t,temp;
	u8 enshow;
	int z_temp,f_temp;      
	z_temp=(int)num;
	if(num<0)
	{
		OLED_ShowChar(x,y,'-',size,1);
		num=-num;
		x+=size/2;
	}
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/mypow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
	}
	OLED_ShowChar(x+(size/2)*(z_len),y,'.',size,1);
	
	f_temp=(int)((num-z_temp)*(mypow(10,f_len)));
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/mypow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size/2)*(t+z_len)+5,y,temp+'0',size,1); 
	}
}
void OLED_ShowTask()
{
	for(int i = 0;i < 7; i++)
	{
		OLED_ShowChar(i*18,10,QRmessage[i],24,1);
	}
	OLED_Refresh_Gram();
}