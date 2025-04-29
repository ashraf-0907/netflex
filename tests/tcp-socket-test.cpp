// #include "./tcp-socket-test.h"

// using namespace std;

// /**
//  * @brief Helper function to create a raw TCP server for testing.
//  */
// void TcpSocketTest::startMockServer() {
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     assert(serverSocket != -1 && "Failed to create server socket");

//     // ✅ Enable SO_REUSEADDR
//     int opt = 1;
//     setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

//     struct sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(TEST_PORT);

//     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
//         perror("Bind failed");
//         assert(false && "Server failed to bind");
//     }

//     if (listen(serverSocket, 5) != 0) {
//         perror("Listen failed");
//         assert(false && "Server failed to listen");
//     }

//     cout << "[INFO] Server started on port " << TEST_PORT << "..." << endl;

//     int clientSocket = accept(serverSocket, nullptr, nullptr);
//     assert(clientSocket != -1 && "Failed to accept client connection");

//     cout << "[INFO] Client connected successfully!" << endl;

//     // ✅ Send Large Data
//     string largeData(LARGE_DATA_SIZE, 'X'); // 5000 bytes of 'X'
//     ssize_t bytesSent = send(clientSocket, largeData.c_str(), largeData.length(), 0);
//     assert(bytesSent == LARGE_DATA_SIZE && "Failed to send large data");

//     cout << "[INFO] Large data sent successfully!" << endl;

//     close(clientSocket);
//     close(serverSocket);
// }

// /**
//  * @brief Test case to check if TcpSocket can connect to a mock server.
//  */
// void TcpSocketTest::testConnectToServer() {
//     cout << "[TEST] Running testConnectToServer..." << endl;

//     thread serverThread(&TcpSocketTest::startMockServer, this);
//     this_thread::sleep_for(chrono::milliseconds(1000)); // ✅ Ensure server starts before connecting

//     TcpSocket client(AddressFamily::IPV4);
//     Response res = client.connectToServer(TEST_IP, TEST_PORT);

//     cout << (ResponseHandler::getResponseMessage(res)) << "response";

//     assert(res == Response::SUCCESS && "Failed to connect to the server");

//     cout << "[PASS] testConnectToServer passed!" << endl;
//     serverThread.join();
// }


// /**
//  * @brief Test case to check sending data over TcpSocket.
//  */
// void TcpSocketTest::testSendData() {
//     cout << "[TEST] Running testSendData..." << endl;

//     thread serverThread(&TcpSocketTest::startMockServer, this);
//     this_thread::sleep_for(chrono::milliseconds(500));

//     TcpSocket client(AddressFamily::IPV4);
//     client.connectToServer(TEST_IP, TEST_PORT);

//     string message = "Hello, Server!";
//     Response res = client.sendData(message);

//     assert(res == Response::SUCCESS && "Failed to send data");

//     cout << "[PASS] testSendData passed!" << endl;
//     serverThread.join();
// }

// /**
//  * @brief Test case to check receiving large data (> 4096 bytes).
//  */
// void TcpSocketTest::testReceiveLargeData() {
//     cout << "[TEST] Running testReceiveLargeData..." << endl;

//     thread serverThread(&TcpSocketTest::startMockServer, this);
//     this_thread::sleep_for(chrono::milliseconds(500));

//     TcpSocket client(AddressFamily::IPV4);
//     client.connectToServer(TEST_IP, TEST_PORT);

//     string receivedData = client.receiveData();

//     assert(receivedData.size() == LARGE_DATA_SIZE && "Received data size does not match expected size");
//     assert(receivedData == string(LARGE_DATA_SIZE, 'X') && "Received data is corrupted");

//     cout << "[PASS] testReceiveLargeData passed!" << endl;
//     serverThread.join();
// }

// /**
//  * @brief Runs all test cases in the TcpSocketTest class.
//  */
// void TcpSocketTest::runAllTests() {
//     testConnectToServer();
//     testSendData();
//     testReceiveLargeData();
// }
