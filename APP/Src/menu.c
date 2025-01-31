// /*2023-10-2	by zxcli
//  *Code version: 1.2
//  *多级菜单
//  * menu结构体为一页菜单
//  * list结构体为一页菜单里面的一个选项
//  * 功能函数命名：
//  * 				通用功能函数：currency_+大写
//  * 				另外的功能函数：当前菜单名(大写)_当前选项名(小写)
//  *
//  * 结构体详情：
// typedef struct
// {
//     char* str;					//显示内容
//     void (*function_list)();	//选项功能函数
//     bool selected;				//是否被选中
// }list;

// typedef struct	MENU
// {
//     uint16_t list_num;			//选项个数
//     uint16_t index;				//菜单选项索引
//     uint16_t offset_box;		//方框偏移量
//     uint16_t offset_page;       //界面偏移量
//     void (*function_menu)();	//当前菜单页功能函数，功能包括上下移动光标
//     struct MENU *last_menu;		//上级菜单
//     list lists[30];				//当前菜单里面的选项，动态数组形式，理论上可以存放无限个选项
// }menu;
//  */
// #include "menu.h"
// #define size 12
// #define fonts_wide 6

// menu IRON_MENU=
// {
// 	0,
// 	0,
// 	0,
//     0,
// 	IRON_UI,
// 	NULL,
// 	{
// 		{"",none,0}
// 	}
// };

// menu GUN_MENU=
// {
// 	0,
// 	0,
// 	0,
//     0,
// 	GUN_UI,
// 	NULL,
// 	{
// 		{"",none,0}
// 	}
// };

// menu SETTING =
// {
//     16,
//     0,
//     0,
//     0,
//     currency_MENU,
//     &IRON_MENU,
//     {
//         {"Choose Tips", SETTING_choose_tips, 0},    //选择烙铁头
//         {"Calibration", SETTING_calibration, 0},    //校准
//         {"About", SETTING_about, 0},                //关于
//         {"Return", currency_RETURN, 0},             //返回烙铁或风枪界面
//         {"Erase All Datas", SETTING_erase_all_datas,0},//test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//         {"test", none, 0},                          //test
//     }
// };

// menu* NOW_MENU = &IRON_MENU;

// menu ABOUT=
// {
// 	1,
// 	0,
// 	0,
//     0,
// 	ABOUT_F,
// 	&SETTING,
// 	{
// 		{"stm32g070cbt6",ABUOT_return,0},
// 	}
// };

// menu CHOOSE_TIPS =
// {
//     2,
//     1,
//     0,
//     0,
//     currency_MENU,
//     &SETTING,
//     {
//         {"Return", currency_RETURN, 0},
//         {&(tips[0][0]), none, 0},
//     }
// };

// char tips[21][16]=
// {
//     {"Return"},
//     {"[ ]  K1"},
//     {"[ ]  K2"},
//     {"[ ]  K3"},
//     {"[ ]  K4"},
//     {"[ ]  K5"},
//     {"[ ]  K6"},
//     {"[ ]  K7"},
//     {"[ ]  K8"},
//     {"[ ]  K9"},
//     {"[ ]  K10"},
//     {"[ ]  K1"},
//     {"[ ]  K2"},
//     {"[ ]  K2"},
//     {"[ ]  K2"},
//     {"[ ]  K2"},
//     {"[ ]  K2"},
//     {"[ ]  K1"},
//     {"[ ]  K2"},
//     {"[ ]  K2"},
//     {"[ ]  K2"},
// };

// void RUN_UI()
// {
//     NOW_MENU->function_menu();
//     NOW_MENU->lists[NOW_MENU->index].function_list();
// }

// void SETTING_choose_tips()
// {
//     if (KeyEvent[ENTER].shortPress) {
//         NOW_MENU = &CHOOSE_TIPS;
//         NOW_MENU->offset_box = 0;
//         NOW_MENU->index  = 0; // 下一级菜单进入时确保在第一个选项
//         KeyEvent[ENTER].shortPress  = false;
//     }
// }

