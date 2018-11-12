#include "pch.h"
#include "TextElement.h"
#include "Graphics.h"

TextElement::TextElement() :
{
}


TextElement::~TextElement()
{
}


void TextElement::Render(int x, int y, int width, ID3D10Device pDevice)
{
	// Draw box with border
	Draw.FullRectBordered(x, y, width, 20, 2, D3DCOLOR_ARGB(155, 000, 000, 000), D3DCOLOR_ARGB(255, 000, 000, 000), pDevice);
	// Draw command
	Draw.Text(x + 2, y + 2, D3DCOLOR_ARGB(255, 255, 165, 0), ">", pDevice);
	Draw.Text(x + 10, y + 2, D3DCOLOR_ARGB(255, 255, 255, 0), command.c_str(), pDevice);

	caretTick += 1;
	if (caretTick >= 25)
	{
		caretTick = 0;
		showCaret = !showCaret;
	}
	if (showCaret)
		Draw.Text(x + 6 + (caretPos * 8), y + 2, D3DCOLOR_ARGB(255, 255, 165, 0), "|", pDevice);
}


void TextElement::takeInput(WPARAM wParam)
{
	if (handled)
	{
		handled = false;
		return;
	}

	switch (wParam)
	{
	case '\b':    // backspace
	{
		if (caretPos>0)
		{
			command.erase(caretPos - 1, 1);
			caretPos -= 1;
		}
	}
	break;

	case '\r':    // return/enter
	{
		prevCommands.push_back(command);

		Send(command);
		command = "";
		caretPos = 0;
		cmdCount = prevCommands.size();
		currCmd = cmdCount;
	}
	break;
	case '\t':    // tab
	{
		if (cmdCount>0)
		{
			if (currCmd>0)
			{
				currCmd -= 1;
				command = prevCommands.at(currCmd);
				caretPos = command.length();
			}
			else
			{
				currCmd = cmdCount - 1;
				command = prevCommands.at(currCmd);
				caretPos = command.length();
			}
		}
	}
	break;

	default:
		command.insert(caretPos, 1, (char)wParam);
		caretPos += 1;
		break;
	} // switch(wParam)
}

void TextElement::Init()
{
	command = "";
	caretPos = 0;
	cmdCount = 0;
	currCmd = 0;
}

void(__cdecl *SendCommandToConsole)(int a1, int a2, const char *cvar) = (void(__cdecl *)(int, int, const char *))ADDR_CONSOLE;
void TextElement::Send(string cmd)
{
	size_t scPos, len;    // scPos = semicolon position. Len = length of cmd.
	char tmp[50];        // temporary char array for splitting multiple commands
	string cmdToSend;   // The command to be sent. Used for splitting multiple commands
	int i = 0;
	do
	{
		cmd.begin();
		scPos = cmd.find(';');
		if (scPos != string::npos)    // There was an ;. More than one command.
		{
			len = cmd.copy(tmp, int(scPos), 0);        // Copy the first command to tmp
			tmp[len] = '\0';                        // End tmp "string"
			cmdToSend = tmp;                        // Assign tmp as the command to be sent
			cmdToSend += '\n';
			SendCommandToConsole(1, 1, cmdToSend.c_str());
			cmd.erase(0, int(scPos) + 1);            // Erase the just-sent command, and the ;
		}
		else                        // There was no ;. Just a single command.
		{
			cmd += '\n';
			SendCommandToConsole(1, 1, cmd.c_str());
		}
	} while (scPos != string::npos);    // Only loop again if a ; was found in the last iteration.
}

void KeyPressed(WPARAM wParam)
{
	Console.handled = false;            // in case it's still true


	if (wParam == VK_SHIFT)            // allows us to use combinations such as Shift+(key)
		shift = true;
	if (wParam == VK_F1)                // F1 is our show/hide hotkey
	{
		Console.handled = true;
		if (console)
		{
			Console.Send("con_minicon 0");    // This is my lazy way of getting "feedback" from the console
			console = false;
		}
		else
		{
			Console.Send("con_minicon 1");
			console = true;
		}
	}

	if (console)        // If the console is visible, take input.
	{
		switch (wParam)
		{
		case VK_LEFT:
			if (Console.caretPos>0) Console.caretPos -= 1;    // Left key. Move the caret back 1 if it's not already at 0.
			break;
		case VK_RIGHT:
			if (Console.caretPos<(int)Console.command.length()) Console.caretPos += 1;    // Right key. Move the caret forward one if it's not already at the end of the string.
			break;
		case VK_DELETE:
			if (shift)                    // Shift+DEL are pressed.
			{
				Console.command = "";        // Empty the string
				Console.caretPos = 0;        // Reset Caret
			}
			else
			{
				// Delete the character in front of the caret if it's not at the end of the string
				// (Note that the caret stays in the same position)
				if (Console.caretPos<(int)Console.command.length()) Console.command.erase(Console.caretPos, 1);
			}
			break;
		case VK_ESCAPE:
		{
			Console.handled = true;
			console = false;            // Hide the console.
		}
		break;
		} // switch(wParam)
	} // if(console)
}