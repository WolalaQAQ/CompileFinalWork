const int constInt = 10;
const float constReal = 20.0;
int Integer = 30;
float Real = 40.0;
int intArray[10] = {0};
float floatArray[10] = {0.0};
int addNum = 0;

int add(int a, int b) {
    return a + b;
}

int main() {
    if (Integer > constInt) {
        Integer = constInt;
    } else {
        Integer = constInt - 10;
    }

    while (Real != 0) {
        Real--;
    }

    addNum = functioncall add(1, 2);

    return 0;
}