// void SETTING_calibration()
// {
	
// }

// /*
//  *通用-菜单列表处理函数
//  */
// void currency_MENU()
// {
//     if(NOW_MENU == &CHOOSE_TIPS){
//         CHOOSE_TIPS.list_num =  (sizeof(tips)>>4);
//         for (uint16_t i = 0; i < CHOOSE_TIPS.list_num; i++) {
//             DrawString(5, size * i + 4 - NOW_MENU->offset_page, tips[i], size >> 3);       
//             if(i&&(i<CHOOSE_TIPS.list_num)){
//                 NOW_MENU->lists[i].function_list = CHOOSE_TIPS_click;
//             }
//         }
//     }else{
// 	    for(uint16_t i = 0;i < NOW_MENU->list_num;i++){
//             DrawString(5, size * i + 4 - NOW_MENU->offset_page, NOW_MENU->lists[i].str, size >> 3);
//         }
//     }
    

// 	if(!NOW_MENU->lists[NOW_MENU->index].selected){		//当前选项未被选中执行时可以向上向下翻动
// 		if(EncoderEvent[ENCODER_1].right){                      //往上翻
//             NOW_MENU->offset_box = ((NOW_MENU->offset_box - size) > 0) ? (NOW_MENU->offset_box - size) : 0;
//             NOW_MENU->index = (NOW_MENU->offset_box/size);
//             if((NOW_MENU->offset_box - NOW_MENU->offset_page + size)<=0){   //选项框到最顶上，让选项下移
//                 NOW_MENU->offset_page -= size;
//             }
// 			EncoderEvent[ENCODER_1].right = false;
// 		}
// 		if(EncoderEvent[ENCODER_1].left){                       //往下翻
//             NOW_MENU->offset_box = ((NOW_MENU->offset_box + size) < (size * (NOW_MENU->list_num - 1))) ? (NOW_MENU->offset_box + size) : (size * (NOW_MENU->list_num - 1));
//             NOW_MENU->index = (NOW_MENU->offset_box/size);
//             if((NOW_MENU->offset_box - NOW_MENU->offset_page + size) > 64){ //选项框到最底下，让选项上移
//                 NOW_MENU->offset_page += size;
//             }
//             EncoderEvent[ENCODER_1].left = false; 
// 		}
// 	}

//     if(NOW_MENU == &CHOOSE_TIPS){
//         DrawMYfillRoundRect(2, NOW_MENU->offset_box - NOW_MENU->offset_page + 1, fonts_wide * (strlen(tips[(NOW_MENU->index)])) + 5, size);
//     }else{
//         DrawMYfillRoundRect(2,(NOW_MENU->offset_box - NOW_MENU->offset_page + 1), fonts_wide * (strlen(NOW_MENU->lists[NOW_MENU->index].str)) + 5, size);
//     }
//     DrawNum(100,30,NOW_MENU->index,2,1,1);
//     if(SETTING.lists[4].selected){//SETTING_erase_all_datas的弹窗
//         DrawMYflaotRoundRect(23,24,50,30);
//     }
// 	flash();
// }

// // void none()
// // {
// // 	__NOP();
// // }

// /*
//  *进入下一级菜单
//  */
// void SETTING_about()
// {
// 	if(KeyEvent[ENTER].shortPress){
// 		NOW_MENU = &ABOUT;
// 		NOW_MENU->offset_box = 0;
// 		NOW_MENU->index = 0;			//下一级菜单进入时确保在第一个选项
// 		KeyEvent[ENTER].shortPress = false;
// 	}
// }

// void SETTING_erase_all_datas()
// {
//     static uint16_t count = 0; 
//     if(KeyEvent[ENTER].shortPress||count){
//         if(KeyEvent[ENTER].shortPress){
//             count = 1000;//给弹窗反馈，虽然几十ms就能清完，但是还是延时几秒再清除弹窗
//             at24_EraseMemFull();
//             KeyEvent[ENTER].shortPress = false;
//         }
//         NOW_MENU->lists[NOW_MENU->index].selected = true;//保证弹窗弹出时其他操作不起作用
//         count--;
//         if(count == 0){
//             NOW_MENU->lists[NOW_MENU->index].selected = false;//延时结束，恢复操作
//         }
//     }
// }

