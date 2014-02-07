#include <common.h>
#include <asm/io.h>
#include <lcd.h>
#include <asm/arch/s3c6410.h>

#define  VSPW         10
#define  VBPD          2
#define  LINEVAL     271
#define  VFPD          2


#define  HSPW         41    
#define  HBPD          2
#define  HOZVAL      479
#define  HFPD          2

#define LeftTopX     0
#define LeftTopY     0

#define RightBotX   479
#define RightBotY   271


void lcd_ctrl_init(void *lcdbase)
{
	unsigned int reg;

	writel(0xaaaaaaaa, GPICON);
	writel(0xaaaaaaaa, GPJCON);

	reg = readl(GPFCON);
	reg &= ~(0x3<<28);
	reg |=  (1<<28);
	writel(reg, GPFCON);

	reg = readl(MIFPCON);
	reg &= ~(1<<3);   /* Normal mode */
	writel(reg, MIFPCON);

	reg = readl(SPCON);
        reg &= ~(0x3);
        reg |= 0x1;            /* RGB I/F style */
	writel(reg, SPCON);
	
	reg = readl(VIDCON0);
	reg &= ~((3<<26) | (3<<17) | (0xff<<6)  | (3<<2) | 0x3);     /* RGB I/F, RGB Parallel format,  */
        reg |= ((14<<6) | (1<<4) );      /* vclk== HCLK / (CLKVAL+1) = 133/15 = 9MHz */
	writel(reg, VIDCON0);

	reg = readl(VIDCON1);
	reg &= ~(1<<7);   /*LCD driver gets the video data at VCLK falling edge */
        reg |= ((1<<6) | (1<<5));  /* HSYNC and VSYNC pulse polarity inverted */
	writel(reg, VIDCON1);

        writel((VBPD << 16) | (VFPD << 8) | (VSPW << 0), VIDTCON0);
        writel((HBPD << 16) | (HFPD << 8) | (HSPW << 0), VIDTCON1);
        writel((LINEVAL << 11) | (HOZVAL << 0), VIDTCON2);

	reg = readl(WINCON0);
	reg &= ~(0xf << 2);
       	reg |= (0xb<<2);    /* unpacked 24 BPP (non-palletized R:8-G:8-B:8 ) */
	writel(reg, WINCON0);

        writel((LeftTopX<<11) | (LeftTopY << 0), VIDOSD0A);
        writel(( 480 <<11) | ( 272 << 0), VIDOSD0B);
        writel((LINEVAL + 1) * (HOZVAL + 1), VIDOSD0C);

        writel(lcdbase, VIDW00ADD0B0);
        writel((((HOZVAL + 1)*4 + 0) * (LINEVAL + 1)) & (0xffffff), VIDW00ADD1B0);
                                        /* VBASEL = VBASEU + (LINEWIDTH+OFFSIZE) x (LINEVAL+1) 
                          *        = 0 + (480*4 + 0) * 272
                          *        = 
                             */
        writel((HOZVAL + 1), VIDW00ADD2);
	
	reg = readl(GPFDAT);
	reg |= (1<<14);
	writel(reg, GPFDAT);

	reg = readl(VIDCON0);
	reg |= 0x3;
	writel(reg, VIDCON0);

	reg = readl(WINCON0);
        reg |= 1;
	writel(reg, WINCON0);
}
