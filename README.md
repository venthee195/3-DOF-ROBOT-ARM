1.Introduction

This project designs and constructs a 3-degree-of-freedom robotic arm model. Calculating and testing kinematics, drawing workspaces and planning the trajectory for robots. Then write the program and control the 3-DOF robot in practice
The 3-degree-of-freedom robot arm model is designed on SolidWorks software as shown below:

![coordinate_axes_robot](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/2736f4e8-916a-4ceb-9249-28dc4a1e8cfa)

2.Drawing workspaces robot 3-DOF arm

The robot's workspace is the area containing the points and positions that the robot's end mechanism can reach. By determining the workspace of the robot, we can meet the requirements and tasks of the robot in reality, as well as upgrade and improve the robot later. To determine the workspace of the robot, the team used the results of forward kinematics and based on the solutions of the inverse kinematics to build a program to draw the workspace on Matlab software. 
The figure below shows the workspace output after running the program.
![workspaces](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/3f2cd2f8-9036-41dd-ada1-d041d7b04cc8)
![test_kinematic](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/159a38e7-7207-4267-97ea-96f02b58cef2)
![robot_3dof_real_1](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/3104dbb5-1303-4800-a1e7-d461c0372670)
![robot_3dof_real](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/fd64f365-57e7-4fa7-b509-7b5aa3d93fc0)
![robot_3dof](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/7185410e-66a6-4f8c-b302-9fef33ec6205)

3.Program and Interface Control

Interface function:

Set Home button: run the robot model in the Set Home position.

STOP button: stop running the robot model.

RESET button: reset the value of theta angles to 0.

SOLVE button: calculate angle theta 1,2,3 from the given position of Px, Py, Pz.

Enter the simulated angle in 2 ways: drag the slider or enter it directly.

Inverse kinematics: synchronous control of joints.

![Gui_control](https://github.com/tranvanthe123/3-DOF-ROBOT-ARM/assets/86809706/bf6efc1b-905b-497b-ae66-9debde0e7b21)
