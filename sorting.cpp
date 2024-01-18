#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Sorting {
public:
    static vector<double> readFourthColumn(const string& filename) {
        vector<double> values;

        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error opening input file." << endl;
            exit(1);
        }
  
        // Read CSV file and store 4th column values
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string value1, value2, value3, value4;

            if (
                getline(iss, value1, ',') &&
                getline(iss, value2, ',') &&
                getline(iss, value3, ',') &&
                getline(iss, value4, ',')
            ) {
                // Assuming the 4th column contains double values
                double floatValue = stod(value4);
                values.push_back(floatValue);
            }
        }

        return values;
    }

    static void displayEpochTimeArray(const vector<double>& arr) {
        for (double value : arr) {
            time_t timeValue = static_cast<time_t>(value);
            cout << ctime(&timeValue);
        }
        cout << endl;
    }

    static void displayArray(const vector<double>& arr) {
        for (double value : arr) {
            cout << value << " ";
        }
        cout << endl;
    }

    static void mergeSort(vector<double>& arr, int left, int right) {
        if (left < right) {
            int middle = left + (right - left) / 2;

            // Recursively sort the two halves
            mergeSort(arr, left, middle);
            mergeSort(arr, middle + 1, right);

            // Merge the sorted halves
            merge(arr, left, middle, right);
        }
    }

    static void merge(vector<double>& arr, int left, int middle, int right) {
        int n1 = middle - left + 1;
        int n2 = right - middle;

        // Create temporary arrays
        vector<double> L(n1), R(n2);

        // Copy data to temporary arrays L[] and R[]
        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[middle + 1 + j];

        // Merge the temporary arrays back into arr[left..right]
        int i = 0;  // Initial index of first subarray
        int j = 0;  // Initial index of second subarray
        int k = left;  // Initial index of merged subarray
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy the remaining elements of L[], if there are any
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        // Copy the remaining elements of R[], if there are any
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    static void quickSort(vector<double>& arr, int low, int high) {
        if (low < high) {
            // Partition the array and get the pivot index
            int pivotIndex = partition(arr, low, high);

            // Recursively sort the subarrays
            quickSort(arr, low, pivotIndex - 1);
            quickSort(arr, pivotIndex + 1, high);
        }
    }

    static int partition(vector<double>& arr, int low, int high) {
        double pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }

        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    static void heapSort(vector<double>& arr) {
        int n = arr.size();

        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        // One by one extract an element from the heap
        for (int i = n - 1; i > 0; i--) {
            // Move the current root to the end
            swap(arr[0], arr[i]);

            // Call max heapify on the reduced heap
            heapify(arr, i, 0);
        }
    }

    static void heapify(vector<double>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // If left child is larger than root
        if (left < n && arr[left] > arr[largest])
            largest = left;

        // If right child is larger than largest so far
        if (right < n && arr[right] > arr[largest])
            largest = right;

        // If largest is not the root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify(arr, n, largest);
        }
    }
    static void writeSortedValuesToFile(const vector<double>& arr, const string& fileName) {
        ofstream outputFile(fileName);
        if (!outputFile.is_open()) {
            cerr << "Error opening output file." << endl;
            return;
        }

        for (double value : arr) {
            time_t timeValue = static_cast<time_t>(value);
            outputFile << ctime(&timeValue);
        }

        cout << "Sorted values written to file: " << fileName << endl;
        outputFile.close();
    }
};

int main() {
    // Read 4th column values from CSV file
    vector<double> values = Sorting::readFourthColumn("input.csv");

    // Display the original array
    cout << "Original Array:\n";
    Sorting::displayArray(values);
    cout << endl;

    // Sort using merge sort
    vector<double> mergeSortedValues = values;
    Sorting::mergeSort(mergeSortedValues, 0, mergeSortedValues.size() - 1);
    cout << "Merge Sort:\n";
    Sorting::displayEpochTimeArray(mergeSortedValues);
    Sorting::writeSortedValuesToFile(mergeSortedValues,"merge_sort.txt");


    // Sort using quick sort
    vector<double> quickSortedValues = values;
    Sorting::quickSort(quickSortedValues, 0, quickSortedValues.size() - 1);
    cout << "Quick Sort:\n";
    Sorting::displayEpochTimeArray(quickSortedValues);
    Sorting::writeSortedValuesToFile(quickSortedValues,"quick_sort.txt");
    

    // Sort using heap sort
    vector<double> heapSortedValues = values;
    Sorting::heapSort(heapSortedValues);
    cout << "Heap Sort:\n";
    Sorting::displayEpochTimeArray(heapSortedValues);
    Sorting::writeSortedValuesToFile(heapSortedValues,"heap_sort.txt");

    return 0;
}
