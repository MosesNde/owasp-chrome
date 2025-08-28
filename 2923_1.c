static inline int square(int x) {
    return x * x;
}

int calculate_area(int length, int width) {
    return square(length) * square(width);
}

int main() {
    int length = 50000;
    int width = 50000;
    int area = calculate_area(length, width);
    return 0;
}