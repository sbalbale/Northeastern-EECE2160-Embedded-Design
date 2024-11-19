#include <iostream>
#include "Spider.cpp"

using namespace std;

int main(int argc, char *argv[])
{
	Spider Spider;

	cout << "Spider Init" << endl;
	Spider.Init();

	cout << "Spider Standup" << endl;
	Spider.Standup();

	cout << "Waiting for Command..." << endl;
	bool done = false;
	while (!done)
	{
		char cmd_chr;
		cout << "Enter Next Command: ";
		cin >> cmd_chr;

		switch (cmd_chr)
		{
		case 'f':
			cout << "CMD_FORDWARD" << endl;
			Spider.MoveForward();
			break;
		case 'b':
			cout << "CMD_BACKWARD" << endl;
			Spider.MoveBackward();
			break;
		case 'l':
			cout << "CMD_LEFT" << endl;
			Spider.TurnLeft();
			break;
		case 'r':
			cout << "CMD_RIGHT" << endl;
			Spider.TurnRight();
			break;
		case 's':
			cout << "CMD_STOP" << endl;
			done = true;
			break;
		default:
			cout << "IDLE or UNKNOWN COMMAND" << endl;
			break;
		}
	}

	return 0;
}
