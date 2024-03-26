from ctypes import sizeof #gọi các hàm từ các thư viện khác không thuộc về python
from tkinter import * # thư viện GUI tiêu chuẩn cho Python
import tkinter
from tkinter import font
from typing import Mapping, Sized #thư viện mảng kich thuoc
import numpy as np #thư viện toán học

from serial.serialutil import Timeout #thu vien serial, timeout: limit the max time for calling a function
import KinematicRobot #thu vien dong hoc
import serial # cong serial
import time # thoi gian

import threading #luồng thực thi riêng biệt


def action(): #ham chuyen doi 2 giao dien

    top.destroy() # tat man hinh dau
    GiaoDien.deiconify() #hien thi man hinh giao dien

GiaoDien = Tk() #tao man hinh giao dien
top=Toplevel()  # màn hình hiện thị đầu tiên
top.title("GIAO DIỆN ROBOT 3 DOF") #tên
top.geometry('1070x605') #kích  thước 
MC = PhotoImage(file="C:\\doan\\3-DOF-Robot-Arm-main\\src\\File code\\ROBOT_3DOF.png") #hinh C:\\Users\\84382\\Pictures\\
Label(top,image=MC).place(x=0,y=0)  # vi tri
#MC=PhotoImage(file="VSP.png") # hình
Label(top,image=MC).grid() # vị trí
Button(top,text="GO CONTROL INTERFACE",font='Times 15 bold', bg='#6666ff',fg='white',command=lambda:action()).place(x=400,y=500) # nút nhấn fg mau vien bg mau nen

GiaoDien.withdraw() # de chi hien thi 1 giao dien TOP ban dau
ser = serial.Serial('COM5',9600)

x_axis = 15
y_axis = 30
y_lable_IK = 250
value1 = 0
value2 = 0
value3 = 0
x=0
L1 = 114
L2 = 162
L3 = 130

background_color = '#FFEBCD' # mã TLP RGB xem ở https://htlit.maytinhhtl.com/lam-web/bang-ma-mau-css-html-code-thet-ke-design.html

GiaoDien.geometry('570x600')
GiaoDien.title("CONTROL INTERFACE")
GiaoDien.configure(bg=background_color)

#------------------------------- VIẾT HÀM CON ------------------------------------
def slider_theta1_value(x):
    value1 = slider_theta1.get()
    txb_slider_theta1.delete(0,END) # Xóa dữ liệu trước đó
    txb_slider_theta1.insert(0,value1) # hiển thị dữ liệu mới
    return value1

def slider_theta2_value(x):
    txb_slider_theta2.delete(0,END)
    value2 = slider_theta2.get()
    txb_slider_theta2.insert(0,value2)
    return value2

def slider_theta3_value(x):
    txb_slider_theta3.delete(0,END)
    value3 = slider_theta3.get()
    txb_slider_theta3.insert(0,value3)  
    return value3

def FK(x):
    txb_Px_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Py_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Pz_FK.delete(0,END) # Xóa dữ liệu trước đó
    value1 = slider_theta1_value(x)
    value2 = slider_theta2_value(x)
    value3 = slider_theta3_value(x)
    Px = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[0]
    Py = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[1]
    Pz = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[2]
    txb_Px_FK.insert(0,Px) # hiển thị dữ liệu mới vao PX PY PZ
    txb_Py_FK.insert(0,Py) # hiển thị dữ liệu mới
    txb_Pz_FK.insert(0,Pz) # hiển thị dữ liệu mới
    #mang = str(value1)+'A'+str(value2)+'B'+str(value3)+'C'
    #ser.write(mang.encode())
    #time.sleep(0.01)

def IK():
    txb_theta1_IK.delete(0,END)
    txb_theta2_IK.delete(0,END)
    txb_theta3_IK.delete(0,END)
    Px = float(txb_Px_IK.get())
    Py = float(txb_Py_IK.get())
    Pz = float(txb_Pz_IK.get())
    theta = float(txb_Theta.get())
    theta1 = KinematicRobot.Inverse_Kinematic(Px,Py,Pz,L1,L2,L3,theta)[0]
    theta2 = KinematicRobot.Inverse_Kinematic(Px,Py,Pz,L1,L2,L3,theta)[1]
    theta3 = KinematicRobot.Inverse_Kinematic(Px,Py,Pz,L1,L2,L3,theta)[2]
    txb_theta1_IK.insert(0,theta1)
    txb_theta2_IK.insert(0,theta2)
    txb_theta3_IK.insert(0,theta3)
    mang = f'I{theta1}A{theta2}B{theta3}C'
    print(mang)
    ser.write(mang.encode()) 
    time.sleep(0.01)  #để thêm độ trễ trong quá trình thực thi chương trình. 
    #txb_Px_IK.delete(0,END)
    #txb_Py_IK.delete(0,END)
    #txb_Pz_IK.delete(0,END)
    #txb_Theta.delete(0,END)

