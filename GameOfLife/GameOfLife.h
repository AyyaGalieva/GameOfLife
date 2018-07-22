#include <iostream>
#include <string>
#include <fstream>

#define dead false
#define alive true

namespace GameOfLife
{
	class Cell {
		bool state;
	public:
		void set_state(bool isalive);
		bool get_state();
	};

	class Model {
		int size;
		Cell **field;
	public:
		Model();
		~Model();
		Model(const Model& other);
		Model(Model && other);
		Model & operator=(const Model & other);
		Model & operator=(Model && other);
		int get_size();
		bool get_cell_state(int i, int j);
		void reset();
		void set(char x, int y);
		void clear(char x, int y);
		void step(int n);
		void save(std::string filename);
		void load(std::string filename);
	};

	class Controller {
		std::string comand;
		Model m;
		Model prev;
	public:
		Controller(std::string comand);
		void print_field();
		void run();
	};
}