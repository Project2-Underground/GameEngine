#include "Puzzles.h"
#include "Game.h"
#include "MouseInput.h"

//this->win = new UIObject();
//this->win->SetTexture("Texture/Test/Win.png");
//this->win->SetPosition(glm::vec3(0, 0, 1));
//this->win->SetSize(1080, -720);

NumberButton::NumberButton(std::string texture, std::string press, int n, float posX, float posY, float sizeX, float sizeY, std::vector<int>* input) : Button(texture) {
	this->pressTexture = Game::GetInstance()->GetRenderer()->LoadTexture(press);
	this->num = n;
	Init(sizeX, sizeY, glm::vec3(posX, posY, 1));
	this->input = input;
}

void NumberButton::action()
{
	if ((*input).size() < 4)
	{
		(*input).push_back(num);
		std::cout << num;
	}

}

EnterButton::EnterButton(std::string texture, std::string press, float posX, float posY, float sizeX, float sizeY, Numpad* n) : Button(texture) {
	this->SetTexture(texture);
	this->pressTexture = Game::GetInstance()->GetRenderer()->LoadTexture(press);
	SetPosition(glm::vec3(posX, posY, 1));
	SetSize(sizeX, sizeY);
	numpad = n;
	Init(sizeX, sizeY, glm::vec3(posX, posY, 1));
}

void EnterButton::action()
{
	numpad->CheckCheat();
}

DeleteButton::DeleteButton(std::string texture, std::string press, float posX, float posY, float sizeX, float sizeY, Numpad* n) : Button(texture) {
	this->pressTexture = Game::GetInstance()->GetRenderer()->LoadTexture(press);
	SetPosition(glm::vec3(posX, posY, 1));
	SetSize(sizeX, sizeY);
	numpad = n;
	Init(sizeX, sizeY, glm::vec3(posX, posY, 1));
}

void DeleteButton::action() {
	numpad->deleteInput();
}

Numpad::Numpad(std::string texture, int posX, int posY, int sizeX, int sizeY)
{
	this->texture = new UIObject();
	this->texture->SetTexture(texture);
	this->texture->SetPosition(glm::vec3(posX, posY, 1));
	this->texture->SetSize((float)sizeX, (float)sizeY);
	pass = false;
}

void Numpad::Init(std::vector<UIObject*> image, std::vector<Button*> b, int* c, int c_size, std::vector<int>* input)
{
	this->image = image;
	n_button = b;
	cheat = c;
	cheat_size = c_size;
	numUI = new TextObject[cheat_size];
	for (int i = 0; i < cheat_size; i++)
	{
		numUI[i].SetPosition(glm::vec3(-150 + (100 * i), 266, 1));
		numUI[i].loadText(" ", numColor, 100);
	}
	this->input = input;
}

void Numpad::Render()
{
	if (display)
	{
		for (int i = 0; i < image.size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(image[i]);
		}
		for (int i = 0; i < n_button.size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(n_button[i]);
		}
		for (int i = 0; i < input->size(); i++)
		{
			Game::GetInstance()->GetRenderer()->Render(&numUI[i]);
		}
	}
}

void Numpad::Update()
{
	glm::vec3 screen = MouseInput::GetInstance()->GetMouseScreenPosition();
	if (pass && !doneAction) {
		ActionAfterPuzzle();
		doneAction = true;
		std::cout << "pass";
	}
	for (int i = 0; i < (*input).size(); i++)
	{
		numUI[i].loadText(to_string((*input)[i]), numColor, 100);
	}
}

void Numpad::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	for (int i = 0; i < n_button.size(); i++)
	{
		n_button[i]->checkColliderPressed(screen.x, screen.y);
	}
}

void Numpad::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	for (int i = 0; i < n_button.size(); i++)
	{
		n_button[i]->checkColliderReleased(screen.x, screen.y);
	}
}

void Numpad::ActionAfterPuzzle()
{
	Game* g = Game::GetInstance();

	NumPadPuzzleAfter* tmp = new NumPadPuzzleAfter();
	InteractableObj* puzzleObj = (InteractableObj*)g->GetCurrentLevel()->GetCurrentRoom()->FindObject("BackDoorRoom_Computer");
	tmp->Init(puzzleObj->getSize().x, puzzleObj->getSize().y, puzzleObj->getPos());
	tmp->SetTexture(puzzleObj->GetTexture());
	g->GetCurrentLevel()->GetCurrentRoom()->objects.push_back(tmp);
	puzzleObj->col->enable = false;
	puzzleObj->SetDisplay(false);
	((GameScreen*)Game::GetInstance()->GetScreen())->ClosePuzzle();
}

