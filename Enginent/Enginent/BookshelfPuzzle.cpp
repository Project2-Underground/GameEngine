#include "Puzzles.h"
#include "Game.h"
#include "MouseInput.h"

Puzzle::~Puzzle() {
	if(background)
		delete background;
	if(puzzle)
		delete puzzle;
	for (auto ui : UI)
		if(ui)
			delete ui;
}

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
	dialogueAfterComplete = "Hall_Bookshelf_L3";
	this->texture = new UIObject();
	this->texture->SetTexture(texture);
	this->texture->SetPosition(glm::vec3(posX, posY, 1));
	this->texture->SetSize((float)sizeX, (float)sizeY);
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
	}
}

void Bookshelf::LeftRelease(glm::vec3 screen, glm::vec3 world) {
	if (select) {
		bool placed = false;
		for (int i = 0; i < log.size(); i++)
		{
			if (log[i]->collider->isClicked(screen.x, screen.y))
			{
				Book* tmp = log[i]->book;
				log[i]->book = select;
				select->SetPosition(log[i]->collider->getPosition());
				select->col->setNewPos(log[i]->collider->getPosition());
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
	if (pass && !doneAction) {
		ActionAfterPuzzle();
		TextBox::GetInstance()->setText(dialogueAfterComplete);
		doneAction = true;
	}
}

void Bookshelf::Reset() {
	glm::vec3 originalPos[6] = { glm::vec3(252,87,1), glm::vec3(352, 87,1), glm::vec3(448, 87,1) ,
								 glm::vec3(252, -88,1) ,glm::vec3(448, -88,1) ,glm::vec3(352, -88,1) };
	for (int i = 0; i < books.size(); i++){
		books[i]->SetPosition(originalPos[i]);
		log[i]->book = nullptr;
	}
	pass = false;
	doneAction = false;
}

void Bookshelf::ActionAfterPuzzle() {
	Game::GetInstance()->GetCurrentLevel()->rooms["MainHallLower"]->doors["StairDoor"]->Open();
	TextBox::GetInstance()->setText("Hall_Bookshelf_L3");
}

void Bookshelf::CompletePuzzle() {
	int solveLog[6] = { 2,1,3,0,4,5 };

	for (int i = 0; i < 6; i++) {
		log[i]->book = books[solveLog[i]];
		books[solveLog[i]]->SetPosition(log[i]->collider->getPosition());
		books[solveLog[i]]->col->setNewPos(log[i]->collider->getPosition());
	}

	pass = true;
	ActionAfterPuzzle();
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
	currentLevel = 1;
	prepTalk.clear();
	enableInventory = false;
	std::vector<Book*> books1;
	std::vector<Space*> space1;
	std::vector<UIObject* > images;

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
	UI.push_back(closeButton);

	puzzle = new Bookshelf("Texture/Puzzle/Puzzle1_BookShelf.png", -300, 0, 200, -400);
	((Bookshelf*)puzzle)->Init(books1, space1, images);
}
void BookshelfPuzzle::Reset() {
	puzzle->Reset();
}
void BookshelfPuzzle::Render()
{
	((Bookshelf*)puzzle)->Render();
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
			button->checkColliderPressed(screen.x, screen.y);
		}
	}
}

bool BookshelfPuzzle::CheckRequirements() {
	if (passedReqiurements)
		return true;

	Inventory* inventory = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
	std::string requireBooks[4] = { "Book1","Book2", "Book3", "Book4" };
	for (int i = 0; i < 4; i++) {
		if (!inventory->IsItemInInventory(requireBooks[i]))
			return false;
	}
	for (int i = 0; i < 4; i++) {
		inventory->RemoveItem(requireBooks[i]);
	}
	prepTalk = "Hall_Bookshelf_L2";
	passedReqiurements = true;
	return true;
}
void BookshelfPuzzle::LeftRelease(glm::vec3 screen, glm::vec3 world) {
	if (puzzle->IsDisplay()) {
		puzzle->LeftRelease(screen, world);
	}
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkColliderReleased(screen.x, screen.y);
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

}

void BookshelfPuzzle::CompletePuzzle() {
	puzzle->CompletePuzzle();
}

Book2::Book2(int id, std::string texture, float sizeX, float sizeY, int posX, int posY, int paperID)
{
	this->paper_id = paperID;
	this->paper = nullptr;
	this->id = id;
	this->SetTexture(texture);
	this->SetSize(sizeX, sizeY);
	this->SetPosition(glm::vec3(posX, posY, 0));
	this->col = new Collider(this);
}

Book2::~Book2()
{

}

bool Book2::checkPaper()
{
	if (paper_id == paper->ID)
	{
		return true;
	}
	return false;
}

Bookshelf2::Bookshelf2(std::string texture, int posX, int posY, int sizeX, int sizeY)
{
	dialogueAfterComplete = "";
	this->texture = new UIObject();
	this->texture->SetTexture(texture);
	this->texture->SetPosition(glm::vec3(posX, posY, 1));
	this->texture->SetSize((float)sizeX, (float)sizeY);
}

void Bookshelf2::Init(std::vector<Book*> book1, std::vector<Book2*> book, std::vector<Space*> space, std::vector<UIObject*> img, std::vector<Paper*> paper)
{
	this->books_2 = book;
	this->books = book1;
	this->log = space;
	this->images = img;
	this->papers = paper;
}

void Bookshelf2::Render()
{
	for (int i = 0; i < images.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(images[i]);
	}
	for (int i = 0; i < log.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(log[i]->colTemp);
	}
	for (int i = 0; i < books.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(books[i]);
	}
	for (int i = 0; i < books_2.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(books_2[i]);
	}
	for (int i = 0; i < papers.size(); i++)
	{
		Game::GetInstance()->GetRenderer()->Render(papers[i]);
	}
}

void Bookshelf2::Update()
{
	glm::vec3 screen = MouseInput::GetInstance()->GetMouseScreenPosition();
	if (select != nullptr)
	{
		select->SetPosition(screen);
	}
	if (select_p != nullptr)
	{
		select_p->SetPosition(screen);
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
	for (int j = 0; j < books_2.size(); j++)
	{
		if (books_2[j]->paper == nullptr) {
			pass = false;
			break;
		}
		else if (books_2[j]->GetPaperid() != books_2[j]->paper->ID)
		{
			pass = false;
			break;
		}
	}
	if (pass && !doneAction) {
		ActionAfterPuzzle();
		doneAction = true;
		TextBox::GetInstance()->setText(dialogueAfterComplete);
	}
}

void Bookshelf2::Reset()
{
	glm::vec3 paperOriginalPos[8] = { glm::vec3(338, 263, 0) ,glm::vec3(338, 68, 0) ,glm::vec3(338, -89, 0) ,glm::vec3(338, -247, 0) ,glm::vec3(497, 263, 0) ,
									  glm::vec3(497, 68, 0) ,glm::vec3(497, -89, 0) ,glm::vec3(497, -247, 0) };

	glm::vec3 bookOriginalPosition[8] = { glm::vec3(-107, 268,1), glm::vec3(107, 268,1), glm::vec3(-107, 93,1), glm::vec3(107, 93,1),
										  glm::vec3(-107, -88,1), glm::vec3(107, -88,1), glm::vec3(-107, -269,1), glm::vec3(107, -269,1) };
	
	int logOriginalBook[8] = { 4,8,3,1,6,5,7,2 };

	for (int i = 0; i < 8; i++) {
		papers[i]->SetPosition(paperOriginalPos[i]);
		papers[i]->SetSize(229, -161);
		papers[i]->SetTexture(papers[i]->norm_paperTexture);
		papers[i]->col->CalNewBound();
		books[i]->SetPosition(bookOriginalPosition[i]);
		log[i]->book = FindBook(logOriginalBook[i]);
	}

	for (int i = 0; i < books_2.size(); i++) {
		books_2[i]->paper = nullptr;
	}
}

void Bookshelf2::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	if (!pass) {
		if (select == nullptr && select_p == nullptr)
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
			for (int j = 0; j < log.size(); j++)
			{
				if (log[j]->collider->isClicked(screen.x, screen.y))
				{
					select_s = log[j];
				}
			}
		}
		if (select_p == nullptr && select == nullptr)
		{
			for (int j = 0; j < papers.size(); j++)
			{
				if (papers[j]->col->isClicked(screen.x, screen.y) && !papers[j]->paste)
				{
					select_p = papers[j];
				}
			}
		}
	}
}

