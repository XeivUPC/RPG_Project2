#pragma once

#include "Vector2.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <nlohmann/json.hpp>
#include <functional>

using namespace std;
using json = nlohmann::json;

// Tipos de datos para señales
using SignalData = variant<monostate, string, float, Vector2>; // Vector2 debes implementarlo
enum class SignalType { Empty, String, Number, Vector2 };

struct Signal {
    string name;
    SignalType type;
    SignalData data;
};

struct Portrait {
    string id;
    string name;
};

struct Condition {
    string variable;
    variant<bool, float> value;
    string comparison; // "==", ">", "<", etc.
    string next_node;
};

struct DialogueChoice {
    string text;
    string next_node;
};



struct DialogueCharacter {
    string id;
    string name;
    vector<Portrait> portraits;
};

struct DialogueNode {
    string id;
    string text;
    DialogueCharacter character;
    string portraitId;
    vector<DialogueChoice> choices;
    vector<Condition> conditions;
    vector<Signal> signals;
    string next_node;
    bool completed = false;
};

class DialogueSystem {


public:
    DialogueSystem();
    ~DialogueSystem();

    void LoadDialogueWorkspace(const string& path);
    void LoadDialogueFromJSON(const string& path); // Nueva función
    void StartDialogue();
    void Update();
    void ProcessInput(int choice);
    void EndDialogue();
    void ResetDialogue();

    const DialogueNode& GetCurrentDialogue();

    // Añadir una variable al game_state
    void AddGameStateVariable(const string& variable_name, const variant<bool, float>& value);
    // Eliminar una variable del game_state
    void RemoveGameStateVariable(const string& variable_name);

    // Getters
    bool IsDialogueActive() const;
    const vector<Signal>& GetActiveSignals() const;

public:
    vector<function<void(Signal*)>> onSignalCall;

    vector<function<void()>> onDialogStart;
    vector<function<void()>> onDialogEnd;
    vector<function<void()>> onDialogNodeChange;

private:
    void ProcessSignals();
    SignalType DetermineSignalType(const SignalData& data);
    bool CheckCondition(const Condition& cond);

private:
    map<string, DialogueCharacter> characters;


    DialogueNode root_node;
    map<string, DialogueNode> nodes;
    string current_node;
    string previous_node; // Para detectar cambios
    vector<Signal> active_signals;
    bool dialogue_active = false;

    // Variables de estado del juego
    map<string, variant<bool, float>> game_state;
protected:
    void TriggerCallbacks(vector<function<void()>>& callbacks);
    void TriggerCallbacks(vector<function<void(Signal*)>>& callbacks, Signal* _value);

protected:

};
