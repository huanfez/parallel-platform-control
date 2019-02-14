#include<iostream>
#include<cmath>
#include"vector3.h"

Vector3 operator +(Vector3 c1,Vector3 c2) //向量加法
{
	return Vector3(c1.a+c2.a,c1.b+c2.b,c1.c+c2.c);
}

Vector3 operator -(Vector3 c1,Vector3 c2) //向量减法
{
	return Vector3(c1.a-c2.a,c1.b-c2.b,c1.c-c2.c);
}

double operator *(Vector3 c1,Vector3 c2) //向量乘法
{
	return (c1.a*c2.a+c1.b*c2.b+c1.c*c2.c);
}


double Vector3::abs3() //求向量的模
{
	return (sqrt(a*a+b*b+c*c));
}

void Vector3::display()
{
	cout<<"("<<a<<","<<b<<","<<c<<")"<<endl;
}


Vector3 mul(double n,Vector3 c1)
{
	return Vector3(n*c1.a,n*c1.b,n*c1.c);
}

Vector3 faxiangliang(Vector3 c1,Vector3 c2,Vector3 c3) //已知三个点坐标求法向量
{
	double A = c1.b * c2.c + c2.b * c3.c + c3.b * c1.c - c1.b * c3.c - c2.b * c1.c - c3.b * c2.c;
	double B = -(c1.a * c2.c + c2.a * c3.c + c3.a * c1.c - c3.a * c2.c - c2.a * c1.c - c1.a * c3.c);
	double C = c1.a * c2.b + c2.a * c3.b + c3.a * c1.b - c1.a * c3.b - c2.a * c1.b - c3.a * c2.b;
	double D = -(c1.a * c2.b * c3.c + c2.a * c3.b * c1.c + c3.a * c1.b * c2.c - c1.a * c3.b * c2.c - c2.a * c1.b * c3.c - c3.a * c2.b * c1.c);
	double E = sqrt(A * A + B * B + C * C);
	return Vector3(A/E,B/E,C/E);
}

Vector3 m3(Vector3 c1,Vector3 c2,Vector3 c3)//求单位向量m
{
	Vector3 n=mul(0.5,(c1+c2));
	Vector3 m=mul((1/3),(c1+c2+c3));
	Vector3 temp=m-n;
	double t0=1/(temp.abs3());
	Vector3 temp1=mul(t0,temp);
	return temp1;
}

double ang(double a1)
{
	return(a1*3.141592653/180);
}

double *forward_kenematics(double L1,double L2,double q1,double q2,double q3,double q4,double q5,double q6)//正向运动学
{
	cout<<"L1="<<L1<<" L2="<<L2<<" θ1="<<q1<<"° θ2="<<q2<<"° θ3="<<q3<<"° θ4="<<q4<<"° θ5="<<q5<<"° θ6="<<q6<<"°"<<endl;
	double c30=0.8660254;
	double R=10;
	Vector3 a0(-c30*R,-0.5*R,0);
	Vector3 b0(c30*R,-0.5*R,0);
	Vector3 c0(0,R,0);
	//cout<<"OA 0:";//测试用
	//a0.display();
	//cout<<"OB 0:";
	//b0.display();
	//cout<<"OC 0:";
	//c0.display();
	Vector3 a1(-c30*R-c30*L1*cos(ang(q1)),-0.5*R-0.5*L1*cos(ang(q1)),L1*sin(ang(q1)));
	Vector3 b1(c30*R+c30*L1*cos(ang(q2)),-0.5*R-0.5*L1*cos(ang(q2)),L1*sin(ang(q2)));
	Vector3 c1(0,R+L1*cos(ang(q3)),L1*sin(ang(q3)));
	//cout<<"OA 1:";
	//a1.display();
	//cout<<"OB 1:";
	//b1.display();
	//cout<<"OC 1:";
	//c1.display();
	Vector3 a3=a1-a0;
	Vector3 b3=b1-b0;
	Vector3 c3=c1-c0;
	//cout<<"OA 3:";
	//a3.display();
	//cout<<"OB 3:";
	//b3.display();
	//cout<<"OC 3:";
	//c3.display();
	Vector3 a3b3=b3-a3;
	Vector3 a3c3=c3-a3;
	Vector3 b3c3=c3-b3;
	//cout<<"A3B3:";
	//a3b3.display();
	//cout<<"A3C3:";
	//a3c3.display();
	//cout<<"B3C3:";
	//b3c3.display();
	double temp0=1/(a3c3.abs3()*b3c3.abs3());
	double temp1=a3c3*b3c3;
	double q=acos(temp0*temp1);
	//cout<<"角度φ:";
	//cout<<q*180/3.141592653<<"°"<<endl;
	Vector3 on=mul(0.5,(a3+b3));//ON用on表示
	//cout<<"ON:";
	//on.display();
	Vector3 m=m3(a3,b3,c3);
	//cout<<"MN单位向量m:";
	//m.display();
	double temp2=1/(2*sin(q));//这里的角度是后面算出来的，本来就是弧度
	double rc=temp2*(a3b3.abs3());
	//cout<<"RC:"<<rc<<endl;
	double temp3=sqrt(rc*rc-a3b3.abs3()*a3b3.abs3()/4);
	Vector3 om=mul(temp3,m)+on;//OM用om表示
	//cout<<"OM:";
	//om.display();
	Vector3 n=faxiangliang(a3,b3,c3);//a3b3c3的法向量
	//cout<<"A3B3C3的单位法向量n:";
	//n.display();
	Vector3 a3m=om-a3;
	//cout<<"A3M:";
	//a3m.display();
	double temp4=sqrt(L2*L2-a3m.abs3()*a3m.abs3());
	//cout<<"根号里面的"<<temp4<<endl;
	Vector3 p=mul(temp4,n)+om;
	cout<<"OP:";
	//p.display();
	double *position = (double*)malloc(6*sizeof(double));
	position[0]=p.a, position[1]=p.b, position[2]=p.c, position[3]=ang(q4), position[4]=ang(q5), position[5]=ang(q6);
	return position;
}