void Bookshelf2::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	if (select != nullptr && select_s != nullptr) {
		if (select_s->collider->isClicked(screen.x, screen.y))
		{
			Book* tmp = select_s->book;
			select_s->book = select;
			select->SetPosition(select_s->colTemp->getPos());
			select->col->setNewPos(select_s->colTemp->getPos());
			select = tmp;
			select_s = nullptr;
		}
	}
	if (select_p)
	{
		bool placed = false;
		for (int i = 0; i < books_2.size(); i++)
		{
			if (books_2[i]->col->isClicked(screen.x, screen.y) && books_2[i]->paper == nullptr)
			{
				select_p->paste = true;
				select_p->SetTexture(select_p->book_paperTexture);
				select_p->SetPosition(books_2[i]->getPos());
				select_p->SetSize(46, -124);
				select_p->col->setNewPos(books_2[i]->getPos());
				select_p->col->setNewSize(select_p->getSize());
				books_2[i]->paper = select_p;
				select_p = nullptr;
				placed = true;
				break;
			}
		}
		if (!placed)
		{
			select_p->SetPosition(select_p->prevPos);
			select_p->col->setNewPos(select_p->prevPos);
			select_p = nullptr;
		}
	}
}

void Bookshelf2::RightClick(glm::vec3 screen, glm::vec3 world)
{
	if (!pass)
	{
		if (select_r == nullptr)
		{
			std::cout << "Check\n";
			for (int i = 0; i < books_2.size(); i++)
			{
				if (books_2[i]->col->isClicked(screen.x, screen.y))
				{
					select_r = books_2[i];
				}
			}
		}
	}
}

