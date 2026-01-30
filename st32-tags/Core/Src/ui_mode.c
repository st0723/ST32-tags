#include "ui_mode.h"
#include "lcd.h"

static void UIKeyEvent(KEY_CODE code);
static void UIInit(ENUM_OPMODE srcMode, void *param, ENUM_OPMODE_FLAG flag);
ST_UI_HANDLE UIHandle_MODEConfig ={OPMODE_MODE_CONFIG, UIKeyEvent, UIInit};

/*相关界面*/
enum 
{
    SYSCFG_MODE_MENU,							//主菜单界面
};
typedef BYTE ENUM_SYS_CONFIG_MODE;

typedef struct
{
	BYTE              		SelectedIndex;		//选择索引
	ENUM_SYS_CONFIG_MODE 	Mode;				//界面模式
} ST_UI_MODE_STATUS;
static ST_UI_MODE_STATUS UIStatus = 
{
    .SelectedIndex = 0,
	.Mode = SYSCFG_MODE_MENU,
};

static void UI_ConfigPageRefresh(void)
{
	switch(UIStatus.Mode)
	{
		case SYSCFG_MODE_MENU:
		{
			lcdPutShortText("P%02d",UIStatus.SelectedIndex);
			break;
		}
	}
}

static void UIKeyEvent(KEY_CODE key)
{
    switch(key)
    {
        case KEY_EX_MODE:
		{
			printf("KEY_EX_UP_MODE\n");
            break;
		}
		case KEY_EX_UP:
		{
			printf("KEY_EX_UP_MODE\n");
			switch(UIStatus.Mode)
			{
				case SYSCFG_MODE_MENU:
				{
					UIStatus.SelectedIndex = (UIStatus.SelectedIndex > 1)?(UIStatus.SelectedIndex - 1):(25);
					UI_ConfigPageRefresh();
					break;
				}			
			}
            break;
		}
		case KEY_EX_DOWN:
		{
			printf("KEY_EX_DOWN_MODE\n");
			switch(UIStatus.Mode)
			{
				case SYSCFG_MODE_MENU:
				{
					UIStatus.SelectedIndex = (UIStatus.SelectedIndex < 25)?(UIStatus.SelectedIndex + 1):(1);
					UI_ConfigPageRefresh();
					break;
				}			
			}
            break;
		}
		case KEY_EX_ENTER:
		{
			printf("KEY_EX_ENTER_MODE\n");
            break;
		}
    }
}

static void UIInit(ENUM_OPMODE srcMode, void *param, ENUM_OPMODE_FLAG flag)
{
	printf("mode UIInit");
	UIStatus.SelectedIndex   = 1;	
	UI_ConfigPageRefresh();	
}




