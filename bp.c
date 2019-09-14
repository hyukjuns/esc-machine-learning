/*역전파 코드
cmd창에 프로그램명 파일명 결과확인용파일명*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTNO 4
#define HIDDENNO 3
#define ALPHA	0.3
#define SEED	65535
#define MAXINPUTNO	100
#define BIGNUM	100
#define LIMIT	0.01

double s(double u); //시그모이드함수
void initwh(double wh[HIDDENNO][INPUTNO + 1]);// 중간층 가중치 초기화
void initwo(double wo[HIDDENNO + 1]);// 출력층 가중치 초기화
double drnd(void);//난수생성
void print(double wh[HIDDENNO][INPUTNO + 1], double wo1[HIDDENNO + 1], double wo2[HIDDENNO + 1], double wo3[HIDDENNO + 1]);//결과출력
double forward(double wh[HIDDENNO][INPUTNO], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1]);//순방향 계산
void olearn(double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output); //출력층 가중치 조정
int getdata(char *argv[], double e[][INPUTNO + 3]);
void hlearn(double wh[HIDDENNO][INPUTNO + 1], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output);

int main(int argc, char *argv[])
{
	double wh[HIDDENNO][INPUTNO + 1];
	double wo1[HIDDENNO + 1];
	double wo2[HIDDENNO + 1];
	double wo3[HIDDENNO + 1];
	double e[MAXINPUTNO][INPUTNO + 3]; //x0,x1,x2,x3,o1,o2,o3
	double hi[HIDDENNO + 1];
	double o1,o2,o3;
	double err1 = BIGNUM;
	double err2 = BIGNUM;
	double err3 = BIGNUM;
	int i, j;
	int n_of_e;
	int count = 0;
	int output1=0, output2=1, output3=2;
	int cnt = 0;
	FILE *fp = fopen(argv[2], "w");


	/*난수초기화*/
	srand(SEED);

	/*가중치초기화*/
	initwh(wh);
	initwo(wo1);
	initwo(wo2);
	initwo(wo3);
	print(wh, wo1, wo2, wo3);

	/*학습 데이터 읽기*/
	n_of_e = getdata(argv,e);
	printf("학습 데이터 개수:%d\n", n_of_e);

	/*학습*/
	while (err1 > LIMIT || err2 > LIMIT || err3 > LIMIT)
	{
		err1 = 0.0, err2 = 0.0, err3 = 0.0;
		for (j = 0; j < n_of_e; ++j) {
			/*순방향계산*/
			o1 = forward(wh, wo1, hi, e[j]);
			o2 = forward(wh, wo2, hi, e[j]);
			o3 = forward(wh, wo3, hi, e[j]);
			/*출력층 가중치 조정*/
			olearn(wo1, hi, e[j], o1, output1);
			olearn(wo2, hi, e[j], o2, output2);
			olearn(wo3, hi, e[j], o3, output3);
			/*중간층 가중치 조정*/
			hlearn(wh, wo1, hi, e[j], o1, output1);
			hlearn(wh, wo2, hi, e[j], o2, output2);
			hlearn(wh, wo3, hi, e[j], o3, output3);
			/*오차 적산*/
			err1 += (o1 - e[j][INPUTNO+output1])*(o1 - e[j][INPUTNO+output1]);
			err2 += (o2 - e[j][INPUTNO+output2])*(o2 - e[j][INPUTNO+output2]);
			err3 += (o3 - e[j][INPUTNO+output3])*(o3 - e[j][INPUTNO+output3]);
		}
		++count;
		/*오차출력*/
		printf("%d\t%lf", count, err1);
		printf("%d\t%lf", count, err2);
		printf("%d\t%lf", count, err3);
	}/*학습 종료*/

	 /*연결강도 출력*/
	print(wh, wo1,wo2,wo3);
	/*학습 데이터 예*/
	for (i = 0; i<n_of_e; ++i) {
		fprintf(fp, "학습데이터순서 = %d \n", i);
		for (j = 0; j < INPUTNO + 3; ++j)
			fprintf(fp, " 데이터 = %lf, ", e[i][j]);
		o1 = forward(wh, wo1, hi, e[i]);
		o2 = forward(wh, wo2, hi, e[i]);
		o3 = forward(wh, wo3, hi, e[i]);
		if (o1 <= 1 && o1 >= 0.9 && o2 < 0.1 && o3 < 0.1)
			cnt++;
		if (o2 <= 1 && o2 >= 0.9 && o1 < 0.1 && o3 < 0.1)
			cnt++;
		if (o3 <= 1 && o3 >= 0.9 && o1 < 0.1 && o2 < 0.1)
			cnt++;
		fprintf(fp, "출력1 %lf\n", o1);
		fprintf(fp, "출력2 %lf\n", o2);
		fprintf(fp, "출력3 %lf\n", o3);
	}
	fclose(fp);
	printf("정확도 : %f", ((double)cnt / 75)*100.0);
	return 0;
}
/*hlearn() 함수
중간층 가중치 학습*/