void Numpad::CheckCheat()
{
	pass = true;
	if ((*input).size() < cheat_size)
	{
		pass = false;
	}
	else 
	{
		for (int i = 0; i < cheat_size; i++)
		{
			if ((*input)[i] != cheat[i])
			{
				pass = false;
				(*input).clear();
				break;
			}
		}
	}
}

void Numpad::deleteInput()
{
	if ((*input).size() > 0)
	{
		(*input).pop_back();
	}
}

Numpad::~Numpad()
{
	delete cheat;
	for (int i = 0; i < image.size(); i++)
	{
		delete image[i];
	}
	for (int i = 0; i < n_button.size(); i++)
	{
		delete n_button[i];
	}
}

NumpadPuzzle::NumpadPuzzle()
{
	puzzle = new Numpad("Texture/Puzzle2/buttonscreen.png", 0, 0, 2560, -1440);

	std::vector<UIObject*> images;
	std::vector<Button*> buttons;

	UIObject* bg = new UIObject();
	bg->SetTexture("Texture/Puzzle2/buttonscreen.png");
	bg->SetPosition(glm::vec3(0, 0, 1));
	bg->SetSize(1280, -720);
	images.push_back(bg);

	UIObject* symbol = new UIObject();
	symbol->SetTexture("Texture/Puzzle2/buttonsymbols.png");
	symbol->SetPosition(glm::vec3(0, -50, 1));
	symbol->SetSize(1018, -641);
	images.push_back(symbol);

	UIObject* space = new UIObject();
	space->SetTexture("Texture/Puzzle2/buttonspaces.png");
	space->SetPosition(glm::vec3(0, 215, 1));
	space->SetSize(371, -15);
	images.push_back(space);

	int* c = new int[4];
	c[0] = 2;
	c[1] = 0;
	c[2] = 4;
	c[3] = 6;
	std::vector<int>* input = new std::vector<int>();

	NumberButton* n1 = new NumberButton("Texture/Puzzle2/button1.png", "Texture/Puzzle2/button1press.png", 1, -126, 118, 118, -118, input);
	NumberButton* n2 = new NumberButton("Texture/Puzzle2/button2.png", "Texture/Puzzle2/button2press.png", 2, 0, 118, 118, -118, input);
	NumberButton* n3 = new NumberButton("Texture/Puzzle2/button3.png", "Texture/Puzzle2/button3press.png", 3, 126, 118, 118, -118, input);
	NumberButton* n4 = new NumberButton("Texture/Puzzle2/button4.png", "Texture/Puzzle2/button4press.png", 4, -126, -9, 118, -118, input);
	NumberButton* n5 = new NumberButton("Texture/Puzzle2/button5.png", "Texture/Puzzle2/button5press.png", 5, 0, -9, 118, -118, input);
	NumberButton* n6 = new NumberButton("Texture/Puzzle2/button6.png", "Texture/Puzzle2/button6press.png", 6, 126, -9, 118, -118, input);
	NumberButton* n7 = new NumberButton("Texture/Puzzle2/button7.png", "Texture/Puzzle2/button7press.png", 7, -126, -135, 118, -118, input);
	NumberButton* n8 = new NumberButton("Texture/Puzzle2/button8.png", "Texture/Puzzle2/button8press.png", 8, 0, -135, 118, -118, input);
	NumberButton* n9 = new NumberButton("Texture/Puzzle2/button9.png", "Texture/Puzzle2/button9press.png", 9, 126, -135, 118, -118, input);
	NumberButton* n0 = new NumberButton("Texture/Puzzle2/button0.png", "Texture/Puzzle2/button0press.png", 0, 0, -263, 118, -118, input);
	EnterButton* e = new EnterButton("Texture/Puzzle2/buttonenter.png", "Texture/Puzzle2/buttonenterpress.png", 126, -263, 118, -118, (Numpad*)puzzle);
	DeleteButton* d = new DeleteButton("Texture/Puzzle2/buttondel.png", "Texture/Puzzle2/buttondelpress.png", -126, -263, 118, -118, (Numpad*)puzzle);

	buttons.push_back(n1);
	buttons.push_back(n2);
	buttons.push_back(n3);
	buttons.push_back(n4);
	buttons.push_back(n5);
	buttons.push_back(n6);
	buttons.push_back(n7);
	buttons.push_back(n8);
	buttons.push_back(n9);
	buttons.push_back(n0);
	buttons.push_back(e);
	buttons.push_back(d);

	((Numpad*)puzzle)->Init(images, buttons, c, 4, input);
}

bool NumpadPuzzle::CheckRequirements() {
	return true;
}

void NumpadPuzzle::Render()
{
	puzzle->Render();
}

void NumpadPuzzle::Update()
{
	puzzle->Update();
}

void NumpadPuzzle::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	puzzle->LeftClick(screen, world);
}

void NumpadPuzzle::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	puzzle->LeftRelease(screen, world);
}

