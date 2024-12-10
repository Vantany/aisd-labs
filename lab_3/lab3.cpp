
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>

struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;

    stats& operator += (const stats& other) {
        comparison_count += other.comparison_count;
        copy_count += other.copy_count;
        return *this;
    }
};

// Insertion Sort
stats insertion_sort(std::vector<int>& arr) {
    stats statistics;
    int n = arr.size();
    
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        statistics.copy_count++;
        int j = i - 1;
        
        while (j >= 0) {
            statistics.comparison_count++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                statistics.copy_count++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
        statistics.copy_count++;
    }
    
    return statistics;
}

// Comb Sort
stats comb_sort(std::vector<int>& arr) {
    stats statistics;
    int n = arr.size();
    int gap = n;
    bool swapped = true;
    const double shrink_factor = 1.3;
    
    while (gap > 1 || swapped) {
        gap = static_cast<int>(gap / shrink_factor);
        if (gap < 1) gap = 1;
        
        swapped = false;
        for (int i = 0; i + gap < n; i++) {
            statistics.comparison_count++;
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                statistics.copy_count += 3;
                swapped = true;
            }
        }
    }
    
    return statistics;
}

// Quick Sort helper function
stats quick_sort_helper(std::vector<int>& arr, int low, int high, stats& statistics) {
    if (low < high) {
        int pivot = arr[high];
        statistics.copy_count++;
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            statistics.comparison_count++;
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
                statistics.copy_count += 3;
            }
        }
        
        std::swap(arr[i + 1], arr[high]);
        statistics.copy_count += 3;
        
        int partition = i + 1;
        quick_sort_helper(arr, low, partition - 1, statistics);
        quick_sort_helper(arr, partition + 1, high, statistics);
    }
    return statistics;
}

// Quick Sort
stats quick_sort(std::vector<int>& arr) {
    stats statistics;
    return quick_sort_helper(arr, 0, arr.size() - 1, statistics);
}

// Generate random array
std::vector<int> generate_random_array(int size, unsigned seed) {
    std::vector<int> arr(size);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(1, 10000);
    
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Generate sorted array
std::vector<int> generate_sorted_array(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    return arr;
}

// Generate reverse sorted array
std::vector<int> generate_reverse_sorted_array(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    return arr;
}

// Function to run analysis for a specific size
void analyze_for_size(int size, std::ofstream& outFile) {
    // Average case (100 random arrays)
    stats avg_insertion = {0, 0}, avg_comb = {0, 0}, avg_quick = {0, 0};
    
    for (int i = 0; i < 100; i++) {
        auto arr1 = generate_random_array(size, i);
        auto arr2 = arr1;
        auto arr3 = arr1;
        
        avg_insertion += insertion_sort(arr1);
        avg_comb += comb_sort(arr2);
        avg_quick += quick_sort(arr3);
    }
    
    avg_insertion.comparison_count /= 100;
    avg_insertion.copy_count /= 100;
    avg_comb.comparison_count /= 100;
    avg_comb.copy_count /= 100;
    avg_quick.comparison_count /= 100;
    avg_quick.copy_count /= 100;
    
    // Best case (sorted array)
    auto sorted_arr1 = generate_sorted_array(size);
    auto sorted_arr2 = sorted_arr1;
    auto sorted_arr3 = sorted_arr1;
    
    auto best_insertion = insertion_sort(sorted_arr1);
    auto best_comb = comb_sort(sorted_arr2);
    auto best_quick = quick_sort(sorted_arr3);
    
    // Worst case (reverse sorted array)
    auto reverse_arr1 = generate_reverse_sorted_array(size);
    auto reverse_arr2 = reverse_arr1;
    auto reverse_arr3 = reverse_arr1;
    
    auto worst_insertion = insertion_sort(reverse_arr1);
    auto worst_comb = comb_sort(reverse_arr2);
    auto worst_quick = quick_sort(reverse_arr3);
    
    // Write results to file
    outFile << size << ","
            << avg_insertion.comparison_count << "," << avg_insertion.copy_count << ","
            << best_insertion.comparison_count << "," << best_insertion.copy_count << ","
            << worst_insertion.comparison_count << "," << worst_insertion.copy_count << ","
            << avg_comb.comparison_count << "," << avg_comb.copy_count << ","
            << best_comb.comparison_count << "," << best_comb.copy_count << ","
            << worst_comb.comparison_count << "," << worst_comb.copy_count << ","
            << avg_quick.comparison_count << "," << avg_quick.copy_count << ","
            << best_quick.comparison_count << "," << best_quick.copy_count << ","
            << worst_quick.comparison_count << "," << worst_quick.copy_count << "\n";
}

int main() {
    std::ofstream outFile("sorting_analysis.csv");
    
    // Write CSV header
    outFile << "Size,"
            << "Insertion_Avg_Comp,Insertion_Avg_Copy,"
            << "Insertion_Best_Comp,Insertion_Best_Copy,"
            << "Insertion_Worst_Comp,Insertion_Worst_Copy,"
            << "Comb_Avg_Comp,Comb_Avg_Copy,"
            << "Comb_Best_Comp,Comb_Best_Copy,"
            << "Comb_Worst_Comp,Comb_Worst_Copy,"
            << "Quick_Avg_Comp,Quick_Avg_Copy,"
            << "Quick_Best_Comp,Quick_Best_Copy,"
            << "Quick_Worst_Comp,Quick_Worst_Copy\n";
    
    // Analyze for different sizes
    std::vector<int> sizes = {1000, 2000, 3000, 4000, 5000}; //, 6000, 7000, 8000, 9000, 10000, 25000, 50000, 100000
    
    for (int size : sizes) {
        std::cout << "Analyzing for size: " << size << std::endl;
        analyze_for_size(size, outFile);
    }
    
    outFile.close();
    std::cout << "Analysis complete. Results written to sorting_analysis.csv" << std::endl;
    
    return 0;
}