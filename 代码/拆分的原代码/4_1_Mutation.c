int * Mutation(int * p){
    srand((unsigned)time(NULL));
    
    int nt = strlen(p);        
    int ranki = rand()%nt;
    int rankj = rand()%nt;

    int * m = (int *)malloc(sizeof(int)*nt);
    int i;
    for(i=0;i<nt;i++){
        m[i] = p[i];
    }

    int temp;
    temp = m[ranki];
    m[ranki] = m[rankj];
    m[rankj] = temp;

    return m;
}


