/*sleep  time �n�վ�A���ઽ���Ospeed ---  �Ȯɥ��o��= = */
/*�D�ʲ��ʦ欰  done */
/*���n�H�u��m�}��  �]���S�ݷ|���B�z�ܪ�����*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<math.h>
#define xx 28
#define yy 28
int length,low_speed_limit,testing=0,point;
void gotoxy(int xpos, int ypos);
void pause(void);
int waterfilling(int *FindPath,int from_x,int from_y,int to_x,int to_y,int *pmap);
void setdir(int headdir,int *pmapdir,int *pdirx,int *pdiry);
void move(int *pheadx,int *pheady,int *ptailx,int *ptaily,int dirx,int diry,int *pmap,int *pi_setfood);
void setfood(int *pfoodx,int *pfoody,int *pmap,int *pi_setfood,int *pfoodpx,int *pfoodpy);
/*�����}���J�a�Ϥj�p*/
/*���٬O�Ϋ��жǻ��}�C*/
/*mode[] {0,1}={snake-control  food-rand   , snake AI and food-rand}*/
/* SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a); */
/*a=2  �`��     a=10 �G��     a=14 �G��     a=7 ���`*/
main()
{
  SetConsoleTitle("Mode : Normal");
  srand(time(NULL));
  int FindPath[xx][yy],map[xx][yy],re[5],t,tt,authordisplay;
  int a,b,snake=0,movepermit,check[5],chk2,test,helpx,helpy,bling,blingcount;
  int from_x,from_y;
  int to_x,to_y;
  int headx,heady,mode,foodx,foody,tailx,taily,roomjudge;
  int dirx,diry,speed,i_setfood,foodpx,foodpy;
  char ch[2];
  int speedlist[]={250,167,125,100,83,71,63,56,50,45,42,38,36,33,31,29,28,26,25,10};
  
  
  headx=xx-1;  heady=2;  tailx=xx-1;  taily=0;
  gotoxy(0,0);
  for(a=0;a<xx+2+10;a++)
    printf("��");
  //printf("\n");
  for(a=0;a<xx;a++)
  {
	printf("��");
	for(b=0;b<yy;b++)
    {
	  FindPath[a][b]=map[a][b]=0;
	  printf("�@");
	}
	printf("���@�@�@�@�@�@�@�@�@��");
  }
  for(a=0;a<xx+2+10;a++)
    printf("��");
  FindPath[xx-1][0]=FindPath[xx-1][1]=-1;  map[xx-1][0]=map[xx-1][1]=26;
  
  foodx=foody=foodpx=foodpy=-1;  mode=0;  dirx=0;diry=1;  speed=0;  i_setfood=1;
  movepermit=1;  chk2=0;  length=3;  bling=0;  blingcount=0;  roomjudge=3;
  t=0;  tt=0;  authordisplay=0;  low_speed_limit=0;  point=0;
  gotoxy(0+2,xx-1+1);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
  printf("����");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
  printf("��");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
  gotoxy(2*yy+4,2);
  printf("�@�g�Y�D");
  gotoxy(2*yy+4,3);
  printf("�@�@ Snake_III_2");
  gotoxy(2*yy+4,5);
  printf("�@�@   By pi314 ");
  gotoxy(2*yy+4,7);
  printf("������������������");
  gotoxy(2*yy+4,9);
  printf("�@�@�C���{�p");
  gotoxy(2*yy+4,11);
  printf("�@����:%4d",length);
  gotoxy(2*yy+4,12);
  printf("�@�t��:%2d/19",speed+1);
  gotoxy(2*yy+4,13);
  printf("�@����:%4d",point);
  gotoxy(2*yy+4,15);
  printf("������������������");
  gotoxy(2*yy+4,17);
  printf("�@�@�C���ާ@");
  gotoxy(2*yy+4,19);
  printf("�@��������");
  gotoxy(2*yy+4,20);
  printf("�@�@����D��");
  gotoxy(2*yy+4,21);
  printf("�@�@�@��i��V");
  gotoxy(2*yy+4,23);
  printf("�@+ -");
  gotoxy(2*yy+4,24);
  printf("�@�@�[�t�M��t");
  gotoxy(2*yy+4,26);
  printf("�@Esc");
  gotoxy(2*yy+4,27);
  printf("�@�@�@AI�Ҧ�~");
  gotoxy(20,13);
  printf("�Ы����N��}�l�C��");
  ch[0] = getch();
  if(ch[0]==-32)
  {
    ch[1] = getch();
  }
  else if(ch[0]==27)
  {
    testing = 1;
  }
  gotoxy(20,13);
  printf("�@�@�@�@�@�@�@   ");
  while(1)
  {
	Sleep(speedlist[speed]);
	blingcount+=speedlist[speed];
	if(speed<low_speed_limit)
	{
	  speed++;
	  gotoxy(2*yy+4,12);
      printf("�@�t��:%2d/19",speed+1);
	}
	if(blingcount>=300)
	{
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
	  gotoxy(foody*2+2,foodx+1);
	  if(bling==0)
	    printf("��");
	  else
	    printf("��");
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	  blingcount=0;
	  bling=(bling+1)%2;
	}
    if(kbhit()!=0)
    {
	  ch[0]=getch();
	  if(ch[0]==-32||ch[0]==0)
	    ch[1]=getch();
	  if(ch[0]==27)/*Esc --mode changing*/
	  {
	    mode=(mode+1)%2;
	    if(mode==0)
	    {
		  SetConsoleTitle("Mode : Normal");
  		  gotoxy(2*yy+4,27);
  		  printf("�@�@�@AI�Ҧ�~");
		}
	    else if(mode==1)
	    {
		  SetConsoleTitle("Mode : Auto");
  		  gotoxy(2*yy+4,27);
  		  printf("�@�@���q�Ҧ�~");
		}
	  }
	  else if(ch[0]==-32)/*��V��*/
	  { 
  	    if(mode==0)
		  setdir(ch[1],&map[headx][heady],&dirx,&diry);
	  }
	  else if(ch[0]==43)/* + speed up*/
	  { 
		if(speed<18)
		  speed++;
		gotoxy(2*yy+4,12);
        printf("�@�t��:%2d/19",speed+1);
	  }
	  else if(ch[0]==45)/* - slow down*/
	  {
		if(speed>low_speed_limit)
		  speed--;
		gotoxy(2*yy+4,12);
  		printf("�@�t��:%2d/19",speed+1);
	  }
	  else if(ch[0]==32)/*Space*/
	    movepermit=(movepermit+1)%2;
	  else if(ch[0]==0&&ch[1]==59)/*F1*/
	  {
		snake=(snake+1)%3;
		if(snake==0)
		{
		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		  gotoxy(0,0+1);
		  for(a=0;a<xx;a++)
		  {
			printf("��");
		    for(b=0;b<yy;b++)
		    {
			  if(map[a][b]>20)
			  {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
			    printf("��");
			    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
			  }
			  else
			    printf("�@");
			}
		    printf("\n");
		  }
		  gotoxy(2*foody+2,foodx+1);
		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
		  printf("��");
		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		}
	  }
	  else if(ch[0]==96||ch[0]==126)
	  {
		authordisplay=(authordisplay+1)%2;
		if(authordisplay==0)
		{
		  gotoxy(2*yy+4,5);
          printf("�@�@   By pi314 ");
		}
	    else
	    {
		  gotoxy(2*yy+4,5);
          printf("�@�@   By ������");
		}
	  }
	}
	if(i_setfood==1)
	{
	  do
	  {
	    foodx=rand()%xx;
	    foody=rand()%yy;
	  }while(map[foodx][foody]!=0);
	}
	if(i_setfood==1)
	  setfood(&foodx,&foody,&map[0][0],&i_setfood,&foodpx,&foodpy);
	if(mode==1)
    {
	  /*check[]={�W,�U,��,�k}*/  /*����W�U���k���i���*/
	  /*-2 ���藍�ਫ      -1 �i�H    n �U��*/
	  if(length>=((xx*yy)/18)*3)
	    roomjudge=1;
	  else if(length>=(xx*yy)/9)
	    roomjudge=2;
	  for(a=0;a<4;a++)
	    check[a]=-1;
	  if(headx>0)
	  {
	    for(a=0;a<xx;a++)
	      for(b=0;b<yy;b++)
	      {
		    if(map[a][b]!=0&&map[a][b]!=1)
		      FindPath[a][b]=-1;
		    else
  		      FindPath[a][b]=0;
		  }
	    if(FindPath[headx-1][heady]==-1)
	      check[0]=-2;
	    else
	    {
		  FindPath[headx][heady]=-1;
		  waterfilling(&FindPath[0][0],headx-1,heady,-1,-1,&map[0][0]);
	      test=0;
	      for(a=0;a<xx;a++)
	        for(b=0;b<yy;b++)
	          if(FindPath[a][b]>0)
		        test++;
	      if(test<length*roomjudge)
	        check[0]=test;
		}
	  }
	  else
	    check[0]=-2;
	  if(headx<xx-1)
	  {
	    for(a=0;a<xx;a++)
	      for(b=0;b<yy;b++)
	      {
		    if(map[a][b]!=0&&map[a][b]!=1)
		      FindPath[a][b]=-1;
		    else
  		      FindPath[a][b]=0;
		  }
	    if(FindPath[headx+1][heady]==-1)
	      check[1]=-2;
	    else
	    {
		  FindPath[headx][heady]=-1;
		  waterfilling(&FindPath[0][0],headx+1,heady,-1,-1,&map[0][0]);
	      test=0;
	      for(a=0;a<xx;a++)
	        for(b=0;b<yy;b++)
	          if(FindPath[a][b]>0)
		        test++;
	      if(test<length*roomjudge)
	        check[1]=test;
		}
	  }
	  else
	    check[1]=-2;
	  
	  if(heady>0)
	  {
	    for(a=0;a<xx;a++)
	      for(b=0;b<yy;b++)
	      {
		    if(map[a][b]!=0&&map[a][b]!=1)
		      FindPath[a][b]=-1;
		    else
  		      FindPath[a][b]=0;
		  }
	    if(FindPath[headx][heady-1]==-1)
	      check[2]=-2;
	    else
	    {
		  FindPath[headx][heady]=-1;
		  waterfilling(&FindPath[0][0],headx,heady-1,-1,-1,&map[0][0]);
	      test=0;
	      for(a=0;a<xx;a++)
	        for(b=0;b<yy;b++)
	          if(FindPath[a][b]>0)
		        test++;
	      if(test<length*roomjudge)
	        check[2]= test;
		}
	  }
	  else
	    check[2]=-2;
	  
	  if(heady<yy-1)
	  {
	    for(a=0;a<xx;a++)
	      for(b=0;b<yy;b++)
	      {
		    if(map[a][b]!=0&&map[a][b]!=1)
		      FindPath[a][b]=-1;
		    else
  		      FindPath[a][b]=0;
		  }
	    if(FindPath[headx][heady+1]==-1)
	      check[3]=-2;
	    else
	    {
		  FindPath[headx][heady]=-1;
		  waterfilling(&FindPath[0][0],headx,heady+1,-1,-1,&map[0][0]);
	      test=0;
	      for(a=0;a<xx;a++)
	        for(b=0;b<yy;b++)
	          if(FindPath[a][b]>0)
		        test++;
	      if(test<length*roomjudge)
	        check[3]= test;
		}
	  }
	  else
	    check[3]=-2;
	  
	  test=0;
	  for(a=0;a<4;a++)
	    if(check[a]==-1)/*-1  �i�H��*/
	      test++;
	  
	  if(testing)
	  {
	    gotoxy(0,0);
	    printf("�W:%-7d",check[0]);
	    printf("�U:%-7d",check[1]);
	    printf("��:%-7d",check[2]);
	    printf("�k:%-7d",check[3]);
	  }/*****************���լq*****************/
	  if(test==0)/*�|�V���U���Τ��q�A��̼e������V*/
	  {
	    for(a=0;a<4;a++)
	      if(check[a]>check[test]&&check[a]!=-2)
		    test=a;
	    check[test]=-1;
	    for(a=0;a<4;a++)
	      if(check[a]!=-1)
	        check[a]=-2;/*��L����V�������Ҽ{*/
	  }
	  /**/
	  for(a=0;a<xx;a++)
	    for(b=0;b<yy;b++)
	    {
	 	  if(a==headx&&b==heady)
		    FindPath[a][b]=0;
		  else if(map[a][b]!=0&&map[a][b]!=1)
		    FindPath[a][b]=-1;
		  else
  		    FindPath[a][b]=0;
		}
	  test=waterfilling(&FindPath[0][0],foodpx,foodpy,headx,heady,&map[0][0]);
	  if(test==1)
	   ;
	  else 
	    test=-5;
	  if(headx>0&&FindPath[headx-1][heady]<FindPath[headx][heady]&&check[0]!=-1&&FindPath[headx-1][heady]>0)
	  {
		test=-5;
	  }
	  if(headx<xx-1&&FindPath[headx+1][heady]<FindPath[headx][heady]&&check[1]!=-1&&FindPath[headx+1][heady]>0)
	  {
   	    test=-5;
	  }
      if(heady>0&&FindPath[headx][heady-1]<FindPath[headx][heady]&&check[2]!=-1&&FindPath[headx][heady-1]>0)
	  {
   	    test=-5;
	  }
	  if(heady<yy-1&&FindPath[headx][heady+1]<FindPath[headx][heady]&&check[3]!=-1&&FindPath[headx][heady+1]>0)
	  {
   	    test=-5;
	  }
	  
	  if(test==-5)
	  {
		for(a=0;a<xx;a++)
	       for(b=0;b<yy;b++)
	       {
		     if(a==headx&&b==heady)
		       FindPath[a][b]=0;
		     else if(map[a][b]!=0&&map[a][b]!=1)
		       FindPath[a][b]=-1;
		     else
  		       FindPath[a][b]=0;
		   }
		test=waterfilling(&FindPath[0][0],0,0,headx,heady,&map[0][0]);
		if(headx>0&&FindPath[headx-1][heady]<FindPath[headx][heady]&&check[0]!=-1&&FindPath[headx-1][heady]>0)
	    {
		  test=-5;
	    }
	    if(headx<xx-1&&FindPath[headx+1][heady]<FindPath[headx][heady]&&check[1]!=-1&&FindPath[headx+1][heady]>0)
	    {
   	      test=-5;
	    }
        if(heady>0&&FindPath[headx][heady-1]<FindPath[headx][heady]&&check[2]!=-1&&FindPath[headx][heady-1]>0)
	    {
   	      test=-5;
	    }
	    if(heady<yy-1&&FindPath[headx][heady+1]<FindPath[headx][heady]&&check[3]!=-1&&FindPath[headx][heady+1]>0)
	    {
   	      test=-5;
	    }
	    if(test==1)
	      ;
	    else
	    {
	      for(a=0;a<xx;a++)
	        for(b=0;b<yy;b++)
	        {
		      if(a==headx&&b==heady)
		        FindPath[a][b]=0;
		      else if(map[a][b]!=0&&map[a][b]!=1)
		        FindPath[a][b]=-1;
		      else
  		        FindPath[a][b]=0;
		    }
	      test=waterfilling(&FindPath[0][0],xx-1,0,headx,heady,&map[0][0]);
	      if(headx>0&&FindPath[headx-1][heady]<FindPath[headx][heady]&&check[0]!=-1&&FindPath[headx-1][heady]>0)
	      {
		    test=-5;
	      }
	      if(headx<xx-1&&FindPath[headx+1][heady]<FindPath[headx][heady]&&check[1]!=-1&&FindPath[headx+1][heady]>0)
	      {
   	        test=-5;
	      }
          if(heady>0&&FindPath[headx][heady-1]<FindPath[headx][heady]&&check[2]!=-1&&FindPath[headx][heady-1]>0)
	      {
   	        test=-5;
	      }
	      if(heady<yy-1&&FindPath[headx][heady+1]<FindPath[headx][heady]&&check[3]!=-1&&FindPath[headx][heady+1]>0)
	      {
   	        test=-5;
	      }
	      if(test==1)
	        ;
		  else
		  {
			for(a=0;a<xx;a++)
			  for(b=0;b<yy;b++)
			  {
				if(a==headx&&b==heady)
				  FindPath[a][b]=0;
				else if(map[a][b]!=0&&map[a][b]!=1)
				  FindPath[a][b]=-1;
				else
		  		  FindPath[a][b]=0;
			  }
			test=waterfilling(&FindPath[0][0],0,yy-1,headx,heady,&map[0][0]);
		    if(test==1)
		      ;
			else
			{
			  for(a=0;a<xx;a++)
			    for(b=0;b<yy;b++)
			    {
			      if(a==headx&&b==heady)
			        FindPath[a][b]=0;
			      else if(map[a][b]!=0&&map[a][b]!=1)
			        FindPath[a][b]=-1;
			      else
			        FindPath[a][b]=0;
			    }
			  test=waterfilling(&FindPath[0][0],xx-1,yy-1,headx,heady,&map[0][0]);
			  if(headx>0&&FindPath[headx-1][heady]<FindPath[headx][heady]&&check[0]!=-1&&FindPath[headx-1][heady]>0)
	          {
		        test=-5;
	          }
	          if(headx<xx-1&&FindPath[headx+1][heady]<FindPath[headx][heady]&&check[1]!=-1&&FindPath[headx+1][heady]>0)
	          {
   	            test=-5;
	          }
              if(heady>0&&FindPath[headx][heady-1]<FindPath[headx][heady]&&check[2]!=-1&&FindPath[headx][heady-1]>0)
	          {
   	            test=-5;
	          }
	          if(heady<yy-1&&FindPath[headx][heady+1]<FindPath[headx][heady]&&check[3]!=-1&&FindPath[headx][heady+1]>0)
	          {
   	            test=-5;
	          }
			  if(test==1)
			    ;
			  else
			  {
			    for(a=0;a<xx;a++)
			      for(b=0;b<yy;b++)
			      {
			        if(a==headx&&b==heady)
			          FindPath[a][b]=0;
			        else if(map[a][b]!=0&&map[a][b]!=1)
			          FindPath[a][b]=-1;
			        else
			          FindPath[a][b]=0;
			      }
			    test=waterfilling(&FindPath[0][0],xx/2,yy/2,headx,heady,&map[0][0]);
			    /*
			    if(headx>0&&FindPath[headx-1][heady]<FindPath[headx][heady]&&check[0]!=1&&FindPath[headx-1][heady]>0)
	            {
		          test=-5;
	            }
	            if(headx<xx-1&&FindPath[headx+1][heady]<FindPath[headx][heady]&&check[1]!=1&&FindPath[headx+1][heady]>0)
	            {
   	              test=-5;
	            }
                if(heady>0&&FindPath[headx][heady-1]<FindPath[headx][heady]&&check[2]!=1&&FindPath[headx][heady-1]>0)
	            {
   	              test=-5;
	            }
	            if(heady<yy-1&&FindPath[headx][heady+1]<FindPath[headx][heady]&&check[3]!=1&&FindPath[headx][heady+1]>0)
	            {
   	              test=-5;
	            }
			    if(test==1)
			      ;
			    else
			    {
				    for(a=0;a<xx;a++)
			          for(b=0;b<yy;b++)
			          {
			            if(a==headx&&b==heady)
			              FindPath[a][b]=0;
			            else if(map[a][b]!=0&&map[a][b]!=1)
			              FindPath[a][b]=-1;
			            else
			              FindPath[a][b]=0;
			          }
			        waterfilling(&FindPath[0][0],headx,heady,-1,-1,&map[0][0]);
	                int max=0;
			        for(a=0;a<xx;a++)
			          for(b=0;b<yy;b++)
			            if(FindPath[a][b]>max)
			            {
						  helpx=a;
						  helpy=b;
						  max=FindPath[a][b];
					    }
				    for(a=0;a<xx;a++)
			          for(b=0;b<yy;b++)
			          {
			     	    if(a==headx&&b==heady)
			              FindPath[a][b]=0;
			       	    else if(map[a][b]!=0&&map[a][b]!=1)
			              FindPath[a][b]=-1;
			       	    else
			              FindPath[a][b]=0;
			      	  }
				    waterfilling(&FindPath[0][0],helpx,helpy,headx,heady,&map[0][0]);
				}/**/
			  }
			}
		  }
		}
	  }
	  /*
	  gotoxy(0+2,xx+1);
	  printf("%d",test);/**/
	  chk2=0;
	  if(FindPath[headx-1][heady]<FindPath[headx][heady]&&FindPath[headx-1][heady]!=0&&FindPath[headx-1][heady]!=-1&&headx>0&&check[0]==-1/**/)
	  {
		ch[1]=72;
		chk2=2;
		if(dirx==-1&&diry==0&&movepermit)
		{
		  setdir(ch[1],&map[headx][heady],&dirx,&diry);
		  chk2=1;
		}
	  }
	  if(FindPath[headx+1][heady]<FindPath[headx][heady]&&FindPath[headx+1][heady]!=0&&FindPath[headx+1][heady]!=-1&&headx<xx-1&&chk2!=1&&check[1]==-1/**/)
	  {
		ch[1]=80;
		chk2=2;
		if(dirx==1&&diry==0&&movepermit)
		{
		  setdir(ch[1],&map[headx][heady],&dirx,&diry);
		  chk2=1;
		}
	  }
	  if(FindPath[headx][heady+1]<FindPath[headx][heady]&&FindPath[headx][heady+1]!=0&&FindPath[headx][heady+1]!=-1&&heady<yy-1&&chk2!=1&&check[3]==-1/**/)
	  {
		ch[1]=77;
		chk2=2;
		if(dirx==0&&diry==1&&movepermit)
		{
		  setdir(ch[1],&map[headx][heady],&dirx,&diry);
		  chk2=1;
		}
	  }
	  if(FindPath[headx][heady-1]<FindPath[headx][heady]&&FindPath[headx][heady-1]!=0&&FindPath[headx][heady-1]!=-1&&heady>0&&chk2!=1&&check[2]==-1/**/)
	  {
		ch[1]=75;
		chk2=2;
		if(dirx==0&&diry==-1&&movepermit)
		{
		  setdir(ch[1],&map[headx][heady],&dirx,&diry);
		  chk2=1;
		}
	  }
	  if(chk2!=1&&movepermit)/*�S���ߧY���u�����u�A�O����V����*/
 	  {/*�e����chk2=2�O���F�קK�v�T��쥻���ʧ@*/
 	    if(dirx!=0)
 	    {
		  if(heady>0&&(map[headx][heady-1]==0||map[headx][heady-1]==1)&&re[(t+3)%5]==77)
		  {
			ch[1]=75;
		    setdir(ch[1],&map[headx][heady],&dirx,&diry);
		    test=5;
		  }
		  else if(heady<yy-1&&(map[headx][heady+1]==0||map[headx][heady+1]==1)&&chk2==0&&re[(t+3)%5]==75)
		  {
		    ch[1]=77;
			setdir(ch[1],&map[headx][heady],&dirx,&diry);
			test=5;
		  }
		}
		else if(diry!=0)
		{
		  if(headx>0&&(map[headx-1][heady]==0||map[headx-1][heady]==1)&&re[(t+3)%5]==80)
		  {
		    ch[1]=72;
			setdir(ch[1],&map[headx][heady],&dirx,&diry);
			test=5;
		  }
		  else if(headx<xx-1&&(map[headx+1][heady]==0||map[headx+1][heady]==1)&&chk2==0&&re[(t+3)%5]==72)
		  {
		    ch[1]=80;
			setdir(ch[1],&map[headx][heady],&dirx,&diry);
			test=5;
		  }
		}
		if(test!=5)
		  test=0;
		if(map[headx+dirx][heady+diry]!=1&&map[headx+dirx][heady+diry]!=0&&test!=5)
		  test=1;
		if(headx+dirx<0||headx+dirx>=xx||heady+diry<0||heady+diry>=yy&&test!=5)
		  test=1;
		if(test==1&&chk2==0)/*�����|����*/
		{
		  tt=1;
		  if(dirx!=0)
		  {
		    if(heady>0&&(map[headx][heady-1]==0||map[headx][heady-1]==1))
		    {
			  ch[1]=75;
			}
			if(heady<yy-1&&(map[headx][heady+1]==0||map[headx][heady+1]==1)&&chk2==0)
			{
			  ch[1]=77;
			}
		  }
		  else if(diry!=0)
		  {
		    if(headx>0&&(map[headx-1][heady]==0||map[headx-1][heady]==1))
		    {
			  ch[1]=72;
			}
			if(headx<xx-1&&(map[headx+1][heady]==0||map[headx+1][heady]==1)&&chk2==0)
			{
			  ch[1]=80;
			}
		  }
		}
	    setdir(ch[1],&map[headx][heady],&dirx,&diry);
	  }
	}
	/*
	if(headx+dirx<0||headx+dirx>=xx||heady+diry<0||heady+diry>=yy||map[headx+dirx][heady+diry]>20)
      movepermit=0;/**/
	if(movepermit==1)
 	{
	  move(&headx,&heady,&tailx,&taily,dirx,diry,&map[0][0],&i_setfood);
	  if(dirx==-1)
	    re[t]=72;
	  else if(dirx==1)
	    re[t]=80;
	  else if(diry==-1)
	    re[t]=75;
	  else if(diry==1)
	    re[t]=77;
	  if(tt==1)
	  {
	    re[t]*=-1;
	    tt=0;
	  }
	  t=(t+1)%5;
	  /*
	  gotoxy(0,0);
	  for(a=0;a<5;a++)
	    printf("%d  ",re[(t+a)%5]);/**/
	}
	
	if(snake==1)
	{
	 gotoxy(0,0+1);
	 for(a=0;a<xx;a++)
	 {
	   printf("��");
	   for(b=0;b<yy;b++)
	   {
		 if(FindPath[a][b]==-1)
		 {
		   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),32);
		   printf("%2d",map[a][b]);
		   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		 }
		 else
	     {
		   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
		   printf("%2d",FindPath[a][b]);
		   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		 }
	   }
	   printf("\n");
	 }
	}
	else if(snake==2)
	{
	 gotoxy(0,0+1);
	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
	 for(a=0;a<xx;a++)
	 {
	   printf("��");
	   for(b=0;b<yy;b++)
	     printf("%2d",map[a][b]);
	   printf("\n");
	 }
	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	}
	/*||map[headx][heady]>20*/
	if(headx<0||headx>=xx||heady<0||heady>=yy||map[headx][heady]>20)
      if(testing==1)
	    movepermit=0;
	  else
	    break;
  }/*game's main loop end*/
  gotoxy(0,xx+2+2);
  printf("�Ы� Space �����C�� >>\n\n");
  do
  {
    ch[0]=getch();
  }while(ch[0]!=32);
  printf("�z�w�w11/21 �w�w�w�w�w�w�w�w�w�w�{\n");
  printf("�x Snake_III_0.exe  �g�Y�D 3.0  �x\n");
  printf("�|�s�w�w�w�w�w�w�w�w�w�w�w�w�w�w�r�w�w�w�w�w�w�w�w�w�w�{\n");
  printf("  �xhttp://www.cs.nctu.edu.tw/~cychih/Snake_III_0.exe �x\n");
  printf("  �|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n");
  printf("�S�O�P��: �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n");
  printf("�x2010/11/22 ex860��X���U��i�Ϥ�V�|�������`��bug.  �x\n");
  printf("�x--> �קאּSnake_III_1.exe                           �x\n");
  printf("�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n\n");
  printf("�S�O�P�¡G�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n");
  printf("�x�b�t��W�ĥ�CMturtle�b2008�~���@�~\"�l�{�D\".   �x\n");
  printf("�u - - - - - - - - - - - - - - - - - - - - - - -�t\n");
  printf("�x11/26�[�j�w���i�J���Ѫ��P�_��O�C             �x\n");
  printf("�x11/29�L�T�[�j���X���Ѫ���O�C                 �x\n");
  printf("�x11/30�[�W�p���t�ΡC                           �x\n");
  printf("�x--> �קאּSnake_III_2.exe                     �x\n");
  printf("�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}");
  printf("\n\n\n");
  printf("�Ы����N�䵲���C�� >>\n\n");
  pause();
  return 0;
}

