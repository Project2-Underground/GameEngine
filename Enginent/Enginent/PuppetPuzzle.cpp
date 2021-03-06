#include "Puzzles.h"
#include "Game.h"

PuppetPuzzle::PuppetPuzzle() {
	passed = false;
	UIObject* background = new UIObject();
	background->SetTexture("Texture/level2/Building4/Puzzle7/ItemView/Puzzle7_Puppet(V).png");
	background->SetSize(1280, 720);
	UI.push_back(background);

	ClosePuzzleButton* closeButton = new ClosePuzzleButton("Texture/tmp_texture/CloseButton.png");
	closeButton->Init(46.0f, -44.0f, glm::vec3(597, 318, 1.0f));
	UI.push_back(closeButton);
}
void PuppetPuzzle::Render() {
	GLRenderer* renderer = Game::GetInstance()->GetRenderer();
	renderer->Render(UI);
}
void PuppetPuzzle::Update() {

}
bool PuppetPuzzle::CheckRequirements() {
	if (passed)
		return false;
	Inventory* i = ((GameScreen*)Game::GetInstance()->GetScreen())->GetInventory();
	bool hasEyeKey = i->IsItemInInventory("Puzzle7_EyeKey");
	bool hasEarKey = i->IsItemInInventory("Puzzle7_EarKey");
	bool hasMouthKey = i->IsItemInInventory("Puzzle7_MouthKey");

	if (hasEarKey && hasEyeKey && hasMouthKey) {
		i->RemoveItem("Puzzle7_EyeKey");
		i->RemoveItem("Puzzle7_EarKey");
		i->RemoveItem("Puzzle7_MouthKey");
		i->RemoveItem("Puzzle7_MedPrescript");
		TextBox::GetInstance()->setText("puzzle7_puppet_item");
		passed = true;
	}
	else {
		TextBox::GetInstance()->setText("puzzle7_puppet_after");
	}
	return true;
}
void PuppetPuzzle::LeftClick(glm::vec3 screen, glm::vec3 world) {
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkColliderPressed(screen.x, screen.y);
		}
	}
}
void PuppetPuzzle::LeftRelease(glm::vec3 screen, glm::vec3 world) {
	for (int j = 0; j < UI.size(); j++)
	{
		if (Button * button = dynamic_cast<Button*>(UI[j]))
		{
			button->checkColliderReleased(screen.x, screen.y);
		}
	}
}
bool PuppetPuzzle::Passed() {
	return passed;
}