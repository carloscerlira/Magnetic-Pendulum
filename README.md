# Magnetic Pendulum
A numerical solution for the equations of motion governing the behavior of a magnetic pendulum was found, this system is of interest as it is known to be chaotic.  
The numerical solution was performed for every point of a nxn image using parallel computing with the help of CUDA, NVIDIA's API.  
The complexity of the problem was reduced from O(n^2) using brute force to O(1).
![Imgur Image](https://imgur.com/cRJI8Ky.png)
![Imgur Image](https://imgur.com/6BFZDp2.png)  
Brief explanation about the construction of a magnetic pendulum:  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/xHux2AIM0a4/0.jpg)](https://www.youtube.com/watch?v=xHux2AIM0a4)
# Documentation
* The cppIntegration.cu contains the program, a graphics API is nedeed for implenetation such as OpenGL.
* The poster.pdf file constains the scientific poster where the problem and the details about the solution can be found.
