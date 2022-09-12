long soma_n_solucao(int a) {
    int count = 0;
    long res = 0;
    while (count < a) {
        res += count;
        count++;
    }
    return res;
}