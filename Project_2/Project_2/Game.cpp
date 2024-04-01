#include "Game.h"


void Game::HandleInput()
{
    sf::Event ev;
    while (win.pollEvent(ev))
    {
        switch (ev.type)
        {
        case sf::Event::Closed:
            win.close();
            break;
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape) {
                cout << "Win close" << endl;
                win.close();
            }
            if (ev.key.code == sf::Keyboard::D || ev.key.code == sf::Keyboard::A) {
                if (StringSound.getStatus() != sf::Sound::Status::Playing) {
                    StringSound.play();
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                if (isOnGround) 
                {
                    Jump();
                }
            }
            break;
        case sf::Event::KeyReleased:
            if (ev.key.code == sf::Keyboard::D || ev.key.code == sf::Keyboard::A) {
                if (StringSound.getStatus() == sf::Sound::Status::Playing) {
                    StringSound.stop();
                }
            }
            break;
        }
    }
}

//Перевірка чи ігрок на поверхності
void Game::CheckCollisions() {
    //LocalBounds функції яка перевіряє ци доторнулись обєкти один до одного
    //Цей код перевіряє чи нижня межа позиція за осі Y плюс висота спрайту  досягає або перевищує висоту вікна 
    if (FoxSprite.getPosition().y + FoxSprite.getLocalBounds().height >= winSize.height) { //отримуємо висоту локальних меж спрайту 
        isOnGround = true; 
    } else isOnGround = false;
   
    //Беремо всі платформи які ми добавили і такж перевіряємо чи спрайт на землі
    for (const auto& shape : otherShapes) {
        if (FoxSprite.getGlobalBounds().intersects(shape.getGlobalBounds())) {
            isOnGround = true;
            break; 
        }
    }

    //Тут ми перевіряємо на межі гри
    if (FoxSprite.getPosition().x < 0) {
        FoxSprite.setPosition(0, FoxSprite.getPosition().y);
    }
    if (FoxSprite.getPosition().x + FoxSprite.getLocalBounds().width > winSize.width) {
        FoxSprite.setPosition(winSize.width - FoxSprite.getLocalBounds().width, FoxSprite.getPosition().y);
    }
    if (FoxSprite.getPosition().y < 50) {
        FoxSprite.setPosition(FoxSprite.getPosition().x, 50);
    }


    //а тут цікавіше тут ми спавнимо монету в випадковому місці при умові якщо він на 60 пікселей дальше за меж гри заігрока та за платформ та перевіряємо чи спрайт доторкнувся
    if (FoxSprite.getGlobalBounds().intersects(CoinSprite.getGlobalBounds())) { //Чесно сказати то з цією умовою допоміг інтернет якщо спрайт торкаєть до спрайт то (використовується для перевірки перетину між двома геометричними обєктами)
        Money++;
        StringSoundCoins.play();
        sf::Vector2f newPosition; //Нова позиція ігрока 
        do {
            newPosition.x = static_cast<float>(rand() % (winSize.width - 60)); //з int в float = static_cast<float> // ми віднімаємо висоту і довжину гри щоб було простіше для компютера тобто у нас 1280, 720 розміри ми їх віднімаємо і монета може спавнитись в 1220 та 640 
            newPosition.y = static_cast<float>(rand() % (winSize.height - 60)); 
        } while (!IsValidCoinPosition(newPosition)); 
        CoinSprite.setPosition(newPosition);
    }
}
bool Game::IsValidCoinPosition(const sf::Vector2f& position) const { //Перевірка на валідність, і з цим мені також допомг інтернет 
    for (const auto& shape : otherShapes) {
        if (shape.getGlobalBounds().contains(position)) return false;
    }
    if (FoxSprite.getGlobalBounds().contains(position)) return false;
    
    sf::Vector2f foxPosition = FoxSprite.getPosition();
    float distance = sqrt(pow(position.x - foxPosition.x, 2) + pow(position.y - foxPosition.y, 2)); // Цей код обчислює відстань між двома точками в двовимірному просторі за допомогою теореми Піфагора
    return distance >= 60.0f;
}


void Game::Jump() { //Прижок
    if (isOnGround) {
        FoxVelocityY = -600.0f; //Це так само як і для гравітації але замість того щоб опускатись ми піднімаємось
        isOnGround = false;
    }
}


