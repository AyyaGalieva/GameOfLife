#include "GameOfLife.h"

namespace GameOfLife
{
	void Cell::set_state(bool isalive) {
		state = isalive;
	}
	bool Cell::get_state() {
		return state;
	}

	Model::Model() {
		size = 10;
		field = new Cell*[size];
		for (int i = 0; i < size; ++i)
		{
			field[i] = new Cell[size];
			for (int j = 0; j < size; ++j)
				field[i][j].set_state(dead);
		}
	}

	Model::Model(const Model& other) {
		size = 10;
		for (int i = 0; i < size; ++i)
		{
			field = new Cell*[size];
			for (int j = 0; j < size; ++j) {
				field[i] = new Cell[size];
				field[i][j].set_state(other.field[i][j].get_state());
			}
		}
	}

	Model::Model(Model && other) {
		size = 10;
		std::swap(field, other.field);
	}

	Model::~Model() {
		for (int i = 0; i < size; ++i)
			delete[] field[i];
	}

	Model & Model::operator=(const Model & other) {
		if (this != &other) {
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j)
					field[i][j].set_state(other.field[i][j].get_state());
			}
		}
		return *this;
	}
	Model & Model::operator=(Model && other) {
		std::swap(field, other.field);
		return *this;
	}

	int Model::get_size() {
		return size;
	}

	bool Model::get_cell_state(int i, int j) {
		return field[i][j].get_state();
	}

	void Controller::print_field() {
		std::cout << ' ';
		for (char i = 'A'; i <= 'J'; ++i)
			std::cout << i;
		std::cout << std::endl;
		for (int i = 0; i < m.get_size(); ++i)
		{
			std::cout << i;
			for (int j = 0; j < m.get_size(); ++j) {
				if (m.get_cell_state(i,j)) std::cout << '*';
				else std::cout << '.';
			}
			std::cout << std::endl;
		}
	}

	Controller::Controller(std::string str) {
		comand = str;
	}

	void Controller::run() {
		std::getline(std::cin, comand);
		while (comand != "stop") {
			if (comand == "reset") {
				prev = m;
				m.reset();
				print_field();
			}
			else if (comand.substr(0, 3) == "set") {
				prev = m;
				m.set(comand[4], comand[5]);
				print_field();
			}
			else if (comand.substr(0, 5) == "clear") {
				prev = m;
				m.clear(comand[6], comand[7]);
				print_field();
			}
			else if (comand.substr(0, 4) == "step") {
				if ((comand[5] - '0') > 0) {
					m.step((comand[5] - '0')-1);
					prev = m;
					m.step(1);
				}
				print_field();
			}
			else if (comand == "back") {
				m = prev;
				print_field();
			}
			else if (comand.substr(0, 4) == "save") {
				m.save(comand.substr(5, comand.length() - 5));
				print_field();
			}
			else if (comand.substr(0, 4) == "load") {
				m.load(comand.substr(5, comand.length() - 5));
				print_field();
			}
			else std::cout << "Please enter a valid comand" << std::endl;

			std::getline(std::cin, comand);
		}
	}

	void Model::reset() {
		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				field[i][j].set_state(dead);
	}

	void Model::set(char x, int y) {
		field[y - '0'][((int)x) - (int)'A'].set_state(alive);
	}
	void Model::clear(char x, int y) {
		field[y - '0'][((int)x) - (int)'A'].set_state(dead);
	}
	void Model::step(int n) {
		for (int i = 0; i < n; ++i) {
			Model tmp;
			tmp = *this;
			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j) {
					int count = 0;
					if (field[(i + size - 1) % size][(j + size - 1) % size].get_state()) count++;
					if (field[(i + size - 1) % size][j].get_state()) count++;
					if (field[(i + size - 1) % size][(j + 1) % size].get_state()) count++;
					if (field[i][(j + 1) % size].get_state()) count++;
					if (field[(i + 1) % size][(j + 1) % size].get_state()) count++;
					if (field[(i + 1) % size][j].get_state()) count++;
					if (field[(i + 1) % size][(j + size - 1) % size].get_state()) count++;
					if (field[i][(j + size - 1) % size].get_state()) count++;
					if (!(field[i][j].get_state()) && (count == 3)) tmp.field[i][j].set_state(alive);
					if ((field[i][j].get_state()) && (count < 2)) tmp.field[i][j].set_state(dead);
					if ((field[i][j].get_state()) && (count > 3)) tmp.field[i][j].set_state(dead);
				}
			*this = tmp;
		}
	}

	void Model::save(std::string filename) {
		std::ofstream fout;
		fout.open(filename);
		fout << ' ';
		for (char i = 'A'; i <= 'J'; ++i)
			fout << i;
		fout << std::endl;
		for (int i = 0; i < size; ++i)
		{
			fout << i;
			for (int j = 0; j < size; ++j) {
				if (field[i][j].get_state()) fout << '*';
				else fout << '.';
			}
			fout << std::endl;
		}
		fout.close();
	}

	void Model::load(std::string filename) {
		std::ifstream fin;
		fin.open(filename);
		std::string s;
		std::getline(fin, s);
		char c;
		for (int i = 0; i < size; ++i) {
			fin >> c;
			for (int j = 0; j < size; ++j) {
				fin >> c;
				if (c == '.')
					field[i][j].set_state(dead);
				else if (c == '*') field[i][j].set_state(alive);
				else std::cerr << "invalid file";
			}
		}
	}
}