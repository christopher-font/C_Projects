// finds the max amount from a double pointer called accounts
int maximumWealth(int** accounts, int accountsSize, int* accountsColSize) {
    int max = 0;
    
    for (int i = 0; i < accountsSize; i++) {
        int tmp = 0;
        
        for (int j = 0 ; j < *accountsColSize; j++) 
            tmp += accounts[i][j];
        if (tmp < max) max = tmp;
    }
    return max;
}
