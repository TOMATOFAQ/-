#include "jobshop.h"
#include "graphics.h"

PIMAGE background1;
PIMAGE list;

PIMAGE background5;
PIMAGE list3;

void image_whole()
{
	int i,j; 
	int page=1;
	int checky=35;
	int checkx1=730;
	int checkx2=805;
	int checkx3=880;
	int checkx4=970;   //坐标记录变量 
    int count=0;   //计数变量 
	background1=newimage();
	getimage(background1,"gantt3.jpg",0,0);
	list=newimage();
	getimage(list,"list.jpg",0,0);
	//一些字体设置，和背景图片设置 
	putimage(0,0,background1);

	image_ganttwhole();
	image_ganttcheckwhole();

	setfont(24, 0, "微软雅黑");
	setfontbkcolor(EGERGB(255, 255, 255));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	outtextrect(725, 10, 1062, 691, "工件号    工序号    所需时间    指定机器");	
	for(i=0;i<n;i++)
    {
    	for(j=0;j<m;j++)
        {
        	switch(i+1){
				case 1:setfontbkcolor(ONE);break;
				case 2:setfontbkcolor(TWO);break;
				case 3:setfontbkcolor(THR);break;
				case 4:setfontbkcolor(FOR);break;
				case 5:setfontbkcolor(FIV);break;
				case 6:setfontbkcolor(SIX);break;
				case 7:setfontbkcolor(SEV);break;
				case 8:setfontbkcolor(EIG);break;
				case 9:setfontbkcolor(NIN);break;
				case 10:setfontbkcolor(TEN);break;
				case 11:setfontbkcolor(ELE);break;
				case 12:setfontbkcolor(TWE);break;
				case 13:setfontbkcolor(THI);break;
				case 14:setfontbkcolor(FOU);break;
				case 15:setfontbkcolor(FIF);break;
			}
			setcolor(EGERGB(0x0, 0x0, 0x0));
    	    xyprintf(checkx1,checky,"%d",i);
        	xyprintf(checkx2,checky,"%d",j);
            xyprintf(checkx3,checky,"%d",I[i][j][1]);
        	xyprintf(checkx4,checky,"%d",I[i][j][0]);
        	checky=checky+21; 
        	count++;
            if(count==6*m)
				{
					checky=35;
					checkx1=730;
					checkx2=805;
					checkx3=880;
					checkx4=970;
					break;
				}
		}
	if(count==6*m)
		{
			count=0;
			break;
		}
    }
	mouse_msg msg = {0};
	for ( ; is_run(); delay_fps(60))
    {
    	msg = getmouse();
		if(msg.is_down()) 
		{
			if(msg.x>=1069&&msg.x<=1105&&msg.y>=2&&msg.y<=49)  //往回翻一页 
			{
				page--;
        		image_wholepage(page);
			}
			if(msg.x>=1072&&msg.x<=1108&&msg.y>=665&&msg.y<=695)  //往下翻一页 
			{
       			page++;
       			image_wholepage(page);
			}
        	if(msg.x>=1121&&msg.x<=1244&&msg.y>=637&&msg.y<=692)  //点击继续按钮时退出此函数并进入下一环节 
        	{
				delimage(background1);
				putimage(0,0,list);
				xyprintf(0,0,"点我退出");
        		break; 
			}
		}
    }
}
void image_ganttcheckwhole()  //检修甘特图的输出
{
	int checkx,checky;
	int i,j;
	int before;
	
	setfontbkcolor(EGERGB(255, 255, 255));
	setcolor(EGERGB(103, 78, 167));
	setfont(15, 0, "微软雅黑");
	checkx=50; 
	checky=400;
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
			bar(checkx+result[i][j][2]*620/final_result, checky, checkx+result[i][j][3]*620/final_result, checky+19);
		}
		xyprintf(checkx+result[i][j-1][3]*620/final_result, checky+17, "|%d",result[i][j-1][3]);
		checky+=30;
	}
	setcolor(EGERGB(103, 78, 167));
	checkx=50; 
	checky=400;
	for(i = 0;i<n*m;i++){
		if(overhaul_list[i][0]>=0)
		{
			line(checkx+overhaul_list[i][1]*620/final_result, checky+30*overhaul_list[i][0], checkx+overhaul_list[i][1]*620/final_result, checky+20+30*overhaul_list[i][0]);
			line(checkx+overhaul_list[i][2]*620/final_result, checky+30*overhaul_list[i][0], checkx+overhaul_list[i][2]*620/final_result, checky+20+30*overhaul_list[i][0]);
			line(checkx+overhaul_list[i][1]*620/final_result, checky+30*overhaul_list[i][0], checkx+overhaul_list[i][2]*620/final_result, checky+30*overhaul_list[i][0]);
		}
	}
	//xyprintf(1000, 1000, "输出完成");
	setcolor(EGERGB(0, 0, 0));
	setfont(20, 0, "微软雅黑");
	checky=400;
	checkx=20; 
	setcolor(EGERGB(103, 78, 167));
	setfontbkcolor(EGERGB(255, 255, 255));
	for(i=0;i<m;i++)
	{
		xyprintf(checkx, checky, "M%d",i);
		line(checkx+30, checky+19, checkx+680, checky+19);
		checky+=30;
	}
}