void Bookshelf2::RightRelease(glm::vec3 screen, glm::vec3 world)
{
	if (select_r != nullptr)
	{
		if (select_r->col->isClicked(screen.x, screen.y) && select_r->paper != nullptr)
		{
			select_r->paper->SetPosition(select_r->paper->prevPos);
			select_r->paper->col->setNewPos(select_r->paper->prevPos);
			select_r->paper->paste = false;
			select_r->paper->SetTexture(select_r->paper->norm_paperTexture);
			select_r->paper->SetSize(229, -161);
		}
		select_r->paper = nullptr;
	}
	select_r = nullptr;
}

void Bookshelf2::ActionAfterPuzzle()
{
	Game* g = Game::GetInstance();
	Door* door = new ChangeLevelDoor(1);
	InteractableObj* obj = (InteractableObj*)g->GetCurrentLevel()->rooms["MainHallUpper"]->FindObject("HallRoomUpperFloor_Door1");
	door->Init(obj->getSize().x, obj->getSize().y, obj->getPos());
	door->SetTexture("Texture/MainHall/UpperFloor/HallRoomUpperFloor_Door1Open.png");
	g->GetCurrentLevel()->rooms["MainHallUpper"]->doors.insert(std::pair<std::string, Door*>("level2Door", door));
	g->GetCurrentLevel()->rooms["MainHallUpper"]->objects.push_back(door);
	obj->Appear(false);
	GameScreen* gs = ((GameScreen*)g->GetScreen());
	Butler* butler = gs->butler;
	butler->currentPhase = Butler::PHASE2;
	butler->Appear(); 
}