// /*
//  *通用-返回上一级菜单
//  */
// void currency_RETURN()	
// {
// 	if(KeyEvent[ENTER].shortPress){
//         EncoderEvent[ENCODER_1].left = false;
//         EncoderEvent[ENCODER_1].right = false;
//         // NOW_MENU->offset_box = 0;
// 		// NOW_MENU->index = 0;		//当前菜单信息不清零，返回时还在进入下一级的时的菜单
// 		NOW_MENU = NOW_MENU->last_menu;
// 		KeyEvent[ENTER].shortPress = false;
// 	}
// }

// void ABOUT_F()
// {
// 	DrawString(33,3,"stm32",2);
// 	DrawString(13,19,"G070CBT6",2);
// 	flash();
// }

// void ABUOT_return()
// {
// 	if(KeyEvent[ENTER].longPress){
// 		NOW_MENU = NOW_MENU->last_menu;
// 		KeyEvent[ENTER].longPress = false;
// 	}
// }

// void CHOOSE_TIPS_click()
// {
//     if(KeyEvent[ENTER].shortPress&&(NOW_MENU->index != 0)){
//         tips[CHOOSE_TIPS.index][1] = 219;
//         if(CHOOSE_TIPS.index != Iron.tips_index){
//             tips[Iron.tips_index][1] = ' ';
//             Iron.tips_index = CHOOSE_TIPS.index;
//         }
//         KeyEvent[ENTER].shortPress = false;
//     }
// }


// // /*2023-10-2	by zxcli
// //  *Code version: 1.2
// //  *多级菜单
// //  * menu结构体为一页菜单
// //  * list结构体为一页菜单里面的一个选项
// //  * 功能函数命名：
// //  * 				通用功能函数：currency_+大写
// //  * 				另外的功能函数：当前菜单名(大写)_当前选项名(小写)
// //  *
// //  * 结构体详情：
// // typedef struct
// // {
// //     char* str;					//显示内容
// //     void (*function_list)();	//选项功能函数
// //     bool selected;				//是否被选中
// // }list;

// // typedef struct	MENU
// // {
// //     uint16_t list_num;			//选项个数
// //     uint16_t index;				//菜单选项索引
// //     uint16_t offset_box;		//方框偏移量
// //     uint16_t offset_page;       //界面偏移量
// //     void (*function_menu)();	//当前菜单页功能函数，功能包括上下移动光标
// //     struct MENU *last_menu;		//上级菜单
// //     list lists[30];				//当前菜单里面的选项，动态数组形式，理论上可以存放无限个选项
// // }menu;
// //  */
// // #include "menu.h"
// // #define size 12
// // #define fonts_wide 6

// // menu SETTING =
// // {
// //     15,
// //     0,
// //     0,
// //     0,
// //     currency_MENU,
// //     none,
// //     {
// //         {"Double pulse test", SETTING_choose_tips, 0},      //双脉冲测试
// //         {"PWM with dead zone", SETTING_calibration, 0},     //PWM带死区
// //         {"CAN Control", none , 0},                //关于
// //         {"Display Settings", none,0},//test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //         {"test", none, 0},                          //test
// //     }
// // };

// // menu* NOW_MENU = &SETTING;

// // menu ABOUT=
// // {
// // 	1,
// // 	0,
// // 	0,
// //     0,
// // 	ABOUT_F,
// // 	&SETTING,
// // 	{
// // 		{"stm32g070cbt6",ABUOT_return,0},
// // 	}
// // };

// // menu CHOOSE_TIPS =
// // {
// //     2,
// //     1,
// //     0,
// //     0,
// //     currency_MENU,
// //     &SETTING,
// //     {
// //         {"Return", currency_RETURN, 0},
// //         {0, none, 0},
// //     }
// // };


