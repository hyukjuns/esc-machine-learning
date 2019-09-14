/*������ �ڵ�
cmdâ�� ���α׷��� ���ϸ� ���Ȯ�ο����ϸ�*/

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

double s(double u); //�ñ׸��̵��Լ�
void initwh(double wh[HIDDENNO][INPUTNO + 1]);// �߰��� ����ġ �ʱ�ȭ
void initwo(double wo[HIDDENNO + 1]);// ����� ����ġ �ʱ�ȭ
double drnd(void);//��������
void print(double wh[HIDDENNO][INPUTNO + 1], double wo1[HIDDENNO + 1], double wo2[HIDDENNO + 1], double wo3[HIDDENNO + 1]);//������
double forward(double wh[HIDDENNO][INPUTNO], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1]);//������ ���
void olearn(double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output); //����� ����ġ ����
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


	/*�����ʱ�ȭ*/
	srand(SEED);

	/*����ġ�ʱ�ȭ*/
	initwh(wh);
	initwo(wo1);
	initwo(wo2);
	initwo(wo3);
	print(wh, wo1, wo2, wo3);

	/*�н� ������ �б�*/
	n_of_e = getdata(argv,e);
	printf("�н� ������ ����:%d\n", n_of_e);

	/*�н�*/
	while (err1 > LIMIT || err2 > LIMIT || err3 > LIMIT)
	{
		err1 = 0.0, err2 = 0.0, err3 = 0.0;
		for (j = 0; j < n_of_e; ++j) {
			/*��������*/
			o1 = forward(wh, wo1, hi, e[j]);
			o2 = forward(wh, wo2, hi, e[j]);
			o3 = forward(wh, wo3, hi, e[j]);
			/*����� ����ġ ����*/
			olearn(wo1, hi, e[j], o1, output1);
			olearn(wo2, hi, e[j], o2, output2);
			olearn(wo3, hi, e[j], o3, output3);
			/*�߰��� ����ġ ����*/
			hlearn(wh, wo1, hi, e[j], o1, output1);
			hlearn(wh, wo2, hi, e[j], o2, output2);
			hlearn(wh, wo3, hi, e[j], o3, output3);
			/*���� ����*/
			err1 += (o1 - e[j][INPUTNO+output1])*(o1 - e[j][INPUTNO+output1]);
			err2 += (o2 - e[j][INPUTNO+output2])*(o2 - e[j][INPUTNO+output2]);
			err3 += (o3 - e[j][INPUTNO+output3])*(o3 - e[j][INPUTNO+output3]);
		}
		++count;
		/*�������*/
		printf("%d\t%lf", count, err1);
		printf("%d\t%lf", count, err2);
		printf("%d\t%lf", count, err3);
	}/*�н� ����*/

	 /*���ᰭ�� ���*/
	print(wh, wo1,wo2,wo3);
	/*�н� ������ ��*/
	for (i = 0; i<n_of_e; ++i) {
		fprintf(fp, "�н������ͼ��� = %d \n", i);
		for (j = 0; j < INPUTNO + 3; ++j)
			fprintf(fp, " ������ = %lf, ", e[i][j]);
		o1 = forward(wh, wo1, hi, e[i]);
		o2 = forward(wh, wo2, hi, e[i]);
		o3 = forward(wh, wo3, hi, e[i]);
		if (o1 <= 1 && o1 >= 0.9 && o2 < 0.1 && o3 < 0.1)
			cnt++;
		if (o2 <= 1 && o2 >= 0.9 && o1 < 0.1 && o3 < 0.1)
			cnt++;
		if (o3 <= 1 && o3 >= 0.9 && o1 < 0.1 && o2 < 0.1)
			cnt++;
		fprintf(fp, "���1 %lf\n", o1);
		fprintf(fp, "���2 %lf\n", o2);
		fprintf(fp, "���3 %lf\n", o3);
	}
	fclose(fp);
	printf("��Ȯ�� : %f", ((double)cnt / 75)*100.0);
	return 0;
}
/*hlearn() �Լ�
�߰��� ����ġ �н�*/

