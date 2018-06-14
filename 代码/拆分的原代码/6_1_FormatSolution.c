
int ** FormatSolution(int * s,int * C,int *** I){
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(T));
    int ** S = (int **)malloc(sizeof((int*)*n));
    
    int i;
    for(i=0;i<n;i++){
        S[i] = (int *)malloc(sizeof((int)*m));
        memset(S[i],0,sizeof(S[i]));
    }

    for(i=0;i<strlen(s);i++){
        int j = s[i];
        int t = T[j];
        S[j][t] = C[i];
        T[j] ++;
    }
    return S;
}

