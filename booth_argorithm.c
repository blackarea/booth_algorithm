//booth algorithm 4비트용 
#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define MINUS 1
#define PLUS 0

int* booth(int a, int b);
void printbinary(int A, int Q, int Q1, int M);
int* decimaltobinary(int deci); //10진수를 2진수로 바꿔주는 함수
int binarytodecimal(int* binary); //2진수를 10진수로 바꿔주는 함수
int* combine(int A, int Q); //4비트 2진수 2개를 8비트로 바꿔주는 함수

struct Flags { //구조체 비트필드
	signed int A : 5;     // A는 -16~15
	unsigned int Q : 4;   // Q는 0~15
	unsigned int Q1 : 1;  // Q1는 0~1
	signed int M : 5;     // M는 -16~15
	unsigned int s : 4;   // 더미 (int i 같은존재)
};

int main() {

	int* arr = (int*)malloc(sizeof(int) * 8); //결과로 나온 2진수 2개를 하나로 합치기 위해 만든 배열
	int i, result = 0;
	int first, second; //곱셈할 두개의 수

	printf(" 7 ~ -8사이의 곱셈을 할 두 수를 입력하세요\n");
	printf("첫 번째 수(Q): \n");
	scanf_s("%d", &first);
	printf("두 번째 수(M) : \n");
	scanf_s("%d", &second);

	arr = booth(first, second); //arr은 8비트 2진수
	result = binarytodecimal(arr); //결과를 10진수로 변환
	printf("\n");
	printf("결과는 %d입니다.\n ", result);

	free(arr);

	return 0;

}

int* booth(int a, int b) {
	struct Flags f;
	int arr[4]; //Q의 2의 0승 자리수를 구하기위해 만든 배열
	int four, i;
	int* arr2 = (int*)malloc(sizeof(int) * 8);
	f.Q = a;
	f.M = b;
	f.A = 0;
	f.Q1 = 0;

	printf(" A   Q   Q1  M\n");
	printbinary(f.A, f.Q, f.Q1, f.M);
	printf("\n");
	for (four = 0; four < 4; four++) {
		printf("-------------------------");
		f.s = f.Q;
		for (i = 0; f.s > 0; i++) { //Q의 2의0승 자리수 구하기
			arr[i] = f.s % 2;
			f.s /= 2;
		}
		if (f.Q == 0) { //만약 Q가 0이면 위 for문이 실행 안되므로
			arr[0] = 0;
		}


		if (arr[0] == f.Q1) { // (0 0)상황
			f.s = f.A; //f.s는 더미

			if (f.Q % 2 == 1) //Q의 2의0승자리가 1이면
				f.Q1 = 1; //Q1은 1
			else
				f.Q1 = 0; //Q1은 0

			f.Q >>= 1;
			if (f.s % 2 == 1) { //A의 2의 0승 자리가 1이면
				f.Q += 8; //Q의 2의3승 자리는 1이 된다 ex)-1 (1111) +8 -> (0111)
			}

			f.A >>= 1;
		}
		else if (arr[0] == 1 && f.Q1 == 0) { // (1 0)상황
			f.A -= f.M;
			printbinary(f.A, f.Q, f.Q1, f.M);
			printf("  A=A-M");
			f.s = f.A;

			if (f.Q % 2 == 1)
				f.Q1 = 1;
			else
				f.Q1 = 0;

			f.Q >>= 1;
			if (f.s % 2 == 1) {
				f.Q += 8;
			}

			f.A >>= 1;
		}
		else if (arr[0] == 0 && f.Q1 == 1) { //(0 1) 상황
			f.A += f.M;
			printbinary(f.A, f.Q, f.Q1, f.M);
			printf("  A=A+M");
			f.s = f.A;
			if (f.Q % 2 == 1)
				f.Q1 = 1;
			else
				f.Q1 = 0;

			f.Q >>= 1;
			if (f.s % 2 == 1) {
				f.Q += 8;
			}

			f.A >>= 1;
		}

		printbinary(f.A, f.Q, f.Q1, f.M);
		printf("  Shift");
		printf("\n");
	}
	arr2 = combine(f.A, f.Q);

	return arr2;
}

void printbinary(int A, int Q, int Q1, int M) {
	int i;
	int size = 4;
	int* binary = (int*)malloc(sizeof(int) * size);

	printf("\n");
	binary = decimaltobinary(A);
	for (i = size - 1; i >= 0; i--)
		printf("%d", binary[i]);
	printf(" ");
	binary = decimaltobinary(Q);
	for (i = size - 1; i >= 0; i--)
		printf("%d", binary[i]);
	printf(" ");
	printf("%d", Q1);
	printf(" ");
	binary = decimaltobinary(M);
	for (i = size - 1; i >= 0; i--)
		printf("%d", binary[i]);

	free(binary);
}

int* decimaltobinary(int dec) {
	int i;
	int sign = PLUS;
	int size = 4;
	int* binary = (int*)malloc(sizeof(int) * size);

	if (dec < 0) {
		sign = MINUS;
		dec *= -1;
	}
	for (i = 0; i < size; ++i) {
		binary[i] = dec % 2; //2로 나눴을때 짝수면 0 홀수면 1
		dec >>= 1; // 하나씩 쉬프트 -> ex) 11은 1011 -> 101 -> 10 -> 1 이렇게 오른쪽으로 쉬프트니까 역순입력임
	}

	if (sign == MINUS) { // 음수면 2의보수
		for (i = 0; i < size; ++i) {
			(binary[i] == 0) ? (binary[i] = 1) : (binary[i] = 0);
		}
		binary[0] += 1; //2의보수 +1

		for (i = 0; i < size - 1; ++i) { //최상위비트는 부호비트
			if (binary[i] == 2) { //2의보수 +1로 인해 2의 0승 자리가 2가 되면
				binary[i] = 0;
				binary[i + 1] += 1; //1씩 더해줌
			}
			else {
				break;
			}
		}
	}

	return binary;
}

int binarytodecimal(int* binary) {
	int i, dec = 0, size = 8;
	int sign = PLUS, position = 0;
	if (binary[0] == MINUS)
		sign = MINUS;
	if (sign == MINUS) { //음수면 2의보수
		for (i = 0; i < size; ++i) {
			(binary[i] == 0) ? (binary[i] = 1) : (binary[i] = 0);
		}
		binary[7] += 1; //2의보수 +1

		for (i = 8 - 1; i >= 0; i--) { //최상위비트는 부호비트
			if (binary[i] == 2) {
				binary[i] = 0;
				binary[i - 1] += 1;
			}
			else {
				break;
			}
		}

	}

	for (i = 8 - 1; i >= 0; i--)
	{
		if (binary[i] == 1)
			dec += 1 << position;

		position++;
	}
	if (sign == MINUS)
		dec = -dec;
	return dec;
}

int* combine(int A, int Q) {
	int i, size = 8;

	int* arr1 = (int*)malloc(sizeof(int) * size / 2);
	int* arr2 = (int*)malloc(sizeof(int) * size);

	arr1 = decimaltobinary(A);

	for (i = 0; i < 4; i++) {
		arr2[3 - i] = arr1[i];
	}

	arr1 = decimaltobinary(Q);
	for (i = 0; i < 4; i++) {
		arr2[7 - i] = arr1[i];
	}

	free(arr1);
	return arr2;
}