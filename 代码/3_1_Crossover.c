#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))  

int * Crossover(int * p1,int *p2,int *** I){//p1,p2�������ȴ������Ⱦɫ��,pΪparent����д
    int i,j,k;//����ѭ���ĸ�������
    int ** idx_p1 = Index(p1,I);
    int ** idx_p2 = Index(p2,I);

    srand((unsigned)time(NULL));
    int nt = strlen(p1);
    int randi = (rand()%(nt-1))+1;//����[1,nt)�������
    int randj = rand()%(nt-1);//����[0,nt-1)�������
    int randk = rand()%nt;//����[0,nt)�������

    //implant �൱������ġ�(A,2)(C,1)(A,3)(B,3)������Parent2��ȡ��Ƭ�� 
    int *** implant =  (int ***)malloc(sizeof(int**)*(MIN(randi+randj,nt)+MIN(randj+randi,nt)));
    j=0;
    for(i=randj;i<MIN(randj+randi,nt);i++,j++){
        implant[j] = &idx_p1[i];
    }
    for(i=0;i<randi-MIN(randj+randi,nt)+reandj);i++,j++){
        implant[j] = &idx_p1[i];
    }

    int ** lft_child = (int **)malloc(sizeof(int *)*randk);
    for(i=0;i<randk;i++){
        lft_child[i] = idx_p2[i];
    }
    
    int ** lft_child = (int **)malloc(sizeof(int *)*(nt-randk));
    for(i=0,j=randk;i<nt-randk;i++,j++){
        rgt_child[i] = idx_p2[j];
    }

    for(i=0;i;i++){
        for(j=0;j;j++){
            if(implant[i] == lft_child[j]){
                remove(lft_child,j);
            }
            if(implant[i] == rgt_child[j]){
                remove(rgt_child,j);
            }
        }
    }


    //�����ǲ��ǿ�����sizeof(implat)������������ֱ��������İ�
    int pingjiesize = 0;
    for(i=0;lft_child[i];i++){
        pingjiesize ++;
    }
    for(i=0;rgt_child[i];i++){
        pingjiesize ++;
    }
    for(i=0;implant[i];i++){
        pingjiesize ++;
    }

    int * child = (int *)malloc(sizeof(int)*pingjiesize);
    
    int j=0
    for(i=0;lft_child[i];i++,j++){
        child[j] =lft_child[i][0];
    }
    for(i=0;rgt_child[i];i++,j++){
        child[j] =rgt_child[i][0];
    }
    for(i=0;implant[i];i++,j++){
        child[j] =implant[i][0];
    }

    return child; 
}

void remove(int ** child,int i){
    for(;child[i];i++) child[i] = child[i+1];
}

int ** Index(int * p1,int *** I){
    int * ct = (int *)malloc(sizeof(int)*n);
    memset(ct,0,sizeof(ct));

    int ** s = (int **)malloc(sizeof(int*)*strlen(p1));
    int i,j;
    for(i=0;i<strlen(p1;i++) s[i] = (int*)malloc(sizeof(int)*2);
    for(i=0,j=0;i<strlen(p1);i++){
        s[i][0] = p1[i];
        s[i][1] = ct[p1[i]]; 
        ct[p1[i]]++;
    }
    return s;
}

