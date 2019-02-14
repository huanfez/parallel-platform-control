#include<cmath>
using namespace std;
class Vector3
{
public:
	double abs3(); //��������ģ
	void display();
	Vector3() {a=0;b=0;c=0;}
	Vector3(double one,double two,double three){a=one;b=two;c=three;}
	friend Vector3 operator +(Vector3 c1,Vector3 c2);//�����ӷ�
	friend Vector3 operator -(Vector3 c1,Vector3 c2);//��������
	friend double operator *(Vector3 c1,Vector3 c2);//�����˷�
	double a;
	double b;
	double c;
};

Vector3 faxiangliang(Vector3 c1,Vector3 c2,Vector3 c3);//��֪����������������
Vector3 m3(Vector3 c1,Vector3 c2,Vector3 c3);//��λ����m
Vector3 mul(double n,Vector3 c2);
double ang(double a1);
double *forward_kenematics(double L1,double L2,double q1,double q2,double q3,double q4,double q5,double q6);//�����˶�ѧ
double **inverce_kinematics(double L1,double L2,double xp,double yp,double zp,double xr,double yr,double zr);//�����˶�ѧ