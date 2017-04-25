#include <stdio.h>
char toByte(int number) {
    return number / 1000000 % 2 << 6 + number / 100000 % 2 << 5 + number / 10000 % 2 << 4 + number / 1000 % 2 << 3 + number / 100 % 2 << 2 + number / 10 % 2 << 1 + number % 2;
}

int main() {
    printf("%c", toByte(1000001));
}