void gotoxy(int xpos, int ypos)
{
  COORD scrn;    

  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);

  scrn.X = xpos; scrn.Y = ypos;

  SetConsoleCursorPosition(hOuput,scrn);
}

void pause(void)
{
  if(getch()==224)
    getch();
}

int waterfilling(int *FindPath,int from_x,int from_y,int to_x,int to_y,int *pmap)
{
  if(*(pmap+from_x*yy+from_y)!=0&&*(pmap+from_x*yy+from_y)!=1)
    return 0;
  int a,b,tempx,tempy,stx[xx*yy],sty[xx*yy],st2x[xx*yy],st2y[xx*yy],top,top2,water;
  int touch;
  top2=0;
  water=2;
  *(FindPath+(from_x)*yy+(from_y))=1;
  stx[0]=from_x;
  sty[0]=from_y;
  top=1;
  touch=0;
  while(1)
  {
    for(top--;top>=0;top--)
    {
	  tempx=stx[top];
	  tempy=sty[top];
	  if(tempx>0)
	  {
	    if(*(FindPath+(tempx-1)*yy+(tempy))==0)
	    {
		  *(FindPath+(tempx-1)*yy+(tempy))=water;
		  st2x[top2]=tempx-1;
		  st2y[top2]=tempy;
		  if(tempx-1==to_x&&tempy==to_y)
		    touch=1;
		  top2++;
		}
	  }
	  if(tempx<xx-1)
	  {
	    if(*(FindPath+(tempx+1)*yy+(tempy))==0)
	    {
		  *(FindPath+(tempx+1)*yy+(tempy))=water;
		  st2x[top2]=tempx+1;
		  st2y[top2]=tempy;
		  if(tempx+1==to_x&&tempy==to_y)
		    touch=1;
		  top2++;
		}
	  }
	  if(tempy>0)
	  {
	    if(*(FindPath+(tempx)*yy+(tempy-1))==0)
	    {
		  *(FindPath+(tempx)*yy+(tempy-1))=water;
		  st2x[top2]=tempx;
		  st2y[top2]=tempy-1;
		  if(tempx==to_x&&tempy-1==to_y)
		    touch=1;
		  top2++;
		}
	  }
	  if(tempy<yy-1)
	  {
	    if(*(FindPath+(tempx)*yy+(tempy+1))==0)
	    {
		  *(FindPath+(tempx)*yy+(tempy+1))=water;
		  st2x[top2]=tempx;
		  st2y[top2]=tempy+1;
		  if(tempx==to_x&&tempy+1==to_y)
		    touch=1;
		  top2++;
		}
	  }
	}
	if(top2==0)
	  break;
	/*
	if(*(FindPath+to_x*yy+to_y)!=0)
	  break;*/
	water++;
	top=top2;
	for(a=0;a<top2;a++)
	{
	  stx[a]=st2x[a];
	  sty[a]=st2y[a];
	}
	top2=0;
  }
  /*
  if(touch==0)
	    SetConsoleTitle("touch=0");
	  else
	    SetConsoleTitle("touch=1");/**/
  return touch;
}