Book2* Bookshelf2::FindBook2(int id) {
	for (int i = 0; i < books_2.size(); i++) 
		if (books_2[i]->GetId() == id)
			return books_2[i];
	return nullptr;
}
Paper* Bookshelf2::FindPaper(int id) {
	for (int i = 0; i < papers.size(); i++)
		if (papers[i]->ID == id)
			return papers[i];
	return nullptr;
}
Book* Bookshelf2::FindBook(int id) {
	for (int i = 0; i < books.size(); i++)
		if (books[i]->GetId() == id)
			return books[i];
	return nullptr;
}
void Bookshelf2::CompletePuzzle()
{
	for (int i = 0; i < log.size(); i++) {
		Book* b = FindBook(log[i]->id);
		log[i]->book = b;
		b->SetPosition(log[i]->collider->getPosition());
		b->col->setNewPos(log[i]->collider->getPosition());
	}
	for (int i = 0; i < books_2.size(); i++) {
		Paper* p = FindPaper(books_2[i]->GetPaperid());
		books_2[i]->paper = p;
		p->SetTexture(p->book_paperTexture);
		p->SetPosition(books_2[i]->getPos());
		p->col->setNewPos(books_2[i]->getPos());
		p->SetSize(46, -124);
	}
	pass = true;
	ActionAfterPuzzle();
}

Bookshelf2::~Bookshelf2()
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
	for (int i = 0; i < books_2.size(); i++)
	{
		delete books_2[i];
	}
	for (int i = 0; i < papers.size(); i++)
	{
		delete papers[i];
	}
}

