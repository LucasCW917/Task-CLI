#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "You need to provide a command!" << std::endl;
        return 1;
    }

    std::string Command = argv[1];

    if (Command == "-help") {
        std::cout << "Task CLI Commands\n"
            << "-init: Initializes the program. Must be used if this is a first time install. Also resets the entire tasking system.\n"
            << "-help: Displays this text\n"
            << "-add \"name\": Adds a task\n"
            << "-pop \"name\": Removes a task. Can be used upon completion or to remove accidental adds.\n"
            << "-list: Lists all tasks\n";
        return 0;
    }

    else if (Command == "-init") {
        std::ofstream jsonfile("tasks.txt");
        if (!jsonfile.is_open()) {
            std::cout << "Failed to initialize." << std::endl;
            return 1;
        }
        jsonfile << "$tasktracker" << std::endl;
        jsonfile.close();
        std::cout << "Successfully created initialization file called \"tasks.txt\"." << std::endl;
    }

    else if (Command == "-add") {
        if (argc < 3) {
            std::cout << "You need to provide a task name!" << std::endl;
            return 1;
        }

        std::string Name;
        for (int i = 2; i < argc; ++i) {
            if (i > 2) Name += " ";
            Name += argv[i];
        }

        std::ofstream jsonfile("tasks.txt", std::ios::app);
        if (!jsonfile.is_open()) {
            std::cout << "Please initialize with \"-init\" first." << std::endl;
            return 1;
        }

        jsonfile << "task " << Name << '\n';
        jsonfile.close();
        std::cout << "Task added: " << Name << std::endl;
    }

    else if (Command == "-pop") {
        if (argc < 3) {
            std::cout << "You need to provide a task name to remove!" << std::endl;
            return 1;
        }

        std::string taskToRemove;
        for (int i = 2; i < argc; ++i) {
            if (i > 2) taskToRemove += " ";
            taskToRemove += argv[i];
        }
        taskToRemove = "task " + taskToRemove;

        std::ifstream inFile("tasks.txt");
        if (!inFile.is_open()) {
            std::cout << "Please initialize with \"-init\" first." << std::endl;
            return 1;
        }

        std::ofstream outFile("tasks_temp.txt");
        std::string line;
        bool found = false;

        while (std::getline(inFile, line)) {
            if (line != taskToRemove) {
                outFile << line << '\n';
            }
            else {
                found = true;
            }
        }

        inFile.close();
        outFile.close();

        if (!found) {
            std::cout << "Task not found: " << argv[2] << std::endl;
            std::remove("tasks_temp.txt");
            return 1;
        }

        std::remove("tasks.txt");
        std::rename("tasks_temp.txt", "tasks.txt");
        std::cout << "Task removed successfully: " << argv[2] << std::endl;
    }

    else if (Command == "-list") {
        std::ifstream inFile("tasks.txt");
        if (!inFile.is_open()) {
            std::cout << "Please initialize with \"-init\" first." << std::endl;
            return 1;
        }

        std::string line;
        std::cout << "Tasks:\n";
        while (std::getline(inFile, line)) {
            if (line != "$tasktracker")
                std::cout << "- " << line.substr(5) << std::endl;
        }
        inFile.close();
    }

    else {
        std::cout << "Invalid command." << std::endl;
        return 1;
    }

    return 0;
}