def Reset_Slider():
    txb_slider_theta1.delete(0,END)
    txb_slider_theta2.delete(0,END)
    txb_slider_theta3.delete(0,END) 
    txb_slider_theta1.insert(0,value1)
    txb_slider_theta2.insert(0,value2)
    txb_slider_theta3.insert(0,value3)    
    slider_theta1.set('0') #đặt lại vị trí thanh slider tương ứng với giá trị lấy từ textbox
    slider_theta2.set('0')
    slider_theta3.set('0')

def Reset_lable_Slider():
    Reset_Slider()
    Px = KinematicRobot.Forward_Kinematic(0,0,0,L1,L2,L3)[0]
    Py = KinematicRobot.Forward_Kinematic(0,0,0,L1,L2,L3)[1]
    Pz = KinematicRobot.Forward_Kinematic(0,0,0,L1,L2,L3)[2] 

    txb_Px_FK.delete(0,END) # hiển thị dữ liệu mới
    txb_Py_FK.delete(0,END) # hiển thị dữ liệu mới
    txb_Pz_FK.delete(0,END) # hiển thị dữ liệu mới 
    txb_Px_FK.insert(0,Px) # hiển thị dữ liệu mới
    txb_Py_FK.insert(0,Py) # hiển thị dữ liệu mới
    txb_Pz_FK.insert(0,Pz) # hiển thị dữ liệu mới    

def ReSet_btn():
    new_thread = threading.Thread(target=Reset_lable_Slider) # Thread là các hàm hay thủ tục chạy độc lập đối với chương trình chính
    new_thread.start()
    mang = f'F0A0B0C'
    print(mang,type(mang))
    ser.write(mang.encode())
    time.sleep(0.01)

def theta1_set_btn():
    slider_theta1.set(txb_slider_theta1.get())
    value1 = slider_theta1_value(x) # lấy giá trị hiện tại của thanh slider 
    value2 = slider_theta2_value(x)
    value3 = slider_theta3_value(x)
    txb_Px_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Py_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Pz_FK.delete(0,END) # Xóa dữ liệu trước đó
    Px = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[0]
    Py = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[1]
    Pz = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[2]
    txb_Px_FK.insert(0,Px) # hiển thị dữ liệu mới
    txb_Py_FK.insert(0,Py) # hiển thị dữ liệu mới
    txb_Pz_FK.insert(0,Pz) # hiển thị dữ liệu mới
    mang = f'F{value1}A{value2}B{value3}C'
    print(mang,type(mang))
    ser.write(mang.encode())
    time.sleep(0.01)


def theta2_set_btn():
    slider_theta2.set(txb_slider_theta2.get())
    value1 = slider_theta1_value(x) # lấy giá trị hiện tại của thanh slider 
    value2 = slider_theta2_value(x)
    value3 = slider_theta3_value(x)
    txb_Px_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Py_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Pz_FK.delete(0,END) # Xóa dữ liệu trước đó
    Px = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[0]
    Py = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[1]
    Pz = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[2]
    txb_Px_FK.insert(0,Px) # hiển thị dữ liệu mới
    txb_Py_FK.insert(0,Py) # hiển thị dữ liệu mới
    txb_Pz_FK.insert(0,Pz) # hiển thị dữ liệu mới
    mang = f'F{value1}A{value2}B{value3}C'
    print(mang,type(mang))
    ser.write(mang.encode())
    time.sleep(0.01)

def theta3_set_btn():
    slider_theta3.set(txb_slider_theta3.get())
    value1 = slider_theta1_value(x) # lấy giá trị hiện tại của thanh slider 
    value2 = slider_theta2_value(x)
    value3 = slider_theta3_value(x)
    txb_Px_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Py_FK.delete(0,END) # Xóa dữ liệu trước đó
    txb_Pz_FK.delete(0,END) # Xóa dữ liệu trước đó
    Px = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[0]
    Py = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[1]
    Pz = KinematicRobot.Forward_Kinematic(value1,value2,value3,L1,L2,L3)[2]
    txb_Px_FK.insert(0,Px) # hiển thị dữ liệu mới
    txb_Py_FK.insert(0,Py) # hiển thị dữ liệu mới
    txb_Pz_FK.insert(0,Pz) # hiển thị dữ liệu mới
    mang = f'F{value1}A{value2}B{value3}C'
    print(mang,type(mang))
    ser.write(mang.encode())
    time.sleep(0.01)

def Start_btn():
    mang = 'S'
    ser.write(mang.encode())
    time.sleep(0.01)

def Stop_btn():
    mang = 'T'
    ser.write(mang.encode())
    time.sleep(0.01)

