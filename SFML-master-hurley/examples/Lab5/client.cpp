/*
Author: Margaret Killmeyer
Class: ECE6122 (Q)
Last Date Modified: 9 Nov. 2025

Description:
Lab 5 - Client
Main code for Client Debug Logging Application
Sends status messages to a central logging server and saves messages to a file 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
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
                  << "Please check your values and press the 'Enter' key to end the program!" << std::endl;

        std::cin.get();
        exit(1);
    }
    catch (const std::out_of_range &)
    {
        std::cout << "Invalid command line argument detected: " << strValue << "\n"
                  << "Please check your values and press the 'Enter' key to end the program!" << std::endl;

        std::cin.get();
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    // Convert command line arguments into string vector
    std::vector<std::string> args(argv, argv + argc);

    sf::IpAddress server = args[1];

    // Check if the input is a valid IP address
    if (server == sf::IpAddress::None)
    {
        std::cout << "Failed to connect to the server at " << args[1] << " on " << args[2] << ".\n"
                  << "Please check your values and press 'Enter' to end program!";
        std::cin.get();
        return 1;
    }

    // Validate the port number entered
    int port;
    parseInt(args[2], port, 61000, 65535);

    // Create a socket for communicating with the server
    sf::TcpSocket socket;
    
    bool failedConnection = true;

    // Attempt to connect to the server, using a timeout to close the application if unsuccessful
    sf::Clock timer;
    while (failedConnection && timer.getElapsedTime() < sf::seconds(10.0f))
    {
        failedConnection = socket.connect(server, port) != sf::Socket::Done;
    }

    if (failedConnection)
    {
        std::cout << "Failed to connect to the server at " << args[1] << " on " << args[2] << ".\n"
                  << "Please check your values and press 'Enter' to end program!";
        std::cin.get();
        return 1;
    }
    
    std::cout << "Connected to server " << server << ":" << port << std::endl;

    std::string msg;

    // Continuously prompt user to enter a message
    while (true)
    {
        std::cout << "Please enter a message: ";
        std::getline(std::cin, msg);
        
        // Send the message to the server
        if (socket.send(msg.data(), msg.size()) != sf::Socket::Done)
        {
            return 1;
        }        
    }

    return 0;
}