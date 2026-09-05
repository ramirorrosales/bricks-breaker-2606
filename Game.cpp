#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	m_bricks.clear();
	for (int i = 0; i < 5; i++)
	{
		Box temporaryBrick;
		temporaryBrick.width = 10;
		temporaryBrick.height = 2;

		temporaryBrick.x_position = 5 + (i * 14);
		temporaryBrick.y_position = 5;

		temporaryBrick.doubleThick = true;
		temporaryBrick.color = ConsoleColor::DarkGreen;

		m_bricks.push_back(temporaryBrick);
		}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (size_t i = 0; i < m_bricks.size(); i++)
	{
		m_bricks[i].Draw();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (size_t i = 0; i < m_bricks.size(); i++)
	{
		int nextX = ball.x_position + ball.x_velocity;
		int nextY = ball.y_position + ball.y_velocity;

		if (m_bricks[i].Contains(nextX, nextY))
		{
			ball.y_velocity = ball.y_velocity * -1;

			int rawColorValue = (int)m_bricks[i].color - 1;
			m_bricks[i].color = (ConsoleColor)rawColorValue;
		}

		// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
		if (m_bricks[i].color == ConsoleColor::Black)
		{
			m_bricks.erase(m_bricks.begin() + i);

			i--;
		}
	}




	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (m_bricks.empty())
	{
		Console::SetCursorPosition(24, 15);
		std::cout << "You win! Press 'R' to play again.";
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (m_bricks.size() == 0 || ball.y_position >= 29)
	{
		ball.moving = false;
	}

	else if (ball.y_position >= 29)
	{
		Console::SetCursorPosition(24, 15);
		std::cout << "You lose. Press 'R' to play again.";
	}
}
