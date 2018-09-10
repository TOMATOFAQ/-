#include "jobshop.h"
#include "graphics.h"
PIMAGE background2;
PIMAGE list0;
void image_checklistpage(int page) //订单显示页面翻页函数
{
	delimage(background2);
    background2=newimage();
	getimage(background2,"showlist.jpg",0,0);
	putimage(0,0,background2);
	list0=newimage();
	getimage(list0,"list.jpg",0,0);
	setfontbkcolor(EGERGB(255, 255, 255));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	outtextrect(20, 10, 1280, 60, "工件号    工序号    所需时间    指定机器\t工件号    工序号    所需时间    指定机器\t工件号    工序号    所需时间    指定机器");
	int i,j; 
	int checky=35;
	int checkx1=40;
	int checkx2=115;
	int checkx3=190;
	int checkx4=280;   //坐标记录变量 
    
    int count=0;   //计数变量 
	for(i=(page-1)*9;i<n;i++)
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
           	if(count==6*m||count==12*m)  //每列最多显示3*m行 
            {
           		checkx1+=353;
           		checkx2+=353;
            	checkx3+=353;
       	    	checkx4+=353;
           		checky=35;
			}	else if(count==18*m)
				{
					checky=35;
					checkx1=40;
					checkx2=115;
					checkx3=190;
					checkx4=280; 
					break;
				}
		}
		if(count==9*m)
		{
			count=0;
			break;
		}
	}
}

void image_checklist()   //订单的输出 
{
	background2=newimage();
	getimage (background2,"showlist.jpg",0,0);

	int i,j; 
	int page=1;
	int checky=35;
	int checkx1=40;
	int checkx2=115;
	int checkx3=190;
	int checkx4=280;   //坐标记录变量 
    
    int count=0;   //计数变量 

	//一些字体设置，和背景图片设置 
	putimage(0,0,background2);
	setfont(24, 0, "微软雅黑");
	setfontbkcolor(EGERGB(255, 255, 255));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	outtextrect(20, 10, 1280, 60, "工件号    工序号    所需时间    指定机器\t工件号    工序号    所需时间    指定机器\t工件号    工序号    所需时间    指定机器");	
	
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
            if(count==6*m||count==12*m)  //每列最多显示3*m行 
        	{
            	checkx1+=353;
            	checkx2+=353;
    	        checkx3+=353;
        	    checkx4+=353;
            	checky=35;
			}	else if(count==18*m)
				{
					checky=35;
					checkx1=40;
					checkx2=115;
					checkx3=190;
					checkx4=280; 
					break;
				}
		}
	if(count==18*m)
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
    		if(msg.x>=1139&&msg.x<=1259&&msg.y>=333&&msg.y<=399)  //点击继续按钮时退出此函数并进入下一环节 
			{
				delimage(background2);
				putimage(0,0,list0);
				xyprintf(0,0,"点我退出");
       			break; 
			}
			if(msg.x>=1063&&msg.x<=1107&&msg.y>=15&&msg.y<=59)  //往回翻一页 
			{
				page--;
        		image_checklistpage(page);
			}
			if(msg.x>=1065&&msg.x<=1112&&msg.y>=652&&msg.y<=694)  //往下翻一页 
			{
       			page++;
       			image_checklistpage(page);
			}
		}
	}
} 

void image_ganttorignal()  //原始甘特图的输出
{
	background2=newimage();
	getimage (background2,"gantt1.jpg",0,0);
	list0=newimage();
	getimage(list0,"list.jpg",0,0);
	int checkx,checky;
	int i,j;
	int before;
	
	putimage(0,0,background2);
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
			if(ganttorignal[i][j][2]-before<=20)
			{
				xyprintf(checkx+ganttorignal[i][j][2]*1040/final_result, checky+23, "|%d",ganttorignal[i][j][2]);
			}else
			{
				xyprintf(checkx+ganttorignal[i][j][2]*1040/final_result, checky-13, "|%d",ganttorignal[i][j][2]);

			}
			bar(checkx+ganttorignal[i][j][2]*1040/final_result, checky, checkx+ganttorignal[i][j][3]*1040/final_result, checky+25);
			
			before=ganttorignal[i][j][2];
		}
		xyprintf(checkx+ganttorignal[i][j-1][3]*1040/final_result, checky+23, "|%d",ganttorignal[i][j-1][3]);
		checky+=65;
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
	xyprintf(checkx+100, checky-30, "程序运行时间:%lf",duraorignal);
	xyprintf(checkx+500, checky-30, "结束时间:%d",finalorignal);
	
	mouse_msg msg = {0};
    for ( ; is_run(); delay_fps(60))
    {
        msg = getmouse();
        if(msg.is_down()) 
        {
        	if(msg.x>=1139&&msg.x<=1264&&msg.y>=341&&msg.y<=412)  //点击继续按钮时退出此函数并进入下一环节 
        	{
        		delimage(background2);
				putimage(0,0,list0);
				xyprintf(0,0,"点我退出");
        		break; 
			}
		}
    }
}