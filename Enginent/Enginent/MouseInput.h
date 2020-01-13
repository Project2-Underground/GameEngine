#pragma once

class MouseInput {


	glm::vec3 FindMousePosition(int x, int y)
	{
		float realX, realY;
		realX = -(winWidth * 0.5) + x;
		realY = -(winHeight * 0.5) + (winHeight - y);
		return glm::vec3(realX, realY, 1);
	}
};