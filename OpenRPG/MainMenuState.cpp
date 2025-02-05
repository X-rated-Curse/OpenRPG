#include "MainMenuState.h"

//Initaillizer functions
void MainMenuState::initVariables()
{

}

void MainMenuState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);
	if (!this->backgroundTexture.loadFromFile("Resources/image/Backgrounds/bg1.png"))
	{
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/R2.ttc"))
	{
		throw("메인메뉴 폰트로딩 실패");
	}
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenuestate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";
		int key_value = 0;
		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();

	this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");
	this->keybinds["MOVE_LEFT"] = this->supportedKeys->at("A");
	this->keybinds["MOVE_RIGHT"] = this->supportedKeys->at("D");
	this->keybinds["MOVE_UP"] = this->supportedKeys->at("W");
	this->keybinds["MOVE_DOWN"] = this->supportedKeys->at("S");
}

void MainMenuState::initButtons()
{
	this->buttons["GAME_STATE"] = new Button(100, 500, 250, 70,
		&this->font, L"새 게임!", 50,
		sf::Color(255, 0, 0,255), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20,50),
		sf::Color(70, 70, 70,0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20,0));

	this->buttons["SETTING_STATE"] = new Button(100, 600, 250, 70,
		&this->font, L"게임 설정", 50,
		sf::Color(0, 0, 0,255), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70,0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	this->buttons["EDITOR_STATE"] = new Button(100, 700, 250, 70,
		&this->font, L"에디터", 50,
		sf::Color(0, 0, 0,255), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	this->buttons["EXIT_STATE"] = new Button(100, 900, 250, 70,
		&this->font, L"종 료!", 50,
		sf::Color(0, 0, 0, 255), sf::Color(150, 150, 150,250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

}

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	:State(window, supportedKeys, states)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();

}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void MainMenuState::updateInput(const float & dt)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		this->background.setFillColor(sf::Color::Cyan);
}

void MainMenuState::updateButtons()
{
	//모든 버튼들의 상태를 기능에맞게 업데이트해줌
	for (auto &it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

	if (this->buttons["GAME_STATE"]->isPressed())
	{
		this->states->push(new GameState(this->window, this->supportedKeys, this->states));
	}

	//종료
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this -> endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateButtons();


}

void MainMenuState::renderButtons(sf::RenderTarget * target)
{
	for (auto &it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);
	this->renderButtons(target);

	//삭제예정. 디버깅용.
	sf::Text mouseText;
	mouseText.setPosition(sf::Vector2f(this->mousePosView.x, this->mousePosView.y-15));
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(15);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText);
}
