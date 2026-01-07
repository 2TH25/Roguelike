#include "bits/RogueCMI.h"

#include <gf/Window.h>
#include <gf/Circ.h>
#include <gf/Collision.h>
#include <gf/Shapes.h>
#include <gf/Views.h>
#include <gf/Font.h>
#include <gf/Text.h>
#include <gf/Texture.h>
#include <gf/Tileset.h>
#include <gf/TileLayer.h>
#include <gf/Random.h>

#include "./bits/Map.h"
#include "./bits/Map.cc"


int main() {
    gf::Window window("Rogue CMI", {1200, 800});
    gf::RenderWindow renderer(window);

    gf::Texture FloorTexture("../data/RogueCMI/sol_pierre.jpg");
    gf::Texture WallTexture("../data/RogueCMI/mur_pierre.jpg");
    gf::Random random;

    rCMI::Map myMap = rCMI::generate_dungeon(
        {10, 10},       
        10,              
        6, 10,           
        3,               
        &random,         
        texture  
    );

    while (window.isOpen()) {
        gf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == gf::EventType::Closed) {
                window.close();
            }
        }

        renderer.clear(gf::Color::Black);
        renderer.draw(myMap.tileLayer);
        renderer.display();
    }

    return 0;
}


// exemple d'utilisation pour créer le plateau 10x10 de manière très simple, dns le main

// int main() {
//     gf::Window window("Rogue CMI", {1200, 800});
//     gf::RenderWindow renderer(window);

//     gf::Texture texture("../data/RogueCMI/skeleton.png");

//     gf::Vector2i mapSize(10, 10);
//     gf::Vector2i tileSize(80, 80);

//     auto tileLayer = gf::TileLayer::createOrthogonal(mapSize, tileSize);


//     std::size_t tilesetId = tileLayer.createTilesetId();
//     gf::Tileset& tileset = tileLayer.getTileset(tilesetId);
//     tileset.setTexture(texture);
//     tileset.setTileSize(tileSize);


//     for (int y = 0; y < mapSize.y; ++y) {
//         for (int x = 0; x < mapSize.x; ++x) {
//             tileLayer.setTile({x, y}, tilesetId, 0); 
//         }
//     }

//     while (window.isOpen()) {
//         gf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == gf::EventType::Closed) {
//                 window.close();
//             }
//         }

//         renderer.clear(gf::Color::Black);
//         renderer.draw(tileLayer);
//         renderer.display();
//     }

//     return 0;
// }



//static constexpr float Speed = 1000.0f;

