
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Launch a server, wait for an incoming connection,
/// send a message and wait for the answer.
///
////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int port = 61000;
    // Create a server socket to accept new connections
    sf::TcpListener listener;

    // Listen to the given port for incoming connections
    if (listener.listen(port) != sf::Socket::Done)
        return 1;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

    // Wait for a connection
    sf::TcpSocket socket;
    if (listener.accept(socket) != sf::Socket::Done)
        return 1;
    std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;

    // Send a message to the connected client
    const char out[] = "Hi, I'm the server";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return 1;
    std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;

    // Receive a message back from the client
    char in[128];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return 1;
    std::cout << "Answer received from the client: \"" << in << "\"" << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');
}



