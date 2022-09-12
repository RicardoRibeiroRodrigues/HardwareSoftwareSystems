long ex3_solucao(long a, long b) {
    long count = 0, res = 0;
    while (count < a) {
        long resto = count % b;
        if (!resto) {
            res += count;
        }
        count++;
    }
    return res;
}