void hlearn(double wh[HIDDENNO][INPUTNO + 1], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output)
{
	int i, j;
	double dj;

		for (j = 0; j < HIDDENNO; ++j) { /*�߰��� �� �� j�� ���*/
			dj = hi[j] * (1 - hi[j])*wo[j] * (e[INPUTNO+output] - o)*o*(1 - o);
		for (i = 0; i < INPUTNO; i++)/*i��° ����ġ ó��*/
			wh[j][i] += ALPHA*e[i] * dj;
		wh[j][i] += ALPHA*(-1)*dj;/*�Ӱ�ġ �н�*/
	}
}

/* getdata()�Լ�
�н� ������ �б�*/
int getdata(char *argv[], double e[][INPUTNO + 3])
{
	int n_of_e = 0; // ������ ��Ʈ ����
	int i, j = 0;
	double ch;
	FILE *fp1 = NULL;
	//FILE *fp2 = NULL;
	fp1 = fopen(argv[1], "r");
	//fp2 = fopen(argv[2], "w");
	if (fp1 == NULL)
		printf("�����͹���");
	/*if (fp2 == NULL)
		printf("�����͹���");*/
	/*������ �Է�*/
	printf("�Է½���!");
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
	printf("�Է³�!");
	fclose(fp1);
	//fclose(fp2);
	return n_of_e;
}

/* olearn()�Լ�
����� ����ġ �н�*/
void olearn(double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1], double o, int output)
{
	int i;
	double d;

	d = (e[INPUTNO+output] - o)*o*(1 - o); //�������
	for (i = 0; i < HIDDENNO; ++i) {
		wo[i] += ALPHA*hi[i] * d; //���ᰭ�� �н�
	}
	wo[i] += ALPHA*(-1.0)*d; //�Ӱ�ġ �н�
}

/* forward()�Լ�
��������*/
double forward(double wh[HIDDENNO][INPUTNO], double wo[HIDDENNO + 1], double hi[], double e[INPUTNO + 1])
{
	int i, j;
	double u; //�����հ��
	double o; //��°��

			  /*hi ���*/
	for (i = 0; i < HIDDENNO; ++i) {
		u = 0;
		for (j = 0; j < INPUTNO; ++j)
			u += e[j] * wh[i][j]; // �������� ���Ѵ�
		u -= wh[i][j]; // �Ӱ�ġ ó��
		hi[i] = s(u);
	}
	/*��� o���*/
	o = 0;
	for (i = 0; i < HIDDENNO; ++i)
		o += hi[i] * wo[i];
	o -= wo[i];//�Ӱ�ġ ó��
	return s(o);
}

/*print �Լ� ������*/
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
/*initwh()�Լ�
�߰��� ����ġ �ʱ�ȭ*/
void initwh(double wh[HIDDENNO][INPUTNO + 1])
{
	int i, j;
	/*������ ���� ���ᰭ�� ����*/
	for (i = 0; i < HIDDENNO; ++i)
		for (j = 0; j < INPUTNO + 1; ++j)
			wh[i][j] = drnd();
}
/*initwo()�Լ�
����� ����ġ �ʱ�ȭ*/
void initwo(double wo[HIDDENNO + 1])
{
	int i;

	for (i = 0; i < HIDDENNO + 1; ++i)
		wo[i] = drnd();
}
/*drnd() �Լ�
���� ����*/
double drnd(void)
{
	double rndno;//������ ����

	while ((rndno = (double)rand() / RAND_MAX) == 1.0);
	rndno = rndno * 2 - 1; /*-1 ~ +1 ���� ���� ����*/
	return rndno;
}
/*s()�Լ� �ñ׸��̵� �Լ�*/
double s(double u)
{
	return 1.0 / (1.0 + exp(-u));
}
