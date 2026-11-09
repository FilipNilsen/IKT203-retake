#include <string>
#include <iostream>

void DumpMem(void* p_data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x ", ((unsigned char*)p_data)[i]);
        if (i % 16 == 0) {
            std::cout<<std::endl;
        }
    }
}

// 1.1 a
template <typename T>
void PrintTypeSize(T v) {
    std::cout<<"Size is: "<< sizeof(v) << std::endl;
}

// 1.1 b
typedef struct custom_t{
    int value;
    bool operator>(const custom_t other){
        if (value > other.value)
            return true;
        return false;
    }
} custom_t;

// 1.1 b
template <typename T>
T GetMax(T a, T b) {
    if (a>b)
    return a;
    return b;
}

// 1.2 a
template <typename ... Args>
struct TDataPack;

// 1.2 b
template <typename T, int Size>
class TStaticArray{
    public:
    T array[Size] = {0};

    TStaticArray() {
        for (size_t i = 0; i < Size; ++i) {
            array[i] = i;
        }
    }

    void Print() {
        for (const auto& n: array) {
            std::cout << n << " ";
        }
        std::cout<<std::endl;
    }
};

// 1.2 c
template <template <typename, int> class Container, typename T, int N>
void PrintContainer(const Container<T,N>& container) {
    for (size_t i = 0; i < N; ++i) {
        std::cout << container.array[i] << " ";
    }
    std::cout << std::endl;
}

// 2 a
int Add(int a, int b) { return a+b; }
int (*p_Add)(int,int) = Add;

// 2 b
std::string Greet(std::string name) { return std::string("Hello ") + name; }
typedef std::string (*GreetPtr)(std::string);
GreetPtr p_Greet = Greet;

// 2 c
bool IsEven(int a) { return a % 2 == 0; }
bool IsOdd(int a) { return a % 2 != 0; }
void FilterAndPrint(int* list, size_t len, bool (*fun)(int)) {
    for (size_t i = 0; i < len; ++i) {
        std::cout << fun(list[i]) << " ";
    }
    std::cout << std::endl;
}

// 2 d
void Play()  {std::cout<<"Playing game...\n";}
void Pause() {std::cout<<"Game paused.\n";}
void Quit()  {std::cout<<"Quitting game...\n";}

// 3 a
template<typename T>
using FActionFunc = void(*)(T&);

// 3 b
template <typename T>
void ProcessArray(T* arr, size_t size, FActionFunc<T> callback) {
    for (size_t i = 0; i < size; ++i) {
        callback(arr[i]);
    }
}

// 3 c
void DoubleInt(int& val) {
    val *= 2;
}

void CapitalizeChar(char& val) {
    val -= 0x20;
}

// 4 a
template<typename T>
class TDataAggregator {
    private:
    T stateTotal;
    public:
    T GetStateTotal() {
        return stateTotal;
    }
    // 4 b
    void Accumulate(T& val) {
        this->stateTotal += val;
        val *= 2;
    }
    // 4 c
    static void StaticCallback(T& val, void* context) {
        ((TDataAggregator<T>*)context)->Accumulate(val);
    }
};

// 4 d
template<typename T>
using FContextFunc = void(*)(T&, void*);

template<typename T>
void ProcessWithContext(T* arr, size_t size, FContextFunc<T> fun, void* context) {
    for (size_t i = 0; i < size; ++i) {
        fun(arr[i], context);
    }
}

int main() {
PrintTypeSize<int>(0);
PrintTypeSize<double>(0);
PrintTypeSize<custom_t>(custom_t{.value=0});

std::cout << GetMax<custom_t>(custom_t{.value=1}, custom_t{.value=10}).value << std::endl;

TStaticArray<int, 30> a;
a.Print();

std::cout << Add(2,2) << " " << (*Add)(2,2) << std::endl;

std::cout<<p_Greet("Filip")<<std::endl;

void (*functions[])() = {Play,Pause,Quit};
for (auto& f: functions) { f(); }

int ints[] = {0,1,2,3,4,5,6,7,8};
char chars[] = "abcdef";
for (int& i: ints) { std::cout << i << " "; }
for (char& c: chars) { std::cout << c << " "; }
std::cout<<std::endl;
ProcessArray<int>(ints, sizeof(ints)/sizeof(int), DoubleInt);
ProcessArray<char>(chars, sizeof(chars), CapitalizeChar);
for (int& i: ints) { std::cout << i << " "; }
for (char& c: chars) { std::cout << c << " "; }
std::cout<<std::endl;

auto b = TDataAggregator<int>();
ProcessWithContext<int>(ints, sizeof(ints)/sizeof(int), TDataAggregator<int>::StaticCallback, &b);
for (int& i: ints) { std::cout << i << " "; }
std::cout << "\n" << b.GetStateTotal() << std::endl;
}