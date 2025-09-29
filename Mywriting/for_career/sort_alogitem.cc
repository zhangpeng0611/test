#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/*
算法稳定性的衡量标准是什么：
算法稳定性的衡量标准是：判断算法在处理过程中是否会破坏具有相同属性的元素之间的原始相对顺序
稳定排序：冒泡排序，归并排序，插入排序
不稳定排序：快速排序，选择排序，堆排序
*/


#if 0

//冒泡排序:最好的时间复杂度为O(n),最差的时间复杂度为O(n*n)
void mySwap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void bubbleSort(int A[], int n) {
	for (int i = 0; i < n - 1; i++) {
		bool flag = false; //标志是否进行了交换
		for (int j = n - 1; j > i; j--) {
			if (A[j] < A[j - 1]) {
				mySwap(A[j], A[j - 1]);
				flag = true;
			}
		}
		if (flag == false) return;
	}
}


int main() {
	int A[6] = { 88,34,26,74,56,94 };
	bubbleSort(A, 6);
	for (auto& it : A) {
		cout << it << " ";
	}
}

#endif

#if 0
//时间复杂度不会超过O(n)
int partition(int A[],int low,int high) {
	int pivot = A[low]; 
	while (low < high) {
		while (low < high && A[high] >= pivot) high--;
		A[low] = A[high];
		while (low < high && A[low] <= pivot) low++;
		A[high] = A[low];
	}
	A[low] = pivot;
	return low;
}


//每一层的递归的时间复杂度都不会超过O(n),所以总体的时间复杂度就是O(n*递归层数)
//最好的时间复杂度:O(nlog2n),最坏的时间复杂度:O(n*n)
void quickSort(int A[],int low,int high) {
	//递归的条件
	if (low < high) {
		//找出枢轴元素的位置
		int pivotpos = partition(A, low, high);
		//将枢轴左边进行快排
		quickSort(A, low, pivotpos - 1);
		//将枢轴右边进行快排
		quickSort(A, pivotpos+1, high);
	}
}

int main() {
	int A[6] = { 88,34,26,74,56,94 };
	quickSort(A, 0, 5);
	for (auto& it : A) {
		cout << it << " ";
	}
}

#endif

#if 0
//堆排序:注意给定的待排序数组下标从1开始，这样才能满足2i为其左分支，2i+1为其右分支，所以
//数组下标为0的位置要被留出来
//时间复杂度为:O(nlog2n)

//以下标为k的分支节点为根节点的堆进行调整
void headAdjust(int A[],int k,int len) {
	//记录根节点的数值
	A[0] = A[k];
	for (int i = 2 * k; i <=len; i *= 2) {
		if (i < len && A[i] < A[i + 1])
			i++;
		if (A[0] >= A[i]) break;
		else {
			//此时A[0]<A[i]说明根节点的数值不是最大的，需要进行调整，将根节点数值
			//和右分支节点进行交换，之后还需要下坠检查右分支节点是否也符合大根堆的定义
			A[k] = A[i];
			//k=i就表示根节点进行下坠的过程
			k = i;
		}
	}
	A[k] = A[0];
}

void buildHeap(int A[],int len) {
	//i从len/2开始是因为下标小于等于len/2的都属于分支节点，下标大于len/2的都属于叶子节点
	for (int i = len / 2; i > 0;i--) {
		headAdjust(A, i, len);
	}
}

void heapSort(int A[], int len) {
	buildHeap(A, len);
	for (int i = len; i > 1; i--) {
		swap(A[i], A[1]);
		headAdjust(A, 1, i-1);
	}
}

int main() {
	// 示例数组，A[0]作为临时空间，不参与排序
	int A[] = { 0, 4, 1, 3, 2, 5 };
	int len = 5; // 实际元素个数（A[1]到A[5]）

	cout << "排序前: ";
	for (int i = 1; i <= len; i++) {
		cout << A[i] << " ";
	}
	cout << endl;

	heapSort(A, len);

	cout << "排序后: ";
	for (int i = 1; i <= len; i++) {
		cout << A[i] << " ";
	}
	cout << endl;

	return 0;
}

#endif

#if 0

//归并排序：
//归并排序非常稳定，其最好最坏和平均时间复杂度均为O(nlog2n)，而快速排序就没有这么稳定，这是因为快速排序的二分是
//根据选定的基准元素进行的，依赖基准元素进行分区，如果基准元素为数组中最小或最大的元素，时间复杂度将会退化为O(n*n),
//而归并元素对于重复元素的处理和其他任何元素都一样
vector<int>B;

void Mege(vector<int>&A, int low, int mid, int high) {
	int i, j, k; //i,j分别指向两个要进行归并排序的数组

	for (k = low; k <= high; k++) {
		B[k] = A[k];
	}
	for (i = low, j = mid + 1, k = i; i <= mid && j <= high; k++) {
		if (B[i] <= B[j]) A[k] = B[i++];
		else A[k] = B[j++];
	}
	while (i <= mid) A[k++] = B[i++];
	while (j <= high) A[k++] = B[j++];
}

void MergeSort(vector<int>&A, int low, int high) {
	if(low==0 && high==A.size()-1) B.resize(A.size());
	if (low < high) {
		int mid = (low + high) / 2;
		MergeSort(A, low, mid);
		MergeSort(A, mid + 1, high);
		Mege(A, low, mid, high);
	}
}

int main() {
	vector<int>A = { 0, 4, 1, 3, 2, 5 };
	MergeSort(A, 0, 5);
	for (auto it : A) {
		cout << it << " ";
	}
}

#endif