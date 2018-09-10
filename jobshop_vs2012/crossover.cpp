#include "jobshop.h"

int ** Index(int * p){//����һ����Ԫ�Ե�����,ÿ����Ԫ���е�һ����ʾ���ǹ���,�ڶ�����ʾ����˳��.P.S.��ôд������Ϊ�˿�������"�߼����� 
    int * ct = (int *)malloc(sizeof(int)*n);
    memset(ct,0,sizeof(int)*n);

    int ** s = (int **)malloc(sizeof(int*)*length);	 
    int i,j;
    for(i=0;i<length;i++) s[i] = (int*)malloc(sizeof(int)*2);
    
    for(i=0,j=0;i<length;i++){
        s[i][0] = p[i];
        s[i][1] = ct[p[i]]; 
        ct[p[i]] = ct[p[i]] + 1;
    }

	//�ڴ��ͷ�
	//�˺����е�s��Crossover��free
	free(ct);

    return s;
}

int * Crossover(int * p1,int *p2){//p1,p2�������ȴ������Ⱦɫ��,pΪparent����д
    int i,j,k;//����ѭ���ĸ�������
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

    int randi = (rand()%(length-1))+1;//����[1,nt)�������,��ʾ��Ƕ��ı���Ƭ��֮�� 
    int randj = rand()%(length-randi);//����[0,nt-randi-1)�������, ��ʾ����Ƭ����parent1�е���ʼλ�� 
    int randk = rand()%length;//����[0,nt)�������,��ʾ����Ƭ����parent2�е���ʼλ�� 

    //implant ��ΪǶ�� .���ɽ�����Ƭ��. 
    int ** implant =  (int **)malloc(sizeof(int*)*(randi));	 
    for(i=0;i<randi;i++){
    	implant[i] = (int *)malloc(sizeof(int)*2);
		implant[i][0] = idx_p1[randj+i][0];
		implant[i][1] = idx_p1[randj+i][1];
	}
	
	//ͬʱ���parent2����Ƕ��Ƭ���ظ�������,������ɾ�� 
	int * aftermutation = (int *)malloc(sizeof(int)*length);
	for(i=0;i<length;i++){
			for(j=0;j<randi;j++){
				if(idx_p2[i][0] == implant[j][0] && idx_p2[i][1] == implant[j][1]) idx_p2[i][0] = -1;//���ϱ�� 
			}
	}

	for(i=0,k=0;i<length;k++){
		if(k==randk){
			for(j=0;j<randi;j++,i++){
				aftermutation[i] = implant[j][0];
			}
		}
		
		if(idx_p2[k][0]!=-1){
			aftermutation[i] = idx_p2[k][0];
			i++;
		}		
	}
	
	 //�ڴ��ͷ�
	for(i=0;i<randi;i++) free(implant[i]);
	free(implant);
	for(i=0;i<length;i++) { free(idx_p1[i]); free(idx_p2[i]); }
	free(idx_p1);
	free(idx_p2); 

	return aftermutation;
}

void Mutation(int * p){
    int ranki = rand()%length;
    int rankj = rand()%length;

    int temp;
    temp = p[ranki];
    p[ranki] = p[rankj];
    p[rankj] = temp;
}