void setdir(int headdir,int *pmapdir,int *pdirx,int *pdiry)
{
    if(headdir==72)
    {
      if(*pdiry!=0)
      {
        *pdirx=-1;
        *pdiry=0;
        *pmapdir=24;
	  }
    }
    else if(headdir==75)
	{
	  if(*pdirx!=0)
	  {
	    *pdirx=0;
  	    *pdiry=-1;
  	    *pmapdir=27;
	  }
	}
	else if(headdir==77)
	{
	  if(*pdirx!=0)
	  {
	    *pdirx=0;
  	    *pdiry=1;
  	    *pmapdir=26;
	  }
	}
	else if(headdir==80)
	{
	  if(*pdiry!=0)
	  {
	    *pdirx=1;
  	    *pdiry=0;
  	    *pmapdir=25;
	  }
	}
}

void move(int *pheadx,int *pheady,int *ptailx,int *ptaily,int dirx,int diry,int *pmap,int *pi_setfood)
{ 
  int tempx,tempy;
  tempx=dirx;
  tempy=diry;
  
  if(dirx==-1)
   *(pmap+(*pheadx)*yy+(*pheady))=24;
  else if(dirx==1)
   *(pmap+(*pheadx)*yy+(*pheady))=25;
  else if(diry==1)
   *(pmap+(*pheadx)*yy+(*pheady))=26;
  else if(diry==-1)
   *(pmap+(*pheadx)*yy+(*pheady))=27;/**/
   gotoxy(2*(*pheady)+2,(*pheadx)+1);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
   printf("��");
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
  *pheadx+=dirx;
  *pheady+=diry;
  
  gotoxy(2*(*pheady)+2,(*pheadx)+1);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
  printf("��");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
  if(*(pmap+(*pheadx)*yy+(*pheady))==1)
  {
    *pi_setfood=1;
    length++;
    gotoxy(2*yy+4,11);
    printf("�@����:%4d",length);
    if((length-3)%7==0)
    {
	  if(testing==0&&low_speed_limit<18)
	    low_speed_limit++;
	}
	point+=(low_speed_limit+1)*(int)sqrt(length);
    gotoxy(2*yy+4,13);
    printf("�@����:%4d",point);
  }
  else
  {
	dirx=diry=0;
	if(*(pmap+(*ptailx)*yy+(*ptaily))==24)
	  dirx=-1;
	else if(*(pmap+(*ptailx)*yy+(*ptaily))==25)
	  dirx=1;
	else if(*(pmap+(*ptailx)*yy+(*ptaily))==26)
	  diry=1;
	else if(*(pmap+(*ptailx)*yy+(*ptaily))==27)
	  diry=-1;
	*(pmap+(*ptailx)*yy+(*ptaily))=0;
	gotoxy(2*(*ptaily)+2,(*ptailx)+1);
	printf("�@");
	*ptailx+=dirx;
	*ptaily+=diry;
  }
  /*
  dirx=tempx;
  diry=tempy;
  
  if(dirx==-1)
   *(pmap+(*pheadx)*yy+(*pheady))=24;
  else if(dirx==1)
   *(pmap+(*pheadx)*yy+(*pheady))=25;
  else if(diry==1)
   *(pmap+(*pheadx)*yy+(*pheady))=26;
  else if(diry==-1)
   *(pmap+(*pheadx)*yy+(*pheady))=27;/**/
}
void setfood(int *pfoodx,int *pfoody,int *pmap,int *pi_setfood,int *pfoodpx,int *pfoodpy)
{
  *pi_setfood=0;
  *(pmap+(*pfoodx)*yy+(*pfoody))=1;
  gotoxy(2*(*pfoody)+2,(*pfoodx)+1);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
  printf("��");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
  *pfoodpx=*pfoodx;
  *pfoodpy=*pfoody;
}
