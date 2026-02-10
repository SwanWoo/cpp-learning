#include <iostream>
#include <iomanip>
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <variant>

// 实验 1: 基础类型探测
void experiment_basic_types() {
    std::cout << "=== 实验 1: 基础类型与内存对齐 ===\n";
    
    // 展示各种整型的实际大小（平台相关）
    std::cout << "整型家族大小:\n";
    std::cout << "char: " << sizeof(char) << " byte(s)\n";
    std::cout << "short: " << sizeof(short) << " bytes\n";
    std::cout << "int: " << sizeof(int) << " bytes\n";
    std::cout << "long: " << sizeof(long) << " bytes\n";
    std::cout << "long long: " << sizeof(long long) << " bytes\n";
    
    // 固定宽度整数（C++11）
    std::cout << "\n固定宽度整数 (C++11):\n";
    std::cout << "int32_t: " << sizeof(int32_t) << " bytes, "
              << "范围: " << INT32_MIN << " to " << INT32_MAX << "\n";
    
    // 浮点类型的精度实验
    std::cout << "\n浮点精度测试:\n";
    float f = 1.0f / 3.0f;
    double d = 1.0 / 3.0;
    long double ld = 1.0L / 3.0L;
    
    std::cout << std::setprecision(20);
    std::cout << "float (32-bit):  " << f << "\n";
    std::cout << "double (64-bit): " << d << "\n";
    std::cout << "long double:     " << ld << "\n";
}

// 实验 2: 联合体内存布局（验证内存共享）
void experiment_union_memory() {
    std::cout << "\n=== 实验 2: 联合体内存布局与类型双关 ===\n";
    
    union Data {
        int32_t i;
        float f;
        uint8_t bytes[4];
    };
    
    Data data;
    data.f = 3.14159f;
    
    std::cout << "float 值: " << data.f << "\n";
    std::cout << "解释为非符号整数: " << data.i << "\n";
    std::cout << "内存字节 (小端序): ";
    for (int i = 0; i < 4; ++i) {
        std::cout << std::hex << static_cast<int>(data.bytes[i]) << " ";
    }
    std::cout << std::dec << "\n";
    
    // 现代 C++ 推荐使用 std::variant 替代部分联合体用法
    std::variant<int, float, std::string> modern_union;
    modern_union = 3.14f;
    std::cout << "variant 存储 float: " << std::get<float>(modern_union) << "\n";
}

// 实验 3: 枚举类型对比
void experiment_enums() {
    std::cout << "\n=== 实验 3: 传统枚举 vs 强类型枚举 ===\n";
    
    // 传统枚举（隐式转换为 int）
    enum OldStyle { RED = 0xff0000, GREEN = 0x00ff00, BLUE = 0x0000ff };
    OldStyle color = RED;
    int color_val = color;  // 允许隐式转换
    
    // 强类型枚举（C++11，指定底层类型）
    enum class Status : uint8_t { OK = 0, WARNING = 1, ERROR = 2 };
    Status s = Status::OK;
    // int status_val = s;  // 编译错误！禁止隐式转换
    auto underlying = static_cast<std::underlying_type_t<Status>>(s);
    
    std::cout << "传统枚举隐式转 int: " << color_val << " (0x" << std::hex << color_val << ")\n";
    std::cout << "enum class 底层值: " << static_cast<int>(underlying) << std::dec << "\n";
}

// 实验 4: 类型推导与 auto
void experiment_type_deduction() {
    std::cout << "\n=== 实验 4: 类型推导实验 ===\n";
    
    int x = 42;
    int& ref = x;
    const int& cref = x;
    
    // auto 推导规则：去掉引用和顶层 const
    auto a1 = x;      // int
    auto a2 = ref;    // int（不是 int&）
    auto a3 = cref;   // int（不是 const int&）
    auto& a4 = cref;  // const int&
    
    // decltype 推导规则：保留引用性和 cv 限定符
    decltype(x)   d1;  // int
    decltype(ref) d2 = x;  // int&（必须初始化）
    decltype(cref) d3 = x; // const int&（必须初始化）
    
    // decltype(auto) (C++14)
    decltype(auto) d4 = (x);  // int&（因为(x)是左值表达式）
    
    std::cout << "sizeof(auto) vs sizeof(int&): " 
              << sizeof(a1) << " vs " << sizeof(d2) << " (相同，都是 int 大小)\n";
    
    // 验证类型特征
    static_assert(std::is_same_v<decltype(a1), int>);
    static_assert(std::is_same_v<decltype(d2), int&>);
    static_assert(std::is_lvalue_reference_v<decltype(d4)>);
    std::cout << "静态断言验证通过: decltype 保留了引用性\n";
}