// // void RUN_UI()
// // {
// //     NOW_MENU->function_menu();
// //     NOW_MENU->lists[NOW_MENU->index].function_list();
// // }

// // void SETTING_choose_tips()
// // {
// //     if (KeyEvent[ENTER].shortPress) {
// //         NOW_MENU = &CHOOSE_TIPS;
// //         NOW_MENU->offset_box = 0;
// //         NOW_MENU->index  = 0; // 下一级菜单进入时确保在第一个选项
// //         KeyEvent[ENTER].shortPress  = false;
// //     }
// // }

// // void SETTING_calibration()
// // {
	
// // }

// // /*
// //  *通用-菜单列表处理函数
// //  */
// // void currency_MENU()
// // {
// //     for(uint16_t i = 0;i < NOW_MENU->list_num;i++){
// //             DrawString(5, size * i + 4 - NOW_MENU->offset_page, NOW_MENU->lists[i].str, size >> 3);
// //     }
    
// // 	if(!NOW_MENU->lists[NOW_MENU->index].selected){		//当前选项未被选中执行时可以向上向下翻动
// // 		if(KeyEvent[LEFT].shortPress){                      //往上翻
// //             NOW_MENU->offset_box = ((NOW_MENU->offset_box - size) > 0) ? (NOW_MENU->offset_box - size) : 0;
// //             NOW_MENU->index = (NOW_MENU->offset_box/size);
// //             if((NOW_MENU->offset_box - NOW_MENU->offset_page + size)<=0){   //选项框到最顶上，让选项下移
// //                 NOW_MENU->offset_page -= size;
// //             }
// // 			KeyEvent[LEFT].shortPress = false;
// // 		}
// // 		if(KeyEvent[RIGHT].shortPress){                       //往下翻
// //             NOW_MENU->offset_box = ((NOW_MENU->offset_box + size) < (size * (NOW_MENU->list_num - 1))) ? (NOW_MENU->offset_box + size) : (size * (NOW_MENU->list_num - 1));
// //             NOW_MENU->index = (NOW_MENU->offset_box/size);
// //             if((NOW_MENU->offset_box - NOW_MENU->offset_page + size) > 64){ //选项框到最底下，让选项上移
// //                 NOW_MENU->offset_page += size;
// //             }
// //             KeyEvent[RIGHT].shortPress = false; 
// // 		}
// // 	}

// //     DrawMYfillRoundRect(2,(NOW_MENU->offset_box - NOW_MENU->offset_page + 1), fonts_wide * (strlen(NOW_MENU->lists[NOW_MENU->index].str)) + 5, size);
// //     DrawNum(100,30,NOW_MENU->index,2,1,1);

// // 	flash();
// // }

// // void none()
// // {
// // 	__NOP();
// // }



// // /*
// //  *通用-返回上一级菜单
// //  */
// // void currency_RETURN()	
// // {
// // 	if(KeyEvent[ENTER].shortPress){
// //         KeyEvent[LEFT].shortPress = false;
// //         KeyEvent[RIGHT].shortPress = false;
// //         // NOW_MENU->offset_box = 0;
// // 		// NOW_MENU->index = 0;		//当前菜单信息不清零，返回时还在进入下一级的时的菜单
// // 		NOW_MENU = NOW_MENU->last_menu;
// // 		KeyEvent[ENTER].shortPress = false;
// // 	}
// // }

// // void ABOUT_F()
// // {
// // 	DrawString(33,3,"stm32",2);
// // 	DrawString(13,19,"G070CBT6",2);
// // 	flash();
// // }

// // void ABUOT_return()
// // {
// // 	if(KeyEvent[ENTER].longPress){
// // 		NOW_MENU = NOW_MENU->last_menu;
// // 		KeyEvent[ENTER].longPress = false;
// // 	}
// // }

// // void CHOOSE_TIPS_click()
// // {
// // }