void hlearn(double wh[HIDDENNO][INPUTNO + 1], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output)
{
	int i, j;
	double dj;

		for (j = 0; j < HIDDENNO; ++j) { /*중간층 각 셀 j를 대상*/
			dj = hi[j] * (1 - hi[j])*wo[j] * (e[INPUTNO+output] - o)*o*(1 - o);
		for (i = 0; i < INPUTNO; i++)/*i번째 가중치 처리*/
			wh[j][i] += ALPHA*e[i] * dj;
		wh[j][i] += ALPHA*(-1)*dj;/*임계치 학습*/
	}
}

/* getdata()함수
학습 데이터 읽기*/
int getdata(char *argv[], double e[][INPUTNO + 3])
{
	int n_of_e = 0; // 데이터 세트 개수
	int i, j = 0;
	double ch;
	FILE *fp1 = NULL;
	//FILE *fp2 = NULL;
	fp1 = fopen(argv[1], "r");
	//fp2 = fopen(argv[2], "w");
	if (fp1 == NULL)
		printf("포인터문제");
	/*if (fp2 == NULL)
		printf("포인터문제");*/
	/*데이터 입력*/
	printf("입력시작!");
	for(i=0;i<75;i++) {
		for (j = 0; j < 7; j++) {
			fscanf(fp1, "%lf,", &e[i][j]);
		}
		++n_of_e;
	}
	/*for (i = 0; i < 75; i++) {
		for (j = 0; j < 7; j++)
			fprintf(fp2, "%f ", e[i][j]);
		fprintf(fp2, "\n");
	}*/
	printf("입력끝!");
	fclose(fp1);
	//fclose(fp2);
	return n_of_e;
}

/* olearn()함수
출력층 가중치 학습*/
void olearn(double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output)
{
	int i;
	double d;

	d = (e[INPUTNO+output] - o)*o*(1 - o); //오차계산
	for (i = 0; i < HIDDENNO; ++i) {
		wo[i] += ALPHA*hi[i] * d; //연결강도 학습
	}
	wo[i] += ALPHA*(-1.0)*d; //임계치 학습
}

/* forward()함수
순방향계산*/
double forward(double wh[HIDDENNO][INPUTNO], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1])
{
	int i, j;
	double u; //가중합계산
	double o; //출력계산

			  /*hi 계산*/
	for (i = 0; i < HIDDENNO; ++i) {
		u = 0;
		for (j = 0; j < INPUTNO; ++j)
			u += e[j] * wh[i][j]; // 가중합을 구한다
		u -= wh[i][j]; // 임계치 처리
		hi[i] = s(u);
	}
	/*출력 o계산*/
	o = 0;
	for (i = 0; i < HIDDENNO; ++i)
		o += hi[i] * wo[i];
	o -= wo[i];//임계치 처리
	return s(o);
}

/*print 함수 결과출력*/
void print(double wh[HIDDENNO][INPUTNO + 1], double wo1[HIDDENNO + 1], double wo2[HIDDENNO + 1], double wo3[HIDDENNO + 1])
{
	int i, j;

	for (i = 0; i < HIDDENNO; ++i)
		for (j = 0; j < INPUTNO + 1; ++j)
			printf("%lf", wh[i][j]);
	printf("\n");
	for (i = 0; i < HIDDENNO + 1; ++i)
		printf("%lf", wo1[i]);
	printf("\n");
	for (i = 0; i < HIDDENNO + 1; ++i)
		printf("%lf", wo2[i]);
	printf("\n");
	for (i = 0; i < HIDDENNO + 1; ++i)
		printf("%lf", wo3[i]);
	printf("\n");
}
/*initwh()함수
중간층 가중치 초기화*/
void initwh(double wh[HIDDENNO][INPUTNO + 1])
{
	int i, j;
	/*난수에 의한 연결강조 결정*/
	for (i = 0; i < HIDDENNO; ++i)
		for (j = 0; j < INPUTNO + 1; ++j)
			wh[i][j] = drnd();
}
/*initwo()함수
출력층 가중치 초기화*/
void initwo(double wo[HIDDENNO + 1])
{
	int i;

	for (i = 0; i < HIDDENNO + 1; ++i)
		wo[i] = drnd();
}
/*drnd() 함수
난수 생성*/
double drnd(void)
{
	double rndno;//생성한 난수

	while ((rndno = (double)rand() / RAND_MAX) == 1.0);
	rndno = rndno * 2 - 1; /*-1 ~ +1 사이 난수 생성*/
	return rndno;
}
/*s()함수 시그모이드 함수*/
double s(double u)
{
	return 1.0 / (1.0 + exp(-u));
}
