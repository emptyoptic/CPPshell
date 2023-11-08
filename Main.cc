#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <conio.h>

// Function to clear the input line
void ClearInputLine(HANDLE hConsole, int inputLineLength)
{
    DWORD charsWritten;
    FillConsoleOutputCharacter(hConsole, ' ', inputLineLength, { 0, 1 }, &charsWritten);
    SetConsoleCursorPosition(hConsole, { 0, 1 });
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { 80, 25 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, 79, 24 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    std::vector<std::string> commandHistory;
    int historyIndex = -1;
    std::string userInput;
    int inputLineLength = bufferSize.X;

    DWORD startWritten;

    SetConsoleCursorPosition(hConsole, { 0, 0 });
    char currentDirectory[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDirectory);
    WriteConsoleA(hConsole, currentDirectory, strlen(currentDirectory), &startWritten, NULL);

    SetConsoleCursorPosition(hConsole, { 0, 1 });
    WriteConsoleA(hConsole, ("~> " + userInput).c_str(), userInput.length() + 3, &startWritten, NULL);

    while (true)
    {
        DWORD charsWritten;
        int ch = _getch();

        if (ch == 13)
        {  // Enter key
            ClearInputLine(hConsole, inputLineLength);

            if (userInput == "quit")
            {
                break;
            }

            if (userInput.empty())
            {
                continue;
            }

            system(userInput.c_str());

            if (!userInput.empty())
            {
                commandHistory.push_back(userInput);
            }
            userInput.clear();
            historyIndex = -1;
        }
        else if (ch == 8)
        {  // Backspace
            if (!userInput.empty())
            {
                userInput.pop_back();
                ClearInputLine(hConsole, inputLineLength);
                SetConsoleCursorPosition(hConsole, { 0, 1 });
                WriteConsoleA(hConsole, ("~> " + userInput).c_str(), userInput.length() + 3, &charsWritten, NULL);
                SetConsoleCursorPosition(hConsole, { short(userInput.length() + 2), 1 });
            }
        }
        else if (ch == 72)
        {  // Up arrow (for command history)
            if (!commandHistory.empty())
            {
                if (historyIndex < static_cast<int>(commandHistory.size()) - 1)
                {
                    historyIndex++;
                }
                userInput = commandHistory[historyIndex];
                ClearInputLine(hConsole, inputLineLength);
                SetConsoleCursorPosition(hConsole, { 0, 1 });
                WriteConsoleA(hConsole, ("~> " + userInput).c_str(), userInput.length() + 3, &charsWritten, NULL);
                SetConsoleCursorPosition(hConsole, { short(userInput.length() + 2), 1 });
            }
        }
        else if (ch == 80)
        {  // Down arrow (for command history)
            if (!commandHistory.empty() && historyIndex >= 0)
            {
                historyIndex--;
                if (historyIndex >= 0)
                {
                    userInput = commandHistory[historyIndex];
                }
                else
                {
                    userInput.clear();
                }
                ClearInputLine(hConsole, inputLineLength);
                SetConsoleCursorPosition(hConsole, { 0, 1 });
                WriteConsoleA(hConsole, ("~> " + userInput).c_str(), userInput.length() + 3, &charsWritten, NULL);
                SetConsoleCursorPosition(hConsole, { short(userInput.length() + 2), 1 });
            }
        }
        else
        {
            userInput += ch;
        }

        SetConsoleCursorPosition(hConsole, { 0, 0 });
        char currentDirectory[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, currentDirectory);
        WriteConsoleA(hConsole, currentDirectory, strlen(currentDirectory), &charsWritten, NULL);

        SetConsoleCursorPosition(hConsole, { 0, 1 });
        WriteConsoleA(hConsole, ("~> " + userInput).c_str(), userInput.length() + 3, &charsWritten, NULL);
    }

    WriteConsoleA(hConsole, "Press any key to exit...", 23, NULL, NULL);
    _getch();

    return 0;
}
