long vezes2(long a);

/* Implementar função solucao_ex2 */
long solucao_ex2(long a, long b) {
    long res = vezes2(b);
    if (a >= res) {
        return a + res;
    }
    return a + a + res;
}