void NumpadPuzzle::UpdateMouseState(glm::vec3, glm::vec3)
{
	
}

void NumpadPuzzle::CompletePuzzle()
{

}

NumpadPuzzle::~NumpadPuzzle()
{

}


////////////////////////////////////
//Numpad 2
////////////////////////////////////

NumpadPuzzle_2::NumpadPuzzle_2()
{
	puzzle = new Numpad("Texture/Puzzle2/buttonscreen.png", 0, 0, 2560, -1440);

	std::vector<UIObject*> images;
	std::vector<Button*> buttons;

	UIObject* bg = new UIObject();
	bg->SetTexture("Texture/Puzzle2/black.png");
	bg->SetPosition(glm::vec3(0, 0, 1));
	bg->SetSize(1280, -720);
	images.push_back(bg);

	UIObject* symbol = new UIObject();
	symbol->SetTexture("Texture/Puzzle2/buttonsymbols.png");
	symbol->SetPosition(glm::vec3(0, -50, 1));
	symbol->SetSize(1018, -641);
	images.push_back(symbol);

	UIObject* space = new UIObject();
	space->SetTexture("Texture/Puzzle2/buttonspaces.png");
	space->SetPosition(glm::vec3(0, 215, 1));
	space->SetSize(371, -15);
	images.push_back(space);

	int* c = new int[3];
	c[0] = 2;
	c[1] = 0;
	c[2] = 6;
	std::vector<int>* input = new std::vector<int>();

	NumberButton* n1 = new NumberButton("Texture/Puzzle2/button1.png", "Texture/Puzzle2/button1press.png", 1, -126, 118, 118, -118, input);
	NumberButton* n2 = new NumberButton("Texture/Puzzle2/button2.png", "Texture/Puzzle2/button2press.png", 2, 0, 118, 118, -118, input);
	NumberButton* n3 = new NumberButton("Texture/Puzzle2/button3.png", "Texture/Puzzle2/button3press.png", 3, 126, 118, 118, -118, input);
	NumberButton* n4 = new NumberButton("Texture/Puzzle2/button4.png", "Texture/Puzzle2/button4press.png", 4, -126, -9, 118, -118, input);
	NumberButton* n5 = new NumberButton("Texture/Puzzle2/button5.png", "Texture/Puzzle2/button5press.png", 5, 0, -9, 118, -118, input);
	NumberButton* n6 = new NumberButton("Texture/Puzzle2/button6.png", "Texture/Puzzle2/button6press.png", 6, 126, -9, 118, -118, input);
	NumberButton* n7 = new NumberButton("Texture/Puzzle2/button7.png", "Texture/Puzzle2/button7press.png", 7, -126, -135, 118, -118, input);
	NumberButton* n8 = new NumberButton("Texture/Puzzle2/button8.png", "Texture/Puzzle2/button8press.png", 8, 0, -135, 118, -118, input);
	NumberButton* n9 = new NumberButton("Texture/Puzzle2/button9.png", "Texture/Puzzle2/button9press.png", 9, 126, -135, 118, -118, input);
	NumberButton* n0 = new NumberButton("Texture/Puzzle2/button0.png", "Texture/Puzzle2/button0press.png", 0, 0, -263, 118, -118, input);
	EnterButton* e = new EnterButton("Texture/Puzzle2/buttonenter.png", "Texture/Puzzle2/buttonenterpress.png", 126, -263, 118, -118, (Numpad*)puzzle);
	DeleteButton* d = new DeleteButton("Texture/Puzzle2/buttondel.png", "Texture/Puzzle2/buttondelpress.png", -126, -263, 118, -118, (Numpad*)puzzle);

	buttons.push_back(n1);
	buttons.push_back(n2);
	buttons.push_back(n3);
	buttons.push_back(n4);
	buttons.push_back(n5);
	buttons.push_back(n6);
	buttons.push_back(n7);
	buttons.push_back(n8);
	buttons.push_back(n9);
	buttons.push_back(n0);
	buttons.push_back(e);
	buttons.push_back(d);

	((Numpad*)puzzle)->Init(images, buttons, c, 3, input);
}

bool NumpadPuzzle_2::CheckRequirements() {
	return true;
}

void NumpadPuzzle_2::Render()
{
	puzzle->Render();
}

void NumpadPuzzle_2::Update()
{
	puzzle->Update();
}

void NumpadPuzzle_2::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	puzzle->LeftClick(screen, world);
}

void NumpadPuzzle_2::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	puzzle->LeftRelease(screen, world);
}

void NumpadPuzzle_2::UpdateMouseState(glm::vec3, glm::vec3)
{

}

void NumpadPuzzle_2::CompletePuzzle()
{

}

NumpadPuzzle_2::~NumpadPuzzle_2()
{

}