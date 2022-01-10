


<!-- TOC -->

- [排序](#排序)
  - [排序算法个人总结](#排序算法个人总结)
  - [推荐掌握程度](#推荐掌握程度)
  - [排序算法复杂度](#排序算法复杂度)
  - [比较类排序](#比较类排序)
    - [交换排序](#交换排序)
      - [Bubble Sort](#bubble-sort)
      - [Quick Sort](#quick-sort)
    - [插入排序](#插入排序)
      - [Insertion Sort](#insertion-sort)
      - [Shell Sort](#shell-sort)
    - [选择排序](#选择排序)
      - [Simple Selection Sort](#simple-selection-sort)
      - [Heap Sort](#heap-sort)
    - [归并排序](#归并排序)
      - [Two-way merge sort](#two-way-merge-sort)
      - [Multiple Merge Sort](#multiple-merge-sort)
  - [非比较类排序](#非比较类排序)
    - [计数排序 Counting Sort](#计数排序-counting-sort)
    - [基数排序 Radix Sort](#基数排序-radix-sort)
    - [桶排序 Bucket Sort](#桶排序-bucket-sort)
    - [非比较类排序算法总结 Summary](#非比较类排序算法总结-summary)
- [附录](#附录)

<!-- /TOC -->

## 排序

### 排序算法个人总结  
快速排序综合考察：递归、分治思想。
归并排序综合考察：递归、分治、有序数组/队列合并 等，是一个能够全面考察候选人的一种排序方法；


### 推荐掌握程度
推荐的标准来源于面试笔试的考察情况，参考链接。

冒泡排序 ■■□□□  
快速排序 ■■■■■  
插入排序 ■■■□□  
希尔排序 ■□□□□  
归并排序 ■■■■■  
选择排序 ■■□□□  
堆排序 ■■■■□  
计数排序 ■■□□□  
基数排序 ■□□□□  
桶排序 ■□□□□  
根据上面的情况，相信你会有所侧重地学习排序算法。



### 排序算法复杂度
![十大排序分类](../../z_images/../MortonNotes/z_images/algorithm/sorting/十大排序分类.png)

![10大排序时间复杂度](../../z_images/../MortonNotes/z_images/algorithm/sorting/10大排序时间复杂度.png)


### 比较类排序

#### 交换排序

##### Bubble Sort
* 冒泡排序   
冒泡排序是一种简单的排序算法。它重复地走访过要排序的数列，一次比较两个元素，如果它们的顺序错误就把它们交换过来。走访数列的工作是重复地进行直到没有再需要交换，
也就是说该数列已经排序完成。这个算法的名字由来是因为越小的元素会经由交换慢慢“浮”到数列的顶端。

* 算法描述  
    ```
    1.比较相邻的元素。如果第一个比第二个大，就交换它们两个；  
    2.对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对，这样在最后的元素应该会是最大的数；  
    3.针对所有的元素重复以上的步骤，除了最后一个；  
    4.重复步骤1~3，直到排序完成。  
    ```

* 代码
```cpp
vector<int> bubbleSort(vector<int>& arr) {
    int len = arr.size();
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {       // 相邻元素两两对比
                int temp = arr[j + 1];       // 元素交换
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr;
}
```

##### Quick Sort
* 快速排序   
快速排序的基本思想：通过一趟排序将待排记录分隔成独立的两部分，其中一部分记录的关键字均比另一部分的关键字小，则可分别对这两部分记录继续进行排序，以达到整个序列有序。

* 算法描述  
快速排序使用分治法来把一个串（list）分为两个子串（sub-lists）。具体算法描述如下：  
    ```
    1.从数列中挑出一个元素，称为 “基准”（pivot）；
    2.重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。在这个分区退出之后，该基准就处于数列的中间位置。这个称为分区（partition）操作；
    3.递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序。
    ```
（使用分治算法的排序还有归并排序）

* 代码
```cpp
class Solution {
    int partition(vector<int>& nums, int left, int right) { // 快速排序 C++
        int pivot = nums[right]; // 选一个数值作为pivot
        int i = left - 1;
        for (int j = left; j <= right - 1; j++) { // 将数组中的数值按照选中的pivot大小进行划分，小于等于的在一起，大于的在一起
            if (nums[j] <= pivot) {
                i = i + 1;
                swap(nums[i], nums[j]); // 交换位置
            }
        }
        swap(nums[i + 1], nums[right]); // 这一行代码的意义是：把一开始就搁置的pivot再拉回来，放回中间的位置。左边是小于等于的元素，右边是大于的。
        return i + 1; // 输出pivot位置
    }
    void quicksort(vector<int>& nums, int left, int right) {
        if (left < right) { // 快速排序
            int pos = partition(nums, left, right);
            quicksort(nums, left, pos - 1);
            quicksort(nums, pos + 1, right);
        }
    }
public:
    vector<int> sortArray(vector<int>& nums) {
        quicksort(nums, 0, (int)nums.size() - 1);
        return nums;
    }
};
```
#### 插入排序
##### Insertion Sort
* 简单插入排序  
一般来说，插入排序就指代简单插入排序。  
插入排序（Insertion-Sort）的算法描述是一种简单直观的排序算法。它的工作原理是通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。  

* 算法描述  
一般来说，插入排序都采用in-place在数组上实现。具体算法描述如下：
    ```
    1.从第一个元素开始，该元素可以认为已经被排序；
    2.取出下一个元素，在已经排序的元素序列中从后向前扫描；
    3.如果该元素（已排序）大于新元素，将该元素移到下一位置；
    4.重复步骤3，直到找到已排序的元素小于或者等于新元素的位置；
    5.将新元素插入到该位置后；
    6.重复步骤2~5。
    ```

* 代码  
    ```cpp
    vector<int> insertionSort(vector<int>& arr) {
        int len = arr.size();
        int preIndex, current;
        for(int i = 1; i < len; i++) {
            preIndex = i - 1;
            current = arr[i];
            while(preIndex >= 0 && arr[preIndex] > current) {
                arr[preIndex + 1] = arr[preIndex];
                preIndex--;
            }
            arr[preIndex + 1] = current;
        }
        return arr;
    }
    ```

##### Shell Sort
* 希尔排序   
    1959年Shell发明，第一个突破O(n2)的排序算法，是简单插入排序的改进版。它与插入排序的不同之处在于，它会优先比较距离较远的元素。希尔排序又叫缩小增量排序。

    相对于简单的插入排序，希尔排序的设计更多地考虑了数据本身的特征。本质上来说，希尔排序也是插入排序，或者说简单插入排序是gap = 1的希尔排序（gap下面代码中会提及，就是增量的意思）。比如对于543210的处理，我们想要让其变成递增序列。如果是传统的插入排序，那么需要比较和移动很多次，相当麻烦。但如果是希尔排序，变化过程是：543210 -> 210543 -> 012345 。很快就得到了答案。543210这串数字是有规律的数字，规律就是递减。实际处理数据的时候，数据本身很可能带有某种规律。简单插入排序是“埋头干”，希尔排序有参考到数据本身的性质，因此效率更高。

* 算法描述  

    先将整个待排序的记录序列分割成为若干子序列分别进行直接插入排序，具体算法描述：
    ```
    1.选择一个增量序列t1，t2，…，tk，其中ti>tj，tk=1；
    2.按增量序列个数k，对序列进行k 趟排序；
    3.每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，分别对各子表进行直接插入排序。仅增量因子为1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。
    ```

* 代码
    ```cpp
    void shellSort(vector<int>& arr) {
        // 增量gap，并逐步缩小增量
        for (int gap = arr.size() / 2; gap > 0; gap /= 2) {
            // 从第gap个元素，逐个对其所在组进行直接插入排序操作
            for (int i = gap; i < arr.size(); i++){
                int j = i;
                while (j - gap >= 0 && arr[j] < arr[j-gap]) {           
                    swap(arr[j], arr[j - gap]);
                    j -= gap;
                }
            }
        }
    }
    ```

#### 选择排序

##### Simple Selection Sort   

* 简单选择排序  

    一般来说，选择排序就指代简单选择排序。  
    选择排序是一种简单直观的排序算法。它的工作原理：首先在未排序序列中找到最小（大）元素，存放到排序序列的起始位置，然后，再从剩余未排序元素中继续寻找最小（大）元素，
    然后放到已排序序列的末尾。以此类推，直到所有元素均排序完毕。  

* 算法描述    

    n个记录的直接选择排序可经过n-1趟直接选择排序得到有序结果。具体算法描述如下：

    ```
    1.初始状态：无序区为R[1..n]，有序区为空；
    2.第i趟排序(i=1,2,3…n-1)开始时，当前有序区和无序区分别为R[1..i-1]和R(i..n）。该趟排序从当前无序区中-选出关键字最小的记录 R[k]，将它与无序区的第1个记录R交换，使R[1..i]和R[i+1..n)分别变为记录个数增加1个的新有序区和记录个数减少1个的新无序区；
    3.n-1趟结束，数组有序化了。
    ```

* 代码
    ```cpp
    vector<int> selectionSort(vector<int>& arr) {
        int len = arr.size();
        int minIndex, temp;
        for(int i = 0; i < len - 1; i++) {
            minIndex = i;
            for(int j = i + 1; j < len; j++) {
                if(arr[j] < arr[minIndex]) {    // 寻找最小的数
                    minIndex = j;                // 将最小数的索引保存
                }
            }
            temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
        return arr;
    } 
    ```


##### Heap Sort
* 堆排序    

    堆排序是指利用堆这种数据结构所设计的一种排序算法。堆积是一个近似完全二叉树的结构，并同时满足堆积的性质：即子结点的键值或索引总是小于（或者大于）它的父节点。   

    除了叫做堆，很多地方也叫作优先队列（priority queue）。因此，在调用一些函数或者使用STL的时候，记得看到优先队列，就知道是堆这样的结构。

* 算法描述  

    ```
    1.将初始待排序关键字序列(R1,R2….Rn)构建成大顶堆，此堆为初始的无序区；
    2.将堆顶元素R[1]与最后一个元素R[n]交换，此时得到新的无序区(R1,R2,……Rn-1)和新的有序区(Rn),且满足R[1,2…n-1]<=R[n]；
    3.由于交换后新的堆顶R[1]可能违反堆的性质，因此需要对当前无序区(R1,R2,……Rn-1)调整为新堆，然后再次将R[1]与无序区最后一个元素交换，得到新的无序区(R1,R2….Rn-2)和新的有序区(Rn-1,Rn)。不断重复此过程直到有序区的元素个数为n-1，则整个排序过程完成。
    ```

* 代码
  
    ```cpp
    class Solution {
    public:
        void heapify (vector<int>& arr, int index, int len) { // 建堆
            int left = 2 * index + 1; // 递归方式构建大根堆(len是arr的长度，index是第一个非叶子节点的下标)
            int right = 2 * index + 2; // 根据堆的结构，可知有这样的left和right数值
            int maxIndex = index; // 因为是大根堆，所以关注max
            if ((left < len) && (arr[maxIndex] < arr[left])) { // 两个if判断，为了把三个节点的最大值找到。同时left和right不可超出索引
                maxIndex = left;
            }
            if ((right < len) && (arr[maxIndex] < arr[right])) {
                maxIndex = right;
            } 
            if (maxIndex != index) { // 如果有变动了
                swap(arr[maxIndex], arr[index]); // 那就把数值进行交换。这边的交换就是堆结构中节点的交换。把大的数值放在上面，小的当子节点。
                heapify(arr, maxIndex, len); // 递归调用，继续让该上浮的元素上浮
            }
        }
        void heapSort (vector<int>& arr, int size) { // 排序
            for (int i = size / 2 - 1; i >=0; i--) { // 从尾巴开始。倒序的原因和堆的结构与我们的定义有关。堆的顶端是我们要的，那个数值是
            // 经过全局比较得到的最大值或最小值。这样一来，最小的索引对应全局的比较，那么我们需要倒序的方式建堆。
                heapify(arr, i, size);
            }
            for (int i = size - 1; i >= 1; i--) { // 将最大的数值放在数组的末尾。堆排序的过程是弹出最大值。我们每次都把最大值放最后，
            // 得到的效果就是递增数组。
                swap(arr[0], arr[i]); // 将弹出的最大值放最后
                heapify(arr, 0, i); // 接下来把弹出的值排除在外，对堆的内部进行移动，得到新的弹出的最大值。
            // 这一段移动的代码还是很巧妙的 
            }
        }
        vector<int> sortArray(vector<int>& nums) {
            heapSort(nums, nums.size());
            return nums;
        }
    };
    ```

#### 归并排序
##### Two-way merge sort
* 二路归并排序 Two-way Merge Sort  

    归并排序是建立在归并操作上的一种有效的排序算法。该算法是采用 分治法（Divide and Conquer） 的一个非常典型的应用。将已有序的子序列合并，得到完全有序的序列；即先使每个子序列有序，再使子序列段间有序。若将两个有序表合并成一个有序表，称为2-路归并。

    一般说到的归并排序就是这种二路归并。

* 算法描述  
    ```
    1.把长度为n的输入序列分成两个长度为n/2的子序列；
    2.对这两个子序列分别采用归并排序；
    3.将两个排序好的子序列合并成一个最终的排序序列。
    ```

* 代码
    ```cpp
    #include <cstdio>
    #include <iostream>
    
    using namespace std;
    
    // Function to Merge Arrays L and R into A.
    // lefCount = number of elements in L
    // rightCount = number of elements in R.
    void Merge(int *A,int *L,int leftCount,int *R,int rightCount) {
        int i,j,k;
    
        // i - to mark the index of left aubarray (L)
        // j - to mark the index of right sub-raay (R)
        // k - to mark the index of merged subarray (A)
        i = 0; j = 0; k =0;
    
        while(i<leftCount && j< rightCount) {
            if(L[i]  < R[j]) A[k++] = L[i++];
            else A[k++] = R[j++];
        }
        while(i < leftCount) A[k++] = L[i++];
        while(j < rightCount) A[k++] = R[j++];
    }
    
    // Recursive function to sort an array of integers.
    void MergeSort(int *A,int n) {
        int mid,i, *L, *R;
        if(n < 2) return; // base condition. If the array has less than two element, do nothing.
    
        mid = n/2;  // find the mid index.
    
        // create left and right subarrays
        // mid elements (from index 0 till mid-1) should be part of left sub-array
        // and (n-mid) elements (from mid to n-1) will be part of right sub-array
        L = new int[mid];
        R = new int [n - mid];
    
        for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
        for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray
    
        MergeSort(L,mid);  // sorting the left subarray
        MergeSort(R,n-mid);  // sorting the right subarray
        Merge(A,L,mid,R,n-mid);  // Merging L and R into A as sorted list.
        // the delete operations is very important
        delete [] R;
        delete [] L;
    }
    
    int main() {
        /* Code to test the MergeSort function. */
    
        int A[] = {6,2,3,1,9,10,15,13,12,17}; // creating an array of integers.
        int i,numberOfElements;
    
        // finding number of elements in array as size of complete array in bytes divided by size of integer in bytes.
        // This won't work if array is passed to the function because array
        // is always passed by reference through a pointer. So sizeOf function will give size of pointer and not the array.
        // Watch this video to understand this concept - http://www.youtube.com/watch?v=CpjVucvAc3g
        numberOfElements = sizeof(A)/sizeof(A[0]);
    
        // Calling merge sort to sort the array.
        MergeSort(A,numberOfElements);
    
        //printing all elements in the array once its sorted.
        for(i = 0;i < numberOfElements;i++)
        cout << " " << A[i];
        return 0;
    }
    ```

##### Multiple Merge Sort
* 多路归并排序   
很难碰到这种排序问题，所以这里就不讨论了。感兴趣的同学可以自行搜索。

### 非比较类排序
#### 计数排序 Counting Sort
* Counting Sort  
计数排序不是基于比较的排序算法，其核心在于将输入的数据值转化为键存储在额外开辟的数组空间中。 作为一种线性时间复杂度的排序，计数排序要求输入的数据必须是有确定范围的整数。

* 算法描述  
    ```
    1.找出待排序的数组中最大和最小的元素；
    2.统计数组中每个值为i的元素出现的次数，存入数组C的第i项；
    3.对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）；
    4.反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1。
    ```
* 代码
    ```cpp
    #include <iostream>
    #include <vector>
    using namespace std;
    
    void countingSort(vector<int> &arr, int maxVal) {
        int len = arr.size();
        if (len < 1) return;
        vector<int> count(maxVal+1, 0);
        vector<int> tmp(arr); // 把原数组备份，后面反向填充的时候要用到
        for (auto x : arr)
            count[x]++;
        for (int i = 1; i <= maxVal; ++i)
            count[i] += count[i - 1];
        for (int i = len - 1; i >= 0; i--) { // 反向填充目标数组
            arr[count[tmp[i]] - 1] = tmp[i]; // 进行排序
            count[tmp[i]]--;    // 排好了一个元素，这边需要减一
        }
    }
    
    int main()
    {
        vector<int> arr = { 1,5,3,7,6,2,8,9,4,3,3 };
        int maxVal = 9;
        countingSort(arr,maxVal);
        for (auto x : arr)
            cout << x << " ";
        cout << endl;
        return 0;
    }
    ```

* 分析  
    计数排序是一个稳定的排序算法。当输入的元素是n个0到k之间的整数时，时间复杂度是O(n+k)，空间复杂度也是O(n+k)，其排序速度快于任何比较排序算法。当k不是很大并且序列比较集中时，计数排序是一个很有效的排序算法。

    此外，很明显可以看到：计数排序局限于正整数。如果要对其他类型的数值进行排序，应该要先进行预处理。



#### 基数排序 Radix Sort
* Radix Sort  

    基数排序是一种非比较型整数排序算法，其原理是将整数按位数切割成不同的数字，然后按每个位数分别比较。由于整数也可以表达字符串（比如名字或日期）和特定格式的浮点数，所以基数排序也不是只能使用于整数。

    基数排序按照低位先排序，然后收集；再按照高位排序，然后再收集；依次类推，直到最高位。有时候有些属性是有优先级顺序的，先按低优先级排序，再按高优先级排序。最后的次序就是高优先级高的在前，高优先级相同的低优先级高的在前。

* 算法描述  

    ```
    1.取得数组中的最大数，并取得位数；
    2.arr为原始数组，从最低位开始取每个位组成radix数组；
    3.对radix进行计数排序（利用计数排序适用于小范围数的特点）。
    ```

* 代码
    ```cpp
    int maxbit(int data[], int n) //辅助函数，求数据的最大位数
    {
        int maxData = data[0];              // 最大数
        // 先求出最大数，再求其位数，这样有原先依次每个数判断其位数，稍微优化点。
        for (int i = 1; i < n; ++i)
        {
            if (maxData < data[i])
                maxData = data[i];
        }
        int d = 1;
        int p = 10;
        while (maxData >= p)
        {
            //p *= 10; // Maybe overflow
            maxData /= 10;
            ++d;
        }
        return d;
    /*  int d = 1; //保存最大的位数
        int p = 10;
        for(int i = 0; i < n; ++i)
        {
            while(data[i] >= p)
            {
                p *= 10;
                ++d;
            }
        }
        return d;*/
    }
    void radixsort(int data[], int n) //基数排序
    {
        int d = maxbit(data, n);
        int *tmp = new int[n];
        int *count = new int[10]; //计数器
        int i, j, k;
        int radix = 1;
        for(i = 1; i <= d; i++) //进行d次排序
        {
            for(j = 0; j < 10; j++)
                count[j] = 0; //每次分配前清空计数器
            for(j = 0; j < n; j++)
            {
                k = (data[j] / radix) % 10; //统计每个桶中的记录数
                count[k]++;
            }
            for(j = 1; j < 10; j++)
                count[j] = count[j - 1] + count[j]; //将tmp中的位置依次分配给每个桶
            for(j = n - 1; j >= 0; j--) //将所有桶中记录依次收集到tmp中
            {
                k = (data[j] / radix) % 10;
                tmp[count[k] - 1] = data[j];
                count[k]--;
            }
            for(j = 0; j < n; j++) //将临时数组的内容复制到data中
                data[j] = tmp[j];
            radix = radix * 10;
        }
        delete []tmp;
        delete []count;
    }
    ```

#### 桶排序 Bucket Sort
* Bucket Sort  

    桶排序是计数排序的升级版。它利用了函数的映射关系，高效与否的关键就在于这个映射函数的确定。桶排序的工作的原理：假设输入数据服从均匀分布，将数据分到有限数量的桶里，每个桶再分别排序（有可能再使用别的排序算法或是以递归方式继续使用桶排序进行排）。

    为了使桶排序更加高效，我们需要做到这两点：

    在额外空间充足的情况下，尽量增大桶的数量；
    使用的映射函数能够将输入的 N 个数据均匀的分配到 K 个桶中。
    同时，对于桶中元素的排序，选择何种比较排序算法对于性能的影响至关重要。

    根据上面的定义，我们来探究一下：

    1.什么时候排序最慢？

    当输入的数据被分配到了同一个桶中。

    2.什么时候排序最快？

    当输入的数据可以均匀的分配到每一个桶中。

* 算法描述  
    ```
    1.设置一个定量的数组当作空桶；
    2.遍历输入数据，并且把数据一个一个放到对应的桶里去；
    3.对每个不是空的桶进行排序；
    4.从不是空的桶里把排好序的数据拼接起来。
    ```

* 代码
    ```cpp
    #include<iterator>
    #include<iostream>
    #include<vector>
    using namespace std;
    const int BUCKET_NUM = 10;

    struct ListNode{
            explicit ListNode(int i=0):mData(i),mNext(NULL){}
            ListNode* mNext;
            int mData;
    };

    ListNode* insert(ListNode* head,int val){
            ListNode dummyNode;
            ListNode *newNode = new ListNode(val);
            ListNode *pre,*curr;
            dummyNode.mNext = head;
            pre = &dummyNode;
            curr = head;
            while(NULL!=curr && curr->mData<=val){
                    pre = curr;
                    curr = curr->mNext;
            }
            newNode->mNext = curr;
            pre->mNext = newNode;
            return dummyNode.mNext;
    }


    ListNode* Merge(ListNode *head1,ListNode *head2){
            ListNode dummyNode;
            ListNode *dummy = &dummyNode;
            while(NULL!=head1 && NULL!=head2){
                    if(head1->mData <= head2->mData){
                            dummy->mNext = head1;
                            head1 = head1->mNext;
                    }else{
                            dummy->mNext = head2;
                            head2 = head2->mNext;
                    }
                    dummy = dummy->mNext;
            }
            if(NULL!=head1) dummy->mNext = head1;
            if(NULL!=head2) dummy->mNext = head2;
        
            return dummyNode.mNext;
    }

    void BucketSort(int n,int arr[]){
            vector<ListNode*> buckets(BUCKET_NUM,(ListNode*)(0));
            for(int i=0;i<n;++i){
                    int index = arr[i]/BUCKET_NUM;
                    ListNode *head = buckets.at(index);
                    buckets.at(index) = insert(head,arr[i]);
            }
            ListNode *head = buckets.at(0);
            for(int i=1;i<BUCKET_NUM;++i){
                    head = Merge(head,buckets.at(i));
            }
            for(int i=0;i<n;++i){
                    arr[i] = head->mData;
                    head = head->mNext;
            }
    }
    ```



#### 非比较类排序算法总结 Summary  

非比较类排序算法总结  
这三种排序算法都利用了桶的概念，但对桶的使用方法上有明显差异：

基数排序：根据键值的每位数字来分配桶；  
计数排序：每个桶只存储单一键值；  
桶排序：每个桶存储一定范围的数值。  


## 附录  
  
[排序总结](https://leetcode-cn.com/circle/article/rzsN73/)