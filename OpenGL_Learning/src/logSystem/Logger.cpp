#include "Logger.h"
#include <iostream>
#include <string>


void Message::init(const char* message, Color fg, Color bg) {
	text = message;

	foreground = fg;
	background = bg;
}

void Message::init(std::string message, Color fg, Color bg) {
	init(message.c_str(), fg, bg);
}


void Logger::logToConsole(bool state) {
	consoleOutput = state;
}


void Logger::add(const char* message) {
	if (consoleOutput) {
		std::cout << message;
	}
}

void Logger::add(const char* message, Color& foreground, Color& background) {
	if (consoleOutput) {
		std::string fgColor = "\033[38;2;" + std::to_string(foreground.r) + ";" + std::to_string(foreground.g) + ";" + std::to_string(foreground.b) + "m";
		std::string bgColor = "\033[48;2;" + std::to_string(background.r) + ";" + std::to_string(background.g) + ";" + std::to_string(background.b) + "m";

		
		std::cout << fgColor << bgColor << message << "\033[0m";
	}
}

void Logger::add(std::string message) {
	if (consoleOutput) {
		std::cout << message;
	}
}

void Logger::add(std::string message, Color& foreground, Color& background) {
	if (consoleOutput) {
		std::string fgColor = "\033[38;2;" + std::to_string(foreground.r) + ";" + std::to_string(foreground.g) + ";" + std::to_string(foreground.b) + "m";
		std::string bgColor = "\033[48;2;" + std::to_string(background.r) + ";" + std::to_string(background.g) + ";" + std::to_string(background.b) + "m";


		std::cout << fgColor << bgColor << message << "\033[0m";
	}
}

void Logger::add(Message& message) {
	if (consoleOutput) {
		std::string fgColor = "\033[38;2;" + std::to_string(message.foreground.r) + ";" + std::to_string(message.foreground.g) + ";" + std::to_string(message.foreground.b) + "m";
		std::string bgColor = "\033[48;2;" + std::to_string(message.background.r) + ";" + std::to_string(message.background.g) + ";" + std::to_string(message.background.b) + "m";


		std::cout << fgColor << bgColor << message.text << "\033[0m";
	}
}







