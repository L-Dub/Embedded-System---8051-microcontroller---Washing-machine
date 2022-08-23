sbit LCD_RS at P0_0_bit;
sbit LCD_EN at P0_1_bit;

sbit LCD_D4 at P0_2_bit;
sbit LCD_D5 at P0_3_bit;
sbit LCD_D6 at P0_4_bit;
sbit LCD_D7 at P0_5_bit;

sbit Door at P1_0_bit;
sbit Start at P1_1_bit;
sbit MOTO at P3_6_bit;

unsigned char *send, *stri, arr[25], stage[] = "Door Open ";
const *mess;
unsigned int cntdel, j;

void initializ()
{
     P2 = 0;
     MOTO = 0;
     P1 = 0;
     TMOD = 0x21;
     TH1 = 0xFD;
     SCON = 0x50;
     
     TH0 = 0xFC;
     TL0 = 0x66;
     TR1_bit = 1;
     TR0_bit = 1;
}

void serial(unsigned char *send)
{
     int i, x;
     i = strlen(send);
     for(x=0; x<i; x++)
     {
         SBUF = send[x];
         while(TI_bit == 0);
         TI_bit = 0;
     }
     //Lcd_Out(1, 1, send);
}

void cpp(char *dest, const char *sour)
{
     while(*sour)
     {
         *dest++ = *sour++;
     }
     *dest = 0;
}

void lcd(const char *mess)
{
     cpp(arr,mess );
     Lcd_Out(1, 1, arr);
}

void stage2();
void stage3();
void stage4();
void stage5();
void stage6();
void comple();

void EXT_INT() iv IVT_ADDR_EX0 ilevel 0 ics ICS_AUTO {
     P0 = 0;
     P2 = 0;
     P3 = 0;
}

void timo() iv IVT_ADDR_ET0 ilevel 0 ics ICS_AUTO {
     cntdel++;
     
     TF0_bit = 0;
     
     TH0 = 0xFC;
     TL0 = 0x66;
}

void stage2()
{
     MOTO = 1;
     serial("Stage 2 ");
     lcd("Door Closed,Dispensing");
     P2 = 0x03;
     while(cntdel<1000);
     cntdel = 0;
}

void stage3()
{
     serial("Stage 3 ");
     lcd("Washing                ");
     P2 = 0x07;
     while(cntdel<2000);
     cntdel = 0;
}

void stage4()
{
     serial("Stage 4 ");
     lcd("Draining               ");
     P2 = 0x0F;
     while(cntdel<500);
     cntdel = 0;
     MOTO = 0;
}

void stage5()
{
     MOTO = 1;
     serial("Stage 5 ");
     lcd("Rinsing                ");
     P2 = 0x1F;
     while(cntdel<2000);
     cntdel = 0;
}

void stage6()
{
     serial("Stage 6 ");
     lcd("Spinning               ");
     P2 = 0x0F;
     while(cntdel<1000);
     cntdel = 0;
}

void comple()
{
     MOTO = 0;
     serial("Complete ");
     lcd("Complete               ");
     P2 = 0xFF;
     while(cntdel<250);
     cntdel = 0;
     P2 = 0x00;
     while(cntdel<250);
     cntdel = 0;
}
void main() {
     EA_bit = 1;
     ET0_bit = 1;
     
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     
     initializ();
     while(1)
     {
         if(Door == 1)
         {
             MOTO = 0;
             //serial("Door Open ");
             for(j=0; j<10; j++)
             {
                 SBUF = stage[j];
                 while(TI_bit == 0);
                 TI_bit = 0;
             }
             
             lcd("Door Open, ");
             while(cntdel<950);
             cntdel = 0;
             P2 = 0x01;
         }
         else if(Door == 0 && Start == 1)
         {
             stage2();
             while(cntdel<250);
             cntdel = 0;
             stage3();
             while(cntdel<250);
             cntdel = 0;
             stage4();
             while(cntdel<250);
             cntdel = 0;
             stage5();
             while(cntdel<250);
             cntdel = 0;
             stage6();
             while(cntdel<250);
             cntdel = 0;
             comple();
         }
     }
}