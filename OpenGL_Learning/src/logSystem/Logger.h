#pragma once
#include <string>
#include <vector>

struct Color {
	unsigned char r, g, b;

	Color() {}
	Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

class Message {
public:
	std::string text;

	Color foreground;
	Color background;

	void init(const char* message, Color fg, Color bg);
	void init(std::string message, Color fg, Color bg);
};

class Logger {
	bool consoleOutput;
	std::vector<Message> messages;

public:
	void logToConsole(bool state);
	void add(const char* message);
	void add(const char* message, Color& foreground, Color& background);
	void add(std::string message);
	void add(std::string message, Color& foreground, Color& background);
	void add(Message& message);

	template<typename... Msg>
	void addCombined(Msg&... msg);

};

template<typename ...Msg>
inline void Logger::addCombined(Msg & ...msg) {
	(add(msg), ...);
}