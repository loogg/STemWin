#include <board.h>
#include "string.h"
#include <u8g2_port.h>

//#define DRV_DEBUG
#define LOG_TAG "drv.lcd"
#include <drv_log.h>

static u8g2_t u8g2;
#define OLED_SPI_PIN_RES					GET_PIN(C, 8)  
#define OLED_SPI_PIN_DC					  GET_PIN(C, 9) 
#define OLED_SPI_PIN_CS					  GET_PIN(G, 7) 

static uint8_t *oled_buf_ptr;

#define LCD_DEVICE(dev) (struct drv_lcd_device *)(dev)

struct drv_lcd_device
{
    struct rt_device parent;

    struct rt_device_graphic_info lcd_info;
};

static struct drv_lcd_device _lcd;

static void oled_thread(void *param)
{
    while(1)
    {
        u8g2_SendBuffer(&u8g2);
        rt_thread_mdelay(100);
    }
}

static rt_err_t drv_lcd_init(struct rt_device *device)
{
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_rt_4wire_hw_spi, u8x8_rt_gpio_and_delay);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_CS, OLED_SPI_PIN_CS);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_DC, OLED_SPI_PIN_DC);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_RESET, OLED_SPI_PIN_RES);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2);
    oled_buf_ptr = u8g2_GetBufferPtr(&u8g2); 

    struct drv_lcd_device *lcd = LCD_DEVICE(device);

    lcd->lcd_info.width = u8g2_GetDisplayWidth(&u8g2);
    lcd->lcd_info.height = u8g2_GetDisplayHeight(&u8g2);
    
    rt_thread_t tid;
    tid = rt_thread_create("oled_refresh", oled_thread, RT_NULL, 1024, RT_THREAD_PRIORITY_MAX - 3, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return RT_EOK;
}

static rt_err_t drv_lcd_control(struct rt_device *device, int cmd, void *args)
{
    struct drv_lcd_device *lcd = LCD_DEVICE(device);
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_GET_INFO:
    {
        struct rt_device_graphic_info *info = (struct rt_device_graphic_info *)args;

        RT_ASSERT(info != RT_NULL);
        
        //this needs to be replaced by the customer
        info->pixel_format  = lcd->lcd_info.pixel_format;
        info->bits_per_pixel = lcd->lcd_info.bits_per_pixel;
        info->width = lcd->lcd_info.width;
        info->height = lcd->lcd_info.height;
    }
    break;
    }

    return RT_EOK;
}

static void set_pixel(const char *pixel, int x, int y)
{
    if(*pixel)
        u8g2_SetDrawColor(&u8g2,1);
    else 
        u8g2_SetDrawColor(&u8g2,0);
    u8g2_DrawPixel(&u8g2, x, y);
}

static void get_pixel(char *pixel, int x, int y)
{
    uint8_t buf_y = y/8;
    uint8_t color = *(oled_buf_ptr + buf_y * 128 + x);
    *pixel = color & (1 << y%8);
}

void draw_hline(const char *pixel, int x1, int x2, int y)
{
    if(*pixel)
        u8g2_SetDrawColor(&u8g2,1);
    else 
        u8g2_SetDrawColor(&u8g2,0);
    u8g2_DrawHLine(&u8g2, x1, y, x2-x1+1);
}

struct rt_device_graphic_ops oled_ops =
    {
        set_pixel,
        get_pixel,
        draw_hline,
        RT_NULL,
        RT_NULL,
};



#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops lcd_ops =
    {
        drv_lcd_init,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        RT_NULL,
        drv_lcd_control};
#endif

int drv_lcd_hw_init(void)
{
    rt_err_t result = RT_EOK;
    struct rt_device *device = &_lcd.parent;
    /* memset _lcd to zero */
    memset(&_lcd, 0x00, sizeof(_lcd));

    _lcd.lcd_info.bits_per_pixel = 1;
    _lcd.lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_MONO;

    device->type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    device->ops = &lcd_ops;
#else
    device->init = drv_lcd_init;
    device->control = drv_lcd_control;
#endif
    device->user_data = &oled_ops;
    /* register lcd device */
    rt_device_register(device, "lcd", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);

    return result;
}
INIT_DEVICE_EXPORT(drv_lcd_hw_init);
