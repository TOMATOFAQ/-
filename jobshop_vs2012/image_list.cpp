#include "jobshop.h"
#include "graphics.h"

PIMAGE background3;
PIMAGE background4;
PIMAGE list1;

void image_list()  //菜单函数
{
	PIMAGE list;
	list=newimage();
	getimage(list,"list.jpg",0,0);
	putimage(0,0,list);
	xyprintf(0,0,"点我退出");
	mouse_msg msg = {0};
	for ( ; is_run(); delay_fps(60))
    {
    	msg = getmouse();
		if(msg.is_down()) 
		{
    		if(msg.x>=291&&msg.x<=595&&msg.y>=146&&msg.y<=332)  //订单查看 
			{
				image_checklist(); 
			}
			if(msg.x>=653&&msg.x<=955&&msg.y>=147&&msg.y<=326)  //Gantt out 
			{
				image_ganttorignal();
			}
			if(msg.x>=653&&msg.x<=955&&msg.y>=385&&msg.y<=564)  //总览 
			{
				image_whole();
			}
			if(msg.x>=290&&msg.x<=595&&msg.y>=385&&msg.y<=564)  //检修
			{
				background4=newimage();
				getimage (background4,"waiting2.jpg",0,0);
				putimage(0,0,background4);
				Sleep(1000);
				overhaulrun();
				delimage(background4);
				image_ganttoverhaul();
			}
			if(msg.x>=0&&msg.x<=80&&msg.y>=0&&msg.y<=20)  //退出键
			{
				break;
			}
		}
	}
}

void image_ganttoverhaul()  //检修甘特图的输出
{
	background3=newimage();
	getimage (background3,"gantt1.jpg",0,0);
	list1=newimage();
	getimage(list1,"list.jpg",0,0);
	int checkx,checky;
	int i,j;
	int before;
	
	putimage(0,0,background3);
	setfontbkcolor(EGERGB(255, 255, 255));
	setcolor(EGERGB(103, 78, 167));
	setfont(15, 0, "微软雅黑");
	checkx=50; 
	checky=35;
	for(i=0;i<m;i++){
		before=0; 
		for(j=0;j<n;j++){
			switch(result[i][j][0]+1){
				case 1:setfillcolor(ONE);break;
				case 2:setfillcolor(TWO);break;
				case 3:setfillcolor(THR);break;
				case 4:setfillcolor(FOR);break;
				case 5:setfillcolor(FIV);break;
				case 6:setfillcolor(SIX);break;
				case 7:setfillcolor(SEV);break;
				case 8:setfillcolor(EIG);break;
				case 9:setfillcolor(NIN);break;
				case 10:setfillcolor(TEN);break;
				case 11:setfillcolor(ELE);break;
				case 12:setfillcolor(TWE);break;
				case 13:setfillcolor(THI);break;
				case 14:setfillcolor(FOU);break;
				case 15:setfillcolor(FIF);break;
			}
			if(result[i][j][2]-before<=20)
			{
				xyprintf(checkx+result[i][j][2]*1040/final_result, checky+23, "|%d",result[i][j][2]);
			}else
			{
				xyprintf(checkx+result[i][j][2]*1040/final_result, checky-13, "|%d",result[i][j][2]);
			}
			
			bar(checkx+result[i][j][2]*1040/final_result, checky, checkx+result[i][j][3]*1040/final_result, checky+25);

			before=result[i][j][2];
		}
		xyprintf(checkx+result[i][j-1][3]*1040/final_result, checky+23, "|%d",result[i][j-1][3]);
		checky+=65;
	}
	setcolor(EGERGB(103, 78, 167));
	checkx=50; 
	checky=35;
	for(i = 0;i<n*m;i++){
		if(overhaul_list[i][0]>=0)
		{
			line(checkx+overhaul_list[i][1]*1040/final_result, checky+65*overhaul_list[i][0], checkx+overhaul_list[i][1]*1040/final_result, checky+25+65*overhaul_list[i][0]);
			line(checkx+overhaul_list[i][2]*1040/final_result, checky+65*overhaul_list[i][0], checkx+overhaul_list[i][2]*1040/final_result, checky+25+65*overhaul_list[i][0]);
			line(checkx+overhaul_list[i][1]*1040/final_result, checky+65*overhaul_list[i][0], checkx+overhaul_list[i][2]*1040/final_result, checky+65*overhaul_list[i][0]);
			xyprintf(checkx+overhaul_list[i][1]*1040/final_result, checky+23+65*overhaul_list[i][0], "|%d",overhaul_list[i][1]);
			xyprintf(checkx+overhaul_list[i][2]*1040/final_result, checky+23+65*overhaul_list[i][0], "|%d",overhaul_list[i][2]);
		}
	}
	//xyprintf(1000, 1000, "输出完成");
	setcolor(EGERGB(0, 0, 0));
	setfont(20, 0, "微软雅黑");
	checky=35;
	checkx=20; 
	setcolor(EGERGB(103, 78, 167));
	setfontbkcolor(EGERGB(255, 255, 255));
	for(i=0;i<m;i++)
	{
		xyprintf(checkx, checky, "M%d",i);
		line(checkx+30, checky+24, checkx+1080, checky+24);
		checky+=65;
	}
	setfont(24, 0, "微软雅黑");
	xyprintf(checkx+100, checky-30, "程序运行时间:%lf",duration);
	xyprintf(checkx+500, checky-30, "结束时间:%d",final_result);
	
	mouse_msg msg = {0};
    for ( ; is_run(); delay_fps(60))
    {
        msg = getmouse();
        if(msg.is_down()) 
        {
        	if(msg.x>=1139&&msg.x<=1264&&msg.y>=341&&msg.y<=412)  //点击继续按钮时退出此函数并进入下一环节 
        	{
        		delimage(background3);
				putimage(0,0,list1);
				xyprintf(0,0,"点我退出");
        		break; 
			}
		}
    }
}