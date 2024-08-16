

# Network Tracker Client-Server (TCP/UDP) Project

## Overview

This project is a client-server application built using C for Linux that enables clients to access each other's servers and obtain comprehensive data about their visitors. The project dynamically provides an in-depth understanding of client-server relationships by leveraging both TCP (Transmission Control Protocol) and UDP (User Datagram Protocol) for communication.

## Features

- **Dual-Protocol Communication**: Implements both TCP and UDP protocols, allowing for reliable and fast data transmission between clients and servers.
- **Real-time Data Access**: Clients can access server data in real-time, retrieving information about visitors and other relevant metrics.
- **Network Programming**: Provides a robust example of network programming concepts, including socket creation, data transmission, and protocol management.
- **Client-Server Architecture**: Demonstrates the use of client-server architecture in a networked environment, with a focus on efficiency and reliability.

## Project Structure

- **Server Code**: Manages incoming connections, handles requests, and processes data for clients.
- **Client Code**: Initiates communication with the server, sends requests, and processes received data.
- **Protocol Implementation**: Separate implementations for TCP and UDP protocols, each handling data transmission in different scenarios.
- **Utility Functions**: Includes functions for socket creation, connection management, and error handling.

## How to Run

1. **Compile the Code**: 
   - Use GCC or any other C compiler to compile the server and client code.
   - Example: `gcc -o server server.c` and `gcc -o client client.c`
   
2. **Run the Server**:
   - Start the server on a designated port.
   - Example: `./server <port_number>`

3. **Run the Client**:
   - Run the client and connect to the server using the server's IP address and port number.
   - Example: `./client <server_ip> <port_number>`

4. **Choose Protocol**:
   - The client can choose between TCP and UDP protocols based on the requirements.
   
5. **Data Transmission**:
   - The client sends a request, and the server processes it and sends back the relevant data.

## Use Cases

- **Network Monitoring**: Track and monitor visitor data across multiple servers.
- **Client-Server Applications**: Serve as a foundation for building more complex client-server applications that require both reliable and fast communication.
- **Learning Resource**: A practical example for students and professionals to learn about TCP/UDP communication in C.

## Skills Demonstrated

- **C Programming**
- **TCP/UDP Protocols**
- **Socket Programming**
- **Client-Server Architecture**
- **Network Communication**

## Contact

For any questions or further information, feel free to reach out:

- **Rishabh Jagtap**
- Email: rjagtap9299@gmail.com
- LinkedIn: [Rishabh Jagtap](https://www.linkedin.com/in/rishabh-jagtap/)