// int main()
// {

  // static constexpr gf::Vector2i ScreenSize(1350, 1350/2);
  // static constexpr gf::RectF World = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 2000.0f, 1000.0f });

  // gf::Window window("test", ScreenSize, ~gf::WindowHints::Resizable);
  // gf::RenderWindow renderer(window);

  // gf::ExtendView view(World);
  // view.setInitialFramebufferSize(ScreenSize);

  // renderer.setView(view);

  // gf::Texture texture("../data/RogueCMI/skeleton.png");
  // texture.setSmooth();

  // gf::Animation animation;
  // animation.addTileset(texture, gf::vec(4, 2), gf::milliseconds(100), 8);

  // gf::AnimatedSprite animatedSprite;
  // animatedSprite.setAnimation(animation);
  // animatedSprite.setOrigin({224 / 2.0f, 364 / 2.0f});
  // animatedSprite.setPosition({1000, 800});

  // gf::RectF itBox;
  // itBox.min = animatedSprite.getPosition() - animatedSprite.getOrigin();
  // itBox.max = animatedSprite.getPosition() + animatedSprite.getOrigin();

  // gf::Vector2f velocity(0.0f, 0.0f);

  // gf::CircF balls[5];

  // for (std::size_t k = 0; k < 5; ++k) {
  //   gf::CircF& circle = balls[k];

  //   circle.radius = 50.0f;
  //   circle.center.x = 500.0f;
  //   circle.center.y = -500.0f - k * 1000;
  // }

  // float chrono = 0;

  // gf::Penetration p;

  // gf::Clock clock;

  // int score = 0;
  // gf::Font font("../data/RogueCMI/DejaVuSans.ttf");

  // gf::Text textChrono;
  // textChrono.setFont(font);
  // textChrono.setCharacterSize(30);
  // textChrono.setColor(gf::Color::Red);
  // textChrono.setString(std::to_string(chrono));
  // textChrono.setPosition({ 20.0f, 20.0f });
  // textChrono.setAnchor(gf::Anchor::TopLeft);

  // gf::Text textScore;
  // textScore.setFont(font);
  // textScore.setCharacterSize(30);
  // textScore.setColor(gf::Color::Red);
  // textScore.setString("Score = " + std::to_string(score));
  // textScore.setPosition({ 20.0f, 60.0f });
  // textScore.setAnchor(gf::Anchor::TopLeft);

  // renderer.clear(gf::Color::White);

  // while (window.isOpen())
  // {
  //   gf::Event event;

  //   while (window.pollEvent(event))
  //   {
  //     switch (event.type)
  //     {
  //     case gf::EventType::Closed:
  //       window.close();
  //       break;

  //     case gf::EventType::KeyPressed:
  //       switch (event.key.scancode)
  //       {
  //       case gf::Scancode::Up:
  //         velocity.y = velocity.y - Speed;
  //         break;

  //       case gf::Scancode::Down:
  //         velocity.y = velocity.y + Speed;
  //         break;

  //       case gf::Scancode::Left:
  //         velocity.x = velocity.x - Speed;
  //         break;

  //       case gf::Scancode::Right:
  //         velocity.x = velocity.x + Speed;
  //         break;

  //       case gf::Scancode::Escape:
  //         window.close();
  //         break;

  //       default:
  //         break;
  //       }
  //       break;

  //     case gf::EventType::KeyReleased:
  //       switch (event.key.scancode)
  //       {
  //       case gf::Scancode::Up:
  //         velocity.y = velocity.y + Speed;
  //         break;

  //       case gf::Scancode::Down:
  //         velocity.y = velocity.y - Speed;
  //         break;

  //       case gf::Scancode::Left:
  //         velocity.x = velocity.x + Speed;
  //         break;

  //       case gf::Scancode::Right:
  //         velocity.x = velocity.x - Speed;
  //         break;

  //       default:
  //         break;
  //       }
  //       break;

  //     default:
  //       break;
  //     }
  //   }

  //   if (chrono > 30)
  //   {
  //     gf::Text textFin;
  //     textFin.setFont(font);
  //     textFin.setCharacterSize(60);
  //     textFin.setColor(gf::Color::Red);
  //     textFin.setString("C'est fini !!!");
  //     textFin.setPosition({ 1000.0f, 450.0f });
  //     textFin.setAnchor(gf::Anchor::Center);

  //     textScore.setCharacterSize(60);
  //     textScore.setString("Votre score = " + std::to_string(score));
  //     textScore.setPosition({ 1000.0f, 550.0f });
  //     textScore.setAnchor(gf::Anchor::Center);

  //     renderer.clear();
  //     renderer.draw(textFin);
  //     renderer.draw(textScore);
  //     renderer.display();

  //     continue;
  //   }

  //   gf::Time time = clock.restart();
  //   float dt = time.asSeconds();
  //   chrono += dt;

  //   if (velocity != gf::Vector2f(0, 0))
  //   {
  //     animatedSprite.update(time);
  //   }

  //   gf::Vector2f vel(0.0f, 500.0f);
  //   for (size_t i = 0; i < 5; i++)
  //   {
  //     balls[i].center = balls[i].center + vel * dt;
  //     if (balls[i].center.y > 1000)
  //     {
  //       balls[i].center.x = static_cast <float> (rand() % 2000);
  //       balls[i].center.y = -500;
  //     }
  //     if (gf::collides(balls[i], itBox, p))
  //     {
  //       balls[i].center.x = static_cast <float> (rand() % 2000);
  //       balls[i].center.y = -500;
  //       score++;
  //     }
  //   }

  //   animatedSprite.move(velocity * dt);
  //   itBox.min = animatedSprite.getPosition() - animatedSprite.getOrigin();
  //   itBox.max = animatedSprite.getPosition() + animatedSprite.getOrigin();

  //   textScore.setString("Score = " + std::to_string(score));
  //   textChrono.setString(std::to_string(chrono));

  //   renderer.clear();
  //   renderer.draw(animatedSprite);

  //   for (size_t i = 0; i < 5; i++)
  //   {
  //     gf::CircleShape shape0(balls[i]);
  //     shape0.setColor(gf::Color::Red);
  //     shape0.setOutlineColor(gf::Color::Red);
  //     shape0.setOutlineThickness(0.05f);
  //     renderer.draw(shape0);
  //   }
  //   renderer.draw(textChrono);
  //   renderer.draw(textScore);
  //   renderer.display();
  // }

//   return 0;
// }



// int main() {
//   rCMI::RogueCMI rogueCMI;
//   rogueCMI.run();
//   return 0;
// }

