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

/*히프트리 구현*/
typedef struct {
	int heap[MAX_SIZE+1];
	int heap_size;
} HeapType;

//생성함수
HeapType* create() {
	return (HeapType*)malloc(sizeof(HeapType));
}

//초기화함수
void init_heap(HeapType* h) {
	h->heap_size = 0;
}

//삽입함수
void insert_max_heap(HeapType* h, int item) {
	int i;
	i = ++(h->heap_size);

	while ((i != 1) && (item > h->heap[i / 2])) {	//트리를 거슬러 올라가며 부모노드와 비교
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item;	//새로운 노드를 삽입
}

//삭제함수
int delete_max_heap(HeapType* h) {
	int parent, child;
	int item, temp;
	
	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	
	while (child <= h->heap_size) {
		//현재 노드의 자식노드 중 더 큰 자식노드를 찾음
		if ((child < h->heap_size) && ((h->heap[child]) < h->heap[child + 1])) child++;
		if (temp >= h->heap[child]) break;
		//한 단계 아래로 이동
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

//히프 정렬 함수
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

/*연결리스트로 큐 구현*/
typedef struct QueueNode {
	int item;
	struct QueueNode* link;
} QueueNode;

typedef struct {
	QueueNode* front, * rear;
} QueueType;

//오류출력함수
void error(const char* message) {
	fprintf(stderr, "%s\n", message);
	exit(1);
}

//초기화함수
void init_queue(QueueType* q) {
	q->front = q->rear = 0;
}

//공백 상태 출력 함수
int is_empty(QueueType* q) {
	return (q->front == NULL);
}

//삽입함수
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

//삭제함수
int dequeue(QueueType* q) {
	QueueNode* temp = q->front;
	int item;
	if (is_empty(q))
		error("큐가 비어 있습니다.");
	else {
		item = temp->item;
		q->front = q->front->link;
		if (q->front == NULL)
			q->rear = NULL;
		free(temp);
		return item;
	}
}

//래딕스 정렬 함수
void radix_sort(int list[], int n) {
	int i, b, d, factor = 1;
	QueueType queues[BUCKETS];

	for (b = 0; b < BUCKETS; b++) init_queue(&queues[b]);	//큐들의 초기화

	for (d = 0; d < DIGITS; d++) {
		for (i = 0; i < n; i++)	//데이터들을 자리수에 따라 큐에 삽입
			enqueue(&queues[(list[i] / factor) % 10], list[i]);

		for (b = i = 0; b < BUCKETS; b++)	//버킷에서 꺼내어 list로 합침
			while (!is_empty(&queues[b]))
				list[i++] = dequeue(&queues[b]);
		factor *= 10;	//그 다음 자리수로 간다.
	}
}

//선택 정렬 함수
void selestion_sort(int list[], int n) {
	int i, j, least, temp;
	for (i = 0; i < n - 1; i++) {
		least = i;
		for (j = i + 1; j < n; j++)	//최소값 탐색
			if (list[j] < list[least]) least = j;
		if (i != least)
			SWAP(list[i], list[least], temp);
	}
}

//버블 정렬 함수
void bubble_sort(int list[], int n) {
	int i, j, temp;
	for (i = n - 1; i > 0; i--) {
		for (j = 0; j < i; j++)
			if (list[j] > list[j + 1])	//앞뒤 레코드 비교 후 교체
				SWAP(list[j], list[j + 1], temp);
	}
}

//삽입 정렬 함수
void insertion_sort(int list[], int n) {
	int i, j, key;
	for (i = 1; i < n; i++) {
		key = list[i];
		for (j = i - 1; j >= 0 && list[j] > key; j--)
			list[j + 1] = list[j];
		list[j + 1] = key;
	}
}

//쉘 정렬 함수
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

//퀵 정렬 함수
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

//합병 정렬 함수
void merge(int list[], int left, int mid, int right) {
	/*i는 정렬된 왼쪽 리스트에 대한 인덱스
	* j는 정렬된 오른쪽 리스트에 대한 인덱스
	* k는 정렬된 리스트에 대한 인덱스*/
	int i, j, k, l;
	i = left; j = mid + 1; k = left;

	//분할 정렬된 list의 합병
	while (i <= mid && j <= right) {
		if (list[i] <= list[j])
			sorted[k++] = list[i++];
		else
			sorted[k++] = list[j++];
	}
	if (i > mid)	//남아있는 레코드 일괄 복사
		for (l = j; l <= right; l++)
			sorted[k++] = list[l];
	else	//남아있는 레코드의 일괄 복사
		for (l = i; l <= mid; l++)
			sorted[k++] = list[l];
	//배열 sorted[]의 리스트를 배열 list[]로 재복사
	for (l = left; l <= right; l++)
		list[l] = sorted[l];
}
void merge_sort(int list[], int left, int right) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;	//리스트 균등 분할
		merge_sort(list, left, mid);	//부분 리스트 정렬
		merge_sort(list, mid+1, right);	//부분 리스트 정렬
		merge(list, left, mid, right);	//합병
	}
}

int main(void) {
	int i;
	int n = MAX_SIZE;
	clock_t start, stop;
	double duration;

	srand(time(NULL));
	for (i = 0; i < n; i++)	//난수 생성 및 각 리스트에 똑같이 삽입
		list1[i] = list2[i] = list3[i] = list4[i] =
		list5[i] = list6[i] = list7[i] = list8[i] = rand();

	start = clock();
	selestion_sort(list1, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("선택정렬의 수행시간: %f초\n", duration);

	start = clock();
	bubble_sort(list2, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("버블정렬의 수행시간: %f초\n", duration);

	start = clock();
	insertion_sort(list3, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("삽입정렬의 수행시간: %f초\n", duration);

	start = clock();
	shell_sort(list4, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("쉘정렬의 수행시간: %f초\n", duration);

	start = clock();
	heap_sort(list5, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("히프정렬의 수행시간: %f초\n", duration);

	start = clock();
	quick_sort(list6, 0, n - 1);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("퀵정렬의 수행시간: %f초\n", duration);

	start = clock();
	merge_sort(list7, 0, n - 1);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("합병정렬의 수행시간: %f초\n", duration);

	start = clock();
	radix_sort(list8, n);
	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("래딕스정렬의 수행시간: %f초\n", duration);

	return 0;
}