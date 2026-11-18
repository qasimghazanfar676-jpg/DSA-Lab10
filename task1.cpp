#include <iostream>
#include <iomanip>

/*
================================================================================
ANSWERS TO TASK 1 QUESTIONS:
1. What is the worst-case number of swaps for one insertion into a heap of n elements?
   - Answer: The worst-case number of swaps is equal to the height of the tree, 
     which is floor(log2(n)). A newly inserted element may need to sift up all 
     the way from the leaf level to the root.

2. What is the time complexity of building the heap by n insertions, and of heap sort?
   - Building heap via n insertions: Each insertion takes O(log k) where k is 
     the current size. For n elements, this takes sum_{k=1}^n O(log k) = O(n log n).
   - Heap Sort: Extracting/swapping the root and sifting down n times takes O(n log n).
     Total Time Complexity = O(n log n).
================================================================================
*/

class MaxHeap {
private:
    int* arr;
    int capacity;
    int size;

    // Helper swap function without STL
    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    /**
     * Sifts up the element at index i to restore max-heap property.
     * Returns the number of swaps performed.
     * Time Complexity: O(log n)
     */
    int siftUp(int i) {
        int swaps = 0;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (arr[i] > arr[parent]) {
                swap(arr[i], arr[parent]);
                swaps++;
                i = parent;
            } else {
                break;
            }
        }
        return swaps;
    }

    /**
     * Sifts down the element at index i within a heap of size 'n'.
     * Time Complexity: O(log n)
     */
    void siftDown(int i, int n) {
        while (true) {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < n && arr[left] > arr[largest]) {
                largest = left;
            }
            if (right < n && arr[right] > arr[largest]) {
                largest = right;
            }

            if (largest != i) {
                swap(arr[i], arr[largest]);
                i = largest;
            } else {
                break;
            }
        }
    }

public:
    /**
     * Constructor initializing dynamic array, capacity, and size.
     * Time Complexity: O(1)
     */
    MaxHeap(int cap = 100) {
        capacity = cap;
        size = 0;
        arr = new int[capacity];
    }

    /**
     * Destructor to free dynamically allocated memory.
     * Time Complexity: O(1)
     */
    ~MaxHeap() {
        delete[] arr;
    }

    /**
     * Inserts a value at the end and sifts it up.
     * Returns the number of swaps performed.
     * Time Complexity: O(log n)
     */
    int insert(int value) {
        if (size >= capacity) {
            std::cout << "Heap is full!" << std::endl;
            return 0;
        }
        arr[size] = value;
        int swaps = siftUp(size);
        size++;
        return swaps;
    }

    /**
     * Prints the current array representation of the heap.
     * Time Complexity: O(n)
     */
    void printHeap() const {
        for (int i = 0; i < size; i++) {
            std::cout << arr[i] << (i + 1 < size ? " " : "");
        }
        std::cout << std::endl;
    }

    /**
     * Sorts the elements in-place in ascending order using heap sort.
     * Swaps root with the last element of the active heap and sifts down.
     * Time Complexity: O(n log n)
     */
    void heapSort() {
        int initialSize = size;
        for (int i = size - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            siftDown(0, i);
        }
        // size remains as initialSize to allow printing the sorted array
        size = initialSize;
    }
};

int main() {
    int inputs[] = {25, 40, 15, 60, 35, 10, 70, 50};
    int n = sizeof(inputs) / sizeof(inputs[0]);

    MaxHeap heap(n + 10);

    std::cout << "Insert | Swaps | Heap after insertion\n";
    std::cout << "-------------------------------------\n";

    for (int i = 0; i < n; i++) {
        int val = inputs[i];
        int swaps = heap.insert(val);
        std::cout << std::setw(6) << std::left << val << " | "
                  << std::setw(5) << std::left << swaps << " | ";
        heap.printHeap();
    }

    heap.heapSort();

    std::cout << "\nSorted output: ";
    heap.printHeap();

    return 0;
}