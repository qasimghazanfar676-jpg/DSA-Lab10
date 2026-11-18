#include <iostream>
#include <iomanip>
#include <string>

/*
================================================================================
ANSWERS TO TASK 2 QUESTIONS:
1. Why is the median a better alert measure here than the average? Use arrival 6 in your answer.
   - Answer: The average (mean) is highly sensitive to extreme outliers, whereas 
     the median is robust against them. At arrival 6, patient 6 arrives with a wait 
     of 90 minutes. 
     The elements are: 15, 20, 30, 45, 60, 90.
     - Mean = (15 + 20 + 30 + 45 + 60 + 90) / 6 = 260 / 6 = 43.33 minutes.
     - Median = (30 + 45) / 2 = 37.5 minutes.
     While both trigger an alert at arrival 6, suppose an extreme outlier arrived 
     (e.g., 200 mins due to a unique paperwork delay). That single anomaly would 
     massively skew the mean upward and trigger false staffing alarms for an otherwise 
     efficient ER. The median accurately reflects the typical patient's experience.

2. What goes wrong if the two heap sizes differ by two or more?
   - Answer: If the heap sizes differ by two or more, the roots of the heaps no 
     longer represent the middle element(s) of the sorted sequence. The invariant 
     that separates the lower 50% and upper 50% of values is broken, meaning 
     getMedian() will return an incorrect, skewed value in O(1) time.

3. How many total operations does repeated sorting need compared with your heap method for n patients?
   - Repeated sorting: After each patient i (from 1 to n), sorting an array of 
     size i takes O(i log i) operations. Over n arrivals, total work is:
     Sum_{i=1}^n O(i log i) = O(n^2 log n).
   - Heap method: Each arrival performs at most one insert and one rebalance 
     (push/pop) taking O(log i) operations. Median query is O(1).
     Total work over n arrivals: Sum_{i=1}^n O(log i) = O(n log n).
     The heap approach reduces total time complexity from O(n^2 log n) to O(n log n).
================================================================================
*/

class Heap {
private:
    int* arr;
    int capacity;
    int size;
    bool isMaxHeap; // true = max-heap, false = min-heap

    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    /**
     * Determines whether child should be prioritized over parent.
     * Time Complexity: O(1)
     */
    bool compare(int child, int parent) const {
        if (isMaxHeap) {
            return child > parent;
        } else {
            return child < parent;
        }
    }

    /**
     * Sifts up element at index i to restore heap property.
     * Time Complexity: O(log n)
     */
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (compare(arr[i], arr[parent])) {
                swap(arr[i], arr[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    /**
     * Sifts down element at index i to restore heap property.
     * Time Complexity: O(log n)
     */
    void siftDown(int i) {
        while (true) {
            int target = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < size && compare(arr[left], arr[target])) {
                target = left;
            }
            if (right < size && compare(arr[right], arr[target])) {
                target = right;
            }

            if (target != i) {
                swap(arr[i], arr[target]);
                i = target;
            } else {
                break;
            }
        }
    }

public:
    /**
     * Constructor setting up storage and heap orientation.
     * Time Complexity: O(1)
     */
    Heap(bool maxType, int cap = 200) {
        isMaxHeap = maxType;
        capacity = cap;
        size = 0;
        arr = new int[capacity];
    }

    /**
     * Destructor to free heap memory.
     * Time Complexity: O(1)
     */
    ~Heap() {
        delete[] arr;
    }

    /**
     * Inserts value into heap and restores ordering.
     * Time Complexity: O(log n)
     */
    void push(int val) {
        if (size >= capacity) return;
        arr[size] = val;
        siftUp(size);
        size++;
    }

    /**
     * Removes and returns the root element.
     * Time Complexity: O(log n)
     */
    int pop() {
        if (size == 0) return -1;
        int rootVal = arr[0];
        arr[0] = arr[size - 1];
        size--;
        if (size > 0) {
            siftDown(0);
        }
        return rootVal;
    }

    /**
     * Returns the root element without removal.
     * Time Complexity: O(1)
     */
    int top() const {
        if (size == 0) return -1;
        return arr[0];
    }

    /**
     * Returns current number of elements in the heap.
     * Time Complexity: O(1)
     */
    int getSize() const {
        return size;
    }
};

class ERWaitTimeMonitor {
private:
    Heap maxHeap; // Smaller half of elements
    Heap minHeap; // Larger half of elements
    const double ALERT_THRESHOLD = 35.0;

public:
    /**
     * Constructor initializing max heap and min heap.
     * Time Complexity: O(1)
     */
    ERWaitTimeMonitor() : maxHeap(true), minHeap(false) {}

    /**
     * Inserts a wait time into the appropriate heap and rebalances.
     * Time Complexity: O(log n)
     */
    void addWait(int x) {
        // Place in appropriate heap
        if (maxHeap.getSize() == 0 || x <= maxHeap.top()) {
            maxHeap.push(x);
        } else {
            minHeap.push(x);
        }

        // Rebalance heaps so sizes differ by at most 1,
        // with maxHeap holding the extra element if sizes are unequal.
        if (maxHeap.getSize() > minHeap.getSize() + 1) {
            minHeap.push(maxHeap.pop());
        } else if (minHeap.getSize() > maxHeap.getSize()) {
            maxHeap.push(minHeap.pop());
        }
    }

    /**
     * Computes the current median wait time.
     * Time Complexity: O(1)
     */
    double getMedian() const {
        if (maxHeap.getSize() == 0) return 0.0;

        if (maxHeap.getSize() > minHeap.getSize()) {
            return static_cast<double>(maxHeap.top());
        } else {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }

    /**
     * Checks if current median exceeds 35 minutes.
     * Time Complexity: O(1)
     */
    bool isAlert() const {
        return getMedian() > ALERT_THRESHOLD;
    }

    int getMaxHeapRoot() const { return maxHeap.top(); }
    int getMinHeapRoot() const { return minHeap.top(); }
    int getMinHeapSize() const { return minHeap.getSize(); }
};

int main() {
    int inputs[] = {45, 20, 60, 15, 30, 90, 10, 55};
    int n = sizeof(inputs) / sizeof(inputs[0]);

    ERWaitTimeMonitor monitor;

    std::cout << "Arrival | Wait | Max heap root | Min heap root | Median | Status\n";
    std::cout << "----------------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
        int wait = inputs[i];
        monitor.addWait(wait);

        std::cout << std::setw(7) << std::left << (i + 1) << " | "
                  << std::setw(4) << std::left << wait << " | "
                  << std::setw(13) << std::left << monitor.getMaxHeapRoot() << " | ";

        if (monitor.getMinHeapSize() == 0) {
            std::cout << std::setw(13) << std::left << "none" << " | ";
        } else {
            std::cout << std::setw(13) << std::left << monitor.getMinHeapRoot() << " | ";
        }

        std::cout << std::setw(6) << std::left << monitor.getMedian() << " | "
                  << (monitor.isAlert() ? "ALERT" : "Normal") << "\n";
    }

    return 0;
}