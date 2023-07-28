#include "ps2.h"
#include "bsp_ps2.h"
#include "bsp_spi.h"

/*********************************************************     
**********************************************************/	 
unsigned int Handkey;	// ����ֵ��ȡ����ʱ�洢��
//uint8_t Key_num;
uint8_t ps2_mode;
uint8_t Comd[9]={0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	//��ʼ�����������
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
uint8_t key_search[12] = {0x00,0x00,0x00,0x00,
						  0x00,0x00,0x00,0x00,
						  0x00,0x00,0x00,0x00};

unsigned int MASK[16][2]={
    {PSB_SELECT,0},
    {PSB_L3,0},
    {PSB_R3,0},
    {PSB_START,0},
    {PSB_PAD_UP,0},
    {PSB_PAD_RIGHT,0},
    {PSB_PAD_DOWN,0},
    {PSB_PAD_LEFT,0},
    {PSB_L2,0},
    {PSB_R2,0},
    {PSB_L1,0},
    {PSB_R1 ,0},
    {PSB_GREEN,0},
    {PSB_RED,0},
    {PSB_BLUE,0},
    {PSB_PINK,0}
	};	//����ֵ�밴����

void PS2_Init(void)
{ 	

	#ifdef BSP_SUPPORT_PS2_GPIO
	bsp_ps2_init();	
	PS2_SCK_H();
	PS2_CS_H();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
	//PS2_VibrationMode();	//������ģʽ
	PS2_ExitConfing();		//��ɲ��������� 
	ps2_mode_get();
	#endif 
	
	#ifdef BSP_SUPPORT_PS2_SPI
	bsp_ps2_init();	
	#endif

}

//���ֱ���������
unsigned char PS2_Cmd(unsigned char CMD)
{    
	unsigned char i,j=1;
	unsigned char res=0; 
	for(i=0;i<=7;i++)     //��λ����     
	{
		 if(CMD&0x01)
			PS2_DO_H();  
		 else
			PS2_DO_L();  
		 
		 CMD=CMD>>1;
		 
		 PS2_Delay_US(100);
		 PS2_SCK_L();   	
		 PS2_Delay_US(100);
		 
		 if(PS2_DI()) 
		 {
			res=res+j;
		 }
		 j=j<<1;
		
		 PS2_SCK_H();   
		 PS2_Delay_US(1);		 
	}

	return res;	
}

//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	unsigned char byte=0;
	#ifdef BSP_SUPPORT_PS2_GPIO
	PS2_CS_L();
    PS2_Delay_US(10);

    for(byte = 0; byte < 9; byte++)
    {
        if(byte < 2)
        {
            Data[byte] = PS2_Cmd(Comd[byte]);
        }
        else
        {
            Data[byte] = PS2_Cmd(0x00);
        }
    }
	PS2_CS_H();
	PS2_Delay_US(10);
	#endif 
	
	#ifdef BSP_SUPPORT_PS2_SPI
		PS2_CS_L();
		PS2_Delay_US(10);
		bsp_spi_transmit(Comd, 5);
        PS2_Delay_US(10);
        PS2_CS_H();
        PS2_Delay_US(10);
        PS2_CS_L();
        bsp_spi_receive(Data,5);
		PS2_Delay_US(10);
		PS2_CS_H();
	#endif
}

//�ж��Ƿ�Ϊ���ģʽ,0x41=ģ���̵ƣ�0x73=ģ����
//����ֵ��0�����ģʽ
//		  ����������ģʽ
unsigned char ps2_mode_get(void)
{	
	if( Data[1] == 0X73)  
	{
		ps2_mode = PSB_REDLIGHT_MODE;
	}
	else if ( Data[1] == 0X41)
	{
		ps2_mode = PSB_GREENLIGHT_MODE;
	}
	else
	{
		ps2_mode = PSB_LOSE;
	}
	return ps2_mode;
}

//�Զ�������PS2�����ݽ��д���,ֻ������������  
//ֻ��һ����������ʱ����Ϊ0�� δ����Ϊ1
unsigned char ps2_key_serch()
{
	unsigned char index;
	unsigned char key_num = 0;
	//PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=4;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index][0]-1)))==0)
		{
			MASK[index][1] = 1; 
			key_num++;
		}
		else
		{
			MASK[index][1] = 0; 
		}
	}
	return key_num;
}

unsigned char ps2_get_key_state(unsigned char key_id)
{
	if (key_id < PSB_SELECT)
		return 0;
	else
		return MASK[key_id - 1][1];
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
unsigned char ps2_get_anolog_data(unsigned char button)
{
	return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
	unsigned char a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

/******************************************************
Function:    void PS2_Vibration(unsigned char motor1, unsigned char motor2)
Description: �ֱ��𶯺�����
Calls:		 void PS2_Cmd(unsigned char CMD);
Input: motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	   motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
******************************************************/
void PS2_Vibration(unsigned char motor1, unsigned char motor2)
{
	PS2_CS_L();
	PS2_Delay_US(100);
    HAL_Delay(1);
	PS2_Cmd(0x01);  //��ʼ����
	PS2_Cmd(0x42);  //��������
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_CS_H();
	PS2_Delay_US(100);
}

//short poll
void PS2_ShortPoll(void)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	PS2_CS_H();
	PS2_Delay_US(100);	
}

//��������
void PS2_EnterConfing(void)
{
    PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_CS_H();
	PS2_Delay_US(100);
}

//����ģʽ����
void PS2_TurnOnAnalogMode(void)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  �������÷���ģʽ
	PS2_Cmd(0xEE); //Ox03�������ã�������ͨ��������MODE������ģʽ��
				   //0xEE�������������ã���ͨ��������MODE������ģʽ��
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_CS_H();
	PS2_Delay_US(100);
}

//������
void PS2_VibrationMode(void)
{
	PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	PS2_CS_H();
	PS2_Delay_US(100);	
}

//��ɲ���������
void PS2_ExitConfing(void)
{
    PS2_CS_L();
	PS2_Delay_US(100);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_CS_H();
	PS2_Delay_US(100);
}

void PS2_CS_H()
{
	bsp_ps2_cs_set_high();
}

void PS2_CS_L()
{
	bsp_ps2_cs_set_low();
}

void PS2_SCK_H()
{
	bsp_ps2_clk_set_high();
}

void PS2_SCK_L()
{
	bsp_ps2_clk_set_low();
}

void PS2_DO_H()
{
	bsp_ps2_do_set_high();	
}

void PS2_DO_L()
{
	bsp_ps2_do_set_low();
}

void PS2_DI_H()
{
	bsp_ps2_do_set_high();	
}

void PS2_DI_L()
{
	bsp_ps2_do_set_low();
}

uint8_t PS2_DI()
{
	uint8_t di = bsp_ps2_get_di();
	return di;
}

void PS2_Delay_US(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}