void Game::UpDate(float deltaTime)
{
    if (isRestarting && restartClock.getElapsedTime().asSeconds() >= 3)
    {
        // Перезапускаємо гру
        Timer.setDuration(sf::seconds(30)); //Таймер для гри
        Timer.start(); // Початок
        isOnGround = false;
        FoxSprite.setPosition(spawnX, spawnY);
        ShowFox = true;
        EndText->setString("");
        CheckCollisions();
        isRestarting = false;
        Money = 0;
        
    }
    if (!otherShapes.empty() && otherShapes.size() > 5) //Перевіряємо чи є цей куб або платформа
    {
        sf::RectangleShape& obstacle6 = otherShapes[5]; 
        static bool moveLeft = true; 

        //І створюєио межі для руху
        if (moveLeft)
        {
            obstacle6.move(-0.05f, 0.0f) ; 
            if (obstacle6.getPosition().x <= 130)  moveLeft = false;
        }
        else
        {
            obstacle6.move(0.05f, 0.0f); 
            if (obstacle6.getPosition().x >= 900) moveLeft = true;
        }
    }

    //Текст для відображення монет
    stringstream ss;
    ss << "You have: ( " << Money << " ) Money";
    string playerPositionString = ss.str();
    CoinsText->setString(playerPositionString);
    

    //таймера
    stringstream ss2;
    ss2 << "Time: " << Timer.getRemainingTime().asSeconds();
    string playerPositionString2 = ss2.str();
    TimerText->setString(playerPositionString2);

    //якщо таймер закінчиний то спрайт знищується і виводиться текст
    if(Timer.getRemainingTime().asSeconds() <= 0 && restartClock.getElapsedTime().asSeconds() >= 4 && isRestarting == false)
    {
        ShowFox = false; 
        if(Money < 10)
        {
            EndText->setString("LOSE");
            EndText->setFillColor(sf::Color::Red);
            isRestarting = true; // Змінюємо на true, щоб підготуватися до перезапуску
            restartClock.restart(); // Перезапускаємо таймер перезапуску
            return;
        }
        EndText->setString("WINE");
        EndText->setFillColor(sf::Color::Green);
        isRestarting = true; // Змінюємо на true, щоб підготуватися до перезапуску
        restartClock.restart(); // Перезапускаємо таймер перезапуску
        return;
    }
    //Якщо ми натискаємо на w
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        Jump();
    }
    //О а тут ми створюємо анімацію тобто якщо тривалість тобто час який пройшов більше за  триваліть то  ми заново запускаємо час і переключаємо анімацію
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        FoxSprite.move(sf::Vector2f(+1, 0) * FoxSpead * deltaTime); 
        sf::Time elepsTime = foxAniter.getElapsedTime(); // беремо час який пройшов
        if(elepsTime > foxFrameInter)
        {
            foxAniter.restart(); // Перезапускаємо час
            FoxSprite.setTextureRect(animsFoxR[FrameIndex]); //міняємо анімацію
            FrameIndex += 1; // Та до i додаємо 1 щоб змінити анімацію
        }
        if(FrameIndex >= 4)
        {
            FrameIndex = 0;
        }
    }
    //Аналогічно
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        FoxSprite.move(sf::Vector2f(-1, 0) * FoxSpead * deltaTime);
        sf::Time elepsTime = foxAniter.getElapsedTime();
        if(elepsTime > foxFrameInter)
        {
            foxAniter.restart();
            FoxSprite.setTextureRect(animsFoxL[FrameIndex]);
            FrameIndex += 1;
        }
        if(FrameIndex >= 4)
        {
            FrameIndex = 0;
        }
    }

    //О А це гравітація тобто ігрок опускається в низ по ігрику до того як він не приземлиця
    FoxVelocityY += GRAVITY * deltaTime;
    FoxSprite.move(sf::Vector2f(0, FoxVelocityY * deltaTime));
    CheckCollisions();
    
    if (isOnGround)
    {
        FoxVelocityY = 0.0f;
    }
    
}

void Game::Draw()
{
    win.clear();
    if(ShowFox)
    {
        win.draw(FoxSprite);
    }
    for (const auto& shape : otherShapes) {
        win.draw(shape);
    }
    win.draw(CoinSprite);
    win.draw(boundaryTop);
    win.draw(boundaryLeft);
    win.draw(boundaryRight);
    win.draw(*CoinsText);
    win.draw(*TimerText);
    win.draw(*EndText);
    win.display();
}