def hut_btn():
    mang = 'H'
    ser.write(mang.encode())
    time.sleep(0.01)
def tha_btn():
    mang = 'L'
    ser.write(mang.encode())
    time.sleep(0.01)

#--------------------------------------------------------------------------

lbl_tieude = Label(GiaoDien,text="   CONTROL INTERFACE",font=("Arial",17,font.BOLD),bg=background_color)
lbl_FK = Label(GiaoDien,text="FORWARD KINEMATIC",fg="blue",font=("Arial",14,font.BOLD),bg=background_color)
lbl_IK = Label(GiaoDien,text="INVERSE KINEMATIC",fg="blue",font=("Arial",14,font.BOLD),bg=background_color)
lbl_theta1_FK = Label(GiaoDien,text="Theta1",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_theta2_FK = Label(GiaoDien,text="Theta2",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_theta3_FK = Label(GiaoDien,text="Theta3",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_theta1_IK = Label(GiaoDien,text="Theta1",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_theta2_IK = Label(GiaoDien,text="Theta2",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_theta3_IK = Label(GiaoDien,text="Theta3",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Px_IK = Label(GiaoDien,text="Px",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Py_IK = Label(GiaoDien,text="Py",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Pz_IK = Label(GiaoDien,text="Pz",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Px_FK = Label(GiaoDien,text="Px",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Py_FK = Label(GiaoDien,text="Py",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Pz_FK = Label(GiaoDien,text="Pz",fg="black",font=("Arial",12,font.BOLD),bg=background_color)
lbl_Theta = Label(GiaoDien,text="Theta",fg="black",font=("Arial",12,font.BOLD),bg=background_color)


txb_slider_theta1 = Entry(GiaoDien,width=6,font=("Arial",12,font.BOLD)) #tao o hien thi goc ben canh slider
txb_slider_theta1.insert(0,value1)

txb_slider_theta2 = Entry(GiaoDien,width=6,font=("Arial",12,font.BOLD))
txb_slider_theta2.insert(0,value2)

txb_slider_theta3 = Entry(GiaoDien,width=6,font=("Arial",12,font.BOLD))
txb_slider_theta3.insert(0,value3)

txb_Px_IK = Entry(GiaoDien,width=10,font=("Arial",12,font.BOLD)) # tao o hien thi Px Invert
txb_Py_IK = Entry(GiaoDien,width=10,font=("Arial",12,font.BOLD))
txb_Pz_IK = Entry(GiaoDien,width=10,font=("Arial",12,font.BOLD))
txb_Px_FK = Entry(GiaoDien,width=7,font=("Arial",12,font.BOLD))
txb_Py_FK = Entry(GiaoDien,width=7,font=("Arial",12,font.BOLD))
txb_Pz_FK = Entry(GiaoDien,width=7,font=("Arial",12,font.BOLD))
txb_Theta = Entry(GiaoDien,width=9,font=("Arial",12,font.BOLD))
txb_theta1_IK = Entry(GiaoDien,width=9,font=("Arial",12,font.BOLD)) #tao o hien thi goc invert
txb_theta2_IK = Entry(GiaoDien,width=9,font=("Arial",12,font.BOLD))
txb_theta3_IK = Entry(GiaoDien,width=9,font=("Arial",12,font.BOLD))

slider_theta1 = Scale(GiaoDien,from_=-90, to_= 90,orient=HORIZONTAL,width=15,resolution=0.5,length=350,command=FK) # gioi han thanh slider
slider_theta1.set(value1)
slider_theta2 = Scale(GiaoDien,from_=-75, to_= 90,orient=HORIZONTAL,width=15,resolution=0.5,length=350,command=FK)
slider_theta2.set(value2)
slider_theta3 = Scale(GiaoDien,from_=-150, to_= 150,orient=HORIZONTAL,width=15,resolution=0.5,length=350,command=FK)
slider_theta3.set(value3)

btn_Start = Button(GiaoDien,text="Set Home",font=("Arial",12,font.BOLD),width=10,height=2,bg='#FF1493',command=Start_btn)
btn_Stop = Button(GiaoDien,text="Stop",font=("Arial",12,font.BOLD),width=10,height=2,bg='#FF0000',command=Stop_btn)
btn_Solve = Button(GiaoDien,text="Solve",font=("Arial",12,font.BOLD),width=10,height=2,bg='#98FB98',command=IK)
btn_ReSet = Button(GiaoDien,text="Reset",font=("Arial",12,font.BOLD),width=10,height=2,bg='#98FB98',command=ReSet_btn)
btn_Set_Theta1 = Button(GiaoDien,text="Set Theta1",font=("Arial",10,font.BOLD),width=8,height=2,bg='#98FB98',command=theta1_set_btn)
btn_Set_Theta2 = Button(GiaoDien,text="Set Theta2",font=("Arial",10,font.BOLD),width=8,height=2,bg='#98FB98',command=theta2_set_btn)
btn_Set_Theta3 = Button(GiaoDien,text="Set Theta3",font=("Arial",10,font.BOLD),width=8,height=2,bg='#98FB98',command=theta3_set_btn)

btn_hut = Button(GiaoDien,text="Hut",font=("Arial",10,font.BOLD),width=8,height=2,bg='#98FB98',command=hut_btn)
btn_tha = Button(GiaoDien,text="Tha",font=("Arial",10,font.BOLD),width=8,height=2,bg='#98FB98',command=tha_btn)

lbl_tieude.place(x=150,y=0)
lbl_FK.place(x=x_axis,y=y_axis)
lbl_IK.place(x=x_axis,y=y_axis+y_lable_IK)
lbl_theta1_FK.place(x=x_axis,y=y_axis+40)
lbl_theta2_FK.place(x=15,y=y_axis+85)
lbl_theta3_FK.place(x=15,y=y_axis+130)
lbl_theta1_IK.place(x=200,y=y_axis+y_lable_IK+53)
lbl_theta2_IK.place(x=200,y=y_axis+y_lable_IK+103)
lbl_theta3_IK.place(x=200,y=y_axis+y_lable_IK+153)
lbl_Px_FK.place(x=x_axis+40,y=y_axis+170)
lbl_Py_FK.place(x=x_axis+140,y=y_axis+170)
lbl_Pz_FK.place(x=x_axis+240,y=y_axis+170)
lbl_Px_IK.place(x=x_axis+20,y=y_axis+y_lable_IK+50)
lbl_Py_IK.place(x=x_axis+20,y=y_axis+y_lable_IK+100)
lbl_Pz_IK.place(x=x_axis+20,y=y_axis+y_lable_IK+150)
lbl_Theta.place(x=x_axis,y=y_axis+y_lable_IK+200)
        
slider_theta1.place(x=x_axis+60,y=y_axis+30)
slider_theta2.place(x=x_axis+60,y=y_axis+75)
slider_theta3.place(x=x_axis+60,y=y_axis+120)

txb_slider_theta1.place(x=x_axis+420,y=y_axis+40)
txb_slider_theta2.place(x=x_axis+420,y=y_axis+80)
txb_slider_theta3.place(x=x_axis+420,y=y_axis+130)
txb_Px_FK.place(x=x_axis+20,y=y_axis+203)
txb_Py_FK.place(x=x_axis+120,y=y_axis+203)
txb_Pz_FK.place(x=x_axis+220,y=y_axis+203)
txb_Px_IK.place(x=x_axis+50,y=y_axis+y_lable_IK+53)
txb_Py_IK.place(x=x_axis+50,y=y_axis+y_lable_IK+103)
txb_Pz_IK.place(x=x_axis+50,y=y_axis+y_lable_IK+153)
txb_Theta.place(x=x_axis+58,y=y_axis+y_lable_IK+203)
txb_theta1_IK.place(x=270,y=y_axis+y_lable_IK+53)
txb_theta2_IK.place(x=270,y=y_axis+y_lable_IK+103)
txb_theta3_IK.place(x=270,y=y_axis+y_lable_IK+153)

btn_Start.place(x=x_axis+60,y=y_axis+y_lable_IK+250)
btn_Stop.place(x=x_axis+370,y=y_axis+y_lable_IK+250)
btn_Solve.place(x=x_axis+370,y=y_axis+y_lable_IK+85)
btn_ReSet.place(x=x_axis+370,y=y_axis+190)
btn_Set_Theta1.place(x=x_axis+480,y=y_axis+26)
btn_Set_Theta2.place(x=x_axis+480,y=y_axis+74)
btn_Set_Theta3.place(x=x_axis+480,y=y_axis+120)

btn_hut.place(x=270,y=y_axis+y_lable_IK+203)
btn_tha.place(x=x_axis+370,y=y_axis+y_lable_IK+203)


#------------------------------------ TRẠNG THÁI BAN ĐẦU --------------------------------------
Px = KinematicRobot.Forward_Kinematic(0,0,0,L1,L2,L3)[0]
Py = KinematicRobot.Forward_Kinematic(0,0,0,L1,L2,L3)[1]
Pz = KinematicRobot.Forward_Kinematic(0,0,0,L1,L2,L3)[2]
txb_Px_FK.insert(0,Px) # hiển thị dữ liệu mới
txb_Py_FK.insert(0,Py) # hiển thị dữ liệu mới
txb_Pz_FK.insert(0,Pz) # hiển thị dữ liệu mới


GiaoDien.mainloop() # vong lap
