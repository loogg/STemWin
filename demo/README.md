# LCD驱动

## 1、LCD设备注册

```
struct drv_lcd_device
{
    struct rt_device parent;

    struct rt_device_graphic_info lcd_info;
};
```
首先定义LCD设备结构体，`rt_device_graphic_info`结构有如下参数：
```
struct rt_device_graphic_info
{
    rt_uint8_t  pixel_format;                           /**< graphic format */
    rt_uint8_t  bits_per_pixel;                         /**< bits per pixel */
    rt_uint16_t reserved;                               /**< reserved field */

    rt_uint16_t width;                                  /**< width of graphic device */
    rt_uint16_t height;                                 /**< height of graphic device */

    rt_uint8_t *framebuffer;                            /**< frame buffer */
};
```
这些参数在设备注册或初始化的时候需要赋值。

| 名称 | 说明 |
| ---- | ---- |
| pixel_format | 颜色格式 |
| bits_per_pixel  | 颜色位数 |
| width | 屏幕宽度 |
| height  | 屏幕高度 |

`rt_device_control`函数的设备回调函数如下：
```
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
        info->width = lcddev.width;
        info->height = lcddev.height;
    }
    break;
    }

    return RT_EOK;
}
```

| 名称 | 说明 |
| ---- | ---- |
| RTGRAPHIC_CTRL_GET_INFO  | 读取参数，参数信息由用户实现 |

对LCD操作函数通过如下结构体实现：
```
struct rt_device_graphic_ops
{
    void (*set_pixel) (const char *pixel, int x, int y);
    void (*get_pixel) (char *pixel, int x, int y);

    void (*draw_hline)(const char *pixel, int x1, int x2, int y);
    void (*draw_vline)(const char *pixel, int x, int y1, int y2);

    void (*blit_line) (const char *pixel, int x, int y, rt_size_t size);
};
```

```
void (*set_pixel) (const char *pixel, int x, int y);
void (*get_pixel) (char *pixel, int x, int y);
```

这两个回调函数必须设置，其余三个函数是为了优化性能提供。

将LCD操作函数填入，如下：
```
struct rt_device_graphic_ops fsmc_lcd_ops =
    {
        LCD_Fast_DrawPoint,
        LCD_ReadPoint,
        LCD_HLine,
        RT_NULL,
        LCD_BlitLine,
};
```

最后将LCD注册进设备，如下：
```
int drv_lcd_hw_init(void)
{
    rt_err_t result = RT_EOK;
    struct rt_device *device = &_lcd.parent;
    /* memset _lcd to zero */
    memset(&_lcd, 0x00, sizeof(_lcd));

    _lcd.lcd_info.bits_per_pixel = 16;
    _lcd.lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB565;

    device->type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    device->ops = &lcd_ops;
#else
    device->init = drv_lcd_init;
    device->control = drv_lcd_control;
#endif
    device->user_data = &fsmc_lcd_ops;
    /* register lcd device */
    rt_device_register(device, "lcd", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);

    return result;
}
```