// 实验 5: 结构体填充与对齐
void experiment_alignment() {
    std::cout << "\n=== 实验 5: 内存对齐与填充字节 ===\n";
    
    struct Normal {
        char c;
        int i;
        short s;
    };
    
    struct Packed {
        char c;
        short s;
        int i;
    };
    
    struct alignas(64) CacheLine {  // C++11 对齐说明符
        char data[60];
    };
    
    std::cout << "Normal 布局 (c[1]-填充[3]-i[4]-s[2]-填充[2]):\n";
    std::cout << "  sizeof: " << sizeof(Normal) << ", alignof: " << alignof(Normal) << "\n";
    
    std::cout << "Packed 布局 (c[1]-s[2]-i[4], 无填充):\n";
    std::cout << "  sizeof: " << sizeof(Packed) << ", alignof: " << alignof(Packed) << "\n";
    
    std::cout << "CacheLine (强制64字节对齐):\n";
    std::cout << "  sizeof: " << sizeof(CacheLine) << ", alignof: " << alignof(CacheLine) << "\n";
}

// 实验 6: 复杂类型声明解读
void experiment_complex_types() {
    std::cout << "\n=== 实验 6: 复杂类型声明 ===\n";
    
    // 使用类型别名提高可读性
    using FuncPtr = int(*)(int);           // 函数指针
    using ArrayRef = int(&)[10];           // 数组引用
    using ConstCharPtr = const char*;      // 指向常量的指针
    using ConstPtrChar = char* const;      // 常量指针（指针本身不可变）
    
    int arr[10] = {0};
    ArrayRef arr_ref = arr;  // 数组引用防止退化为指针
    
    std::cout << "数组引用保留大小信息: " << sizeof(arr_ref) << " bytes\n";
    std::cout << "指针丢失大小信息: " << sizeof(&arr[0]) << " bytes (仅指针大小)\n";
    
    // 右值引用与移动语义
    std::string str = "Hello";
    std::string&& rref = std::move(str);  // 窃取资源
    
    std::cout << "移动后原字符串: \"" << str << "\" (可能为空，取决于实现)\n";
    std::cout << "右值引用内容: \"" << rref << "\"\n";
}

// 实验 7: 类型特征萃取（元编程）
template<typename T>
void analyze_type() {
    std::cout << "\n分析类型: " << typeid(T).name() << "\n";
    std::cout << "  是整型: " << std::is_integral_v<T> << "\n";
    std::cout << "  是浮点: " << std::is_floating_point_v<T> << "\n";
    std::cout << "  是指针: " << std::is_pointer_v<T> << "\n";
    std::cout << "  是引用: " << std::is_reference_v<T> << "\n";
    std::cout << "  是否平凡: " << std::is_trivial_v<T> << "\n";
    std::cout << "  是否标准布局: " << std::is_standard_layout_v<T> << "\n";
    
    // 类型转换特征
    using NoRef = std::remove_reference_t<T>;
    using NoPtr = std::remove_pointer_t<T>;
    using Decayed = std::decay_t<T>;
    
    std::cout << "  remove_reference: " << typeid(NoRef).name() << "\n";
    std::cout << "  decay (数组退化为指针): " << std::is_array_v<Decayed> << "\n";
}

// 实验 8: nullptr 与指针类型
void experiment_nullptr() {
    std::cout << "\n=== 实验 8: nullptr_t 类型系统 ===\n";
    
    int* p1 = nullptr;           // 类型安全
    void* p2 = nullptr;
    
    // std::nullptr_t 是一种特殊类型，只能被 nullptr 赋值
    decltype(nullptr) null = nullptr;
    
    // 重载解析实验
    auto overload = [](int) { return "int"; };
    auto overload_ptr = [](int*) { return "int*"; };
    auto overload_null = [](std::nullptr_t) { return "nullptr_t"; };
    
    std::cout << "overload(0): " << overload(0) << "\n";
    // std::cout << overload(NULL);  // 歧义！可能是 int 0 或 void*
    std::cout << "overload(nullptr): 选择 nullptr_t 重载 (如果存在) 或指针版本\n";
}

int main() {
    std::cout << "C++ 类型系统实验程序 (C++17)\n";
    std::cout << "=============================\n";
    
    experiment_basic_types();
    experiment_union_memory();
    experiment_enums();
    experiment_type_deduction();
    experiment_alignment();
    experiment_complex_types();
    
    std::cout << "\n=== 实验 7: 模板类型特征萃取 ===\n";
    analyze_type<int>();
    analyze_type<double&>();
    analyze_type<int[10]>();
    
    experiment_nullptr();
    
    std::cout << "\n=== 实验结束 ===\n";
    return 0;
}