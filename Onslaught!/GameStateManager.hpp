#pragma once

//　ゲームの状態を管理/変更するためのクラス
class GameStateManager {
public:
    enum State {
        Title,
        Playing,
        Pause,
        GameOver,
        Victory,
        Exit
    };

    static GameStateManager& getInstance();
    void setState(State newState); 
    GameStateManager::State getState() const;

private:
    State currentState = State::Title;
};