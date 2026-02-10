#include <iostream>
#include <vector>
#include <optional>
#include <cassert>

// 返回找到的索引，未找到返回 std::nullopt
std::optional<size_t> binarySearch(const std::vector<int>& arr, int target) {
    if (arr.empty()) return std::nullopt;
    
    size_t left = 0;
    size_t right = arr.size() - 1;  // 闭区间 [left, right]
    
    while (left <= right) {
        // 防止溢出的写法
        size_t mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            // 处理 mid == 0 的下溢情况
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return std::nullopt;
}

// 辅助函数：打印测试结果
void testSearch(const std::vector<int>& arr, int target, const std::string& desc) {
    auto result = binarySearch(arr, target);
    std::cout << "测试: " << desc << "\n";
    std::cout << "  查找目标: " << target << " -> ";
    if (result) {
        std::cout << "找到，索引 = " << *result << "，值 = " << arr[*result] << "\n";
    } else {
        std::cout << "未找到\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== 折半查找算法测试 ===\n\n";
    
    // 测试数据：已排序数组
    std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    
    std::cout << "测试数组: [";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << (i < arr.size() - 1 ? ", " : "");
    }
    std::cout << "]\n\n";
    
    // 1. 正常查找（中间元素）
    testSearch(arr, 9, "查找中间元素 9");
    
    // 2. 查找第一个元素（边界）
    testSearch(arr, 1, "查找第一个元素 1");
    
    // 3. 查找最后一个元素（边界）
    testSearch(arr, 19, "查找最后一个元素 19");
    
    // 4. 不存在的元素（中间范围）
    testSearch(arr, 10, "查找不存在的元素 10");
    
    // 5. 不存在的元素（小于最小值）
    testSearch(arr, 0, "查找小于最小值的 0");
    
    // 6. 不存在的元素（大于最大值）
    testSearch(arr, 20, "查找大于最大值的 20");
    
    // 7. 空数组测试
    std::vector<int> empty_arr;
    testSearch(empty_arr, 5, "空数组测试");
    
    // 8. 单元素数组测试
    std::vector<int> single_arr = {5};
    testSearch(single_arr, 5, "单元素数组（存在）");
    testSearch(single_arr, 3, "单元素数组（不存在）");
    
    // 9. 重复元素测试（返回其中一个索引，通常是中间的）
    std::vector<int> dup_arr = {1, 2, 2, 2, 3};
    testSearch(dup_arr, 2, "重复元素数组查找 2");
    
    // 使用 assert 进行自动化验证
    std::cout << "=== 自动化断言测试 ===\n";
    assert(binarySearch(arr, 9).value() == 4);
    assert(binarySearch(arr, 1).value() == 0);
    assert(binarySearch(arr, 19).value() == 9);
    assert(!binarySearch(arr, 100).has_value());
    assert(!binarySearch(std::vector<int>{}, 5).has_value());
    std::cout << "所有断言测试通过!\n";
    
    return 0;
}