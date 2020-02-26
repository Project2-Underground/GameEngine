#include "Puzzles.h"
#include "Game.h"
#include "MouseInput.h"

Book::Book(int id, std::string texture, float sizeX, float sizeY, int posX, int posY)
{
	this->id = id;
	SetTexture(texture);
	SetSize(sizeX, sizeY);
	SetPosition(glm::vec3(posX, posY, 1));
	prevPos = this->pos;
	this->col = new Collider(this);
}

Book::~Book()
{
	if (col != nullptr)
		delete col;
}

int Book::GetId()
{
	return this->id;
}

void Book::UpdatePrevPos()
{
	//this->prevPos = this->getPos();
}

glm::vec3 Book::GetPrevPos()
{
	return prevPos;
}

bool Book::CheckCollider(float x, float y)
{
	if (this->col->isClicked(x, y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Space::CheckCollide(Collider* col)
{
	if (this->collider->isCollide(col))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Bookshelf::Bookshelf(std::string texture, int posX, int posY, int sizeX, int sizeY)
{
	this->texture = new UIObject();
	this->texture->SetTexture(texture);
	this->texture->SetPosition(glm::vec3(posX, posY, 1));
	this->texture->SetSize((float)sizeX, (float)sizeY);
	this->win = new UIObject();
	this->win->SetTexture("Texture/Test/Win.png");
	this->win->SetPosition(glm::vec3(0, 0, 1));
	this->win->SetSize(1080, -720);
}

void Bookshelf::Init(std::vector<Book*> books, std::vector<Space*> spaces, std::vector<UIObject*> image)
{
	this->books = books;
	this->log = spaces;
	this->images = image;
}

void Bookshelf::Render()
{
	for (int i = 0; i < images.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(images[i]);
	}
	for (int i = 0; i < books.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(books[i]);
	}
	
}

void Bookshelf::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	if (!pass) {
		if (select == nullptr)
		{
			for (int i = 0; i < books.size(); i++)
			{
				if (books[i]->col->isClicked(screen.x, screen.y))
				{
					select = books[i];
				}
			}
			for (int j = 0; j < log.size(); j++)
			{
				if (log[j]->collider->isClicked(screen.x, screen.y))
				{
					select = log[j]->book;
					log[j]->book = nullptr;
				}
			}
		}
		else
		{
			bool placed = false;
			for (int i = 0; i < log.size(); i++)
			{
				if (log[i]->collider->isClicked(screen.x, screen.y))
				{
					Book* tmp = log[i]->book;
					log[i]->book = select;
					select->SetPosition(log[i]->collider->getPosition());
					select->col->setNewPos(log[i]->collider->getPosition());
					select->UpdatePrevPos();
					select = tmp;
					placed = true;
					break;
				}
			}
			if (!placed)
			{
				select->SetPosition(select->GetPrevPos());
				select->col->setNewPos(select->GetPrevPos());
				select = nullptr;
			}
		}
	}
	else
	{
		ActionAfterPuzzle();
	}
}

void Bookshelf::Update()
{
	glm::vec3 screen = MouseInput::GetInstance()->GetMouseScreenPosition();
	if (select != nullptr)
	{
		select->SetPosition(screen);
	}
	pass = true;
	for (int i = 0; i < log.size(); i++)
	{
		if (log[i]->book == nullptr) {
			pass = false;
			break;
		}
		else if (log[i]->id != log[i]->book->GetId())
		{
			pass = false;
			break;
		}
	}
}

void Bookshelf::ActionAfterPuzzle() {
	Game::GetInstance()->GetCurrentLevel()->GetCurrentRoom()->doors["StairDoor"]->Open();
}

Bookshelf::~Bookshelf()
{
	for (int i = 0; i < images.size(); i++)
	{
		delete images[i];
	}
	for (int j = 0; j < log.size(); j++)
	{
		delete log[j];
	}
	for (int i = 0; i < books.size(); i++)
	{
		delete books[i];
	}
}

BookshelfPuzzle::BookshelfPuzzle()
{
	enableInventory = false;
	std::vector<Book*> books1;
	std::vector<Space*> space1;
	std::vector<UIObject* > images;

	background = new UIObject();
	background->SetTexture("Texture/Puzzle/Puzzle1_Background.png");
	background->SetSize(1280, -720);
	UI.push_back(background);

	UIObject* bg = new UIObject();
	bg->SetTexture("Texture/Puzzle/Puzzle1_Background.png");
	bg->SetSize(1280, -720);
	images.push_back(bg);

	UIObject* shelf = new UIObject();
	shelf->SetTexture("Texture/Puzzle/Puzzle1_BookShelf.png");
	shelf->SetPosition(glm::vec3(-213, 0, 1));
	shelf->SetSize(455, -736);
	images.push_back(shelf);

	UIObject* block = new UIObject();
	block->SetTexture("Texture/Puzzle/Puzzle1_Boxes.png");
	block->SetPosition(glm::vec3(358, 8, 1));
	block->SetSize(358, -417);
	images.push_back(block);

	Book* book1 = new Book(1, "Texture/Puzzle/Puzzle1_Book1.png", 56, -154, 252, 87);
	Book* book2 = new Book(2, "Texture/Puzzle/Puzzle1_Book2.png", 56, -154, 352, 87);
	Book* book3 = new Book(3, "Texture/Puzzle/Puzzle1_Book3.png", 56, -154, 448, 87);
	Book* book4 = new Book(4, "Texture/Puzzle/Puzzle1_Book4.png", 56, -154, 252, -88);
	Book* book5 = new Book(5, "Texture/Puzzle/Puzzle1_Book5.png", 56, -154, 448, -88);
	Book* book6 = new Book(6, "Texture/Puzzle/Puzzle1_Book6.png", 56, -154, 352, -88);
	Space* s1 = new Space(1, glm::vec3(-185, 93, 0), 57.0f, -155.0f);
	Space* s2 = new Space(2, glm::vec3(-303, 93, 0), 57.0f, -155.0f);
	Space* s3 = new Space(3, glm::vec3(-243, 267, 0), 57.0f, -155.0f);
	Space* s4 = new Space(4, glm::vec3(-242, -85, 0), 57.0f, -155.0f);
	Space* s5 = new Space(5, glm::vec3(-124, -85, 0), 57.0f, -155.0f);
	Space* s6 = new Space(6, glm::vec3(-303, -266, 0), 57.0f, -155.0f);

	books1.push_back(book1);
	books1.push_back(book2);
	books1.push_back(book3);
	books1.push_back(book4);
	books1.push_back(book5);
	books1.push_back(book6);
	space1.push_back(s3);
	space1.push_back(s2);
	space1.push_back(s4);
	space1.push_back(s1);
	space1.push_back(s5);
	space1.push_back(s6);

	ClosePuzzleButton* closeButton = new ClosePuzzleButton("Texture/Puzzle/CloseButton.png");
	closeButton->SetPosition(glm::vec3(505.0f, 181.0f, 1.0f));
	closeButton->SetSize(46.0f, -44.0f);
	closeButton->SetCollder(new Collider(closeButton));

	puzzle = new Bookshelf("Texture/Puzzle/Puzzle1_BookShelf.png", -300, 0, 200, -400);
	((Bookshelf*)puzzle)->Init(books1, space1, images);
	UI.push_back(closeButton);
	UI.push_back(puzzle);
}

void BookshelfPuzzle::Render()
{
	Game::GetInstance()->GetRenderer()->Render(UI);
}

void BookshelfPuzzle::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	if (puzzle->IsDisplay()) {
		puzzle->LeftClick(screen, world);
	}
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkCollider(screen.x, screen.y);
		}
	}
}

void BookshelfPuzzle::Update() {
	puzzle->Update();
}

void BookshelfPuzzle::UpdateMouseState(glm::vec3 screen, glm::vec3 world)
{
	/*if (puzzle->IsDisplay())
		((Bookshelf*)puzzle)->Update(screen, world);*/
}

BookshelfPuzzle::~BookshelfPuzzle()
{
	delete background;
	delete puzzle;
	for (auto ui : UI)
		delete ui;
}

void BookshelfPuzzle::CompletePuzzle() {

}