double **inverce_kinematics(double L1,double L2,double xp,double yp,double zp,double xr,double yr,double zr)//逆向运动学
{
	double t1=sqrt(4*L1*L1*(yp*yp+zp*zp)-(xp*xp+yp*yp+zp*zp+L1*L1-L2*L2)*(xp*xp+yp*yp+zp*zp+L1*L1-L2*L2));
	double t2=yp*yp+zp*zp;
	double t3=xp*xp-yp*yp+zp*zp-L1*L1+L2*L2;
	double j11=2*xp/t1;            //第一行
	double j12=t3*yp/t1/t1-zp/t2;
	double j13=t3*zp/t1/t2+yp/t2;  //
	double t4=3*xp*xp+yp*yp+4*zp*zp-2*sqrt(3.0)*xp*yp;
	double t5=xp*xp+yp*yp+zp*zp+L1*L1-L2*L2;
	double t6=sqrt(3.0)*xp-yp;
	double t7=t6*t6;
	double t8=t7+4*zp*zp;
	double t9=L1*L1*t8;
	double t10=sqrt(t9-t5*t5);
	double j21=(t4*2*xp-t5*sqrt(3.0)*t6)/(t8*t10)-2*sqrt(3.0)*zp/t8;//第二行
	double j22=(t4*2*yp+t5*t6)/(t8*t10)+2*zp/t8;
	double j23=(t4*2*zp-t5*4*zp)/(t8*t10)+2*t6/t8;                  //
	double t11=3*xp*xp+yp*yp+4*zp*zp+2*sqrt(3.0)*xp*yp;
	double t12=sqrt(3.0)*xp+yp;
	double t13=t12*t12+4*zp*zp;
	double t14=sqrt(L1*L1*t13-t5*t5);
	double j31=(t11*2*xp-t5*sqrt(3.0)*t12)/(t13*t14)+2*sqrt(3.0)*zp/t13;//第三行
	double j32=(t11*2*yp-t5*t12)/(t13*t14)+2*zp/t13;
	double j33=(t11*2*zp-t5*4*zp)/(t13*t14)-2*t12/t13;              //
	Vector3 j1(j11,j12,j13);
	Vector3 j2(j21,j22,j23);
	Vector3 j3(j31,j32,j33);
	//j1.display();
	//j2.display();
	//j3.display();

	double **JacobianMarix=(double**) malloc(sizeof(double*)*3);
	for(int numi=0;numi<3;++numi)
	{
		JacobianMarix[numi] = (double*)malloc(3*sizeof(double));
	}
	JacobianMarix[0][0]=j11, JacobianMarix[0][1]=j12, JacobianMarix[0][2]=j13;
	JacobianMarix[1][0]=j21, JacobianMarix[1][1]=j22, JacobianMarix[1][2]=j23;
	JacobianMarix[2][0]=j31, JacobianMarix[2][1]=j32, JacobianMarix[2][2]=j33;
	return JacobianMarix;
}