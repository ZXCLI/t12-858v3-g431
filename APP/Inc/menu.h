// #ifndef __MENU_H__
// #define __MENU_H__
// #include "loop.h"
// #include "encoder.h"

// typedef struct
// {
// 	char* str;					//显示内容
// 	void (*function_list)();	//选项功能函数
// 	bool selected;				//是否被选中
// }list;

// typedef struct	MENU
// {
// 	uint16_t list_num;			//选项个数
// 	uint16_t index;				//菜单选项索引
// 	uint16_t offset_box;		//方框偏移量
//     uint16_t offset_page;       //界面偏移量
// 	void (*function_menu)();	//当前菜单页功能函数，功能包括上下移动光标
// 	struct MENU *last_menu;		//上级菜单
// 	list lists[30];			    //当前菜单里面的选项，动态数组会出现内存问题，改为使用固定长度数组//动态数组形式，理论上可以存放无限个选项
// }menu;

// extern char tips[21][16];
// extern menu* NOW_MENU;
// extern menu IRON_MENU,GUN_MENU,SETTING;

// void RUN_UI(void);

// //函数注册：
// void SETTING_choose_tips();
// void SETTING_calibration();
// void SETTING_about();
// void SETTING_erase_all_datas();

// void ABOUT_F();
// void ABUOT_return();


// void CHOOSE_TIPS_click();

// void currency_MENU();       //通用菜单显示函数
// void currency_RETURN();	    //通用返回函数

// void none();                //该函数不执行任何代码

// #endif