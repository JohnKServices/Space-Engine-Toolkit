std::vector<std::string> inputHandler(std::string fullInput)
{
	std::stringstream ss(fullInput);
	std::string buf;

	std::vector<std::string> tokens;
	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	SetConsoleCursorPosition(console, topLeft);
}

struct commandHandlerStruct
{
	char* input;
	char padding[10];
	DWORD charcount;
};

typedef void(__stdcall* FN_PRINT_TO_CONSOLE_PNT)(char* a1, char* a2);
FN_PRINT_TO_CONSOLE_PNT printToConsole;
typedef void(__stdcall* FN_COMMAND_HANDLER_PNT)(commandHandlerStruct* input);
FN_COMMAND_HANDLER_PNT commandHandler;


DWORD WINAPI NewThread(HMODULE hModule)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONIN$", "r", stdin);

	const unsigned int moduleBase = 0x400000;

	printToConsole = (FN_PRINT_TO_CONSOLE_PNT)(((DWORD)moduleBase) + 0x154850);
	commandHandler = (FN_COMMAND_HANDLER_PNT)(((DWORD)moduleBase) + 0x151110);

	float* testFloatVar = (float*)(((DWORD)moduleBase) + 0x4618BC);
	int* testIntVar = (int*)(((DWORD)moduleBase) + 0x4618B8);

	commandHandlerStruct commandStruct;
  
	std::string input;
	std::string output;

	while (true)
	{
		std::getline(std::cin, input);
		std::vector<std::string> tokens = inputHandler(input);

		if (tokens[0] == "consolePrint:")
		{
			for (int i = 1; i < tokens.size(); i++)
			{
				output += tokens[i] + ' ';
			}
			printToConsole((char*)output.c_str(), (char)0);
		}
		else if (tokens[0] == "setVar:")
		{
			if (tokens[1] == "testFloat")
			{
				*testFloatVar = (float)atof(tokens[2].c_str());
			}
			else if (tokens[1] == "testInt")
			{
				*testIntVar = (int)round(atof(tokens[2].c_str()));
			}
		}
		else if (tokens[0] == "getVar:")
		{
			if (tokens[1] == "testFloat")
			{
				std::cout << "Value of testFloat == " << *testFloatVar << std::endl;
			}
			else if (tokens[1] == "testInt")
			{
				std::cout << "Value of testInt == " << *testIntVar << std::endl;
			}
		}
		else if (tokens[0] == "clear")
		{
			clear();
		}
		else if (tokens[0] == "command:")
		{
			for (int i = 1; i < tokens.size(); i++)
			{
				if (i < tokens.size() - 1)
				{
					output += tokens[i] + ' ';
				} else if (i == tokens.size() - 1)
				{
					output += tokens[i];
				}
			}
			commandStruct.input = (char*)output.c_str();
			commandStruct.charcount = output.size();
			commandHandler(&commandStruct);
		} 
		else
		{
			std::cout << "Command not found! Perhaps misspelled?\n";
		}

		output = "";
		input = "";
		tokens = {}; // All of these are just in case

		Sleep(5);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)NewThread, hModule, 0, nullptr));
		break;
	}
	}
	return TRUE;
}
