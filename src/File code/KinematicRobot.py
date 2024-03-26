from math import *

def Forward_Kinematic(t1,t2,t3,L1,L2,L3):
    t1=(t1/180)*3.14
    t2=(t2/180)*3.14
    t3=(t3/180)*3.14
    c1 = cos(radians(t1))
    s1 = sin(radians(t1))
    c2 = cos(radians(t2))
    s2 = sin(radians(t2))
    c23 = cos(radians(t2+t3))
    s23 = sin(radians(t2+t3))
    d1=250
    d3=55
    Px=L1*cos(t1) + d3*sin(t1) + L2*cos(t1)*cos(t2) + L3*cos(t1)*cos(t2)*cos(t3) - L3*cos(t1)*sin(t2)*sin(t3)
    Py=L1*sin(t1) - d3*cos(t1) + L2*cos(t2)*sin(t1) + L3*cos(t2)*cos(t3)*sin(t1) - L3*sin(t1)*sin(t2)*sin(t3)
    Pz=d1 + L3*sin(t2 + t3) + L2*sin(t2)
    Px = round(Px,3)
    Py = round(Py,3)
    Pz = round(Pz,3)
    return Px,Py,Pz

def Inverse_Kinematic(Px,Py,Pz,L1,L2,L3,theta):
    d1=250
    d2=0
    d3=55
    L1 = 114
    L2 = 162
    L3 = 130
    #theta=(theta/180)*3.14
    a1 = sin(radians(theta))
    print("theta=")
    print(theta)
    print("a1=")
    print(a1)
    h=(Pz-d1-L3*a1)/L2
    print("h=") 
    print(h)
    if h<0:
        h=h
        #print(h)
    if ((Px<0)&(Py<0)):
     theta1 = asin(d3/(sqrt(Px*Px+Py*Py)))-acos(Px/sqrt((Px*Px+Py*Py)))
    else:
     theta1 = asin(d3/(sqrt(Px*Px+Py*Py)))-asin(-Py/sqrt((Px*Px+Py*Py)))
    #theta1 = atan2(Py,Px)
    theta1 = degrees(theta1)
    theta2 = asin(h)
    theta2 = degrees(theta2)
    theta3 = theta - theta2
    theta1 = round(theta1,2)
    theta2 = round(theta2,2)
    theta3 = round(theta3,2)
    return theta1,theta2,theta3

# FK = Forward_Kinematic(120,12,-12,185,180,81)
# print(FK)
# IK = Inverse_Kinematic(-115.98, 200.884, 283.035,185,180,81,0)
 #print("hello")