void image_ganttwhole()  //甘特图的输出
{
	int checkx,checky;
	int i,j;
	int before=0;
	
	checkx=50; 
	checky=20;
	setfont(12, 0, "微软雅黑");
	for(i=0;i<m;i++){
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
			bar(checkx+ganttorignal[i][j][2]*620/final_result, checky, checkx+ganttorignal[i][j][3]*620/final_result, checky+24);
		}
		xyprintf(checkx+ganttorignal[i][j-1][3]*620/final_result, checky+24, "|%d",ganttorignal[i][j-1][3]);
		checky+=35;
	}
	
	setcolor(EGERGB(0, 0, 0));
	setfont(20, 0, "微软雅黑");
	checky=20;
	checkx=20; 
	setcolor(EGERGB(103, 78, 167));
	setfontbkcolor(EGERGB(255, 255, 255));
	for(i=0;i<m;i++)
	{
		xyprintf(checkx, checky, "M%d",i);
		line(checkx+30, checky+24, checkx+680, checky+24);
		checky+=35;
	}
}

void image_wholepage(int page) //订单显示页面翻页函数
{
	delimage(background1);
    background1=newimage();
	getimage(background1,"gantt3.jpg",0,0);
	putimage(0,0,background1);
	image_ganttwhole();
	image_ganttcheckwhole();
	int i,j; 
	int checky=35;
	int checkx1=730;
	int checkx2=805;
	int checkx3=880;
	int checkx4=970;   //坐标记录变量 
	setfontbkcolor(EGERGB(255, 255, 255));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	setfont(24, 0, "微软雅黑");
    outtextrect(725, 10, 1062, 691, "工件号    工序号    所需时间    指定机器");
    int count=0;   //计数变量 
	for(i=(page-1)*3;i<n;i++)
    {
    	for(j=0;j<m;j++)
        {
        	switch(i+1){
			case 1:setfontbkcolor(ONE);break;
			case 2:setfontbkcolor(TWO);break;
			case 3:setfontbkcolor(THR);break;
			case 4:setfontbkcolor(FOR);break;
			case 5:setfontbkcolor(FIV);break;
			case 6:setfontbkcolor(SIX);break;
			case 7:setfontbkcolor(SEV);break;
			case 8:setfontbkcolor(EIG);break;
			case 9:setfontbkcolor(NIN);break;
			case 10:setfontbkcolor(TEN);break;
			case 11:setfontbkcolor(ELE);break;
			case 12:setfontbkcolor(TWE);break;
			case 13:setfontbkcolor(THI);break;
			case 14:setfontbkcolor(FOU);break;
			case 15:setfontbkcolor(FIF);break;
			}
			setcolor(EGERGB(0x0, 0x0, 0x0));
        	xyprintf(checkx1,checky,"%d",i);
        	xyprintf(checkx2,checky,"%d",j);
            xyprintf(checkx3,checky,"%d",I[i][j][1]);
            xyprintf(checkx4,checky,"%d",I[i][j][0]);
        	checky=checky+21; 
        	count++;
           	if(count==6*m)
				{
					checky=35;
					checkx1=730;
					checkx2=805;
					checkx3=880;
					checkx4=970;
					break;
				}
		}
	if(count==6*m)
		{
			count=0;
			break;
		}
	}
}