BookshelfPuzzle_2::BookshelfPuzzle_2()
{
	currentLevel = 1;
	prepTalk.clear();
	puzzle = new Bookshelf2("Texture/puzzle5/Puzzle5_BookShelf.png", 0, 0, 1280, -720);
	//void Init(std::vector<Book2*>, std::vector<Space2*>, std::vector<UIObject*>, std::vector<Paper*>);
	std::vector<Book2*> books;
	std::vector<Book*> books_1;
	std::vector<Space*> spaces;
	std::vector<UIObject*> images;
	std::vector<Paper*> papers;

	Book2* tmp_book = new Book2(11, "Texture/puzzle5/Puzzle5_IncomBook1.png", 65, -155, 0, 268, 1);
	books.push_back(tmp_book);
	tmp_book = new Book2(12, "Texture/puzzle5/Puzzle5_IncomBook2.png", 65, -155, 0, 98, 5);
	books.push_back(tmp_book);
	tmp_book = new Book2(13, "Texture/puzzle5/Puzzle5_IncomBook3.png", 65, -155, 0, -89, 6);
	books.push_back(tmp_book);
	tmp_book = new Book2(14, "Texture/puzzle5/Puzzle5_IncomBook4.png", 65, -155, 0, -270, 4);
	books.push_back(tmp_book);

	Book* tmp_book1 = new Book(4, "Texture/puzzle5/Puzzle5_WhiteBook4.png", 65, -155, -107, 268);
	books_1.push_back(tmp_book1);
	Space* tmp_space = new Space(1, glm::vec3(-107, 268, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(8, "Texture/puzzle5/Puzzle5_WhiteBook8.png", 65, -155, 107, 268);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(2, glm::vec3(107, 268, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(3, "Texture/puzzle5/Puzzle5_WhiteBook3.png", 65, -155, -107, 93);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(3, glm::vec3(-107, 93, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(1, "Texture/puzzle5/Puzzle5_WhiteBook1.png", 65, -155, 107, 93);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(4, glm::vec3(107, 93, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(6, "Texture/puzzle5/Puzzle5_WhiteBook6.png", 65, -155, -107, -88);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(5, glm::vec3(-107, -88, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(5, "Texture/puzzle5/Puzzle5_WhiteBook5.png", 65, -155, 107, -88);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(6, glm::vec3(107, -88, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(7, "Texture/puzzle5/Puzzle5_WhiteBook7.png", 65, -155, -107, -269);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(7, glm::vec3(-107, -269, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	tmp_book1 = new Book(2, "Texture/puzzle5/Puzzle5_WhiteBook2.png", 65, -155, 107, -269);
	books_1.push_back(tmp_book1);
	tmp_space = new Space(8, glm::vec3(107, -269, 0), 70, -160);
	tmp_space->book = tmp_book1;
	spaces.push_back(tmp_space);

	UIObject* tmp_image = new UIObject();
	tmp_image->SetTexture("Texture/puzzle5/Puzzle5_BookShelf.png");
	tmp_image->SetSize(1280, -720);
	tmp_image->SetPosition(glm::vec3(0, 0, 0));
	images.push_back(tmp_image);

	Paper* tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper1(I).png", "Texture/puzzle5/Puzzle5_Paper1.png", glm::vec3(338, 263, 0), 229, -161, 1);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper2(I).png", "Texture/puzzle5/Puzzle5_Paper2.png", glm::vec3(338, 68, 0), 229, -161, 2);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper3(I).png", "Texture/puzzle5/Puzzle5_Paper3.png", glm::vec3(338, -89, 0), 229, -161, 3);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper4(I).png", "Texture/puzzle5/Puzzle5_Paper4.png", glm::vec3(338, -247, 0), 229, -161, 4);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper5(I).png", "Texture/puzzle5/Puzzle5_Paper5.png", glm::vec3(497, 263, 0), 229, -161, 5);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper6(I).png", "Texture/puzzle5/Puzzle5_Paper6.png", glm::vec3(497, 68, 0), 229, -161, 6);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper7(I).png", "Texture/puzzle5/Puzzle5_Paper7.png", glm::vec3(497, -89, 0), 229, -161, 7);
	papers.push_back(tmp_paper);
	tmp_paper = new Paper("Texture/puzzle5/Puzzle5_Paper8(I).png", "Texture/puzzle5/Puzzle5_Paper8.png", glm::vec3(497, -247, 0), 229, -161, 8);
	papers.push_back(tmp_paper);

	ClosePuzzleButton* closeButton = new ClosePuzzleButton("Texture/Puzzle/CloseButton.png");
	closeButton->Init(46.0f, -44.0f, glm::vec3(617, 338, 1.0f));
	UI.push_back(closeButton);

	((Bookshelf2*)puzzle)->Init(books_1, books, spaces, images, papers);
}

void BookshelfPuzzle_2::Render()
{
	((Bookshelf2*)puzzle)->Render();
	Game::GetInstance()->GetRenderer()->Render(UI);
}

void BookshelfPuzzle_2::Update()
{
	((Bookshelf2*)puzzle)->Update();
}

void BookshelfPuzzle_2::Reset()
{
	puzzle->Reset();
}

bool BookshelfPuzzle_2::CheckRequirements()
{
	return true;
}

void BookshelfPuzzle_2::LeftClick(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->LeftClick(screen, world);
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkColliderPressed(screen.x, screen.y);
		}
	}
}

void BookshelfPuzzle_2::LeftRelease(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->LeftRelease(screen, world);
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkColliderReleased(screen.x, screen.y);
		}
	}
}

void BookshelfPuzzle_2::RightClick(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->RightClick(screen, world);
}

void BookshelfPuzzle_2::RightRelease(glm::vec3 screen, glm::vec3 world)
{
	((Bookshelf2*)puzzle)->RightRelease(screen, world);
}

void BookshelfPuzzle_2::UpdateMouseState(glm::vec3 screen, glm::vec3 world)
{

}

void BookshelfPuzzle_2::CompletePuzzle()
{
	puzzle->CompletePuzzle();
}

BookshelfPuzzle_2::~BookshelfPuzzle_2()
{
}