Game::Game() : win(winSize, winTitle)
{
    //Загружамо всі фото
    string foxTextFilename = "One/fox.png";
    string IconsName = "One/mono.jpg";
    string MusicName = "music/foxstep.ogg";
    string CoinMus = "music/MoneyMus.ogg";
    string CoinPng = "One/coin.png";

    //перевіка чи загрузилось фото або щось інше
    if(!FoxTex.loadFromFile(foxTextFilename)) { cerr << "Error loading " << foxTextFilename << endl; }
    if(!Icons.loadFromFile(IconsName)) { cerr << "Error loading " << IconsName << endl; }
    if(!StringSoundBuffer.loadFromFile(MusicName)) { cerr << "Error open " << MusicName << endl; }
    if(!StringSoundBufferCoins.loadFromFile(CoinMus)) { cerr << "Error open " << CoinMus << endl; }
    if(!Coin.loadFromFile(CoinPng)) { cerr << "Error open " << CoinPng << endl; }

    
    StringSound.setBuffer(StringSoundBuffer); //StringSoundBuffer це загрузка музики а Sound вже для проігрування
    StringSoundCoins.setBuffer(StringSoundBufferCoins);
    win.setIcon(Icons.getSize().x, Icons.getSize().y, Icons.getPixelsPtr()); // Іконка я на початку зробив mono і так і залишив :)


    FoxSprite.setTexture(FoxTex); //Ми беремо текстуру і робимо hitbox
    Timer.setDuration(sf::seconds(30)); //Таймер для гри
    Timer.start(); // Початок

    
    CoinSprite.setTexture(Coin);
    CoinSprite.setScale(0.1, 0.1);
    CoinSprite.setPosition(10, 300);

    //Загрузка анімації
    animsFoxR.push_back({208, 140, 32, 46});
    animsFoxR.push_back({144, 142, 32, 46});
    animsFoxR.push_back({80, 140, 32, 48});
    animsFoxR.push_back({16, 142, 32, 46});

    animsFoxL.push_back({208, 76, 32, 46});
    animsFoxL.push_back({144, 78, 32, 46});
    animsFoxL.push_back({80, 76, 32, 48});
    animsFoxL.push_back({16, 78, 32, 46});

    animsFoxD.push_back({14, 12, 34, 48});
    animsFoxD.push_back({78, 10, 34, 52});
    animsFoxD.push_back({142, 12, 34, 48});
    animsFoxD.push_back({206, 10, 34, 52});

    animsFoxU.push_back({206, 204, 34, 50});
    animsFoxU.push_back({142, 206, 34, 46});
    animsFoxU.push_back({78, 204, 34, 50});
    animsFoxU.push_back({14, 206, 34, 46});
    
    FoxSprite.setTextureRect(animsFoxR[FrameIndex]);
    FoxSprite.setPosition(spawnX, spawnY);


    // Створення платформ
    sf::RectangleShape obstacle1(sf::Vector2f(100, 50));
    obstacle1.setPosition(200, 550);
    otherShapes.push_back(obstacle1); 

    sf::RectangleShape obstacle2(sf::Vector2f(80, 30)); 
    obstacle2.setPosition(400, 400);
    otherShapes.push_back(obstacle2);

    sf::RectangleShape obstacle3(sf::Vector2f(100, 30)); 
    obstacle3.setPosition(10, 350);
    otherShapes.push_back(obstacle3);

    sf::RectangleShape obstacle4(sf::Vector2f(100, 50));
    obstacle4.setPosition(750, 500);
    otherShapes.push_back(obstacle4); 

    sf::RectangleShape obstacle5(sf::Vector2f(100, 50));
    obstacle5.setPosition(1030, 300);
    otherShapes.push_back(obstacle5);

    sf::RectangleShape obstacle6(sf::Vector2f(150, 30)); 
    obstacle6.setPosition(130, 250);
    otherShapes.push_back(obstacle6);

//Межі
    boundaryTop.setSize(sf::Vector2f(winSize.width, 10)); 
    boundaryTop.setPosition(0, 50);
    boundaryTop.setFillColor(sf::Color::Green);

    boundaryLeft.setSize(sf::Vector2f(10, winSize.height));
    boundaryLeft.setPosition(0, 0);
    boundaryLeft.setFillColor(sf::Color::Green);

    boundaryRight.setSize(sf::Vector2f(10, winSize.height));
    boundaryRight.setPosition(winSize.width - 5, 0);
    boundaryRight.setFillColor(sf::Color::Green);
//текст
    CoinsText = new sf::Text();
    TimerText = new sf::Text();
    EndText = new sf::Text();
    
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error loading font!" << endl;
    }
    CoinsText->setFont(font);
    CoinsText->setCharacterSize(20); 
    CoinsText->setFillColor(sf::Color::White);
    CoinsText->setPosition(20.0f, 10.0f);
    
    
    TimerText->setFont(font);
    TimerText->setCharacterSize(20); 
    TimerText->setFillColor(sf::Color::White);
    TimerText->setPosition(win.getSize().x - 150.0f, 10.0f);
    
    EndText->setFont(font);
    EndText->setCharacterSize(100); 
    EndText->setFillColor(sf::Color::White);
    EndText->setPosition(win.getSize().x / 2 - 120, win.getSize().y / 2);
}


void Game::Run()
{
    sf::Clock deltaTimeClock;
    while (win.isOpen())
    {
        
        sf::Time deltaTime = deltaTimeClock.restart();
        HandleInput();
        UpDate(deltaTime.asSeconds());
        Draw();
    }
}
