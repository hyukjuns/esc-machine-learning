/*k_means clustering
 cmd창에 프로그램명 파일명*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#define SIZE 150
#define K 3
typedef struct
{
	double pos1, pos2, pos3, pos4;
	char lable[50];
	double distance;
} Info;
typedef struct
{
	double distInfo;
	char lableInfo[50];
} Dist;

int comp(const void*a, const void*b);
int comp2(const void*a, const void*b);
int main(int argc, char *argv[])
{
	srand(time(NULL));
	char flower1[] = "Iris-setosa";
	char flower2[] = "Iris-versicolor";
	char flower3[] = "Iris-virginica";

	FILE *fp1 = fopen(argv[1], "r");

	Info *info = (Info*)malloc(sizeof(Info)*SIZE);
	Info *cluster1 = (Info*)malloc(sizeof(Info)*SIZE); //군집3개생성
	Info *cluster2 = (Info*)malloc(sizeof(Info)*SIZE);
	Info *cluster3 = (Info*)malloc(sizeof(Info)*SIZE);
	Info centroid[3] = { 0 };
	Dist dist[3] = { 0 };

	int i, j, r;
	int cnt1 = 0, cnt2 = 0, cnt3 = 0;
	int oldcnt1 = 0, oldcnt2 = 0, oldcnt3 = 0;
	int repeat = 0;

	for (i = 0; i < SIZE; i++)
		fscanf(fp1, "%lf,%lf,%lf,%lf,%s", &info[i].pos1, &info[i].pos2, &info[i].pos3, &info[i].pos4, info[i].lable);
	fclose(fp1);
	for (i = 0; i < K; i++)
	{
		j = rand() % 50 + (i * 50);
		centroid[i] = info[j];
		printf("초기 centroid%d 주소 : %d \n", i + 1, j);
		printf("초기 centroid%d = %f %f %f %f %s \n", i + 1, centroid[i].pos1, centroid[i].pos2, centroid[i].pos3, centroid[i].pos4, centroid[i].lable);
	}
	while (repeat <20)
	{
		printf("%d번째 군집 형성시작 => \n", repeat + 1);;
		cnt1 = 0, cnt2 = 0, cnt3 = 0;
		for (i = 0; i < SIZE; i++) //info 0~149 가지
		{
			double sum[K] = { 0 };
			for (j = 0; j < K; j++) //3개의 중심과 비교
			{
				sum[j] = pow(centroid[j].pos1 - info[i].pos1, 2) + pow(centroid[j].pos2 - info[i].pos2, 2) + pow(centroid[j].pos3 - info[i].pos3, 2) + pow(centroid[j].pos4 - info[i].pos4, 2);
				dist[j].distInfo = sum[j];
				strcpy(dist[j].lableInfo, centroid[j].lable); //각 중심과의 거리와 중심의 정보를 dist배열에 저장
			}
			qsort(dist, K, sizeof(Dist), comp);//3개의 중심과의 거리중 가장 가까운 거리 산출
			if (!strcmp(centroid[0].lable, dist[0].lableInfo))//첫번째군집생성
			{
				cluster1[cnt1] = info[i];
				cnt1++;
			}
			else if (!strcmp(centroid[1].lable, dist[0].lableInfo))//두번째군집생성
			{
				cluster2[cnt2] = info[i];
				cnt2++;
			}
			else if (!strcmp(centroid[2].lable, dist[0].lableInfo))//세번째군집생성
			{
				cluster3[cnt3] = info[i];
				cnt3++;
			}
		}
		printf("=> %d 번째 첫번째군집 %d 두번째군집 %d 세번째군집 %d \n ", repeat + 1, cnt1, cnt2, cnt3);
		printf("--------------------------------------------------\n");
		if (oldcnt1 == cnt1&&oldcnt2 == cnt2&&oldcnt3 == cnt3)
		{
			int hcnt1 = 0, hcnt2 = 0, hcnt3 = 0;
			printf("군집변화없음--->종료\n");
			for (i = 0; i < cnt1; i++)
			{
				if (!strcmp(cluster1[i].lable, flower1))	hcnt1++;
			}
			for (i = 0; i < cnt2; i++)
			{
				if (!strcmp(cluster2[i].lable, flower2))	hcnt2++;
			}
			for (i = 0; i < cnt3; i++)
			{
				if (!strcmp(cluster3[i].lable, flower3))	hcnt3++;
			}
			printf("확률: 첫번째군집 = %f, 두번째군집 = %f, 세번째군집 = %f \n", (((double)hcnt1 / 50)*100.0), (((double)hcnt2 / 50)*100.0), (((double)hcnt3 / 50)*100.0));
			break;
		}
		//새로운중심찾기시작
		printf("---새로운 중심 찾기 시작---\n");
		for (i = 0; i < cnt1; i++)
		{
			cluster1[i].distance = 0;
		}
		for (i = 0; i < cnt2; i++)
		{
			cluster2[i].distance = 0;
		}
		for (i = 0; i < cnt3; i++)
		{
			cluster3[i].distance = 0;
		}
		double npos1 = 0, npos2 = 0, npos3 = 0, npos4 = 0;
		for (i = 0; i < cnt1; i++)
		{
			npos1 += cluster1[i].pos1;
			npos2 += cluster1[i].pos2;
			npos3 += cluster1[i].pos3;
			npos4 += cluster1[i].pos4;
		}
		npos1 /= cnt1;
		npos2 /= cnt1;
		npos3 /= cnt1;
		npos4 /= cnt1;
		double newsum[150] = { 0 };
		for (j = 0; j < cnt1; j++)
		{
			newsum[j] = pow(npos1 - cluster1[j].pos1, 2) + pow(npos2 - cluster1[j].pos2, 2) + pow(npos3 - cluster1[j].pos3, 2) + pow(npos4 - cluster1[j].pos4, 2);
			cluster1[j].distance = newsum[j];
		}
		qsort(cluster1, cnt1, sizeof(Info), comp2);//새로운중심과의 거리중 가장 가까운 거리정보 산출
		centroid[0].pos1 = cluster1[0].pos1;
		centroid[0].pos2 = cluster1[0].pos2;
		centroid[0].pos3 = cluster1[0].pos3;
		centroid[0].pos4 = cluster1[0].pos4;
		strcpy(centroid[0].lable, cluster1[0].lable);
		printf("new centroid1 : %f , %f , %f , %f , %s \n", centroid[0].pos1, centroid[0].pos2, centroid[0].pos3, centroid[0].pos4, centroid[0].lable);

		npos1 = 0, npos2 = 0, npos3 = 0, npos4 = 0;
		for (i = 0; i < cnt2; i++)
		{
			npos1 += cluster2[i].pos1;
			npos2 += cluster2[i].pos2;
			npos3 += cluster2[i].pos3;
			npos4 += cluster2[i].pos4;
		}
		npos1 /= cnt2;
		npos2 /= cnt2;
		npos3 /= cnt2;
		npos4 /= cnt2;
		double newsum2[150] = { 0 };
		for (j = 0; j < cnt2; j++)
		{
			newsum2[j] = pow(npos1 - cluster2[j].pos1, 2) + pow(npos2 - cluster2[j].pos2, 2) + pow(npos3 - cluster2[j].pos3, 2) + pow(npos4 - cluster2[j].pos4, 2);
			cluster2[j].distance = newsum2[j];
		}
		qsort(cluster2, cnt2, sizeof(Info), comp2); //새로운중심과의 거리중 가장 가까운 거리정보 산출
		centroid[1].pos1 = cluster2[0].pos1;
		centroid[1].pos2 = cluster2[0].pos2;
		centroid[1].pos3 = cluster2[0].pos3;
		centroid[1].pos4 = cluster2[0].pos4;
		strcpy(centroid[1].lable, cluster2[0].lable);
		printf("new centroid2 : %f , %f , %f , %f , %s \n", centroid[1].pos1, centroid[1].pos2, centroid[1].pos3, centroid[1].pos4, centroid[1].lable);

		npos1 = 0, npos2 = 0, npos3 = 0, npos4 = 0;
		for (i = 0; i < cnt3; i++)
		{
			npos1 += cluster3[i].pos1;
			npos2 += cluster3[i].pos2;
			npos3 += cluster3[i].pos3;
			npos4 += cluster3[i].pos4;
		}
		npos1 /= cnt3;
		npos2 /= cnt3;
		npos3 /= cnt3;
		npos4 /= cnt3;
		double newsum3[150] = { 0 };
		for (j = 0; j < cnt3; j++)
		{
			newsum3[j] = pow(npos1 - cluster3[j].pos1, 2) + pow(npos2 - cluster3[j].pos2, 2) + pow(npos3 - cluster3[j].pos3, 2) + pow(npos4 - cluster3[j].pos4, 2);
			cluster3[j].distance = newsum3[j];
		}
		qsort(cluster3, cnt3, sizeof(Info), comp2); //새로운중심과의 거리중 가장 가까운 거리정보 산출해서 새로운중심점좌표 선정
		centroid[2].pos1 = cluster3[0].pos1;
		centroid[2].pos2 = cluster3[0].pos2;
		centroid[2].pos3 = cluster3[0].pos3;
		centroid[2].pos4 = cluster3[0].pos4;
		strcpy(centroid[2].lable, cluster3[0].lable);
		printf("new centroid3 : %f , %f , %f , %f , %s \n", centroid[2].pos1, centroid[2].pos2, centroid[2].pos3, centroid[2].pos4, centroid[2].lable);
		npos1 = 0, npos2 = 0, npos3 = 0, npos4 = 0;
		printf("---새로운 중심 찾기 끝---\n");
		oldcnt1 = cnt1, oldcnt2 = cnt2, oldcnt3 = cnt3;
		//새로운중심잡기끝
		repeat++;//반복
	}
	free(info);
	free(cluster1);
	free(cluster2);
	free(cluster3);
	return 0;
}
int comp(const void*a, const void*b)
{
	const Dist *x = (const Dist *)a;
	const Dist *y = (const Dist *)b;
	if (x->distInfo < y->distInfo)   return -1;
	else if (x->distInfo > y->distInfo) return 1;
	return 0;
}
int comp2(const void*a, const void*b)
{
	const Info *x = (const Info *)a;
	const Info *y = (const Info *)b;
	if (x->distance < y->distance)   return -1;
	else if (x->distance > y->distance) return 1;
	return 0;
}
