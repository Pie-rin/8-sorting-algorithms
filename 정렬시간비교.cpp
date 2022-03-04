#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 200000
#define BUCKETS 10
#define DIGITS 6
#define SWAP(x,y,t) ( (t)=(x), (x)=(y), (y)=(t) )

int list1[MAX_SIZE], list2[MAX_SIZE], list3[MAX_SIZE], list4[MAX_SIZE],
	list5[MAX_SIZE], list6[MAX_SIZE], list7[MAX_SIZE], list8[MAX_SIZE];
int sorted[MAX_SIZE];

/*����Ʈ�� ����*/
typedef struct {
	int heap[MAX_SIZE+1];
	int heap_size;
} HeapType;

//�����Լ�
HeapType* create() {
	return (HeapType*)malloc(sizeof(HeapType));
}

//�ʱ�ȭ�Լ�
void init_heap(HeapType* h) {
	h->heap_size = 0;
}

//�����Լ�
void insert_max_heap(HeapType* h, int item) {
	int i;
	i = ++(h->heap_size);

	while ((i != 1) && (item > h->heap[i / 2])) {	//Ʈ���� �Ž��� �ö󰡸� �θ���� ��
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item;	//���ο� ��带 ����
}

//�����Լ�
int delete_max_heap(HeapType* h) {
	int parent, child;
	int item, temp;
	
	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	
	while (child <= h->heap_size) {
		//���� ����� �ڽĳ�� �� �� ū �ڽĳ�带 ã��
		if ((child < h->heap_size) && ((h->heap[child]) < h->heap[child + 1])) child++;
		if (temp >= h->heap[child]) break;
		//�� �ܰ� �Ʒ��� �̵�
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

//���� ���� �Լ�
void heap_sort(int list[], int n) {
	int i;
	HeapType* h;

	h = create();
	init_heap(h);

	for (i = 0; i < n; i++)
		insert_max_heap(h, list[i]);
	for (i = n - 1; i >= 0; i--)
		list[i] = delete_max_heap(h);
	free(h);
}

/*���Ḯ��Ʈ�� ť ����*/
typedef struct QueueNode {
	int item;
	struct QueueNode* link;
} QueueNode;

typedef struct {
	QueueNode* front, * rear;
} QueueType;

//��������Լ�
void error(const char* message) {
	fprintf(stderr, "%s\n", message);
	exit(1);
}

//�ʱ�ȭ�Լ�
void init_queue(QueueType* q) {
	q->front = q->rear = 0;
}

//���� ���� ��� �Լ�
int is_empty(QueueType* q) {
	return (q->front == NULL);
}

//�����Լ�
void enqueue(QueueType* q, int item) {
	QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
	temp->item = item;
	temp->link = NULL;
	if (is_empty(q)) {
		q->front = temp;
		q->rear = temp;
	}
	else {
		q->rear->link = temp;
		q->rear = temp;
	}
}

//�����Լ�
int dequeue(QueueType* q) {
	QueueNode* temp = q->front;
	int item;
	if (is_empty(q))
		error("ť�� ��� �ֽ��ϴ�.");
	else {
		item = temp->item;
		q->front = q->front->link;
		if (q->front == NULL)
			q->rear = NULL;
		free(temp);
		return item;
	}
}

//���� ���� �Լ�
void radix_sort(int list[], int n) {
	int i, b, d, factor = 1;
	QueueType queues[BUCKETS];

	for (b = 0; b < BUCKETS; b++) init_queue(&queues[b]);	//ť���� �ʱ�ȭ

	for (d = 0; d < DIGITS; d++) {
		for (i = 0; i < n; i++)	//�����͵��� �ڸ����� ���� ť�� ����
			enqueue(&queues[(list[i] / factor) % 10], list[i]);

		for (b = i = 0; b < BUCKETS; b++)	//��Ŷ���� ������ list�� ��ħ
			while (!is_empty(&queues[b]))
				list[i++] = dequeue(&queues[b]);
		factor *= 10;	//�� ���� �ڸ����� ����.
	}
}

//���� ���� �Լ�
void selestion_sort(int list[], int n) {
	int i, j, least, temp;
	for (i = 0; i < n - 1; i++) {
		least = i;
		for (j = i + 1; j < n; j++)	//�ּҰ� Ž��
			if (list[j] < list[least]) least = j;
		if (i != least)
			SWAP(list[i], list[least], temp);
	}
}

//���� ���� �Լ�
void bubble_sort(int list[], int n) {
	int i, j, temp;
	for (i = n - 1; i > 0; i--) {
		for (j = 0; j < i; j++)
			if (list[j] > list[j + 1])	//�յ� ���ڵ� �� �� ��ü
				SWAP(list[j], list[j + 1], temp);
	}
}

//���� ���� �Լ�
void insertion_sort(int list[], int n) {
	int i, j, key;
	for (i = 1; i < n; i++) {
		key = list[i];
		for (j = i - 1; j >= 0 && list[j] > key; j--)
			list[j + 1] = list[j];
		list[j + 1] = key;
	}
}

//�� ���� �Լ�
void inc_insertion_sort(int list[], int first, int last, int gap) {
	int i, j, key;
	for (i = first + gap; i <= last; i = i + gap) {
		key = list[i];
		for (j = i - gap; j >= first && key < list[j]; j = j - gap)
			list[j + gap] = list[j];
		list[j + gap] = key;
	}
}
void shell_sort(int list[], int n) {
	int i, gap;
	for (gap = n / 2; gap > 0; gap = gap / 2) {
		if ((gap % 2) == 0) gap++;
		for (i = 0; i < gap; i++)
			inc_insertion_sort(list, i, n - 1, gap);
	}
}

//�� ���� �Լ�
int partition(int list[], int left, int right)
{
	int pivot, temp;
	int low, high;

	low = left;
	high = right + 1;
	pivot = list[left];

	do {
		do {
			low++;
		} while (list[low] < pivot);
		do {
			high--;
		} while (list[high] > pivot);
		if (low < high) SWAP(list[low], list[high], temp);
	} while (low < high);

	SWAP(list[left], list[high], temp);
	return high;
}
void quick_sort(int list[], int left, int right) {
	if (left < right) {
		int q = partition(list, left, right);
		quick_sort(list, left, q - 1);
		quick_sort(list, q + 1, right);
	}
}

//�պ� ���� �Լ�
void merge(int list[], int left, int mid, int right) {
	/*i�� ���ĵ� ���� ����Ʈ�� ���� �ε���
	* j�� ���ĵ� ������ ����Ʈ�� ���� �ε���
	* k�� ���ĵ� ����Ʈ�� ���� �ε���*/
	int i, j, k, l;
	i = left; j = mid + 1; k = left;

	//���� ���ĵ� list�� �պ�
	while (i <= mid && j <= right) {
		if (list[i] <= list[j])
			sorted[k++] = list[i++];
		else
			sorted[k++] = list[j++];
	}
	if (i > mid)	//�����ִ� ���ڵ� �ϰ� ����
		for (l = j; l <= right; l++)
			sorted[k++] = list[l];
	else	//�����ִ� ���ڵ��� �ϰ� ����
		for (l = i; l <= mid; l++)
			sorted[k++] = list[l];
	//�迭 sorted[]�� ����Ʈ�� �迭 list[]�� �纹��
	for (l = left; l <= right; l++)
		list[l] = sorted[l];
}
void merge_sort(int list[], int left, int right) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;	//����Ʈ �յ� ����
		merge_sort(list, left, mid);	//�κ� ����Ʈ ����
		merge_sort(list, mid+1, right);	//�κ� ����Ʈ ����
		merge(list, left, mid, right);	//�պ�
	}
}

int main(void) {
	int i;
	int n = MAX_SIZE;
	clock_t start, stop;
	double duration;

	srand(time(NULL));
	for (i = 0; i < n; i++)	//���� ���� �� �� ����Ʈ�� �Ȱ��� ����
		list1[i] = list2[i] = list3[i] = list4[i] =
		list5[i] = list6[i] = list7[i] = list8[i] = rand();

	start = clock();
	selestion_sort(list1, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("���������� ����ð�: %f��\n", duration);

	start = clock();
	bubble_sort(list2, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("���������� ����ð�: %f��\n", duration);

	start = clock();
	insertion_sort(list3, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("���������� ����ð�: %f��\n", duration);

	start = clock();
	shell_sort(list4, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("�������� ����ð�: %f��\n", duration);

	start = clock();
	heap_sort(list5, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("���������� ����ð�: %f��\n", duration);

	start = clock();
	quick_sort(list6, 0, n - 1);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("�������� ����ð�: %f��\n", duration);

	start = clock();
	merge_sort(list7, 0, n - 1);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("�պ������� ����ð�: %f��\n", duration);

	start = clock();
	radix_sort(list8, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("���������� ����ð�: %f��\n", duration);

	return 0;
}