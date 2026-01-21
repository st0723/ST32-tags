#include "UI_Main.h"


static void UIKeyEvent(KEY_CODE code);
static void UIInit(ENUM_OPMODE srcMode, void *param, ENUM_OPMODE_FLAG flag);
ST_UI_HANDLE UIHandle_Main ={OPMODE_MAIN, UIKeyEvent, UIInit};

static void UIInit(ENUM_OPMODE srcMode, void *param, ENUM_OPMODE_FLAG flag)
{
    switch(flag)
    {

    }
}

static void UIKeyEvent(KEY_CODE key)
{
    switch(key)
    {
        case KEY_EX_MODE:
		{
			printf("KEY_EX_MODE\n");
            break;
		}
		case KEY_EX_UP:
		{
			printf("KEY_EX_UP\n");
            break;
		}
		case KEY_EX_DOWN:
		{
			printf("KEY_EX_DOWN\n");
            break;
		}
		case KEY_EX_ENTER:
		{
			printf("KEY_EX_ENTER\n");
            break;
		}
    }
}

