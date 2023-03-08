//compile using g++ <filename> -o ds
/**************************************************
* File: dragonScript.c
* Author: PotatoDOnkey
* Date: 2023/02/26
* Description: A programming language inspired by bearded dragons and assembly.
**************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <random>
#include <map>
#include <stdio.h>
#include <cstring>
#include "dragonscript.h"
#include "drawing.h"

void delay(int milliseconds) {
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

int interpret(std::vector<std::string> instructions) {
  std::map<std::string, int> registers;
  std::map<std::string, std::string> strings;
  std::map<std::string, int> labels;
  int STACK_SIZE = 10;
  int stack[STACK_SIZE];
  int top = -1;
  int line_called_from;
    
  for (int it = 0; it < instructions.size(); ++it) {
    const auto& instruction = instructions[it];
    if (instruction.empty()) {
      continue;
    }
    std::string op, x, y, z;
    std::istringstream iss(instruction);
    iss >> op >> x >> y >> z;

    if (op == "imp") {
      // replace the current line with the contents of a file written after this command
        std::string filename = x;
        std::ifstream infile(filename);
        std::string line;
        int lineno = it + 1;
        while (std::getline(infile, line)) {
            instructions.insert(instructions.begin() + lineno, line);
            ++lineno;
        }
    }
  }

  for (int it = 0; it < instructions.size(); ++it) {
    const auto& instruction = instructions[it];
    std::string op, x, y, z;
    std::istringstream iss(instruction);
    iss >> op >> x >> y >> z;

    if (op == "lbl") {
      labels[x] = it;
    }
  }

  // Interpreting the instructions
  for (int it = 0; it < instructions.size(); ++it) {
    const auto& instruction = instructions[it];
    std::string op, x, y, z;
    std::istringstream iss(instruction);
    iss >> op >> x >> y >> z;

    if (op == "int") {
        //move
        if (registers.count(y) == 1) {
            registers[x] = registers[y];
        } else {
            registers[x] = stoi(y);
        }
    } else if(op == "str") {
        //move
        if (strings.count(y) == 1) {
            strings[x] = strings[y];
        } else {
            strings[x] = y;
        }
    } else if (op == "inc") {
        //increment
      ++registers[x];
    } else if (op == "dec") {
        //decrement
      --registers[x];
    } else if (op == "add") {
        //add
      registers[x] += registers[y];
    } else if (op == "sub") {
        //substract
      registers[x] -= registers[y];
    } else if(op == "mul") {
       //multiply
       registers[x] *= registers[y];
    } else if(op == "div") {
       //divide
       registers[x] /= registers[y];
    } else if(op == "con") {
        strings[x] += strings[y];
    } else if (op == "prt") {
        //print
        std::cout << registers[x];
    } else if (op == "prs") {
        //print
        if(x == "endl") {
            std::cout << std::endl;
        } else if(x == "space") {
            std::cout << " ";
        } else {
            std::cout << strings[x];
        }
    } else if (op == "pra") {
      //print string
      int number = registers[x];
      if(number == 9000) {
        std::cout << " ";
      } else if(number == 9001) {
        std::cout << std::endl;
      } else {
        char ascii_value = static_cast<char>(number);
        std::cout << ascii_value;
      }
    } else if(op == "del") {
        if(registers.count(x) == 1) {
            //variable is an integer
            registers[x] = 0;
        } else if(strings.count(x) == 1) {
            strings[x] = "";
        }
    } else if(op == "cmp") {
        //compare if two strings are equal
        if(strings[x] == strings[y]) {
            int index = labels[z];
            it = index;
        } else {
            continue;
        }
    } else if(op == "ini") {
        //take input
        //std::cout << "Input (integer): ";
        int user_input;
        std::cin >> user_input;
        registers[x] = user_input;
    } else if(op == "ins") {
        //take input
        //std::cout << "Input (string): ";
        std::string user_input;
        std::cin >> user_input;
        strings[x] = user_input;
        //std::cout << strings[x];
    } else if(op == "hz") {
        //Halt if zero
        if(registers[x] == 0){
            return 1;
        } else {
            continue;
        }
    } else if(op == "hlt"){
      return 1;
    } else if(op == "hgz") {
        //Halt if greater than zero
        if(registers[x] > 0){
            continue;
        } else {
            return 1;
        }
    } else if(op == "hlz") {
        //Halt if less than zero
        if(registers[x] < 0){
            continue;
        } else {
            return 1;
        }
    } else if(op == "hb") {
        //Halt if bigger than second register
        if(registers[x] > registers[y]){
            continue;
        } else {
            return 1;
        }
    } else if(op == "hl") {
        //Halt if less than second register
        if(registers[x] < registers[y]){
            continue;
        } else {
            return 1;
        }
    } else if(op == "hne") {
        //Halt if not equal
        if(registers[x] == registers[y]){
            continue;
        } else {
            return 1;
        }
    } else if(op == "he") {
        //Halt if equal
        if(registers[x] != registers[y]){
            continue;
        } else {
            return 1;
        }
    } else if(op == "jmp"){
        int index = labels[x];
        line_called_from = it;
        it = index;
    } else if(op == "jz"){
        if(registers[x] == 0){
            int index = labels[y];
            line_called_from = it;
            it = index;
        } else {
            continue;
        }
    }else if(op == "je"){
        if(registers[x] == registers[y]){
            int index = labels[z];
            line_called_from = it;
            it = index;
        } else {
            continue;
        }
    } else if(op == "jg"){
        if(registers[x] > registers[y]){
            int index = labels[z];
            line_called_from = it;
            it = index;
        } else {
            continue;
        }
    }else if(op == "jl"){
        if(registers[x] < registers[y]){
            int index = labels[z];
            line_called_from = it;
            it = index;
        } else {
            continue;
        }
    } else if(op == "jnz"){
        if(registers[x] != 0){
            int index = labels[y];
            line_called_from = it;
            it = index-1;
        } else {
            continue;
        }
    } else if(op == "jgz"){
        if(registers[x] > 0){
            int index = labels[y];
            line_called_from = it;
            it = index-1;
        } else {
            continue;
        }
    } else if(op == "jlz"){
        if(registers[x] < 0){
            int index = labels[y];
            line_called_from = it;
            it = index-1;
        } else {
            continue;
        }
    } else if(op == "jne"){
        if(registers[x] != registers[y]){
            int index = labels[z];
            line_called_from = it;
            it = index-1;
        } else {
            continue;
        }
    } else if(op == "not") {
        //invert a variable
        if(registers[x] != 0) {
            registers[x] = 0;
        } else if(registers[x] == 0) {
            registers[x] = 1;
        } else {
            continue;
        }
    } else if(op == "slp") {
        delay(std::stoi(x));
    } else if(op == "rnd") {
        int min_range = registers[y];
        int max_range = registers[z];

        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<int> distribution(min_range, max_range);
        int random_number = distribution(generator);

        registers[x] = random_number;
    } else if(op == "imp") {
        continue;
    } else if(op == "push") {
        if (top == STACK_SIZE - 1) {
            // stack is full, cannot push any more values
            printf("Error: Stack overflow\n");
            return 1;
        }
        top++; // increment the top of the stack
        stack[top] = registers[x]; // add the value to the top of the stack
    } else if(op == "pop") {
        if (top == -1) {
            // stack is empty, cannot pop any values
            printf("Error: Stack underflow\n");
            return 1;
        }
        int value = stack[top]; // get the value at the top of the stack
        top--; // decrement the top of the stack
        registers[x] = value;
    } else if(op == "ret") {
        it = line_called_from;
    } else if(op == ";") {
        continue;
    } else if(op == "str/") {
        char result[50];
        int num = registers[y];
        sprintf(result, "%d", num);
        strings[x] = result;
    } else if(op == "cmd") {
        char command[50];
        const char* c = x.c_str();
        const char* d = y.c_str();
        const char* e = z.c_str();

        std::string result = std::string(c) + " " + std::string(d) + " " + std::string(e);
        const char* const_char_ptr = result.c_str();

        strcpy(command, const_char_ptr);
        system(command);
    } else if(op == "pxi") {
        int X = std::stoi(x);
        int Y = std::stoi(y);
        int Z = std::stoi(z);
        drawPixel(X, Y, Z);
    } else if(op == "pxv") {
        drawPixel(registers[x], registers[y], registers[z]);
    } else if(op == "cls") {
        // Detect the operating system
        #ifdef _WIN32
            // Windows command
            system("cls");
        #else
            // Unix command
            system("clear");
        #endif
    } else if(op == "lbl") {
        continue;
    } else {
        //continue;
        std::cerr << "Error: Unknown instruction: " << op << " at line " << it + 1<< std::endl;
        return 1;
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
      return 1;
    }

    std::vector<std::string> instructions;

    // Read the file and store the instructions
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        instructions.push_back(line);
    }

    interpret(instructions);
    return 0;
}
