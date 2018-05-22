
int * ComputeStartTimes(int * s,int ** I,graph * originG,int *st){
    G = *originG;
    st = *st;
    
    int nodenum = strlen(s)+1;
    
    int * C = (int *)malloc(sizeof(int)*nodenum);
    memset(C,0,sizeof(C));

    int i;
    for(i=0;i<nodenum;i++){
        if(strlen(G.children[i])==0) C[i] = 0;
        else{
            int max = 0;
            int k;
            for(k=0;k<strlen(G.children[i]);k++){
                if((C[k] + I[s[k]][st[k]][1]) > max) max = C[k] + I[s[k]][st[k]][1];
            }
            c[i] = max;
        }
    }

    return C;
}

