#pragma once
#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <stratega/ForwardModel/ActionAssignment.h>
#include <stratega/GUI/AssetCache.h>
#include <stratega/GUI/EntityRenderer.h>
#include <stratega/GUI/GameRenderer.h>
#include <stratega/GUI/TileMap.h>
#include <stratega/GUI/Widgets/ActionsController.h>
#include <stratega/GUI/Widgets/FogOfWarController.h>

namespace SGA {
class TBSGameRenderer: public GameRenderer {
  public:
   TBSGameRenderer(SGA::Vector2f& resolution);

   void init(const GameState& initialState, const GameConfig& gameConfig) override;
   void update(const GameState& state) override;
   void render() override;
   void setPlayerPointOfView(int playerID) override;
   ActionAssignment getPlayerActions() override;

   bool isActionAvailable() const;
   bool isGameEndRequested() override;

   void closeWindow() override { window.close(); }

  private:
   void updateFow();
   void handleInput();
   void renderLayers();

   // Event Handling
   void mouseScrolled(const sf::Event& event);
   void mouseButtonReleased(const sf::Event& event);
   void mouseButtonPressed(const sf::Event& event);
   void mouseMoved(const sf::Event& event);

   // ImGUI
   void createHUD();
   void createWindowInfo();
   void createWindowUnits();
   void createWindowActions();
   void createWindowPlayerParameters();
   void createWindowFogOfWar();
   void createEntityInformation();
   void createActionBar();

   const GameConfig* config;
   GameState state;
   GameState fowState;
   std::optional< Action > selectedAction;
   bool endGameRequested = false;

   sf::RenderWindow window;
   sf::Clock deltaClock;
   int pointOfViewPlayerID;
   AssetCache assetCache;
   Widgets::FogOfWarSettings fowSettings;
   Widgets::ActionsSettings actionsSettings;
   TileMap tileMap;
   EntityRenderer entityRenderer;
   // Variables for input handling
   float zoomValue;
   bool dragging;
   sf::Vector2f oldMousePosition;
};
}  // namespace SGA
