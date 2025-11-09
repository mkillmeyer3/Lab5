// Import libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <SFML/Network.hpp>

/**
 * @brief Helper function for parsing and validating integers from the command line
 *
 * @param strValue Command line argument value
 * @param outputValue Reference to where parsed integer should be stored
 * @param minValue Argument's minimum valid value
 * @param maxValue Argument's maximum valid value
 */
void parseInt(const std::string &strValue, int &outputValue, int minValue, int maxValue)
{
    // Try/catch providing descriptive error messages
    try
    {
        size_t pos;
        int value = std::stoi(strValue, &pos); // Try to convert the string value into an integer

        // If not all characters converted i.e. decimal value was seen throw an error
        if (pos != strValue.length())
        {
            throw std::invalid_argument(""); // Use a more descript error message from the catch block below
        }

        // Check that the minimum value was met and the maximum was not exceeded
        if (value < minValue || value > maxValue)
        {
            throw std::out_of_range(""); // Use a more descript error message from the catch block below
        }

        // Valid input gets stored into output reference
        outputValue = value;
    }
    catch (const std::invalid_argument &)
    {
        std::cout << "Invalid command line argument detected: " << strValue << "\n"
                  << "Please check your values and press the 'Enter' key to end the program!";

        std::cin.get();
        exit(1);
    }
    catch (const std::out_of_range &)
    {
        std::cout << "Invalid command line argument detected: " << strValue << "\n"
                  << "Please check your values and press the 'Enter' key to end the program!";

        std::cin.get();
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int port;
    parseInt(argv[1], port, 61000, 65535);

    std::ofstream logFile("server.log", std::ios::app);

    // Create a server socket to accept new connections
    sf::TcpListener listener;

    // Listen to the given port for incoming connections
    if (listener.listen(port) != sf::Socket::Done)
    {
        return 1;
    }
    std::cout << "Server is listening on port " << port << ", waiting for connections... " << std::endl;
    logFile << "Server is listening on port " << port << ", waiting for connections... " << std::endl;

    // Endless loop that waits for new connections
    // Create a list to store the future clients
    std::vector<sf::TcpSocket *> clients;

    // Create a selector
    sf::SocketSelector selector;

    // Add the listener to the selector
    selector.add(listener);

    // Endless loop that waits for new connections
    // Based off of example code inside of `SocketSelector.h`
    while (true)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (selector.isReady(listener))
            {
                // The listener is ready: there is a pending connection
                sf::TcpSocket *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    unsigned short remotePort = client->getRemotePort();

                    // Add the new client to the clients list
                    clients.push_back(client);

                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);

                    // Print messages to log and console output
                    std::cout << "Client " << remotePort << " connected." << std::endl;
                    logFile << "Client " << remotePort << " connected." << std::endl;
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete client;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (auto it = clients.begin(); it != clients.end();)
                {
                    sf::TcpSocket *client = *it;

                    // Flag which is set when a client is disconnected
                    bool removed = false;

                    // Only try to process message if the client is ready
                    if (selector.isReady(*client))
                    {   
                        char buffer[1024];
                        std::size_t received;

                        // Receive data into the buffer
                        sf::Socket::Status status = client->receive(buffer, sizeof(buffer), received);

                        unsigned short remotePort = client->getRemotePort();
                        
                        // If a message was received, print the message to log and console
                        if (status == sf::Socket::Done)
                        {
                            std::string msg(buffer, received);
                            std::cout << "\tClient " << remotePort << ": " << msg << std::endl;
                            logFile << "\tClient " << remotePort << ": " << msg << std::endl;
                        }
                        // If the client was disconnected, notify the server and remove it from the vector of clients
                        else if (status == sf::Socket::Disconnected)
                        {
                            std::cout << "Client " << remotePort << " disconnected." << std::endl;
                            logFile << "Client " << remotePort << " disconnected." << std::endl;
                            it = clients.erase(it);
                            removed = true;
                        }
                    }

                    // Needed a separate flag, to ensure that iterator moves to next client properly
                    if (!removed)
                    {
                        it++;
                    }
                }
            }
        }
    }
}