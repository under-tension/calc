#include "app.h"

namespace
{
typedef struct Task {
	int val1;
	int val2;
	int result;
	int status;
	char operation;
} Task;

void parse(int argc, char **argv, Task &task) {

	if (argc == MIN_ARG_COUNT) {
		task.val1 = std::atoi(argv[1]);
		task.operation = *argv[2];
		task.val2 = std::atoi(argv[3]);
		task.status = 0;
	} else if (argc == 2 && argv[1][strlen(argv[1]) - 1] == '!') {
		sscanf(argv[1], "%d!", &task.val1);
		task.operation = '!';
		task.val2 = 0;
		task.status = 0;
	} else {
		task.status = 1;
	}
}

void calc(Task &task) {
	switch (task.operation) {
		case '+':
			task.result = libmath::add(task.val1, task.val2);
			task.status = 0;
			break;
		case '-':
			task.result = libmath::sub(task.val1, task.val2);
			task.status = 0;
			break;
		case '*':
			task.result = libmath::multiplication(task.val1, task.val2);
			task.status = 0;
			break;
		case '/':
			task.status = libmath::division(task.val1, task.val2, task.result);
			break;
		case '^':
			task.result = libmath::power(task.val1, task.val2);
			task.status = 0;
			break;
		case '!':
			task.result = libmath::factorial(task.val1);
			task.status = 0;
			break;
		default:
			task.status = 3;
	}
}

void output(const Task &task) {

	switch (task.status) {
		case 0:
			if (task.operation == '!') {
				std::cout << task.val1 << "! = " << task.result << std::endl;
			} else {
				std::cout << task.val1 << " " << task.operation << " " << task.val2 << " = " << task.result << std::endl;
			}
			break;
		case 1:
			std::cout << "Error! Not enough arguments" << std::endl;
			break;
		case 2:
			std::cout << "Error! Division by zero is not allowed" << std::endl;
			break;
		case 3:
			std::cout << "Error! No operation" << std::endl;
			break;
		default:
			std::cout << "Unknown error at division operation" << std::endl;
	}
}

void print_help(const char *prog) {
    std::cout << "Usage: " << prog << " <number1> <operation> <number2>\n"
			<< "  -v, --version       show version\n"
            << "  -h, --help          show this help message\n";
}

int process_flags(int argc, char **argv)
{
	const char *short_opts = "vh";
    const option long_opts[] = {
        {"version", no_argument, nullptr, 'v'},
        {"help",    no_argument, nullptr, 'h'},
		{nullptr, 0, nullptr, 0}
    };

    int opt;
    int long_index = 0;

    while ((opt = getopt_long(argc, argv, short_opts, long_opts, &long_index)) != -1) {
        switch (opt) {
            case 'v':
                std::cout << "Version 1.0\n";
                return 0;
            case 'h':
                print_help(argv[0]);
                return 0;
            default:
                std::cout << "Unknown flag " << opt << std::endl;
                return -1;
        }
    }

	return -1;
}
}

namespace app
{
void run(int argc, char **argv) {
	int flag_res = process_flags(argc, argv);
	if (flag_res > -1) {
		return;
	}

	Task task;

	parse(argc, argv, task);
	if (task.status != 0) {
		goto output;
	}

	calc(task);

	output:
	output(task);
}
}